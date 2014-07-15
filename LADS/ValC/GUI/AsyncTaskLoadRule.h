#ifndef ASYNCTASKLOADRULEH
#define ASYNCTASKLOADRULEH

#include "AsyncTask.h"
#include <memory>
#include "TaskWithCallback.h"

namespace stef
{
    class ThreadPool;
}

namespace valcui
{

struct LoadRule
{
    int test, machine, project;
    LoadRule( int t, int m, int p );
    LoadRule( const LoadRule& other );
    LoadRule& operator=( const LoadRule& other );
    void operator()( std::string& rule );
};

class AsyncTaskLoadRule : public AsyncTask
{
public:
    AsyncTaskLoadRule( int test, int machine, int project );
    std::string getDesc() const;
    HANDLE queue( stef::ThreadPool* tp );
    void callback( ModelEventListener* eventSink );
private:
    LoadRule m_loadFunc;
    std::unique_ptr< stef::Submission<std::string, LoadRule> > m_submission;
};

}

#endif

