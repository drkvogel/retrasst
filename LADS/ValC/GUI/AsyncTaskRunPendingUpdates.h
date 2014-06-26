#ifndef ASYNCTASKRUNPENDINGUPDATESH
#define ASYNCTASKRUNPENDINGUPDATESH

#include "API.h"
#include "AsyncTask.h"
#include <memory>
#include "TaskWithCallback.h"

namespace valcui
{

struct RunPendingUpdates
{
    valc::SnapshotPtr snapshot;

    void operator()(int& i);
};

class AsyncTaskRunPendingUpdates : public AsyncTask
{
public:
    AsyncTaskRunPendingUpdates( valc::SnapshotPtr snapshot );
    std::string getDesc() const;
    HANDLE      queue( stef::ThreadPool* tp );
    void        callback( ModelEventListener* eventSink );
private:
    RunPendingUpdates m_func;
    std::unique_ptr< stef::Submission<int, RunPendingUpdates> > m_taskSubmission;
};

}

#endif

