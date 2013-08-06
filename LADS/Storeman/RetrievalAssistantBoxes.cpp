#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistantBoxes.h"
#include "ReferredBoxes.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmBoxes *frmBoxes;

void debugLog(String s) { frmBoxes->memoDebug->Lines->Add(s); }

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

__fastcall TfrmBoxes::TfrmBoxes(TComponent* Owner) : TForm(Owner) { }

void __fastcall TfrmBoxes::FormCreate(TObject *Sender) {
    cbLog->Visible = MYDEBUG;
    memoDebug->Visible = MYDEBUG;
    job = NULL;
    sgChunks->Cells[SGCHUNKS_COL_SECTION]   [0] = "Section";
    sgChunks->Cells[SGCHUNKS_COL_START]     [0] = "Start";
    sgChunks->Cells[SGCHUNKS_COL_END]       [0] = "End";
    sgChunks->Cells[SGCHUNKS_COL_SIZE]      [0] = "Size";
    sgChunks->ColWidths[SGCHUNKS_COL_SECTION]   = 100;
    sgChunks->ColWidths[SGCHUNKS_COL_START]     = 100;
    sgChunks->ColWidths[SGCHUNKS_COL_END]       = 100;
    sgChunks->ColWidths[SGCHUNKS_COL_SIZE]      = 100;
    showChunks();
    radbutDefault->Caption = DEFAULT_NUMROWS;
}

void __fastcall TfrmBoxes::FormShow(TObject *Sender) {
    // show job: list of boxes or cryovials
    std::ostringstream oss;
    oss //<< (autochunk ? "auto-chunk" : "manual chunk") << ", "
    << ((job->getJobType() == LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL) ? "SAMPLE_RETRIEVAL;" : "!SAMPLE_RETRIEVAL");
    debugLog(oss.str().c_str()); //;
    btnSave->Enabled = true;
}

void __fastcall TfrmBoxes::btnCancelClick(TObject *Sender) { Close(); }

void __fastcall TfrmBoxes::sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
/*
#define RETRIEVAL_ASSISTANT_HIGHLIGHT_COLOUR    clActiveCaption
#define RETRIEVAL_ASSISTANT_NEW_JOB_COLOUR      clMoneyGreen
#define RETRIEVAL_ASSISTANT_IN_PROGRESS_COLOUR  clLime
#define RETRIEVAL_ASSISTANT_DONE_COLOUR         clSkyBlue
#define RETRIEVAL_ASSISTANT_ERROR_COLOUR        clRed
#define RETRIEVAL_ASSISTANT_DELETED_COLOUR      clGray*/
    TColor background = clWindow;
    if (0 == ARow) {
        background = clBtnFace;
    } else {
        Chunk * chunk = NULL;
        chunk = (Chunk *)sgChunks->Objects[0][ARow];
        if (NULL == chunk)
            background = RETRIEVAL_ASSISTANT_ERROR_COLOUR;
        else
            background = RETRIEVAL_ASSISTANT_ERROR_COLOUR;
        //else if (chunk->
    }
    TCanvas * cnv = sgChunks->Canvas;
	cnv->Brush->Color = background;
	cnv->FillRect(Rect);
    if (State.Contains(gdSelected)) {
        TFontStyles oldFontStyle = cnv->Font->Style;
        TPenStyle oldPenStyle = cnv->Pen->Style;
        cnv->Pen->Style = psDot;
        cnv->Rectangle(Rect.Left+1, Rect.Top+1, Rect.Right-1, Rect.Bottom-1);
        cnv->Font->Style = TFontStyles() << fsBold;
    	cnv->TextOut(Rect.Left+5, Rect.Top+5, sgChunks->Cells[ACol][ARow]);
        cnv->Pen->Style     = oldPenStyle;
        cnv->Font->Style    = oldFontStyle;
	} else {
        cnv->TextOut(Rect.Left+5, Rect.Top+5, sgChunks->Cells[ACol][ARow]);
    }
}

