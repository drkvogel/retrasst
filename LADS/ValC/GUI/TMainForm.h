//---------------------------------------------------------------------------


#ifndef TMainFormH
#define TMainFormH

//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include "TSnapshotFrame.h"
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.TabControl.hpp>

//---------------------------------------------------------------------------

#include "Config.h"
#include "DirUtil.h"
#include <FMX.Menus.hpp>
#include <memory>
#include "ModelEventListenerAdapter.h"

class LogManager;
class TLogFrame;


namespace valcui
{
    class IdleService;
	class MenuViewController;
	class Model;
	class QCViewController;
    class SampleRunViewController;
    class SnapshotFrameController;
    class WorklistItemViewController;
}


class TMainForm : public TForm

{

__published:	// IDE-managed Components

	TPanel *rootContainer;
	TPanel *topPanel;
	TSplitter *Splitter1;
	TPanel *bottomPanel;
	TTabControl *topTabCtrl;
	TTabItem *snapshotTab;
	TPanel *snapshotFrameContainer;
	TTabItem *logTab;
	TTabItem *warnTab;
	TPanel *logFrameContainer;
	TPanel *warningFrameContainer;
	TPanel *bottomPanelLeft;
	TSplitter *Splitter2;
	TPanel *bottomPanelRight;
	TStyleBook *StyleBook1;
	TPanel *midPanel;
	TSplitter *Splitter3;
	TStyleBook *SampleRunView;
	TMainMenu *MainMenu1;
	TMenuItem *menuItemDatabase;
	TMenuItem *menuItemRunPendingUpdates;
	TMenuItem *menuItemApplication;
	TMenuItem *menuItemClose;
	TTimer *idleTimer;
	void __fastcall onCreate(TObject *Sender);
	void __fastcall onResize(TObject *Sender);
	void __fastcall onClose(TObject *Sender, TCloseAction &Action);
	void __fastcall idleTime(TObject *Sender);

private:	// User declarations

	paulst::Dir m_appDataDir;
	paulst::Config m_config;
	TLogFrame* m_logFrame;
	std::unique_ptr<LogManager> m_logManager;
	std::unique_ptr<valcui::Model> m_model;
	std::unique_ptr<valcui::SnapshotFrameController> m_snapshotFrameController;
	std::unique_ptr<valcui::QCViewController> m_qcViewController;
	std::unique_ptr<valcui::WorklistItemViewController> m_worklistItemViewController;
	std::unique_ptr<valcui::SampleRunViewController> m_sampleRunViewController;
    std::unique_ptr<valcui::MenuViewController>      m_menuViewController;
    std::unique_ptr<valcui::IdleService>            m_idleService;
    valcui::ModelEventListenerAdapter<TMainForm> m_modelEventListener;
    bool                                            m_okToClose;
public:		// User declarations

	__fastcall TMainForm(TComponent* Owner);
    void notify( int modelEventID, const valcui::EventData& ed );
	void __fastcall warningAlarmOn();
	void __fastcall warningAlarmOff();

};

//---------------------------------------------------------------------------

extern PACKAGE TMainForm *MainForm;

//---------------------------------------------------------------------------


#endif

