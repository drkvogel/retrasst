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

    void notify( int eventID )
    {
        m_t->notify( eventID );
    }

private:
    T* m_t;
};

}

#endif

