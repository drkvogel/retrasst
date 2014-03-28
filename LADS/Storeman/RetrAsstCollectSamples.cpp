#include <vcl.h>
#pragma hdrstop
#include "StoreDAO.h"
#include "RetrAsstCollectSamples.h"
#include "LCDbAuditTrail.h"
#include <ctime>
    // struct tm
#include <locale>
    // locale, time_put
#include "TfrmConfirm.h"
#include "SMLogin.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

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

TfrmProcess *frmProcess;

__fastcall TfrmProcess::TfrmProcess(TComponent* Owner) : TForm(Owner) {
    destroying = false;
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
    sgwVials->addCol("item",     "Item",             30);
    sgwVials->addCol("barcode",  "Barcode",          91);
    sgwVials->addCol("site",     "Site",             90);
    sgwVials->addCol("vesspos",  "Pos",              28);
    sgwVials->addCol("vessel",   "Vessel",           107);
    sgwVials->addCol("shelf",    "Shelf",            31);
    sgwVials->addCol("structpos","Pos",              27);
    sgwVials->addCol("struct",   "Structure",        100);
    sgwVials->addCol("boxpos",   "Slot",             26);
    sgwVials->addCol("srcbox",   "Source box",      257);
    sgwVials->addCol("srcpos",   "Pos",              31);
    sgwVials->addCol("destbox",  "Destination box",  240);
    sgwVials->addCol("destpos",  "Pos",              25);
#ifdef _DEBUG
    sgwVials->addCol("status",   "Status",           91);
    sgwVials->addCol("aliquot",  "Aliquot",          90);
#endif
    sgwVials->init();
}

__fastcall TfrmProcess::~TfrmProcess() {
    destroying = true;
    delete sgwChunks;
    delete sgwVials;
}

void __fastcall TfrmProcess::FormCreate(TObject *Sender) {
    cbLog->Visible      = RETRASSTDEBUG;
    cbLog->Checked      = RETRASSTDEBUG;
    sgVials->Enabled    = RETRASSTDEBUG;
    panelDebug->Visible = cbLog->Checked;
    job                 = NULL;
    loadingMessage = "Loading retrieval list, please wait...";
}

void __fastcall TfrmProcess::FormClose(TObject *Sender, TCloseAction &Action) {
    //exit(); //???
}

void TfrmProcess::debugLog(String s) {
    String tmp = Now().CurrentDateTime().DateTimeString() + ": " + s;
    memoDebug->Lines->Add(tmp); // could use varargs: http://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c
}

