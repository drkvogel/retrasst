#ifndef BusinessLayerH
#define BusinessLayerH

#include "API.h"
#include "ExceptionHandler.h"
#include <memory>
#include <sstream>
#include <string>
#include <System.Classes.hpp>
#include "ValCDialogs.h"

namespace paulst
{
	class LoggingService;
}


namespace stef
{
	class ThreadPool;
}


namespace valcui
{

/** Encapsulates any non-trivial interaction with the business layer.
  * It presents a synchronous interface to operations that might,
  * in fact, be asynchronous (eg queuing a rerun).
  *
  * It has its own thread on which to interact with the business layer.
  * This ensures that the UI thread doesn't get stuck, for example it can
  * feel free to time-out whilst waiting for a database lock.
  */
class BusinessLayer
{
public:
	BusinessLayer(
		int machineID,
		int userID,
		const std::string& config,
		paulst::LoggingService* log,
		valc::UserAdvisor* warningsListener
		);
	~BusinessLayer();
    void borrowSnapshot( TThreadMethod callback );
    bool close();
    void forceReload();
    valc::SnapshotPtr& getSnapshot();
    void rerun(
        int worklistID, 
        const std::string& sampleRunID, 
        const std::string& sampleDescriptor,
        const std::string& barcode,
        const std::string& testName );
    void runPendingUpdates();

private:
	stef::ThreadPool* m_threadPool;
	valc::SnapshotPtr m_snapshot;
    std::unique_ptr<valc::UserAdvisor> m_warningsListener;

	BusinessLayer( const BusinessLayer& );
	BusinessLayer& operator=( const BusinessLayer& );

	static const int THREAD_POOL_SHUTDOWN_WAIT_MILLIS = 1000;
};


/*
    Designed to work with types built using stef::Submission, defined in
    paulst/stef/TaskWithCallback.h
*/
template<typename AsyncTask>
void waitForAsyncTask( AsyncTask& t, const std::string& taskName, bool showDialog = true )
{
    unsigned long waitResult{};
    const unsigned long MAX_WAIT_MILLIS = 60 * 1000;

    if ( showDialog )
    {
        std::ostringstream waitDlgText;

        waitDlgText << "Executing task '" << taskName << "'...";

        waitResult = showWaitDialog( t.completionSignal(), waitDlgText.str(), MAX_WAIT_MILLIS );
    }
    else
    {
        waitResult = WaitForSingleObject( t.completionSignal(), MAX_WAIT_MILLIS );
    }

    if ( waitResult != WAIT_OBJECT_0 )
    {
        throwWaitException( waitResult, taskName );
    }

    assertion( t.completed(), "Task should have completed, given that it didn't timeout." );

    if ( t.error().size() )
    {
        throw Exception( t.error().c_str() );
    }

    if ( t.cancelled() )
    {
        throw Exception( "Task got cancelled." );
    }
}

}
#endif

