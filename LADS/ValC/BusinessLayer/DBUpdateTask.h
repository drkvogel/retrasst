#ifndef DBUPDATETASKH
#define DBUPDATETASKH

#include "SnapshotUpdateHandle.h"
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

class DBUpdateTask : public stef::Task
{
public:
    DBUpdateTask();
    virtual ~DBUpdateTask();
    void setConfig( const paulst::Config* c );
    void setConnection( paulstdb::DBConnection* c );
    void setLog( paulst::LoggingService* log );
    void setSnapshotUpdateHandle( const SnapshotUpdateHandle& s );
protected:
    virtual std::string             describeUpdate() const = 0;
    void                            doStuff();
    virtual void                    updateDatabase() = 0;

    SnapshotUpdateHandle            snapshotUpdateHandle;
    paulst::LoggingService*         log;
    paulstdb::DBConnection*         connection;
    const paulst::Config*           config;
};

};

#endif

