#include "API.h"
#include "AsyncTaskClose.h"
#include "ModelEventConstants.h"
#include "ModelEventListener.h"

namespace valcui
{

void ReleaseBusinessLayerResources::operator()(int& i )
{
    valc::DeleteApplicationContext();
}

std::string AsyncTaskClose::getDesc() const
{
    return "Deleting Application Context...";
}

HANDLE AsyncTaskClose::queue( stef::ThreadPool* tp )
{
    ReleaseBusinessLayerResources func;
    m_submission.reset( new stef::Submission<int, ReleaseBusinessLayerResources>( func, tp ) );
    return m_submission->completionSignal();
}

void AsyncTaskClose::callback( ModelEventListener* eventSink )
{
    // Here ignoring the possibility that
    //    a) the submitted task never got to run
    //    b) the submitted task failed
    // These possibilities are ignored because, at this stage of the game, 
    // the application is wanting to close and so the most important thing 
    // is to broadcast the APPLICATION_CONTEXT_DELETED event for thos components 
    // holding out for this event before allowing graceful shutdown to proceed.
    eventSink->notify( MODEL_EVENT::APPLICATION_CONTEXT_DELETED );
}

}


