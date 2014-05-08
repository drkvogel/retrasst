#include "API.h"
#include "BusinessLayer.h"
#include "ExceptionHandler.h"
#include "TaskWithCallback.h"
#include "ThreadPool.h"
#include "ValCDialogs.h"

namespace valcui
{

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

void BusinessLayer::forceReload()
{
    ForceReload func;
    stef::Submission<valc::SnapshotPtr, ForceReload> runAsync( func, m_threadPool );

    if ( showWaitDialog( runAsync.completionSignal(), "Force reload...", 30 * 1000 ) )
    {
        assertion( runAsync.completed(), "ForceReload should have completed, given that it didn't timeout." );

        if ( runAsync.error().size() )
        {
            throw Exception( runAsync.error().c_str() );
        }

        if ( runAsync.cancelled() )
        {
            throw Exception( "ForceReload got cancelled." );
        }

        m_snapshot = runAsync.returnValue();
    }
    else
    {
        throwTimeoutException( "Force Reload" );
    }
}

valc::SnapshotPtr& BusinessLayer::getSnapshot()
{
    return m_snapshot;
}

bool BusinessLayer::close()
{
    ReleaseBusinessLayerResources func;
    stef::Submission<int, ReleaseBusinessLayerResources> runAsync( func, m_threadPool );

    return showWaitDialog( runAsync.completionSignal(), "Closing...", 5000 ) &&
        runAsync.completed() && runAsync.error().empty();
}


}

