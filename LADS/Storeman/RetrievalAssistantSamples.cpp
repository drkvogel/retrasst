#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistantSamples.h"
#include "StoreUtil.h"
//#include "ReferredBoxes.h"
    // for MYDEBUG - should move somewhere else
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSamples *frmSamples;

void TfrmSamples::debugLog(String s) {
    frmSamples->memoDebug->Lines->Add(s);
}

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

//enum {SGVIALS_COL_1, SGVIALS_COL_2, SGVIALS_COL_3, SGVIALS_COL_4, SGVIALS_COL_5, SGVIALS_NUMCOLS} sg_vials_cols;

__fastcall TfrmSamples::TfrmSamples(TComponent* Owner) : TForm(Owner) { }

void __fastcall TfrmSamples::FormCreate(TObject *Sender) {
    cbLog->Visible      = RETRASSTDEBUG;
    memoDebug->Visible  = RETRASSTDEBUG;

    autochunk           = false;
    numrows             = DEFAULT_NUMROWS;
    job                 = NULL;
//    sgChunks->Cells[SGCHUNKS_COL_SECTION]   [0] = "Section";
//    sgChunks->Cells[SGCHUNKS_COL_START]     [0] = "Start";
//    sgChunks->Cells[SGCHUNKS_COL_END]       [0] = "End";
//    sgChunks->Cells[SGCHUNKS_COL_SIZE]      [0] = "Size";
//    sgChunks->ColWidths[SGCHUNKS_COL_SECTION]   = 100;
//    sgChunks->ColWidths[SGCHUNKS_COL_START]     = 100;
//    sgChunks->ColWidths[SGCHUNKS_COL_END]       = 100;
//    sgChunks->ColWidths[SGCHUNKS_COL_SIZE]      = 100;

    //setupStringGrid(TStringGrid * sg, const int cols, char * colnames[], int colwidths[])
//    sgChunks->ColCount = SGCHUNKS_NUMCOLS;
//    for (int i=0; i<SGCHUNKS_NUMCOLS; i++) {
//        sgChunks->Cells[i][0]    = sgChunksColName[i];
//        sgChunks->ColWidths[i]   = sgChunksColWidth[i];
//    }
    setupStringGrid(sgChunks, SGCHUNKS_NUMCOLS, sgChunksColName, sgChunksColWidth);

//    sgVials->ColCount = SGVIALS_NUMCOLS;
//    for (int i=0; i<SGVIALS_NUMCOLS; i++) {
//        sgVials->Cells[i][0]    = sgVialColName[i];
//        sgVials->ColWidths[i]   = sgVialColWidth[i];
//    }
    setupStringGrid(sgVials, SGVIALS_NUMCOLS, sgVialColName, sgVialColWidth);

    radbutDefault->Caption = DEFAULT_NUMROWS;
}

void __fastcall TfrmSamples::FormShow(TObject *Sender) {
    std::ostringstream oss;
    oss << (autochunk ? "auto-chunk" : "manual chunk") << ", "
    << ((job->getJobType() == LCDbCryoJob::JobKind::SAMPLE_RETRIEVAL) ? "SAMPLE_RETRIEVAL;" : "!SAMPLE_RETRIEVAL");
    debugLog(oss.str().c_str()); //;
    btnSave->Enabled = true;
    showChunks();
    loadRows();
    showRows();
    if (IDYES == Application->MessageBox(L"Do you want to automatically create chunks for this list?", L"Question", MB_YESNO)) {
        autoChunk();
    }
}

void __fastcall TfrmSamples::btnCancelClick(TObject *Sender) { Close(); }

void __fastcall TfrmSamples::btnSaveClick(TObject *Sender) {
    //btnSave->Enabled = false;
    // TODO insert rows into c_box_retrieval and l_cryovial_retrieval
    // TODO update c_retrieval_job (in progress)
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
    std::ostringstream oss;
    oss <<__FUNC__<<": numrows: "<<numrows;
    debugLog(oss.str().c_str());
    //loadRows();
    showRows();
}

