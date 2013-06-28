#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistant.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmRetrievalAssistant *frmRetrievalAssistant;

__fastcall TfrmRetrievalAssistant::TfrmRetrievalAssistant(TComponent* Owner) : TForm(Owner) { }

void TfrmRetrievalAssistant::init() {
    loadJobs();
}

void TfrmRetrievalAssistant::loadJobs() {
    LQuery qc(LIMSDatabase::getCentralDb());

    qc.setSQL("SELECT ");

    //comboJob->AddItem(, (TObject *));
      /*
    c_retrieval_job.status = new job (0); job type = box retrieval (2) or disposal (3)


    */
}

/*
jobs are LCDbCryoJob

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
    LQuery qp = Util::projectQuery(box->project_cid, true); // true 2nd param gets ddb
    // Find where the boxes are supposed to be:

    qp.setSQL("select … from box_name n, box_store bs, c_rack_number r, c_tank_map m"
        " where n.box_cid=bs.box_cid and bs.rack_cid=r.rack_cid and r.tank_cid=m.tank_cid"
        " and bs.retrieval_cid = jobID");

    sgBoxes->RowCount = 1;

    for (int i; i < 10; i++) {
        sgBoxes->Cells[][] = "";
    }


}


// void TfrmRetrievalAssistant::() { }
