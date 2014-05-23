#include "BusinessLayer.h"
#include "ExceptionHandler.h"
#include <memory>
#include "Model.h"
#include "ModelEventConstants.h"
#include "ModelEventListener.h"
#include "Require.h"
#include <SysUtils.hpp>

namespace valcui
{

Model::Model()
    :
    m_log(0),
    m_selectedWorklistEntry(0)
{
}

void Model::borrowSnapshot( TThreadMethod callback )
{
    require( m_businessLayer );
    m_businessLayer->borrowSnapshot( callback );
}

void Model::close()
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

void Model::notifyListeners( int eventID )
{
    for ( ModelEventListener* l : m_listeners )
    {
        l->notify( eventID );
    }
}

void Model::registerModelEventListener( ModelEventListener* l )
{
    m_listeners.push_back( l );
}

void Model::doForceReload()
{
    ExceptionHandler handleException( m_log );

    try
    {
        m_businessLayer->forceReload();

        notifyListeners( MODEL_EVENT::FORCE_RELOAD );
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

        notifyListeners( MODEL_EVENT::WORKLIST_ENTRY_SELECTION_CHANGE );
    }
}

void __fastcall Model::warningAlarmOn()
{
    notifyListeners( MODEL_EVENT::WARNING_ALARM_ON );
}

void __fastcall Model::warningAlarmOff()
{
    notifyListeners( MODEL_EVENT::WARNING_ALARM_OFF );
}

}

