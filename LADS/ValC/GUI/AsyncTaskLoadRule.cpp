#include "API.h"
#include "AsyncTaskLoadRule.h"
#include "ModelEventConstants.h"
#include <sstream>
#include "ThreadPool.h"

namespace valcui
{

LoadRule::LoadRule( int t, int m, int p )
    :
    test    (t),
    machine (m),
    project (p)
{
}

LoadRule::LoadRule( const LoadRule& other )
    :
    test    ( other.test    ),
    machine ( other.machine ),
    project ( other.project )
{
}

LoadRule& LoadRule::operator=( const LoadRule& other )
{
    test    = other.test;
    machine = other.machine;
    project = other.project;
    return *this;
}

void LoadRule::operator()( std::string& rule )
{
    // rule = valc::loadRule( test, machine, project );
    std::ostringstream fakeRule;
    fakeRule << "Here's a rule for test " << test << " machine " << machine << " project " << project << "!";
    rule = fakeRule.str();
}

AsyncTaskLoadRule::AsyncTaskLoadRule( int test, int machine, int project )
    :
    m_loadFunc( test, machine, project )
{
}

std::string AsyncTaskLoadRule::getDesc() const
{
    std::ostringstream msg;
    msg << "Loading configuration rule for test " << m_loadFunc.test 
        << " machine " << m_loadFunc.machine
        << " project " << m_loadFunc.project << ".";

    return msg.str();
}

HANDLE AsyncTaskLoadRule::queue( stef::ThreadPool* tp )
{
    m_submission.reset( new stef::Submission<std::string, LoadRule>( m_loadFunc, tp ) );

    return m_submission->completionSignal();
}

void AsyncTaskLoadRule::callback( ModelEventListener* eventSink )
{
    require( m_submission );

    if ( succeeded( m_submission.get(), eventSink ) )
    {
        std::string rule = m_submission->returnValue();
        eventSink->notify( MODEL_EVENT::RULE_LOADED, rule );
    }
}


}

