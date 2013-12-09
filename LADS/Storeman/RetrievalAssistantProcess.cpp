#include <vcl.h>
#pragma hdrstop
#include "StoreDAO.h"
#include "RetrievalAssistantProcess.h"
#include "LCDbAuditTrail.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmProcess *frmProcess;

__fastcall TfrmProcess::TfrmProcess(TComponent* Owner) : TForm(Owner) {
    sgwChunks = new StringGridWrapper< Chunk< SampleRow > >(sgChunks, &chunks);
    sgwChunks->addCol("section",  "Section",  60);
    sgwChunks->addCol("status",   "Status",   91);
    sgwChunks->addCol("progress", "Progress", 91);
    sgwChunks->addCol("start",    "Start",    70);
    sgwChunks->addCol("startbox", "Box",      250);
    sgwChunks->addCol("startvial","Vial",     150);
    sgwChunks->addCol("end",      "End",      66);
    sgwChunks->addCol("endbox",   "Box",      242);
    sgwChunks->addCol("endvial",  "Vial",     150);
    sgwChunks->addCol("size",     "Size",     87);
    sgwChunks->init();

    sgwVials = new StringGridWrapper<SampleRow>(sgVials, &vials);
    sgwVials->addCol("barcode",  "Barcode",          91);
    sgwVials->addCol("site",     "Site",             90);
    sgwVials->addCol("vesspos",  "VPos",              28);
    sgwVials->addCol("vessel",   "Vessel",           107);
    sgwVials->addCol("shelf",    "Shelf",            31);
    sgwVials->addCol("structpos","SPos",              27);
    sgwVials->addCol("struct",   "Structure",        100);
    sgwVials->addCol("boxpos",   "Slot",             26);
    sgwVials->addCol("currbox",  "Current box",      257);
    sgwVials->addCol("currpos",  "CPos",              31);
    sgwVials->addCol("destbox",  "Destination box",  240);
    sgwVials->addCol("destpos",  "Pos",              25);
#ifdef _DEBUG
    sgwVials->addCol("status",   "Status",           91);
    sgwVials->addCol("aliquot",  "Aliquot",          90,    SampleRow::sort_asc_aliquot,    "aliquot");
#endif
    sgwVials->init();
}

void __fastcall TfrmProcess::FormCreate(TObject *Sender) {
    cbLog->Visible      = RETRASSTDEBUG;
    cbLog->Checked      = RETRASSTDEBUG;
    panelDebug->Visible = cbLog->Checked;
    job                 = NULL;
    loadingMessage = "Loading retrieval list, please wait...";
}

void __fastcall TfrmProcess::FormClose(TObject *Sender, TCloseAction &Action) {
    delete_referenced< vector <SampleRow * > >(vials);
    delete_referenced< vector< Chunk< SampleRow > * > >(chunks); // chunk objects, not contents of chunks
}

void __fastcall TfrmProcess::FormDestroy(TObject *Sender) {
    delete sgwChunks;
    delete sgwVials;
}

void TfrmProcess::debugLog(String s) {
    String tmp = Now().CurrentDateTime().DateTimeString() + ": " + s;
    memoDebug->Lines->Add(tmp); // could use varargs: http://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c
}

void __fastcall TfrmProcess::FormShow(TObject *Sender) {
    ostringstream oss; oss<<job->getName()<<" : "<<job->getDescription()<<" [id: "<<job->getID()<<"]";
    Caption = oss.str().c_str();
    timerLoadPlan->Enabled = true;
    panelLoading->Caption = loadingMessage;
    chunks.clear();
    sgwChunks->clear();
    sgwVials->clear();
    labelSampleID->Caption  = "loading...";
    labelStorage->Caption   = "loading...";
    labelDestbox->Caption   = "loading...";
}

