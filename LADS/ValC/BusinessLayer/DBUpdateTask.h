#ifndef DBUPDATETASKH
#define DBUPDATETASKH

#include <string>

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
class SampleRunIDResolutionService;

class DBUpdateTask 
{
public:
    virtual ~DBUpdateTask();
    bool execute();
    void setConnection( paulstdb::DBConnection* c );
    void setExceptionHandlingPolicy( DBUpdateExceptionHandlingPolicy* p );
    void setLog( paulst::LoggingService* log );
    void setSampleRunIDResolutionService( SampleRunIDResolutionService* s );
protected:
    virtual std::string             describeUpdate() const = 0;
    virtual void                    updateDatabase() = 0;
    SampleRunIDResolutionService*   getSampleRunIDResolutionService() const;
    paulstdb::DBConnection*         getConnection() const;
private:
    SampleRunIDResolutionService*       m_sampleRunIDResolutionService;
    paulst::LoggingService*             m_log;
    DBUpdateExceptionHandlingPolicy*    m_exceptionHandlingPolicy;
    paulstdb::DBConnection*             m_connection;

    void                                handleException( const std::string& msg, bool& continuePerformingOtherUpdates );
};

};

#endif

