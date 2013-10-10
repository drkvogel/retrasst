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
    std::string str;

    if ( paulst::ifind( "c_project", sql ) )
    {
        str = MockConnectionFactory::projects;
    }
    else if ( paulst::ifind( "c_cluster_machine", sql ) )
    {
        str = MockConnectionFactory::clusters;
    }
    else if ( paulst::ifind( "LoadWorklistEntries", sql ) )// Assuming MockConfig
    {
        str = MockConnectionFactory::worklist;
    }
    else if ( paulst::ifind( "LoadBuddyDatabase", sql ) )// Assuming MockConfig
    {
        str = MockConnectionFactory::buddyDB;
    }
    else if ( paulst::ifind( "LoadNonLocalResults", sql ) )// Assuming MockConfig
    {
        str = MockConnectionFactory::nonLocalResults;
    }
    else if ( paulst::ifind( "c_test", sql ) )
    {
        // query for test names
        str = MockConnectionFactory::testNames;
    }
    else if ( paulst::ifind( "sample_run_id.nextval", sql ) )
    {
        str = "1,\n";
    }

    return new paulstdb::StringBackedCursor( str );
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