/*#define RETRIEVAL_ASSISTANT_HIGHLIGHT_COLOUR  clActiveCaption
#define RETRIEVAL_ASSISTANT_NEW_COLOUR          clMoneyGreen
#define RETRIEVAL_ASSISTANT_IN_PROGRESS_COLOUR  clLime
#define RETRIEVAL_ASSISTANT_DONE_COLOUR         clSkyBlue
#define RETRIEVAL_ASSISTANT_NOT_FOUND_COLOUR    clFuchsia
#define RETRIEVAL_ASSISTANT_IGNORED_COLOUR      clGray
#define RETRIEVAL_ASSISTANT_ERROR_COLOUR        clRed
#define RETRIEVAL_ASSISTANT_DELETED_COLOUR      clPurple*/

void __fastcall TfrmProcess::sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
    TColor background = RETRIEVAL_ASSISTANT_ERROR_COLOUR;
    if (0 == ARow) {
        background = clBtnFace;
    } else {
        Chunk< SampleRow > * chunk = (Chunk< SampleRow > *)sgChunks->Objects[0][ARow];
        if (NULL == chunk) {
            background = clWindow; // whilst loading
        } else {
            int status = chunk->getStatus();  //chunkStatus(chunk);
            switch (status) {
                case Chunk< SampleRow >::NOT_STARTED:
                    background = RETRIEVAL_ASSISTANT_NEW_COLOUR; break;
                case Chunk< SampleRow >::INPROGRESS:
                    background = RETRIEVAL_ASSISTANT_IN_PROGRESS_COLOUR; break;
                case Chunk< SampleRow >::DONE:
                    background = RETRIEVAL_ASSISTANT_DONE_COLOUR; break;
                default:
                    background = RETRIEVAL_ASSISTANT_ERROR_COLOUR; break;
            }
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
    TColor background = RETRIEVAL_ASSISTANT_ERROR_COLOUR;
    if (0 == ARow) {
        background = clBtnFace;
    } else {
        SampleRow * row = (SampleRow *)sgVials->Objects[0][ARow];
        if (NULL == row) {
            background = clWindow; /// whilst loading //RETRIEVAL_ASSISTANT_ERROR_COLOUR;
        } else {
            int status = row->retrieval_record->getStatus();
            switch (status) {
                case LCDbCryovialRetrieval::EXPECTED:
                    background = RETRIEVAL_ASSISTANT_NEW_COLOUR; break;
                case LCDbCryovialRetrieval::IGNORED:
                    background = RETRIEVAL_ASSISTANT_IGNORED_COLOUR; break;
                case LCDbCryovialRetrieval::COLLECTED:
                    background = RETRIEVAL_ASSISTANT_DONE_COLOUR; break;
                case LCDbCryovialRetrieval::NOT_FOUND:
                    background = RETRIEVAL_ASSISTANT_NOT_FOUND_COLOUR; break;
                //case LCDbCryovialRetrieval::DELETED:
                default:
                    background = RETRIEVAL_ASSISTANT_ERROR_COLOUR;
            }
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

void __fastcall TfrmProcess::cbLogClick(TObject *Sender) {
    panelDebug->Visible = cbLog->Checked;
    //splitterDebug->Visible  = cbLog->Checked;
}

void __fastcall TfrmProcess::menuItemExitClick(TObject *Sender) {
    exit();
}

void __fastcall TfrmProcess::btnExitClick(TObject *Sender) {
    exit();
}

void __fastcall TfrmProcess::sgChunksFixedCellClick(TObject *Sender, int ACol, int ARow) {
    ostringstream oss; oss << __FUNC__;
    oss<<sgwChunks->printColWidths()<<" clicked on col: "<<ACol<<".";
    //debugLog(oss.str().c_str());
}

void __fastcall TfrmProcess::sgChunksClick(TObject *Sender) {
    int row = sgChunks->Row;
    if (row < 1)
    //Chunk< SampleRow > * loadingChunk = (Chunk< SampleRow > *)sgChunks->Objects[0][row];
    timerLoadPlan->Enabled = true;
    //loadChunk(chunk);
    //showChunk();
}

void TfrmProcess::showChunks() {
    if (0 == chunks.size()) { throw Exception("No chunks"); } // must always have one chunk anyway
    else { sgChunks->RowCount = chunks.size() + 1; sgChunks->FixedRows = 1; } // "Fixed row count must be LESS than row count"
    int row = 1;
    for (vector< Chunk< SampleRow > * >::const_iterator it = chunks.begin(); it != chunks.end(); it++, row++) {
        Chunk< SampleRow > * chunk = *it;
        sgChunks->Cells[sgwChunks->colNameToInt("section")]   [row] = chunk->getSection();
        sgChunks->Cells[sgwChunks->colNameToInt("status")]    [row] = chunk->statusString().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("progress")]  [row] = chunk->progressString().c_str();
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

void TfrmProcess::showChunk(Chunk< SampleRow > * chunk) {
    Screen->Cursor = crSQLWait; Enabled = false;
    if (NULL == chunk) { // default
        chunk = currentChunk();
    }
    if (chunk->getSize() <= 0) {
        sgwVials->clear();
    } else {
        sgVials->RowCount = chunk->getSize()+1;
        sgVials->FixedRows = 1;
    }
    for (int row=0; row < chunk->getSize(); row++) {
        SampleRow *         sampleRow = chunk->rowAt(row);
        LPDbCryovial *      vial    = sampleRow->cryo_record;
        LPDbCryovialStore * store   = sampleRow->store_record;
        int rw = row+1; // for stringgrid
        sgVials->Cells[sgwVials->colNameToInt("barcode")]  [rw] = sampleRow->cryovial_barcode.c_str();
        sgVials->Cells[sgwVials->colNameToInt("status") ]  [rw] = sampleRow->retrieval_record->statusString(sampleRow->retrieval_record->getStatus());
        sgVials->Cells[sgwVials->colNameToInt("aliquot")]  [rw] = sampleRow->aliquot_type_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("currbox")]  [rw] = sampleRow->src_box_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("currpos")]  [rw] = sampleRow->store_record->getPosition();
        sgVials->Cells[sgwVials->colNameToInt("site"   )]  [rw] = sampleRow->site_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("vesspos")]  [rw] = sampleRow->vessel_pos;
        sgVials->Cells[sgwVials->colNameToInt("vessel" )]  [rw] = sampleRow->vessel_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("shelf"  )]  [rw] = sampleRow->shelf_number;
        sgVials->Cells[sgwVials->colNameToInt("structpos")][rw] = sampleRow->structure_pos;
        sgVials->Cells[sgwVials->colNameToInt("struct" )]  [rw] = sampleRow->structure_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("boxpos" )]  [rw] = sampleRow->box_pos;
        sgVials->Cells[sgwVials->colNameToInt("destbox")]  [rw] = sampleRow->dest_box_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("destpos")]  [rw] = sampleRow->dest_cryo_pos;
        sgVials->Objects[0][rw] = (TObject *)sampleRow;
    }
    //sgVials->Row = 1;
    if (1.0 == chunk->getProgress()) { // completed
        btnAccept->Enabled   = false;
        btnSkip->Enabled     = false;
        btnNotFound->Enabled = false;
    } else {
        btnAccept->Enabled   = true;
        btnSkip->Enabled     = true;
        btnNotFound->Enabled = true;
    }
    showCurrentRow();
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

