#include <vcl.h>
#pragma hdrstop
#include "Temp.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmTest *frmTest;

__fastcall TfrmTest::TfrmTest(TComponent* Owner) : TForm(Owner) { }

/*
// template
    ostringstream oss; oss<<__FUNC__; debugLog(oss.str().c_str());
    LQuery q(LIMSDatabase::getCentralDb());
    //LQuery q(Util::projectQuery(project), true); // get ddb with central and project dbs
    q.setSQL("SELECT * FROM  WHERE status != 99");
    Screen->Cursor = crSQLWait;
    q.open();
    delete_referenced<vecp>(s);
    while (!q.eof()) {
        RetrievalPlan * plan = new RetrievalPlan(q.readString("name"));
        //ob-> = q.readInt("");
        //ob-> = q.readString("");
        s.push_back();
        q.next();
    }
    Screen->Cursor = crDefault;
*/

/*    //"SELECT br.box_id FROM c_box_retrieval br WHERE br.retrieval_cid = :rtid AND br.section = :sect AND status != 99");
//    // no 'chunks' yet, we haven't created them!
//    // they will exist in c_box_retrieval, but don't already exist in cryovial_store where the job comes from
//    q.setSQL("SELECT * FROM c_retrieval_job rj, cryovial_store cs WHERE rj.retrieval_cid = cs.retrieval_cid ORDER BY cs.box_cid");
*/

    // SampleRow's destructor now deletes store_record, so the following is not needed:
    //for (vecpSampleRow::const_iterator it = vials.begin(); it != vials.end(); it++) { delete (*it)->store_record; }
// next not needed - objects pointed to by vials and by chunk->rows already deleted above
//    for (vecpSampleChunk::const_iterator ch = chunks.begin(); ch != chunks.end(); ch++) {
//        delete_referenced<vecpSampleRow>((*ch)->rows);
//    }
    // similarly, the vector of pointers should be deleted on destruct - or close of the form
    // http://www.borlandtalk.com/whats-the-different-between-formclose-and-tform1--vt18757.html

/* SampleRow(  LPDbCryovialStore * store_rec, string barcode, string aliquot, string box, string site, int pos, string vessel, int shelf, string rack, int slot) :  */

/*
    // load
    ostringstream oss; oss<<__FUNC__<<": var: "; debugLog(oss.str().c_str());
    Screen->Cursor = crSQLWait;
    LQuery q(LIMSDatabase::getCentralDb());
    LQuery q(Util::projectQuery(project), true); // get ddb with central and project dbs
    q.setSQL("SELECT * FROM obs WHERE ");
    q.open();
    delete_referenced<vecpOb>(obs);
    while (!q.eof()) {
        Ob * ob = new Ob();
        ob-> = q.readInt("");
        ob-> = q.readString("");
        obs.push_back(ob);
        q.next();
    }
    Screen->Cursor = crDefault;

    // show
    int row = 1;
    sgObs->RowCount = obs.size()+1;
    vecpOb::const_iterator it;
    for (it = obs.begin(); it != obs.end(); it++, row++) {
        Ob * ob = *it;
        sgObs->Cells[SGOBJS_COL_1][row] = ob->;
        sgObs->Objects[0][row] = (TObject *)ob;
    }
*/

/* some queries
select * from c_retrieval_job rj, c_box_retrieval br where rj.retrieval_cid = br.retrieval_cid
  only two:
    -1180118
    -116

# query showing existing jobs, one a sample retrieval (4), one a box retrieval (2)
# you can see that no cryovials are attached to the boxes in the second job, and not to all boxes in the first
# ie these show existing *retrieval plans* - not the source boxes

select
  rj.retrieval_cid,
  rj.external_name,
  rj.description,
  rj.primary_aliquot,
  rj.secondary_aliquot,
  br.section,
  br.box_id,
  br.rj_box_cid,
  cr.position,
  cr.cryovial_barcode,
  rj.finish_date
from
  c_retrieval_job rj, c_box_retrieval br
left join
  l_cryovial_retrieval cr on cr.rj_box_cid = br.rj_box_cid
  and cr.position < 10
where
  rj.retrieval_cid = br.retrieval_cid
order by
  rj_box_cid, position

  (not allowed on ddb)

*/

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

	// LCDbCryoJob::Status { NEW_JOB, INPROGRESS, DONE, DELETED = 99 };
	// LCDbCryoJob::JobKind { UNKNOWN, BOX_MOVE, BOX_RETRIEVAL, BOX_DISCARD, SAMPLE_RETRIEVAL, SAMPLE_DISCARD, NUM_TYPES };

		//if (jr->isAvailable() && jr->getProjectID() == projectCID
		//&& (jr->getJobType() == LCDbCryoJob::BOX_RETRIEVAL
		// || jr->getJobType() == LCDbCryoJob::SAMPLE_RETRIEVAL)) {
			//writeJob( *jr, row++ );
		//}

    //qc.setSQL("SELECT ");
    //comboJob->AddItem(, (TObject *));
/* c_retrieval_job.status = new job (0); job type = box retrieval (2) or disposal (3) */


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
