#ifndef EXCEPTIONHANDLERH
#define EXCEPTIONHANDLERH

#include <string>
#include <SysUtils.hpp>

class LogManager;

namespace valcui
{

struct ExceptionHandler
{
    ExceptionHandler( LogManager* l = NULL );
    void operator()( const Exception& e );
private:
    LogManager* m_log;
};

void        assertion( bool expression, const char* msg );
std::string describeWaitResult( unsigned long waitResult );
void        throwWaitException( unsigned long waitResult, const std::string& operation );

}

#endif

