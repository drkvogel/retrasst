#include "Model.h"
#include "ModelEventConstants.h"
#include "ModelEventListeners.h"
#include "SnapshotObserverAdapter.h"
#include "StrUtil.h"

namespace valcui
{

SnapshotObserverAdapter::SnapshotObserverAdapter( ModelEventListeners* l, Model* m )
    :
    m_modelEventListenerInterface(this),
    m_eventSink(l),
    m_model(m)
{
    m_eventSink->registerListener( &m_modelEventListenerInterface );
}

void SnapshotObserverAdapter::checkForErrors()
{
    if ( m_errors.size() )
    {
        throw Exception( m_errors.begin()->c_str() );
    }
}

void SnapshotObserverAdapter::clearErrors()
{
    m_errors.clear();
}

void SnapshotObserverAdapter::notify( int modelEvent, const EventData& eventData )
{
    if ( modelEvent == MODEL_EVENT::FORCE_RELOAD )
    {
        m_model->borrowSnapshot( startObserving );
    }
}

// SnapshotObserver interface implementation:
void SnapshotObserverAdapter::notifyWorklistEntryChanged( const valc::WorklistEntry* we )
{
    m_eventSink->notify( MODEL_EVENT::WORKLIST_ENTRY_CHANGED, we->getID() );
}

void SnapshotObserverAdapter::notifyNewWorklistEntry( const valc::WorklistEntry* we )
{
    m_eventSink->notify( MODEL_EVENT::NEW_WORKLIST_ENTRY, we->getID() );
}

void SnapshotObserverAdapter::notifySampleAddedToQueue( const std::string& sampleDescriptor )
{
    m_eventSink->notify( MODEL_EVENT::SAMPLE_ADDED_TO_QUEUE, sampleDescriptor );
}

void SnapshotObserverAdapter::notifySampleRunClosedOff( const valc::IDToken& runID )
{
    m_eventSink->notify( MODEL_EVENT::SAMPLE_RUN_CLOSED_OFF, runID );
}

void SnapshotObserverAdapter::notifyUpdateFailed( const char* errorMsg )
{
    m_errors.push_back( errorMsg );
    m_eventSink->notify( MODEL_EVENT::UPDATE_FAILED, std::string(errorMsg) );
}

void __fastcall SnapshotObserverAdapter::startObserving()
{
    valc::SnapshotPtr snapshot = m_model->getSnapshot();

    snapshot->setObserver( this );
}

}

