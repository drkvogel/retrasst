#include <vcl.h>
#pragma hdrstop
#include "StoreDAO.h"
#include "RetrAsstCollectSamples.h"
#include "LCDbAuditTrail.h"
#include <ctime>      // struct tm
#include <locale>     // locale, time_put

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
    sgwVials->addCol("barcode",  "Barcode",          91);
    sgwVials->addCol("site",     "Site",             90);
    sgwVials->addCol("vesspos",  "Pos",              28);
    sgwVials->addCol("vessel",   "Vessel",           107);
    sgwVials->addCol("shelf",    "Shelf",            31);
    sgwVials->addCol("structpos","Pos",              27);
    sgwVials->addCol("struct",   "Structure",        100);
    sgwVials->addCol("boxpos",   "Slot",             26);
    sgwVials->addCol("currbox",  "Current box",      257);
    sgwVials->addCol("currpos",  "Pos",              31);
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
    delete_referenced< vector <SampleRow * > >(vials);
    delete_referenced< vector< Chunk< SampleRow > * > >(chunks); // chunk objects, not contents of chunks
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

void __fastcall TfrmProcess::menuItemExitClick(TObject *Sender) { exit(); }

void __fastcall TfrmProcess::btnExitClick(TObject *Sender) { exit(); }

void __fastcall TfrmProcess::btnAcceptClick(TObject *Sender) { accept(editBarcode->Text); }

void __fastcall TfrmProcess::btnSimAcceptClick(TObject *Sender) { // simulate a correct barcode scanned
    editBarcode->Text = currentChunk()->rowAt(currentChunk()->getCurrentRow())->cryovial_barcode.c_str();
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
    sgVials->Cells[sgwVials->colNameToInt("barcode")]  [rw] = sample->cryovial_barcode.c_str();
    sgVials->Cells[sgwVials->colNameToInt("status") ]  [rw] = LCDbCryovialRetrieval::statusString(sample->retrieval_record->getStatus());
    sgVials->Cells[sgwVials->colNameToInt("aliquot")]  [rw] = sample->aliquotName().c_str();
    sgVials->Cells[sgwVials->colNameToInt("currbox")]  [rw] = sample->src_box_name.c_str();
    sgVials->Cells[sgwVials->colNameToInt("currpos")]  [rw] = sample->store_record->getPosition();
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
    loadChunk(); //loadRows(); //loadChunk(loadingChunk); //loadChunk(currentChunk());
}

void TfrmProcess::loadChunk() { //(Chunk< SampleRow > *) {
    panelLoading->Caption = loadingMessage;
    panelLoading->Visible = true; // appearing in wrong place because called in OnShow, form not yet maximized
    panelLoading->Top = (sgVials->Height / 2) - (panelLoading->Height / 2);
    panelLoading->Left = (sgVials->Width / 2) - (panelLoading->Width / 2);
    progressBottom->Style = pbstMarquee; progressBottom->Visible = true;
    Screen->Cursor = crSQLWait; // disable mouse? //ShowCursor(false);
    DEBUGSTREAM("loadRows for job "<<job->getID()<<" started")
    Enabled = false;
    loadPlanWorkerThread = new LoadPlanWorkerThread();
    loadPlanWorkerThread->OnTerminate = &loadPlanWorkerThreadTerminated;
}

__fastcall LoadPlanWorkerThread::LoadPlanWorkerThread() : TThread(false) { FreeOnTerminate = true; }

void __fastcall LoadPlanWorkerThread::updateStatus() { // can't use args for synced method, don't know why
    frmProcess->panelLoading->Caption = loadingMessage.c_str(); frmProcess->panelLoading->Repaint();
}

void __fastcall LoadPlanWorkerThread::debugLog() { frmProcess->debugLog(debugMessage.c_str()); }

void __fastcall LoadPlanWorkerThread::msgbox() { Application->MessageBox(String(debugMessage.c_str()).c_str(), L"Info", MB_OK); }

void __fastcall LoadPlanWorkerThread::Execute() { NotUsingTempTable(); } //UsingTempTable();

