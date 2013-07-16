#ifndef DebugH
#define DebugH

#include <fstream>
#include <memory>
#include <string>

#include "TraceTime.h"

#ifdef _DEBUG

#define trace(optionalString)                               \
do                                                          \
{                                                           \
    std::auto_ptr<std::string> s(                           \
        new std::string(optionalString) );                  \
    std::ofstream _file( "trex.trace", std::ios_base::app );\
    if ( _file )                                            \
    {                                                       \
        _file <<  __FILE__   << ", " << __LINE__ ;          \
        if ( s->size() )                                    \
        {                                                   \
            _file << ": " << *s;                            \
        }                                                   \
        _file << "\n";                                      \
    }                                                       \
}                                                           \
while(0)
 
#define dump(arg)                                           \
do                                                          \
{                                                           \
    std::ostringstream s;                                   \
    s << arg;                                               \
    std::ofstream _file( "trex.trace", std::ios_base::app );\
    if ( _file )                                            \
    {                                                       \
        _file <<  __FILE__   << ", " << __LINE__            \
            << ": " << s.str() << "\n";                     \
    }                                                       \
}                                                           \
while(0)
 
#define traceTime()  paulst::TraceTime t(__FILE__, __LINE__);

#else
#define trace(optionalString) 
#define traceTime()
#define dump(arg)
#endif

#endif

