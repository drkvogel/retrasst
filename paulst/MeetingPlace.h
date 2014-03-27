#ifndef MEETINGPLACEH
#define MEETINGPLACEH

#include "AcquireCriticalSection.h"
#include "CritSec.h"

namespace paulst
{

/*
    MeetingPlace is a component that facilitates inter-thread communication.
    It manages the risk that one party may have terminated by the time thother party is wanting to make a callback.

    MeetingPlace is a shared space, controlled by a Critical Section, shared by 2 parties, X and Y.

    When one party leaves, the other is notified, unless the other has already left.

    The instance deletes itself if, on one party leaving, it is found that no parties are left present.
*/
template< class X, class Y, class XMsg, class YMsg >
class MeetingPlace
{
private:
    CritSec m_cs;
    volatile X* m_x;
    volatile Y* m_y;

public:
    MeetingPlace()
        : m_x(0), m_y(0)
    {
    }

    void arrive( X* x )
    {
        AcquireCriticalSection a(m_cs);

        m_x = x;
    }

    void arrive( Y* y )
    {
        AcquireCriticalSection a(m_cs);

        m_y = y;
    }

    void deleteIfEmptied()
    {
        if ( (m_x == 0) && (m_y == 0) )
        {
            delete this;
        }
    }

    void leave( X* x, XMsg& xmsg )
    {
        AcquireCriticalSection a(m_cs);

        if ( m_y )
        {
            m_y->notify( xmsg );
        }

        m_x = 0;

        deleteIfEmptied();
    }

    void leave( Y* y, YMsg& ymsg )
    {
        AcquireCriticalSection a(m_cs);

        if ( m_x )
        {
            m_x->notify( ymsg );
        }

        m_y = 0;

        deleteIfEmptied();
    }

};

}

#endif