void LoadPlanWorkerThread::NotUsingTempTable() {
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
        "    cbr.retrieval_cid, section AS chunk, cbr.rj_box_cid, "//cbr.status, "
        "    cs.box_cid, sb.external_name AS src_box, cs.tube_position AS source_pos,  "
        "    cbr.box_id AS dest_id, db.external_name AS dest_box, slot_number AS dest_pos, "
        "    lcr.process_cid AS lcr_procid, lcr.status AS lcr_status, lcr.slot_number AS lcr_slot, lcr.cryovial_barcode, lcr.aliquot_type_cid, "
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
        "    chunk, source_pos, rj_box_cid, aliquot_type_cid "
        << (primary_aliquot < secondary_aliquot ? "ASC" : "DESC"); //debugMessage = oss.str(); Synchronize((TThreadMethod)&debugLog);
    qd.setSQL(oss.str()); debugMessage = "open query"; Synchronize((TThreadMethod)&debugLog);
    qd.setParam("rtid", job->getID()); //int retrieval_cid = job->getID();
    qd.open();
    rowCount = 0; int curchunk = 0, chunk = 0; SampleRow * previous = NULL; debugMessage = "foreach row"; Synchronize((TThreadMethod)&debugLog);
    while (!qd.eof()) {
        chunk = qd.readInt("chunk"); //wstringstream oss; oss<<__FUNC__<<oss<<"chunk:"<<chunk<<", rowCount: "<<rowCount; OutputDebugString(oss.str().c_str());
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
            new LCDbCryovialRetrieval(qd), // fixme
            qd.readString(  "cryovial_barcode"),
            qd.readString(  "src_box"),
            qd.readInt(     "dest_id"),
            qd.readString(  "dest_name"),
            qd.readInt(     "dest_pos"),
            "", 0, "", 0, 0, "", 0 ); // no storage details yet

        int currentAliquotType  = row->cryo_record->getAliquotType();
        int previousAliquotType = previous == NULL? 0 : previous->cryo_record->getAliquotType();
        if (secondary_aliquot != 0 && secondary_aliquot == currentAliquotType &&
            previous != NULL && previous->cryovial_barcode == row->cryovial_barcode) { // secondary?
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
    } oss.str(""); oss<<"finished loading "<<rowCount<<" samples"; debugMessage = oss.str(); Synchronize((TThreadMethod)&debugLog);

    if (0 == rowCount || 0 == frmProcess->chunks.size()) { // something wrong here...
        return;
    }

    frmProcess->chunks[frmProcess->chunks.size()-1]->setEnd(frmProcess->vials.size()-1);

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
	}
    debugMessage = "finished load storage details";
    Synchronize((TThreadMethod)&debugLog);
}