void __fastcall TfrmProcess::timerLoadPlanTimer(TObject *Sender) {
    timerLoadPlan->Enabled = false;
    //loadRows();
    //loadChunk(loadingChunk);
    //loadChunk(currentChunk());
    loadChunk();
}

//void TfrmProcess::loadRows() {
void TfrmProcess::loadChunk() {
//void TfrmProcess::loadChunk(Chunk< SampleRow > *) {
    panelLoading->Caption = loadingMessage;
    panelLoading->Visible = true; // appearing in wrong place because called in OnShow, form not yet maximized
    panelLoading->Top = (sgVials->Height / 2) - (panelLoading->Height / 2);
    panelLoading->Left = (sgVials->Width / 2) - (panelLoading->Width / 2);
    progressBottom->Style = pbstMarquee; progressBottom->Visible = true;
    Screen->Cursor = crSQLWait; // disable mouse? //ShowCursor(false);
    wstringstream oss; oss<<"loadRows for job "<<job->getID()<<" started";
    debugLog(oss.str().c_str());
    Enabled = false;
    loadPlanWorkerThread = new LoadPlanWorkerThread();
    loadPlanWorkerThread->loadingChunk = currentChunk();
    loadPlanWorkerThread->OnTerminate = &loadPlanWorkerThreadTerminated;
}

