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
#include <FMX.Layouts.hpp>
#include <map>
#include <memory>
#include "ModelEventListenerAdapter.h"
#include <string>
#include "ToolTemplate.h"


namespace valcui
{
    class IdleService;
	class MenuViewController;
	class Model;
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
	TMenuItem *menuItemView;
	TMenuItem *menuItemBatchNav;
	TFlowLayout *toolBox;
	TMenuItem *menuItemQCControl;
	TMenuItem *menuItemWorklistItem;
	TTabItem *batch;
	TPanel *batchViewContainer;
	TStyleBook *BatchView;
	TTabItem *ruleTab;
	TPanel *ruleViewContainer;
	void __fastcall onCreate(TObject *Sender);
	void __fastcall onResize(TObject *Sender);
	void __fastcall onClose(TObject *Sender, TCloseAction &Action);
	void __fastcall idleTime(TObject *Sender);

private:

	paulst::Dir                                         m_appDataDir;
	paulst::Config                                      m_config;
    paulst::Config                                      m_guiConfig;
	std::unique_ptr<valcui::Model>                      m_model;
    std::unique_ptr<valcui::IdleService>                m_idleService;
    valcui::ModelEventListenerAdapter<TMainForm>        m_modelEventListener;
    std::unique_ptr<valcui::MenuViewController>         m_menuViewController;
	bool                                                m_okToClose;
	std::vector<valcui::Tool*>                          m_tools;

    template<typename Controller, typename View, typename Container>
    void addTool( 
        const std::string& name,
        Controller* controller,
        Container* container,
        TAlignLayout alignment = TAlignLayout::alClient )
    {
        m_tools.push_back(
            new valcui::ToolTemplate< Controller, View, Container >(
                name,
                controller,
                container,
                m_model.get(),
                m_idleService.get(),
                alignment ) );
    }

public:		// User declarations

	__fastcall TMainForm(TComponent* Owner);
	__fastcall ~TMainForm();
	void addTool( const std::string& toolName );
	void notify( int modelEventID, const valcui::EventData& ed );
	void removeTool( const std::string& toolName );
	

};

//---------------------------------------------------------------------------

extern PACKAGE TMainForm *MainForm;

//---------------------------------------------------------------------------


#endif