void LoadPlanWorkerThread::UsingTempTable() {
    /** load retrieval plan
    For a box retrieval, the retrieval plan will be given by: Select * from c_box_retrieval b order by b.section, b.rj_box_cid
    For a cryovial retrieval, the retrieval plan will be: Select * from c_box_retrieval b, l_cryovial_retrieval c where b.rj_box_cid = c.rj_box_cid order by b.section, c.position */

    delete_referenced< vector<SampleRow * > >(frmProcess->vials); frmProcess->chunks.clear();
    ostringstream oss; oss<<frmProcess->loadingMessage<<" (preparing query)"; loadingMessage = oss.str().c_str(); //return;
    if (NULL == frmProcess || NULL == frmProcess->job) { throw "wtf?"; }
    loadingMessage = frmProcess->loadingMessage;
    const int pid = LCDbAuditTrail::getCurrent().getProcessID();
    job = frmProcess->job;

    time_t rawtime;
    struct tm * timeinfo;
    char buffer [80];
    time(&rawtime);
    timeinfo = localtime(&rawtime);
    strftime(buffer, 80, "%Y_%m_%dT%H_%M_%S", timeinfo);
    puts(buffer);
    oss.str(""); oss<<"retrieval_assistant_temp_"<<pid<<"_"<<buffer;
    frmProcess->tempTableName.assign(oss.str());
    {
        LQuery qd(Util::projectQuery(job->getProjectID(), true)); // ddb
        oss.str(""); oss<<"DROP TABLE IF EXISTS "<<frmProcess->tempTableName; // ingres doesn't like table names passed as parameters
        debugMessage = oss.str(); Synchronize((TThreadMethod)&debugLog);
        qd.setSQL(oss.str());
        qd.execSQL();
        oss.str("");
        oss<<"CREATE TABLE "<<frmProcess->tempTableName<<" AS"<<
            "   SELECT"
            "       cbr.section AS chunk, cbr.rj_box_cid, cbr.retrieval_cid, cbr.status AS cbr_status, cbr.box_id,"
            "       lcr.position AS dest_pos, lcr.slot_number AS lcr_slot, lcr.process_cid AS lcr_procid, lcr.status AS lcr_status,"
            "       lcr.cryovial_barcode, lcr.aliquot_type_cid"
            "   FROM"
            "       c_box_retrieval cbr, l_cryovial_retrieval lcr"
            "   WHERE"
            "       cbr.retrieval_cid = :rtid"
            "   AND"
            "       lcr.rj_box_cid = cbr.rj_box_cid";
        debugMessage = oss.str();
        Synchronize((TThreadMethod)&debugLog);
        qd.setSQL(oss.str());
        int retrieval_cid = job->getID();
        qd.setParam("rtid", retrieval_cid); //qd.setParam("chnk", loading0Chunk->getSection()); //frmProcess->currentChunk()->getSection()); //frmProcess->chunk); //
        qd.execSQL();
    }
    debugMessage = "finished create temp table"; Synchronize((TThreadMethod)&debugLog);

    // gj added a secondary index on cryovial_store (on t_ldb20 only) - how to check this?
    bool stats_c_barcode                = Util::statsOnColumn(job->getProjectID(), "cryovial",      "cryovial_barcode");
    bool stats_c_aliquot                = Util::statsOnColumn(job->getProjectID(), "cryovial",      "aliquot_type_cid");
    bool stats_cs_cryovial_id           = Util::statsOnColumn(job->getProjectID(), "cryovial_store","cryovial_id");
    bool stats_cs_status                = Util::statsOnColumn(job->getProjectID(), "cryovial_store","status");
    bool stats_cs_retrieval_cid         = Util::statsOnColumn(job->getProjectID(), "cryovial_store","retrieval_cid");

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

    int primary_aliquot     = job->getPrimaryAliquot();
    int secondary_aliquot   = job->getSecondaryAliquot();

    debugMessage = "select sample details from plan"; Synchronize((TThreadMethod)&debugLog);
    LQuery ql(Util::projectQuery(job->getProjectID(), true)); // must have ddb to see temp table just created in ddb
    oss.str("");
    oss<<
        " SELECT"
        "     g.retrieval_cid, g.chunk, g.rj_box_cid, g.cbr_status, g.dest_pos, g.lcr_slot, g.lcr_procid, g.lcr_status, g.box_id AS dest_id,"
        "     c.cryovial_barcode, c.sample_id, c.aliquot_type_cid, c.note_exists AS cryovial_note,"
        "     s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position, s1.record_id,"
        "     s1.status, s1.tube_position, s1.note_exists AS cs_note,"
        "     b1.external_name AS src_box, "
        "     b2.external_name AS dest_name,"
        "     s2.tube_position AS slot_number, s2.status AS dest_status"
        " FROM "
        <<frmProcess->tempTableName<<" g, cryovial c, cryovial_store s1, cryovial_store s2, box_name b1, box_name b2"
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
        "     s1.retrieval_cid, chunk, g.rj_box_cid, c.aliquot_type_cid "
        << (primary_aliquot < secondary_aliquot ? "ASC" : "DESC");
    debugMessage = oss.str(); Synchronize((TThreadMethod)&debugLog);
    ql.setSQL(oss.str());
    rowCount = 0; // class variable needed for synchronise
    debugMessage = "open query"; Synchronize((TThreadMethod)&debugLog);
    ql.open();
    int curchunk = 0, chunk = 0; SampleRow * previous = NULL;
    debugMessage = "foreach row"; Synchronize((TThreadMethod)&debugLog);
    while (!ql.eof()) {
        chunk = ql.readInt("chunk"); //wstringstream oss; oss<<__FUNC__<<oss<<"chunk:"<<chunk<<", rowCount: "<<rowCount; OutputDebugString(oss.str().c_str());
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
            ql.readString(  "src_box"),
            ql.readInt(     "dest_id"),
            ql.readString(  "dest_name"),
            ql.readInt(     "dest_pos"),
            "", 0, "", 0, 0, "", 0 ); // no storage details yet

        frmProcess->vials.push_back(row);
        ql.next();
        rowCount++;
    }
    oss.str(""); oss<<"finished loading "<<rowCount<<" samples"; debugMessage = oss.str(); Synchronize((TThreadMethod)&debugLog);

    oss.str(""); oss<<"DROP TABLE IF EXISTS "<<frmProcess->tempTableName;
    ql.setSQL(oss.str());
    if (!RETRASSTDEBUG) {
        ql.execSQL();
        debugMessage = "finished drop temp table"; Synchronize((TThreadMethod)&debugLog);
    } else {
        debugMessage = "didn't drop temp table in debug mode"; Synchronize((TThreadMethod)&debugLog);
    }

    frmProcess->chunks[frmProcess->chunks.size()-1]->setEnd(frmProcess->vials.size()-1);

    // find locations of source boxes
    int rowCount2 = 0;
	for (vector<SampleRow *>::iterator it = frmProcess->vials.begin(); it != frmProcess->vials.end(); ++it, rowCount2++) {
        SampleRow * sample = *it;
        ostringstream oss; oss<<"Finding storage for "<<sample->cryovial_barcode<<" ["<<rowCount2<<"/"<<rowCount<<"]: ";
        frmRetrievalAssistant->getStorage(sample);
        oss<<sample->storage_str(); loadingMessage = oss.str().c_str(); Synchronize((TThreadMethod)&updateStatus);
	}
    debugMessage = "finished load storage details";
    Synchronize((TThreadMethod)&debugLog);
}

