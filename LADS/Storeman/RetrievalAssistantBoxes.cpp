#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistantBoxes.h"
#include "ReferredBoxes.h"
#include "StoreUtil.h"
#include "LQuery.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmBoxes *frmBoxes;

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

enum {SGBOXES_COL_1, SGBOXES_COL_2, SGBOXES_COL_3, SGBOXES_COL_4, SGBOXES_COL_5} sg_boxes_cols;

void TfrmBoxes::debugLog(String s) { frmBoxes->memoDebug->Lines->Add(s); }

__fastcall TfrmBoxes::TfrmBoxes(TComponent* Owner) : TForm(Owner) { }

void __fastcall TfrmBoxes::FormCreate(TObject *Sender) {
    cbLog->Visible      = RETRASSTDEBUG;
    memoDebug->Visible  = RETRASSTDEBUG;
    job                 = NULL;
    numrows             = DEFAULT_NUMROWS;
    setupStringGrid(sgChunks, SGCHUNKS_NUMCOLS, sgChunksColName, sgChunksColWidth);

//    sgChunks->Cells[SGCHUNKS_SECTION]   [0] = "Section";
//    sgChunks->Cells[SGCHUNKS_START]     [0] = "Start";
//    sgChunks->Cells[SGCHUNKS_END]       [0] = "End";
//    sgChunks->Cells[SGCHUNKS_SIZE]      [0] = "Size";
//    sgChunks->ColWidths[SGCHUNKS_COL_SECTION]   = 100;
//    sgChunks->ColWidths[SGCHUNKS_COL_START]     = 100;
//    sgChunks->ColWidths[SGCHUNKS_COL_END]       = 100;
//    sgChunks->ColWidths[SGCHUNKS_COL_SIZE]      = 100;
    radbutDefault->Caption = DEFAULT_NUMROWS;
}

void __fastcall TfrmBoxes::FormShow(TObject *Sender) {
    std::ostringstream oss;
    oss << ((job->getJobType() == LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL) ? "SAMPLE_RETRIEVAL;" : "!SAMPLE_RETRIEVAL");
    debugLog(oss.str().c_str()); //;
    btnSave->Enabled = true;
    //showChunks();
    loadRows();
    //showRows();
}

void __fastcall TfrmBoxes::btnCancelClick(TObject *Sender) { Close(); }

void __fastcall TfrmBoxes::sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {

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
    //btnSave->Enabled = false;
    // TODO insert rows into c_box_retrieval
    // TODO update c_retrieval_job (in progress)
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
    // user changed text
    if (0 == ARow) {
        // header - prevent editing somehow
        return;
    }
    switch (ACol) {
    case SGCHUNKS_SECTION:
        break;
    case SGCHUNKS_START:
        break;
    case SGCHUNKS_END:
        break;
    case SGCHUNKS_SIZE:
        break;
    default:
        break;
    }
}

void __fastcall TfrmBoxes::sgChunksFixedCellClick(TObject *Sender, int ACol, int ARow) {
    // prevent editing
}

void __fastcall TfrmBoxes::radbutDefaultClick(TObject *Sender) { radgrpRowsChange(); }
void __fastcall TfrmBoxes::radbutAllClick(TObject *Sender) { radgrpRowsChange(); }
void __fastcall TfrmBoxes::radbutCustomClick(TObject *Sender) { radgrpRowsChange(); }

void __fastcall TfrmBoxes::editCustomRowsChange(TObject *Sender) {
    timerCustomRows->Enabled = false; // reset
    timerCustomRows->Enabled = true;;
}

void __fastcall TfrmBoxes::timerCustomRowsTimer(TObject *Sender) {
    timerCustomRows->Enabled = false;
    numrows = editCustomRows->Text.ToIntDef(0);
    ostringstream oss;
    oss <<__FUNC__<<": load"<<": numrows: "<<numrows;
    debugLog(oss.str().c_str());
    loadRows();
}

