#include <FMX.Dialogs.hpp>
#include "Model.h"
#include "ModelEventConstants.h"
#include "SnapshotFrameController.h"

namespace valcui
{

SnapshotFrameController::SnapshotFrameController( 
    TSnapshotFrame* tsf, 
    Model* model, 
    LogManager* log, 
    const std::string& guiConfig)
    :
    m_model(model),
    m_eventListener(this),
    m_dataManager( new DataManager(log) ),
    m_guiManager( new GUImanager(tsf, m_dataManager.get(), log, guiConfig) )
{
    m_model->registerModelEventListener( &m_eventListener );
    tsf->setObserver(this);
    m_guiManager->setObserver(this);
}

void SnapshotFrameController::notifyAssociatedWith( TSnapshotFrame* tsf )
{
}

void SnapshotFrameController::notifyDestroyed( TSnapshotFrame* tsf )
{
}

void SnapshotFrameController::notifyForceReloadButtonClick( TSnapshotFrame* tsf )
{
    m_model->doForceReload();
}

void SnapshotFrameController::notifyMainSplitterMouseUp( TSnapshotFrame* tsf )
{
    m_guiManager->recordPanelHeights();
}

void SnapshotFrameController::notifySelected( int worklistEntryID )
{
    m_model->setSelectedWorklistEntry( worklistEntryID );
}

void __fastcall SnapshotFrameController::update()
{
    m_dataManager->haveSnapshot = false;
    m_dataManager->clearAll();
    m_guiManager->clearAll();
    m_dataManager->makeDataModel(m_model->getSnapshot());
    m_guiManager->setUpGUIcomponents();  // components representing the data model
}

void SnapshotFrameController::notify( int modelEvent )
{
    using namespace MODEL_EVENT;

    switch( modelEvent )
    {
    case FORCE_RELOAD       : m_model->borrowSnapshot( update ); break;
    case WARNING_ALARM_ON   : ShowMessage( "Warning alarm on" ); break;
    case WARNING_ALARM_OFF  : ShowMessage( "Warning alarm off" ); break;
    }
}

void SnapshotFrameController::resize()
{
    m_guiManager->adjustPanelHeights();
}

}

