#ifndef DBUPDATETASKH
#define DBUPDATETASKH

#include <string>
#include "Task.h"

namespace paulst
{
    class Config;
    class LoggingService;
};

namespace paulstdb
{
    class DBConnection;
}

namespace valc
{

class SampleRunIDResolutionService;

class DBUpdateTask : public stef::Task
{
public:
    DBUpdateTask();
    virtual ~DBUpdateTask();
    void setConfig( const paulst::Config* c );
    void setConnection( paulstdb::DBConnection* c );
    void setLog( paulst::LoggingService* log );
    void setSampleRunIDResolutionService( SampleRunIDResolutionService* s );
protected:
    virtual std::string             describeUpdate() const = 0;
    void doStuff();
    virtual void                    updateDatabase() = 0;
    SampleRunIDResolutionService*   getSampleRunIDResolutionService() const;
    const paulst::Config*           getConfig() const;
    paulstdb::DBConnection*         getConnection() const;
private:
    SampleRunIDResolutionService*       m_sampleRunIDResolutionService;
    paulst::LoggingService*             m_log;
    paulstdb::DBConnection*             m_connection;
    const paulst::Config*               m_config;
};

};

#endif

