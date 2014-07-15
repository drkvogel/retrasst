#ifndef SnapshotFrameControllerH
#define SnapshotFrameControllerH

#include "IdleServiceUserAdapter.h"
#include <memory>
#include "ModelEventListenerAdapter.h"
#include "TSnapshotFrame.h"
#include "WorklistEntriesView.h"

                                class LogManager;

namespace paulst
{
    class Config;
}

namespace valcui
{

                                class Model;

class SnapshotFrameController : public SnapshotFrameObserver
{
public:
	SnapshotFrameController( LogManager* log, const paulst::Config* guiConfig);

    IdleServiceUser*    getIdleServiceUserInterface();
    ModelEventListener* getModelEventListenerInterface();
    void                init();
	// methods in the SnapshotFrameObserver interface:
	void                notifyAssociatedWith(TSnapshotFrame* tsf);
	void                notifyDestroyed(TSnapshotFrame* tsf);
	void                notifyForceReloadButtonClick(TSnapshotFrame* tsf);
	void                notifyMainSplitterMouseUp(TSnapshotFrame* tsf);
	void                notifySelected(int worklistEntryID);

	void                notify(int modelEvent, const EventData& eventData ); // ModelEventConstants.h
    void                onIdle();
	void                onResize();
    void                setModel( Model* model );
    void                setView( TSnapshotFrame* view );
	void __fastcall     update();

private:
    valcui::Model*                                      m_model;
    const paulst::Config*                               m_guiConfig;
    LogManager* const                                   m_log;
    ModelEventListenerAdapter<SnapshotFrameController>  m_eventListener;
    IdleServiceUserAdapter<SnapshotFrameController>     m_idleServiceUser;
    std::unique_ptr<WorklistEntriesView>                m_entriesView;
};

}

#endif

