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
    sgwVials->addCol("barcode",  "Barcode",          91,    SampleRow::sort_asc_barcode,    "barcode");
    sgwVials->addCol("aliquot",  "Aliquot",          90,    SampleRow::sort_asc_aliquot,    "aliquot");
    sgwVials->addCol("currbox",  "Current box",      257,   SampleRow::sort_asc_currbox,    "source box name");
    sgwVials->addCol("currpos",  "Pos",              31,    SampleRow::sort_asc_currpos,    "source box position");
    sgwVials->addCol("site",     "Site",             120,   SampleRow::sort_asc_site,       "site name");
    sgwVials->addCol("vesspos",  "Pos",              28,    SampleRow::sort_asc_vesspos,    "vessel position");
    sgwVials->addCol("vessel",   "Vessel",           107,   SampleRow::sort_asc_vessel,     "vessel name");
    sgwVials->addCol("shelf",    "Shelf",            31,    SampleRow::sort_asc_shelf,      "shelf number");
    sgwVials->addCol("structpos","Pos",              27,    SampleRow::sort_asc_structpos,  "structure position");
    sgwVials->addCol("struct",   "Structure",        123,   SampleRow::sort_asc_structure,  "structure name");
    sgwVials->addCol("boxpos",   "Slot",             26,    SampleRow::sort_asc_slot,       "slot");
    sgwVials->addCol("destbox",  "Destination box",  267,   SampleRow::sort_asc_destbox,    "dest. box name");
    sgwVials->addCol("destpos",  "Pos",              25,    SampleRow::sort_asc_destpos,    "dest. box position");
    sgwVials->init();
}

void __fastcall TfrmProcess::FormCreate(TObject *Sender) {
    cbLog->Visible      = RETRASSTDEBUG;
    cbLog->Checked      = RETRASSTDEBUG;
    panelDebug->Visible = cbLog->Checked;
    job                 = NULL;
    loadingMessage = "Loading retrieval list, please wait...";
}

void __fastcall TfrmProcess::FormDestroy(TObject *Sender) {
    delete sgwChunks;
    delete sgwVials;
}

void __fastcall TfrmProcess::FormShow(TObject *Sender) {
    ostringstream oss; oss<<job->getName()<<" : "<<job->getDescription()<<" [id: "<<job->getID()<<"]";
    Caption = oss.str().c_str();
    timerLoadPlan->Enabled = true;
    panelLoading->Caption = loadingMessage;
    chunks.clear();
    sgwChunks->clear();
    sgwVials->clear();
    labelStorage->Caption   = "loading...";
    labelSampleID->Caption  = "loading...";
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
    sgwChunks->clear();
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
    sgVials->Row = 1;
    //showRowDetails(sampleRow);
    showCurrentRow();
    Screen->Cursor = crDefault; Enabled = true;
}

void TfrmProcess::process() {
/*
 * Work through list or sub-section by giving the storage location and sample ID of each sample on the list in the order saved above (REQ 8.3.8);
 * As each sample is retrieved its barcode should be scanned, if the scanned barcode matches that on the list
the destination location should be displayed and the next ID/location should be displayed (REQ 8.3.9)
 * if the ID�s do not match a warning should be displayed and re-entry of the barcode required (REQ 8.3.10).
     what if it's the wrong sample, or it's missing? handle this
 * When working through the list the previous five successfully entered ID�s should always be visible (REQ 8.3.11).
 * The option to exit the process saving progress should be offered, with an �are you sure?� message in case of accidental selection (REQ 8.3.12).

extra:
 * if, at the end of processing a chunk, there are any source boxes which have become empty, the user may want to discard them instead of replacing them.
   if so, provide an option to discard these empty boxes, recording it in the database

 destination box+position, cryovial barcode and current box+position+structure+location of the primary and secondary aliquots.

*/
}

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
/** load retrieval plan

For a box retrieval, the retrieval plan will be given by
Select * from c_box_retrieval b order by b.section, b.rj_box_cid

For a cryovial retrieval, the retrieval plan will be:
Select * from c_box_retrieval b, l_cryovial_retrieval c where b.rj_box_cid = c.rj_box_cid order by b.section, c.position
*/
    delete_referenced< vector<SampleRow * > >(frmProcess->vials);
    frmProcess->chunks.clear();
    ostringstream oss; oss<<frmProcess->loadingMessage<<" (preparing query)"; loadingMessage = oss.str().c_str(); //return;
    if (NULL != frmProcess && NULL != frmProcess->job) { frmProcess->job = frmProcess->job; } else { throw "wtf?"; }

    LQuery qd(Util::projectQuery(frmProcess->job->getProjectID(), true)); // ddb
    qd.setSQL( // from spec 2013-09-11
        " SELECT"
        "    s1.retrieval_cid,cbr.section as chunk, cbr.rj_box_cid, lcr.position as dest_pos, cbr.status as cbr_status,"
        "    s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position," // for LPDbCryovialStore
        "    s1.record_id, c.cryovial_barcode, c.sample_id, c.aliquot_type_cid, c.note_exists as cryovial_note,"
        "    s1.box_cid, b1.external_name as src_box, s1.status, s1.tube_position, s1.note_exists as cs_note,"
        "    cbr.box_id as dest_id, b2.external_name as dest_name, s2.tube_position as slot_number, s2.status as dest_status"
        " FROM"
        "    c_box_retrieval cbr, l_cryovial_retrieval lcr, cryovial c, cryovial_store s1, box_name b1, cryovial_store s2, box_name b2,"
        "    c_object_name t"
        " WHERE"
        "    cbr.retrieval_cid = :rtid AND"
        "    s1.retrieval_cid = cbr.retrieval_cid AND"
        "    lcr.rj_box_cid = cbr.rj_box_cid AND"
        "    lcr. cryovial_barcode = c.cryovial_barcode AND lcr.aliquot_type_cid = c.aliquot_type_cid AND"
        "    b2.box_cid = cbr.box_id AND"
        "    t.object_cid = c.aliquot_type_cid AND"
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
    int curchunk = 0, chunk = 0;
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
            // new LCDbCryovialRetrieval(qd), // fixme
            qd.readString(  "cryovial_barcode"),
            Util::getAliquotDescription(qd.readInt("aliquot_type_cid")),
            qd.readString(  "src_box"),
            qd.readInt(     "dest_id"),
            qd.readString(  "dest_name"),
            qd.readInt(     "dest_pos"),
            "", 0, "", 0, 0, "", 0 ); // no storage details yet
        frmProcess->vials.push_back(row);
        qd.next();
        rowCount++;
    }
    frmProcess->chunks[frmProcess->chunks.size()-1]->setEnd(frmProcess->vials.size()-1);

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
}

