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
#include "LPDbBoxes.h"
#include "RetrAsstCollectEmpties.h"

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

 destination box+position, cryovial barcode and current box+position+structure+location of the primary and secondary aliquots. */

TfrmRetrAsstCollectSamples *frmRetrAsstCollectSamples;

void TfrmRetrAsstCollectSamples::debugLog(String s) {
    String tmp = Now().CurrentDateTime().DateTimeString() + ": " + s;
    memoDebug->Lines->Add(tmp); // could use varargs: http://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c
}

void __fastcall LoadPlanThread::debugLog() { frmRetrAsstCollectSamples->debugLog(debugMessage.c_str()); }

void __fastcall LoadPlanThread::msgbox() { Application->MessageBox(String(debugMessage.c_str()).c_str(), L"Info", MB_OK); }

__fastcall LoadPlanThread::LoadPlanThread() : TThread(false) {
    main    = frmRetrievalAssistant;
    collect = frmRetrAsstCollectSamples;
    FreeOnTerminate = true;
}

__fastcall SaveProgressThread::SaveProgressThread() : TThread(false) {
    main    = frmRetrievalAssistant;
    collect = frmRetrAsstCollectSamples;
    FreeOnTerminate = true;
}

void __fastcall LoadPlanThread::updateStatus() { // can't use args for synced method, don't know why
	frmRetrAsstCollectSamples->panelLoading->Caption = loadingMessage.c_str(); frmRetrAsstCollectSamples->panelLoading->Repaint();
}

void __fastcall SaveProgressThread::updateStatus() {
	frmRetrAsstCollectSamples->panelLoading->Caption = loadingMessage.c_str(); frmRetrAsstCollectSamples->panelLoading->Repaint();
}

__fastcall TfrmRetrAsstCollectSamples::TfrmRetrAsstCollectSamples(TComponent* Owner) : TForm(Owner) {
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

    sgwVials = new StringGridWrapper<SampleRow>(sgVials, &combined);
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
    sgwVials->addCol("destype",  "Type",             67);
    sgwVials->addCol("destpos",  "Pos",              25);
    sgwVials->addCol("status",   "Status",           91);
    sgwVials->addCol("aliquot",  "Aliquot",          90);
    sgwVials->init();

    // chunk colour key
    labelNew->Color         = RETRIEVAL_ASSISTANT_CHUNK_NEW_COLOUR;
    labelInProgress->Color  = RETRIEVAL_ASSISTANT_CHUNK_INPROGRESS_COLOUR;
    labelDone->Color        = RETRIEVAL_ASSISTANT_CHUNK_COMPLETED_COLOUR;

    // vial colour key
    labelVialKeyExpectedPrimary->Color      = RETRIEVAL_ASSISTANT_EXPECTED_COLOUR;
    labelVialKeyExpectedBackup->Color       = RETRIEVAL_ASSISTANT_SECONDARY_COLOUR;
    labelVialKeyIgnored->Color              = RETRIEVAL_ASSISTANT_IGNORED_COLOUR;
    labelVialKeyCollected->Color            = RETRIEVAL_ASSISTANT_COLLECTED_COLOUR;
    labelVialKeyNotFound->Color             = RETRIEVAL_ASSISTANT_NOT_FOUND_COLOUR;
}

void __fastcall TfrmRetrAsstCollectSamples::FormCreate(TObject *Sender) {
    cbLog->Visible      = RETRASSTDEBUG;
    cbLog->Checked      = RETRASSTDEBUG;
    sgVials->Enabled    = RETRASSTDEBUG;
    panelDebug->Visible = cbLog->Checked;
    job                 = NULL;
    progressMessage = "Loading retrieval list, please wait...";
}

void __fastcall TfrmRetrAsstCollectSamples::FormClose(TObject *Sender, TCloseAction &Action) {
    //exit(); //???
}

__fastcall TfrmRetrAsstCollectSamples::~TfrmRetrAsstCollectSamples() {
    destroying = true;
    delete sgwChunks;
    delete sgwVials;
}

void __fastcall TfrmRetrAsstCollectSamples::sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
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
                    background = RETRIEVAL_ASSISTANT_CHUNK_NEW_COLOUR; break;
                case Chunk< SampleRow >::INPROGRESS:
                    background = RETRIEVAL_ASSISTANT_CHUNK_INPROGRESS_COLOUR; break;
                case Chunk< SampleRow >::DONE:
                    background = RETRIEVAL_ASSISTANT_CHUNK_COMPLETED_COLOUR; break;
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

