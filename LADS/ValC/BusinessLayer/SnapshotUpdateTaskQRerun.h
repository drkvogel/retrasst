#ifndef SNAPSHOTUPDATETASKQRERUNH
#define SNAPSHOTUPDATETASKQRERUNH

#include "DBTransactionRerun.h"
#include "DBUpdateTaskWithCallback.h"
#include "Require.h"
#include "SnapshotUpdateTask.h"
#include <string>
#include <SysUtils.hpp>

namespace valc
{

class SnapshotUpdateTaskQRerun : public SnapshotUpdateTask
{
public:

    typedef DBUpdateTaskWithCallback<SnapshotUpdateTaskQRerun, DBTransactionRerun> DBUpdateTaskRerun;

    SnapshotUpdateTaskQRerun( int worklistID, const std::string& sampleRunID )
        :
        m_worklistID( worklistID ),
        m_dbUpdateCallbackEvent( 0 )
    {
    }

    ~SnapshotUpdateTaskQRerun()
    {
        CloseHandle( m_dbUpdateCallbackEvent );
    }

    void notifyDBTransactionCompleted( DBTransactionRerun& dbt )  volatile
    {
        m_newWorklistID = dbt.newWorklistID;
        log->log( "notifyDBTransactionCompleted" );
        SetEvent( m_dbUpdateCallbackEvent );
    }

protected:
    void updateDatabase() 
    {
        m_dbUpdateCallbackEvent = CreateEvent( NULL, false, false, NULL );

        require( m_dbUpdateCallbackEvent );

        DBTransactionRerun dbt;
        dbt.worklistID = m_worklistID;

        DBUpdateTaskRerun* task = new DBUpdateTaskRerun( this, dbt );
        
        dbTransactionHandler->queue( task );

        if ( WAIT_OBJECT_0 != WaitForSingleObject( m_dbUpdateCallbackEvent, 5000 ) )
        {
            task->unregisterCallback();
            throw Exception( L"Abandoned waiting for database transaction" );
        }
        log->log( "Done updating database" );
    }

    void updateSnapshot() 
    {
        snapshot.insertRerun( m_worklistID, m_newWorklistID );
    }

    void notifyObserver() 
    {
        snapshotObserver->notifyNewWorklistEntry    ( snapshot.getWorklistEntry( m_newWorklistID ) );
        snapshotObserver->notifyWorklistEntryChanged( snapshot.getWorklistEntry( m_worklistID ) );
    }

private:
    volatile int m_newWorklistID;
    const    int m_worklistID;
    HANDLE       m_dbUpdateCallbackEvent;
};

}

#endif