void __fastcall TfrmSamples::timerCustomRowsTimer(TObject *Sender) {
    timerCustomRows->Enabled = false;
    numrows = editCustomRows->Text.ToIntDef(0);
    std::ostringstream oss;
    oss <<__FUNC__<<": load"<<": numrows: "<<numrows;
    debugLog(oss.str().c_str());
    //loadRows();
    showRows();
}

void __fastcall TfrmSamples::editCustomRowsChange(TObject *Sender) {
    timerCustomRows->Enabled = false; // reset
    timerCustomRows->Enabled = true;;
}

void __fastcall TfrmSamples::sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
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
    if (0 == chunks.size()) { // must always have one chunk anyway
        clearGridSelection(sgChunks);
    } else {
        sgChunks->RowCount = chunks.size() + 1;
        sgChunks->FixedRows = 1; // "Fixed row count must be LESS than row count"
        // use clearSG/clearGridSelection
    }
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

/*
    //"SELECT br.box_id FROM c_box_retrieval br WHERE br.retrieval_cid = :rtid AND br.section = :sect AND status != 99");
//    // no 'chunks' yet, we haven't created them!
//    // they will exist in c_box_retrieval, but don't already exist in cryovial_store where the job comes from
//    q.setSQL("SELECT * FROM c_retrieval_job rj, cryovial_store cs WHERE rj.retrieval_cid = cs.retrieval_cid ORDER BY cs.box_cid");
*/

void TfrmSamples::loadRows() {
    std::ostringstream oss; oss<<__FUNC__<<": numrows: "<<numrows; debugLog(oss.str().c_str());
    Screen->Cursor = crSQLWait;
    delete_referenced<vecpSampleRow>(vials);
    LQuery q(Util::projectQuery(job->getProjectID(), true));
    q.setSQL(
    /*
    LPDbCryovialStore(q) expects:
     Cryovial_id, Note_Exists, retrieval_cid, box_cid, status, cryovial_position, cryovial_id
 */

//        "SELECT"
//        "   Cryovial_id, Note_Exists, retrieval_cid, box_cid, status, cryovial_position, cryovial_id,"
//        "   cryovial_barcode, t.external_name AS aliquot, b.external_name AS box,"
//        "   cryovial_position, s.external_name AS site, m.position, v.external_full AS vessel,"
//        "   shelf_number, r.external_name AS rack, bs.slot_position"
        "SELECT"
        "   cs.cryovial_id, cs.note_exists, cs.retrieval_cid, cs.box_cid, cs.status, cs.cryovial_position,"
        "   c.cryovial_barcode, t.external_name AS aliquot, b.external_name AS box,"
        "   s.external_name AS site, m.position, v.external_full AS vessel,"
        "   shelf_number, r.external_name AS rack, bs.slot_position"

        " FROM"
        "   cryovial c, cryovial_store cs, box_name b, box_store bs, c_rack_number r,"
        "   c_tank_map m, c_object_name s,"   // site
        "   c_object_name v,"                 // vessel
        "   c_object_name t"                  // aliquot?
        " WHERE"
        "   c.cryovial_id = cs.cryovial_id AND"
        "   b.box_cid = cs.box_cid AND"
        "   b.box_cid = bs.box_cid AND"
        "   bs.status = 6 AND"    // 6?
        "   t.object_cid = aliquot_type_cid AND"
        "   bs.rack_cid = r.rack_cid AND"
        "   r.tank_cid = m.tank_cid AND"
        "   s.object_cid = location_cid AND"
        "   v.object_cid = storage_cid AND"
        "   cs.retrieval_cid = :jobID");
    q.setParam("jobID", job->getID());
    /* -- may have destination box defined, could find with left join:
    from
         cryovial_store s1
    left join
        cryovial c on c.cryovial_cid = s1.cryovial_id
    left join
        box_name n1 on n1.box_cid = s1.box_cid
    left join
        cryovial_store s2 on s1.cryovial_id = s2.cryovial_id and
        s2.status = 0
    left join
        box_name n2 on n2.box_cid = s2.box_cid
    where
        s1.retrieval_cid = :jobID*/

    q.open();
    while (!q.eof()) {
        /*
        LPDbCryovialStore * store_record;
        std::string     cryovial_barcode;
        std::string     aliquote_type_name;
        std::string     box_name;
        std::string     site_name;
        int             position;
        std::string     vessel_name;
        int             shelf_number;
        std::string     rack_name;
        int             slot_position;*/


        LPDbCryovialStore * vial = new LPDbCryovialStore(q);
/*    SampleRow(  LPDbCryovialStore * store_rec, string barcode, string aliquot, string box,
                string site, int pos, string vessel, int shelf, string rack, int slot) :

        "   c.cryovial_barcode, t.external_name AS aliquot, b.external_name AS box,"
        "   s.external_name AS site, m.position, v.external_full AS vessel,"
        "   shelf_number, r.external_name AS rack, bs.slot_position"
                */

        pSampleRow  row = new SampleRow(
            vial,
            q.readString("cryovial_barcode"),
            q.readString("aliquot"),
            q.readString("box"),
            q.readString("site"),
            q.readInt("position"),
            q.readString("vessel"),
            q.readInt("shelf_number"),
            q.readString("rack"),
            q.readInt("slot_position")
            );
        vials.push_back(row);
        q.next();
    }
    Screen->Cursor = crDefault;
}

