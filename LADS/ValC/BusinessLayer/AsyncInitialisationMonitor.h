#ifndef ASYNCINITIALISATIONMONITORH
#define ASYNCINITIALISATIONMONITORH

#include "ExecutionCallback.h"
#include <string>
#include <utility>
#include <vector>

namespace valc
{

class AsyncInitialisationMonitor : public stef::ExecutionCallback
{
public:
    typedef std::vector< std::string > ExceptionMessages;
    typedef ExceptionMessages::const_iterator exception_msg_iterator;
    std::pair< exception_msg_iterator, exception_msg_iterator > exceptionMessages() const;
    void notifyDone( stef::Task* t, int taskSequencePosition );
    void notifyException( stef::Task* t, int taskSequencePosition, const std::string& exceptionMsg );
private:
    std::vector< std::string > m_exceptionMessages;
};

}

#endif

