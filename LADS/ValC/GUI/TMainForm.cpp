//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "BusinessLayer.h"
#include "FMXTemplates.h"
#include "LogManager.h"
#include "Model.h"
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
	m_logFrame(NULL)
{
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

	auto warningCache = new valcui::UserAdvisorAdapter<valcui::UserAdvisorPanel>();

	valcui::BusinessLayer* businessLayer = new valcui::BusinessLayer(
		-1019429,
		1234,
		paulst::loadContentsOf(m_config.get("BusinessLayerConfig")),
		m_logManager->logService,
	   	warningCache );

	m_model = std::unique_ptr<valcui::Model>( new valcui::Model() );
	m_model->setLog( m_logManager.get() );
	m_model->setBusinessLayer( businessLayer );

	topTabCtrl->TabIndex = 0; // Snapshot tab

	new valcui::UserAdvisorPanel(
		warningFrameContainer,
		m_model->warningAlarmOn,
		m_model->warningAlarmOff,
		"Data irregularities",
		warningCache );

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

void __fastcall TMainForm::onClose(TObject *Sender, TCloseAction &Action)
{
	m_model->close();
	Action = TCloseAction::caFree;
}
//---------------------------------------------------------------------------


