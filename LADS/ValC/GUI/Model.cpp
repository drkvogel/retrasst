#include "BusinessLayer.h"
#include <memory>
#include "Model.h"
#include "ModelEventConstants.h"
#include "ModelEventListener.h"
#include "Require.h"
#include <SysUtils.hpp>

namespace valcui
{

Model::Model(IdleService* idleService)
    :
    m_log(0),
    m_snapshotObserver( &m_listeners, this, idleService ),
    m_selectedWorklistEntry(0)
{
}

void Model::borrowSnapshot( TThreadMethod callback )
{
    require( m_businessLayer );
    m_businessLayer->borrowSnapshot( callback );
}

void Model::doClose()
{
    if ( m_businessLayer )
    {
        m_businessLayer->close();
    }
}

int Model::getSelectedWorklistEntry() const
{
    return m_selectedWorklistEntry;
}

ModelEventListener* Model::getEventListenerInterface()
{
    return &m_listeners;
}

valc::SnapshotPtr Model::getSnapshot() const
{
    require( m_businessLayer );
    return m_businessLayer->getSnapshot();
}

bool Model::isSnapshotLoaded() const
{
    return m_snapshotObserver.isSnapshotLoaded();
}

void Model::registerModelEventListener( ModelEventListener* l )
{
    m_listeners.registerListener( l );
}

void Model::doForceReload()
{
    m_businessLayer->forceReload();
}

void Model::doLoadRule( int test, int machine, int project )
{
    m_businessLayer->loadRule( test, machine, project );
}

void Model::doRerun(
        int worklistID, 
        const valc::IDToken& sampleRunID, 
        const std::string& sampleDescriptor,
        const std::string& barcode,
        const std::string& testName )
{
    m_businessLayer->rerun( worklistID, sampleRunID, sampleDescriptor, barcode, testName );
}

void Model::doRunPendingUpdates()
{
    m_businessLayer->runPendingUpdates();
}

void Model::init()
{
    require( m_businessLayer );
    m_businessLayer->init();
}

void Model::setLog( LogManager* l )
{
    m_log = l;
}

void Model::setBusinessLayer( BusinessLayer* bl )
{
    m_businessLayer = std::unique_ptr<BusinessLayer>(bl);
}

void Model::setSelectedWorklistEntry( int worklistEntryID )
{
    if ( m_selectedWorklistEntry != worklistEntryID )
    {
        m_selectedWorklistEntry = worklistEntryID;

        m_listeners.notify( MODEL_EVENT::WORKLIST_ENTRY_SELECTION_CHANGE, worklistEntryID );
    }
}

void Model::unregisterModelEventListener( ModelEventListener* l )
{
    m_listeners.unregisterListener( l );
}

void Model::warningAlarmOn()
{
    m_listeners.notify( MODEL_EVENT::WARNING_ALARM_ON );
}

void Model::warningAlarmOff()
{
    m_listeners.notify( MODEL_EVENT::WARNING_ALARM_OFF );
}

}

