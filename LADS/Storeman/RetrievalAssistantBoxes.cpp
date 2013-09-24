#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistantBoxes.h"
#include "ReferredBoxes.h"
#include "StoreUtil.h"
#include "LQuery.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmBoxes *frmBoxes;

Sorter<BoxRow> boxSorter[SGBOXES_NUMCOLS] = {
    { BoxRow::sort_asc_currbox,   sgBoxesColName[0] },
//        { BoxRow::sort_asc_destbox,   sgBoxesColName[1] },
//        { BoxRow::sort_asc_destpos,   sgBoxesColName[2] },
    { BoxRow::sort_asc_site,      sgBoxesColName[1] },
    { BoxRow::sort_asc_position,  sgBoxesColName[2] },
    { BoxRow::sort_asc_shelf,     sgBoxesColName[3] },
    { BoxRow::sort_asc_vessel,    sgBoxesColName[4] },
    { BoxRow::sort_asc_structure, sgBoxesColName[5] },
    { BoxRow::sort_asc_slot,      sgBoxesColName[6] },
};

__fastcall LoadBoxesWorkerThread::LoadBoxesWorkerThread() {
    FreeOnTerminate = true;
}

void __fastcall LoadBoxesWorkerThread::updateStatus() {
    ostringstream oss; oss<<frmBoxes->loadingMessage<<"\n"<<rowCount<<" boxes";//<<numerator<<" of "<<denominator;
    frmBoxes->panelLoading->Caption = oss.str().c_str();
    frmBoxes->panelLoading->Repaint();
}

void __fastcall LoadBoxesWorkerThread::Execute() {
/*  c_retrieval_job.status = new job (0); job type = box retrieval (2) or disposal (3)

Find where the boxes are supposed to be:
    Select ... from box_name n, box_store bs, c_rack_number r, c_tank_map m
    where n.box_cid=bs.box_cid and bs.rack_cid=r.rack_cid and r.tank_cid=m.tank_cid
    and bs.retrieval_cid = jobID; */

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
Set status = 1 when the position's confirmed */
    delete_referenced<vecpBoxRow>(frmBoxes->boxes);
    LQuery q(Util::projectQuery(frmBoxes->job->getProjectID(), true)); // get ddb
    q.setSQL(
        "SELECT"
        "   bs.box_cid, bs.rack_cid, b.status, bs.process_cid,"
        "   b.external_name as box, s.external_name as site, m.position,"
        "   v.external_full as vessel, m.shelf_number, r.external_name as rack,"
        "   bs.slot_position "
        " FROM"
        "   box_name b, box_store bs, c_rack_number r, c_tank_map m, c_object_name s, c_object_name v"
        " WHERE"
        "   b.box_cid = bs.box_cid AND"
        "   bs.rack_cid = r.rack_cid AND"
        "   r.tank_cid = m.tank_cid AND"
        "   s.object_cid = location_cid AND"
        "   v.object_cid = storage_cid AND"
        "   bs.retrieval_cid = :jobID"); // e.g. -636363
    q.setParam("jobID", frmBoxes->job->getID());
    q.open();
    while (!q.eof()) {
        if (rowCount > 0 && 0 == rowCount % 10) Synchronize((TThreadMethod)&(this->updateStatus)); // seems to cause thread to terminate
        LCDbBoxStore * store = new LCDbBoxStore(q); // ???
        BoxRow * box = new BoxRow(
            store,
            q.readString("box"),
            q.readString("site"),
            q.readInt("position"),
            q.readString("vessel"),
            q.readInt("shelf_number"),
            q.readString("rack"),
            q.readInt("slot_position")
        );
        frmBoxes->boxes.push_back(box);
        q.next();
        rowCount++;
    }
}

__fastcall TfrmBoxes::TfrmBoxes(TComponent* Owner) : TForm(Owner) { }