void __fastcall TfrmProcess::FormShow(TObject *Sender) {
    ostringstream oss; oss<<job->getName()<<" : "<<job->getDescription()<<" [id: "<<job->getID()<<"]";
    Caption = oss.str().c_str();
    panelLoading->Caption = loadingMessage;
    chunks.clear();
    sgwChunks->clear();
    sgwVials->clear();
    frmRetrievalAssistant->clearStorageCache();
    labelSampleID->Caption  = "loading...";
    labelStorage->Caption   = "loading...";
    labelDestbox->Caption   = "loading...";
    labelPrimary->Caption   = Util::getAliquotDescription(job->getPrimaryAliquot()).c_str();
    labelSecondary->Caption = Util::getAliquotDescription(job->getSecondaryAliquot()).c_str();
    labelPrimary->Enabled   = true;
    labelSecondary->Enabled = false;
    timerLoadPlan->Enabled = true;
}

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
                    background = RETRIEVAL_ASSISTANT_COLLECTED_COLOUR; break;
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
            background = clWindow; // whilst loading
        } else {
            int status = row->retrieval_record->getStatus();
            switch (status) {
                // could use currentAliquot() here?
                case LCDbCryovialRetrieval::EXPECTED:
                    background = RETRIEVAL_ASSISTANT_NEW_COLOUR; break;
                case LCDbCryovialRetrieval::IGNORED:
                    background = RETRIEVAL_ASSISTANT_IGNORED_COLOUR; break;
                case LCDbCryovialRetrieval::COLLECTED:
                    background = RETRIEVAL_ASSISTANT_COLLECTED_COLOUR; break;
                case LCDbCryovialRetrieval::NOT_FOUND:
                    if (NULL != row->secondary) {
                        int secondaryStatus = row->secondary->retrieval_record->getStatus();
                        switch (secondaryStatus) {
                            case LCDbCryovialRetrieval::EXPECTED:
                                background = RETRIEVAL_ASSISTANT_SECONDARY_COLOUR; break;
                            case LCDbCryovialRetrieval::IGNORED:
                                background = RETRIEVAL_ASSISTANT_IGNORED_COLOUR; break;
                            case LCDbCryovialRetrieval::COLLECTED:
                                background = RETRIEVAL_ASSISTANT_COLLECTED_COLOUR; break;
                            default:
                                background = RETRIEVAL_ASSISTANT_NOT_FOUND_COLOUR; break;
                        }
                    } else {
                        background = RETRIEVAL_ASSISTANT_NOT_FOUND_COLOUR; break;
                    }
                    break;
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

void __fastcall TfrmProcess::sgChunksFixedCellClick(TObject *Sender, int ACol, int ARow) {
    ostringstream oss; oss << __FUNC__; oss<<sgwChunks->printColWidths()<<" clicked on col: "<<ACol<<"."; //debugLog(oss.str().c_str());
}

void __fastcall TfrmProcess::sgChunksClick(TObject *Sender) {
    int row = sgChunks->Row;
    if (row < 1) return;
    Chunk< SampleRow > * chunk = (Chunk< SampleRow > *)sgChunks->Objects[0][row];
    showChunk(chunk);
}

void __fastcall TfrmProcess::sgVialsClick(TObject *Sender) { // show details in debug window
    SampleRow * sample = (SampleRow *)sgVials->Objects[0][sgVials->Row];
    DEBUGSTREAM(__FUNC__
        <<" retrieval_status: "<<sample->retrieval_record->getStatus()
        <<" ("<<sample->retrieval_record->statusString(sample->retrieval_record->getStatus())<<")"
        <<", cryo_status: "<<sample->cryo_record->getStatus()
        <<", store_status: "<<sample->store_record->getStatus()
        <<", barcode: "<<sample->cryovial_barcode.c_str()
        <<", storage: "<<sample->storage_str().c_str()
        <<", dest: "<<sample->dest_str().c_str()
        )
    SampleRow * secondary = sample->secondary;
    if (!secondary) {
        DEBUGSTREAM(" (no secondary)")
        return;
    }
    DEBUGSTREAM(
        " (secondary) retrieval_status: "<<secondary->retrieval_record->getStatus()
        <<" ("<<secondary->retrieval_record->statusString(secondary->retrieval_record->getStatus())<<")"
        <<", cryo_status: "<<secondary->cryo_record->getStatus()
        <<", store_status: "<<secondary->store_record->getStatus()
        <<", barcode: "<<secondary->cryovial_barcode.c_str()
        <<", storage: "<<secondary->storage_str().c_str()
        <<", dest: "<<secondary->dest_str().c_str()
        )
    //int row = sgVials->Row; sgVials->Row = row; // how to put these before and after to save row clicked on?
}

void __fastcall TfrmProcess::editBarcodeKeyUp(TObject *Sender, WORD &Key, TShiftState Shift) {
    if (VK_RETURN == Key) { accept(editBarcode->Text); }
}

void __fastcall TfrmProcess::FormResize(TObject *Sender) { // gets called *after* FormDestroy
    if (!destroying) { // check in case pointers have been deleted in FormDestroy
        sgwChunks->resize(); sgwVials->resize();
    }
}

void __fastcall TfrmProcess::cbLogClick(TObject *Sender) { panelDebug->Visible = cbLog->Checked; }

void __fastcall TfrmProcess::menuItemExitClick(TObject *Sender) { checkExit(); }

void __fastcall TfrmProcess::btnExitClick(TObject *Sender) { checkExit(); } //exit(); }

void __fastcall TfrmProcess::btnAcceptClick(TObject *Sender) { accept(editBarcode->Text); }

void __fastcall TfrmProcess::btnSimAcceptClick(TObject *Sender) { // simulate a correct barcode scanned
    editBarcode->Text = (currentChunk()->currentObject())->cryovial_barcode.c_str();
    btnAcceptClick(this);
}

void __fastcall TfrmProcess::btnNotFoundClick(TObject *Sender) { notFound(); }

void __fastcall TfrmProcess::btnSkipClick(TObject *Sender) { skip(); }

void TfrmProcess::showChunks() {
    if (0 == chunks.size()) { throw Exception("No chunks"); } // must always have one chunk anyway
    else { sgChunks->RowCount = chunks.size() + 1; sgChunks->FixedRows = 1; } // "Fixed row count must be LESS than row count"
    int row = 1;
    for (vector< Chunk< SampleRow > * >::const_iterator it = chunks.begin(); it != chunks.end(); it++, row++) {
        Chunk< SampleRow > * chunk = *it;
        sgChunks->Cells[sgwChunks->colNameToInt("section")]   [row] = chunk->getSection();
        sgChunks->Cells[sgwChunks->colNameToInt("status")]    [row] = chunk->statusString().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("progress")]  [row] = chunk->progressString().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("start")]     [row] = chunk->getStartAbs() + 1;
        sgChunks->Cells[sgwChunks->colNameToInt("startbox")]  [row] = chunk->getStartBox().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("startvial")] [row] = chunk->getStartVial().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("end")]       [row] = chunk->getEndAbs() + 1;
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
        SampleRow *         sampleRow = chunk->objectAtRel(row);
        fillRow(sampleRow, row+1); // row+1 for stringgrid
    }
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