void __fastcall TfrmProcess::loadPlanWorkerThreadTerminated(TObject *Sender) {
    progressBottom->Style = pbstNormal; progressBottom->Visible = false;
    panelLoading->Visible = false;
    Screen->Cursor = crDefault;
    try {
        showChunks();
        currentChunk()->setCurrentRow(0); //currentChunk = 0;
        showCurrentRow();
    } catch (Exception & e) {
        msgbox(e.Message);
    }
    Enabled = true;
    DEBUGSTREAM(__FUNC__<<"loadRows for job "<<job->getID()<<" finished")
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

void TfrmProcess::accept(String barcode) { // fixme check correct vial; could be missing, swapped etc
    SampleRow * sample  = currentSample();
    SampleRow * aliquot = currentAliquot();
    switch (aliquot->retrieval_record->getStatus()) {
        case LCDbCryovialRetrieval::EXPECTED:
        case LCDbCryovialRetrieval::IGNORED:
            break; // ok, carry on
        case LCDbCryovialRetrieval::COLLECTED:
            msgbox("Already collected - please inform Core Programming"); return;
        case LCDbCryovialRetrieval::NOT_FOUND:
            if (IDOK != Application->MessageBox(L"Confirm sample has now been found", L"Question", MB_OKCANCEL)) return;
    }
    if (barcode == aliquot->cryovial_barcode.c_str()) { // save
        aliquot->retrieval_record->setStatus(LCDbCryovialRetrieval::COLLECTED);
        //if secondary
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
    showCurrentRow();
    nextRow();
}

void TfrmProcess::notFound() {
    DEBUGSTREAM("Save not found row")
    int rowIdx = currentChunk()->getCurrentRow();
    SampleRow * row = currentChunk()->rowAt(rowIdx);
    if (row->retrieval_record->getStatus() != LCDbCryovialRetrieval::NOT_FOUND) {
        row->retrieval_record->setStatus(LCDbCryovialRetrieval::NOT_FOUND);
        if (row->secondary) {
            msgbox("Secondary aliquot found");
            if (row->secondary->retrieval_record->getStatus() != LCDbCryovialRetrieval::NOT_FOUND) { // secondary already marked not found
                fillRow(row, rowIdx+1); // refresh sg row - now keeps pointer to row
                showCurrentRow();
                showDetails(row->secondary);
                return;
            } else {
                throw "secondary already NOT_FOUND";
            }
        } else {
            msgbox("No secondary aliquot exists, continuing to next sample");
            nextRow();
        }
    } else {  // primary already marked not found
        if (row->secondary) {
            row->secondary->retrieval_record->setStatus(LCDbCryovialRetrieval::NOT_FOUND);
            nextRow();
        } else {
            throw "no secondary for not found primary, should have moved on to next row";
        }
    }
}

SampleRow * TfrmProcess::currentSample() { //Chunk< SampleRow > * chunk = currentChunk(); //int current = chunk->getCurrentRow(); //SampleRow * sample = chunk->rowAt(current); //SampleRow * row = currentChunk()->rowAt(currentChunk()->getCurrentRow());
    return currentChunk()->rowAt(currentChunk()->getCurrentRow());
}

SampleRow * TfrmProcess::currentAliquot() { //Chunk< SampleRow > * chunk = currentChunk(); //int current = chunk->getCurrentRow(); //SampleRow * sample = chunk->rowAt(current); //SampleRow * row = currentChunk()->rowAt(currentChunk()->getCurrentRow());
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
    int current = chunk->getCurrentRow(); //SampleRow * sample = currentAliquot(); // which may be the secondary aliquot //
    SampleRow * sample = chunk->rowAt(current);

    // save both primary and secondary
    sample->retrieval_record->saveRecord(LIMSDatabase::getCentralDb());
    if (sample->secondary) {
        sample->secondary->retrieval_record->saveRecord(LIMSDatabase::getCentralDb());
    }
    if (current < chunk->getSize()-1) {
        int lookAhead = sgVials->VisibleRowCount/2;
        if (current+lookAhead < chunk->getSize()-1) {
            sgVials->Row = current+lookAhead+1; // bodge to scroll next few samples into view; ScrollBy doesn't seem to work
        } else {
            sgVials->Row = sgVials->RowCount-1;
        }
        chunk->setCurrentRow(current+1);
        showCurrentRow();
    } else { // skipped last row
        chunk->setCurrentRow(current+1); // past end to show complete?
        debugLog("Save chunk"); // no, don't save - completedness or otherwise of 'chunk' should be implicit from box/cryo plan
        if (chunk->getSection() < (int)chunks.size()) {
            sgChunks->Row = sgChunks->Row+1; // next chunk
        } else {
            if (IDYES != Application->MessageBox(L"Save job? Are all chunks completed?", L"Info", MB_YESNO)) return;
            Application->MessageBox(L"Handle disposal of empty boxes", L"Info", MB_OK);
        }
    }
    labelPrimary->Enabled = true; labelSecondary->Enabled = false;
    showChunks();
    editBarcode->Clear();
    ActiveControl = editBarcode; // focus for next barcode
}

void TfrmProcess::exit() {
    if (IDYES == Application->MessageBox(L"Are you sure you want to exit?\n\nCurrent progress will be saved.", L"Question", MB_YESNO)) {
    /* Ask the relevant question(s) from the URS when they’re ready to finish
  and update cryovial_store (old and new, primary and secondary) when they enter their password to confirm */
        Application->MessageBox(L"Save completed boxes", L"Info", MB_OK);
        Application->MessageBox(L"Signoff form (or on open form?)", L"Info", MB_OK);
        // how to update boxes? check at save and exit that all vials in a box have been saved?
        Close();
    }
}

//    using namespace boost::local_time;
//    //local_date_time
//
//    time_t t(time(NULL)); // current time
//    tm tm(*localtime(&t));
//    std::locale loc("");
//    const std::time_put<char> &tput = std::use_facet< std::time_put< char > >(loc);
//    tput.put(oss.rdbuf(), oss, _T('\0'), &tm, _T('x'));

// LCDbBoxRetrieval::Status::NEW|PART_FILLED|COLLECTED|NOT_FOUND|DELETED
// LCDbCryovialRetrieval::Status::EXPECTED|IGNORED|COLLECTED|NOT_FOUND


