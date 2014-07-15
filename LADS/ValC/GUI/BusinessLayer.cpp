#include "AcquireCriticalSection.h"
#include "API.h"
#include "AsyncTask.h"
#include "AsyncTaskClose.h"
#include "AsyncTaskForceReload.h"
#include "AsyncTaskInitBusinessLayer.h"
#include "AsyncTaskLoadRule.h"
#include "AsyncTaskRerun.h"
#include "AsyncTaskRunPendingUpdates.h"
#include <boost/shared_ptr.hpp>
#include "BusinessLayer.h"
#include "FileTimeUtil.h"
#include <FMX.Dialogs.hpp>
#include <iostream>
#include "ThreadPool.h"
#include "ValCDialogs.h"

namespace valcui
{

VOID CALLBACK WaitCallback(
    PTP_CALLBACK_INSTANCE Instance,
    PVOID                   Context,
    PTP_WAIT                Wait,
    TP_WAIT_RESULT          WaitResult )
{
    BusinessLayer* bl = (BusinessLayer*)Context;
    bl->marshallCallback();
}

class UIThreadCallback : public stef::Task
{
public:
    UIThreadCallback( TThreadMethod callback )
        :
        m_callback(callback)
    {
    }
protected:
    void doStuff()
    {
        TThread::Synchronize( NULL, m_callback );
    }
private:
    TThreadMethod m_callback;
};

BusinessLayer::BusinessLayer(
	int machineID,
	int userID,
	const std::string& config,
	paulst::LoggingService* log,
	valc::UserAdvisor* warningsListener,
    ModelEventListener* eventSink
	)
	:
	m_threadPool( new stef::ThreadPool() ),
    m_warningsListener( warningsListener ),
    m_ptpWait( CreateThreadpoolWait( &WaitCallback, this, NULL ) ),
    m_currentlyExecutingTask( NULL ),
    m_currentlyExecutingTaskHandle( NULL ),
    m_eventSink( eventSink ),
    m_busyDialog( new TWaitDlg(NULL) ),
    m_initialisationTask( new AsyncTaskInitBusinessLayer( machineID, userID, config, log, warningsListener ) )
{
    require( m_ptpWait );
}

BusinessLayer::~BusinessLayer()
{
	m_threadPool->shutdown( THREAD_POOL_SHUTDOWN_WAIT_MILLIS, true );
    CloseThreadpoolWait( m_ptpWait );
}

void __fastcall BusinessLayer::asyncTaskCompletionCallback()
{
    paulst::AcquireCriticalSection a(m_critSec);

    try
    {
        require( m_currentlyExecutingTask );
        require( m_currentlyExecutingTaskHandle );
        m_currentlyExecutingTask->callback( m_eventSink );
        hideMessage();
        delete m_currentlyExecutingTask;
    }
    catch( const Exception& e )
    {
        showErrorMsg( AnsiString( e.Message.c_str() ).c_str() );
    }
    catch( ... )
    {
        showErrorMsg( "Unspecified exception in BusinessLayer::asyncTaskCompletionCallback." );
    }

    m_currentlyExecutingTask = NULL;
    m_currentlyExecutingTaskHandle = NULL;
}

void BusinessLayer::borrowSnapshot( TThreadMethod callback )
{
    m_threadPool->addTask( new UIThreadCallback( callback ) );
}

void BusinessLayer::execute( AsyncTask* t )
{
    paulst::AcquireCriticalSection a(m_critSec);

    if (  m_currentlyExecutingTask ) 
    {
        showWarningMsg( "Cannot start requested task. Already busy with a different task." );
    }
    else
    {
        m_currentlyExecutingTask = t;

        std::string error, taskDesc;

        try
        {
            taskDesc = t->getDesc();

            m_currentlyExecutingTaskHandle = t->queue( m_threadPool );
        }
        catch( const Exception& e )
        {
            error = AnsiString( e.Message.c_str() ).c_str();
        }
        catch( ... )
        {
            error = "Unspecified exception while attempting to queue requested task.";
        }

        if ( error.empty() )
        {
            FILETIME timeout;
            SecureZeroMemory( &timeout, sizeof(timeout) );
            paulst::InitFileTimeWithMs( &timeout, ASYNC_TASK_TIMEOUT_MILLIS );

            SetThreadpoolWait( m_ptpWait, m_currentlyExecutingTaskHandle, &timeout );

            showMessage( taskDesc );
        }
        else
        {
            m_currentlyExecutingTask = NULL;
            m_currentlyExecutingTaskHandle = NULL;
            showErrorMsg( error );
        }
    }
}

void BusinessLayer::forceReload()
{
    execute( new AsyncTaskForceReload(m_snapshot) );
}

valc::SnapshotPtr& BusinessLayer::getSnapshot()
{
    return m_snapshot;
}

void BusinessLayer::close()
{
    execute( new AsyncTaskClose() );
}

void BusinessLayer::hideMessage()
{
    m_busyDialog->Hide();
}

void BusinessLayer::init()
{
    require( m_initialisationTask );
    AsyncTask* t = m_initialisationTask;
    m_initialisationTask = NULL;
    execute( t );
}

void BusinessLayer::loadRule( int test, int machine, int project )
{
    execute( new AsyncTaskLoadRule( test, machine, project ) );
}

void BusinessLayer::marshallCallback()
{
    TThread::Synchronize( NULL, asyncTaskCompletionCallback );
}

void BusinessLayer::rerun(
    int worklistID, 
    const valc::IDToken& sampleRunID, 
    const std::string& sampleDescriptor,
    const std::string& barcode,
    const std::string& testName )
{
    execute( new AsyncTaskRerun( m_snapshot, worklistID, sampleRunID, sampleDescriptor, barcode, testName, MAX_WAIT_MILLIS ) );
}

void BusinessLayer::runPendingUpdates()
{
    execute( new AsyncTaskRunPendingUpdates( m_snapshot ) );
}

void BusinessLayer::showMessage( const std::string& msg )
{
    m_busyDialog->show( msg );
}

}