void __fastcall TfrmProcess::loadPlanWorkerThreadTerminated(TObject *Sender) {
    progressBottom->Style = pbstNormal; progressBottom->Visible = false;
    panelLoading->Visible = false;
    Screen->Cursor = crDefault;
    showChunks();
    //SampleRow * sample = currentChunk()->rowAt(0);
    currentChunk()->setCurrentRow(0);
    //currentChunk = 0;
    showCurrentRow();
    Enabled = true;
}

void TfrmProcess::showCurrentRow() {
    SampleRow * row = currentChunk()->rowAt(currentChunk()->getCurrentRow());
    showRowDetails(row);
    sgVials->Row = currentChunk()->getCurrentRow()+1; // causes double header row!
}

void TfrmProcess::showRowDetails(SampleRow * sample) {
    labelStorage->Caption   = sample->storage_str().c_str();//"loading...";
    labelSampleID->Caption  = sample->cryovial_barcode.c_str();//"loading...";
    //sgVials->Row = currentChunk()->getCurrentRow();
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
    // check correct vial; could be missing, swapped etc
    SampleRow * sample = currentChunk()->rowAt(currentChunk()->getCurrentRow());
    if (editBarcode->Text == sample->cryovial_barcode.c_str()) {
        // save
        sample->retrieval_record->setStatus(LCDbCryovialRetrieval::COLLECTED);
        Application->MessageBox(L"Save accepted row", L"Info", MB_OK);
        nextRow();
    } else {
        //IDYES == Application->MessageBox(L"Are you sure you want to exit?\n\nCurrent progress will be saved.", L"Question", MB_YESNO)
        Application->MessageBox(L"Barcode not matched", L"Info", MB_OK);
    }
}

void __fastcall TfrmProcess::btnSimAcceptClick(TObject *Sender) {
    editBarcode->Text = currentChunk()->rowAt(currentChunk()->getCurrentRow())->cryovial_barcode.c_str();
    btnAcceptClick(this);
}

void __fastcall TfrmProcess::btnSkipClick(TObject *Sender) {
    Application->MessageBox(L"Save skipped row", L"Info", MB_OK);
    nextRow();
}

void __fastcall TfrmProcess::btnNotFoundClick(TObject *Sender) {
    Application->MessageBox(L"Save not found row", L"Info", MB_OK);
    nextRow();
}

Chunk< SampleRow >::Status TfrmProcess::chunkStatus(Chunk< SampleRow > * chunk) {
    bool complete = true;
    bool not_started = true;
    for (int i=0; i<chunk->getSize(); i++) {
        int status = chunk->rowAt(i)->retrieval_record->getStatus();
        switch (status) {
            case LCDbCryovialRetrieval::EXPECTED:
                complete = false; break;
            case LCDbCryovialRetrieval::IGNORED:
            case LCDbCryovialRetrieval::COLLECTED:
            case LCDbCryovialRetrieval::NOT_FOUND:
                not_started = false; break;
            default:
                throw "unexpected LCDbCryovialRetrieval status";
        }
    }
    if (complete) {
        return Chunk< SampleRow >::DONE;
    } else if (not_started) {
        return Chunk< SampleRow >::NOT_STARTED;
    } else {
        return Chunk< SampleRow >::INPROGRESS;
    }
}

void TfrmProcess::nextRow() {
    if (currentChunk()->getCurrentRow() < currentChunk()->getSize()-1) {
        currentChunk()->setCurrentRow(currentChunk()->getCurrentRow()+1); //???
        showCurrentRow();
    } else { // skipped last row
        Application->MessageBox(L"Save chunk", L"Info", MB_OK);

        if (sgChunks->Row < sgChunks->RowCount) { // if (sgChunks->Row == chunks.size()) {
            sgChunks->Row++; // next chunk
        } else {
            // at the end - save?
            //Application->MessageBox(L"Are all chunks completed?", L"Info", MB_OK);
            Application->MessageBox(L"Save job?", L"Info", MB_OK);
        }
    }
}

void TfrmProcess::exit() {
    if (IDYES == Application->MessageBox(L"Are you sure you want to exit?\n\nCurrent progress will be saved.", L"Question", MB_YESNO)) {
        // save stuff
        Application->MessageBox(L"Save completed boxes", L"Info", MB_OK);
        // how to update boxes? check at save and exit that all vials in a box have been saved?
        Close();
    }
}