void __fastcall TfrmBoxes::FormCreate(TObject *Sender) {
    cbLog->Visible      = RETRASSTDEBUG;
    job                 = NULL;
    sgwChunks = new StringGridWrapper<BoxChunk>(sgChunks, &chunks);
    sgwBoxes  = new StringGridWrapper<BoxRow>(sgBoxes, &boxes);
    sgwChunks->init();
    sgwBoxes->init();
    loadingMessage = "Loading boxes, please wait...";
}

void __fastcall TfrmBoxes::FormClose(TObject *Sender, TCloseAction &Action) {
    delete_referenced<vecpBoxRow>(boxes);
    delete_referenced<vecpBoxChunk>(chunks);
}

VOID CALLBACK TfrmBoxes::TimerProc(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime) { //cout << "CALLBACK " << dwTime << '\n'; cout.flush();
    KillTimer(NULL, frmBoxes->TimerId);
    msgbox("Win Timer finished");
    frmBoxes->loadRows();
}

void __fastcall TfrmBoxes::FormShow(TObject *Sender) {
    std::ostringstream oss; oss << ((job->getJobType() == LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL) ? "SAMPLE_RETRIEVAL;" : "!SAMPLE_RETRIEVAL"); debugLog(oss.str().c_str()); //;
    btnSave->Enabled = true;
    //clearSG(sgChunks);
    //clearSG(sgBoxes);
    sgwChunks->clear();
    sgwBoxes->clear();

    timerLoadBoxes->Enabled = true; // "not enough timers are available" - misleading error message cause by mem corruption
        //TfrmBoxes::TimerId = SetTimer(NULL, 0, 50, &TimerProc); //$3: milliseconds // tried WinAPI timer instead
}

void __fastcall TfrmBoxes::btnAddChunkClick(TObject *Sender) { addChunk(); }

void __fastcall TfrmBoxes::cbLogClick(TObject *Sender) { memoDebug->Visible = cbLog->Checked; }

void __fastcall TfrmBoxes::btnCancelClick(TObject *Sender) { Close(); }

void __fastcall TfrmBoxes::sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
    TColor background = clWindow;
    if (0 == ARow) {
        background = clBtnFace;
    } else {
        Chunk * chunk = NULL;
        chunk = (Chunk *)sgChunks->Objects[0][ARow];
        if (NULL == chunk) {
            background = clWindow; //RETRIEVAL_ASSISTANT_ERROR_COLOUR;
        } else {
            background = RETRIEVAL_ASSISTANT_DONE_COLOUR; //RETRIEVAL_ASSISTANT_ERROR_COLOUR;
        }
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

void __fastcall TfrmBoxes::sgBoxesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
    //          clWindow
    TColor background = clWindow;
    if (0 == ARow) {
        background = clBtnFace;
    } else {
        Chunk * chunk = NULL;
        chunk = (Chunk *)sgBoxes->Objects[0][ARow];
        if (NULL == chunk) {
            background = clWindow; //RETRIEVAL_ASSISTANT_ERROR_COLOUR;
        } else {
            background = RETRIEVAL_ASSISTANT_DONE_COLOUR; //RETRIEVAL_ASSISTANT_ERROR_COLOUR;
        }
    }
    TCanvas * cnv = sgBoxes->Canvas;
	cnv->Brush->Color = background;
	cnv->FillRect(Rect);
    if (State.Contains(gdSelected)) {
        TFontStyles oldFontStyle = cnv->Font->Style;
        TPenStyle oldPenStyle = cnv->Pen->Style;
        cnv->Pen->Style = psDot;
        cnv->Rectangle(Rect.Left+1, Rect.Top+1, Rect.Right-1, Rect.Bottom-1);
        cnv->Font->Style = TFontStyles() << fsBold;
    	cnv->TextOut(Rect.Left+5, Rect.Top+5, sgBoxes->Cells[ACol][ARow]);
        cnv->Pen->Style     = oldPenStyle;
        cnv->Font->Style    = oldFontStyle;
	} else {
        cnv->TextOut(Rect.Left+5, Rect.Top+5, sgBoxes->Cells[ACol][ARow]);
    }

}

