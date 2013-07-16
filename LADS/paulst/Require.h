#ifndef REQUIRE_H
#define REQUIRE_H

#include <iostream>
#include <sstream>
#include <string>

#include "Util.h"

#include <SysUtils.hpp>

#define require(exp)                               \
do                                                 \
{   if (! (exp) )                                  \
    {                                              \
        std::ostringstream s;                      \
        s << "Assertion failure: " << __FILE__     \
            << ", " << __LINE__ << ":";            \
        throw Exception( ( s.str() + std::string(#exp) ).c_str() );        \
    }                                              \
}                                                  \
while(0)

#define throwLastError()                            \
do                                                  \
{                                                   \
    std::ostringstream s;                          \
    s << "WINAPI error: " << __FILE__              \
        << ", " << __LINE__ << ": "                 \
        << GetLastError() << ", " <<                     \
        paulst::getErrorMessage( GetLastError() );       \
    throw Exception( s.str().c_str() );            \
}                                                   \
while(0)

#define throwException(msg)                                         \
do                                                                  \
{                                                                   \
    throw Exception( ( std::string( "Error. " ) + msg ).c_str() );  \
}                                                                   \
while(0)

#define throwRuntimeError(msg)                  \
do                                              \
{                                               \
    throw Exception(                            \
        (                                       \
            std::string( "Runtime Error: " ) +  \
            std::string( __FILE__ ) +           \
            std::string( "," ) +                \
            paulst::toString( __LINE__ ) +      \
            std::string( ": " ) +               \
            msg                                 \
        ).c_str()                               \
    );                                          \
}                                               \
while(0)

#define notImplemented() throwRuntimeError("not implemented")

#define throwIf(exp)            \
do                                                 \
{   if ( (exp) )                                  \
    {                                              \
        std::ostringstream s;                      \
        s << "Assertion failure: " << __FILE__     \
            << ", " << __LINE__ << ":";            \
        throw Exception( ( s.str() + std::string(#exp) ).c_str() );        \
    }                                              \
}                                                  \
while(0)

#define throwUnless( condition ) require( condition )


#endif
