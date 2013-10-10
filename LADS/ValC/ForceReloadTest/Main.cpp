#include <vcl.h>
#include <windows.h>

#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>

#include "AcquireCriticalSection.h"
#include <algorithm>
#include "API.h"
#include <boost/scoped_ptr.hpp>
#include <boost/variant.hpp>
#include "ConsoleWriter.h"
#include "CritSec.h"
#include <cstdio>
#include <cstdlib>
#include "DBConnection.h"
#include <Dialogs.hpp>
#include <iostream>
#include "LoggingService.h"
#include <memory>
#include "Require.h"
#include <string>
#include "StringBuilder.h"
#include "StrUtil.h"
#include <vector>

//const char* connectionString = "dsn=paulst_brat_64;db=paulst_test2";

const int USER_ID = 9789;

const std::string configFile = "J:\\cvs\\LADS\\ValC\\BusinessLayer\\config.txt";

static bool dbErrorCallback( const std::string object, const int instance,
    const int ecount, const int ecode, const std::string error_txt )
{
    std::cerr << error_txt;
    return true ;
}

class CountingVisitor : public boost::static_visitor<>
{
private:
    const valc::SnapshotPtr snapshot;

public:
    int localRuns, delimiters, worklistEntries;

    CountingVisitor(valc::SnapshotPtr s) : localRuns(0), delimiters(0), worklistEntries(0), snapshot(s) {}

    void operator()( const valc::LocalRun& r ) 
	{
		using namespace valc;

		localRuns++;

		Range<WorklistEntryIterator> iteratorRange =
			snapshot->getWorklistEntries( r.getSampleDescriptor() );

		worklistEntries +=
			std::distance( iteratorRange.first, iteratorRange.second );
    }

    void operator()( const valc::BatchDelimiter& b ) 
    {
        delimiters++;
    }
};

int countResults( const valc::WorklistEntry* wle )
{
    valc::Range< valc::TestResultIterator > results = wle->getTestResults();
    return std::distance( results.first, results.second );
}

char resultActionFlag( const valc::WorklistEntry* wle )
{
    char returnValue = ' ';
    valc::Range< valc::TestResultIterator > results = wle->getTestResults();
    int numResults = std::distance( results.first, results.second );
    if ( numResults == 0 )
    {
        returnValue = ' ';
    }
    else if ( numResults == 1 )
    {
        const valc::TestResult* result = *(results.first);
        returnValue = result->getActionFlag();
    }
    else
    {
        returnValue = '*';
    }

    return returnValue;
}

std::string describe( valc::Range< valc::WorklistEntryIterator >& worklistEntries )
{
    std::string desc;
    char buffer[1024];

    for ( valc::WorklistEntryIterator i = worklistEntries.first; i != worklistEntries.second; ++i )
    {
        const valc::WorklistEntry* wle = *i;
        std::sprintf( buffer, "|%-8d %-1c%-1d%-1c", wle->getID(), wle->getStatus(), countResults(wle), resultActionFlag(wle) );
        desc.append( buffer );
    }

    return desc;
}


class PrintingVisitor : public boost::static_visitor<>
{
private:
    const valc::SnapshotPtr snapshot;
    paulst::LoggingService* log;

public:

    PrintingVisitor(valc::SnapshotPtr s, paulst::LoggingService* l) : snapshot(s), log(l) {}

    void operator()( const valc::LocalRun& r ) 
	{
		using namespace valc;

		Range<WorklistEntryIterator> iteratorRange = snapshot->getWorklistEntries( r.getSampleDescriptor() );

        const std::string worklistEntries = describe( iteratorRange );

		std::string barcode;
		TDateTime timeStamp;

		if ( iteratorRange.first != iteratorRange.second )
        {
			const WorklistEntry* wle = *(iteratorRange.first);
			barcode = wle->getBarcode();
			timeStamp = wle->getTimeStamp();
		}
		else
		{
			BuddyDatabaseEntries bdes = snapshot->listBuddyDatabaseEntriesFor( r.getRunID() );
			require( bdes.size() );
			barcode = bdes.at(0).barcode;
			timeStamp = bdes.at(0).date_analysed;
		}

		UnicodeString timeStringW = timeStamp;
		std::string timeStringA = paulst::stdstring( timeStringW );
        char buffer[1024];
		std::sprintf(buffer, "%-13s %-20s %s", barcode.c_str(), timeStringA.c_str(), worklistEntries.c_str() );
        log->log( std::string(buffer) );
    }