void __fastcall TfrmBoxes::btnSaveClick(TObject *Sender) {
    if (IDYES == Application->MessageBox(L"Save changes? Press 'No' to go back and re-order", L"Question", MB_YESNO)) {
        // sign off?
        for (vecpBoxChunk::const_iterator it = chunks.begin(); it != chunks.end(); it++) { // for chunks
            BoxChunk * chunk = *it;
            //      for boxes
            //          insert box into C_BOX_RETRIEVAL with current section (chunk) number
            for (vecpBoxRow::const_iterator it = chunk->rows.begin(); it != chunk->rows.end(); it++) { // vecpDataRow?
                pBoxRow boxRow = (pBoxRow)*it;
                //LPDbCryovialStore * vial = sampleRow->store_record;
            }
            /*
            retrieval_cid	 i4		c_retrieval_job	 The retrieval task this entry is part of
            retrieval_type	 i2			obsolete - see c_retrieval_job
            box_id	 i4		box_name	 The box being retrieved (for box retrieval/disposal) or retrieved into (for sample retrieval/disposal)
            section	 i2			 Which chunk of the retrieval plan this entry belongs to (0 = retrieve all boxes in parallel)
            position	 i2			obsolete
            box_name	 v32			obsolete
            rj_box_cid	 i4	 1		 Unique ID for this retrieval list entry (also determines retrieval order for box retrievals)
            status	 i2			 0: new record; 1: part-filled, 2: collected; 3: not found; 99: record deleted
            time_stamp	 d/t			 When this record was inserted or updated*/
        }
        return;
        // update c_retrieval_job (in progress)
        job->setStatus(LCDbCryoJob::INPROGRESS);
        job->saveRecord(LIMSDatabase::getCentralDb());
        btnSave->Enabled = false;
        Close();
    } else { // start again
        chunks.clear(); // delete memory
        addChunk();
    }
}

void __fastcall TfrmBoxes::btnDelChunkClick(TObject *Sender) {
    // move last chunk's samples into preceding chunk
    if (RETRASSTDEBUG || IDYES == Application->MessageBox(L"Are you sure you want to delete the last chunk?", L"Question", MB_YESNO)) {
        delete chunks.back();
        chunks.pop_back();
        showChunks();
    }
    if (chunks.size() == 0) btnDelChunk->Enabled = false;
}

void __fastcall TfrmBoxes::btnDecrClick(TObject *Sender) {
    //
}
void __fastcall TfrmBoxes::btnIncrClick(TObject *Sender) {    //
}

void __fastcall TfrmBoxes::sgChunksSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value) {
    if (0 == ARow) { // header - prevent editing somehow
        return;
    }
    switch (ACol) {
    case SGCHUNKS_SECTION:  break;
    case SGCHUNKS_START:    break;
    case SGCHUNKS_END:      break;
    case SGCHUNKS_SIZE:     break;
    default:                break;
    }
}

void __fastcall TfrmBoxes::sgChunksFixedCellClick(TObject *Sender, int ACol, int ARow) {
    // prevent editing
}

void __fastcall TfrmBoxes::sgBoxesFixedCellClick(TObject *Sender, int ACol, int ARow) {
    sortList(ACol);
}

void __fastcall TfrmBoxes::timerLoadBoxesTimer(TObject *Sender) {
    timerLoadBoxes->Enabled = false;
    loadRows();
}

void __fastcall TfrmBoxes::sgBoxesClick(TObject *Sender) {
    BoxRow*box=(BoxRow*)sgBoxes->Objects[0][sgBoxes->Row];
    box?debugLog(box->str().c_str()):debugLog("NULL box");
    job?debugLog(job->getName().c_str()):debugLog("NULL job");
    debugLog(sgwBoxes->printColWidths().c_str());
}

void __fastcall TfrmBoxes::loadBoxesWorkerThreadTerminated(TObject *Sender) {
    showRows(); // must do this outside thread, unless synchronised - does gui stuff
    progressBottom->Style = pbstNormal; progressBottom->Visible = false;
    panelLoading->Visible = false;
    chunks.clear();
    sgwChunks->clear();
    addChunk(); // create a default chunk now list is loaded
    showChunks();
    Screen->Cursor = crDefault;
}

