#include "BusinessLayer.h"
#include "ExceptionHandler.h"
#include <memory>
#include "Model.h"
#include "ModelEventConstants.h"
#include "ModelEventListener.h"
#include "Require.h"
#include "StrUtil.h"
#include <SysUtils.hpp>

namespace valcui
{

Model::Model()
    :
    m_log(0),
    m_snapshotObserver( &m_listeners, this ),
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

valc::SnapshotPtr Model::getSnapshot() const
{
    require( m_businessLayer );
    return m_businessLayer->getSnapshot();
}

void Model::registerModelEventListener( ModelEventListener* l )
{
    m_listeners.registerListener( l );
}

void Model::doForceReload()
{
    ExceptionHandler handleException( m_log );

    try
    {
        m_businessLayer->forceReload();

        m_listeners.notify( MODEL_EVENT::FORCE_RELOAD );
    }
    catch( const Exception& e )
    {
        handleException(e);
    }
    catch( ... )
    {
        std::unique_ptr<Exception> e( new Exception("Unspecified") );
        handleException(*e);
    }
}

void Model::doRerun(
        int worklistID, 
        const valc::IDToken& sampleRunID, 
        const std::string& sampleDescriptor,
        const std::string& barcode,
        const std::string& testName )
{
    ExceptionHandler handleException( m_log );

    try
    {
        m_snapshotObserver.clearErrors();

        m_businessLayer->rerun( worklistID, sampleRunID, sampleDescriptor, barcode, testName );

        m_snapshotObserver.checkForErrors();

        m_listeners.notify( MODEL_EVENT::RERUN_QUEUED, worklistID );
    }
    catch( const Exception& e )
    {
        handleException(e);
    }
    catch( ... )
    {
        std::unique_ptr<Exception> e( new Exception("Unspecified") );
        handleException(*e);
    }
}

void Model::doRunPendingUpdates()
{
    ExceptionHandler handleException( m_log );

    try
    {
        m_snapshotObserver.clearErrors();

        m_businessLayer->runPendingUpdates();

        m_snapshotObserver.checkForErrors();

        m_listeners.notify( MODEL_EVENT::RUN_PENDING_UPDATES );
    }
    catch( const Exception& e )
    {
        handleException(e);
    }
    catch( ... )
    {
        std::unique_ptr<Exception> e( new Exception("Unspecified") );
        handleException(*e);
    }
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

void __fastcall Model::warningAlarmOn()
{
    m_listeners.notify( MODEL_EVENT::WARNING_ALARM_ON );
}

void __fastcall Model::warningAlarmOff()
{
    m_listeners.notify( MODEL_EVENT::WARNING_ALARM_OFF );
}

}

