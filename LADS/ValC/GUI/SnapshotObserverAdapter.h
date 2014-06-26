#ifndef SNAPSHOTOBSERVERADAPTERH
#define SNAPSHOTOBSERVERADAPTERH

#include "API.h"
#include "CritSec.h"
#include "IdleServiceUserAdapter.h"
#include "ModelEventListenerAdapter.h"
#include <vector>

namespace valcui
{

class Model;
class ModelEventListeners;

class SnapshotObserverAdapter : public valc::SnapshotObserver
{
public:
    SnapshotObserverAdapter( ModelEventListeners* l, Model* m, IdleService* idleService );
    void notify( int modelEvent, const EventData& eventData );
    // SnapshotObserver interface implementation:
    void notifyWorklistEntryChanged ( const valc::WorklistEntry* we );
    void notifyNewWorklistEntry     ( const valc::WorklistEntry* we );
    void notifySampleAddedToQueue   ( const std::string& sampleDescriptor );
    void notifySampleRunClosedOff   ( const valc::IDToken& runID );
    void notifyUpdateFailed         ( const char* errorMsg );
    void onIdle();
    void __fastcall startObserving();

private:
    ModelEventListenerAdapter<SnapshotObserverAdapter> m_modelEventListenerInterface;
    IdleServiceUserAdapter<SnapshotObserverAdapter> m_idleServiceUserInterface;
    ModelEventListeners* m_eventSink;
    Model* m_model;
    std::vector< std::pair< int, EventData > > m_eventQueue;
    paulst::CritSec m_critSec;

    SnapshotObserverAdapter( const SnapshotObserverAdapter& );
    SnapshotObserverAdapter& operator=( const SnapshotObserverAdapter& );
    void flushQueuedEvents();
    void queueEvent( int eventID, const EventData& eventData );
};

}

#endif

