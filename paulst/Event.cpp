#include "Event.h"
#include "Require.h"

namespace paulst
{

Event::Event()
    : m_h( CreateEvent( NULL, TRUE, FALSE, NULL ) )
{
    throwUnless (  m_h );
}

Event::~Event()
{
	CloseHandle( m_h );
}

unsigned long Event::wait( unsigned long millis ) const
{
    return WaitForSingleObject( m_h, millis );
}

bool Event::reset()
{
	return ResetEvent( m_h );
}


bool Event::set()
{
    return SetEvent( m_h );
}

}

