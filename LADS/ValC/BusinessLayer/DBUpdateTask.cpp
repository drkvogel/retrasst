#include "API.h"
#include "DBUpdateTask.h"
#include "LoggingService.h"
#include "StrUtil.h"
#include <System.hpp>

namespace valc
{

DBUpdateTask::~DBUpdateTask()
{
}

bool DBUpdateTask::execute()
{
    require( m_sampleRunIDResolutionService );
    require( m_log );
    require( m_exceptionHandlingPolicy );
    require( m_connection );

    bool continuePerformingOtherUpdates = true;

    try
    {
        std::string logMsg = std::string("Updating database: ") + describeUpdate();
        LOG( logMsg );
        updateDatabase();
    }
    catch( const Exception& e )
    {
        handleException( paulst::stdstring( e.Message ), continuePerformingOtherUpdates );
    }
    catch( ... )
    {
        handleException( "Unknown exception.", continuePerformingOtherUpdates );
    }

    return continuePerformingOtherUpdates;
}

DBConnection* DBUpdateTask::getConnection() const
{
    return m_connection;
}

SampleRunIDResolutionService* DBUpdateTask::getSampleRunIDResolutionService() const
{
    return m_sampleRunIDResolutionService;
}

void DBUpdateTask::handleException( const std::string& msg, bool& continuePerformingOtherUpdates )
{
    try
    {
        std::string logMsg = std::string("Error. ") + msg + " Failed to perform the following update: " + describeUpdate();
        LOG( logMsg );
        m_exceptionHandlingPolicy->handleException( msg );
    }
    catch( ... )
    {
    }
    continuePerformingOtherUpdates = false;
}

void DBUpdateTask::setConnection( DBConnection* c )
{
    m_connection = c;
}

void DBUpdateTask::setExceptionHandlingPolicy( DBUpdateExceptionHandlingPolicy* p )
{
    m_exceptionHandlingPolicy = p;
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

