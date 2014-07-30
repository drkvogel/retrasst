#include <vcl.h>
#pragma hdrstop
#include "RetrievalListTreeView.h"
#include "LCDbJob.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmRetrievalTreeView *frmRetrievalTreeView;

__fastcall TfrmRetrievalTreeView::TfrmRetrievalTreeView(TComponent* Owner) : TForm(Owner) { }

void __fastcall TfrmRetrievalTreeView::FormShow(TObject *Sender) {
    init();
}

void __fastcall TfrmRetrievalTreeView::FormCreate(TObject *Sender) {
    //
}

void TfrmRetrievalTreeView::init() {
//    cbLog->Checked      = false;//RETRASSTDEBUG;
//    cbLog->Visible      = true;//= RETRASSTDEBUG;
//	panelDebug->Visible = cbLog->Checked;
/*
    sgwJobs = new StringGridWrapper<LCDbCryoJob>(sgJobs, &vecJobs);
    sgwJobs->addCol("boxset",   "Box set",          30);
    sgwJobs->addCol("desc",     "Description",      312);
    sgwJobs->addCol("type",     "Job type",         95);
    sgwJobs->addCol("status",   "Status",           82);
    sgwJobs->addCol("primary",  "Primary Aliquot",  84);
    sgwJobs->addCol("secondary","Secondary Aliquot",94);
    sgwJobs->addCol("project",  "Project",          103);
    sgwJobs->addCol("reason",   "Reason",           155);
    sgwJobs->addCol("start",    "Started",          74);
    sgwJobs->addCol("finish",   "Finished",         74);
    sgwJobs->addCol("claimed",  "Claimed until",    74);
    sgwJobs->init();
*/
    //initCustom();
    //loadJobs();

    Screen->Cursor = crSQLWait;
    LQuery qc(LIMSDatabase::getCentralDb());
    jobs = LCDbCryoJobs::records();
	jobs.read(LIMSDatabase::getCentralDb(), LCDbCryoJob::UNKNOWN, true); // $2 true: readall
	delete_referenced<tdvecpJob>(vecJobs);
    for (Range< LCDbCryoJob > jr = jobs; jr.isValid(); ++jr) {
        if (!jr->isAvailable()) continue;
        if (jr->getStatus() == LCDbCryoJob::Status::NEW_JOB             && !cbNewJob->Checked) continue;
        if (jr->getStatus() == LCDbCryoJob::Status::INPROGRESS          && !cbInProgress->Checked) continue;
        if (jr->getStatus() == LCDbCryoJob::Status::DONE                && !cbDone->Checked) continue;
        if (jr->getStatus() == LCDbCryoJob::Status::DELETED             && !cbDeleted->Checked) continue;
        if (jr->getStatus() == LCDbCryoJob::Status::REJECTED            && !cbRejected->Checked) continue;
        if (jr->getJobType() == LCDbCryoJob::JobKind::BOX_RETRIEVAL     && !cbBoxRetrieval->Checked) continue;
        if (jr->getJobType() == LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL  && !cbSampleRetrieval->Checked) continue;
        if (jr->getJobType() == LCDbCryoJob::JobKind::BOX_MOVE          && !cbBoxMove->Checked) continue;
        if (jr->getJobType() == LCDbCryoJob::JobKind::BOX_DISCARD       && !cbBoxDiscard->Checked) continue;
        if (jr->getJobType() == LCDbCryoJob::JobKind::SAMPLE_DISCARD    && !cbSampleDiscard->Checked) continue;
        LCDbCryoJob * job = new LCDbCryoJob(); *job = *jr;
        vecJobs.push_back(job);
    }
    Screen->Cursor = crDefault;
    //sgJobs->RowCount = vecJobs.size() + 1;
    tdvecpJob::const_iterator it;
    //int row = 1;

	tree->Items->Clear();

	tree->Items->Add(NULL, "Exercises"); // Projects?
	TTreeNode* root = tree->Items->GetFirstNode();
	//sites = data;
	//parent->Data = sites;
	//Util::InitPropertyGrid( grdProps );
	//Util::ShowTreeNode( SampleTree, parent, true, true );
    TTreeNode* exercise, * boxset, *retrieval;

    for (it = vecJobs.begin(); it != vecJobs.end(); it++) { //, row++) {
        LCDbCryoJob * job = *it;

        //exercise, * boxset, *retrieval;
//        sgJobs->Cells[sgwJobs->colNameToInt("boxset" )]  [row] = job->getBoxSet(); // short
//        sgJobs->Cells[sgwJobs->colNameToInt("desc" )]    [row] = job->getDescription().c_str();
//        sgJobs->Cells[sgwJobs->colNameToInt("type")]     [row] = jobTypeString(job->getJobType()); // UNKNOWN, BOX_MOVE, BOX_RETRIEVAL, BOX_DISCARD, SAMPLE_RETRIEVAL, SAMPLE_DISCARD, NUM_TYPES
//        sgJobs->Cells[sgwJobs->colNameToInt("status")]   [row] = jobStatusString(job->getStatus()); // NEW_JOB, INPROGRESS, DONE, DELETED = 99
//        sgJobs->Cells[sgwJobs->colNameToInt("primary")]  [row] = Util::getAliquotDescription(job->getPrimaryAliquot()).c_str(); // int
//        sgJobs->Cells[sgwJobs->colNameToInt("secondary")][row] = Util::getAliquotDescription(job->getSecondaryAliquot()).c_str(); // int
//        sgJobs->Cells[sgwJobs->colNameToInt("project")]  [row] = getProjectDescription(job->getProjectID()).c_str();
//        sgJobs->Cells[sgwJobs->colNameToInt("reason")]   [row] = job->getReason().c_str();
//        sgJobs->Cells[sgwJobs->colNameToInt("start")]    [row] = job->getStartDate().DateTimeString();
//        sgJobs->Cells[sgwJobs->colNameToInt("finish")]   [row] = job->getFinishDate().DateTimeString();
//        sgJobs->Cells[sgwJobs->colNameToInt("claimed")]  [row] = job->getClaimedUntil().DateTimeString();
//        sgJobs->Objects[0][row] = (TObject *)job;
    }
    ActiveControl = tree;
}

void __fastcall TfrmRetrievalTreeView::btnCloseClick(TObject *Sender) {
    Close();
}


