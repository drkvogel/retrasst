#ifndef SNAPSHOTUPDATETASKQRERUNH
#define SNAPSHOTUPDATETASKQRERUNH

#include <boost/shared_ptr.hpp>
#include "DBTransactionRerun.h"
#include "DBUpdateTaskWithCallback.h"
#include "MeetingPlace.h"
#include "Require.h"
#include "SnapshotUpdateTask.h"
#include <string>
#include <SysUtils.hpp>

namespace valc
{

class SnapshotUpdateTaskQRerun;

void leaveMeetingPlace( SnapshotUpdateTaskQRerun* s );

class SnapshotUpdateTaskQRerun : public SnapshotUpdateTask
{
public:
    friend void leaveMeetingPlace( SnapshotUpdateTaskQRerun* s );

    typedef DBUpdateTaskWithCallback<SnapshotUpdateTaskQRerun, DBTransactionRerun> DBUpdateTaskRerun;

    SnapshotUpdateTaskQRerun( int worklistID, const std::string& sampleRunID, const std::string& sampleDescriptor, int user )
        :
        m_worklistID( worklistID ),
        m_dbUpdateCallbackEvent( 0 ),
        m_sampleRunID( sampleRunID ),
        m_sampleDescriptor( sampleDescriptor ),
        m_user( user )
    {
    }

    ~SnapshotUpdateTaskQRerun()
    {
        CloseHandle( m_dbUpdateCallbackEvent );
    }

    void notify( DBTransactionRerun& dbt )  volatile
    {
        if ( dbt.success )
            m_newWorklistID = dbt.newWorklistID;

        log->logFormatted( "notifyDBTransactionCompleted: %s", dbt.success ? "OK" : "FAILED" );

        SetEvent( m_dbUpdateCallbackEvent );
    }

protected:
    void updateDatabase() 
    {
        m_dbUpdateCallbackEvent = CreateEvent( NULL, false, false, NULL );

        require( m_dbUpdateCallbackEvent );

        DBTransactionRerun dbt;
        dbt.worklistID = m_worklistID;
        dbt.userID = m_user;
        dbt.sampleRunID = snapshot.getDatabaseIDForSampleRun(m_sampleRunID);

        m_meetingPlace = new MeetingPlace< SnapshotUpdateTaskQRerun, DBUpdateTaskRerun, int, DBTransactionRerun >();

        m_meetingPlace->arrive( this );

        boost::shared_ptr<void> onBlockExit( this, leaveMeetingPlace );

        DBUpdateTaskRerun* task = new DBUpdateTaskRerun( m_meetingPlace, dbt );
        
        dbTransactionHandler->queue( task );

        if ( WAIT_OBJECT_0 != WaitForSingleObject( m_dbUpdateCallbackEvent, 5000 ) )
        {
            throw Exception( L"Abandoned waiting for database transaction" );
        }
        log->log( "Done updating database" );
    }

    void updateSnapshot() 
    {
        snapshot.closeOff( m_sampleRunID );
        snapshot.insertRerun( m_worklistID, m_newWorklistID );
        snapshot.appendToQueue( m_sampleDescriptor );
    }

    void notifyObserver() 
    {
        snapshotObserver->notifyNewWorklistEntry    ( snapshot.getWorklistEntry( m_newWorklistID ) );
        snapshotObserver->notifyWorklistEntryChanged( snapshot.getWorklistEntry( m_worklistID ) );
        snapshotObserver->notifySampleRunClosedOff  ( m_sampleRunID );
        snapshotObserver->notifySampleAddedToQueue  ( m_sampleDescriptor );
    }

private:
    volatile int         m_newWorklistID;
    const    int         m_worklistID;
    const    int         m_user;
    HANDLE               m_dbUpdateCallbackEvent;
    const    std::string m_sampleRunID;
    const    std::string m_sampleDescriptor;
    MeetingPlace< SnapshotUpdateTaskQRerun, DBUpdateTaskRerun, int, DBTransactionRerun >* m_meetingPlace;
};

void leaveMeetingPlace( SnapshotUpdateTaskQRerun* s )
{
    int i = 0;
    s->m_meetingPlace->leave( s, i );
}

}

#endif

