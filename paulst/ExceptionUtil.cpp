#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include "ExceptionUtil.h"
#include <SysUtils.hpp>

namespace paulst
{

void exception( const char* format, ... )
{
    char buffer[1024];
    va_list args;
    va_start(args, format );
    std::vsprintf( buffer, format, args );
    va_end(args);
    throw new Exception( buffer );
}

}