void __fastcall TfrmBoxes::btnSaveClick(TObject *Sender) {
    //
    btnSave->Enabled = false;
    // insert rows into c_box_retrieval and l_cryovial_retrieval
}

void __fastcall TfrmBoxes::btnAddChunkClick(TObject *Sender) {
    Chunk * chunk = new Chunk;
    chunk->section = chunks.size() + 1;
    chunks.push_back(chunk);
    if (chunks.size() > 0) btnDelChunk->Enabled = true;
    showChunks();
}

void __fastcall TfrmBoxes::cbLogClick(TObject *Sender) {
    memoDebug->Visible = cbLog->Checked;
}

void __fastcall TfrmBoxes::btnDelChunkClick(TObject *Sender) {
    if (MYDEBUG || IDYES == Application->MessageBox(L"Are you sure you want to delete the last chunk?", L"Question", MB_YESNO)) {
        delete chunks.back();
        chunks.pop_back();
        showChunks();
    }
    if (chunks.size() == 0) btnDelChunk->Enabled = false;
}

void __fastcall TfrmBoxes::radbutDefaultClick(TObject *Sender) { radgrpRowsChange(); }
void __fastcall TfrmBoxes::radbutAllClick(TObject *Sender) { radgrpRowsChange(); }
void __fastcall TfrmBoxes::radbutCustomClick(TObject *Sender) { radgrpRowsChange(); }

//void TfrmRetrievalManager::loadChunks() {
//    ostringstream oss; oss<<__FUNC__<<": var: "; debugLog(oss.str().c_str());
//    Screen->Cursor = crSQLWait;
//    LQuery q(LIMSDatabase::getCentralDb());
//    delete_referenced<vecpChunk>(chunks);
//    q.setSQL("SELECT * FROM c_retrieval_plan_chunk WHERE status != 99");
//    q.open();
//    while (!q.eof()) {
//        Chunk * chunk = new Chunk();
//        //Chunk chunk;
//        //chunk-> = q.readInt("");
//        //chunk-> = q.readString("");
//        chunks.push_back(chunk);
//        q.next();
//    }
//    showChunks();
//    Screen->Cursor = crDefault;
//}

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

void TfrmBoxes::showChunks() {
    sgChunks->RowCount = chunks.size() + 1;
    //sgChunks->FixedRows = 1;
    vecpChunk::const_iterator it;
    int row = 1;
    for (it = chunks.begin(); it != chunks.end(); it++, row++) {
        Chunk * chunk = *it;
        sgChunks->Cells[SGCHUNKS_COL_SECTION]   [row] = chunk->section;
        sgChunks->Cells[SGCHUNKS_COL_START]     [row] = chunk->start;
        sgChunks->Cells[SGCHUNKS_COL_END]       [row] = chunk->end;
        sgChunks->Cells[SGCHUNKS_COL_SIZE]      [row] = chunk->end - chunk->start;
        sgChunks->Objects[0][row] = (TObject *)chunk;
    }
}

/*
    c_retrieval_job.status = new job (0); job type = box retrieval (2) or disposal (3)

Find where the boxes are supposed to be:
    Select … from box_name n, box_store bs, c_rack_number r, c_tank_map m
    where n.box_cid=bs.box_cid and bs.rack_cid=r.rack_cid and r.tank_cid=m.tank_cid
    and bs.retrieval_cid = jobID;
*/

void TfrmBoxes::autoChunk() {
    // not for boxes
}

void __fastcall TfrmBoxes::sgChunksSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value) {
    // user changed text
    if (0 == ARow) {
        // header - prevent editing somehow
        return;
    }
    switch (ACol) {
    case SGCHUNKS_COL_SECTION:
        break;
    case SGCHUNKS_COL_START:
        break;
    case SGCHUNKS_COL_END:
        break;
    case SGCHUNKS_COL_SIZE:
        break;
    default:
        break;
    }
}

void __fastcall TfrmBoxes::sgChunksFixedCellClick(TObject *Sender, int ACol, int ARow) {
    // prevent editing
}

