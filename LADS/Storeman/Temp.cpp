#include <vcl.h>
#pragma hdrstop
#include "Temp.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmTest *frmTest;

__fastcall TfrmTest::TfrmTest(TComponent* Owner) : TForm(Owner) { }

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
