#ifndef MODELEVENTLISTENERADAPTERH
#define MODELEVENTLISTENERADAPTERH

#include "ModelEventListener.h"

namespace valcui
{

template<class T>
class ModelEventListenerAdapter : public ModelEventListener
{
public:
    ModelEventListenerAdapter(T* t)
        :
        m_t(t)
    {
    }

    void notify( int eventID, const EventData& eventData )
    {
        m_t->notify( eventID, eventData );
    }

private:
    T* m_t;
};

}

#endif

