#ifndef EVENTH
#define EVENTH

#include <windows.h>

namespace paulst
{

class Event
{
    public:
        // Throws exception if construction fails
        Event();
        ~Event();
        unsigned long wait( unsigned long millis ) const;
        bool reset();
        bool set();
        
    private:
        const HANDLE m_h;

        Event( const Event& );
        Event& operator=( const Event& );
};


}

#endif