void __fastcall TfrmRetrAsstCollectSamples::sgVialsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
    TColor background = RETRIEVAL_ASSISTANT_ERROR_COLOUR;
    if (0 == ARow) {
        background = clBtnFace;
    } else {
        SampleRow * row = (SampleRow *)sgVials->Objects[0][ARow];
        if (NULL == row) {
            background = clWindow; // whilst loading
        } else {
            int status = row->lcr_record->getStatus();
            switch (status) {
                // could use currentAliquot() here?
                case LCDbCryovialRetrieval::EXPECTED:
                    background = RETRIEVAL_ASSISTANT_EXPECTED_COLOUR; break;
                case LCDbCryovialRetrieval::IGNORED:
                    background = RETRIEVAL_ASSISTANT_IGNORED_COLOUR; break;
                case LCDbCryovialRetrieval::COLLECTED:
                    background = RETRIEVAL_ASSISTANT_COLLECTED_COLOUR; break;
                case LCDbCryovialRetrieval::NOT_FOUND:
                    if (NULL != row->backup) {
                        int backupStatus = row->backup->lcr_record->getStatus();
                        switch (backupStatus) {
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

void __fastcall TfrmRetrAsstCollectSamples::sgChunksFixedCellClick(TObject *Sender, int ACol, int ARow) {
    ostringstream oss; oss << __FUNC__; oss<<sgwChunks->printColWidths()<<" clicked on col: "<<ACol<<"."; //debugLog(oss.str().c_str());
}

void __fastcall TfrmRetrAsstCollectSamples::sgChunksClick(TObject *Sender) {
    int row = sgChunks->Row;
    if (row < 1) return;

    // reset deferred rows
    Chunk< SampleRow > * chunk = (Chunk< SampleRow > *)sgChunks->Objects[0][row];
    for (int row=0; row < chunk->getSize(); row++) {
        SampleRow * sampleRow = chunk->objectAtRel(row);
        if (sampleRow->lcr_record->getStatus() == LCDbCryovialRetrieval::IGNORED) {
            sampleRow->lcr_record->setStatus(LCDbCryovialRetrieval::EXPECTED);
        } else if ( sampleRow->lcr_record->getStatus() == LCDbCryovialRetrieval::NOT_FOUND && sampleRow->backup != NULL) {
            if (sampleRow->backup->lcr_record->getStatus() == LCDbCryovialRetrieval::IGNORED) {
                sampleRow->backup->lcr_record->setStatus(LCDbCryovialRetrieval::EXPECTED);
            }
        }
    }

    showChunk(chunk);
}

void __fastcall TfrmRetrAsstCollectSamples::sgVialsClick(TObject *Sender) { // show details in debug window
    SampleRow * sample = (SampleRow *)sgVials->Objects[0][sgVials->Row];
    ostringstream oss; oss<<sample->debug_str(); debugLog(oss.str().c_str());
    SampleRow * backup = sample->backup;
    if (!backup) { debugLog(" (no backup)"); return; }
    oss.str(); oss<<" (backup) "<<sample->debug_str(); debugLog(oss.str().c_str());

    //int row = sgVials->Row; sgVials->Row = row; // how to put these before and after to save row clicked on?
}

void __fastcall TfrmRetrAsstCollectSamples::editBarcodeKeyUp(TObject *Sender, WORD &Key, TShiftState Shift) {
    if (VK_RETURN == Key) { accept(editBarcode->Text); }
}

void __fastcall TfrmRetrAsstCollectSamples::FormResize(TObject *Sender) { // gets called *after* FormDestroy
    if (!destroying) { // check in case pointers have been deleted in FormDestroy
        sgwChunks->resize(); sgwVials->resize();
    }
}

void __fastcall TfrmRetrAsstCollectSamples::cbLogClick(TObject *Sender) { panelDebug->Visible = cbLog->Checked; }

void __fastcall TfrmRetrAsstCollectSamples::menuItemExitClick(TObject *Sender) { checkExit(); }

void __fastcall TfrmRetrAsstCollectSamples::btnExitClick(TObject *Sender) { checkExit(); } //exit(); }

void __fastcall TfrmRetrAsstCollectSamples::btnAcceptClick(TObject *Sender) { accept(editBarcode->Text); }

void __fastcall TfrmRetrAsstCollectSamples::btnSimAcceptClick(TObject *Sender) { // simulate a correct barcode scanned
    editBarcode->Text = (currentChunk()->currentObject())->cryovial_barcode.c_str();
    btnAcceptClick(this);
}

void __fastcall TfrmRetrAsstCollectSamples::btnNotFoundClick(TObject *Sender) { notFound(); }

void __fastcall TfrmRetrAsstCollectSamples::btnSkipClick(TObject *Sender) { skip(); }

void TfrmRetrAsstCollectSamples::showProgressMessage(const char * loadingMessage) {
	panelLoading->Caption = loadingMessage;
	panelLoading->Visible = true;
	panelLoading->Top = (sgVials->Height / 2) - (panelLoading->Height / 2);
    panelLoading->Left = (sgVials->Width / 2) - (panelLoading->Width / 2);
    progressBottom->Style = pbstMarquee; progressBottom->Visible = true;
}

void TfrmRetrAsstCollectSamples::checkExit() {
    if (IDYES == Application->MessageBox(L"Are you sure you want to exit?\n\nCurrent progress will be saved.", L"Question", MB_YESNO)) {
        exit();
    }
}

void __fastcall TfrmRetrAsstCollectSamples::FormShow(TObject *Sender) {
    ostringstream oss; oss<<job->getName()<<" : "<<job->getDescription()<<" [id: "<<job->getID()<<"]";
    Caption = oss.str().c_str();
    chunks.clear();
    sgwChunks->clear();
    sgwVials->clear();
    frmRetrievalAssistant->clearStorageCache();
    labelSampleID->Caption  = "loading...";
    labelStorage->Caption   = "loading...";
    labelDestbox->Caption   = "loading...";
    labelDestype->Caption   = "loading...";
    labelPrimary->Caption   = Util::getAliquotDescription(job->getPrimaryAliquot()).c_str();
    labelSecondary->Caption = Util::getAliquotDescription(job->getSecondaryAliquot()).c_str();
    labelPrimary->Enabled   = true;
    labelSecondary->Enabled = false;
    timerLoadPlan->Enabled  = true;
}

void TfrmRetrAsstCollectSamples::showChunks() {
    if (0 == chunks.size()) { throw runtime_error("No chunks"); } // must always have one chunk anyway
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

Chunk< SampleRow > * TfrmRetrAsstCollectSamples::currentChunk() {
    if (sgChunks->Row < 1) sgChunks->Row = 1; // force selection of 1st row
    Chunk< SampleRow > * chunk = (Chunk< SampleRow > *)sgChunks->Objects[0][sgChunks->Row];
    if (NULL == chunk) throw runtime_error("null chunk");
    return chunk;
}

void TfrmRetrAsstCollectSamples::showChunk(Chunk< SampleRow > * chunk) {
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

void TfrmRetrAsstCollectSamples::fillRow(SampleRow * row, int rw) {
    SampleRow * sample;
    if (row->lcr_record->getStatus() == LCDbCryovialRetrieval::NOT_FOUND && row->backup != NULL)
        sample = row->backup;
    else
        sample = row;

    sgVials->Cells[sgwVials->colNameToInt("item")]     [rw] = rw;
    sgVials->Cells[sgwVials->colNameToInt("barcode")]  [rw] = sample->cryovial_barcode.c_str();
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
    sgVials->Cells[sgwVials->colNameToInt("destype")]  [rw] = sample->dest_type_name.c_str();
    sgVials->Cells[sgwVials->colNameToInt("destpos")]  [rw] = sample->dest_cryo_pos;
    sgVials->Cells[sgwVials->colNameToInt("status") ]  [rw] = LCDbCryovialRetrieval::statusString(sample->lcr_record->getStatus());
    sgVials->Cells[sgwVials->colNameToInt("aliquot")]  [rw] = sample->aliquotName().c_str();
    sgVials->Objects[0][rw] = (TObject *)row; // keep all data, primary and secondary
}

void __fastcall TfrmRetrAsstCollectSamples::timerLoadPlanTimer(TObject *Sender) {
    timerLoadPlan->Enabled = false;
	Enabled = false; Screen->Cursor = crSQLWait;
    showProgressMessage(progressMessage);
	loadPlan();
}

void TfrmRetrAsstCollectSamples::loadPlan() {
    ostringstream oss; oss<<__FUNC__; debugLog(oss.str().c_str());
    loadPlanThread = new LoadPlanThread();
    loadPlanThread->OnTerminate = &loadPlanThreadTerminated;
}

void __fastcall LoadPlanThread::Execute() {
/** load cryovial retrieval plan:
Select * from c_box_retrieval b, l_cryovial_retrieval c where b.rj_box_cid = c.rj_box_cid order by b.section, c.position */
    delete_referenced< vector<SampleRow * > >(collect->primaries);
    delete_referenced< vector<SampleRow * > >(collect->secondaries);
    collect->combined.clear();
    collect->chunks.clear();

    //ostringstream oss; oss<<collect->progressMessage<<" (preparing query)"; loadingMessage = collect->progressMessage; //loadingMessage = oss.str().c_str(); //return;

    int primary_aliquot = collect->job->getPrimaryAliquot(); int secondary_aliquot = collect->job->getSecondaryAliquot();

    //const LCDbProject * proj; proj = LCDbProjects::records().findByID(collect->job->getProjectID());
    const LCDbProject * proj = LCDbProjects::records().findByID(collect->job->getProjectID());
    ostringstream oss; oss<<__FUNC__<<": job: "<<collect->job->str()<<", project: "<<proj->getName()<<" ["<<proj->getID()<<"], "<<proj->getDbName();
    debugMessage = oss.str().c_str(); debugLog();

    //debugMessage = "select sample details from plan"; Synchronize((TThreadMethod)&debugLog);
    LQuery qd(Util::projectQuery(collect->job->getProjectID(), true)); // ddb


    oss.str(""); oss<<
        " SELECT "
        "    db.project_cid," // project of destination box (db) or source (sb)?
        "    cbr.retrieval_cid, cbr.section, cbr.rj_box_cid, cbr.box_id, "
        "    lcr.position AS lcr_position, lcr.cryovial_barcode, lcr.aliquot_type_cid, "
        "    lcr.old_box_cid, lcr.old_position, "
        "    lcr.process_cid AS lcr_procid, lcr.status AS lcr_status, " // lcr.slot_number AS lcr_slot, "
        "    lcr.new_position, " // lcr.new_position AS dest_pos, // s2.cryovial_position as dest_pos
        "    cs.box_cid, sb.external_name AS src_box, cs.cryovial_position AS source_pos,  "
        "    db.external_name AS dest_box, "
        "    db.box_type_cid AS dest_box_type, "
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
        "    cs.box_cid          = sb.box_cid " //"    AND db.status != 99 AND sb.status != 99"
        " ORDER BY "
        //"    section, rj_box_cid, lcr_position";
        "    section, lcr_position"; // lcr_position should make sure backup secondaries are after primaries
         //, aliquot_type_cid "
        //<< (primary_aliquot < secondary_aliquot ? "ASC" : "DESC"
    qd.setSQL(oss.str());
    debugMessage = "open query"; Synchronize((TThreadMethod)&debugLog);
    debugMessage = oss.str(); Synchronize((TThreadMethod)&debugLog);
    qd.setParam("rtid", collect->job->getID()); //int retrieval_cid = job->getID();
    qd.open();

    rowCount = 0; // class variable
    //int curchunk = 1, chunk = 0;
    debugMessage = "foreach row"; Synchronize((TThreadMethod)&debugLog);
    while (!qd.eof()) {
        if (0 == rowCount % 10) { ostringstream oss; oss<<"Found "<<rowCount<<" vials"; loadingMessage = oss.str().c_str(); Synchronize((TThreadMethod)&updateStatus); }

//		chunk = qd.readInt("chunk");
//        //wstringstream oss; oss<<__FUNC__<<oss<<"chunk:"<<chunk<<", rowCount: "<<rowCount; OutputDebugString(oss.str().c_str());
//        if (chunk > curchunk) { // new chunk, add the previous one
//            collect->addChunk(curchunk, rowCount-1);
//            curchunk = chunk;
//        }

        SampleRow * row = new SampleRow(
            qd.readInt(     "project_cid"),
            new LCDbBoxRetrieval(qd),
            new LPDbCryovial(qd),
            new LPDbCryovialStore(qd),
            new LCDbCryovialRetrieval(qd),
            qd.readString(  "cryovial_barcode"),
            qd.readString(  "src_box"),
            qd.readInt(     "box_id"), //qd.readInt(     "dest_id"),
            qd.readString(  "dest_name"),
            qd.readInt(     "dest_box_type"),
            qd.readInt(     "new_position"), // not AS dest_pos
            "", 0, "", 0, 0, "", 0); // no storage details yet

        //row->dest_type_name = Util::boxTubeTypeName(row->project_cid, row->dest_box_id).c_str();
        row->dest_type_name = Util::boxTubeTypeName(row->cbr_record->getProjId(), row->dest_box_id).c_str();
        //main->getStorage(row);

        const int aliquotType = row->cryo_record->getAliquotType();
        if (aliquotType == secondary_aliquot) {
            collect->secondaries.push_back(row);
        } else { // everything else, even if not explicitly primary
            collect->primaries.push_back(row);
        }

        qd.next();
        rowCount++;
    } oss.str(""); oss<<"finished loading "<<rowCount<<" samples"; debugMessage = oss.str(); Synchronize((TThreadMethod)&debugLog);

    // try to match secondaries with primaries on same destination position
    main->combineAliquots(collect->primaries, collect->secondaries, collect->combined);
    int combinedCount = collect->combined.size();

//    // add box tube type name
//    for (auto &row: collect->combined) {
//            row->dest_type_name = Util::boxTubeTypeName(row->cbr_record->getProjId(), row->dest_box_id).c_str();
//            main->getStorage(row);
//        if (NULL != row->backup) {
//            row->backup->dest_type_name = Util::boxTubeTypeName(row->backup->cbr_record->getProjId(), row->backup->dest_box_id).c_str();
//            main->getStorage(row->backup);
//        }
//    }

    // add box tube type name
    rowCount = 0;
    //ostringstream oss; oss<<"Adding storage details ["<<rowCount<<"/"<<combinedCount<<"]"; loadingMessage = oss.str().c_str(); Synchronize((TThreadMethod)&updateStatus); }
    for (auto &row: collect->combined) {
        if (0 == ++rowCount % 10) { ostringstream oss; oss<<"Adding storage details ["<<rowCount<<"/"<<combinedCount<<"]"; loadingMessage = oss.str().c_str(); Synchronize((TThreadMethod)&updateStatus); }
        addSampleDetails(row);
    }

    // create chunks
    rowCount = 0; // class variable
    int curchunk = 1; // the first row is the start of the first chunk
    int chunk = 0;
    for (auto &row: collect->combined) {
		chunk = row->cbr_record->getSection();

        if (chunk > curchunk) { // new chunk, add chunk object at *end* of each set of samples
            collect->addChunk(curchunk, rowCount-1);//rowCount-1);
            curchunk = chunk;
        }
        rowCount++;
    }
    collect->addChunk(curchunk, rowCount-1); // the last chunk
}

void LoadPlanThread::addSampleDetails(SampleRow * row) {

    row->dest_type_name = Util::boxTubeTypeName(row->cbr_record->getProjId(), row->dest_box_id).c_str();
    main->getStorage(row);
    if (NULL != row->backup) {
        row->backup->dest_type_name = Util::boxTubeTypeName(row->backup->cbr_record->getProjId(), row->backup->dest_box_id).c_str();
        main->getStorage(row->backup);
    }
}

void __fastcall TfrmRetrAsstCollectSamples::loadPlanThreadTerminated(TObject *Sender) {
	progressBottom->Style = pbstNormal; progressBottom->Visible = false; panelLoading->Visible = false; Enabled = true; Screen->Cursor = crDefault;
    try {
        showChunks();
    } catch (std::exception & e) {
        TfrmRetrievalAssistant::msgbox(e.what());
    }
    DEBUGSTREAM(__FUNC__<<"loadRows for job "<<job->getID()<<" finished")
}

void TfrmRetrAsstCollectSamples::showCurrentRow() {
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

void TfrmRetrAsstCollectSamples::showDetails(SampleRow * sample) {
    if (NULL == sample) {
        labelSampleID->Caption  = "";
        labelStorage->Caption   = "Chunk completed";
        labelDestbox->Caption   = "";
        labelDestype->Caption   = "";
    } else {
        labelSampleID->Caption  = sample->cryovial_barcode.c_str();
        labelStorage->Caption   = sample->storage_str().c_str();
        labelDestbox->Caption   = sample->dest_str().c_str();
        labelDestype->Caption   = sample->dest_type_name.c_str();
    }
}

void TfrmRetrAsstCollectSamples::addChunk(int number, int endRowAbs) { // don't assume chunk/section numbers instead of assuming it's the next number?
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

void TfrmRetrAsstCollectSamples::accept(String barcode) { // fixme check correct vial; could be missing, swapped etc
    SampleRow * primary = currentSample();  // could be primary, primary w/backup, or secondary
    SampleRow * aliquot = currentAliquot(); // primary or secondary - this is a bit confusing
    switch (aliquot->lcr_record->getStatus()) {
        case LCDbCryovialRetrieval::EXPECTED:
        case LCDbCryovialRetrieval::IGNORED:
            break; // ok, carry on
        case LCDbCryovialRetrieval::COLLECTED:
			TfrmRetrievalAssistant::msgbox("Already collected - please inform Core Programming"); return;
        case LCDbCryovialRetrieval::NOT_FOUND:
            if (IDOK != Application->MessageBox(L"Confirm sample has now been found", L"Question", MB_OKCANCEL)) return;
    }
    if (barcode == aliquot->cryovial_barcode.c_str()) { // save
        aliquot->lcr_record->setStatus(LCDbCryovialRetrieval::COLLECTED);
        if (aliquot == primary && primary->backup != NULL) { // has backup
            primary->backup->lcr_record->setStatus(LCDbCryovialRetrieval::IGNORED); //???
            TfrmRetrievalAssistant::msgbox("setting secondary status");
        } // else, it was the secondary - primary should

        //sample->lcr_record->setStatus(LCDbCryovialRetrieval::IGNORED); //???
        debugLog("Save accepted row");
        nextRow();
    } else {
        Application->MessageBox(L"Barcode not matched", L"Info", MB_OK);
    }
}

void TfrmRetrAsstCollectSamples::skip() { // defer
    currentAliquot()->lcr_record->setStatus(LCDbCryovialRetrieval::IGNORED); // not saved to db
    nextRow();
}

void TfrmRetrAsstCollectSamples::notFound() {
    DEBUGSTREAM("Save not found row")
    int rowRel = currentChunk()->getRowRel();
    SampleRow * sample = currentChunk()->objectAtRel(rowRel);
    if (sample->lcr_record->getStatus() != LCDbCryovialRetrieval::NOT_FOUND) {
        sample->lcr_record->setStatus(LCDbCryovialRetrieval::NOT_FOUND);
        if (sample->backup) {
            TfrmRetrievalAssistant::msgbox("Secondary aliquot found");
            if (sample->backup->lcr_record->getStatus() != LCDbCryovialRetrieval::NOT_FOUND) { // backup already marked not found
                fillRow(sample, rowRel + 1); // refresh sg row - now keeps pointer to row
                showCurrentRow();
                showDetails(sample->backup);
                labelPrimary->Enabled = false; labelSecondary->Enabled = true;
                return;
            } else {
                throw runtime_error("backup already NOT_FOUND");
            }
        } else {
            TfrmRetrievalAssistant::msgbox("No backup aliquot exists, continuing to next sample");
            nextRow();
        }
    } else {  // primary already marked not found
        if (sample->backup) {
            sample->backup->lcr_record->setStatus(LCDbCryovialRetrieval::NOT_FOUND);
            nextRow();
        } else {
            throw runtime_error("no secondary for not found primary, should have moved on to next row");
        }
    }
}

SampleRow * TfrmRetrAsstCollectSamples::currentSample() {
    return currentChunk()->objectAtRel(currentChunk()->getRowRel());  //return currentChunk()->currentObject();
}

SampleRow * TfrmRetrAsstCollectSamples::currentAliquot() {
    SampleRow * row = currentSample();
    if (row->lcr_record->getStatus() == LCDbCryovialRetrieval::NOT_FOUND) {
        if (NULL == row->backup) {
            return NULL;
        } else {
            return row->backup;
        }
    } else {
        return row;
    }
}

void TfrmRetrAsstCollectSamples::nextRow() {
/** save both primary and secondary - secondary aliquots should always be saved if present
* accept(): if primary aliquot !collected # expected, ignored, not found (now found?)
* save secondary as `IGNORED` if not required? primary was */
    Chunk< SampleRow > * chunk = currentChunk();
    //int current = chunk->getRowRel(); //SampleRow * sample = currentAliquot();  //
    SampleRow * sample = chunk->currentObject(); // which may be the secondary aliquot

    // save changes both primary and secondary in l_cryovial_retrieval (not cryovial/_store at this point)
    if (!sample->lcr_record->saveRecord(LIMSDatabase::getCentralDb())) { throw runtime_error("saveRecord() failed"); }
    if (sample->backup) {
        if (!sample->backup->lcr_record->saveRecord(LIMSDatabase::getCentralDb())) { throw runtime_error("saveRecord() failed for secondary"); }
    } // deferred (IGNORED) vials are not actually saved to the database, they remain EXPECTED

    // don't need to save chunk - completedness or otherwise of 'chunk' should be implicit from box/cryo plan
    if (chunk->getRowRel() < chunk->getSize()-1) {
        chunk->setRowAbs(chunk->nextUnresolvedAbs()); // fast-forward to first non-dealt-with row
    } else { // last row
        if (chunk->getSection() < (int)chunks.size()) {
            sgChunks->Row = sgChunks->Row+1; // next chunk
        }
    }
    labelPrimary->Enabled = true; labelSecondary->Enabled = false;
    showChunks();
    editBarcode->Clear();
    ActiveControl = editBarcode; // focus for next barcode

    if (Chunk< SampleRow >::Status::DONE == chunk->getStatus()) { // chunk is complete (no EXPECTED or REFERRED vials)
        chunkComplete(chunk);
    }

    if (isJobComplete()) { // job is complete
        if (IDYES != Application->MessageBox(L"Save job? Are all chunks completed?", L"Info", MB_YESNO)) return;
        ModalResult = mrOk;
    }
}

bool TfrmRetrAsstCollectSamples::isJobComplete() {
/** are all chunks complete? */
    for (auto &chunk : chunks) {
        if (Chunk< SampleRow >::Status::DONE != chunk->getStatus())
            return false;
    }
    return true;
}

void TfrmRetrAsstCollectSamples::chunkComplete(Chunk< SampleRow > * chunk) {
/** At the end of chunk, check if the chunk is actually finished (no REFERRED vials). If finished:
    * Require user to sign off
    * update cryo store records
    * calculate if there are any empty boxes
    * create tick list or switch list of boxes, empty/otherwise
    * ask user to comfirm that empty boxes are in fact empty
    * if error, create referred box (INVALID/EXTRA/MISSING CONTENT?) in `c_box_name` and/or `c_slot_allocation`
*/
    //int row = sgChunks->Row; if (row < 1) return;

    // If not finished

    // Require user to sign off
	frmConfirm->initialise(TfrmSMLogin::RETRIEVE, "Ready to sign off boxes"); // std::set<int> projects; projects.insert(job->getProjectID()); frmConfirm->initialise(TfrmSMLogin::RETRIEVE, "Ready to sign off boxes", projects);
	if (!RETRASSTDEBUG && mrOk != frmConfirm->ShowModal()) {
		Application->MessageBox(L"Signoff cancelled", L"Info", MB_OK);
		return; // fixme what now?
	}

//    Screen->Cursor = crSQLWait; Enabled = false; DEBUGSTREAM("save progress for job "<<job->getID()<<" started")
//    saveProgressThread = new SaveProgressThread();
//    saveProgressThread->OnTerminate = &saveProgressThreadTerminated;

    // update cryo store records
    // calculate if there are any empty boxes
    collectEmpties();
    // create tick list or switch list of boxes, empty/otherwise
    // ask user to comfirm that empty boxes are in fact empty
    // if error, create referred box (INVALID/EXTRA/MISSING CONTENT?) in `c_box_name` and/or `c_slot_allocation`
    // `c_box_name` and `c_slot_allocation` fields together == `l_box_arrival`

}

void TfrmRetrAsstCollectSamples::exit() { // definitely exiting
/** update cryovial_store (old and new, primary and secondary) when they enter their password to confirm */
	frmConfirm->initialise(TfrmSMLogin::RETRIEVE, "Ready to sign off boxes"); // std::set<int> projects; projects.insert(job->getProjectID()); frmConfirm->initialise(TfrmSMLogin::RETRIEVE, "Ready to sign off boxes", projects);
	if (!RETRASSTDEBUG && mrOk != frmConfirm->ShowModal()) {
		Application->MessageBox(L"Signoff cancelled", L"Info", MB_OK);
		return; // fixme what now?
	}

	showProgressMessage(progressMessage); Screen->Cursor = crSQLWait; Enabled = false;
    DEBUGSTREAM("save progress for job "<<job->getID()<<" started")

    saveProgressThread = new SaveProgressThread();
    saveProgressThread->OnTerminate = &saveProgressThreadTerminated;
}

void __fastcall SaveProgressThread::Execute() {
/** check cryo/store old/new params correct for `LCDbCryovialRetrieval`
`c_box_retrieval`: set `time_stamp`, `status` = 1 (PART_FILLED)
`box_name` (if record): update `time_stamp`, `box_capacity`, `status=1` (IN_USE)
`c_box_name` (if record): update `time_stamp`, `box_capacity`, `status=1` (IN_USE)
how to update boxes? check at save and exit that all vials in a box have been saved? */
	typedef std::set< SampleRow * > SetOfVials;
	typedef std::map< int, SetOfVials > VialsInBoxesMap;
	VialsInBoxesMap boxes;
	VialsInBoxesMap::iterator found;

	frmRetrAsstCollectSamples->unactionedSamples = false; frmRetrAsstCollectSamples->info.clear(); frmRetrAsstCollectSamples->warnings.clear(); frmRetrAsstCollectSamples->errors.clear();
	try {
		for (vector<SampleRow *>::iterator it = frmRetrAsstCollectSamples->combined.begin(); it != frmRetrAsstCollectSamples->combined.end(); ++it) {
			SampleRow * sample = *it;

			int sourceBox = sample->store_record->getBoxID(); // should get id of secondary box as well and add it to map, we are checking for all empty boxes

            // build a map of source box ID to a set of vials contained in that box
			found = boxes.find(sourceBox);
			if (found == boxes.end()) { // not added yet
				SetOfVials setOfVials;
				setOfVials.insert(sample);
				boxes[sourceBox] = setOfVials;
			} else { // already in map
				found->second.insert(sample);
			}

            ostringstream oss; oss<<sample->cryovial_barcode<<" "<<sample->aliquotName<<" in box with id "<<sourceBox;
            loadingMessage = oss.str().c_str(); Synchronize((TThreadMethod)&updateStatus);

			int status  = sample->lcr_record->getStatus();
			if (status != LCDbCryovialRetrieval::EXPECTED && status != LCDbCryovialRetrieval::IGNORED) { // changed
				storeSample(sample);
			} else {
				frmRetrAsstCollectSamples->unactionedSamples = true;
			}
            if (NULL != sample->backup) {
                storeSample(sample->backup);
            }
		}

        frmRetrAsstCollectSamples->emptyBoxes.clear();  //std::set< int > discardBoxes; //std::set< LCDbBoxRetrieval * > discardBoxes;

		// now check for completed boxes
		for (found = boxes.begin(); found != boxes.end(); found++) { // for each source box
			bool vialRemains = false;
			SetOfVials & setOfVials = found->second;
			SetOfVials::const_iterator it;
			for (it = setOfVials.begin(); it != setOfVials.end(); it++) { // for each vial in the box
				SampleRow * sample = *it;
				switch (sample->lcr_record->getStatus()) { // what about secondaries?
				case LCDbCryovialRetrieval::EXPECTED:
					vialRemains = true;
                default:
                    ;
				}
			}
			if (!vialRemains) { // empty/completed box, mark for discard
                frmRetrAsstCollectSamples->emptyBoxes.insert(found->first); // set of int box_cids, used in discardBoxes() //LCDbBoxRetrieval * box = found->first;
			}
		}
    } catch (std::exception & e) {
        TfrmRetrievalAssistant::msgbox(e.what());
    }

    if (frmRetrAsstCollectSamples->errors.size() != 0) {
        //msgbox
    } else if (frmRetrAsstCollectSamples->unactionedSamples) {
        ;
    } else if (!frmRetrAsstCollectSamples->unactionedSamples) { // job finished
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
    LQuery qp(Util::projectQuery(frmRetrAsstCollectSamples->job->getProjectID(), false)); //LQuery qd(Util::projectQuery(job->getProjectID(), true));
    LQuery qc(LIMSDatabase::getCentralDb());

    // all boxes must be finished if here (ie. all samples are finished)
    qc.setSQL("SELECT * FROM c_box_retrieval WHERE retrieval_cid = :rtid");
    qc.setParam("rtid", frmRetrAsstCollectSamples->job->getID());
    qc.open();
    while (!qc.eof()) {
        // `c_box_retrieval`: set `time_stamp`, `status=2` (collected)
        LCDbBoxRetrieval cbr(qc);
        cbr.setStatus(LCDbBoxRetrieval::COLLECTED);
        cbr.saveRecord(LIMSDatabase::getCentralDb()); // time_stamp set by default - should be in 2.7.2

        // `box_name`: (if record): update `time_stamp`, `box_capacity`, `status=2` (CONFIRMED)

        // `c_box_name`: (if record): update `time_stamp`, `box_capacity`, `status=2` (CONFIRMED)
    }

    // `c_retrieval_job`: update `finish_date`, `status` = 2
    frmRetrAsstCollectSamples->job->setStatus(LCDbCryoJob::DONE);
    frmRetrAsstCollectSamples->job->saveRecord(qc); // finish date is updated by this method
}
// `cryovial_store`: as above (dealt with already?)

void __fastcall TfrmRetrAsstCollectSamples::saveProgressThreadTerminated(TObject *Sender) {
	progressBottom->Style = pbstNormal; progressBottom->Visible = false; panelLoading->Visible = false; Screen->Cursor = crDefault;
    try {
        // anything more to do?
        collectEmpties();
    } catch (std::exception & e) {
        TfrmRetrievalAssistant::msgbox(e.what());
    }

    Enabled = true; DEBUGSTREAM(__FUNC__<<"save plan for job "<<job->getID()<<" finished")

    vector<string>::const_iterator strIt;
    if (frmRetrAsstCollectSamples->errors.size() > 0) {
        ostringstream out;
        for (strIt = errors.begin(); strIt != errors.end(); strIt++) { out<<*strIt<<endl; }
        Application->MessageBox(String(out.str().c_str()).c_str(), L"Error", MB_OK);
        return;
	} else {
        delete_referenced< vector <SampleRow * > >(combined);
        delete_referenced< vector< Chunk< SampleRow > * > >(chunks); // chunk objects, not contents of chunks
        Close();
    }
}

void TfrmRetrAsstCollectSamples::discardBoxes() {
    LQuery qp(Util::projectQuery(job->getProjectID(), false));
    LPDbBoxNames boxNames;
    boxNames.readFilled(qp); // reads CONFIRMED [2], ANALYSED [3]. boxes.readCurrent(qp) reads EMPTY [0], IN_USE [1]
    const LPDbBoxName * pBoxName;
    typedef std::vector< LPDbBoxName > VecBoxes;
    VecBoxes boxes;

    if (!emptyBoxes.empty()) {
        if (IDYES != Application->MessageBox(L"There are empty boxes. Would you like to mark these as discarded?", L"Info", MB_YESNO)) {
            throw runtime_error("user did not want to deal with empty boxes");
        }
    }

    std::set< int >::const_iterator idIt;
    for (idIt = emptyBoxes.begin(); idIt != emptyBoxes.end(); idIt++) {
        int box_cid = *idIt;
        pBoxName = boxNames.findByID(box_cid);
        if (NULL == pBoxName) { // IN_TANK [4] records not read and findByID for those ids returns a null
            throw runtime_error("box not found"); //???
        } else {
            //pBoxName->saveRecord(qp); // can't do this on const *
            LPDbBoxName boxName = *(pBoxName); // so make mutable object
            boxes.push_back(boxName); // can't convert const * to *
            // not supposed to change - but I think we need to in this instance
        }
    }

    VecBoxes::const_iterator boxIt;
    for (boxIt = boxes.begin(); boxIt != boxes.end(); boxIt++) {
        LPDbBoxName box = *boxIt;
        // add to tick/switch list

        //boxName.setStatus(LPDbBoxName::Status::IN_TANK);
        //LQuery cq(LIMSDatabase::getCentralDb()); // for extra param to LPDbBoxName::saveRecord()
        //boxName.saveRecord(qp, cq);
        frmCollectEmpties->ShowModal();
    }
}

void TfrmRetrAsstCollectSamples::collectEmpties() {
/** if, at the end of processing a chunk, there are any source boxes which have become empty,
    the user may want to discard them instead of replacing them.
    if so, provide an option to discard these empty boxes, recording it in the database */

    // find out if there are any

    //if (IDYES != Application->MessageBox(L"There are empty boxes. Would you like to mark these as discarded?", L"Info", MB_YESNO)) return;

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

//    } catch (Exception & e) {
//		TfrmRetrievalAssistant::msgbox(e.Message);
//    }

//    } catch (Exception & e) {
//		TfrmRetrievalAssistant::msgbox(e.Message);
//    } catch (...) {
//        TfrmRetrievalAssistant::msgbox("problem displaying plan");
//    }

//	} catch(Exception & e) {
//		AnsiString msg = e.Message;
//		frmRetrAsstCollectSamples->errors.push_back(msg.c_str());
//		frmRetrAsstCollectSamples->unactionedSamples = true;
//    } catch(char * e) {
//		frmRetrAsstCollectSamples->errors.push_back(e);
//		frmRetrAsstCollectSamples->unactionedSamples = true;
//	} catch (...) {
//		frmRetrAsstCollectSamples->errors.push_back("Unknown error");
//		frmRetrAsstCollectSamples->unactionedSamples = true;
//    }

//if (NULL == box) throw runtime_error(__FUNC__ +" null LPDbBoxName");
//if (NULL == size) throw runtime_error(__FUNC__ +" null LCDbBoxSize");
//if (NULL == tube) throw runtime_error(__FUNC__ +" null LCDbObject");
//operator() (const ProjBox lhs, const ProjBox rhs) { return lhs.project_cid == rhs.project_cid && lhs.box_cid == rhs.box_cid; }

/*
Chunk< SampleRow >::NOT_STARTED:RETRIEVAL_ASSISTANT_NEW_COLOUR;
Chunk< SampleRow >::INPROGRESS: RETRIEVAL_ASSISTANT_IN_PROGRESS_COLOUR;
Chunk< SampleRow >::DONE:       RETRIEVAL_ASSISTANT_COLLECTED_COLOUR;
*/

/*
    switch (status)  // could use currentAliquot() here?
        case LCDbCryovialRetrieval::EXPECTED:               background = RETRIEVAL_ASSISTANT_NEW_COLOUR;
        case LCDbCryovialRetrieval::IGNORED:                background = RETRIEVAL_ASSISTANT_IGNORED_COLOUR;
        case LCDbCryovialRetrieval::COLLECTED:              background = RETRIEVAL_ASSISTANT_COLLECTED_COLOUR;
        case LCDbCryovialRetrieval::NOT_FOUND:
            if (NULL != row->backup)
                switch (backupStatus)
                    case LCDbCryovialRetrieval::EXPECTED:   background = RETRIEVAL_ASSISTANT_SECONDARY_COLOUR
                    case LCDbCryovialRetrieval::IGNORED:    background = RETRIEVAL_ASSISTANT_IGNORED_COLOUR
                    case LCDbCryovialRetrieval::COLLECTED:  background = RETRIEVAL_ASSISTANT_COLLECTED_COLOUR
                    default:                                background = RETRIEVAL_ASSISTANT_NOT_FOUND_COLOUR
            else
                background = RETRIEVAL_ASSISTANT_NOT_FOUND_COLOUR */

// could use combineAliquots()?
        // void LoadVialsJobThread::combineAliquots(const vecpSampleRow & primaries, const vecpSampleRow & secondaries, vecpSampleRow & combined) {
        // primary_aliquot and secondary_aliquot are already defined
        //int currentAliquotType = row->cryo_record->getAliquotType();
        //int previousAliquotType = previous == NULL? 0 : previous->cryo_record->getAliquotType();
//        if (secondary_aliquot != 0 &&
//            secondary_aliquot == currentAliquotType &&
//            previous != NULL &&
//            previous->cryovial_barcode == row->cryovial_barcode) { // secondary aliquot, previous was primary of same sample
//            if (previousAliquotType == currentAliquotType) {
//                throw runtime_error("duplicate aliquot");
//            } else if (currentAliquotType != secondary_aliquot) {
//                throw runtime_error("spurious aliquot");
//            } else { // secondary
//                previous->backup = row;
//            }
//        } else {
//            frmRetrAsstCollectSamples->combined.push_back(row); // new primary
//            previous = row;
//            rowCount++; // only count primary aliquots
//        }

        // add box tube type name
        // I put this in a loop outside the main loop to avoid running another query...
        // doesn't seem to matter that it's not?
        //for (vector<SampleRow *>::iterator it = frmRetrAsstCollectSamples->combined.begin(); it != frmRetrAsstCollectSamples->combined.end(); ++it) {//, rowCount2++) {
//        for (vector<SampleRow *>::iterator it = frmRetrAsstCollectSamples->combined.begin(); it != frmRetrAsstCollectSamples->combined.end(); ++it) {//, rowCount2++) {
//            (*it)->dest_type_name = Util::boxTubeTypeName((*it)->project_cid, (*it)->dest_box_id).c_str();
//        }

//    int size1 = collect->primaries.size(), size2 = collect->secondaries.size(), size3 = collect->combined.size();
/*
    // find locations of source boxes
    // fixme should get storage for secondaries as well?
    // fixme this was put outside the main loop to avoid multiple queries as well - could actually be included in main loop?
    int rowCount2 = 0;
	for (vector<SampleRow *>::iterator it = collect->combined.begin(); it != collect->combined.end(); ++it, rowCount2++) {
        SampleRow * sample = *it;
        ostringstream oss; oss<<"Finding storage for "<<sample->cryovial_barcode<<" ["<<rowCount2<<"/"<<rowCount<<"]: ";
        main->getStorage(sample);
        if (NULL != sample->backup) {
            main->getStorage(sample->backup);
        }
        oss<<sample->storage_str(); loadingMessage = oss.str().c_str(); Synchronize((TThreadMethod)&updateStatus);
	} debugMessage = "finished load storage details"; Synchronize((TThreadMethod)&debugLog);
*/

//    // add box tube type name
//    for (auto &row: collect->primaries) {
//        try {
//            row->dest_type_name = Util::boxTubeTypeName(row->cbr_record->getProjId(), row->dest_box_id).c_str();
//        } catch (...) {
//            row->dest_type_name = "error";
//        }
//        try {
//            main->getStorage(row);
//        } catch (...) {
//            row->setLocation("error", 0, "error", 0, 0, "error", 0);
//        }
//        //row->dest_type_name = Util::boxTubeTypeName(row->cbr_record->getProjId(), row->dest_box_id).c_str();
//        //main->getStorage(row);
//    }
//
//    for (auto &row: collect->secondaries) {
//        try {
//            row->dest_type_name = Util::boxTubeTypeName(row->cbr_record->getProjId(), row->dest_box_id).c_str();
//        } catch (...) {
//            row->dest_type_name = "error";
//        }
//        try {
//            main->getStorage(row);
//        } catch (...) {
//            row->setLocation("error", 0, "error", 0, 0, "error", 0);
//        }
////        row->dest_type_name = Util::boxTubeTypeName(row->cbr_record->getProjId(), row->dest_box_id).c_str();
////        main->getStorage(row);
//    }

//const int pid = LCDbAuditTrail::getCurrent().getProcessID();
//wstringstream oss; oss<<__FUNC__<<oss<<"chunk:"<<chunk<<", rowCount: "<<rowCount; OutputDebugString(oss.str().c_str());
