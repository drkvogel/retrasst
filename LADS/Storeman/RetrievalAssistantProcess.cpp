#include <vcl.h>
#pragma hdrstop
#include "StoreDAO.h"
#include "RetrievalAssistantProcess.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmProcess *frmProcess;

__fastcall TfrmProcess::TfrmProcess(TComponent* Owner) : TForm(Owner) {
    sgwChunks = new StringGridWrapper< Chunk< SampleRow > >(sgChunks, &chunks);
    sgwChunks->addCol("section",  "Section",  87);
    sgwChunks->addCol("start",    "Start",    70);
    sgwChunks->addCol("startbox", "Box",      304);
    sgwChunks->addCol("startvial","Vial",     150);
    sgwChunks->addCol("end",      "End",      66);
    sgwChunks->addCol("endbox",   "Box",      242);
    sgwChunks->addCol("endvial",  "Vial",     160);
    sgwChunks->addCol("size",     "Size",     87);
    sgwChunks->init();

    sgwVials = new StringGridWrapper<SampleRow>(sgVials, &vials);
    sgwVials->addCol("barcode",  "Barcode",          102,   SampleRow::sort_asc_barcode);
    sgwVials->addCol("aliquot",  "Aliquot",          100,   SampleRow::sort_asc_aliquot);
    sgwVials->addCol("currbox",  "Current box",      275,   SampleRow::sort_asc_currbox);
    sgwVials->addCol("currpos",  "Pos",              43,    SampleRow::sort_asc_currpos);
    sgwVials->addCol("site",     "Site",             116,   SampleRow::sort_asc_site);
    sgwVials->addCol("vesspos",  "Position",         50,    SampleRow::sort_asc_vesspos);
    sgwVials->addCol("shelf",    "Shelf",            100,   SampleRow::sort_asc_shelf);
    sgwVials->addCol("vessel",   "Vessel",           43,    SampleRow::sort_asc_vessel);
    sgwVials->addCol("struct",   "Structure",        121,   SampleRow::sort_asc_structure);
    sgwVials->addCol("boxpos",   "Slot",             40,    SampleRow::sort_asc_slot);
    sgwVials->addCol("destbox",  "Destination box",  213,   SampleRow::sort_asc_destbox);
    sgwVials->addCol("destpos",  "Pos",              37,    SampleRow::sort_asc_destpos);
    sgwVials->init();
}

void __fastcall TfrmProcess::FormCreate(TObject *Sender) {
    cbLog->Visible      = RETRASSTDEBUG;
    memoDebug->Visible  = RETRASSTDEBUG;
    job                 = NULL;
    loadingMessage = "Loading retrieval list, please wait...";
}

void __fastcall TfrmProcess::FormDestroy(TObject *Sender) {
    delete sgwChunks;
    delete sgwVials;
}

void __fastcall TfrmProcess::FormShow(TObject *Sender) {
    timerLoadPlan->Enabled = true;
    panelLoading->Caption = loadingMessage;
}

void __fastcall TfrmProcess::cbLogClick(TObject *Sender) {
    memoDebug->Visible = cbLog->Checked;
}

void __fastcall TfrmProcess::menuItemExitClick(TObject *Sender) {
    if (IDYES == Application->MessageBox(L"Are you sure you want to exit?\n\nCurrent progress will be saved.", L"Question", MB_YESNO)) {
        // save stuff
        Close();
    }
}

void __fastcall TfrmProcess::sgChunksFixedCellClick(TObject *Sender, int ACol, int ARow) {
    ostringstream oss; oss << __FUNC__;
    oss<<sgwChunks->printColWidths()<<" clicked on col: "<<ACol<<".";
    //debugLog(oss.str().c_str());
}

void __fastcall TfrmProcess::sgChunksClick(TObject *Sender) {
    showChunk();
}

