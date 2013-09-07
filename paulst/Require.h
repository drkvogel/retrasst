#ifndef REQUIRE_H
#define REQUIRE_H

#include <string>
#include "StringBuilder.h"
#include "StrUtil.h"

#include <SysUtils.hpp>

#define require(exp)                               \
do                                                 \
{   if (! (exp) )                                  \
    {                                              \
        std::string _s = std::string("Assertion failure: ") << __FILE__ << ", " << __LINE__ << ":" << #exp;            \
        throw Exception( _s.c_str() );             \
    }                                              \
}                                                  \
while(0)

#define throwLastError()                                \
do                                                      \
{                                                       \
    std::string _s = std::string("WINAPI error: ")      \
        << __FILE__                                     \
        << ", "                                         \
        << __LINE__                                     \
        << ": "                                         \
        << GetLastError()                               \
        << ".";                                         \
    throw Exception( _s.c_str() );                      \
}                                                       \
while(0)

#define throwException(msg)  throw Exception( ( std::string( "Error. " ) << msg ).c_str() );

#define throwRuntimeError(msg)                  \
    throw Exception( ( std::string( "Runtime Error: " ) << __FILE__ << "," << __LINE__ << ": " << msg ).c_str() );

#define notImplemented() throwRuntimeError("not implemented")

#define throwIf(exp)                \
    if ( (exp) )                    \
        throwRuntimeError( #exp );

#define throwUnless( condition )  throwIf( ! (condition) )                                                                  \

#endif

