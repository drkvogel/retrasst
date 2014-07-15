#include "AcquireCriticalSection.h"
#include <boost/variant.hpp>
#include "Model.h"
#include "ModelEventConstants.h"
#include "ModelEventListeners.h"
#include "SnapshotObserverAdapter.h"
#include "StrUtil.h"
#include "ValCDialogs.h"

namespace valcui
{

SnapshotObserverAdapter::SnapshotObserverAdapter( ModelEventListeners* l, Model* m, IdleService* is )
    :
    m_modelEventListenerInterface(this),
    m_idleServiceUserInterface(this),
    m_eventSink(l),
    m_model(m),
    m_isSnapshotLoaded(false)
{
    m_eventSink->registerListener( &m_modelEventListenerInterface );
    is->registerUser( &m_idleServiceUserInterface );
}

bool SnapshotObserverAdapter::isSnapshotLoaded() const
{
    return m_isSnapshotLoaded;
}

void SnapshotObserverAdapter::notify( int modelEvent, const EventData& eventData )
{
    if ( modelEvent == MODEL_EVENT::FORCE_RELOAD )
    {
        m_isSnapshotLoaded = true;
        m_model->borrowSnapshot( startObserving );
    }

    if ( modelEvent == MODEL_EVENT::ASYNC_TASK_ERROR )
    {
        const std::string* errorMsg = boost::get<std::string>(&eventData);
        std::ostringstream msg;
        msg << "AsyncTask Error. ";

        if ( errorMsg )
        {
            msg << errorMsg;
        }

        showErrorMsg( msg.str() );
    }
}

// SnapshotObserver interface implementation:
void SnapshotObserverAdapter::notifyWorklistEntryChanged( const valc::WorklistEntry* we )
{
    queueEvent( MODEL_EVENT::WORKLIST_ENTRY_CHANGED, we->getID() );
}

void SnapshotObserverAdapter::notifyNewWorklistEntry( const valc::WorklistEntry* we )
{
    queueEvent( MODEL_EVENT::NEW_WORKLIST_ENTRY, we->getID() );
}

void SnapshotObserverAdapter::notifySampleAddedToQueue( const std::string& sampleDescriptor )
{
    queueEvent( MODEL_EVENT::SAMPLE_ADDED_TO_QUEUE, sampleDescriptor );
}

void SnapshotObserverAdapter::notifySampleRunClosedOff( const valc::IDToken& runID )
{
    queueEvent( MODEL_EVENT::SAMPLE_RUN_CLOSED_OFF, runID );
}

void SnapshotObserverAdapter::notifyUpdateFailed( const char* errorMsg )
{
    queueEvent( MODEL_EVENT::UPDATE_FAILED, std::string(errorMsg) );
}

void SnapshotObserverAdapter::onIdle()
{
    flushQueuedEvents();
}

void SnapshotObserverAdapter::flushQueuedEvents()
{
    paulst::AcquireCriticalSection a(m_critSec);

    for ( auto event : m_eventQueue )
    {
        m_eventSink->notify( event.first, event.second );
    }
    
    m_eventQueue.clear();
}

void SnapshotObserverAdapter::queueEvent( int eventID, const EventData& eventData )
{
    paulst::AcquireCriticalSection a(m_critSec);

    m_eventQueue.push_back( std::make_pair( eventID, eventData ) );
}

void __fastcall SnapshotObserverAdapter::startObserving()
{
    valc::SnapshotPtr snapshot = m_model->getSnapshot();

    snapshot->setObserver( this );
}

}

