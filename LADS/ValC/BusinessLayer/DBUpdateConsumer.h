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

class DBUpdateExceptionHandlingPolicy;
class DBUpdateSchedule;
class SampleRunIDResolutionService;

class DBUpdateConsumer : public paulst::Runnable
{
public:
    DBUpdateConsumer( 
        paulstdb::DBConnection* connection, 
        paulst::LoggingService* log,
        SampleRunIDResolutionService* s,
        DBUpdateSchedule* updateSchedule,
        DBUpdateExceptionHandlingPolicy* exceptionCallback );
    void run( const paulst::Event* stopSignal );
    void waitFor();
private:
    paulstdb::DBConnection*             m_connection;
    paulst::LoggingService*             m_log;
    SampleRunIDResolutionService*       m_sampleRunIDResolutionService;
    DBUpdateSchedule*                   m_updateSchedule;
    DBUpdateExceptionHandlingPolicy*    m_exceptionCallback;
    paulst::Thread m_thread;
};

};

#endif

