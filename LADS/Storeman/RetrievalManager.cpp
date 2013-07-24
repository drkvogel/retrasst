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
    oss << (autochunk ? "auto-chunk" : "manual chunk") << ", "
    << ((jobType == LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL) ? "SAMPLE_RETRIEVAL;" : "!SAMPLE_RETRIEVAL");
    Label1->Caption = oss.str().c_str();
}

void __fastcall TfrmRetrievalManager::btnCancelClick(TObject *Sender) {
    Close();
}

void TfrmRetrievalManager::autoChunk() {
/*
Display the size of the job and ask user if they want to divide up the list.  If they do:

1.	Ask them the maximum section size (default = 500 cryovials)
2.	Calculate slot/box (where `c_box_size.box_size_cid = box_content.box_size_cid`)
3.	Ask them to select the size of first section from a list – it must be a multiple of the box size (from 2) and no more than the maximum (from 1)
4.	Allocate the appropriate number of destination boxes to the first section
5.	Repeat steps (2) and (3) until every entry has been allocated to a section
*/

}
