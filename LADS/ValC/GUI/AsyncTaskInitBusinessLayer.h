#ifndef ASYNCTASKINITBUSINESSLAYERH
#define ASYNCTASKINITBUSINESSLAYERH

#include "AsyncTask.h"
#include <memory>
#include "TaskWithCallback.h"

namespace paulst
{
    class Config;
    class LoggingService;
}

namespace valc
{
    class UserAdvisor;
}

namespace valcui
{

struct InitialiseBusinessLayer
{
	int machineID;
	int userID;
	std::string config;
	paulst::LoggingService* log;
	valc::UserAdvisor* warningsListener;

    void operator()( int& i );
};

class AsyncTaskInitBusinessLayer : public AsyncTask
{
public:
    AsyncTaskInitBusinessLayer( 
        int                     machineID,
        int                     userID,
        const std::string&      config,
        paulst::LoggingService* log,
        valc::UserAdvisor*      warningsListener );

    std::string getDesc () const;
    HANDLE      queue   ( stef::ThreadPool* tp );
    void        callback( ModelEventListener* eventSink );

private:
    InitialiseBusinessLayer m_initialisationFunc;
    std::unique_ptr< stef::Submission<int, InitialiseBusinessLayer> > m_taskSubmission;
};

}

#endif

    

