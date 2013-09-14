//---------------------------------------------------------------------------

#include <vcl.h>

#include "Storemain.h"
#include "TfrmAboutBox.h"
#include "TfrmConfirm.h"
#include "LCDbAuditTrail.h"
#include "LCDbProject.h"
#include "LeaseIDs.h"
#include "TfrmLogin.h"
#include "SampleEntry.h"
#include "BrowseSample.h"
#include "Configure.h"
#include "DiscardDb.h"
#include "DiscardContext.h"
#include "DiscardSamples.h"
#include "DiscardPrologue.h"
#include "MoveJobs.h"
#include "LPDbBoxes.h"
#include "LDbBoxType.h"
#include "LDbBoxSize.h"
#include "LPDbCryovialStore.h"
#include "LPDbCryovial.h"
#include "LCDbTankMap.h"
#include "LCDbTankLayout.h"
#include "BoxTransfer.h"
#include "AddSpecimens.h"
#include "AnalyseSamples.h"
#include "StoreDAO.h"
#include "ReferredBoxes.h"
#include "StoreUtil.h"
#include "SampleMove.h"
#include "RetrievalAssistant.h"

//---------------------------------------------------------------------------

#pragma hdrstop

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmStoremain *frmStoremain;
TForm * TfrmLoginBase::mainForm = NULL;

const char * TfrmAboutBox::author = "Core Programming";
const char * TfrmAboutBox::extension = "x3532";
const char * TfrmAboutBox::compiled = "__date__time__";

const char * LCDbAuditTrail::default_email = "Nicholas.Goodwin@ctsu.ox.ac.uk";

const int LCDbProjects::leaseID = LEASEE_STOREMAN;

//---------------------------------------------------------------------------

__fastcall TfrmStoremain::TfrmStoremain(TComponent* Owner)
 : TForm(Owner), updateStatus( STOPPED )
{
	TfrmLoginBase::mainForm = this;
}

//---------------------------------------------------------------------------
//	user may have logged in again; may need to read everything in again
//---------------------------------------------------------------------------

void __fastcall TfrmStoremain::FormShow(TObject *Sender)
{
	updateStatus = STOPPED;
    //if (!RETRASSTDEBUG)
    timer -> Enabled = true;
}

//---------------------------------------------------------------------------
//	update status if this form has focus; re-start count each time as other
//	forms can affect the number of jobs but no need to re-read project data
//---------------------------------------------------------------------------

void __fastcall TfrmStoremain::timerTimer(TObject *Sender)
{
	timer -> Enabled = false;
	if( Active && Visible ) {
		updateCounts();
		showCounts();
		timer -> Enabled = true;
	} else {
		updateStatus = STOPPED;
	}
}

//---------------------------------------------------------------------------

void TfrmStoremain::resetCounts()
{
	boxesReady = moveJobs = retrievals = discards = 0;
	nextProject = 0;
}

//---------------------------------------------------------------------------

void TfrmStoremain::updateCounts()
{
	Screen -> Cursor = crSQLWait;
	switch( updateStatus )
	{
		case STOPPED:
			resetCounts();
			updateStatus = TRANSFERS;
			break;

		case TRANSFERS:
			if( switchProject() ) {
				countBoxes();
			} else {
				updateStatus = JOBCOUNTS;
			}
			break;

		case JOBCOUNTS:
			countJobs();
			updateStatus = FINISHED;
	}
	Screen -> Cursor = crDefault;
}

//---------------------------------------------------------------------------

