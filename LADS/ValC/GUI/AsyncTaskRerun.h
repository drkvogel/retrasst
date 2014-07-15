#ifndef ASYNCTASKRERUNH
#define ASYNCTASKRERUNH

#include "API.h"
#include "AsyncTask.h"
#include <memory>
#include "TaskWithCallback.h"

namespace valcui
{

struct RerunFunc
{
    unsigned long       maxWaitMillis;
    valc::IDToken       sampleRunID;
    std::string         sampleDescriptor;
    std::string         barcode;
    valc::SnapshotPtr   snapshot;
    int                 worklistID;

    void operator()( int& );
};


class AsyncTaskRerun : public AsyncTask
{
public:
    AsyncTaskRerun(
        valc::SnapshotPtr       snapshot,
        int                     worklistID,
        const valc::IDToken&    sampleRunID,
        const std::string&      sampleDescriptor,
        const std::string&      barcode,
        const std::string&      testName,
        unsigned long           maxWaitMillis );

    std::string getDesc() const;
    HANDLE      queue( stef::ThreadPool* tp );
    void        callback( ModelEventListener* eventSink );

private:
    RerunFunc m_rerunFunc;
    const int m_worklistID;
    const std::string m_barcode;
    const std::string m_testName;

    std::unique_ptr< stef::Submission<int, RerunFunc> > m_taskSubmission;
};

}

#endif



