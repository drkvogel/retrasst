#include <algorithm>
#include "API.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/functional.hpp>
#include <boost/shared_ptr.hpp>
#include <cctype>
#include "IntList.h"
#include "LoadReferencedWorklistEntries.h"
#include "LoadWorklistEntries.h"
#include "LoggingService.h"
#include <memory>
#include "Require.h"
#include "StringBuilder.h"
#include "Trace.h"
#include "WorklistEntries.h"

namespace valc
{

LoadReferencedWorklistEntries::LoadReferencedWorklistEntries( const ClusterIDs* clusterIDs, int localMachineID, DBConnection* con, paulst::LoggingService* log, 
    WorklistEntries* worklistEntries, ResultIndex* resultIndex )
    :
    m_localMachineID( localMachineID ),
    m_con( con ),
    m_log( log ),
    m_worklistEntries( worklistEntries ),
    m_resultIndex( resultIndex ),
    m_clusterIDs( clusterIDs )
{
}

bool isAlphaNumeric( const char& c )
{
    return std::isalnum(c);
}

std::string* generateTemporaryTableName()
{
    char buf[200];
    DWORD charCount = 200;

    require( GetComputerName( buf, &charCount ) );

    require( ( charCount > 1 ) && ( charCount < 200 ) );

    char* end = std::remove_if( buf, buf + charCount, boost::not1(isAlphaNumeric) );
    
    require( ( end > buf ) && ( end <= ( buf + charCount ) ) );

    *end = '\0';

    std::string tableName = std::string("tmp") << buf;

    return new std::string(tableName);
}

std::string* createTempTable( DBConnection* con, paulst::LoggingService* log )
{
    std::auto_ptr<std::string> tableName( generateTemporaryTableName() );

    std::string stmt = 
        std::string("declare global temporary table ") + (*tableName) + " as "
        "select record_no from valc_worklist where record_no is null "
        "on commit preserve rows with norecovery";

    con->executeStmt( stmt );

    log->log( std::string("Created temporary table '") + (*tableName) + "'." );

    return tableName.release();
}

void dropTable( std::string* tableName, DBConnection* con, paulst::LoggingService* log )
{
    std::auto_ptr<std::string> tName(tableName);

    std::string stmt = std::string("drop table ") + (*tName);

    con->executeStmt( stmt );

    log->log( std::string("Dropped table '") << (*tName) << "'." );
}

void insertIntoTable( const int& id, const std::string* tableName, DBConnection* con, paulst::LoggingService* log )
{
    std::string stmt = std::string("insert into ") << (*tableName) << " values( " << id << " )";
    
    con->executeStmt( stmt );
}

void LoadReferencedWorklistEntries::execute()
{
    const int maxIterations = 3;

    for ( int iteration = 0; findReferencedButNotLoaded() && ( iteration < maxIterations ); ++iteration )
    {
        std::string* tempTable = createTempTable( m_con, m_log );

        boost::shared_ptr<void> dropTableOnBlockExit( tempTable, boost::bind( dropTable, _1, m_con, m_log ) );

        std::for_each( m_idList.begin(), m_idList.end(), boost::bind( insertIntoTable, _1, tempTable, m_con, m_log ) );

        const std::string queryWorklist = 
            LoadWorklistEntries::SQL_WORKLIST_SELECT + 
            " FROM valc_worklist wl, " + (*tempTable) + " tt " +
            " WHERE wl.record_no = tt.record_no " + 
            LoadWorklistEntries::SQL_ORDER_BY;

        const std::string queryWorklistRelation = 
            LoadWorklistEntries::SQL_WORKLIST_RELATION_SELECT + 
            " FROM c_worklist_relation wr, " + (*tempTable) + " wl " +
            " WHERE wl.record_no in ( wr.rerun_id, wr.parent_id ) " +
            LoadWorklistEntries::SQL_ORDER_BY;

        /*
        The above queries both reference a session temporary table.  Results from each are processed IN PARALLEL.
        A possible problem is that one set of results is not obtained, with the complaint that the session 
        temporary table is not visible or does not exist. A cause of such a problem is that, behind the scenes, the ODBC
        driver has created a distinct connection/session.

        Ways of resolving this:
        1) Use SysWOW64/odbcad32 to Configure the User Data Source. On the Advanced tab, select 'Cursors' rather than 
            'Select Loops'.

        2) Alternatively, don't use a session temporary table. Use an ordinary table.
        */
        LoadWorklistEntries loadWorklistEntries( m_clusterIDs, m_worklistEntries, m_localMachineID, m_con, m_log, m_resultIndex, 
            queryWorklist, queryWorklistRelation );

        loadWorklistEntries.execute();
    }
}

void LoadReferencedWorklistEntries::execute( const WorklistEntry* wle )
{
    IntList idList = wle->getIDsOfRelatedEntries();

    BOOST_FOREACH( int id, idList )
    {
        if ( NULL == m_worklistEntries->get( id ) )
        {
            m_idList.push_back( id );
        }
    }
}

int LoadReferencedWorklistEntries::findReferencedButNotLoaded() 
{
    m_idList.resize(0);

    m_worklistEntries->forEach( *this );

    LOG( std::string( "Found " ) << m_idList.size() << " entries referenced but not (yet) loaded." );

    return m_idList.size();
}
 
}

