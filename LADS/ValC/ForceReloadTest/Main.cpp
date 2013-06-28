#include <vcl.h>
#include <windows.h>

#pragma hdrstop
#pragma argsused

#include <tchar.h>
#include <stdio.h>

#include "API.h"
#include <boost/scoped_ptr.hpp>
#include <boost/variant.hpp>
#include <Dialogs.hpp>
#include <iostream>
#include <memory>
#include "Require.h"
#include <string>
#include "StringBuilder.h"

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
        localRuns++;
        valc::SampleWorklistEntries sampleWorklistEntries = snapshot->getWorklistEntries(r.getSampleDescriptor());
        worklistEntries += sampleWorklistEntries.size();
    }

    void operator()( const valc::BatchDelimiter& b ) 
    {
        delimiters++;
    }
};

int _tmain(int argc, _TCHAR* argv[])
{
    try
    {
        valc::Properties connectionProperties;
        connectionProperties.setProperty( "ConnectionString", "dsn=paulst_test_on_brat;db=paulst_test" );
        connectionProperties.setProperty( "SessionReadLockSetting", "set lockmode session where readlock = nolock" );
        boost::scoped_ptr<valc::DBConnection> connection( valc::DBConnectionFactory::createConnection( connectionProperties ) );
        
        boost::scoped_ptr<valc::AnalysisActivitySnapshot> s( valc::SnapshotFactory::load( -1019349, 1234, connection.get() ) );
        require( s.get() );

        CountingVisitor counts(s.get());

        for ( valc::LocalEntryIterator i = s->localBegin(); i != s->localEnd(); ++i )
        {
            boost::apply_visitor( counts, *i );
        }

        std::cout << (
            std::string("local runs: ") << counts.localRuns <<
            "; delimiters: " << counts.delimiters << 
            "; worklist entries: " << counts.worklistEntries << "\n" );

        // Any queued stuff?
        int numQueued = std::distance( s->queueBegin(), s->queueEnd() );
        
        std::cout << ( std::string("queued samples: " ) << numQueued << "\n" );
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

    std::cout << "All done.\n";
	return 0;
}