/*

LPDbCryovialStore::Status { ALLOCATED, CONFIRMED, MOVE_EXPECTED, DESTROYED, ANALYSED, TRANSFERRED, DELETED = 99 };
LPDbCryovial::Status { EXPECTED  = 0, STORED = 1, SPLIT = 2, DESTROYED = 3, DELETED = 99 };

What is the difference between cryovial_store/LPDbCryovialStore and cryovial/LPDbCryovial?
cryovial/LPDbCryovial is one record per cryovial.
cryovial_store/LPDbCryovialStore can have many records per cryovial, detailing the storage history.

retrieve 4000 THRIVE samples:

c_retrieval_job:
record -1015, job type 4 [SAMPLE_RETRIEVAL], status 0 [NEW_JOB], primary aliquot EDTA_2, no secondary aliquot.
  (where is secondary aliquot defined? primary aliquot is a field of c_retrieval_job)
The retrieval assistant might split this into (say) eight chunks of five boxes in c_box_retrieval with cryovials in l_cryovial_retrieval.
It would set c_retrieval_job.status to 1 and the start date to 'now'.
The operator would retrieve a chunk at a time, ticked off in in c_box_retrieval and l_cryovial_retrieval.
When that's finished, it would set c_retrieval_job.status to 2 and finish date to 'now'.

cryovial_store:
the 4000-odd records saying where the cryovials are at the moment have retrieval_cid -1015, status = 2, removed = ''.
Set status = 5, removed = 'now' when they're retrieved.
Each cryovial has a second cryovial_store record giving the expected destination, status = 0.
Set status = 1 when the position's confirmed

*/

void TfrmBoxes::loadRows(int numrows) {
    ostringstream oss;
    oss<<__FUNC__<<": numrows: "<<numrows;
    debugLog(oss.str().c_str());
//    //LQuery qc(LIMSDatabase::getCentralDb());
//    LQuery q(Util::projectQuery(project), true);
//    //qp.setSQL("SELECT br.box_id FROM c_box_retrieval br WHERE br.retrieval_cid = :rtid AND br.section = :sect AND status != 99");
//    // no 'chunks' yet, we haven't created them!
//    // they will exist in c_box_retrieval, but don't already exist in cryovial_store where the job comes from
//    q.setSQL("SELECT * FROM c_retrieval_job rj, cryovial_store cs WHERE rj.retrieval_cid = cs.retrieval_cid ORDER BY cs.box_cid");
//    Screen->Cursor = crSQLWait;
//    q.open();
//    delete_referenced<vecpBox>(boxes);
//    while (!q.eof()) {
//        LCDbBoxStore * box = new LCDbBoxStore();
//        box->set = q.readInt("");
//        ob-> = q.readString("");
//        boxes.push_back(box);
//        q.next();
//    }
//    Screen->Cursor = crDefault;
//
//    int row = 1;
//    vecpOb::const_iterator it;
//    for (it = .begin(); it != .end(); it++) {
//        LCDbBoxStore * box = *it;
//        sgObs->Cells[SGOBJS_COL_1][row] = ob->;
//        sgObs->Objects[0][row] = (TObject *)ob;
//    }

}

void __fastcall TfrmBoxes::editCustomRowsChange(TObject *Sender) {
    timerCustomRows->Enabled = false; // reset
    timerCustomRows->Enabled = true;;
}

void __fastcall TfrmBoxes::timerCustomRowsTimer(TObject *Sender) {
    timerCustomRows->Enabled = false;
    int numrows = editCustomRows->Text.ToIntDef(0);
    ostringstream oss;
    oss <<__FUNC__<<": load"<<": numrows: "<<numrows;
    debugLog(oss.str().c_str());
    loadRows(numrows);
}

void TfrmBoxes::radgrpRowsChange() {
    int numrows = 0;
    if (radbutCustom->Checked) {
        editCustomRows->Enabled = true;
        return; // allow user to edit value
    } else {
        editCustomRows->Enabled = false;
        if (radbutDefault->Checked) {
            numrows = DEFAULT_NUMROWS;
        } else if (radbutAll->Checked) {
            numrows = -1;
        }
    }
    ostringstream oss;
    oss <<__FUNC__<<": numrows: "<<numrows;
    debugLog(oss.str().c_str());
    loadRows(numrows);
}

