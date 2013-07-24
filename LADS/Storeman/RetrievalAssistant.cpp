#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistant.h"
#include "StoreUtil.h"
#include "LCDbAuditTrail.h"
#include "LCDbObject.h"
#include "StringUtil.h"
#include "LCDbProject.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmRetrievalAssistant *frmRetrievalAssistant;

__fastcall TfrmRetrievalAssistant::TfrmRetrievalAssistant(TComponent* Owner) : TForm(Owner) { }

void TfrmRetrievalAssistant::init() {
    sgJobs->Cells[0][0] = "Description";
    sgJobs->Cells[1][0] = "Job type";
    sgJobs->Cells[2][0] = "Project";
    sgJobs->Cells[3][0] = "Reason";
    sgJobs->Cells[4][0] = "Status";
    sgJobs->Cells[5][0] = "UserID";
    sgJobs->Cells[6][0] = "Primary Aliquot";
    sgJobs->Cells[7][0] = "TimeStamp";
    sgJobs->ColWidths[0] = 400;
    sgJobs->ColWidths[1] = 120;
    sgJobs->ColWidths[2] = 100;
    sgJobs->ColWidths[3] = 200;
    sgJobs->ColWidths[4] = 100;
    sgJobs->ColWidths[5] = 50;
    sgJobs->ColWidths[6] = 200;
    sgJobs->ColWidths[7] = 100;
    // c_retrieval_job
    // exercise_cid -> c_object_name
    // external_name
    // description
    // reason          - obsolete
    // job_type
    // project_cid
    // primary_aliquo
    // process_cid
    // status
    // time_stamp       - obsolete
    // start_date
    // claimed_until
    // finish_date
    loadJobs();
}

void __fastcall TfrmRetrievalAssistant::sgJobsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
/* clSilver clMoneyGreen  clGray  clSkyBlue
#define RETRIEVAL_ASSISTANT_HIGHLIGHT_COLOUR    clActiveCaption
#define RETRIEVAL_ASSISTANT_NEW_JOB_COLOUR      clMoneyGreen
#define RETRIEVAL_ASSISTANT_IN_PROGRESS_COLOUR  clGreen
#define RETRIEVAL_ASSISTANT_DONE_COLOUR         clSkyBlue
#define RETRIEVAL_ASSISTANT_ERROR_COLOUR        clRed
#define RETRIEVAL_ASSISTANT_DELETED_COLOUR      clGray
 */
    LCDbCryoJob * job;
    TColor background = clWindow;
    if (0 == ARow)
        job = NULL;
    else
        job = (LCDbCryoJob *)sgJobs->Objects[0][ARow];
    if (NULL == job) {
        if (0 == ARow) {
            background = clBtnFace; // header row
        } else {
            background = RETRIEVAL_ASSISTANT_ERROR_COLOUR; // error
        }
    } else if (State.Contains(gdSelected)) { // && sgReferredBoxes->Focused())
		background = RETRIEVAL_ASSISTANT_HIGHLIGHT_COLOUR; //clActiveCaption; //clHighlight;
	} else {
        //background = RETRIEVAL_ASSISTANT_NEW_JOB_COLOUR;
 /*	enum Status { NEW_JOB, INPROGRESS, DONE, DELETED = 99 };
	enum JobKind { UNKNOWN, BOX_MOVE, BOX_RETRIEVAL, BOX_DISCARD, SAMPLE_RETRIEVAL, SAMPLE_DISCARD, NUM_TYPES }; */
        switch (job->getStatus()) {
        case LCDbCryoJob::Status::NEW_JOB:
            background = RETRIEVAL_ASSISTANT_NEW_JOB_COLOUR;
            break;
        case LCDbCryoJob::Status::INPROGRESS:
            background = RETRIEVAL_ASSISTANT_IN_PROGRESS_COLOUR;
            break;
        case LCDbCryoJob::Status::DONE:
            background = RETRIEVAL_ASSISTANT_DONE_COLOUR;
            break;
        case LCDbCryoJob::Status::DELETED:
            background = RETRIEVAL_ASSISTANT_DELETED_COLOUR;
            break;
        default:
            background = RETRIEVAL_ASSISTANT_ERROR_COLOUR;
        }
    }
	TCanvas * cnv = sgJobs->Canvas;
	cnv->Brush->Color = background;
	cnv->FillRect(Rect);
	cnv->TextOut(Rect.Left+5, Rect.Top+5, sgJobs->Cells[ACol][ARow]);
}


void TfrmRetrievalAssistant::loadJobs() {
    LQuery qc(LIMSDatabase::getCentralDb());
    LCDbCryoJobs &jobs = LCDbCryoJobs::records();
    jobs.read(LCDbCryoJob::JobKind::UNKNOWN, true); // $2 true: readall

    delete_referenced<tdvecpJob>(vecJobs);
    for (Range< LCDbCryoJob > jr = jobs; jr.isValid(); ++jr) {
        if (!jr->isAvailable()) continue;
        LCDbCryoJob * job = new LCDbCryoJob();
        *job = *jr;
        vecJobs.push_back(job);
    }
    showJobs();
}

