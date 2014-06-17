#ifndef SNAPSHOTOBSERVERADAPTERH
#define SNAPSHOTOBSERVERADAPTERH

#include "API.h"
#include "ModelEventListenerAdapter.h"
#include <vector>

namespace valcui
{

class Model;
class ModelEventListeners;

class SnapshotObserverAdapter : public valc::SnapshotObserver
{
public:
    SnapshotObserverAdapter( ModelEventListeners* l, Model* m );
    void checkForErrors();
    void clearErrors();
    void notify( int modelEvent, const std::string& eventData );
    // SnapshotObserver interface implementation:
    void notifyWorklistEntryChanged ( const valc::WorklistEntry* we );
    void notifyNewWorklistEntry     ( const valc::WorklistEntry* we );
    void notifySampleAddedToQueue   ( const std::string& sampleDescriptor );
    void notifySampleRunClosedOff   ( const std::string& runID );
    void notifyUpdateFailed         ( const char* errorMsg );
    
    void __fastcall startObserving();

private:
    ModelEventListenerAdapter<SnapshotObserverAdapter> m_modelEventListenerInterface;
    ModelEventListeners* m_eventSink;
    Model* m_model;
    std::vector< std::string > m_errors;

    SnapshotObserverAdapter( const SnapshotObserverAdapter& );
    SnapshotObserverAdapter& operator=( const SnapshotObserverAdapter& );
};

}

#endif

