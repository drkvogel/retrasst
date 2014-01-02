#include "API.h"
#include "DBUpdateTask.h"
#include "LoggingService.h"
#include "StrUtil.h"
#include <System.hpp>

namespace valc
{

DBUpdateTask::DBUpdateTask()
    :
    m_sampleRunIDResolutionService( 0 ),
    m_log( 0 ),
    m_connection( 0 ),
    m_config(0)
{
}

DBUpdateTask::~DBUpdateTask()
{
}

void DBUpdateTask::doStuff()
{
    require( m_sampleRunIDResolutionService );
    require( m_log );
    require( m_connection );
    require( m_config );

    std::string logMsg = std::string("Updating database: ") + describeUpdate();
    LOG( logMsg );
    updateDatabase();
}

const paulst::Config* DBUpdateTask::getConfig() const
{
    return m_config;
}

paulstdb::DBConnection* DBUpdateTask::getConnection() const
{
    return m_connection;
}

SampleRunIDResolutionService* DBUpdateTask::getSampleRunIDResolutionService() const
{
    return m_sampleRunIDResolutionService;
}

void DBUpdateTask::setConfig( const paulst::Config* c )
{
    m_config = c;
}

void DBUpdateTask::setConnection( paulstdb::DBConnection* c )
{
    m_connection = c;
}

void DBUpdateTask::setLog( paulst::LoggingService* log )
{
    m_log = log;
}

void DBUpdateTask::setSampleRunIDResolutionService( SampleRunIDResolutionService* s )
{
    m_sampleRunIDResolutionService = s;
}

}

