#include "API.h"
#include <boost/shared_ptr.hpp>
#include "BusinessLayer.h"
#include <FMX.Dialogs.hpp>
#include <iostream>
#include "TaskWithCallback.h"
#include "ThreadPool.h"

namespace valcui
{

void closeHandle( HANDLE h )
{
    CloseHandle( h );
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

struct ForceReload
{
    void operator()( valc::SnapshotPtr& sp )
    {
        sp = valc::Load();
    }
};

struct InitialiseBusinessLayer
{
	int machineID;
	int userID;
	std::string config;
	paulst::LoggingService* log;
	valc::UserAdvisor* warningsListener;

    void operator()( int& i )
    {
        valc::InitialiseApplicationContext( machineID, userID, config, log, warningsListener );
    }
};

struct ReleaseBusinessLayerResources
{
    void operator()(int& i )
    {
        valc::DeleteApplicationContext();
    }
};

struct RunPendingUpdates
{
    valc::SnapshotPtr snapshot;

    void operator()(int& i)
    {
        assertion( snapshot, "Snapshot cannot be NULL" );
        snapshot->runPendingDatabaseUpdates( true );
    }
};

struct Rerun
{
    valc::SnapshotPtr snapshot;
    int worklistID;
    valc::IDToken sampleRunID;
    std::string sampleDescriptor;
    

    void operator()( HANDLE& h )
    {
        h = snapshot->queueForRerun( worklistID, sampleRunID, sampleDescriptor );
    }
};

BusinessLayer::BusinessLayer(
	int machineID,
	int userID,
	const std::string& config,
	paulst::LoggingService* log,
	valc::UserAdvisor* warningsListener
	)
	:
	m_threadPool( new stef::ThreadPool() ),
    m_warningsListener( warningsListener )
{

    InitialiseBusinessLayer init;
    init.machineID          = machineID;
    init.userID             = userID;
    init.config             = config;
    init.log                = log;
    init.warningsListener   = warningsListener;

    stef::Submission<int, InitialiseBusinessLayer> runAsync( init, m_threadPool );
}

BusinessLayer::~BusinessLayer()
{
	m_threadPool->shutdown( THREAD_POOL_SHUTDOWN_WAIT_MILLIS, true );
}

void BusinessLayer::borrowSnapshot( TThreadMethod callback )
{
    m_threadPool->addTask( new UIThreadCallback( callback ) );
}

void BusinessLayer::forceReload()
{
    ForceReload func;
    stef::Submission<valc::SnapshotPtr, ForceReload> runAsync( func, m_threadPool );

    waitForAsyncTask( runAsync, "Force Reload" );

    m_snapshot = runAsync.returnValue();
}

valc::SnapshotPtr& BusinessLayer::getSnapshot()
{
    return m_snapshot;
}

bool BusinessLayer::close()
{
    ReleaseBusinessLayerResources func;
    stef::Submission<int, ReleaseBusinessLayerResources> runAsync( func, m_threadPool );

    return 
        ( WAIT_OBJECT_0 == showWaitDialog( runAsync.completionSignal(), "Closing...", 5000 ) ) 
        &&
        runAsync.completed() 
        && 
        runAsync.error().empty();
}

void BusinessLayer::rerun(
    int worklistID, 
    const valc::IDToken& sampleRunID, 
    const std::string& sampleDescriptor,
    const std::string& barcode,
    const std::string& testName )
{
    Rerun func;
    func.worklistID = worklistID;
    func.sampleRunID = sampleRunID;
    func.sampleDescriptor = sampleDescriptor;
    func.snapshot = m_snapshot;

    stef::Submission<HANDLE, Rerun> runAsync( func, m_threadPool );

    waitForAsyncTask( runAsync, "Rerun", false );

    HANDLE h = runAsync.returnValue();

    boost::shared_ptr<void> onBlockExit( h, closeHandle );

    std::ostringstream msg;

    msg << "Queueing rerun of test '" << testName << "' for barcode '" << barcode << "' (Worklist ID:" << worklistID << ")";

    unsigned long waitResult = showWaitDialog( h, msg.str(), 20 * 1000 );

    if ( waitResult != WAIT_OBJECT_0 )
    {
        throwWaitException( waitResult, "Rerun" );
    }
}

void BusinessLayer::runPendingUpdates()
{
    RunPendingUpdates func;
    func.snapshot = m_snapshot;
    stef::Submission<int, RunPendingUpdates> runAsync( func, m_threadPool );
    waitForAsyncTask( runAsync, "Run Pending Updates" );
}

}

