#include "AcquireCriticalSection.h"
#include "StrUtil.h"
#include "ThreadExceptionMsgs.h"

namespace valc
{

void ThreadExceptionMsgs::add( const std::string& msg )
{
	paulst::AcquireCriticalSection a(m_cs);

	m_msgs.push_back( msg );
}

void ThreadExceptionMsgs::add( const UnicodeString& msg )
{
	add( paulst::stdstring( msg ) );
}

void ThreadExceptionMsgs::add( const char* msg )
{
	add( std::string( msg ) );
}

void ThreadExceptionMsgs::throwFirst()
{
	if ( m_msgs.size() )
	{
		throw Exception( UnicodeString( m_msgs.at(0).c_str() ) );
	}
}

}

