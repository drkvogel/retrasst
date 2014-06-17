#ifndef SNAPSHOTUPDATETASKH
#define SNAPSHOTUPDATETASKH

#include "API.h"
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include "SnapshotUpdateHandle.h"
#include "Task.h"

namespace paulst
{
    class LoggingService;
}

namespace valc
{

class AnalysisActivitySnapshotImpl;
class DBTransactionHandler;
class SnapshotObserver;

void setSignal( HANDLE h );

/*
Constructor acquires an event HANDLE.  This is exposed via 'getDoneSignal'.
It is the responsibility of client code to close this handle. Otherwise
this resource will be leaked.
*/
class SnapshotUpdateTask : public stef::Task
{
public:
    SnapshotUpdateTask();
    HANDLE getDoneSignal();
    void setDBTransactionHandler( DBTransactionHandler* dbth );
    void setLog( paulst::LoggingService* log );
    void setSnapshot( SnapshotUpdateHandle s );
    void setSnapshotObserver( SnapshotObserver* so );
protected:
    SnapshotUpdateHandle            snapshot;
    DBTransactionHandler*           dbTransactionHandler;
    SnapshotObserver*               snapshotObserver;
    paulst::LoggingService*         log;
    HANDLE                          doneSignal;

    void doStuff()
    {
        boost::shared_ptr<void> setDoneSignalOnBlockExit( static_cast<void*>(0), boost::bind( setSignal, doneSignal ) );
        try
        {
            updateDatabase();
            updateSnapshot();
            notifyObserver();
        }
        catch( const Exception& e )
        {
            snapshotObserver->notifyUpdateFailed( AnsiString( e.Message.c_str() ).c_str() );
            throw;
        }
        catch( ... )
        {
            snapshotObserver->notifyUpdateFailed( "Unspecified exception" );
            throw;
        }
    }

    virtual void updateDatabase() = 0;
    virtual void updateSnapshot() = 0;
    virtual void notifyObserver() = 0;
};

}

#endif

