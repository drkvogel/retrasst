#ifndef SERIALIZEDSTORAGEH
#define SERIALIZEDSTORAGEH

#include "AcquireCriticalSection.h"
#include "CritSec.h"

namespace paulst
{

/*
    Wraps a variable within a critical section, so that parallel access to the variable is serialized.
*/
template<typename Value>
class SerializedStorage
{
public:

    SerializedStorage() 
    {
    }

    void store( const Value& in )
    {
        paulst::AcquireCriticalSection a(m_cs);

        {
            m_value = in;
        }
    }

    void load( Value& out ) const 
    { 
        paulst::AcquireCriticalSection a(m_cs);

        {
            out = m_value;
        }
    }
    
private:
    Value           m_value;
    paulst::CritSec m_cs;

    SerializedStorage( const SerializedStorage& );
    SerializedStorage& operator=( const SerializedStorage& );
}; 

}

#endif

