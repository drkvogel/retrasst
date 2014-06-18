#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "Config.h"
#include "Cursor.h"
#include "DBConnection.h"
#include "DBTransactionUtil.h"
#include "DBUpdateTaskSyncBuddyDatabaseAndSampleRun.h"
#include <memory>
#include "Require.h"

namespace valc
{

std::string DBUpdateTaskSyncBuddyDatabaseAndSampleRun::describeUpdate() const
{
    return "Associating entries in buddy_database with entries in sample_run for instances when no such "
        "association currently exists.  Creating entries in sample_run as required.";
}

void DBUpdateTaskSyncBuddyDatabaseAndSampleRun::updateDatabase()
{
    setAutoCommit( connection, false );

    boost::shared_ptr<void> restoreAutoCommitOnBlockExit( connection, boost::bind( setAutoCommit, _1, true ) );

    try
    {
        for ( const SampleRun& sr : m_newSampleRuns )
        {
                int newSampleRunID = insertNewSampleRunEntry( sr );
                snapshotUpdateHandle.updateSampleRunIDValue( sr.getID(), paulst::toString(newSampleRunID) );
        }

        for ( const BuddyRun& br : m_newBuddyRuns )
        {
            updateBuddyDatabaseEntry( br.buddySampleID, paulst::toInt( br.sampleRunID.value() ) );
        }

        commit( connection );
    }
    catch( const Exception& e )
    {
        rollback( connection );
        throw;
    }
    catch( ... )
    {
        rollback( connection );
        throw;
    }
}


int DBUpdateTaskSyncBuddyDatabaseAndSampleRun::insertNewSampleRunEntry( const SampleRun& sr )
{
    std::auto_ptr<paulstdb::Cursor> c( connection->executeQuery( config->get("SampleRunIDQueryString") ) );

    if ( c->endOfRecordSet() )
    {
        throw Exception( L"No rows obtained for: select sample_run_id.nextval" );
    }

    int newID = 0;
    c->read( 0, newID );
    c->close();

    std::string sql = paulst::format( config->get("SampleRunInsertSQL").c_str(), newID, sr.getSequencePosition(), 
        snapshotUpdateHandle.getGroupIDForSampleRun( sr.getID() ) );

    connection->executeStmt( sql );

    return newID;
}

void DBUpdateTaskSyncBuddyDatabaseAndSampleRun::updateBuddyDatabaseEntry( int buddySampleID, int sampleRunID  )
{
    const std::string sql = paulst::format( 
        config->get("BuddyDatabaseSampleRunIDUpdateSQL").c_str(), sampleRunID, buddySampleID );

    connection->executeStmt( sql );
}

}

