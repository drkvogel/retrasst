#include <boost/shared_ptr.hpp>
#include "DBTransactionHandler.h"
#include "DBTransactionRerun.h"
#include "Require.h"
#include "SnapshotUpdateTaskQRerun.h"
#include <string>
#include <SysUtils.hpp>
#include "TaskWithCallback.h"


namespace valc
{
SnapshotUpdateTaskQRerun::SnapshotUpdateTaskQRerun( 
    int worklistID, 
    const IDToken& sampleRunID, 
    const std::string& sampleDescriptor, 
    const std::string& barcode, 
    int user,
    DBTransactionHandler* dbth )
    :
    m_worklistID        ( worklistID ),
    m_sampleRunID       ( sampleRunID ),
    m_sampleDescriptor  ( sampleDescriptor ),
    m_barcode           ( barcode ),
    m_user              ( user ),
    m_dbTransactionHandler( dbth )
{
}

void SnapshotUpdateTaskQRerun::updateDatabase() 
{
    DBTransactionRerun dbt( m_worklistID,
                            m_user,
                            m_sampleRunID,
                            m_dbTransactionHandler );

    stef::Submission< int, DBTransactionRerun > dbUpdate( dbt, m_dbTransactionHandler->workerThread );
    
    std::string errorMsg;

    do
    {
        if ( WAIT_OBJECT_0 != WaitForSingleObject( dbUpdate.completionSignal(), 5000 ) )
        {
            errorMsg = "Error (timeout?) waiting for database operations to complete.";
            break;
        }

        if ( dbUpdate.error().size() )
        {
            errorMsg = dbUpdate.error();
            break;
        }

        if ( dbUpdate.cancelled() )
        {
            errorMsg = "Database operation was cancelled.";
            break;
        }

        m_newWorklistID = dbUpdate.returnValue();
    }
    while( false );

    if ( ! errorMsg.empty() )
    {
        throw Exception( errorMsg.c_str() );
    }
}

void SnapshotUpdateTaskQRerun::updateSnapshot() 
{
    snapshot.closeOff( m_sampleRunID );
    snapshot.insertRerun( m_worklistID, m_newWorklistID );
    snapshot.appendToQueue( QueuedSample( m_sampleDescriptor, m_barcode ) );
}

void SnapshotUpdateTaskQRerun::notifyObserver() 
{
    snapshotObserver->notifyNewWorklistEntry    ( snapshot.getWorklistEntry( m_newWorklistID ) );
    snapshotObserver->notifyWorklistEntryChanged( snapshot.getWorklistEntry( m_worklistID ) );
    snapshotObserver->notifySampleRunClosedOff  ( m_sampleRunID );
    snapshotObserver->notifySampleAddedToQueue  ( m_sampleDescriptor );
}

}