void __fastcall TfrmProcess::sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
    TColor background = clWindow;
    if (0 == ARow) {
        background = clBtnFace;
    } else {
        Chunk< SampleRow > * chunk = (Chunk< SampleRow > *)sgChunks->Objects[0][ARow];
        background = RETRIEVAL_ASSISTANT_DONE_COLOUR; //break;
        if (NULL == chunk) {
            background = clWindow; //RETRIEVAL_ASSISTANT_ERROR_COLOUR;
        } else {
            background = RETRIEVAL_ASSISTANT_DONE_COLOUR; //background = RETRIEVAL_ASSISTANT_ERROR_COLOUR;
        }
    }
    TCanvas * cnv = sgChunks->Canvas;
	cnv->Brush->Color = background;
	cnv->FillRect(Rect);
    if (State.Contains(gdSelected)) {
        TFontStyles oldFontStyle = cnv->Font->Style;
        TPenStyle oldPenStyle = cnv->Pen->Style;
        cnv->Pen->Style     = psDot;
        cnv->Rectangle(Rect.Left+1, Rect.Top+1, Rect.Right-1, Rect.Bottom-1);
        cnv->Font->Style    = TFontStyles() << fsBold;
    	cnv->TextOut(Rect.Left+5, Rect.Top+5, sgChunks->Cells[ACol][ARow]);
        cnv->Pen->Style     = oldPenStyle;
        cnv->Font->Style    = oldFontStyle;
	} else {
        cnv->TextOut(Rect.Left+5, Rect.Top+5, sgChunks->Cells[ACol][ARow]);
    }
}

void __fastcall TfrmProcess::sgVialsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
    TColor background = clWindow;
    if (0 == ARow) {
        background = clBtnFace;
    } else {
        SampleRow * row = (SampleRow *)sgVials->Objects[0][ARow];
        background = RETRIEVAL_ASSISTANT_DONE_COLOUR; //break;
        if (NULL == row) {
            background = clWindow;
        } else {
            background = RETRIEVAL_ASSISTANT_DONE_COLOUR;
        }
    }
    TCanvas * cnv = sgVials->Canvas;
	cnv->Brush->Color = background;
	cnv->FillRect(Rect);
    if (State.Contains(gdSelected)) {
        TFontStyles oldFontStyle = cnv->Font->Style;
        TPenStyle oldPenStyle = cnv->Pen->Style;
        cnv->Pen->Style     = psDot;
        cnv->Rectangle(Rect.Left+1, Rect.Top+1, Rect.Right-1, Rect.Bottom-1);
        cnv->Font->Style    = TFontStyles() << fsBold;
    	cnv->TextOut(Rect.Left+5, Rect.Top+5, sgVials->Cells[ACol][ARow]);
        cnv->Pen->Style     = oldPenStyle;
        cnv->Font->Style    = oldFontStyle;
	} else {
        cnv->TextOut(Rect.Left+5, Rect.Top+5, sgVials->Cells[ACol][ARow]);
    }
}

void TfrmProcess::showChunks() {
    if (0 == chunks.size()) { throw Exception("No chunks"); } // must always have one chunk anyway
    else { sgChunks->RowCount = chunks.size() + 1; sgChunks->FixedRows = 1; } // "Fixed row count must be LESS than row count"

    int row = 1;
    for (vector< Chunk< SampleRow > * >::const_iterator it = chunks.begin(); it != chunks.end(); it++, row++) {
        Chunk< SampleRow > * chunk = *it;
        sgChunks->Cells[sgwChunks->colNameToInt("section")]   [row] = chunk->getSection();
        sgChunks->Cells[sgwChunks->colNameToInt("start")]     [row] = chunk->getStart();
        sgChunks->Cells[sgwChunks->colNameToInt("startbox")]  [row] = chunk->getStartBox().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("startvial")] [row] = chunk->getStartVial().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("end")]       [row] = chunk->getEnd();
        sgChunks->Cells[sgwChunks->colNameToInt("endbox")]    [row] = chunk->getEndBox().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("endvial")]   [row] = chunk->getEndVial().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("size")]      [row] = chunk->getSize();
        sgChunks->Objects[0][row] = (TObject *)chunk;
    }
    showChunk();
}