__fastcall LoadPlanWorkerThread::LoadPlanWorkerThread() : TThread(false) {
    FreeOnTerminate = true;
}

void __fastcall LoadPlanWorkerThread::updateStatus() { // can't use args for synced method, don't know why
    frmProcess->panelLoading->Caption = loadingMessage.c_str();
    frmProcess->panelLoading->Repaint();
}

void __fastcall LoadPlanWorkerThread::debugLog() {
    frmProcess->debugLog(debugMessage.c_str());
}

void __fastcall LoadPlanWorkerThread::msgbox() {
    Application->MessageBox(String(debugMessage.c_str()).c_str(), L"Info", MB_OK);
}

void __fastcall LoadPlanWorkerThread::Execute() {
    /** load retrieval plan
    For a box retrieval, the retrieval plan will be given by: Select * from c_box_retrieval b order by b.section, b.rj_box_cid
    For a cryovial retrieval, the retrieval plan will be: Select * from c_box_retrieval b, l_cryovial_retrieval c where b.rj_box_cid = c.rj_box_cid order by b.section, c.position */

    delete_referenced< vector<SampleRow * > >(frmProcess->vials); frmProcess->chunks.clear();

    ostringstream oss; oss<<frmProcess->loadingMessage<<" (preparing query)"; loadingMessage = oss.str().c_str(); //return;
    if (NULL != frmProcess && NULL != frmProcess->job) { frmProcess->job = frmProcess->job; } else { throw "wtf?"; }
    loadingMessage = frmProcess->loadingMessage;

    //const int pid = LCDbAuditTrail::getCurrent().getProcessID();
    //char tempTableName[128]; sprintf(tempTableName, "retrieval_assistant_temp_%d", pid);
    char * tempTableName = "retrieval_assistant_temp";

    {
        LQuery qd(Util::projectQuery(frmProcess->job->getProjectID(), true)); // ddb

        //qd.setSQL("DROP TABLE IF EXISTS "TEMP_TABLE_NAME);
        qd.setSQL("DROP TABLE IF EXISTS :temp");
        qd.setParam("temp", tempTableName);
        qd.execSQL();

        qd.setSQL(
            "CREATE TABLE :temp AS"
            "   SELECT"
            "       cbr.section AS chunk, cbr.rj_box_cid, cbr.retrieval_cid, cbr.status AS cbr_status, cbr.box_id,"
            "       lcr.position AS dest_pos, lcr.slot_number AS lcr_slot, lcr.process_cid AS lcr_procid, lcr.status AS lcr_status,"
            "       lcr.cryovial_barcode, lcr.aliquot_type_cid"
            "   FROM"
            "       c_box_retrieval cbr, l_cryovial_retrieval lcr"
            "   WHERE"
            "       cbr.retrieval_cid = :rtid"
            "   AND chunk = :chnk"
            "   AND"
            "       lcr.rj_box_cid = cbr.rj_box_cid");
        int retrieval_cid = frmProcess->job->getID();
        qd.setParam("temp", tempTableName);
        qd.setParam("rtid", retrieval_cid);
        qd.setParam("chnk", loadingChunk->getSection()); //frmProcess->currentChunk()->getSection()); //frmProcess->chunk); //
        qd.execSQL();
    }
    debugMessage = "finished create temp table";
    Synchronize((TThreadMethod)&debugLog);

    // gj added a secondary index on cryovial_store (on t_ldb20 only) - how to check this?
    bool stats_c_barcode                = Util::statsOnColumn(frmProcess->job->getProjectID(), "cryovial",      "cryovial_barcode");
    bool stats_c_aliquot                = Util::statsOnColumn(frmProcess->job->getProjectID(), "cryovial",      "aliquot_type_cid");
    bool stats_cs_cryovial_id           = Util::statsOnColumn(frmProcess->job->getProjectID(), "cryovial_store","cryovial_id");
    bool stats_cs_status                = Util::statsOnColumn(frmProcess->job->getProjectID(), "cryovial_store","status");
    bool stats_cs_retrieval_cid         = Util::statsOnColumn(frmProcess->job->getProjectID(), "cryovial_store","retrieval_cid");

    stats = stats_c_barcode && stats_c_aliquot && stats_cs_cryovial_id && stats_cs_status && stats_cs_retrieval_cid;

    if (!stats) {
        oss.str(""); oss<<"missing stats on "
            <<(stats_c_barcode        ? "cryovial_barcode" : "")
            <<(stats_c_aliquot        ? "aliquot_type_cid" : "")
            <<(stats_cs_cryovial_id   ? "cryovial_id"      : "")
            <<(stats_cs_status        ? "status"           : "")
            <<(stats_cs_retrieval_cid ? "retrieval_cid"    : "");
        debugMessage = oss.str();
        Synchronize((TThreadMethod)&msgbox);
        Synchronize((TThreadMethod)&debugLog);
    } else {
        debugMessage = "have stats";
        Synchronize((TThreadMethod)&debugLog);
    }

    LQuery ql(Util::projectQuery(frmProcess->job->getProjectID(), false)); // no ddb
    ql.setSQL(
        " SELECT"
        "     g.retrieval_cid, g.chunk, g.rj_box_cid, g.cbr_status, g.dest_pos, g.lcr_slot, g.lcr_procid, g.lcr_status, g.box_id AS dest_id,"
        "     c.cryovial_barcode, c.sample_id, c.aliquot_type_cid, c.note_exists AS cryovial_note,"
        "     s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position, s1.record_id,"
        "     s1.status, s1.tube_position, s1.note_exists AS cs_note,"
        "     b1.external_name AS src_box, "
        "     b2.external_name AS dest_name,"
        "     s2.tube_position AS slot_number, s2.status AS dest_status"
        " FROM "
        "     :temp g, cryovial c, cryovial_store s1, cryovial_store s2, box_name b1, box_name b2"
        " WHERE"
        "     c.cryovial_barcode = g.cryovial_barcode"
        "     AND c.aliquot_type_cid = g.aliquot_type_cid"
        "     AND s1.cryovial_id = c.cryovial_id"
        "     AND s1.retrieval_cid = g.retrieval_cid"
        "     AND b2.box_cid = g.box_id"
        "     AND b1.box_cid = s1.box_cid"
        "     AND s2.cryovial_id = c.cryovial_id"
        "     AND b2.box_cid = s2.box_cid"
        " ORDER BY"
        "     s1.retrieval_cid, chunk, g.rj_box_cid, dest_pos"
    );
    ql.setParam("temp", tempTableName);
    rowCount = 0; // class variable needed for synchronise
    ql.open();
    int curchunk = 0, chunk = 0;
    while (!ql.eof()) {
        chunk = ql.readInt("chunk");
        //wstringstream oss; oss<<__FUNC__<<oss<<"chunk:"<<chunk<<", rowCount: "<<rowCount; OutputDebugString(oss.str().c_str());
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
            new LPDbCryovial(ql),
            new LPDbCryovialStore(ql),
            new LCDbCryovialRetrieval(ql), // fixme
            ql.readString(  "cryovial_barcode"),
            Util::getAliquotDescription(ql.readInt("aliquot_type_cid")),
            ql.readString(  "src_box"),
            ql.readInt(     "dest_id"),
            ql.readString(  "dest_name"),
            ql.readInt(     "dest_pos"),
            "", 0, "", 0, 0, "", 0 ); // no storage details yet
        frmProcess->vials.push_back(row);
        ql.next();
        rowCount++;
    }

    wstringstream wss; wss<<"finished loading "<<rowCount<<"samples";
    debugMessage = oss.str();
    Synchronize((TThreadMethod)&debugLog);

    //qd.setSQL("DROP "TEMP_TABLE_NAME);
    ql.setSQL("DROP :temp");
    ql.setParam("temp", tempTableName);
    if (!RETRASSTDEBUG) ql.execSQL();

    debugMessage = "finished drop temp table";
    Synchronize((TThreadMethod)&debugLog);

    frmProcess->chunks[frmProcess->chunks.size()-1]->setEnd(frmProcess->vials.size()-1);

    // find locations of source boxes
    map<int, const SampleRow *> samples; ROSETTA result; StoreDAO dao;
    int rowCount2 = 0;
	for (vector<SampleRow *>::iterator it = frmProcess->vials.begin(); it != frmProcess->vials.end(); ++it, rowCount2++) {
        SampleRow * sample = *it;
        ostringstream oss; oss<<"Finding storage for "<<sample->cryovial_barcode<<" ["<<rowCount2<<"/"<<rowCount<<"]: ";
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
    debugMessage = "finished load storage details";
    Synchronize((TThreadMethod)&debugLog);
}

