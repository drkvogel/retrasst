#include <FMX.Dialogs.hpp>
#include "Model.h"
#include "ModelEventConstants.h"
#include "SnapshotFrameController.h"
#include "WorklistEntriesPositioning.h"

namespace valcui
{

SnapshotFrameController::SnapshotFrameController(LogManager* log,
												 const paulst::Config* topConfig,
												 const paulst::Config* guiConfig)
    : 
    m_eventListener(this),
    m_idleServiceUser(this),
	m_log(log),
	m_topConfig(topConfig),
	m_guiConfig(guiConfig)
{
}

IdleServiceUser* SnapshotFrameController::getIdleServiceUserInterface()
{
    return &m_idleServiceUser;
}

ModelEventListener* SnapshotFrameController::getModelEventListenerInterface()
{
    return &m_eventListener;
}

void SnapshotFrameController::init()
{
	m_entriesView->setObserver(this);
}

//------------ begin SnapshotFrameObserver methods -------------//

void SnapshotFrameController::notifyAssociatedWith(TSnapshotFrame* tsf)
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
	m_entriesView->positioner->recordViewsHeights();
}

void SnapshotFrameController::notifySelected( int worklistEntryID )
{
	m_model->setSelectedWorklistEntry(worklistEntryID);
}

//------------- end SnapshotFrameObserver methods --------------//


void __fastcall SnapshotFrameController::update()
{
	m_entriesView->clearAll();
	m_entriesView->setUpVisualComponents(m_model->getSnapshot());
}

void SnapshotFrameController::notify( int modelEvent, const EventData& eventData )
{
    using namespace MODEL_EVENT;

    switch(modelEvent)
    {
	case FORCE_RELOAD       : m_model->borrowSnapshot(update); break;
	case WARNING_ALARM_ON   : ShowMessage("Warning alarm on"); break;
	case WARNING_ALARM_OFF  : ShowMessage("Warning alarm off"); break;
    }
}

void SnapshotFrameController::onIdle()
{
}

void SnapshotFrameController::onResize()
{
    m_entriesView->positioner->adjustViewsHeights();
}

void SnapshotFrameController::setModel( Model* model )
{
    m_model = model;
}

void SnapshotFrameController::setView( TSnapshotFrame* tsf )
{
    tsf->setObserver(this);
	m_entriesView.reset(new WorklistEntriesView(tsf,m_log,m_topConfig,m_guiConfig));
}

}

