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
#include <windows.h>

namespace valc
{

LoadReferencedWorklistEntries::LoadReferencedWorklistEntries( 
        DBConnection*           con, 
        paulst::LoggingService* log, 
        WorklistEntries*        worklistEntries, 
        ResultIndex*            resultIndex,
        const std::string&      tempTableName,
        const std::string&      worklistSQL,
        const std::string&      worklistRelationSQL )
    :
    m_con( con ),
    m_log( log ),
    m_worklistEntries( worklistEntries ),
    m_resultIndex( resultIndex ),
    m_tempTableName( tempTableName ),
    m_worklistSQL( worklistSQL ),
    m_worklistRelationSQL( worklistRelationSQL )
{
}

std::string* createTempTable( DBConnection* con, paulst::LoggingService* log, const std::string& tableName )
{
    std::string stmt = 
        std::string("declare global temporary table ") + tableName + " as "
        "select record_no from valc_worklist where record_no is null "
        "on commit preserve rows with norecovery";

    con->executeStmt( stmt );

    log->log( std::string("Created temporary table '") + tableName + "'." );
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
        createTempTable( m_con, m_log, m_tempTableName );

        std::string* tempTable = new std::string( m_tempTableName );

        boost::shared_ptr<void> dropTableOnBlockExit( tempTable, boost::bind( dropTable, _1, m_con, m_log ) );

        std::for_each( m_idList.begin(), m_idList.end(), boost::bind( insertIntoTable, _1, tempTable, m_con, m_log ) );

        LoadWorklistEntries loadWorklistEntries( m_worklistEntries, m_con, m_log, m_resultIndex, m_worklistSQL, m_worklistRelationSQL );

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

