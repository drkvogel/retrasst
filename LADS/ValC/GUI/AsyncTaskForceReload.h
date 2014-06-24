#ifndef ASYNCTASKFORCERELOADH
#define ASYNCTASKFORCERELOADH

#include "API.h"
#include "AsyncTask.h"
#include "TaskWithCallback.h"

namespace valcui
{

struct ForceReload
{
    void operator()( valc::SnapshotPtr& sp );
};

class AsyncTaskForceReload : public AsyncTask
{
public:
    AsyncTaskForceReload( valc::SnapshotPtr& snapshot );
    std::string getDesc() const;
    HANDLE queue( stef::ThreadPool* tp );
    void callback( ModelEventListener* eventSink );
private:
    valc::SnapshotPtr& m_snapshot;

    std::unique_ptr< stef::Submission<valc::SnapshotPtr, ForceReload> > m_submission;
};

}

#endif

