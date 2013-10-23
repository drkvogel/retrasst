#include "AsyncInitialisationMonitor.h"
#include "StrUtil.h"
#include "../../../paulst/stef/Task.h"

namespace valc
{

std::pair< AsyncInitialisationMonitor::exception_msg_iterator, AsyncInitialisationMonitor::exception_msg_iterator > 
AsyncInitialisationMonitor::exceptionMessages() const
{
    return std::make_pair( m_exceptionMessages.begin(), m_exceptionMessages.end() );
}

void AsyncInitialisationMonitor::notifyDone( stef::Task* t, int taskSequencePosition )
{
}

void AsyncInitialisationMonitor::notifyException( stef::Task* t, int taskSequencePosition, const std::string& exceptionMsg )
{
    m_exceptionMessages.push_back( 
        paulst::format( "Unhandled exception for task '\%s': \%s", t->getName().c_str(), exceptionMsg.c_str() ) );
}

}

