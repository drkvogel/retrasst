#include <FMX.Dialogs.hpp>
#include "Model.h"
#include "ModelEventConstants.h"
#include "SnapshotFrameController.h"

namespace valcui
{

SnapshotFrameController::SnapshotFrameController(TSnapshotFrame* tsf,
												 Model* model,
												 LogManager* log,
												 const std::string& guiConfig)
  : m_model(model),
    m_eventListener(this),
	// m_dataManager(new DataManager(log)),
	m_entriesView(new WorklistEntriesView(tsf,log,guiConfig))
{
	m_model->registerModelEventListener(&m_eventListener);
	tsf->setObserver(this);
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
    m_entriesView->recordPanelHeights();
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

void SnapshotFrameController::resize()
{
    m_entriesView->adjustPanelHeights();
}

}