void TfrmBoxes::radgrpRowsChange() {
    numrows = 0;
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
    loadRows();
}

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
        sgChunks->Cells[SGCHUNKS_SECTION]   [row] = chunk->section;
        sgChunks->Cells[SGCHUNKS_START]     [row] = chunk->start;
        sgChunks->Cells[SGCHUNKS_END]       [row] = chunk->end;
        sgChunks->Cells[SGCHUNKS_SIZE]      [row] = chunk->end - chunk->start;
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

void TfrmBoxes::loadRows() {
    ostringstream oss; oss<<__FUNC__<<": numrows: "<<numrows; debugLog(oss.str().c_str());
    Screen->Cursor = crSQLWait;
    LQuery q(Util::projectQuery(job->getProjectID(), true)); // get ddb
    delete_referenced<vecpBox>(boxes);
    q.setSQL("SELECT"
        " b.external_name as box, s.external_name as site, m.position,"
        " v.external_full as vessel, m.shelf_number, r.external_name as rack,"
        " bs.slot_position as slot"
        " FROM"
        " box_name b, box_store bs, c_rack_number r, c_tank_map m, c_object_name s, c_object_name v"
        " WHERE"
        " b.box_cid = bs.box_cid AND"
        " bs.rack_cid = r.rack_cid AND"
        " r.tank_cid = m.tank_cid AND"
        " s.object_cid = location_cid AND"
        " v.object_cid = storage_cid AND"
        " bs.retrieval_cid = :jobID"); // e.g. -636363
    q.setParam("jobID", job->getID());
    q.open();
    while (!q.eof()) {
        LCDbBoxStore * box = new LCDbBoxStore(q); // ???
        //box->
        //q.readString("name")
        //box-> = q.readInt("");
        //ob-> = q.readString("");
        boxes.push_back(box);
        q.next();
    }
//    //qp.setSQL("SELECT br.box_id FROM c_box_retrieval br WHERE br.retrieval_cid = :rtid AND br.section = :sect AND status != 99");
//    // no 'chunks' yet, we haven't created them!
//    // they will exist in c_box_retrieval, but don't already exist in cryovial_store where the job comes from
//    q.setSQL("SELECT * FROM c_retrieval_job rj, cryovial_store cs WHERE rj.retrieval_cid = cs.retrieval_cid ORDER BY cs.box_cid");

    Screen->Cursor = crDefault;
}

/* sample showRows():

    if (vials.size() <= 0) {
        clearSG(sgVials);
    } else {
        //sgVials->RowCount = vials.size() + 1;
        sgVials->RowCount = (-1 == numrows) ? vials.size() + 1 : numrows + 1;
        sgVials->FixedRows = 1;
    }
    int row = 1;
    vecpSampleRow::const_iterator it;
    for (it = vials.begin(); it != vials.end(); it++, row++) {
        pSampleRow sampleRow = *it;
        LPDbCryovialStore * vial = sampleRow->store_record;
        sgVials->Cells[SGVIALS_BARCODE][row] = sampleRow->cryovial_barcode.c_str();
        sgVials->Cells[SGVIALS_DESTBOX][row] = "tba"; //sampleRow->;
        sgVials->Cells[SGVIALS_DESTPOS][row] = "tba"; //sampleRow->;
        sgVials->Cells[SGVIALS_CURRBOX][row] = sampleRow->box_name.c_str();
        sgVials->Cells[SGVIALS_CURRPOS][row] = sampleRow->position;
        sgVials->Cells[SGVIALS_STRUCTURE][row] = sampleRow->vessel_name.c_str(); //??
        sgVials->Cells[SGVIALS_LOCATION][row] = sampleRow->site_name.c_str();
        sgVials->Objects[0][row] = (TObject *)sampleRow;
        if (-1 != numrows && row >= numrows) break;
    }
*/

void TfrmBoxes::showRows() {
    int row = 1;
    vecpBox::const_iterator it;
    for (it = boxes.begin(); it != boxes.end(); it++, row++) {
        LCDbBoxStore * box = *it;
        sgBoxes->Cells[SGBOXES_COL_1][row] = box->getBoxID();
        sgBoxes->Objects[0][row] = (TObject *)box;
        if (row >= numrows) break;
    }
}
