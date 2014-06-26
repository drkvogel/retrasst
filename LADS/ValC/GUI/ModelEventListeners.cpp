#include "AcquireCriticalSection.h"
#include "ModelEventListener.h"
#include "ModelEventListeners.h"

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

}

