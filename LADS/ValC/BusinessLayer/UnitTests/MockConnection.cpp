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
    else if ( paulst::ifind( "valc_worklist", sql ) && ! paulst::ifind( "c_worklist_relation", sql ) )
    {
        // query for worklist entries
        str = m_worklist;
    }
    else if ( paulst::ifind( "buddy_result_float", sql ) )
    {
        // query for buddy_database entries, results, sample-runs
        str = m_buddyDB;
    }

    return new StringBackedCursor( str );
}

void MockConnection::executeStmt ( const std::string& sql )
{
}

void MockConnection::setBuddyDB( const std::string& buddyDB )
{
    m_buddyDB = buddyDB;
}

void MockConnection::setClusters( const std::string& clusters )
{
    m_clusters = clusters;
}

void MockConnection::setProjects( const std::string& projects )
{
    m_projects = projects;
}

void MockConnection::setWorklist( const std::string& worklist )
{
    m_worklist = worklist;
}

}

