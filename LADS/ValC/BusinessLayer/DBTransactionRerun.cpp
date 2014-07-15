#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "Config.h"
#include "Cursor.h"
#include "DBConnection.h"
#include "DBTransactionRerun.h"
#include "DBTransactionResources.h"
#include "DBTransactionUtil.h"
#include "ExceptionUtil.h"
#include <memory>
#include "StrUtil.h"

namespace valc
{

DBTransactionRerun::DBTransactionRerun(
    int worklistID,
    int userID,
    IDToken sampleRunID,
    const DBTransactionResources* dbTransactionResources )
    :
    m_worklistID            ( worklistID ),
    m_userID                ( userID ),
    m_sampleRunID           ( sampleRunID ),
    m_dbTransactionResources( dbTransactionResources )
{
}


DBTransactionRerun::DBTransactionRerun( const DBTransactionRerun& other )
    :
    m_worklistID            ( other.m_worklistID ),
    m_userID                ( other.m_userID ),
    m_sampleRunID           ( other.m_sampleRunID ),
    m_dbTransactionResources( other.m_dbTransactionResources )
{
}

DBTransactionRerun& DBTransactionRerun::operator=( const DBTransactionRerun& other )
{
    m_worklistID            = other.m_worklistID;
    m_userID                = other.m_userID;
    m_sampleRunID           = other.m_sampleRunID;
    m_dbTransactionResources= other.m_dbTransactionResources;
    return *this;
}

void DBTransactionRerun::confirmOKToCloseSampleRun( paulstdb::DBConnection* c, const paulst::Config* config )
{
    const int intSampleRunID = paulst::toInt( m_sampleRunID.value() );
    std::auto_ptr<paulstdb::Cursor> cursor( c->executeQuery( paulst::format( config->get( "QueryForSampleRun" ).c_str(), intSampleRunID ) ) );

    require( *cursor );

    int sampleRunEntryFound = paulstdb::read<int>( *cursor, 0 );

    cursor->close();

    if ( ! sampleRunEntryFound )
    {
        paulst::exception( "Failed to find entry %d in buddy_sample_run", intSampleRunID );
    }
}

void DBTransactionRerun::reserveIDValueForNewWorklistEntry( 
    paulstdb::DBConnection* c, 
    const paulst::Config* config,
    int& newWorklistID )
{
    std::auto_ptr<paulstdb::Cursor> cursor( c->executeQuery( config->get( "SQLStmtNextWorklistID" ) ) );
    require( *cursor );
    newWorklistID = paulstdb::read<int>( *cursor, 0 );
    cursor->close();
}


void DBTransactionRerun::operator()( int& newWorklistID )
{
    paulstdb::DBConnection* c    = m_dbTransactionResources->getConnection();
    const paulst::Config* config = m_dbTransactionResources->getConfig();

    setAutoCommit( c, false );

    boost::shared_ptr<void> restoreAutoCommitOnBlockExit( c, boost::bind( setAutoCommit, _1, true ) );

    try
    {
        reserveIDValueForNewWorklistEntry( c, config, newWorklistID );
        confirmOKToCloseSampleRun( c, config );
        const int intSampleRunID = paulst::toInt( m_sampleRunID.value() );
        c->executeStmt( paulst::format( config->get( "SQLStmtCloseSampleRun"         ).c_str(), m_userID, intSampleRunID    ) );
        c->executeStmt( paulst::format( config->get( "SQLStmtInsertRerun"            ).c_str(), newWorklistID, m_worklistID ) );
        c->executeStmt( paulst::format( config->get( "SQLStmtInsertWorklistRelation" ).c_str(), newWorklistID, m_worklistID ) );
        commit( c );
    }
    catch( const Exception& e )
    {
        rollback( c );
        throw;
    }
    catch( ... )
    {
        rollback( c );
        throw;
    }
}

}

