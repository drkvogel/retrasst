#ifndef BusinessLayerH
#define BusinessLayerH

#include "API.h"
#include "CritSec.h"
#include "ExceptionHandler.h"
#include <FMX.Dialogs.hpp>
#include <memory>
#include "Require.h"
#include <sstream>
#include <string>
#include <System.Classes.hpp>
#include "TWaitDlg.h"

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

class AsyncTask;
class ModelEventListener;

VOID CALLBACK WaitCallback(
    PTP_CALLBACK_INSTANCE Instance,
    PVOID                   Context,
    PTP_WAIT                Wait,
    TP_WAIT_RESULT          WaitResult );

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

    friend VOID CALLBACK WaitCallback( PTP_CALLBACK_INSTANCE , PVOID, PTP_WAIT, TP_WAIT_RESULT ); 


	BusinessLayer(
		int machineID,
		int userID,
		const std::string& config,
		paulst::LoggingService* log,
		valc::UserAdvisor* warningsListener,
        ModelEventListener* eventSink
		);
	~BusinessLayer();
    /*
    The only method on the BusinessLayer interface that 
    may be called on a thread other than the UI thread.
    */
    void borrowSnapshot( TThreadMethod callback );
    /* UI thread */
    void __fastcall asyncTaskCompletionCallback();
    /* UI thread */
    void close();
    /* UI thread */
    void forceReload();
    /* UI thread */
    valc::SnapshotPtr& getSnapshot();
    /* UI thread */
    void rerun(
        int worklistID, 
        const valc::IDToken& sampleRunID, 
        const std::string& sampleDescriptor,
        const std::string& barcode,
        const std::string& testName );
    /* UI thread */
    void runPendingUpdates();

private:
	stef::ThreadPool* m_threadPool;
	valc::SnapshotPtr m_snapshot;
    std::unique_ptr<valc::UserAdvisor> m_warningsListener;
    PTP_WAIT            m_ptpWait;
    AsyncTask*          m_currentlyExecutingTask;
    HANDLE              m_currentlyExecutingTaskHandle;
    ModelEventListener* m_eventSink;
    std::unique_ptr<TWaitDlg> m_busyDialog;
    paulst::CritSec     m_critSec;

	BusinessLayer( const BusinessLayer& );
	BusinessLayer& operator=( const BusinessLayer& );
    void execute( AsyncTask* t );
    void hideMessage();
    void marshallCallback();
    void showMessage( const std::string& msg );

	static const int THREAD_POOL_SHUTDOWN_WAIT_MILLIS = 1000;
    static const unsigned long MAX_WAIT_MILLIS = 20 * 1000;
    static const unsigned long ASYNC_TASK_TIMEOUT_MILLIS = 30 * 1000;
};

}
#endif