Chunk< SampleRow > * TfrmProcess::currentChunk() {
    if (sgChunks->Row < 1) sgChunks->Row = 1; // force selection of 1st row
    Chunk< SampleRow > * chunk = (Chunk< SampleRow > *)sgChunks->Objects[0][sgChunks->Row];
    if (NULL == chunk) throw Exception("null chunk");
    return chunk;
}

//void TfrmProcess::showRows() {
void TfrmProcess::showChunk(Chunk< SampleRow > * chunk) {
    Screen->Cursor = crSQLWait; Enabled = false;

    if (NULL == chunk) { chunk = currentChunk(); } // default
    if (chunk->getSize() <= 0) { sgwVials->clear(); }
    else { sgVials->RowCount = chunk->getSize(); sgVials->FixedRows = 1; }

    for (int row = 1; row < chunk->getSize(); row++) {
        SampleRow * sampleRow = chunk->rowAt(row);
        LPDbCryovial *      vial    = sampleRow->cryo_record;
        LPDbCryovialStore * store   = sampleRow->store_record;
        sgVials->Cells[sgwVials->colNameToInt("barcode")] [row] = sampleRow->cryovial_barcode.c_str();
        sgVials->Cells[sgwVials->colNameToInt("aliquot")] [row] = sampleRow->aliquot_type_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("currbox")] [row] = sampleRow->src_box_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("currpos")] [row] = sampleRow->store_record->getPosition();
        sgVials->Cells[sgwVials->colNameToInt("site"   )] [row] = sampleRow->site_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("vesspos")] [row] = sampleRow->vessel_pos;
        sgVials->Cells[sgwVials->colNameToInt("shelf"  )] [row] = sampleRow->structure_pos;
        sgVials->Cells[sgwVials->colNameToInt("vessel" )] [row] = sampleRow->vessel_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("struct" )] [row] = sampleRow->structure_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("boxpos" )] [row] = sampleRow->box_pos;
        sgVials->Cells[sgwVials->colNameToInt("destbox")] [row] = sampleRow->dest_box_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("destpos")] [row] = sampleRow->dest_cryo_pos;
        sgVials->Objects[0][row] = (TObject *)sampleRow;
    }

    Screen->Cursor = crDefault; Enabled = true;
}

void TfrmProcess::process() {
/*
 * Work through list or sub-section by giving the storage location and sample ID of each sample on the list in the order saved above (REQ 8.3.8);
 * As each sample is retrieved its barcode should be scanned, if the scanned barcode matches that on the list
the destination location should be displayed and the next ID/location should be displayed (REQ 8.3.9)
 * if the ID’s do not match a warning should be displayed and re-entry of the barcode required (REQ 8.3.10).
     what if it's the wrong sample, or it's missing? handle this
 * When working through the list the previous five successfully entered ID’s should always be visible (REQ 8.3.11).
 * The option to exit the process saving progress should be offered, with an “are you sure?” message in case of accidental selection (REQ 8.3.12).

extra:
 * if, at the end of processing a chunk, there are any source boxes which have become empty, the user may want to discard them instead of replacing them.
   if so, provide an option to discard these empty boxes, recording it in the database


 destination box+position, cryovial barcode and current box+position+structure+location of the primary and secondary aliquots.
*/
}