void __fastcall TfrmProcess::loadPlanWorkerThreadTerminated(TObject *Sender) {
    progressBottom->Style = pbstNormal; progressBottom->Visible = false;
    panelLoading->Visible = false;
    Screen->Cursor = crDefault;
    showChunks();
    currentChunk()->setCurrentRow(0); //currentChunk = 0;
    showCurrentRow();
    Enabled = true;
    wstringstream oss; oss<<__FUNC__<<"loadRows for job "<<job->getID()<<" finished";
    debugLog(oss.str().c_str());
}

void TfrmProcess::showCurrentRow() {
    SampleRow * sample;
    int rowIdx = currentChunk()->getCurrentRow();
    if (rowIdx == currentChunk()->getSize()) {  // ie. past the end, chunk completed
        sample = NULL;              // no details to show
        sgVials->Row = rowIdx;      // just show the last row
    } else {
        sample = currentChunk()->rowAt(rowIdx);
        sgVials->Row = rowIdx+1;    // allow for header row
    }
    showRowDetails(sample);
}

void TfrmProcess::showRowDetails(SampleRow * sample) {
    if (NULL == sample) {
        labelSampleID->Caption  = "";
        labelStorage->Caption   = "Chunk completed";
        labelDestbox->Caption   = "";
    } else {
        labelSampleID->Caption  = sample->cryovial_barcode.c_str();
        labelStorage->Caption   = sample->storage_str().c_str();
        labelDestbox->Caption   = sample->dest_str().c_str();
    }
}

