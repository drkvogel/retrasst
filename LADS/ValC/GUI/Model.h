#ifndef MODELH
#define MODELH

#include "API.h"
#include <memory>
#include "ModelEventListenerAdapter.h"
#include "ModelEventListeners.h"
#include "SnapshotObserverAdapter.h"
#include <System.Classes.hpp>
#include <SysUtils.hpp>
#include <vector>

class LogManager;

namespace valcui
{

class BusinessLayer;
class ModelEventListener;

/** This encapsulates the BusinessLayer, and supports the registration
  * of ModelEventListeners.
  */
class Model
{
public:
    Model();

	/**
    Call this method to queue a request to borrow the snapshot (see 'getSnapshot' below).
    Provide a method ('callback') to be invoked when the snapshot is available.  This callback method 
    will be invoked on the main UI thread.

    Requests to borrow the snapshot are processed one-at-a-time.  Thus, provided all snapshot users only
    use the snapshot in this way (i.e. via borrowSnapshot), then they can be confident that, when using 
    the snapshot, no-one else is using it at the same time.
    
    Because the callback is invoked on the main UI thread, the callback method should refrain from 
    operations that might tie up the UI thread for too long, as this would render the application
    unresponsive.

    Note that, for the duration of the callback, the single BusinessLayer thread is suspended in a wait state. 
    Therefore callback code must not call methods on the Model that depend on this thread for their execution 
    and are blocking methods.  Deadlock will result. Blocking methods that depend on the BusinessLayer thread for 
    their execution have the prefix 'do' - e.g. doForceReload

    borrowSnapshot exists for READING / QUERYING the model.  To UPDATE the model, use the (blocking) doXYZ methods.

    Because borrowSnapshot is only queueing a request and because the callback happens asynchronously,
    'borrowSnapshot' consumes negligible processing resources.
    */
    void borrowSnapshot( TThreadMethod callback );

	/**
    Close the Model in order to release resources.
    */
    void doClose();

	/**
    Retrieve the SnapshotPtr that is maintained by the underlying BusinessLayer instance.

    Only use 'getSnapshot' from within a callback via 'borrowSnapshot' (above)
    */
    valc::SnapshotPtr getSnapshot() const;



	/**
    Performs a 'force reload'.
    
    Those wanting to call this method should call it on the main UI thread.

    The method effectively blocks until the force reload has been completed, at which 
    point registered listeners are notified.
    */
    void doForceReload();

    /*
    Queues a rerun of the specified worklist entry.

    Those wanting to call this method should call it on the main UI thread.

    The method effectively blocks until the rerun has been queued, at which 
    point registered listeners are notified.
    */
    void doRerun( 
        int worklistID, 
        const std::string& sampleRunID, 
        const std::string& sampleDescriptor,
        const std::string& barcode,
        const std::string& testName );

	void doRunPendingUpdates();

	/**
    Via getSelectedWorklistEntry and setSelectedWorklistEntry, the Model offers the opportunity
    for distinct views to synchronize with each other in respect of the data that they show.

    Usage scenario:

    User clicks on a worklist entry.
    A view component receives notification of this and calls 'setSelectedWorklistEntry' on the Model.
    The Model informs registered event listeners that a worklist entry has been selected.
    Event listeners queue a request to 'borrowSnapshot'.
    Event listeners get a callback, in which they use 'getSelectedWorklistEntry' in order to 
    retrieve the ID of the worklist entry that has been clicked on and update their UI components as necessary.
    */
    int  getSelectedWorklistEntry() const;

	/**
	Register for notifications of when the Model has changed,
	i.e. some kind of an event has occurred.
	An event could be a ForceReload, for example.
    */
	void registerModelEventListener( ModelEventListener*  );

	/**
    Specify the LogManager instance to which log messages should be written.
    
    Typically called immediately after the Model has been constructed.

    The Model does NOT assume ownership of the LogManager instance.
    */
    void setLog( LogManager* l );

	/**
    Specify the underlying BusinessLayer instance.

    Typically called immediately after the Model has been constructed.

    The Model DOES assume ownership of the BusinessLayer instance.
    */
    void setBusinessLayer( BusinessLayer* bl );

	/**
    Refer to 'getSelectedWorklistEntry' (above).
    */
    void setSelectedWorklistEntry( int worklistEntryID );

	/**
    Method by means of which UserAdvisorPanel notifies the Model 
    that it is in transition from a 'no-warning' state to a 'warning' state.

    Model notifies registered listeners (see 'registerModelEventListener' above).
    */
    void __fastcall warningAlarmOn();

    /**
    Method by means of which UserAdvisorPanel notifies the Model 
    that it is in transition from a 'warning' state to a 'no-warning' state.

    Model notifies registered listeners (see 'registerModelEventListener' above).
    */
    void __fastcall warningAlarmOff();

private:
    ModelEventListeners m_listeners;
    std::unique_ptr<BusinessLayer> m_businessLayer;
    LogManager* m_log;
    SnapshotObserverAdapter m_snapshotObserver;
    int m_selectedWorklistEntry;
    
    void handleException( const Exception& e );
};

}

#endif