void TfrmProcess::fillRow(SampleRow * row, int rw) {
    SampleRow * sample;
    if (    row->retrieval_record->getStatus() == LCDbCryovialRetrieval::NOT_FOUND
        &&  row->secondary != NULL) {
        sample = row->secondary;
    } else {
        sample = row;
    }
    sgVials->Cells[sgwVials->colNameToInt("item")]     [rw] = rw;//??sample->cryovial_barcode.c_str();
    sgVials->Cells[sgwVials->colNameToInt("barcode")]  [rw] = sample->cryovial_barcode.c_str();
    sgVials->Cells[sgwVials->colNameToInt("status") ]  [rw] = LCDbCryovialRetrieval::statusString(sample->retrieval_record->getStatus());
    sgVials->Cells[sgwVials->colNameToInt("aliquot")]  [rw] = sample->aliquotName().c_str();
    sgVials->Cells[sgwVials->colNameToInt("srcbox")]   [rw] = sample->src_box_name.c_str();
    sgVials->Cells[sgwVials->colNameToInt("srcpos")]   [rw] = sample->store_record->getPosition();
    sgVials->Cells[sgwVials->colNameToInt("site"   )]  [rw] = sample->site_name.c_str();
    sgVials->Cells[sgwVials->colNameToInt("vesspos")]  [rw] = sample->vessel_pos;
    sgVials->Cells[sgwVials->colNameToInt("vessel" )]  [rw] = sample->vessel_name.c_str();
    sgVials->Cells[sgwVials->colNameToInt("shelf"  )]  [rw] = sample->shelf_number;
    sgVials->Cells[sgwVials->colNameToInt("structpos")][rw] = sample->structure_pos;
    sgVials->Cells[sgwVials->colNameToInt("struct" )]  [rw] = sample->structure_name.c_str();
    sgVials->Cells[sgwVials->colNameToInt("boxpos" )]  [rw] = sample->box_pos;
    sgVials->Cells[sgwVials->colNameToInt("destbox")]  [rw] = sample->dest_box_name.c_str();
    sgVials->Cells[sgwVials->colNameToInt("destpos")]  [rw] = sample->dest_cryo_pos;
    sgVials->Objects[0][rw] = (TObject *)row; // keep all data, primary and secondary
}

void __fastcall TfrmProcess::timerLoadPlanTimer(TObject *Sender) {
    timerLoadPlan->Enabled = false;
    loadPlan();
}

void TfrmProcess::loadPlan() {
    panelLoading->Caption = loadingMessage;
    panelLoading->Visible = true; // appearing in wrong place because called in OnShow, form not yet maximized
    panelLoading->Top = (sgVials->Height / 2) - (panelLoading->Height / 2);
    panelLoading->Left = (sgVials->Width / 2) - (panelLoading->Width / 2);
    progressBottom->Style = pbstMarquee; progressBottom->Visible = true;
    Screen->Cursor = crSQLWait; // disable mouse? //ShowCursor(false);
    DEBUGSTREAM("loadRows for job "<<job->getID()<<" started")
    Enabled = false;
    loadPlanThread = new LoadPlanThread();
    loadPlanThread->OnTerminate = &loadPlanThreadTerminated;
}

__fastcall LoadPlanThread::LoadPlanThread() : TThread(false) { FreeOnTerminate = true; }

__fastcall SaveProgressThread::SaveProgressThread() : TThread(false) { FreeOnTerminate = true; }

void __fastcall LoadPlanThread::updateStatus() { // can't use args for synced method, don't know why
    frmProcess->panelLoading->Caption = loadingMessage.c_str(); frmProcess->panelLoading->Repaint();
}

void __fastcall SaveProgressThread::updateStatus() {
    frmProcess->panelLoading->Caption = loadingMessage.c_str(); frmProcess->panelLoading->Repaint();
}

void __fastcall LoadPlanThread::debugLog() { frmProcess->debugLog(debugMessage.c_str()); }

void __fastcall LoadPlanThread::msgbox() { Application->MessageBox(String(debugMessage.c_str()).c_str(), L"Info", MB_OK); }

