#include "API.h"
#include "DBUpdateTask.h"
#include "LoggingService.h"
#include "StrUtil.h"
#include <System.hpp>

namespace valc
{

DBUpdateTask::DBUpdateTask()
    :
    log( 0 ),
    connection( 0 ),
    config(0)
{
}

DBUpdateTask::~DBUpdateTask()
{
}

void DBUpdateTask::doStuff()
{
    require( snapshotUpdateHandle );
    require( log );
    require( connection );
    require( config );

    std::string logMsg = std::string("Updating database: ") + describeUpdate();
    log->log( logMsg );
    updateDatabase();
}

void DBUpdateTask::setConfig( const paulst::Config* c )
{
    config = c;
}

void DBUpdateTask::setConnection( paulstdb::DBConnection* c )
{
    connection = c;
}

void DBUpdateTask::setLog( paulst::LoggingService* l )
{
    log = l;
}

void DBUpdateTask::setSnapshotUpdateHandle( const SnapshotUpdateHandle& s )
{
    snapshotUpdateHandle = s;
}

}

