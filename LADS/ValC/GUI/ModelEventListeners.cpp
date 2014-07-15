#include "AcquireCriticalSection.h"
#include <algorithm>
#include "ModelEventListener.h"
#include "ModelEventListeners.h"
#include "Require.h"

namespace valcui
{

ModelEventListeners::ModelEventListeners()
{
}

void ModelEventListeners::registerListener( ModelEventListener* mel )
{
    paulst::AcquireCriticalSection a(m_critSec);

    m_listeners.push_back( mel );
}

void ModelEventListeners::notify( int modelEvent, const EventData& eventData )
{
    paulst::AcquireCriticalSection a(m_critSec);

    for ( ModelEventListener* l : m_listeners )
    {
        l->notify( modelEvent, eventData );
    }
}

void ModelEventListeners::unregisterListener( ModelEventListener* l )
{
    paulst::AcquireCriticalSection a(m_critSec);

    auto i = std::find( m_listeners.begin(), m_listeners.end(), l );

    require( i != m_listeners.end() );

    m_listeners.erase( i );
}

}

