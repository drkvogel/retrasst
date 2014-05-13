#ifndef SnapshotFrameControllerH
#define SnapshotFrameControllerH

#include "DataManager.h"
#include "GUImanager.h"
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
	SnapshotFrameController(TSnapshotFrame* tsf, Model* m, LogManager* log, const std::string& guiConfig);
	void notifyAssociatedWith( TSnapshotFrame* tsf );
	void notifyDestroyed( TSnapshotFrame* tsf );
	void notifyForceReloadButtonClick( TSnapshotFrame* tsf );
	void notifyMainSplitterMouseUp( TSnapshotFrame* tsf );
    void notifySelected( int worklistEntryID );
    void onForceReload( valc::SnapshotPtr& );
    void onWarningAlarmOn();
	void onWarningAlarmOff();
	void onWorklistEntrySelected( int worklistEntryID );
    void resize();
private:
    valcui::Model* m_model;
    ModelEventListenerAdapter<SnapshotFrameController> m_eventListener;
    std::unique_ptr<DataManager> m_dataManager;
    std::unique_ptr<GUImanager> m_guiManager;
};

}

#endif
