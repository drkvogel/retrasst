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

    SnapshotUpdateTaskQRerun( int worklistID, const IDToken& sampleRunID, const std::string& sampleDescriptor, int user )
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

    void notify( DBTransactionRerun& dbt )  
    {
        if ( dbt.errorMsg.empty() )
        {
            m_newWorklistID = dbt.newWorklistID;
            log->log( "notifyDBTransactionCompleted OK" );
        }
        else
        {
            m_transactionError = dbt.errorMsg;
            log->logFormatted( "Database transaction failed: %s", m_transactionError.c_str() );
        }

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
        dbt.sampleRunID = m_sampleRunID;

        m_meetingPlace = new paulst::MeetingPlace< SnapshotUpdateTaskQRerun, DBUpdateTaskRerun, int, DBTransactionRerun >();

        m_meetingPlace->arrive( this );

        boost::shared_ptr<void> onBlockExit( this, leaveMeetingPlace );

        DBUpdateTaskRerun* task = new DBUpdateTaskRerun( m_meetingPlace, dbt );
        
        dbTransactionHandler->queue( task );

        if ( WAIT_OBJECT_0 != WaitForSingleObject( m_dbUpdateCallbackEvent, 5000 ) )
        {
            throw Exception( L"Abandoned waiting for database transaction" );
        }

        if ( ! m_transactionError.empty() )
        {
            throw Exception( 
                paulst::format( 
                    "Failed to update the database: %s",
                    m_transactionError.c_str() 
                    ).c_str()
                );
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
    const    IDToken     m_sampleRunID;
    const    std::string m_sampleDescriptor;
    paulst::MeetingPlace< SnapshotUpdateTaskQRerun, DBUpdateTaskRerun, int, DBTransactionRerun >* m_meetingPlace;
    std::string          m_transactionError;
};

void leaveMeetingPlace( SnapshotUpdateTaskQRerun* s )
{
    int i = 0;
    s->m_meetingPlace->leave( s, i );
}

}

#endif

