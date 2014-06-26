#ifndef ASYNCTASKCLOSEH
#define ASYNCTASKCLOSEH

#include "AsyncTask.h"
#include <memory>
#include "TaskWithCallback.h"

namespace valcui
{

struct ReleaseBusinessLayerResources
{
    void operator()(int& i );
};
 
class AsyncTaskClose : public AsyncTask
{
public:
    std::string getDesc() const;
    HANDLE queue( stef::ThreadPool* tp );
    void callback( ModelEventListener* eventSink );
private:
    std::unique_ptr< stef::Submission<int, ReleaseBusinessLayerResources> > m_submission;
};

}

#endif