void TfrmProcess::addChunk(int row) {
    Chunk< SampleRow > * newchunk;
    if (chunks.size() == 0) { // first chunk, make default chunk from entire listrows
        newchunk = new Chunk< SampleRow >(sgwVials, chunks.size()+1, 0, row); // empty chunk, don't know how big it will be yet
    } else {
        chunks[chunks.size()-1]->setEnd(row-1);
        newchunk = new Chunk< SampleRow >(sgwVials, chunks.size()+1, row, row);
    }
    chunks.push_back(newchunk);
}

// LCDbBoxRetrieval::Status::NEW|PART_FILLED|COLLECTED|NOT_FOUND|DELETED
// LCDbCryovialRetrieval::Status::EXPECTED|IGNORED|COLLECTED|NOT_FOUND

void __fastcall TfrmProcess::btnAcceptClick(TObject *Sender) {
    accept(editBarcode->Text);
}

void __fastcall TfrmProcess::btnSimAcceptClick(TObject *Sender) {
    editBarcode->Text = currentChunk()->rowAt(currentChunk()->getCurrentRow())->cryovial_barcode.c_str();
    btnAcceptClick(this);
}

void TfrmProcess::accept(String barcode) {
    // check correct vial; could be missing, swapped etc
    SampleRow * sample = currentSample();
    switch (sample->retrieval_record->getStatus()) {
        case LCDbCryovialRetrieval::EXPECTED:
        case LCDbCryovialRetrieval::IGNORED:
            break; // ok, carry on
        case LCDbCryovialRetrieval::COLLECTED:
            msgbox("Already collected"); return;
        case LCDbCryovialRetrieval::NOT_FOUND:
            if (IDOK != Application->MessageBox(L"Confirm sample has now been found", L"Question", MB_OKCANCEL)) {
                return;
            }
    }
    if (barcode == sample->cryovial_barcode.c_str()) { // save
        sample->retrieval_record->setStatus(LCDbCryovialRetrieval::COLLECTED);
        debugLog("Save accepted row"); //Application->MessageBox(L"Save accepted row", L"Info", MB_OK);
        nextRow();
    } else {
        Application->MessageBox(L"Barcode not matched", L"Info", MB_OK);
    }
}