void __fastcall TfrmStoremain::TreeBrowse(TObject *Sender)
{
	frmBrowse->init( NULL, true );
	frmBrowse->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TfrmStoremain::OnCreateList(TObject *Sender)
{
	frmRetrieveMain->init();
	frmRetrieveMain->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TfrmStoremain::ConfigureClick(TObject *Sender)
{
	frmConfigure->init();
	frmConfigure->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TfrmStoremain::BtnXferClick(TObject *Sender)
{
	frmBoxList->init( NULL );
	frmBoxList->ShowModal();
}

//---------------------------------------------------------------------------
//	count boxes in the current project that have yet to be stored
//---------------------------------------------------------------------------

void TfrmStoremain::countBoxes() {
	LPDbBoxNames boxes;
	if( boxes.readFilled( LIMSDatabase::getProjectDb() ) ) {
		boxesReady += boxes.size();
	}
}

//---------------------------------------------------------------------------
//	count retrieval lists but don't hold any of them open afterwards
//---------------------------------------------------------------------------

void TfrmStoremain::countJobs() {
	LCDbCryoJobs jobList( false );
	if( jobList.read( LIMSDatabase::getCentralDb(), LCDbCryoJob::UNKNOWN, false ) ) {
		for( Range< LCDbCryoJob > jli = jobList; jli.isValid(); ++ jli ) {
			if( jli -> getStatus() == LCDbCryoJob::NEW_JOB
			 || jli -> getStatus() == LCDbCryoJob::INPROGRESS ) {
				switch( jli -> getJobType() ) {
					case LCDbCryoJob::BOX_MOVE:
						moveJobs ++;
						break;
					case LCDbCryoJob::BOX_RETRIEVAL:
					case LCDbCryoJob::SAMPLE_RETRIEVAL:
						retrievals ++;
						break;
					case LCDbCryoJob::BOX_DISCARD:
					case LCDbCryoJob::SAMPLE_DISCARD:
						discards ++;
				}
			}
		}
	}
}

//---------------------------------------------------------------------------
//	select next active project; return false and wrap around at end of list
//---------------------------------------------------------------------------

bool TfrmStoremain::switchProject() {
	LCDbProjects & projects = LCDbProjects::records();
	while( nextProject < projects.size() ) {
		const LCDbProject & project = projects[ nextProject ++ ];
		if( project.isInCurrentSystem() && !project.isCentral() ) {
			projects.setCurrent( project );
			return true;
		}
	}
	nextProject = 0;
	return false;
}

//---------------------------------------------------------------------------
// 	update the status indicator to show the totals as they are calculated
//---------------------------------------------------------------------------

void TfrmStoremain::showCounts()
{
	if( boxesReady > 0 || updateStatus > TRANSFERS ) {
		lbBoxesReady -> Caption = String( boxesReady ) + " boxes ready to transfer";
	}
	if( updateStatus > JOBCOUNTS ) {
		lbForRemoval -> Caption = String( moveJobs ) + " movement jobs to complete";
		lbRemoved -> Caption = String( discards ) + " sets to be removed";
		lbRetrievalLists -> Caption = String( retrievals ) + " retrieval lists created";
	}
}

//---------------------------------------------------------------------------
//	Allow user to add cryovials back into the worklist for analysis
//---------------------------------------------------------------------------

void __fastcall TfrmStoremain::BtnAnalyseClick(TObject *Sender)
{
	frmAnalyseSpecimens -> init();
	frmAnalyseSpecimens -> ShowModal();
}

//---------------------------------------------------------------------------
//	Move boxes: start a new job or continue an existing one
//---------------------------------------------------------------------------

void __fastcall TfrmStoremain::btnMoveClick(TObject *Sender)
{
	frmSelectJob->initMoveJobs();
	if( frmSelectJob->ShowModal( ) == mrOk ) {
		const LCDbCryoJob * existing = frmSelectJob->getSelected();
		if( existing == NULL || frmMove->init( existing ) ) {
			frmMove->ShowModal( );
        }
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmStoremain::BtnReferredClick(TObject *Sender)
{
	frmReferred -> init();
	frmReferred -> ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TfrmStoremain::BtnDiscardClick(TObject *Sender)
{
	do
	{
		// central db
		LIMSDatabase cdb = LIMSDatabase::getCentralDb();
		Discard::Db db( &cdb );

//		std::auto_ptr<TQuery> ctq(new TQuery(frmLogin->Owner));
//		ctq->DatabaseName = db.getCdbname().c_str();
		LQuery cq( cdb );
		db.setCq(&cq);

		Discard::Context context(&db);

		// get project db name and process step

		frmDiscardPrologue->init(&context);
		if (frmDiscardPrologue->ShowModal() != mrOk) break;

		// project db

		{
			LCDbProjects & projects = LCDbProjects::records();
			const std::string projectName = context.getProjectName();
			const LCDbProject * project =
				projects.findByName(projectName.c_str());
			if (project == 0)
			{
				std::string error = "";
				error += "failed to find project ";
				error += projectName.c_str();
				error += " at ";
				error += HERE;
				throw Exception(error.c_str());
			}
			projects.setCurrent(*project);
		}

		LIMSDatabase pdb = LIMSDatabase::getProjectDb();
		db.setPdb( &pdb );

//		std::auto_ptr<TQuery> ptq(new TQuery(frmLogin->Owner));
//		ptq->DatabaseName = db.getPdbname().c_str();
		LQuery pq( pdb );
		db.setPq(&pq);

		// main form

		frmDiscardSamples->init(&context);
		if (frmDiscardSamples->ShowModal() != mrOk) break;

	} while (false);
}

//---------------------------------------------------------------------------

void __fastcall TfrmStoremain::BtnRetrieveClick(TObject *Sender)
{
	frmRetrievalAssistant->init();
	frmRetrievalAssistant->ShowModal();
}

//---------------------------------------------------------------------------


