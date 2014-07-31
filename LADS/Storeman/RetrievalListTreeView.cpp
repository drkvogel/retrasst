#include <vcl.h>
#pragma hdrstop
#include "RetrievalListTreeView.h"
#include "LCDbJob.h"
#include "LCDbObject.h"

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

void __fastcall TfrmRetrievalTreeView::cbNewJobClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalTreeView::cbInProgressClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalTreeView::cbDoneClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalTreeView::cbDeletedClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalTreeView::cbRejectedClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalTreeView::cbBoxRetrievalClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalTreeView::cbSampleRetrievalClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalTreeView::cbBoxMoveClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalTreeView::cbBoxDiscardClick(TObject *Sender) { loadJobs(); }
void __fastcall TfrmRetrievalTreeView::cbSampleDiscardClick(TObject *Sender) { loadJobs(); }

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
    loadJobs();
}

void TfrmRetrievalTreeView::loadJobs() {

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

    tdvecpJob::const_iterator it; //int row = 1;

	tree->Items->Clear();
	tree->Items->Add(NULL, "Exercises"); // Projects?
	TTreeNode* root = tree->Items->GetFirstNode();
	//sites = data;
	//parent->Data = sites;
	//Util::InitPropertyGrid( grdProps );
	//Util::ShowTreeNode( SampleTree, parent, true, true );
    TTreeNode* tnExercise, * tnBoxset, *tnJob;
    LCDbObject * exercise;
    int exerciseID, boxsetID, jobID;

    /*  Exercise
            Box Set
                Jobs */
/*
	typedef std::set< LCDbCryoJob * > SetOfTasks;
    typedef std::set< SetOfTasks > SetOf
	typedef std::map< int, SetOfVials > VialsInBoxesMap;
    BoxsetsInExercises boxsets;
	JobsInBoxsetsMap jobs;
	JobsInBoxsetsMap::iterator found;
*/

/*
    for (auto& job : vecJobs) {
        int
        found = .find(  );
        if (found == .end()) { // not added yet
            SetOfVials setOfVials;
            setOfVials.insert(sample);
            boxes[sourceBox] = setOfVials;
        } else { // already in map
            found->second.insert(sample);
        } ostringstream oss; oss<<sample->cryovial_barcode<<" "<<sample->aliquotName<<" in box with id "<<sourceBox; updateStatus(oss.str());
    }*/

    //LCDbObject
//		case STORAGE_EXERCISE: return "Storage exercise";
    //TTreeNode * newNode = tree->Items->AddObject(parent, tmp, (TObject *)newDatum);

    // for (auto& job : vecJobs)
    int jobsets[] = { 1,1,1,1,1,2,1,1,1,3,1,4};
    int jobcount = 0, jobset = 0;
    enum Level { EXERCISE, BOXSET, JOB } ;
    Level thelevel = EXERCISE;
    for (it = vecJobs.begin(); it != vecJobs.end(); it++) { //, row++) {
        if (EXERCISE == thelevel) {
            tnExercise = tree->Items->AddObject(root, "Exercise", (TObject *)NULL);
        } else if (BOXSET == thelevel) {
            tree->Items->AddObject(root, exercise->getDescription().c_str(), (TObject *)exercise);
        } else if (JOB == thelevel) {

        }
        LCDbCryoJob * job = *it;
        //if (newExercise) {
          //  root->AddNode(
        //}
        jobID       = job->getID();
        exerciseID  = job->getExercise();
        const LCDbObject * exercise = LCDbObjects::records().findByID(exerciseID);

        boxsetID    = job->getBoxSet();

        // add exercise
        if (NULL == exercise) {
            tree->Items->AddObject(root, "null", (TObject *)exercise);
        } else {
            tree->Items->AddObject(root, exercise->getDescription().c_str(), (TObject *)exercise);
        }

        // add boxset
        ostringstream oss; oss<<"Box Set "<<to_string((long long)boxsetID);
        //String tmp; tmp += "Box Set "+to_string((long long)boxsetID);
        tree->Items->AddObject(root, oss.str().c_str(), (TObject *)NULL);

        // add task
        tree->Items->AddObject(root, job->getDescription().c_str(), (TObject *)job);

        //exercise, * boxset, *retrieval;
//        = job->getBoxSet(); // short
//        = job->getDescription().c_str();
//        = jobTypeString(job->getJobType()); // UNKNOWN, BOX_MOVE, BOX_RETRIEVAL, BOX_DISCARD, SAMPLE_RETRIEVAL, SAMPLE_DISCARD, NUM_TYPES
//        = jobStatusString(job->getStatus()); // NEW_JOB, INPROGRESS, DONE, DELETED = 99
//        = Util::getAliquotDescription(job->getPrimaryAliquot()).c_str(); // int
//        = Util::getAliquotDescription(job->getSecondaryAliquot()).c_str(); // int
//        = getProjectDescription(job->getProjectID()).c_str();
//        = job->getReason().c_str();
//        = job->getStartDate().DateTimeString();
//        = job->getFinishDate().DateTimeString();
//        = job->getClaimedUntil().DateTimeString();
//        sgJobs->Objects[0][row] = (TObject *)job;
    }
    ActiveControl = tree;
}

void __fastcall TfrmRetrievalTreeView::btnCloseClick(TObject *Sender) {
    Close();
}

void __fastcall TfrmRetrievalTreeView::btnNewJobClick(TObject *Sender) {
    // add a new exercise, box set, job
}


void __fastcall TfrmRetrievalTreeView::btnExtendJobClick(TObject *Sender) {
    // add a job to an existing box set
}


