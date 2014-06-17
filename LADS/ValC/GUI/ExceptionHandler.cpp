#include "ExceptionHandler.h"
#include "LogManager.h"
#include <sstream>
#include "ValCDialogs.h"

namespace valcui
{

ExceptionHandler::ExceptionHandler( LogManager* log )
    :
    m_log( log )
{
}

void ExceptionHandler::operator()( const Exception& e )
{
    if ( m_log )
    {
        m_log->logException( e );
    }

    reportException(e);
}

void assertion( bool expression, const char* msg )
{
    if ( ! expression )
    {
        throw Exception( msg );
    }
}

std::string describeWaitResult( unsigned long waitResult )
{
    std::string desc;

    switch( waitResult )
    {
    case WAIT_ABANDONED : desc = "WAIT_ABANDONED"   ; break;
    case WAIT_OBJECT_0  : desc = "WAIT_OBJECT_0"    ; break;
    case WAIT_TIMEOUT   : desc = "WAIT_TIMEOUT"     ; break;
    case WAIT_FAILED    : desc = "WAIT_FAILED"      ; break;
    }

    return desc;
}

void throwWaitException( unsigned long waitResult, const std::string& operation )
{
    std::ostringstream ss;
    
    ss << "Abandoned waiting for operation '" << operation << "': " << describeWaitResult( waitResult );

    throw Exception( ss.str().c_str() );
}

}

