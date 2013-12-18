#ifndef DBUPDATETASKWITHCALLBACKH
#define DBUPDATETASKWITHCALLBACKH

#include "AcquireCriticalSection.h"
#include "CritSec.h"
#include "DBUpdateTask.h"
#include "Require.h"

namespace valc
{

template<class Callback, class DBTransaction>
class DBUpdateTaskWithCallback : public DBUpdateTask
{
private:
    paulst::CritSec     m_cs;
    DBTransaction       transaction;
    volatile Callback*  callback;

public:

    DBUpdateTaskWithCallback( Callback* c, DBTransaction t )
        :
        callback    ( c ),
        transaction ( t )
    {
        require( callback );
    }

    void unregisterCallback()
    {
        paulst::AcquireCriticalSection a(m_cs);

        {
            callback = 0;
        }
    }

protected:
    std::string describeUpdate() const
    {
        return transaction.describe();
    }

    void updateDatabase()
    {
        transaction.execute( getConnection(), getConfig() );
        
        {
            paulst::AcquireCriticalSection a(m_cs); // Protect against unregistration in between testing 'callback' and using it.

            if ( callback ) // Check that the callback hasn't unregistered.
            {
                callback->notifyDBTransactionCompleted( transaction ); 
            }
        }
    }
    
};

}

#endif