void __fastcall LoadPlanThread::Execute() { 
    /** load retrieval plan
    For a box retrieval, the retrieval plan will be given by: Select * from c_box_retrieval b order by b.section, b.rj_box_cid
    For a cryovial retrieval, the retrieval plan will be: Select * from c_box_retrieval b, l_cryovial_retrieval c where b.rj_box_cid = c.rj_box_cid order by b.section, c.position */
    delete_referenced< vector<SampleRow * > >(frmProcess->vials); frmProcess->chunks.clear();
    ostringstream oss; oss<<frmProcess->loadingMessage<<" (preparing query)"; loadingMessage = oss.str().c_str(); //return;
    if (NULL == frmProcess || NULL == frmProcess->job) { throw "wtf?"; }
    loadingMessage = frmProcess->loadingMessage;
    job = frmProcess->job; //const int pid = LCDbAuditTrail::getCurrent().getProcessID();

    int primary_aliquot = job->getPrimaryAliquot(); int secondary_aliquot = job->getSecondaryAliquot();

    debugMessage = "select sample details from plan"; Synchronize((TThreadMethod)&debugLog);
    LQuery qd(Util::projectQuery(job->getProjectID(), true)); // ddb
    oss.str("");
    oss<<
        " SELECT "
        "    cbr.retrieval_cid, section AS chunk, cbr.rj_box_cid, cbr.box_id AS dest_id, "//cbr.status, "
        "    lcr.position AS lcr_position, lcr.cryovial_barcode, lcr.aliquot_type_cid, "
        "    lcr.process_cid AS lcr_procid, lcr.status AS lcr_status, lcr.slot_number AS lcr_slot, "
        "    lcr.slot_number AS dest_pos, "
        "    cs.box_cid, sb.external_name AS src_box, cs.cryovial_position AS source_pos,  "
        "    db.external_name AS dest_box, "
        "    cs.note_exists, cs.cryovial_id, cs.cryovial_position, cs.status, "
        "    c.sample_id, cs.record_id, "
        "    db.external_name AS dest_name "
        " FROM "
        "    c_box_retrieval cbr, l_cryovial_retrieval lcr, c_box_name db, c_box_name sb, cryovial c, cryovial_store cs "
        " WHERE "
        "    cbr.retrieval_cid   = :rtid AND "
        "    cbr.rj_box_cid      = lcr.rj_box_cid AND "
        "    cbr.box_id          = db.box_cid AND "
        "    c.cryovial_barcode  = lcr.cryovial_barcode AND "
        "    c.aliquot_type_cid  = lcr.aliquot_type_cid  AND "
        "    cs.cryovial_id      = c.cryovial_id  AND "
        "    cbr.retrieval_cid   = cs.retrieval_cid AND "
        "    cs.box_cid          = sb.box_cid "
        " ORDER BY "
        "    chunk, rj_box_cid, lcr_position, aliquot_type_cid "
        << (primary_aliquot < secondary_aliquot ? "ASC" : "DESC"); //debugMessage = oss.str(); Synchronize((TThreadMethod)&debugLog);
    qd.setSQL(oss.str()); debugMessage = "open query"; Synchronize((TThreadMethod)&debugLog);
    qd.setParam("rtid", job->getID()); //int retrieval_cid = job->getID();
    qd.open();
    int rowCountTemp = 0;
    rowCount = 0; // class variable
    int curchunk = 1, chunk = 0; SampleRow * previous = NULL;
    debugMessage = "foreach row"; Synchronize((TThreadMethod)&debugLog);
    while (!qd.eof()) {
        if (0 == rowCount % 10) { ostringstream oss; oss<<"Found "<<rowCount<<" vials"; loadingMessage = oss.str().c_str(); Synchronize((TThreadMethod)&updateStatus); }

        chunk = qd.readInt("chunk"); //wstringstream oss; oss<<__FUNC__<<oss<<"chunk:"<<chunk<<", rowCount: "<<rowCount; OutputDebugString(oss.str().c_str());
        if (chunk > curchunk) { // new chunk, add the previous one
            frmProcess->addChunk(curchunk, rowCount-1);
            curchunk = chunk;
        }

        SampleRow * row = new SampleRow(
            new LPDbCryovial(qd),
            new LPDbCryovialStore(qd),
            new LCDbCryovialRetrieval(qd), // fixme
            qd.readString(  "cryovial_barcode"),
            qd.readString(  "src_box"),
            qd.readInt(     "dest_id"),
            qd.readString(  "dest_name"),
            qd.readInt(     "dest_pos"),
            "", 0, "", 0, 0, "", 0); // no storage details yet

        int currentAliquotType = row->cryo_record->getAliquotType();

        // primary_aliquot and secondary_aliquot are already defined
        int previousAliquotType = previous == NULL? 0 : previous->cryo_record->getAliquotType();
        if (secondary_aliquot != 0 &&
            secondary_aliquot == currentAliquotType &&
            previous != NULL &&
            previous->cryovial_barcode == row->cryovial_barcode) { // secondary aliquot, previous was primary of same sample
            if (previousAliquotType == currentAliquotType) {
                throw Exception("duplicate aliquot");
            } else if (currentAliquotType != secondary_aliquot) {
                throw Exception("spurious aliquot");
            } else { // secondary
                previous->secondary = row;
            }
        } else {
            frmProcess->vials.push_back(row); // new primary
            previous = row;
            rowCount++; // only count primary aliquots
        }
        qd.next();
        rowCountTemp++;
    } oss.str(""); oss<<"finished loading "<<rowCount<<" samples"; debugMessage = oss.str(); Synchronize((TThreadMethod)&debugLog);
    frmProcess->addChunk(curchunk, rowCount-1); // the last chunk
    if (0 == rowCount || 0 == frmProcess->chunks.size()) { return; } // something wrong here...

    // find locations of source boxes
    int rowCount2 = 0;
	for (vector<SampleRow *>::iterator it = frmProcess->vials.begin(); it != frmProcess->vials.end(); ++it, rowCount2++) {
        SampleRow * sample = *it;
        ostringstream oss; oss<<"Finding storage for "<<sample->cryovial_barcode<<" ["<<rowCount2<<"/"<<rowCount<<"]: ";
        frmRetrievalAssistant->getStorage(sample);
        if (NULL != sample->secondary) {
            frmRetrievalAssistant->getStorage(sample->secondary);
        }
        oss<<sample->storage_str(); loadingMessage = oss.str().c_str(); Synchronize((TThreadMethod)&updateStatus);
	} debugMessage = "finished load storage details"; Synchronize((TThreadMethod)&debugLog);
}

