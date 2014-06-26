//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "BusinessLayer.h"
#include "FMXTemplates.h"
#include "IdleService.h"
#include "LogManager.h"
#include "MenuViewController.h"
#include "Model.h"
#include "ModelEventConstants.h"
#include "QCViewController.h"
#include "SampleRunViewController.h"
#include "SnapshotFrameController.h"
#include "StrUtil.h"
#include "TLogFrame.h"
#include "TMainForm.h"
#include "TQCViewFrame.h"
#include "TSampleRunFrame.h"
#include "TWorklistItemViewFrame.h"
#include "UserAdvisorAdapter.h"
#include "UserAdvisorPanel.h"
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
	m_config( paulst::loadContentsOf( m_appDataDir.path() + "\\config-top.txt" ) ),
	m_logFrame(NULL),
    m_idleService( new valcui::IdleService() ),
    m_modelEventListener(this),
    m_okToClose(false)
{
    OnClose = onClose;
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
	m_logFrame                    = valcui::addSubComponent<TLogFrame>     ( logFrameContainer );
	TSnapshotFrame* snapshotFrame = valcui::addSubComponent<TSnapshotFrame>( snapshotFrameContainer );
	TQCViewFrame*   qcViewFrame   = valcui::addSubComponent<TQCViewFrame>  ( bottomPanelLeft );
	TWorklistItemViewFrame* wiFrame = valcui::addSubComponent<TWorklistItemViewFrame>  ( bottomPanelRight );
	TSampleRunFrame* srFrame = valcui::addSubComponent<TSampleRunFrame>  ( midPanel );
	m_logManager = std::unique_ptr<LogManager>(new LogManager( m_logFrame, m_config.get("logFile") ));
    m_logFrame->registerWithIdleService( m_idleService.get() );

	auto warningCache = new valcui::UserAdvisorAdapter();

	m_model = std::unique_ptr<valcui::Model>( new valcui::Model(m_idleService.get()) );
    m_model->registerModelEventListener( &m_modelEventListener );
	valcui::BusinessLayer* businessLayer = new valcui::BusinessLayer(
		-1019429,
		1234,
		paulst::loadContentsOf(m_config.get("BusinessLayerConfig")),
		m_logManager->logService,
	   	warningCache,
        m_model->getEventListenerInterface() );

	m_model->setLog( m_logManager.get() );
	m_model->setBusinessLayer( businessLayer );

	topTabCtrl->TabIndex = 0; // Snapshot tab

	new valcui::UserAdvisorPanel(
		warningFrameContainer,
		m_model.get(),
		"Data irregularities",
		warningCache,
        m_idleService.get() );

	m_snapshotFrameController = std::unique_ptr<valcui::SnapshotFrameController>(
        new valcui::SnapshotFrameController(
            snapshotFrame,
			m_model.get(),
			m_logManager.get(),
			m_config.get("GUIconfig") ) );

	m_qcViewController = std::unique_ptr<valcui::QCViewController>(
		new valcui::QCViewController(
			qcViewFrame,
			m_model.get() ) );

	m_worklistItemViewController = std::unique_ptr<valcui::WorklistItemViewController>(
		new valcui::WorklistItemViewController(
			wiFrame,
			m_model.get() ) );

	m_sampleRunViewController = std::unique_ptr<valcui::SampleRunViewController>(
		new valcui::SampleRunViewController(
			srFrame,
			m_model.get() ) );

	m_menuViewController = std::unique_ptr<valcui::MenuViewController>(
		new valcui::MenuViewController(
			MainMenu1,
			m_model.get(),
			this ) );
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::onResize(TObject *Sender)
{
	m_logFrame->onResize();
	m_snapshotFrameController->resize();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::warningAlarmOn()
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::warningAlarmOff()
{
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::idleTime(TObject *Sender)
{
	idleTimer->Enabled = false;
	m_idleService->onIdle();
	idleTimer->Enabled = true;
}
//---------------------------------------------------------------------------

