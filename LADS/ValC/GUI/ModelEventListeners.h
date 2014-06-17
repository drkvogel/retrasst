#ifndef MODELEVENTLISTENERSH
#define MODELEVENTLISTENERSH

#include "CritSec.h"
#include <string>
#include <vector>

namespace valcui
{

class ModelEventListener;

/*
Does not OWN the listeners.  Does not delete them.
*/
class ModelEventListeners
{
public:
    ModelEventListeners();
    void registerListener( ModelEventListener* mel );
    void notify( int modelEvent, const std::string& eventData = "" );
private:
    std::vector< ModelEventListener* > m_listeners;
    paulst::CritSec m_critSec;

    ModelEventListeners( const ModelEventListeners& );
    ModelEventListeners& operator=( const ModelEventListeners& );
};

}

#endif