//void TfrmProcess::loadRows() {
/*
For a box retrieval, the retrieval plan will be given by
Select * from c_box_retrieval b order by b.section, b.rj_box_cid

For a cryovial retrieval, the retrieval plan will be:
Select * from c_box_retrieval b, l_cryovial_retrieval c where b.rj_box_cid = c.rj_box_cid order by b.section, c.position
*/
/* SELECT
   cs.Cryovial_id, cs.Note_Exists, cs.retrieval_cid, cs.box_cid, cs.status, cs.tube_position,
   cryovial_barcode, t.external_name AS aliquot, b.external_name AS box,
   s.external_name AS site, m.position, v.external_full AS vessel,
   shelf_number, r.external_name AS rack, bs.slot_position
 FROM
   cryovial c, cryovial_store cs, box_name b, box_store bs, c_rack_number r,
   c_tank_map m, c_object_name s,
   c_object_name v,
   c_object_name t
 WHERE
   c.cryovial_id = cs.cryovial_id AND
   b.box_cid = cs.box_cid AND
   b.box_cid = bs.box_cid AND
   bs.status = 6 AND
   t.object_cid = aliquot_type_cid AND
   bs.rack_cid = r.rack_cid AND
   r.tank_cid = m.tank_cid AND
   s.object_cid = location_cid AND
   v.object_cid = storage_cid AND
   cs.retrieval_cid = -1015 */
//}

void __fastcall TfrmProcess::timerLoadPlanTimer(TObject *Sender) {
    timerLoadPlan->Enabled = false;
    loadRows();
}

void TfrmProcess::loadRows() {
    panelLoading->Caption = loadingMessage;
    panelLoading->Visible = true; // appearing in wrong place because called in OnShow, form not yet maximized
    panelLoading->Top = (sgVials->Height / 2) - (panelLoading->Height / 2);
    panelLoading->Left = (sgVials->Width / 2) - (panelLoading->Width / 2);
    progressBottom->Style = pbstMarquee; progressBottom->Visible = true;
    Screen->Cursor = crSQLWait; // disable mouse? //ShowCursor(false);
    Enabled = false;
    loadPlanWorkerThread = new LoadPlanWorkerThread();
    loadPlanWorkerThread->OnTerminate = &loadPlanWorkerThreadTerminated;
}

__fastcall LoadPlanWorkerThread::LoadPlanWorkerThread() : TThread(false) {
    FreeOnTerminate = true;
}

void __fastcall LoadPlanWorkerThread::updateStatus() { // can't use args for synced method, don't know why
    frmProcess->panelLoading->Caption = loadingMessage.c_str();
    frmProcess->panelLoading->Repaint();
}

