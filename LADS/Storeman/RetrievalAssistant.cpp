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

    // exercise_cid
    // external_name
    // description
    // reason
    // job_type
    // project_cid
    // primary_aliquo
    // process_cid
    // status
    // time_stamp
    // start_date
    // claimed_until
    // finish_date
    loadJobs();
}

void TfrmRetrievalAssistant::loadJobs() {
    LQuery qc(LIMSDatabase::getCentralDb());

    LCDbCryoJobs &jobs = LCDbCryoJobs::records();
//        case LCDbCryoJob::BOX_MOVE:
//		case LCDbCryoJob::BOX_RETRIEVAL:
//		case LCDbCryoJob::BOX_DISCARD:
//		case LCDbCryoJob::SAMPLE_RETRIEVAL:
//		case LCDbCryoJob::SAMPLE_DISCARD:
    jobs.read(LCDbCryoJob::JobKind::UNKNOWN, true); // $2 true: readall

    //Range< LCDbCryoJob > jr;
    //
	//for (int i=1, jr = jobs; jr.isValid(); ++jr, i++) {
    int size = 0;
    for (Range< LCDbCryoJob > jr = jobs; jr.isValid(); ++jr) {
        if (jr->isAvailable()) size++;
    }
    sgJobs->RowCount = size + 1;
    int i=1;
    for (Range< LCDbCryoJob > jr = jobs; jr.isValid(); ++jr, i++) {
        if (!jr->isAvailable()) continue;
//        jr->getName();
//        jr->getDescription();
//        jr->getJobType();
//        jr->getProjectID();
//        jr->getPrimaryAliquot();
//        jr->getStatus();
//        jr->getTimeStamp();
//        jr->getProcessCID();
//        jr->getUserID();
//        jr->getReason();

//        std::ostringstream oss;
//        oss
//        //<<"Name: "<<jr->getName()
//        <<", Description: "<<jr->getDescription()
//        <<", JobType: "<<jr->getJobType()
//        <<", ProjectID: "<<jr->getProjectID()
//        <<", PrimaryAliquot: "<<jr->getPrimaryAliquot()
//        <<", Status: "<<jr->getStatus()
//        <<", TimeStamp: "<<bcsToStd(jr->getTimeStamp().DateTimeString())
//        <<", ProcessCID: "<<jr->getProcessCID()
//        <<", UserID: "<<jr->getUserID()
//        <<", Reason: "<<jr->getReason();
//        sgJobs->Cells[0][i] = oss.str().c_str();

        sgJobs->Cells[0][i] = jr->getDescription().c_str();
        sgJobs->Cells[1][i] = jobTypeString(jr->getJobType()); // UNKNOWN, BOX_MOVE, BOX_RETRIEVAL, BOX_DISCARD, SAMPLE_RETRIEVAL, SAMPLE_DISCARD, NUM_TYPES
        sgJobs->Cells[2][i] = getProjectDescription(jr->getProjectID()).c_str();
        sgJobs->Cells[3][i] = jr->getReason().c_str();
        sgJobs->Cells[4][i] = jobStatusString(jr->getStatus()); // NEW_JOB, INPROGRESS, DONE, DELETED = 99
        sgJobs->Cells[5][i] = jr->getUserID();
        sgJobs->Cells[6][i] = getAliquotDescription(jr->getPrimaryAliquot()).c_str(); // int
        sgJobs->Cells[7][i] = jr->getTimeStamp().DateTimeString();
        sgJobs->Objects[0][i] = (TObject *)&(*jr);

        clbTest->AddItem(jr->getDescription().c_str(), (TObject *)&(*jr));
    }
		//if (jr->isAvailable() && jr->getProjectID() == projectCID
		//&& (jr->getJobType() == LCDbCryoJob::BOX_RETRIEVAL
		// || jr->getJobType() == LCDbCryoJob::SAMPLE_RETRIEVAL)) {
			//writeJob( *jr, row++ );
		//}

    //qc.setSQL("SELECT ");

    //comboJob->AddItem(, (TObject *));
      /*
    c_retrieval_job.status = new job (0); job type = box retrieval (2) or disposal (3)


    */
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

void __fastcall TfrmRetrievalAssistant::sgJobsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
    //
}


