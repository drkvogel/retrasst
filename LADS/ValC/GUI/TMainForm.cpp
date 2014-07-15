//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "BatchNavController.h"
#include "BatchViewController.h"
#include "BusinessLayer.h"
#include "FMXTemplates.h"
#include "IdleService.h"
#include "LogManager.h"
#include "MenuViewController.h"
#include "Model.h"
#include "ModelEventConstants.h"
#include "QCViewController.h"
#include "Require.h"
#include "RuleViewController.h"
#include "SampleRunViewController.h"
#include "SnapshotFrameController.h"
#include "StrUtil.h"
#include "TBatchNavFrame.h"
#include "TBatchViewFrame.h"
#include "TLogFrame.h"
#include "TMainForm.h"
#include "ToolTemplate.h"
#include "TQCViewFrame.h"
#include "TRuleFrame.h"
#include "TSampleRunFrame.h"
#include "TWorklistItemViewFrame.h"
#include "UserAdvisorAdapter.h"
#include "UserAdvisorPanel.h"
#include "ValCDialogs.h"
#include "WorklistItemViewController.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TSnapshotFrame"
#pragma resource "*.fmx"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner),
	m_appDataDir( paulst::appDataDir() + "\\ValC" ),
	m_config   ( paulst::loadContentsOf( m_appDataDir.path() + "\\config-top.txt" ) ),
    m_guiConfig( paulst::loadContentsOf( m_config.get("GUIconfig") ) ),
    m_idleService( new valcui::IdleService() ),
    m_modelEventListener(this),
	m_okToClose(false)
{
	OnClose = onClose;
}

__fastcall TMainForm::~TMainForm()
{
    for ( valcui::Tool* t : m_tools )
    {
        delete t;
    }
}

void TMainForm::addTool( const std::string& toolName)
{
    if ( toolName == "QC Control" )
    {
        addTool<valcui::QCViewController, TQCViewFrame, TFlowLayout >(
            "QC Control",
            new valcui::QCViewController(),
            toolBox, 
            TAlignLayout::alNone );
    } 
    else if ( toolName == "Worklist Item" )
    {
        addTool< valcui::WorklistItemViewController, TWorklistItemViewFrame, TFlowLayout >(
            "Worklist Item",
            new valcui::WorklistItemViewController(),
            toolBox,
            TAlignLayout::alNone );
    }
    else if ( toolName == "Batch Nav" )
    {
        addTool< valcui::BatchNavController, TBatchNavFrame, TFlowLayout >(
            "Batch Nav",
            new valcui::BatchNavController(),
            toolBox,
            TAlignLayout::alNone );
    }
    else
    {
        std::ostringstream msg;
        msg << "Tool '" << toolName << "' is not supported.";
        valcui::showErrorMsg( msg.str() );
    }
}

void TMainForm::notify( int modelEventID, const valcui::EventData& ed )
{
    if ( modelEventID == valcui::MODEL_EVENT::APPLICATION_CONTEXT_DELETED )
    {
        m_okToClose = true;
        Close();
    }
}
void __fastcall TMainForm::onClose( TObject* sender, TCloseAction& action )
{
    if ( m_okToClose )
    {
        action = TCloseAction::caFree;
        ShowMessage("Goodbye!");
    }
    else
    {
        m_model->doClose();
        action = TCloseAction::caNone;
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::onCreate(TObject *Sender)
{
	m_model = std::unique_ptr<valcui::Model>( new valcui::Model(m_idleService.get()) );

    LogManager* logManager = new LogManager( m_config.get("logFile") ); 

    addTool<LogManager, TLogFrame, TPanel>( "Log", logManager, logFrameContainer );
    addTool<valcui::SnapshotFrameController, TSnapshotFrame, TPanel>( 
        "Snapshot", 
        new valcui::SnapshotFrameController( logManager, &m_guiConfig ),
        snapshotFrameContainer );

    addTool<valcui::BatchViewController, TBatchViewFrame, TPanel>(
        "Batch",
        new valcui::BatchViewController(&m_guiConfig),
        batchViewContainer );

    addTool<valcui::RuleViewController, TRuleFrame, TPanel>(
        "Rule",
        new valcui::RuleViewController(),
        ruleViewContainer );

    addTool< valcui::SampleRunViewController, TSampleRunFrame, TPanel >(
        "Sample Run",
        new valcui::SampleRunViewController(),
        midPanel );

	auto warningCache = new valcui::UserAdvisorAdapter();

    m_model->registerModelEventListener( &m_modelEventListener );

	valcui::BusinessLayer* businessLayer = new valcui::BusinessLayer(
		-1019429,
		1234,
		paulst::loadContentsOf(m_config.get("BusinessLayerConfig")),
		logManager->logService,
	   	warningCache,
        m_model->getEventListenerInterface() );

	m_model->setLog( logManager );
	m_model->setBusinessLayer( businessLayer );

	topTabCtrl->TabIndex = 0; // Snapshot tab

	new valcui::UserAdvisorPanel(
		warningFrameContainer,
		m_model.get(),
		"Data irregularities",
		warningCache,
        m_idleService.get() );

	m_menuViewController = std::unique_ptr<valcui::MenuViewController>(
		new valcui::MenuViewController(
			MainMenu1,
			m_model.get(),
			this ) );

    m_model->init();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::onResize(TObject *Sender)
{
    for ( valcui::Tool* t : m_tools )
    {
        t->onResize();
    }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::idleTime(TObject *Sender)
{
	idleTimer->Enabled = false;
	m_idleService->onIdle();
	idleTimer->Enabled = true;
}
//---------------------------------------------------------------------------
void TMainForm::removeTool( const std::string& toolName )
{
    auto i = m_tools.begin();

    while ( i != m_tools.end() && (*i)->getName() != toolName )
    {
        ++i;
    }

    if ( i == m_tools.end() )
    {
        std::ostringstream errMsg;
        errMsg << "Tool '"<< toolName << "' not found.";
        valcui::showErrorMsg( errMsg.str() );
    }
    else
    {
        valcui::Tool* t = *i;
        m_tools.erase( i );
        delete t;
    }
}

