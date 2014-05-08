#include "BusinessLayer.h"
#include "ExceptionHandler.h"
#include <memory>
#include "ModelEventListener.h"
#include "Model.h"
#include <SysUtils.hpp>

namespace valcui
{

Model::Model()
    :
    m_log(0),
    m_selectedWorklistEntry(0)
{
}

void Model::close()
{
    if ( m_businessLayer )
    {
        m_businessLayer->close();
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

        for ( Listeners::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i )
        {
            (*i)->onForceReload( m_businessLayer->getSnapshot() );
        }
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

        for ( Listeners::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i )
        {
            (*i)->onWorklistEntrySelected(m_selectedWorklistEntry);
        }
    }
}

void __fastcall Model::warningAlarmOn()
{
    for ( Listeners::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i )
    {
        (*i)->onWarningAlarmOn();
    }
}

void __fastcall Model::warningAlarmOff()
{
    for ( Listeners::iterator i = m_listeners.begin(); i != m_listeners.end(); ++i )
    {
        (*i)->onWarningAlarmOff();
    }
}

}