void __fastcall TfrmBoxes::btnRejectClick(TObject *Sender) {
    if (IDYES == Application->MessageBox(L"Are you sure you want to reject this list?", L"Question", MB_YESNO)) {
        //xxxxrejectList();
        job->setStatus(LCDbCryoJob::Status::REJECTED);
        job->saveRecord(LIMSDatabase::getCentralDb());
        Close();
    }
}

void __fastcall TfrmBoxes::btnAutoChunkClick(TObject *Sender) {
    autoChunk();
}

void TfrmBoxes::debugLog(String s) { frmBoxes->memoDebug->Lines->Add(s); }

void TfrmBoxes::addChunk() {
    if (chunks.size() == 0) {
        // first chunk, make default chunk from entire list
    }
    BoxChunk * chunk = new BoxChunk;
    chunk->section = chunks.size() + 1;
    chunks.push_back(chunk);
    btnDelChunk->Enabled = true;
    showChunks();
}

void TfrmBoxes::showChunks() {
    sgChunks->RowCount = chunks.size() + 1;
    int row = 1;
    for (vecpBoxChunk::const_iterator it = chunks.begin(); it != chunks.end(); it++, row++) {
        Chunk * chunk = *it;
        sgChunks->Cells[SGCHUNKS_SECTION]   [row] = chunk->section;
        sgChunks->Cells[SGCHUNKS_START]     [row] = chunk->start.c_str();
        sgChunks->Cells[SGCHUNKS_END]       [row] = chunk->end.c_str();
        sgChunks->Cells[SGCHUNKS_SIZE]      [row] = 0;//chunk->end - chunk->start;
        sgChunks->Objects[0][row] = (TObject *)chunk;
    }
}

void TfrmBoxes::loadRows() {
    //std::ostringstream oss; oss<<__FUNC__<<": numrows: "<<maxRows; debugLog(oss.str().c_str());
    panelLoading->Caption = loadingMessage;
    panelLoading->Visible = true;
    panelLoading->Top = (sgBoxes->Height / 2) - (panelLoading->Height / 2);
    panelLoading->Left = (sgBoxes->Width / 2) - (panelLoading->Width / 2);
    progressBottom->Style = pbstMarquee; progressBottom->Visible = true;
    Screen->Cursor = crSQLWait; // Screen-> // disable mouse?
    loadBoxesWorkerThread = new LoadBoxesWorkerThread();
    loadBoxesWorkerThread->OnTerminate = &loadBoxesWorkerThreadTerminated;
}

void TfrmBoxes::showRows() {
    if (boxes.size() <= 0) {
        sgwBoxes->clear();
    } else {
        sgBoxes->RowCount = boxes.size() + 1;
        sgBoxes->FixedRows = 1;
    }
    int row = 1;
    vecpBoxRow::const_iterator it;
    for (it = boxes.begin(); it != boxes.end(); it++, row++) {
        BoxRow * box = *it;
        sgBoxes->Cells[SGBOXES_BOXNAME]  [row] = box->box_name.c_str();
        sgBoxes->Cells[SGBOXES_SITE]     [row] = box->site_name.c_str();
        sgBoxes->Cells[SGBOXES_POSITION] [row] = box->position;
        sgBoxes->Cells[SGBOXES_SHELF]    [row] = box->shelf_number;
        sgBoxes->Cells[SGBOXES_VESSEL]   [row] = box->vessel_name.c_str();
        sgBoxes->Cells[SGBOXES_STRUCTURE][row] = box->rack_name.c_str();
        sgBoxes->Cells[SGBOXES_SLOT]     [row] = box->slot_position;
        sgBoxes->Objects[0][row] = (TObject *)box;
    }
}

void TfrmBoxes::sortList(int col) {
    Screen->Cursor = crSQLWait;
    boxSorter[col].sort_toggle(boxes);
    //boxSorter[col].sort(boxes, Sorter::SortOrder::TOGGLE);
    showRows();
    Screen->Cursor = crDefault;
}

void TfrmBoxes::autoChunk() {
    //
}
