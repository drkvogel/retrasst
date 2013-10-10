#ifndef DBUPDATECONSUMERH
#define DBUPDATECONSUMERH

#include "Thread.h"

namespace paulst
{
    class LoggingService;
};

namespace paulstdb
{
    class DBConnection;
}

namespace valc
{

class ApplicationContext;
class DBUpdateExceptionHandlingPolicy;
class DBUpdateSchedule;
class SampleRunIDResolutionService;

class DBUpdateConsumer : public paulst::Runnable
{
public:
    DBUpdateConsumer( 
        ApplicationContext*                 appContext,
        SampleRunIDResolutionService*       s,
        DBUpdateSchedule*                   updateSchedule,
        DBUpdateExceptionHandlingPolicy*    exceptionCallback );
    void run( const paulst::Event* stopSignal );
    void waitFor();
private:
    ApplicationContext*                 m_appContext;
    SampleRunIDResolutionService*       m_sampleRunIDResolutionService;
    DBUpdateSchedule*                   m_updateSchedule;
    DBUpdateExceptionHandlingPolicy*    m_exceptionCallback;
    paulst::Thread m_thread;
};

};

#endif

