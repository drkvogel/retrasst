#include "AsyncTaskRunPendingUpdates.h"
#include "ExceptionHandler.h"

namespace valcui
{

void RunPendingUpdates::operator()(int& i)
{
    assertion( snapshot, "Snapshot cannot be NULL" );
    snapshot->runPendingDatabaseUpdates( true );
}

AsyncTaskRunPendingUpdates::AsyncTaskRunPendingUpdates( valc::SnapshotPtr snapshot )
{
    m_func.snapshot = snapshot;
}

std::string AsyncTaskRunPendingUpdates::getDesc() const { return "Run Pending Database Updates"; }

HANDLE AsyncTaskRunPendingUpdates::queue( stef::ThreadPool* tp )
{
    m_taskSubmission.reset( new stef::Submission<int, RunPendingUpdates>( m_func, tp ) );

    return m_taskSubmission->completionSignal();
}

void AsyncTaskRunPendingUpdates::callback( ModelEventListener* eventSink )
{
    if ( succeeded( m_taskSubmission.get(), eventSink ) )
    {
        eventSink->notify( MODEL_EVENT::PROCESSED_PENDING_UPDATES );
    }
}

}

