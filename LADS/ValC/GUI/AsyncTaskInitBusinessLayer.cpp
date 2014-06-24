#include "AsyncTaskInitBusinessLayer.h"

namespace valcui
{

void InitialiseBusinessLayer::operator()( int& i )
{
    valc::InitialiseApplicationContext( machineID, userID, config, log, warningsListener );
}

AsyncTaskInitBusinessLayer::AsyncTaskInitBusinessLayer( 
    int                     machineID,
    int                     userID,
    const std::string&      config,
    paulst::LoggingService* log,
    valc::UserAdvisor*      warningsListener )
{
    m_initialisationFunc.machineID          = machineID;
    m_initialisationFunc.userID             = userID;
    m_initialisationFunc.config             = config;
    m_initialisationFunc.log                = log;
    m_initialisationFunc.warningsListener   = warningsListener;
}

std::string AsyncTaskInitBusinessLayer::getDesc() const { return "Initialise Business Layer"; }

HANDLE AsyncTaskInitBusinessLayer::queue( stef::ThreadPool* tp )
{
    m_taskSubmission.reset(
        new stef::Submission<int, InitialiseBusinessLayer>( m_initialisationFunc, tp ) );

    return m_taskSubmission->completionSignal();
}

void AsyncTaskInitBusinessLayer::callback( ModelEventListener* eventSink )
{
    if ( succeeded( m_taskSubmission.get(), eventSink ) )
    {
        eventSink->notify( MODEL_EVENT::APPLICATION_INITIALISED );
    }
}

}