void __fastcall TfrmBoxes::btnDecrClick(TObject *Sender) {
    //
}
void __fastcall TfrmBoxes::btnIncrClick(TObject *Sender) {    //
}

//void __fastcall TfrmRetrievalManager::btnDeletePlanClick(TObject *Sender) {
///*
//    ostringstream oss; oss<<__FUNC__<<": var: "<<var;
//    debugLog(oss.str().c_str());
//
//    LQuery q(LIMSDatabase::getCentralDb());
//    LQuery q(Util::projectQuery(project), true); // get ddb with central and project dbs
//    q.setSQL("SELECT * FROM obs WHERE ");
//    Screen->Cursor = crSQLWait;
//    q.open();
//    delete_referenced<vecpOb>(obs);
//    while (!q.eof()) {
//        Ob * ob = new Ob();
//        ob-> = q.readInt("");
//        ob-> = q.readString("");
//        obs.push_back(ob);
//        q.next();
//    }
//    Screen->Cursor = crDefault;
//
//    int row = 1;
//    vecpOb::const_iterator it;
//    for (it = .begin(); it != .end(); it++) {
//        Ob * ob = *it;
//        sgObs->Cells[SGOBJS_COL_1][row] = ob->;
//        sgObs->Objects[0][row] = (TObject *)ob;
//    }
//*/
//}
//void TfrmRetrievalManager::loadPlans() {
//    ostringstream oss; oss<<__FUNC__; debugLog(oss.str().c_str());
//    Screen->Cursor = crSQLWait;
//    LQuery q(LIMSDatabase::getCentralDb());
//    q.setSQL("SELECT * FROM c_retrieval_plan WHERE status != 99");
//    //q.setParam("name", bcsToStd(comboPlan->Text));
//    q.open();
//    delete_referenced<vecpRetrievalPlan>(plans);
//    while (!q.eof()) {
//        RetrievalPlan * plan = new RetrievalPlan(q.readString("name"));
//        //ob-> = q.readInt("");
//        //ob-> = q.readString("");
//        plans.push_back(plan);
//        q.next();
//    }
//    showPlans();
//    Screen->Cursor = crDefault;
//}
//void TfrmRetrievalManager::showPlans() {
//    int row = 1;
//    comboPlans->Clear();
//    vecpRetrievalPlan::const_iterator it;
//    for (it = plans.begin(); it != plans.end(); it++, row++) {
//        RetrievalPlan * plan = *it;
//        comboPlans->AddItem(plan->getName().c_str(), (TObject *)plan);
//    }
//}
//void __fastcall TfrmRetrievalManager::btnPlanSaveClick(TObject *Sender) {
//    ostringstream oss; oss<<__FUNC__; debugLog(oss.str().c_str());
//    Screen->Cursor = crSQLWait;
//    LQuery q(LIMSDatabase::getCentralDb());
//    RetrievalPlan * plan = (RetrievalPlan *)comboPlans->Items->Objects[comboPlans->ItemIndex];
//    q.setSQL("SELECT * FROM c_retrieval_plan WHERE name = :name");
//    q.setParam("name", bcsToStd(comboPlans->Text));
//    if (q.open()) { // if plan name exists, replace it
//        q.setSQL(   "UPDATE c_retrieval_plan SET "
//                    " WHERE name = :name");
//    } else {
//        plan->claimNextID(q); //
//        plan->getID();
//        q.setSQL(   "INSERT INTO c_retrieval_plan (,,,) "
//                    " VALUES (name = :name, ) ");
//    }
//    q.setParam("name", bcsToStd(comboPlans->Text));
//    q.execSQL();
//    Screen->Cursor = crDefault;
//}
//void __fastcall TfrmRetrievalManager::comboPlansSelect(TObject *Sender) {
//    //
//}
//
//void __fastcall TfrmRetrievalManager::comboPlansChange(TObject *Sender) {
//    //
//}