void TfrmSamples::showRows() {
    int row = 1;
    vecpSampleRow::const_iterator it;
    for (it = vials.begin(); it != vials.end(); it++, row++) {
        pSampleRow sampleRow = *it;
        LPDbCryovialStore * vial = sampleRow->store_record;
/*    SampleRow(  LPDbCryovialStore * store_rec, string barcode, string aliquot, string box,
                string site, int pos, string vessel, int shelf, string rack, int slot) :

        "   c.cryovial_barcode, t.external_name AS aliquot, b.external_name AS box,"
        "   s.external_name AS site, m.position, v.external_full AS vessel,"
        "   shelf_number, r.external_name AS rack, bs.slot_position"
                */

        //sgVials->Cells[SGVIALS_COL_1][row] = vial->getID();
        //sgVials->Cells[][row] = vial->;
        //sgVials->Cells[][row] = sampleRow->;
        sgVials->Cells[SGVIALS_BARCODE][row] = sampleRow->cryovial_barcode.c_str();
        sgVials->Cells[SGVIALS_DESTBOX][row] = "tba"; //sampleRow->;
        sgVials->Cells[SGVIALS_DESTPOS][row] = "tba"; //sampleRow->;
        sgVials->Cells[SGVIALS_CURRBOX][row] = sampleRow->box_name.c_str();
        sgVials->Cells[SGVIALS_CURRPOS][row] = sampleRow->position;
        sgVials->Cells[SGVIALS_STRUCTURE][row] = sampleRow->vessel_name.c_str(); //??
        sgVials->Cells[SGVIALS_LOCATION][row] = sampleRow->site_name.c_str();

        sgVials->Objects[0][row] = (TObject *)sampleRow;
        if (row >= numrows) break;
    }
}

void __fastcall TfrmSamples::btnAutoChunkClick(TObject *Sender) {
    autoChunk();
}

void __fastcall TfrmSamples::btnIncrClick(TObject *Sender) {
    //
}

void __fastcall TfrmSamples::btnDecrClick(TObject *Sender) {
    //
}


void __fastcall TfrmSamples::btnSaveChunkClick(TObject *Sender) {
    //?? not needed? not 'saving' chunks anywhere, not to db anyway
}


void __fastcall TfrmSamples::sgVialsFixedCellClick(TObject *Sender, int ACol, int ARow) {
    // sort by column
}


void __fastcall TfrmSamples::sgVialsColumnMoved(TObject *Sender, int FromIndex, int ToIndex) {
    ostringstream oss; oss << __FUNC__ << " ok";
    debugLog(oss.str().c_str());
}


void __fastcall TfrmSamples::sgVialsClick(TObject *Sender) {
    ostringstream oss; oss << __FUNC__;

    oss << " ok";
    debugLog(oss.str().c_str());
}