void TfrmRetrievalAssistant::showJobs() {
    sgJobs->RowCount = vecJobs.size() + 1;
    tdvecpJob::const_iterator it;
    int row = 1;
    for (it = vecJobs.begin(); it != vecJobs.end(); it++, row++) {
        LCDbCryoJob * job = *it;
        sgJobs->Cells[0][row] = job->getDescription().c_str();
        sgJobs->Cells[1][row] = jobTypeString(job->getJobType()); // UNKNOWN, BOX_MOVE, BOX_RETRIEVAL, BOX_DISCARD, SAMPLE_RETRIEVAL, SAMPLE_DISCARD, NUM_TYPES
        sgJobs->Cells[2][row] = getProjectDescription(job->getProjectID()).c_str();
        sgJobs->Cells[3][row] = job->getReason().c_str();
        sgJobs->Cells[4][row] = jobStatusString(job->getStatus()); // NEW_JOB, INPROGRESS, DONE, DELETED = 99
        sgJobs->Cells[5][row] = job->getUserID();
        sgJobs->Cells[6][row] = getAliquotDescription(job->getPrimaryAliquot()).c_str(); // int
        sgJobs->Cells[7][row] = job->getTimeStamp().DateTimeString();
        sgJobs->Objects[0][row] = (TObject *)job;
    }
}

std::string TfrmRetrievalAssistant::getExerciseDescription(int exercise_cid) {
    // c_object_name: 20: storage exercise
    std::ostringstream oss;
    const LCDbObject * exercise = LCDbObjects::records().findByID(exercise_cid);
    oss << exercise->getName().c_str();
    return oss.str();
}

std::string TfrmRetrievalAssistant::getProjectDescription(int project_cid) {
    if (0 == project_cid) return "Project not specified";
    try {
        return LCDbProjects::records().get(project_cid).getName().c_str();
    } catch (...) {
        std::ostringstream oss; oss<<"Project ID "<<project_cid<<" not found"; return oss.str();
    }
}

std::string TfrmRetrievalAssistant::getAliquotDescription(int primary_aliquot_cid) {
    // c_object_name 6: aliquot type?
    std::ostringstream oss;
    if (0 == primary_aliquot_cid) return "Aliquot not specified";
    try {
        const LCDbObject * primary_aliquot = LCDbObjects::records().findByID(primary_aliquot_cid);
        oss << primary_aliquot->getName().c_str();
    } catch (...) {
        oss << "Aliquot ID "<<primary_aliquot_cid<<" not found";
    }
    return oss.str();
}

std::string TfrmRetrievalAssistant::getAuditInfo(int process_cid) {
    // c_audit_trail
    //LCDbCryoJob::getUserID()
    return "";
}

/*
jobs are LCDbCryoJob
#define 	LEASEE_STOREMAN		   100		// Storage management; blocks itself and Storage Sync

C_RETRIEVAL_JOB
A list of one or more tasks for each retrieval exercise. For example, an analysis exercise may include one task retrieving the cryovials and another for analysing them
Field name	    Type	Key Refers to	Description / Comments
retrieval_cid	i4	    1	            A unique ID for this task
exercise_cid	i4		c_object_name	The exercise this task is part of
external_name	v30			            obsolete - use exercise's name
description	    v120			        Summarises what needs to be done in this job
reason	        v120			        obsolete - use exercise's full name
job_type	    i2			            0: unknown, 1: box move, 2: box retrieval, 3: box disposal, 4: cryovial retrieval, 5: cryovial disposal
project_cid	    i4		c_project	    The project these boxes comes from (0 if boxes from more than one project)
primary_aliquot	i4		c_object_name	The aliquot being dealt with or, for sample retrieval, the preferred aliquot
process_cid	    i4		c_audit_trail	Who set this task up, when, how etc.
status	        i2			            How near this task is to reaching completion: 0 = new job, 1 = in progress, 2 = completed, 99 = invalid/obsolete
time_stamp	    d/t			            obsolete
start_date	    d/t			            When this task started (if status >= 1)
claimed_until	d/t			            Marks this task as active; no other program can use this record until the specified time
finish_date	    d/t			            When this task finished (if status == 2)

retrieval_cid, exercise_cid, external_name, description, reason, job_type, project_cid, primary_aliquot, process_cid, status, time_stamp, start_date, claimed_until, finish_date

select jobs, filter by in progress/new, retrieval/disposal
user selects one
if new && user wants to divide
    allow user to define division points
    present each section for retrieval in turn:
    foreach retrieval:
        allow re-order (sort?) by sample ID/primary aliquot location/secondary location/destination
    endfor
else # no, don't divide
    allow re-order by sample ID/primary aliquot location/secondary location/destination
endif

allow save progress at any time # REQ 8.3.6)

# 'For in progress lists these steps should be skipped (REQ 8.3.7)' ie. pick up a saved in progress job

if divided
    foreach section
        foreach sample
            process_sample()
        endfor
    endfor
    ??
else
    foreach sample
        process_sample()
    endfor
endif

process_sample()
    display sample id/barcode on screen and where it should go
    user scans/types in sample into textbox (barcode scanner just inputs text)
    if not correct
        display error
    else
        user puts sample in storage location
        user says OK somehow
    endif
end

*/

void TfrmRetrievalAssistant::loadBoxes() {
    LQuery qp = Util::projectQuery(0 /*box->project_cid*/, true); // true 2nd param gets ddb

// Find where the boxes are supposed to be:
//    qp.setSQL("select … from box_name n, box_store bs, c_rack_number r, c_tank_map m"
//        " where n.box_cid=bs.box_cid and bs.rack_cid=r.rack_cid and r.tank_cid=m.tank_cid"
//        " and bs.retrieval_cid = jobID");
//
//    sgBoxes->RowCount = 1;
    for (int i; i < 10; i++) {
        //sgBoxes->Cells[][] = "";
    }
}




