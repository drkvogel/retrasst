#include <algorithm>
#include <boost/bind.hpp>
#include "MockConnection.h"
#include "StringBackedCursor.h"
#include "StrUtil.h"

namespace valc
{

MockConnection::MockConnection()
{
}

void MockConnection::close()
{
}

Cursor* MockConnection::executeQuery( const std::string& sql )
{
    std::string str;

    if ( paulst::ifind( "c_project", sql ) )
    {
        str = m_projects;
    }
    else if ( paulst::ifind( "c_cluster_machine", sql ) )
    {
        str = m_clusters;
    }
    else if ( paulst::ifind( "LoadWorklistEntries", sql ) )// Assuming MockConfig
    {
        str = m_worklist;
    }
    else if ( paulst::ifind( "LoadBuddyDatabase", sql ) )// Assuming MockConfig
    {
        str = m_buddyDB;
    }
    else if ( paulst::ifind( "LoadNonLocalResults", sql ) )// Assuming MockConfig
    {
        str = m_nonLocalResults;
    }
    else if ( paulst::ifind( "c_test", sql ) )
    {
        // query for test names
        str = m_testNames;
    }
    else if ( paulst::ifind( "sample_run_id.nextval", sql ) )
    {
        str = "1,\n";
    }

    return new StringBackedCursor( str );
}

void MockConnection::executeStmt ( const std::string& sql )
{
    m_updateStmts.push_back( sql );
}

void MockConnection::setBuddyDB( const std::string& buddyDB )
{
    m_buddyDB = buddyDB;
}

void MockConnection::setClusters( const std::string& clusters )
{
    m_clusters = clusters;
}

void MockConnection::setNonLocalResults( const std::string& nonLocalResults )
{
    m_nonLocalResults = nonLocalResults;
}

void MockConnection::setProjects( const std::string& projects )
{
    m_projects = projects;
}

void MockConnection::setTestNames( const std::string& testNames )
{
    m_testNames = testNames;
}

void MockConnection::setWorklist( const std::string& worklist )
{
    m_worklist = worklist;
}

int MockConnection::totalNewSampleRuns() const
{
    return std::count_if( m_updateStmts.begin(), m_updateStmts.end(), boost::bind( paulst::ifind, "into sample_run", _1 ) );
}

int MockConnection::totalUpdatesForSampleRunIDOnBuddyDatabase() const
{
    return std::count_if( m_updateStmts.begin(), m_updateStmts.end(), boost::bind( paulst::ifind, "set sample_run_id", _1 ) );
}

}