void __fastcall LoadPlanWorkerThread::Execute() {
    delete_referenced< vector<SampleRow * > >(frmProcess->vials);
    ostringstream oss; oss<<frmProcess->loadingMessage<<" (preparing query)"; loadingMessage = oss.str().c_str(); //return;
    if (NULL != frmProcess && NULL != frmProcess->job) { frmProcess->job = frmProcess->job; } else { throw "wtf?"; }

    LQuery qd(Util::projectQuery(frmProcess->job->getProjectID(), true)); // ddb
    qd.setSQL( // from spec 2013-09-11
/*Rosetta error: ROSETTA Error: member "dest_pos" not found
Rosetta error: ROSETTA Error: member "dest_name" not found
Rosetta error: ROSETTA Error: member "dest_id" not found
Rosetta error: ROSETTA Error: member "source_name" not found
Rosetta error: ROSETTA Error: member "aliquot" not found
Rosetta error: ROSETTA Error: member "Cryovial_id" not found
Rosetta error: ROSETTA Error: member "Note_Exists" not found
Rosetta error: ROSETTA Error: member "cryovial_id" not found
Rosetta error: ROSETTA Error: member "box_cid" not found
Rosetta error: ROSETTA Error: member "tube_position" not found
Rosetta error: ROSETTA Error: member "Cryovial_id" not found
Rosetta error: ROSETTA Error: member "Note_Exists" not found
Rosetta error: ROSETTA Error: member "box_cid" not found
Rosetta error: ROSETTA Error: member "sample_id" not found
Rosetta error: ROSETTA Error: member "record_id" not found
Rosetta error: ROSETTA Error: member "tube_position" not found'.*/

//        "SELECT"
//        "    s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position," // for LPDbCryovialStore
//        "    s1.record_id, c.sample_id, c.aliquot_type_cid, " // for LPDbCryovial
//        "    cbr.rj_box_cid, cbr.box_id as dest_id, cbr.section as chunk, lcr.position as dest_pos, b1.external_name as dest_name,"
//        "    cbr.status as box_status, lcr.slot_number as cryo_slot, lcr.status as cryo_status,"
//        "    s1.box_cid as dest_id," //???
//        "    c.cryovial_barcode"//, s1.tube_position as source_pos"
//        " FROM"
//        "    c_box_retrieval cbr, l_cryovial_retrieval lcr, cryovial c, cryovial_store s1, box_name b1, box_name b2"
//        " WHERE"
//        "    cbr.retrieval_cid = :rtid AND"
//        "    cbr.rj_box_cid = lcr.rj_box_cid AND"
//        "    lcr.cryovial_barcode = c.cryovial_barcode AND lcr.aliquot_type_cid = c.aliquot_type_cid AND"
//        "    c.cryovial_id = s1.cryovial_id AND"
//
//        "    s1.status = 2 AND"
//        "    b1.box_cid = s1.box_cid"
//        " ORDER BY"
//        "    chunk, cbr.rj_box_cid, lcr.position"

        " SELECT"
        "    s1.retrieval_cid,cbr.section as chunk, cbr.rj_box_cid, lcr.position as dest_pos, cbr.status as cbr_status,"
        "    s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position," // for LPDbCryovialStore
        "    s1.record_id, c.cryovial_barcode, c.sample_id, c.aliquot_type_cid, c.note_exists as cryovial_note,"
        "    s1.box_cid, b1.external_name as source_box, s1.status, s1.tube_position, s1.note_exists as cs_note,"
        "    cbr.box_id as dest_id, b2.external_name as dest_name, s2.tube_position as slot_number, s2.status as dest_status"
        " FROM"
        "    c_box_retrieval cbr, l_cryovial_retrieval lcr, cryovial c, cryovial_store s1, box_name b1, cryovial_store s2, box_name b2"
        " WHERE"
        "    cbr.retrieval_cid = :rtid AND"
        "    s1.retrieval_cid = cbr.retrieval_cid AND"
        "    lcr.rj_box_cid = cbr.rj_box_cid AND"
        "    lcr. cryovial_barcode = c.cryovial_barcode AND lcr.aliquot_type_cid = c.aliquot_type_cid AND"
        "    b2.box_cid = cbr.box_id AND"
        "    c.cryovial_id = s1.cryovial_id AND"
        "    c.cryovial_id = s2.cryovial_id AND"
        "    b1.box_cid = s1.box_cid AND"
        "    s2.box_cid = b2.box_cid"
        " ORDER BY"
        "    s1.retrieval_cid,chunk, cbr.rj_box_cid, lcr.position"
    );
    rowCount = 0; // class variable needed for synchronise
    int retrieval_cid = frmProcess->job->getID();
    qd.setParam("rtid", retrieval_cid);
    loadingMessage = frmProcess->loadingMessage;
    qd.open();
    int curchunk = 1, chunk = 0;
    while (!qd.eof()) {
        chunk = qd.readInt("chunk");
        wstringstream oss; oss<<__FUNC__<<oss<<"chunk:"<<chunk<<", rowCount: "<<rowCount; OutputDebugString(oss.str().c_str());
        if (chunk > curchunk) {
            frmProcess->addChunk(rowCount);
            curchunk = chunk;
        }

        if (0 == rowCount % 10) {
            ostringstream oss; oss<<"Found "<<rowCount<<" vials";
            loadingMessage = oss.str().c_str();
            Synchronize((TThreadMethod)&updateStatus);
        }
        SampleRow * row = new SampleRow(
            new LPDbCryovial(qd),
            new LPDbCryovialStore(qd),
            qd.readString(  "cryovial_barcode"),
            "", //qd.readString(  "aliquot"),
            "", //qd.readString(  "source_name"),
            qd.readInt(     "dest_id"),
            qd.readString(  "dest_name"),
            qd.readInt(     "dest_pos"),
            "", 0, "", 0, 0, "", 0 ); // no storage details yet
        frmProcess->vials.push_back(row);
        qd.next();
        rowCount++;
    }

    // find locations of source boxes
    map<int, const SampleRow *> samples; ROSETTA result; StoreDAO dao;
    int rowCount2 = 0;
	for (vector<SampleRow *>::iterator it = frmProcess->vials.begin(); it != frmProcess->vials.end(); ++it, rowCount2++) {
        SampleRow * sample = *it;
        ostringstream oss; oss<<"Finding storage for "<<sample->cryovial_barcode<<"["<<rowCount2<<"/"<<rowCount<<"]";
        map<int, const SampleRow *>::iterator found = samples.find(sample->store_record->getBoxID());
        if (found != samples.end()) { // fill in box location from cache map
            sample->copyLocation(*(found->second));
        } else {
            if (dao.findBox(sample->store_record->getBoxID(), LCDbProjects::getCurrentID(), result)) {
                sample->copyLocation(result);
            } else {
                sample->setLocation("not found", 0, "not found", 0, 0, "not found", 0); //oss<<"(not found)";
            }
            samples[sample->store_record->getBoxID()] = (*it); // cache result
        }
        oss<<sample->storage_str();
        loadingMessage = oss.str().c_str();
        Synchronize((TThreadMethod)&updateStatus);
	}
    //showChunks();
}