void __fastcall TfrmProcess::loadPlanThreadTerminated(TObject *Sender) {
    progressBottom->Style = pbstNormal; progressBottom->Visible = false;
    panelLoading->Visible = false;
    Screen->Cursor = crDefault;
    try {
        showChunks();
        //showCurrentRow();
    } catch (Exception & e) {
		TfrmRetrievalAssistant::msgbox(e.Message);
    }
    Enabled = true;
    DEBUGSTREAM(__FUNC__<<"loadRows for job "<<job->getID()<<" finished")
}

void TfrmProcess::showCurrentRow() {
    SampleRow * sample;
    Chunk<SampleRow> * chunk = currentChunk();
    chunk->setRowAbs(chunk->nextUnresolvedAbs()); // fast-forward to first non-dealt-with row
    int rowRel = chunk->getRowRel();

    if (rowRel == chunk->getSize()) {   // ie. past the end, chunk completed
        sample = NULL;                  // no details to show
        sgVials->Row = rowRel;          // just show the last row
    } else {
        int lookAhead = sgVials->VisibleRowCount / 2;
        if (rowRel + lookAhead < chunk->getSize() - 1) {
            sgVials->Row = 1;
            sgVials->Row = rowRel + lookAhead + 1;  // bodge to scroll next few samples into view; ScrollBy doesn't seem to work
        } else {                                    // fixme put into class
            sgVials->Row = sgVials->RowCount - 1;
        }
        sample = chunk->objectAtRel(rowRel);
        sgVials->Row = rowRel + 1;    // allow for header row
    }
    showDetails(sample);
}

