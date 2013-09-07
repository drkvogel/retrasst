#ifndef DebugH
#define DebugH

#include <cstdio>
#include <fstream>
#include <memory>
#include <string>
#include "StringBuilder.h"

#include "TraceTime.h"

#ifdef _DEBUG

#define trace(msg)                                          \do                                                          \{                                                           \    FILE* fp = fopen( "trace.txt", "a" );                   \    fprintf( fp, "%s,%d:%s\n", __FILE__, __LINE__, msg );       \    fclose( fp );                                           \}                                                           \while(0)

#define dump(arg)                                           \do                                                          \
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
#define trace(msg)
#define traceTime()#define dump(arg)
#endif

#endif