void __fastcall TfrmProcess::loadPlanWorkerThreadTerminated(TObject *Sender) {
    progressBottom->Style = pbstNormal; progressBottom->Visible = false;
    panelLoading->Visible = false;
    Screen->Cursor = crDefault;
    //chunks.clear();
    //sgwChunks->clear(); //??
    //addChunks(); // create chunks based on c_box_retrieval.section, order by l_cryovial_retrieval.position
    showChunks();
    Enabled = true;
}

void TfrmProcess::addChunks() {
    if (vials.size() == 0) throw "vials.size() == 0";
    int numvials = vials.size(); int numchunks = chunks.size();

    for (vecpSampleRow::const_iterator it = vials.begin(); it != vials.end(); it++) {
        Chunk< SampleRow > * curchunk, * newchunk;
        if (chunks.size() == 0) { // first chunk, make default chunk from entire listrows
            newchunk = new Chunk< SampleRow >(sgwVials, chunks.size()+1, 0, vials.size()-1); // 0-indexed // size is calculated
        } else {
            //if (offset <= 0 || offset > vials.size()) throw "invalid offset"; // ok only for first chunk
            curchunk = currentChunk();
            int currentchunksize = curchunk->getSize(); // no chunks until first added
            //if (curchunk->getStart()+offset > vials.size()) { // current last chunk is too small to be split at this offset
                //return false; // e.g. for auto-chunk to stop chunking
            //}
            //curchunk->setEnd(curchunk->getStart()+offset-1); // row above start of new chunk
            //newchunk = new Chunk< SampleRow >(sgwVials, chunks.size()+1, curchunk->getStart()+offset, vials.size()-1);
        }
        chunks.push_back(newchunk);
    }
}

void TfrmProcess::addChunk(int row) {
    Chunk< SampleRow > * chunk;// = new Chunk< SampleRow >;
    chunk = new Chunk< SampleRow >(sgwVials, chunks.size()+1, 1, row);
    chunks.push_back(chunk);
}

void __fastcall TfrmProcess::btnAcceptClick(TObject *Sender) {
    // check correct vial; could be missing, swapped etc
}




