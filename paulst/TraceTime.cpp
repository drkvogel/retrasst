#include "Trace.h"
#include "TraceTime.h"

#include <ctime>
#include <sstream>

namespace paulst
{

TraceTime::TraceTime( const char* file, int line )
    : m_start( static_cast<long>( std::clock() ) ),
    m_file( file ),
    m_line( line )
{
}

TraceTime::~TraceTime()
{
    long end = static_cast<long>( std::clock() );
    long elapsed = end - m_start;
    std::ostringstream msg;
    msg << "Elapsed: " << elapsed << " (" << m_file << ", " << m_line << ")";
    std::string elapsedMsg = msg.str();
    trace( elapsedMsg.c_str() );
}

}

