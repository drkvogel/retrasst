#ifndef SnapshotFrameControllerH
#define SnapshotFrameControllerH


#include "WorklistEntriesView.h"
#include <memory>
#include "ModelEventListenerAdapter.h"
#include "TSnapshotFrame.h"

                                class LogManager;

namespace valcui
{

                                class Model;

class SnapshotFrameController : public SnapshotFrameObserver
{
public:
	SnapshotFrameController(TSnapshotFrame* tsf,
							Model* m,
							LogManager* log,
							const std::string& guiConfig);


	// methods in the SnapshotFrameObserver interface:
	void notifyAssociatedWith(TSnapshotFrame* tsf);
	void notifyDestroyed(TSnapshotFrame* tsf);
	void notifyForceReloadButtonClick(TSnapshotFrame* tsf);
	void notifyMainSplitterMouseUp(TSnapshotFrame* tsf);
	void notifySelected(int worklistEntryID);
	void notify(int modelEvent, const std::string& eventData ); // ModelEventConstants.h


	void resize();
	void __fastcall update();

private:
    valcui::Model* m_model;
    ModelEventListenerAdapter<SnapshotFrameController> m_eventListener;
    // std::unique_ptr<DataManager> m_dataManager;
    std::unique_ptr<WorklistEntriesView> m_entriesView;
};

}

#endif

