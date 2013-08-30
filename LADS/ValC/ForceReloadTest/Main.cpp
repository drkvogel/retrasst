#include <vcl.h>
#include <windows.h>

#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>

#include <algorithm>
#include "API.h"
#include <boost/scoped_ptr.hpp>
#include <boost/variant.hpp>
#include "ConsoleWriter.h"
#include <cstdio>
#include <Dialogs.hpp>
#include <iostream>
#include "LoggingService.h"
#include <memory>
#include "Require.h"
#include <string>
#include "StringBuilder.h"
#include "StrUtil.h"

const int LOCAL_MACHINE_ID = -1019349;

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
    const valc::AnalysisActivitySnapshot* snapshot;

public:
    int localRuns, delimiters, worklistEntries;

    CountingVisitor(const valc::AnalysisActivitySnapshot* s) : localRuns(0), delimiters(0), worklistEntries(0), snapshot(s) {}

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

std::string describe( valc::Range< valc::WorklistEntryIterator >& worklistEntries )
{
    std::string desc;
    char buffer[1024];

    for ( valc::WorklistEntryIterator i = worklistEntries.first; i != worklistEntries.second; ++i )
    {
        const valc::WorklistEntry* wle = *i;
        std::sprintf( buffer, "|%-9d %-1c%-2d", wle->getID(), wle->getStatus(), countResults(wle) );
        desc.append( buffer );
    }

    return desc;
}


class PrintingVisitor : public boost::static_visitor<>
{
private:
    const valc::AnalysisActivitySnapshot* snapshot;
    paulst::LoggingService* log;

public:

    PrintingVisitor(const valc::AnalysisActivitySnapshot* s, paulst::LoggingService* l) : snapshot(s), log(l) {}

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

void print( const valc::QueuedSample& qs, const valc::AnalysisActivitySnapshot* snapshot, paulst::LoggingService* log )
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

int _tmain(int argc, _TCHAR* argv[])
{
    try
    {
        const std::string configString = paulst::loadContentsOf( configFile );
        valc::Properties connectionProperties;
        connectionProperties.setProperty( "ConnectionString", "dsn=paulst_brat_64;db=paulst_test" );
        connectionProperties.setProperty( "SessionReadLockSetting", "set lockmode session where readlock = nolock" );
        boost::scoped_ptr<valc::DBConnection> connection( valc::DBConnectionFactory::createConnection( connectionProperties ) );
        
	    std::auto_ptr<paulst::LoggingService> log( new paulst::LoggingService( new paulst::ConsoleWriter() ) );
        boost::scoped_ptr<valc::AnalysisActivitySnapshot> s( valc::SnapshotFactory::load( LOCAL_MACHINE_ID, 1234, connection.get(), log.get(),
            configString ) );

        CountingVisitor counts (s.get());
        PrintingVisitor printer(s.get(), log.get());

        log->log("LOCAL ENTRIES");

        for ( valc::LocalEntryIterator i = s->localBegin(); i != s->localEnd(); ++i )
        {
            boost::apply_visitor( counts , *i );
            boost::apply_visitor( printer, *i );
        }

        log->log("QUEUED");

        for ( valc::QueuedSampleIterator i = s->queueBegin(); i != s->queueEnd(); ++i )
        {
            print( *i, s.get(), log.get() );
        }

        log->log( std::string() << "local runs: " << counts.localRuns <<
            "; delimiters: " << counts.delimiters << 
            "; worklist entries: " << counts.worklistEntries << "\n" );

        // Any queued stuff?
        int numQueued = std::distance( s->queueBegin(), s->queueEnd() );
        
		log->log( std::string() << "queued samples: " << numQueued << "\n" );

        log->log( "Processing updates...\n" );

        ExceptionHandlingPolicy cachedExceptionMsg;

        s->runPendingDatabaseUpdates( connection.get(), &cachedExceptionMsg, true );

        if ( ! cachedExceptionMsg.empty() )
        {
            log->log( cachedExceptionMsg.get() );
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

	return 0;
}

