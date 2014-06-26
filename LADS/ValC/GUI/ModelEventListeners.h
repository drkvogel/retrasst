#ifndef MODELEVENTLISTENERSH
#define MODELEVENTLISTENERSH

#include "CritSec.h"
#include "ModelEventListener.h"
#include <string>
#include <vector>

namespace valcui
{

/*
Does not OWN the listeners.  Does not delete them.
*/
class ModelEventListeners : public ModelEventListener
{
public:
    ModelEventListeners();
    void registerListener( ModelEventListener* mel );
    void notify( int modelEvent, const EventData& eventData = EventData() );
private:
    std::vector< ModelEventListener* > m_listeners;
    paulst::CritSec m_critSec;

    ModelEventListeners( const ModelEventListeners& );
    ModelEventListeners& operator=( const ModelEventListeners& );
};

}

#endif

