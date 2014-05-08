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

void throwTimeoutException( const std::string& operation )
{
    std::ostringstream ss;
    
    ss << "Given up on '" << operation << "'. Taking too long.";

    throw Exception( ss.str().c_str() );
}

}

