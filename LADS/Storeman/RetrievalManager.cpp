#include <vcl.h>
#pragma hdrstop
#include "RetrievalManager.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmRetrievalManager *frmRetrievalManager;

__fastcall TfrmRetrievalManager::TfrmRetrievalManager(TComponent* Owner) : TForm(Owner) { }

void __fastcall TfrmRetrievalManager::FormCreate(TObject *Sender) {
    autochunk = false;
    jobType = LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL;
}

/*
    c_retrieval_job.status = new job (0); job type = box retrieval (2) or disposal (3)

Find where the boxes are supposed to be:

    Select … from box_name n, box_store bs, c_rack_number r, c_tank_map m
    where n.box_cid=bs.box_cid and bs.rack_cid=r.rack_cid and r.tank_cid=m.tank_cid
    and bs.retrieval_cid = jobID;

*/
void __fastcall TfrmRetrievalManager::FormShow(TObject *Sender) {
    // show job: list of boxes or cryovials
    std::ostringstream oss;
    oss << (autochunk ? "auto-chunk" : "manual chunk")
    << ((jobType == LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL) ? "jobType == LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL;" : "jobType != LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL");
    Label1->Caption = oss.str().c_str();

}

void __fastcall TfrmRetrievalManager::btnCancelClick(TObject *Sender) {
    Close();
}