void TfrmProcess::showDetails(SampleRow * sample) {
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

void TfrmProcess::addChunk(int number, int endRowAbs) { // don't assume chunk/section numbers instead of assuming it's the next number?
/** add chunk with section = number, start row = end of previous chunk + 1, end row = endRowAbs */
    Chunk< SampleRow > * newchunk; // Chunk(sgw, section, startAbs, endAbs, rowRel(0))
    if (chunks.size() == 0) { // first chunk
        newchunk = new Chunk< SampleRow >(sgwVials, number, 0, endRowAbs);
    } else {
        int previousEnd = chunks[chunks.size() - 1]->getEndAbs();
        newchunk = new Chunk< SampleRow >(sgwVials, number, previousEnd + 1, endRowAbs);
    }
    chunks.push_back(newchunk);
}

void TfrmProcess::accept(String barcode) { // fixme check correct vial; could be missing, swapped etc
    SampleRow * primary = currentSample();
    SampleRow * aliquot = currentAliquot();
    switch (aliquot->retrieval_record->getStatus()) {
        case LCDbCryovialRetrieval::EXPECTED:
        case LCDbCryovialRetrieval::IGNORED:
            break; // ok, carry on
        case LCDbCryovialRetrieval::COLLECTED:
			TfrmRetrievalAssistant::msgbox("Already collected - please inform Core Programming"); return;
        case LCDbCryovialRetrieval::NOT_FOUND:
            if (IDOK != Application->MessageBox(L"Confirm sample has now been found", L"Question", MB_OKCANCEL)) return;
    }
    if (barcode == aliquot->cryovial_barcode.c_str()) { // save
        aliquot->retrieval_record->setStatus(LCDbCryovialRetrieval::COLLECTED);
        if (aliquot == primary) {

        } else { // secondary

        }


        TfrmRetrievalAssistant::msgbox("save secondary");
        //sample->retrieval_record->setStatus(LCDbCryovialRetrieval::IGNORED); //???
        debugLog("Save accepted row");
        nextRow();
    } else {
        Application->MessageBox(L"Barcode not matched", L"Info", MB_OK);
    }
}

void TfrmProcess::skip() {
    debugLog("Save deferred row");
    currentAliquot()->retrieval_record->setStatus(LCDbCryovialRetrieval::IGNORED);
    nextRow();
}

void TfrmProcess::notFound() {
    DEBUGSTREAM("Save not found row")
    int rowAbs = currentChunk()->getRowAbs();
    SampleRow * sample = currentChunk()->objectAtAbs(rowAbs);
    if (sample->retrieval_record->getStatus() != LCDbCryovialRetrieval::NOT_FOUND) {
        sample->retrieval_record->setStatus(LCDbCryovialRetrieval::NOT_FOUND);
        if (sample->secondary) {
            TfrmRetrievalAssistant::msgbox("Secondary aliquot found");
            if (sample->secondary->retrieval_record->getStatus() != LCDbCryovialRetrieval::NOT_FOUND) { // secondary already marked not found
                fillRow(sample, rowAbs + 1); // refresh sg row - now keeps pointer to row
                showCurrentRow();
                showDetails(sample->secondary);
                return;
            } else {
                throw "secondary already NOT_FOUND";
            }
        } else {
            TfrmRetrievalAssistant::msgbox("No secondary aliquot exists, continuing to next sample");
            nextRow();
        }
    } else {  // primary already marked not found
        if (sample->secondary) {
            sample->secondary->retrieval_record->setStatus(LCDbCryovialRetrieval::NOT_FOUND);
            nextRow();
        } else {
            throw "no secondary for not found primary, should have moved on to next row";
        }
    }
}

SampleRow * TfrmProcess::currentSample() {
    return currentChunk()->objectAtRel(currentChunk()->getRowRel());  //return currentChunk()->currentObject();
}

SampleRow * TfrmProcess::currentAliquot() {
    SampleRow * row = currentSample();
    if (row->retrieval_record->getStatus() == LCDbCryovialRetrieval::NOT_FOUND) {
        if (NULL == row->secondary) {
            return NULL;
        } else {
            return row->secondary;
        }
    } else {
        return row;
    }
}

void TfrmProcess::nextRow() {
    Chunk< SampleRow > * chunk = currentChunk();
    int current = chunk->getRowRel(); //SampleRow * sample = currentAliquot(); // which may be the secondary aliquot //
    SampleRow * sample = chunk->currentObject();

/*  * make sure both aliquots are dealt with whatever happens
    * save both primary and secondary - secondary aliquots should always be saved if present
    * accept():
        * if primary aliquot !collected # expected, ignored, not found (now found?)
    * save secondary as `IGNORED` if not required? primary was */

    // save both primary and secondary
    if (!sample->retrieval_record->saveRecord(LIMSDatabase::getCentralDb())) { throw "saveRecord() failed"; }
    if (sample->secondary) {
        if (!sample->secondary->retrieval_record->saveRecord(LIMSDatabase::getCentralDb())) { throw "saveRecord() failed for secondary"; }
    }
    if (current < chunk->getSize()-1) {
        chunk->setRowAbs(chunk->nextUnresolvedAbs()); // fast-forward to first non-dealt-with row
    } else { // last row
        //chunk->setRowRel(current+1); // past end to show complete?
        TfrmRetrievalAssistant::msgbox("review");
        debugLog("Save chunk"); // no, don't save - completedness or otherwise of 'chunk' should be implicit from box/cryo plan
        if (chunk->getSection() < (int)chunks.size()) {
            sgChunks->Row = sgChunks->Row+1; // next chunk
        } else {
            if (IDYES != Application->MessageBox(L"Save job? Are all chunks completed?", L"Info", MB_YESNO)) return;

        }
    }
    labelPrimary->Enabled = true; labelSecondary->Enabled = false;
    showChunks(); // calls showCurrentRow();
    editBarcode->Clear();
    ActiveControl = editBarcode; // focus for next barcode
}

void TfrmProcess::checkExit() {
    if (IDYES == Application->MessageBox(L"Are you sure you want to exit?\n\nCurrent progress will be saved.", L"Question", MB_YESNO)) {
        exit();
    }
}

void TfrmProcess::exit() { // definitely exiting
/*  * update cryovial_store (old and new, primary and secondary) when they enter their password to confirm */

	frmConfirm->initialise(TfrmSMLogin::RETRIEVE, "Ready to sign off boxes"); // std::set<int> projects; projects.insert(job->getProjectID()); frmConfirm->initialise(TfrmSMLogin::RETRIEVE, "Ready to sign off boxes", projects);
	if (!RETRASSTDEBUG && mrOk != frmConfirm->ShowModal()) {
		Application->MessageBox(L"Signoff cancelled", L"Info", MB_OK);
		return; // fixme what now?
	}

    panelLoading->Caption = loadingMessage;
    panelLoading->Visible = true;
    panelLoading->Top = (sgVials->Height / 2) - (panelLoading->Height / 2);
    panelLoading->Left = (sgVials->Width / 2) - (panelLoading->Width / 2);
    progressBottom->Style = pbstMarquee; progressBottom->Visible = true;

    Screen->Cursor = crSQLWait; Enabled = false; DEBUGSTREAM("save progress for job "<<job->getID()<<" started")

    saveProgressThread = new SaveProgressThread();
    saveProgressThread->OnTerminate = &saveProgressThreadTerminated;
}
void __fastcall SaveProgressThread::Execute() {

/*	* check cryo/store old/new params correct for `LCDbCryovialRetrieval`
	* `c_box_retrieval`: set `time_stamp`, `status` = 1 (PART_FILLED)
    * `box_name` (if record): update `time_stamp`, `box_capacity`, `status=1` (IN_USE)
	* `c_box_name` (if record): update `time_stamp`, `box_capacity`, `status=1` (IN_USE)
    * how to update boxes? check at save and exit that all vials in a box have been saved? */

            /*int rj_box_cid;
			map<int, int> boxes; // box_id to rj_box_id, per chunk
			map<int, int>::iterator found = boxes.find(dest_box_id);
			if (found == boxes.end()) { // not added yet, add record and cache
				LCDbBoxRetrieval box(//rj_box_cid,
                    job->getID(),
					dest_box_id,
                    job->getProjectID(),
                    chunk->getSection(),
                    LCDbBoxRetrieval::Status::NEW);
				box.saveRecord(qc);
				rj_box_cid = boxes[dest_box_id] = box.getRJBId(); // cache result
            } else {
				rj_box_cid = found->second;
			}
			return rj_box_cid;*/
	typedef std::set< SampleRow * > SetOfVials;
	typedef std::map< int, SetOfVials > VialsInBoxesMap;
	VialsInBoxesMap boxes;
	VialsInBoxesMap::iterator found;

	frmProcess->unactionedSamples = false; frmProcess->info.clear(); frmProcess->warnings.clear(); frmProcess->errors.clear();
	try {
		for (vector<SampleRow *>::iterator it = frmProcess->vials.begin(); it != frmProcess->vials.end(); ++it) {
			SampleRow * sample = *it;

			// build a map of source box ID to a set of vials contained in that box
			int sourceBox = sample->store_record->getBoxID();
            // should get id of secondary box as well and add it to map, we are checking for all empty boxes


			found = boxes.find(sourceBox);
			if (found == boxes.end()) { // not added yet
				SetOfVials setOfVials;
				setOfVials.insert(sample);

				boxes[sourceBox] = setOfVials;
			} else { // already in map
				found->second.insert(sample);
			}

            ostringstream oss; oss<<"blah blah blah"; loadingMessage = oss.str().c_str(); Synchronize((TThreadMethod)&updateStatus);
			int status  = sample->retrieval_record->getStatus();
			if (status != LCDbCryovialRetrieval::EXPECTED && status != LCDbCryovialRetrieval::IGNORED) { // changed
				storeSample(sample);
			} else {
				frmProcess->unactionedSamples = true;
			}
            if (NULL != sample->secondary) {
                storeSample(sample->secondary);
            }
		}

		// now check for completed boxes
		for (found = boxes.begin(); found != boxes.end(); found++) { // for each source box
			bool vialRemains = false;
			SetOfVials & setOfVials = found->second;
			SetOfVials::const_iterator it;
			for (it = setOfVials.begin(); it != setOfVials.end(); it++) { // for each vial in the box
				SampleRow * sample;
				switch (sample->retrieval_record->getStatus()) { // what about secondaries?
				case LCDbCryovialRetrieval::EXPECTED:
					vialRemains = true;
                default:
                    ;
				}
			}

			if (!vialRemains) { // empty/completed box, discard
				// ???
			}
		}



	} catch(Exception & e) {
		AnsiString msg = e.Message;
		frmProcess->errors.push_back(msg.c_str());
		frmProcess->unactionedSamples = true;
    } catch(char * e) {
		frmProcess->errors.push_back(e);
		frmProcess->unactionedSamples = true;
	} catch (...) {
		frmProcess->errors.push_back("Unknown error");
		frmProcess->unactionedSamples = true;
    }

	if (!frmProcess->unactionedSamples) { // job finished
		jobFinished();
	}
}

void SaveProgressThread::storeSample(SampleRow * sample) {
/* * should be 4 cryovial_store recs/sample: src + dest * primary + secondary
	* new `NOT_FOUND` status (ALLOCATED, CONFIRMED, MOVE_EXPECTED, DESTROYED, ANALYSED, TRANSFERRED, NOT_FOUND, DELETED = 99) (no IGNORED status?)
	* NOT_FOUND will be 6, not 7 - unless there is supposed to be another new status?
	* if primary aliquot found:
		- primary src TRANSFERRED?
		- primary dest CONFIRMED?
		- secondary src CONFIRMED?
		- secondary dest DELETED?
		- l_cryo
	* if secondary aliquot found:
        - primary src NOT_FOUND?
        - primary dest DELETED?
        - secondary src TRANSFERRED?
		- secondary dest CONFIRMED?
	* `cryovial_store` (source and dest, primary and secondary)
		* Primary, source:
			* if found: update `removed`, `status=5` (TRANSFERRED)
			* else `status=7` (NOT_FOUND??)
        * Primary, dest:
			* if found: update `time_stamp`, `status=1` (STORED)
            * else `status=99`
        * Secondary, src:
            * if primary found: clear `retrieval_cid`, `status=1`
            * else if secondary found update `removed`, `status=5`,
			* else `status=7` (NOT_FOUND)
        * Secondary, dest:
            * if primary found: `status=99`
            * else if secondary found: update `time_stamp`, `status=1`,
			* else `status=99` */
}

void SaveProgressThread::jobFinished() {
    LQuery qp(Util::projectQuery(frmProcess->job->getProjectID(), false)); //LQuery qd(Util::projectQuery(job->getProjectID(), true));
    LQuery qc(LIMSDatabase::getCentralDb());

    // all boxes must be finished if here (ie. all samples are finished)
    qc.setSQL("SELECT * FROM c_box_retrieval WHERE retrieval_cid = :rtid");
    qc.setParam("rtid", frmProcess->job->getID());
    qc.open();
    while (!qc.eof()) {
        // `c_box_retrieval`: set `time_stamp`, `status=2` (collected)
        LCDbBoxRetrieval cbr(qc);
        cbr.setStatus(LCDbBoxRetrieval::COLLECTED);
        cbr.saveRecord(LIMSDatabase::getCentralDb()); // time_stamp set by default - should be in 2.7.2

        //qc.readInt

        // `cryovial_store`: as above (dealt with already?)

        // `box_name`: (if record): update `time_stamp`, `box_capacity`, `status=2` (CONFIRMED)

        // `c_box_name`: (if record): update `time_stamp`, `box_capacity`, `status=2` (CONFIRMED)

    }

    // `c_retrieval_job`: update `finish_date`, `status` = 2
    frmProcess->job->setStatus(LCDbCryoJob::DONE);
    frmProcess->job->saveRecord(qc); // finish date is updated by this method
}

void __fastcall TfrmProcess::saveProgressThreadTerminated(TObject *Sender) {
    progressBottom->Style = pbstNormal; progressBottom->Visible = false; panelLoading->Visible = false; Screen->Cursor = crDefault;
    try {
        // anything more to do?
        collectEmpties();
    } catch (Exception & e) {
		TfrmRetrievalAssistant::msgbox(e.Message);
    }
    Enabled = true; DEBUGSTREAM(__FUNC__<<"save plan for job "<<job->getID()<<" finished")

    vector<string>::const_iterator strIt;
    if (frmProcess->errors.size() > 0) {
        ostringstream out;
        for (strIt = errors.begin(); strIt != errors.end(); strIt++) { out<<*strIt<<endl; }
        Application->MessageBox(String(out.str().c_str()).c_str(), L"Error", MB_OK);
        return;
	} else {
        delete_referenced< vector <SampleRow * > >(vials);
        delete_referenced< vector< Chunk< SampleRow > * > >(chunks); // chunk objects, not contents of chunks
        Close();
    }
}

void TfrmProcess::collectEmpties() {
    /** if, at the end of processing a chunk, there are any source boxes which have become empty,
        the user may want to discard them instead of replacing them.
	    if so, provide an option to discard these empty boxes, recording it in the database */

    // find out if there are any

    if (IDYES != Application->MessageBox(L"There are empty boxes. Would you like to mark these as discarded?", L"Info", MB_YESNO)) return;

//    for


/*  * collect empties (all vials "accepted" or "not found") for discard
    * at the end of processing each chunk, if source boxes are now empty
    * unlikely for test tasks but rat tanks may throw old boxes away
    * all source boxes from a reorganisation task should end up empty
    * ask user to confirm that vessel/structure/slot is now empty
    * otherwise box should be referred */
    Application->MessageBox(L"Handle disposal of empty boxes", L"Info", MB_OK);
}

//* Ask the relevant question(s) from the URS when they’re ready to finish
//    * only this: "). The option to exit the process saving progress should be offered, with an “are you sure?” message in case of accidental selection (REQ 8.3.12)."