    void operator()( const valc::BatchDelimiter& b ) 
    {
    }
};


class ExceptionHandlingPolicy : public valc::DBUpdateExceptionHandlingPolicy
{
public:
    bool empty() const { return m_msg.empty(); }
    std::string get() const { return m_msg; }
    void handleException( const std::string& msg ) { m_msg = msg; }
private:
    std::string m_msg;
};

void print( const valc::QueuedSample& qs, valc::SnapshotPtr snapshot, paulst::LoggingService* log )
{
    using namespace valc;

    Range<WorklistEntryIterator> iteratorRange = snapshot->getWorklistEntries( qs.getSampleDescriptor() );

    require( iteratorRange.first != iteratorRange.second );

    const WorklistEntry* wle       = *(iteratorRange.first);
    const std::string    barcode   = wle->getBarcode();
    const TDateTime      timeStamp = wle->getTimeStamp();
    const std::string    worklistEntries = describe( iteratorRange );
    
    UnicodeString timeStringW = timeStamp;
    std::string timeStringA = paulst::stdstring( timeStringW );
    char buffer[1024];
    std::sprintf(buffer, "%-13s %-20s %s", barcode.c_str(), timeStringA.c_str(), worklistEntries.c_str() );
    log->log( std::string(buffer) );
 
}

class QueuedWarnings : public valc::UserAdvisor
{
public:
    typedef std::vector< std::string > Messages;
    typedef Messages::const_iterator const_iterator;

    QueuedWarnings() 
    {
    }

    void advise( const std::string& warning )
    {
        paulst::AcquireCriticalSection a(m_critSec);

        {
            m_messages.push_back( warning );
        }
    }

    const_iterator begin() const { return m_messages.begin(); }
    const_iterator end()   const { return m_messages.end()  ; }

private:
    paulst::CritSec m_critSec;
    Messages m_messages;
};

void throwUsageException()
{
    throw Exception(
        L"Usage\n"
        L"\n"
        L"Must supply local machine ID as an argument\n\n"
        );
}

int _tmain(int argc, _TCHAR* argv[])
{
    try
    {
        if ( argc < 2 )
        {
            throwUsageException();
        }

        int localMachineID = std::atoi( argv[1] );

		const std::string configString = paulst::loadContentsOf( configFile );

	    std::auto_ptr<paulst::LoggingService> log( new paulst::LoggingService( new paulst::ConsoleWriter() ) );

        log->log( configString );

        valc::InitialiseApplicationContext( localMachineID, USER_ID, configString, log.get() );

        QueuedWarnings warnings;

        valc::SnapshotPtr s = valc::Load( &warnings );

        CountingVisitor counts (s);
        PrintingVisitor printer(s, log.get());

        log->log("LOCAL ENTRIES");

        for ( valc::LocalEntryIterator i = s->localBegin(); i != s->localEnd(); ++i )
        {
            boost::apply_visitor( counts , *i );
            boost::apply_visitor( printer, *i );
        }

        log->log("QUEUED");

        for ( valc::QueuedSampleIterator i = s->queueBegin(); i != s->queueEnd(); ++i )
        {
            print( *i, s, log.get() );
        }

        log->log( std::string() << "local runs: " << counts.localRuns <<
            "; delimiters: " << counts.delimiters << 
            "; worklist entries: " << counts.worklistEntries << "\n" );

        // Any queued stuff?
        int numQueued = std::distance( s->queueBegin(), s->queueEnd() );
        
		log->log( std::string() << "queued samples: " << numQueued << "\n" );

        /*
        log->log( "Processing updates...\n" );

        ExceptionHandlingPolicy cachedExceptionMsg;

        s->runPendingDatabaseUpdates( connection.get(), &cachedExceptionMsg, true );

        if ( ! cachedExceptionMsg.empty() )
        {
            log->log( cachedExceptionMsg.get() );
        }
        */

        for ( QueuedWarnings::const_iterator i = warnings.begin(); i != warnings.end(); ++i )
        {
            std::string warningMsg = std::string("WARNING: ") + *i;
            log->log( warningMsg );
        }

        log->log( "All done.\n" );
    }
    catch( const Exception& e )
    {
        std::wcerr << e.Message.c_str();
    }
    catch( const std::string& errorMsg )
    {
        std::cerr << errorMsg;
    }
    catch( ... )
    {
        std::cerr << "Unknown exception.\n";
    }

    valc::DeleteApplicationContext();
	return 0;
}

