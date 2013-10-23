#include <algorithm>
#include <boost/bind.hpp>
#include "MockConnection.h"
#include "MockConnectionFactory.h"
#include "StringBackedCursor.h"
#include "StrUtil.h"

namespace valc
{

std::vector< std::string> MockConnection::m_updateStmts;

MockConnection::MockConnection()
{
    m_updateStmts.clear();
}

void MockConnection::close()
{
}

paulstdb::Cursor* MockConnection::executeQuery( const std::string& sql )
{
    SerializedRecordset sr;

    if ( paulst::ifind( "c_project", sql ) )
    {
        sr = MockConnectionFactory::projects;
    }
    else if ( paulst::ifind( "c_cluster_machine", sql ) )
    {
        sr = MockConnectionFactory::clusters;
    }
    else if ( paulst::ifind( "LoadWorklistEntries", sql ) )// Assuming MockConfig
    {
        sr = MockConnectionFactory::worklist;
    }
    else if ( paulst::ifind( "LoadBuddyDatabase", sql ) )// Assuming MockConfig
    {
        sr = MockConnectionFactory::buddyDB;
    }
    else if ( paulst::ifind( "LoadNonLocalResults", sql ) )// Assuming MockConfig
    {
        sr = MockConnectionFactory::nonLocalResults;
    }
    else if ( paulst::ifind( "c_test", sql ) )
    {
        // query for test names
        sr = MockConnectionFactory::testNames;
    }
    else if ( paulst::ifind( "sample_run_id.nextval", sql ) )
    {
        sr = "1,\n";
    }
    else if ( paulst::ifind( "LoadRules", sql ) )
    {
        sr = MockConnectionFactory::rules;
    }
    else if ( paulst::ifind( "LoadRuleConfig", sql ) )
    {
        sr = MockConnectionFactory::ruleConfig;
    }

    return new paulstdb::StringBackedCursor( sr.data, sr.fieldParsingStrategy );
}

void MockConnection::executeStmt ( const std::string& sql )
{
    m_updateStmts.push_back( sql );
}

int MockConnection::totalNewResult2WorklistLinks()
{
    return std::count_if( m_updateStmts.begin(), m_updateStmts.end(), boost::bind( paulst::ifind, "set cbw_record_no", _1 ) );
}

int MockConnection::totalNewSampleRuns()
{
    return std::count_if( m_updateStmts.begin(), m_updateStmts.end(), boost::bind( paulst::ifind, "into sample_run", _1 ) );
}

int MockConnection::totalUpdatesForSampleRunIDOnBuddyDatabase()
{
    return std::count_if( m_updateStmts.begin(), m_updateStmts.end(), boost::bind( paulst::ifind, "set sample_run_id", _1 ) );
}

}

