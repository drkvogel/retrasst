#ifndef DBUPDATETASKWITHCALLBACKH
#define DBUPDATETASKWITHCALLBACKH

#include "AcquireCriticalSection.h"
#include "CritSec.h"
#include "DBUpdateTask.h"
#include "MeetingPlace.h"
#include "Require.h"

namespace valc
{

template<class Callback, class DBTransaction>
class DBUpdateTaskWithCallback : public DBUpdateTask
{
private:
    DBTransaction       transaction;
    paulst::MeetingPlace<   
        Callback, 
        DBUpdateTaskWithCallback < Callback, DBTransaction >, 
        int, 
        DBTransaction 
        >* meetingPlace;

public:

    DBUpdateTaskWithCallback( 
        paulst::MeetingPlace< Callback, DBUpdateTaskWithCallback < Callback, DBTransaction >, int, DBTransaction >* mp, DBTransaction t )
        :
        transaction ( t ),
        meetingPlace( mp )
    {
        mp->arrive( this );
    }

    void notify( int& i ) {}

protected:

    std::string describeUpdate() const
    {
        return transaction.describe();
    }

    void updateDatabase()
    {
        try
        {
            transaction.execute( connection, config );
            meetingPlace->leave( this, transaction );
        }
        catch( ... )
        {
            meetingPlace->leave( this, transaction );
            throw;
        }
    }
    
};

}

#endif