void __fastcall TfrmProcess::btnSkipClick(TObject *Sender) {
    debugLog("Save skipped row"); //Application->MessageBox(L"Save skipped row", L"Info", MB_OK);
    currentSample()->retrieval_record->setStatus(LCDbCryovialRetrieval::IGNORED);
    nextRow();
}

void __fastcall TfrmProcess::btnNotFoundClick(TObject *Sender) {
    debugLog("Save not found row"); //Application->MessageBox(L"Save not found row", L"Info", MB_OK);
    currentSample()->retrieval_record->setStatus(LCDbCryovialRetrieval::NOT_FOUND);
    nextRow();
}

SampleRow * TfrmProcess::currentSample() {
    Chunk< SampleRow > * chunk = currentChunk();
    int current = chunk->getCurrentRow();
    SampleRow * sample = chunk->rowAt(current);
    return sample;
}

void TfrmProcess::nextRow() {
    Chunk< SampleRow > * chunk = currentChunk();
    int current = chunk->getCurrentRow();
    SampleRow * sample = chunk->rowAt(current);

    //sample->retrieval_record->saveRecord(LIMSDatabase::getProjectDb());
    if (current < chunk->getSize()-1) {
        chunk->setCurrentRow(current+1); //???
        showCurrentRow();
    } else { // skipped last row
        chunk->setCurrentRow(current+1); // past end to show complete?
        debugLog("Save chunk"); // no, don't save - completedness or otherwise of 'chunk' should be implicit from box/cryo plan
        if (chunk->getSection() < chunks.size()) {
            sgChunks->Row = sgChunks->Row+1; // next chunk
        } else {
            if (IDYES != Application->MessageBox(L"Save job? Are all chunks completed?", L"Info", MB_YESNO)) return;
            Application->MessageBox(L"Handle disposal of empty boxes", L"Info", MB_OK);
        }
    }
    showChunks();
    editBarcode->Clear();
    ActiveControl = editBarcode; // focus for next barcode
}

void TfrmProcess::exit() {
    if (IDYES == Application->MessageBox(L"Are you sure you want to exit?\n\nCurrent progress will be saved.", L"Question", MB_YESNO)) {
        Application->MessageBox(L"Save completed boxes", L"Info", MB_OK);
        Application->MessageBox(L"Signoff form (or on open form?)", L"Info", MB_OK);
        // how to update boxes? check at save and exit that all vials in a box have been saved?
        Close();
    }
}

void __fastcall TfrmProcess::editBarcodeKeyUp(TObject *Sender, WORD &Key, TShiftState Shift) {
    if (VK_RETURN == Key) {
        accept(editBarcode->Text);
    }
}

void __fastcall TfrmProcess::FormResize(TObject *Sender) {
    if (sgwChunks) sgwChunks->resize(); // in case has been deleted in FormDestroy
    if (sgwVials)  sgwVials->resize();
}

