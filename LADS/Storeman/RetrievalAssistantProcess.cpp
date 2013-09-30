#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistantProcess.h"
#include "StoreDAO.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmProcess *frmProcess;

__fastcall TfrmProcess::TfrmProcess(TComponent* Owner) : TForm(Owner) {
    sgwChunks = new StringGridWrapper< Chunk< SampleRow > >(sgChunks, &chunks);
    sgwChunks->addCol("section",  "Section",  200);
    sgwChunks->addCol("start",    "Start",    200);
    sgwChunks->addCol("end",      "End",      200);
    sgwChunks->addCol("size",     "Size",     200);
    sgwChunks->init();

    //sgwVials = new StringGridWrapper<SampleRow>(sgRetrieval, &vials);
    sgwVials = new StringGridWrapper<SampleRow>(sgVials, &vials);
    sgwVials->addCol("barcode",  "Barcode",          102,   SampleRow::sort_asc_barcode);
    sgwVials->addCol("aliquot",  "Aliquot",          100,   SampleRow::sort_asc_aliquot);
    sgwVials->addCol("currbox",  "Current box",      275,   SampleRow::sort_asc_currbox);
    sgwVials->addCol("currpos",  "Pos",              43,    SampleRow::sort_asc_currpos);
    sgwVials->addCol("site",     "Site",             116,   SampleRow::sort_asc_site);
    sgwVials->addCol("vesspos",  "Position",         50,    SampleRow::sort_asc_position);
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
    //maxRows             = DEFAULT_NUMROWS;
    job                 = NULL;
    loadingMessage = "Loading retrieval list, please wait...";
}

void __fastcall TfrmProcess::FormDestroy(TObject *Sender) {
    delete sgwChunks;
    delete sgwVials;
}

void __fastcall TfrmProcess::FormShow(TObject *Sender) {
    //loadRows();
    timerLoadPlan->Enabled = false;
    //showChunk();
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

void TfrmProcess::addChunk() {
    Chunk< SampleRow > * chunk;// = new Chunk< SampleRow >;
    if (chunks.size() == 0) { // first chunk, make default chunk from entire listrows
        chunk = new Chunk< SampleRow >(
            sgwVials, chunks.size() + 1,
            //vector< SampleRow * > * rows;
            1,              vials[0]->src_box_name,                vials[0]->cryo_record->getBarcode(),
            vials.size(),   vials[vials.size()-1]->src_box_name,   vials[vials.size()-1]->cryo_record->getBarcode()
        ); // 1-indexed // size is calculated
        chunk->setEnd(vials.size());
        chunk->setStart(1);
    } else {
        chunk = new Chunk< SampleRow >(
            sgwVials, chunks.size() + 1,
            currentChunk()->getSize()+1,    vials[0]->src_box_name,                vials[0]->cryo_record->getBarcode(), // first
            vials.size(),                   vials[vials.size()-1]->src_box_name,   vials[vials.size()-1]->cryo_record->getBarcode() // last
        );
    }
    chunks.push_back(chunk);
    showChunks();
    sgChunks->Row = sgChunks->RowCount-1; // fixme make it the current chunk
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
 * if the ID�s do not match a warning should be displayed and re-entry of the barcode required (REQ 8.3.10).
 * When working through the list the previous five successfully entered ID�s should always be visible (REQ 8.3.11).
 * The option to exit the process saving progress should be offered, with an �are you sure?� message in case of accidental selection (REQ 8.3.12).

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
   cs.Cryovial_id, cs.Note_Exists, cs.retrieval_cid, cs.box_cid, cs.status, cs.cryovial_position,
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

    rowCount = 0;
    LQuery qd(Util::projectQuery(frmProcess->job->getProjectID(), true)); // ddb

    qd.setSQL( // from spec 2013-09-11
        "SELECT"
        " *"
        " FROM"
        "   c_box_retrieval b, l_cryovial_retrieval c"
        " WHERE"
        "   retrieval_cid = :rtid AND"
        "   b.rj_box_cid = c.rj_box_cid "
        " ORDER BY"
        "   b.section, b.rj_box_cid, c.position"
    );
//    qd.setSQL( // from spec 2013-09-11
//        "SELECT"
//        "  s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.cryovial_position," // for LPDbCryovialStore
//        "  s1.record_id, c.sample_id, c.aliquot_type_cid, " // for LPDbCryovial
//            // LPDbCryovial::storeID( query.readInt( "record_id" ) ) <-- record_id comes from cryovial_store?
//        "  c.cryovial_barcode, t.external_name AS aliquot,"
//        "  b1.box_cid as source_id,"
//        "  b1.external_name as source_name,"
//        "  s1.cryovial_position as source_pos,"
//        "  s2.box_cid as dest_id,"
//        "  b2.external_name as dest_name,"
//        "  s2.cryovial_position as dest_pos"
//        " FROM"
//        "  cryovial c, cryovial_store s1, box_name b1,"
//        "  cryovial_store s2, box_name b2,"
//        "  c_object_name t"
//        " WHERE"
//        "  c.cryovial_id = s1.cryovial_id AND"
//        "  b1.box_cid = s1.box_cid AND"
//        "  s1.cryovial_id = s2.cryovial_id AND"
//        "  s2.status = 0 AND"
//        "  b2.box_cid = s2.box_cid AND"
//        "  t.object_cid = aliquot_type_cid AND"
//        "  s1.retrieval_cid = :jobID"
//        " ORDER BY"
//        "  cryovial_barcode"
//        );
    qd.setParam("rtid", frmProcess->job->getID());
    loadingMessage = frmProcess->loadingMessage;
    qd.open();
    while (!qd.eof()) {
        if (0 == rowCount % 10) {
            ostringstream oss; oss<<"Found "<<rowCount<<" vials";
            loadingMessage = oss.str().c_str();
            Synchronize((TThreadMethod)&updateStatus);
        }
        SampleRow * row = new SampleRow(
            new LPDbCryovial(qd),
            new LPDbCryovialStore(qd),
            qd.readString(  "cryovial_barcode"),
            qd.readString(  "aliquot"),
            qd.readString(  "source_name"),
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
        try {
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
        } catch (...) { // it used to crash occasionally
            sample->setLocation("error!", 0, "error!", 0, 0, "error!", 0);
        }
        loadingMessage = oss.str().c_str();
        Synchronize((TThreadMethod)&updateStatus);
	}

    //showChunks();
}

void __fastcall TfrmProcess::loadPlanWorkerThreadTerminated(TObject *Sender) {
    progressBottom->Style = pbstNormal; progressBottom->Visible = false;
    panelLoading->Visible = false;
    Screen->Cursor = crDefault;
    Enabled = true;
    chunks.clear();
    sgwChunks->clear(); //??
    //loadChunks();
    showChunks();
}

