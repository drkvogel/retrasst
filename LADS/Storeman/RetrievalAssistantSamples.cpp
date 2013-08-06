#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistantSamples.h"
//#include "ReferredBoxes.h"
    // for MYDEBUG - should move somewhere else
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSamples *frmSamples;

void debugLog(String s) { frmSamples->memoDebug->Lines->Add(s); }

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

__fastcall TfrmSamples::TfrmSamples(TComponent* Owner) : TForm(Owner) { }


void __fastcall TfrmSamples::FormCreate(TObject *Sender) {
    //
    cbLog->Visible = RETRASSTDEBUG;
    memoDebug->Visible = RETRASSTDEBUG;
    autochunk = false;
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

void __fastcall TfrmSamples::FormShow(TObject *Sender) {
    // show job: list of cryovials
    std::ostringstream oss;
    oss << (autochunk ? "auto-chunk" : "manual chunk") << ", "
    << ((job->getJobType() == LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL) ? "SAMPLE_RETRIEVAL;" : "!SAMPLE_RETRIEVAL");
    debugLog(oss.str().c_str()); //;
    btnSave->Enabled = true;
}

void __fastcall TfrmSamples::btnCancelClick(TObject *Sender) { Close(); }

void __fastcall TfrmSamples::btnSaveClick(TObject *Sender) {
    btnSave->Enabled = false;
    // insert rows into c_box_retrieval and l_cryovial_retrieval?
}

void __fastcall TfrmSamples::btnAddChunkClick(TObject *Sender) {
    Chunk * chunk = new Chunk;
    chunk->section = chunks.size() + 1;
    chunks.push_back(chunk);
    if (chunks.size() > 0) btnDelChunk->Enabled = true;
    showChunks();
}

void __fastcall TfrmSamples::cbLogClick(TObject *Sender) {
    memoDebug->Visible = cbLog->Checked;
}

void __fastcall TfrmSamples::btnDelChunkClick(TObject *Sender) {
    if (RETRASSTDEBUG || IDYES == Application->MessageBox(L"Are you sure you want to delete the last chunk?", L"Question", MB_YESNO)) {
        delete chunks.back();
        chunks.pop_back();
        showChunks();
    }
    if (chunks.size() == 0) btnDelChunk->Enabled = false;
}

void __fastcall TfrmSamples::radbutDefaultClick(TObject *Sender) { radgrpRowsChange(); }
void __fastcall TfrmSamples::radbutAllClick(TObject *Sender) { radgrpRowsChange(); }
void __fastcall TfrmSamples::radbutCustomClick(TObject *Sender) { radgrpRowsChange(); }

void TfrmSamples::radgrpRowsChange() {
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
    std::ostringstream oss;
    oss <<__FUNC__<<": numrows: "<<numrows;
    debugLog(oss.str().c_str());
    loadRows(numrows);
}

void __fastcall TfrmSamples::timerCustomRowsTimer(TObject *Sender) {
    timerCustomRows->Enabled = false;
    int numrows = editCustomRows->Text.ToIntDef(0);
    std::ostringstream oss;
    oss <<__FUNC__<<": load"<<": numrows: "<<numrows;
    debugLog(oss.str().c_str());
    loadRows(numrows);
}

void __fastcall TfrmSamples::editCustomRowsChange(TObject *Sender) {
    timerCustomRows->Enabled = false; // reset
    timerCustomRows->Enabled = true;;
}

void __fastcall TfrmSamples::sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
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

void TfrmSamples::showChunks() {
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

void TfrmSamples::autoChunk() {
/*

box_content.box_type_cid

18  EDTA_1(UK)  HPS2-THRIVE EDTA 1 UK samples

c_box_size.box_type_cid


Display the size of the job and ask user if they want to divide up the list.  If they do:

1.	Ask them the maximum section size (default = 500 cryovials)
2.	Calculate slot/box (where `c_box_size.box_size_cid = box_content.box_size_cid`)
3.	Ask them to select the size of first section from a list – it must be a multiple of the box size (from 2) and no more than the maximum (from 1)
4.	Allocate the appropriate number of destination boxes to the first section
5.	Repeat steps (2) and (3) until every entry has been allocated to a section
*/
}

void TfrmSamples::loadRows(int numrows) {
    std::ostringstream oss;
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


