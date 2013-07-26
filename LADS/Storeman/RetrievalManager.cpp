#include <vcl.h>
#pragma hdrstop
#include "RetrievalManager.h"
#include "RetrievalAssistant.h"
#include "ReferredBoxes.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmRetrievalManager *frmRetrievalManager;

void debugLog(String s) { frmRetrievalManager->memoDebug->Lines->Add(s); }

__fastcall TfrmRetrievalManager::TfrmRetrievalManager(TComponent* Owner) : TForm(Owner) { }

#define DEFAULT_NUMROWS 25

#define SGCHUNKS_COL_SECTION    0
#define SGCHUNKS_COL_START      1
#define SGCHUNKS_COL_END        2
#define SGCHUNKS_COL_SIZE       3

void __fastcall TfrmRetrievalManager::FormCreate(TObject *Sender) {
    cbLog->Visible = MYDEBUG;
    memoDebug->Visible = MYDEBUG;

    autochunk = false;
    jobType = LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL;

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

void __fastcall TfrmRetrievalManager::FormShow(TObject *Sender) {
    // show job: list of boxes or cryovials
    std::ostringstream oss;
    oss << (autochunk ? "auto-chunk" : "manual chunk") << ", "
    << ((jobType == LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL) ? "SAMPLE_RETRIEVAL;" : "!SAMPLE_RETRIEVAL");
    debugLog(oss.str().c_str()); //;
    btnSave->Enabled = true;
}

void __fastcall TfrmRetrievalManager::sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
/*
#define RETRIEVAL_ASSISTANT_HIGHLIGHT_COLOUR    clActiveCaption
#define RETRIEVAL_ASSISTANT_NEW_JOB_COLOUR      clMoneyGreen
#define RETRIEVAL_ASSISTANT_IN_PROGRESS_COLOUR  clLime
#define RETRIEVAL_ASSISTANT_DONE_COLOUR         clSkyBlue
#define RETRIEVAL_ASSISTANT_ERROR_COLOUR        clRed
#define RETRIEVAL_ASSISTANT_DELETED_COLOUR      clGray*/
    TColor background = clWindow;

    Chunk * chunk = (Chunk *)sgChunks->Objects[0][ARow];
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

void __fastcall TfrmRetrievalManager::btnSaveClick(TObject *Sender) {
    //
    btnSave->Enabled = false;
}

void __fastcall TfrmRetrievalManager::btnCancelClick(TObject *Sender) {
    Close();
}

void __fastcall TfrmRetrievalManager::btnAddChunkClick(TObject *Sender) {
    // add chunk
    Chunk * chunk = new Chunk;
    chunk->section = chunks.size() + 1;
    chunks.push_back(chunk);
    if (chunks.size() > 0) btnDelChunk->Enabled = true;
    showChunks();
}

void __fastcall TfrmRetrievalManager::cbLogClick(TObject *Sender) {
    memoDebug->Visible = cbLog->Checked;
}

void __fastcall TfrmRetrievalManager::btnDelChunkClick(TObject *Sender) {
    if (MYDEBUG || IDYES == Application->MessageBox(L"Are you sure you want to delete the last chunk?", L"Question", MB_YESNO)) {
        //tdvecpChunk::iterator it = chunks.back();  //Chunk * chunk = *it; //delete chunk;
        delete chunks.back();
        chunks.pop_back();
        showChunks();
    }
    if (chunks.size() == 0) btnDelChunk->Enabled = false;
}

void __fastcall TfrmRetrievalManager::radbutDefaultClick(TObject *Sender) { radgrpRowsChange(); }
void __fastcall TfrmRetrievalManager::radbutAllClick(TObject *Sender) { radgrpRowsChange(); }
void __fastcall TfrmRetrievalManager::radbutCustomClick(TObject *Sender) { radgrpRowsChange(); }

void TfrmRetrievalManager::loadChunks() {
    chunks.clear();
    showChunks();
}

/*
    LQuery qc(LIMSDatabase::getCentralDb());
    LQuery qp(Util::projectQuery(project));
    qc.setSQL("");
    Screen->Cursor = crSQLWait;
    qc.open();
    delete_referenced<tdvecp...>(...);
    while (!qc.eof()) {
        .push_back(box);
        qc.next();
    }
    Screen->Cursor = crDefault;

    tdvecp::const_iterator it;
    for (it = .begin(); it != .end(); it++) {

    }
*/

void TfrmRetrievalManager::showChunks() {
    sgChunks->RowCount = chunks.size() + 1;
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


void TfrmRetrievalManager::autoChunk() {
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

void __fastcall TfrmRetrievalManager::sgChunksSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value) {
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



void __fastcall TfrmRetrievalManager::sgChunksFixedCellClick(TObject *Sender, int ACol, int ARow) {
    // prevent editing
}

void TfrmRetrievalManager::loadRows(int numrows) {
    ostringstream oss;
    oss <<__FUNC__<<": numrows: "<<numrows;
    debugLog(oss.str().c_str());
}

void __fastcall TfrmRetrievalManager::editCustomRowsChange(TObject *Sender) {
    // timer
    timerCustomRows->Enabled = false; // reset
    timerCustomRows->Enabled = true;;
}

void __fastcall TfrmRetrievalManager::timerCustomRowsTimer(TObject *Sender) {
    timerCustomRows->Enabled = false;
    int numrows = editCustomRows->Text.ToIntDef(0);
    ostringstream oss;
    oss <<__FUNC__<<": load"<<": numrows: "<<numrows;
    debugLog(oss.str().c_str());
    loadRows(numrows);
}

void TfrmRetrievalManager::radgrpRowsChange() {
    int numrows = 0;
    if (radbutCustom->Checked) {
        editCustomRows->Enabled = true;
        return; // allow user to edit value
    } else {
        editCustomRows->Enabled = false;
        if (radbutDefault->Checked) {
            // load default no of rows
            numrows = DEFAULT_NUMROWS;
        } else if (radbutAll->Checked) {
            numrows = -1;
        }
    }
    ostringstream oss;
    oss << "radgrpRowsClick: "<<__FUNC__<<": numrows: "<<numrows;
    debugLog(oss.str().c_str());
    loadRows(numrows);
}
