#include "AsyncTaskForceReload.h"
#include "ModelEventConstants.h"
#include "Require.h"

namespace valcui
{

void ForceReload::operator()( valc::SnapshotPtr& sp )
{
    sp = valc::Load();
}

AsyncTaskForceReload::AsyncTaskForceReload( valc::SnapshotPtr& snapshot )
    :
    m_snapshot(snapshot)
{
}

std::string AsyncTaskForceReload::getDesc() const
{
    return "Force Reload";
}

HANDLE AsyncTaskForceReload::queue( stef::ThreadPool* tp )
{
    ForceReload func;
    m_submission.reset( new stef::Submission<valc::SnapshotPtr, ForceReload>( func, tp ) );

    return m_submission->completionSignal();
}

void AsyncTaskForceReload::callback( ModelEventListener* eventSink )
{
    require( m_submission );

    if ( succeeded( m_submission.get(), eventSink ) )
    {
        m_snapshot = m_submission->returnValue();
        eventSink->notify( MODEL_EVENT::FORCE_RELOAD );
    }
}

}

