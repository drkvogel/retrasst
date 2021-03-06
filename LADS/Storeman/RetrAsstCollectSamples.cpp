#include <vcl.h>
#pragma hdrstop
#include "StoreDAO.h"
#include "RetrAsstCollectSamples.h"
#include <ctime>
    // struct tm
#include <locale>
    // locale, time_put
#include "TfrmConfirm.h"
#include "SMLogin.h"
#include "LPDbBoxes.h"
#include "RetrAsstCollectEmpties.h"
#include "RetrAsstAddNote.h"


#pragma package(smart_init)
#pragma resource "*.dfm"

/*
 * Work through list or sub-section by giving the storage location and sample ID of each sample on the list in the order saved above (REQ 8.3.8);
 * As each sample is retrieved its barcode should be scanned, if the scanned barcode matches that on the list
the destination location should be displayed and the next ID/location should be displayed (REQ 8.3.9)
 * if the IDs do not match a warning should be displayed and re-entry of the barcode required (REQ 8.3.10).
     what if it's the wrong sample, or it's missing? handle this
 * When working through the list the previous five successfully entered IDs should always be visible (REQ 8.3.11).
 * The option to exit the process saving progress should be offered, with an "are you sure?" message in case of accidental selection (REQ 8.3.12).

extra:
 * if, at the end of processing a chunk, there are any source boxes which have become empty, the user may want to discard them instead of replacing them.
   if so, provide an option to discard these empty boxes, recording it in the database

 destination box+position, cryovial barcode and current box+position+structure+location of the primary and secondary aliquots. */

TfrmRetrAsstCollectSamples *frmRetrAsstCollectSamples;

void TfrmRetrAsstCollectSamples::debugLog(String s) {
    String tmp = Now().CurrentDateTime().DateTimeString() + ": " + s;
    memoDebug->Lines->Add(tmp); // could use varargs: http://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c
}


// fixme should put these in main but had a circular dependency problem, leave for now

__fastcall RetrAsstThread::RetrAsstThread() {
    main    = frmRetrievalAssistant;
    collect = frmRetrAsstCollectSamples;
    FreeOnTerminate = true;
}

void RetrAsstThread::debugLog(string body) {
    messageBody = body;
    Synchronize((TThreadMethod)&_debugLog);
}

void RetrAsstThread::_debugLog() {
    collect->debugLog(messageBody.c_str());
}

void RetrAsstThread::msgbox(string body, string title="Info") {
    messageBody  = body;
    messageTitle = title;
    Synchronize((TThreadMethod)&_msgbox);
}

void RetrAsstThread::_msgbox() {
    Application->MessageBox(String(messageBody.c_str()).c_str(), String(messageTitle.c_str()).c_str(), MB_OK);
}

void RetrAsstThread::updateStatus(string body, string title="Info") {
    messageBody  = body;
    messageTitle = title;
    Synchronize((TThreadMethod)&_updateStatus);
}

void RetrAsstThread::_updateStatus() {
    collect->panelLoading->Caption = messageBody.c_str(); collect->panelLoading->Repaint();
}

__fastcall TfrmRetrAsstCollectSamples::TfrmRetrAsstCollectSamples(TComponent* Owner) : TForm(Owner) {
    destroying = false;

    sgwChunks = new StringGridWrapper< Chunk< SampleRow > >(sgChunks, &chunks);
    sgwChunks->addCol("section",  "Section",  60);
    sgwChunks->addCol("status",   "Status",   91);
    sgwChunks->addCol("item",     "Item",     50);
    sgwChunks->addCol("progress", "Progress", 91);
    sgwChunks->addCol("start",    "Start",    70);
    sgwChunks->addCol("startbox", "Box",      250);
    sgwChunks->addCol("startvial","Vial",     150);
    sgwChunks->addCol("end",      "End",      66);
    sgwChunks->addCol("endbox",   "Box",      242);
    sgwChunks->addCol("endvial",  "Vial",     150);
    //sgwChunks->addCol("size",     "Size",     87);
    sgwChunks->init();

    sgwVials = new StringGridWrapper<SampleRow>(sgVials, &combined);
    sgwVials->addCol("item",     "Item",             30);
    sgwVials->addCol("barcode",  "Barcode",          91);
    sgwVials->addCol("site",     "Site",             90);
    sgwVials->addCol("vesspos",  "Loc",              28);
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
    labelVialKeyIgnored->Color              = RETRIEVAL_ASSISTANT_DEFERRED_COLOUR;
    labelVialKeyCollected->Color            = RETRIEVAL_ASSISTANT_COLLECTED_COLOUR;
    labelVialKeyNotFound->Color             = RETRIEVAL_ASSISTANT_NOT_FOUND_COLOUR;
}

void __fastcall TfrmRetrAsstCollectSamples::FormCreate(TObject *Sender) {
    cbLog->Visible      = RETRASSTDEBUG;
    cbLog->Checked      = false;
    sgVials->Enabled    = false; // dbg button enables it
    panelDebug->Visible = cbLog->Checked;
    job                 = NULL;
}

void __fastcall TfrmRetrAsstCollectSamples::FormClose(TObject *Sender, TCloseAction &Action) {

    TMsgDlgBtn myYes = mbYes;
    TMsgDlgBtn myNo = mbNo;
    if (MessageDlg("Close application ?", mtConfirmation, TMsgDlgButtons() << myYes << myNo,0) == mrYes) {
	    Action = caFree;
    } else {
	    //Action = caMinimize;
        Action = caNone;
    }

    TfrmRetrievalAssistant::msgbox("closed form");
    // Abort(); what does this do? Can't find in docs
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
                case LCDbCryovialRetrieval::DEFERRED:
                    background = RETRIEVAL_ASSISTANT_DEFERRED_COLOUR; break;
                case LCDbCryovialRetrieval::COLLECTED:
                    background = RETRIEVAL_ASSISTANT_COLLECTED_COLOUR; break;
                case LCDbCryovialRetrieval::NOT_FOUND:
                    if (NULL != row->backup) {
                        int backupStatus = row->backup->lcr_record->getStatus();
                        switch (backupStatus) {
                            case LCDbCryovialRetrieval::EXPECTED:
                                background = RETRIEVAL_ASSISTANT_SECONDARY_COLOUR; break;
                            case LCDbCryovialRetrieval::DEFERRED:
                                background = RETRIEVAL_ASSISTANT_DEFERRED_COLOUR; break;
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
        if (sampleRow->lcr_record->getStatus() == LCDbCryovialRetrieval::DEFERRED) {
            sampleRow->lcr_record->setStatus(LCDbCryovialRetrieval::EXPECTED);
        } else if ( sampleRow->lcr_record->getStatus() == LCDbCryovialRetrieval::NOT_FOUND && sampleRow->backup != NULL) {
            if (sampleRow->backup->lcr_record->getStatus() == LCDbCryovialRetrieval::DEFERRED) {
                sampleRow->backup->lcr_record->setStatus(LCDbCryovialRetrieval::EXPECTED);
            }
        }
    }

    showChunk(chunk);
}

void __fastcall TfrmRetrAsstCollectSamples::sgVialsClick(TObject *Sender) { // show details in debug window
    debugLog(".");
    SampleRow * sample = (SampleRow *)sgVials->Objects[0][sgVials->Row];
    ostringstream oss; oss<<"(prefer): "<<sample->debug_str(); debugLog(oss.str().c_str());
    SampleRow * backup = sample->backup; //shared_ptr< SampleRow > backup = sample->backup;
    if (!backup) { debugLog("(no backup)"); return; }
    oss.str(""); oss<<"(backup): "<<backup->debug_str(); debugLog(oss.str().c_str());
}

void __fastcall TfrmRetrAsstCollectSamples::editBarcodeKeyUp(TObject *Sender, WORD &Key, TShiftState Shift) {
    if (VK_RETURN == Key) { accept(AnsiString(editBarcode->Text).c_str()); }
}

void __fastcall TfrmRetrAsstCollectSamples::FormResize(TObject *Sender) { // gets called *after* FormDestroy
    if (!destroying) { // check in case pointers have been deleted in FormDestroy
        sgwChunks->resize(); sgwVials->resize();
    }
}

void __fastcall TfrmRetrAsstCollectSamples::cbLogClick(TObject *Sender) {
    panelDebug->Visible     = cbLog->Checked;
    sgVials->Enabled        = cbLog->Checked;
}

void __fastcall TfrmRetrAsstCollectSamples::FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift) {
/** form's KeyPreview property needs to be set to see this */
    if (Key == ' ') toggleLog();
    if (Key == 68)  skip();     // 'd'
}

void TfrmRetrAsstCollectSamples::toggleLog() {
    cbLog->Checked           = !(cbLog->Checked);
    panelDebug->Visible      = cbLog->Checked;
    splitterDebug->Visible   = cbLog->Checked;
    labelStorage->Visible    = cbLog->Checked;
    labelDestString->Visible = cbLog->Checked;
}

void __fastcall TfrmRetrAsstCollectSamples::menuItemExitClick(TObject *Sender) { checkExit(); }

void TfrmRetrAsstCollectSamples::showProgressMessage(const char * loadingMessage) {
	panelLoading->Caption = loadingMessage;
	panelLoading->Visible = true;
	panelLoading->Top = (sgVials->Height / 2) - (panelLoading->Height / 2);
    panelLoading->Left = (sgVials->Width / 2) - (panelLoading->Width / 2);
    progressBottom->Style = pbstMarquee; progressBottom->Visible = true;
}

void TfrmRetrAsstCollectSamples::hideProgressMessage() {
    progressBottom->Style = pbstNormal; progressBottom->Visible = false; panelLoading->Visible = false; //Screen->Cursor = crDefault;
}

void __fastcall TfrmRetrAsstCollectSamples::FormShow(TObject *Sender) {
    ostringstream oss; oss<<job->getName()<<" : "<<job->getDescription()<<" [id: "<<job->getID()<<"]";
    Caption = oss.str().c_str();
    chunks.clear();
    sgwChunks->clear();
    sgwVials->clear();
    frmRetrievalAssistant->clearStorageCache();

    // sample ID
    labelSampleID->Caption  = "loading...";

    // source
    labelSrcPos->Caption    = "";
    labelSrcBox->Caption    = "loading...";
    labelSite->Caption      = "";
    labelVessel->Caption    = "";
    labelStructure->Caption = "";
    labelSlot->Caption      = "";

    // debug
    labelStorage->Caption    = "loading...";
    labelDestString->Caption = "loading...";
    labelStorage->Visible    = cbLog->Checked;
    labelDestString->Visible = cbLog->Checked;

    // destination
    labelDestPos->Caption   = "";
    labelDestBox->Caption   = "loading...";

    labelPrimary->Caption   = Util::getAliquotDescription(job->getPrimaryAliquot()).c_str();
    labelSecondary->Caption = Util::getAliquotDescription(job->getSecondaryAliquot()).c_str();
    labelPrimary->Enabled   = true;
    labelSecondary->Enabled = false;
    timerLoadPlan->Enabled  = true;
}

void TfrmRetrAsstCollectSamples::showChunks() {
    if (0 == chunks.size()) {
        //return; // e.g. for "cannot handle multiple projects"
        throw runtime_error("No chunks"); // must always have one chunk?
    } else {
        sgChunks->RowCount = chunks.size() + 1; sgChunks->FixedRows = 1;
    } // "Fixed row count must be LESS than row count"

    int row = 1;
    for (vector< Chunk< SampleRow > * >::const_iterator it = chunks.begin(); it != chunks.end(); it++, row++) {
        Chunk< SampleRow > * chunk = *it;
        sgChunks->Cells[sgwChunks->colNameToInt("section")]   [row] = chunk->getSection();
        sgChunks->Cells[sgwChunks->colNameToInt("status")]    [row] = chunk->statusString().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("item")]      [row] = chunk->getRowRel()+1;
        sgChunks->Cells[sgwChunks->colNameToInt("progress")]  [row] = chunk->progressString().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("start")]     [row] = chunk->getStartAbs() + 1;
        sgChunks->Cells[sgwChunks->colNameToInt("startbox")]  [row] = chunk->getStartBox().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("startvial")] [row] = chunk->getStartVial().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("end")]       [row] = chunk->getEndAbs() + 1;
        sgChunks->Cells[sgwChunks->colNameToInt("endbox")]    [row] = chunk->getEndBox().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("endvial")]   [row] = chunk->getEndVial().c_str();
        //sgChunks->Cells[sgwChunks->colNameToInt("size")]      [row] = chunk->getSize();
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

void TfrmRetrAsstCollectSamples::showChunk(Chunk< SampleRow > * chunk) { // default: NULL
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
        SampleRow * sampleRow = chunk->objectAtRel(row);
        fillRow(sampleRow, row+1); // row+1 for stringgrid
    }
    if (1.0 == chunk->getProgress()) { // completed
        btnAccept->Enabled      = false;
        btnDefer->Enabled       = false;
        btnNotFound->Enabled    = false;
        btnBadVial->Enabled     = false;
        btnAddNote->Enabled     = true;
    } else {
        btnAccept->Enabled      = true;
        btnDefer->Enabled       = true;
        btnNotFound->Enabled    = true;
        btnBadVial->Enabled     = true;
        btnAddNote->Enabled     = true;
    }

    showCurrentRow();

    // progress including current item is only known once vials are displayed and next unactioned sample is found
    int currentChunkRow = sgChunks->Row;
    string temp  = chunk->progressString().c_str();
    sgChunks->Cells[sgwChunks->colNameToInt("item")]      [currentChunkRow] = chunk->getRowRel()+1;
    sgChunks->Cells[sgwChunks->colNameToInt("progress")]  [currentChunkRow] = chunk->progressString().c_str();

    Screen->Cursor = crDefault; Enabled = true;
}

void TfrmRetrAsstCollectSamples::fillRow(SampleRow * row, int rw) { // shared_ptr < SampleRow > row
    SampleRow * sample; //shared_ptr < SampleRow > sample;
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
    showProgressMessage("Loading retrieval list, please wait...");
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
    delete_referenced< vector<SampleRow * > >(collect->combined);
    //collect->combined.clear();
    collect->chunks.clear();
    int primary_aliquot = collect->job->getPrimaryAliquot(); int secondary_aliquot = collect->job->getSecondaryAliquot();
    ostringstream oss;
    int project_cid = collect->job->getProjectID();

    if (0 == project_cid) {
        // fixme handle multiple projects
        msgbox("cannot handle multiple projects at the moment - contact core programming");
        return;
    }

    const LCDbProject * proj = LCDbProjects::records().findByID(project_cid);
    oss<<__FUNC__<<": job: "<<collect->job->getName()<<", project: "<<proj->getName()<<" ["<<proj->getID()<<"], "<<proj->getDbName();
    debugLog(oss.str());

    LQuery qd(Util::projectQuery(project_cid, true)); // ddb
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
        "    section, lcr_position"; // lcr_position should make sure backup secondaries are after primaries
    qd.setSQL(oss.str());
    debugLog("open query");
    debugLog(oss.str());
    qd.setParam("rtid", collect->job->getID()); //int retrieval_cid = job->getID();
    qd.open();

    rowCount = 0; // class variable
    SampleRow * previous;
    debugLog("foreach row");
    while (!qd.eof()) {
        if (0 == rowCount % 10) { ostringstream oss; oss<<"Found "<<rowCount<<" vials"; updateStatus(oss.str()); }

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

        const int aliquotType = row->cryo_record->getAliquotType();
        if (    aliquotType         == secondary_aliquot
            &&  row->dest_box_id    == previous->dest_box_id
            &&  row->dest_cryo_pos  == previous->dest_cryo_pos) { // backup for previous
            previous->backup = row;
        } else { // everything else, even if not explicitly primary
            collect->combined.push_back(row);
        }

        previous = row;
        qd.next();
        rowCount++;
    }
    oss.str(""); oss<<"finished loading "<<rowCount<<" samples";
    debugLog(oss.str());
    int combinedCount = collect->combined.size();

    // add storage details and box tube type name
    rowCount = 0;
    for (auto &row: collect->combined) {
        if (0 == ++rowCount % 10) {
            ostringstream oss; oss<<"Finding storage details ["<<rowCount<<"/"<<combinedCount<<"]";
            updateStatus(oss.str());
        }
        addSampleDetails(row); // adds storage details and box tube type name, good for combined aliquots
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

void TfrmRetrAsstCollectSamples::addChunk(int number, int endRowAbs) { // don't assume chunk/section numbers instead of assuming it's the next number?
/** add chunk with section = number, start row = end of previous chunk + 1, end row = endRowAbs */
    Chunk< SampleRow > * newchunk;
    if (chunks.size() == 0) { // first chunk
        newchunk = new Chunk< SampleRow >(sgwVials, number, 0, endRowAbs);
    } else {
        int previousEnd = chunks[chunks.size() - 1]->getEndAbs();
        newchunk = new Chunk< SampleRow >(sgwVials, number, previousEnd + 1, endRowAbs);
    }
    chunks.push_back(newchunk);
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
        ModalResult = mrCancel;
    }
    DEBUGSTREAM(__FUNC__<<"loadRows for job "<<job->getID()<<" finished")
}

void TfrmRetrAsstCollectSamples::showCurrentRow() {
    SampleRow * sample; Chunk<SampleRow> * chunk = currentChunk();

    int next = chunk->nextUnresolvedAbs();
    if (Chunk< SampleRow >::NextUnresolvedStatus::NONE_FOUND == next) { // ie. past the end, chunk completed/deferred
        sample = NULL;                  // no details to show
        sgVials->Row = 1; //fixme       // just show the last row
        return;
    } else {
        chunk->setRowAbs(next); // fast-forward to first non-dealt-with row
        int rowRel = chunk->getRowRel();
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

    // flash src and/or dest box if they are different to previous
    static int lastSrcBox = 0, lastDstBox = 0;
    if (sample->store_record->getBoxID() != lastSrcBox) {
        flash(groupSource, clRed);
        lastSrcBox = sample->store_record->getBoxID();
    }
    if (sample->dest_box_id != lastDstBox) {
        flash(groupDest, clBlue);
        lastDstBox = sample->dest_box_id;
    }
}

void TfrmRetrAsstCollectSamples::showDetails(SampleRow * sample) {
    if (NULL == sample) {
        labelSrcPos->Caption    = "";
        labelSrcBox->Caption    = "";
        labelSite->Caption      = "";
        labelVessel->Caption    = "";
        labelStructure->Caption = "";
		labelSlot->Caption      = "";
		labelSampleID->Caption  = "";
		labelDestPos->Caption   = "";
		labelDestBox->Caption   = "";
		labelStorage->Caption   = "";//???"Chunk completed";
	} else {
		labelSrcPos->Caption    = sample->store_record->getPosition();  // source
		labelSrcBox->Caption    = sample->src_box_name.c_str();
		labelSite->Caption      = sample->site_name.c_str();
		labelVessel->Caption    = sample->vessel_name.c_str();
		labelStructure->Caption = sample->structure_name.c_str();
		labelSlot->Caption      = sample->box_pos;
		labelSampleID->Caption  = sample->cryovial_barcode.c_str();     // sample ID
        labelDestPos->Caption   = sample->dest_cryo_pos;                // destination
        labelDestBox->Caption   = sample->dest_box_name.c_str();
		labelDestString->Caption   = sample->dest_str().c_str();
		labelStorage->Caption   = sample->storage_str().c_str();        // debug
	}
}

void TfrmRetrAsstCollectSamples::flash(TGroupBox *box, TColor other) { // TControl::Color is protected
    TColor orig = box->Color;
    int sleepms = RETRASSTDEBUG ? 10 : 250; // get on with it : HSE-approved
    for (int i = 0; i < 4; i++) {
        if (i % 2) {
            box->Color = orig;
        } else {
            box->Color = other;
        }
        Sleep(sleepms); // ms
        /* In the UK, the flash rate of strobe lights is restricted to a maximum of
        four flashes a second by the Health and Safety Executive. This rate is considered to be safe for most people.
        http://www.nhs.uk/ipgmedia/national/epilepsy%20action/assets/photosensitiveepilepsy.pdf */
        Repaint(); // ParentBackground propery of component must be set to false to see change in colour
    }
}

void __fastcall TfrmRetrAsstCollectSamples::btnSimAcceptClick(TObject *Sender) { // simulate a correct barcode scanned
    editBarcode->Text = (currentChunk()->currentObject())->cryovial_barcode.c_str();
    btnAcceptClick(this);
}

void __fastcall TfrmRetrAsstCollectSamples::btnAcceptClick(TObject *Sender) {
    accept(AnsiString(editBarcode->Text).c_str());
    //std::wstring mywstring = editBarcode->Text.c_str();
}

void TfrmRetrAsstCollectSamples::accept(string entered) { // fixme check correct vial; could be missing, swapped etc

    SampleRow * primary = currentSample();  // could be primary, primary w/backup, or secondary
    SampleRow * aliquot = currentAliquot(); // primary or secondary - this is a bit confusing
    switch (aliquot->lcr_record->getStatus()) {
        case LCDbCryovialRetrieval::EXPECTED:
        case LCDbCryovialRetrieval::DEFERRED:
            break; // ok, carry on
        case LCDbCryovialRetrieval::COLLECTED:
			TfrmRetrievalAssistant::msgbox("Already retrieved"); return;
        case LCDbCryovialRetrieval::NOT_FOUND:
            if (IDOK != Application->MessageBox(L"Confirm sample has now been found", L"Question", MB_OKCANCEL)) return;
    }
    if (entered == aliquot->cryovial_barcode) { // save
        if (aliquot == primary) {
            if (primary->backup != NULL) { // has backup
                primary->backup->lcr_record->setStatus(LCDbCryovialRetrieval::DEFERRED);
            }
        } else { // it was the secondary - primary should already have been set NOT_FOUND, but make sure
            if (primary->lcr_record->getStatus() != LCDbCryovialRetrieval::NOT_FOUND) {
                throw runtime_error("primary not set NOT_FOUND"); //primary->lcr_record->setStatus(LCDbCryovialRetrieval::NOT_FOUND); //???
            }
        }
        aliquot->lcr_record->setStatus(LCDbCryovialRetrieval::COLLECTED); // set COLLECTED, primary or secondary
        debugLog("Save accepted row");
        nextRow();
    } else {
        frmWrongBarcode->expected   = aliquot->cryovial_barcode.c_str();
        frmWrongBarcode->entered    = entered;
        frmWrongBarcode->ShowModal();
        editBarcode->Text = ""; // in case not cleared up in TfrmWrongBarcode, but should be
    }
}

void __fastcall TfrmRetrAsstCollectSamples::btnAddNoteClick(TObject *Sender) { addNote(); }

void TfrmRetrAsstCollectSamples::addNote() {
    //Application->MessageBox(L"Add a note", L"Info", MB_OK);
    // fixme
    // existing form to do this? no
    ostringstream oss; oss<<"a note"; // barcode: '"<<entered.c_str()<<"', expected: '"<<expected.c_str()<<"'";
    frmRetrAsstAddNote->setText(oss.str());
    frmRetrAsstAddNote->ShowModal();
    // if note clicked on complete chunk, add note to chunk?
}

void __fastcall TfrmRetrAsstCollectSamples::btnBadVialClick(TObject *Sender) { badVial(); }

void TfrmRetrAsstCollectSamples::badVial() {
    // fixme
    Application->MessageBox(L"Bad Vial", L"Info", MB_OK);
    Application->MessageBox(L"What to do?", L"Info", MB_OK);
}

void __fastcall TfrmRetrAsstCollectSamples::btnNotFoundClick(TObject *Sender) { notFound(); }

void TfrmRetrAsstCollectSamples::notFound() {
    DEBUGSTREAM("Save not found row")
    int rowRel = currentChunk()->getRowRel();
    SampleRow * sample = currentChunk()->objectAtRel(rowRel);
    if (sample->lcr_record->getStatus() != LCDbCryovialRetrieval::NOT_FOUND) {
        sample->lcr_record->setStatus(LCDbCryovialRetrieval::NOT_FOUND);
        if (sample->backup) {
            TfrmRetrievalAssistant::msgbox("Backup aliquot found");
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
            throw runtime_error("no backup for NOT_FOUND primary, should have moved on to next row");
        }
    }
}

void TfrmRetrAsstCollectSamples::replace(string barcode) {
    // replace

    TfrmRetrievalAssistant::msgbox("Todo: replace expected sample with found one");
    //TfrmRetrievalAssistant::msgbox("Todo: And go to next row - replace() should be a method in TfrmRetrAsstCollectSamples");
    //frmRetrAsstCollectSamples->replace();

    // in lcr
    //sample->backup->lcr_record->setStatus(LCDbCryovialRetrieval::REPLACED);

    // in cryovial store, or later?
    // add record for 'new' vial


    nextRow();

}

void __fastcall TfrmRetrAsstCollectSamples::btnDeferClick(TObject *Sender) { skip(); }

void TfrmRetrAsstCollectSamples::skip() { // defer
    currentAliquot()->lcr_record->setStatus(LCDbCryovialRetrieval::DEFERRED); // not saved to db
    // fixme actually, record as DEFERRED in db
    nextRow();
}

void __fastcall TfrmRetrAsstCollectSamples::btnSaveClick(TObject *Sender) {
    Application->MessageBox(L"C.", L"Question", MB_OK);
    Application->MessageBox(L"Are you sure you want to exit?\n\nCurrent progress will be saved.", L"Question", MB_OK);
    //checkExit();
} //exit(); }

void TfrmRetrAsstCollectSamples::checkExit() {
    if (IDYES == Application->MessageBox(L"Are you sure you want to exit?\n\nCurrent progress will be saved.", L"Question", MB_YESNO)) {
        exit();
    }
}

void TfrmRetrAsstCollectSamples::exit() { // definitely exiting

    // is this "Exit for now" or "Job finished"?

    // fixme 2014-07-29
    // "strong warning

	frmConfirm->initialise(TfrmSMLogin::RETRIEVE, "Ready to sign off boxes"); // std::set<int> projects; projects.insert(job->getProjectID()); frmConfirm->initialise(TfrmSMLogin::RETRIEVE, "Ready to sign off boxes", projects);
	if (!RETRASSTDEBUG && mrOk != frmConfirm->ShowModal()) {
		Application->MessageBox(L"Signoff cancelled", L"Info", MB_OK);
		return; // fixme what now?
	}
    saveProgress();//???
}

void __fastcall TfrmRetrAsstCollectSamples::btnSignOffClick(TObject *Sender) {
    // fixme
    // check all vials actioned in some way, i.e. not EXPECTED
    // mark all deferred NOT_FOUND ?
    Application->MessageBox(L"What should this do?", L"Info", MB_OK);
}


//void TfrmRetrAsstCollectSamples::foundElsewhere() {
///** the vial expected to be in the source location was found somewhere else */
//// should be pretty much the same as accept but doesn't try to match the barcode, updates it and saves with a new status
//    Application->MessageBox(L"The vial that was supposed to be found in this location was found somewhere else.", L"Info", MB_OK);
//    Application->MessageBox(L"Record it's actual found location?", L"Info", MB_OK);
//    Application->MessageBox(L"Save it as normal as either COLLECTED or a new status (FOUND_ELSEWHERE?)", L"Info", MB_OK);
//}
//void TfrmRetrAsstCollectSamples::wrongVial() {
///** the vial found in the source location is not the expected one */
//    Application->MessageBox(L"The vial found in this position is not the expected one. ", L"Info", MB_OK);
//    Application->MessageBox(L"Barcode should have been scanned in, accept it instead of rejecting", L"Info", MB_OK);
//    Application->MessageBox(L"Save to database as COLLECTED or new status (WRONG_VIAL?).", L"Info", MB_OK);
//}//void TfrmRetrAsstCollectSamples::alreadyRetrieved() {//    Application->MessageBox(L"Vial has already been retrieved", L"Info", MB_OK);
//    Application->MessageBox(L"What to do?", L"Info", MB_OK);
//}


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
/** save primary (and backup) in l_cryovial_retrieval, but not cryovial/_store at this point, move to next */
    Chunk< SampleRow > * chunk = currentChunk();
    SampleRow * sample = chunk->currentObject(); // which may be the backup aliquot

    if (!sample->lcr_record->saveRecord(LIMSDatabase::getCentralDb())) { throw runtime_error("saveRecord() failed"); }
    if (sample->backup) { // fixme what is being saved if backup was not needed?
        if (!sample->backup->lcr_record->saveRecord(LIMSDatabase::getCentralDb())) { throw runtime_error("saveRecord() failed for backup"); }
    } // deferred (DEFERRED) vials are not actually saved to the database, they remain EXPECTED

    int nextAbs = chunk->nextUnresolvedAbs(); // fast-forward to first non-dealt-with row
    if (nextAbs != Chunk< SampleRow >::NextUnresolvedStatus::NONE_FOUND) {
        chunk->setRowAbs(nextAbs);
    } else { // past last row
        if (chunk->getSection() < (int)chunks.size()) {
            sgChunks->Row = sgChunks->Row+1; // next chunk
        } else {
            sgChunks->Row = 1; //  could cause a loop but should be picked up by isJobComplete() //TfrmRetrievalAssistant::msgbox("End of last chunk");
        }
    }
    showChunks(); // don't need to save chunk - completedness or otherwise of 'chunk' should be implicit from box/cryo plan
    labelPrimary->Enabled = true; labelSecondary->Enabled = false; editBarcode->Clear(); ActiveControl = editBarcode; // focus for next barcode

    if (Chunk< SampleRow >::Status::DONE == chunk->getStatus()) { // check if chunk is complete (no EXPECTED or DEFERRED vials)
        chunkCompleted(chunk);
    }
}

void TfrmRetrAsstCollectSamples::chunkCompleted(Chunk< SampleRow > * chunk) {
    // Require user to sign off, skip in debug
    if (RETRASSTDEBUG) {
        //if (mrYes != Application->MessageBox(L"'Sign off' (debug)?", L"Info", MB_YESNO)) {
        if (mrYes != Application->MessageBox(L"'Sign off' (debug)?", L"Info", MB_YESNO)) {
		    Application->MessageBox(L"Signoff cancelled - what now?", L"Info", MB_OK); return;
        }
    } else {
	    frmConfirm->initialise(TfrmSMLogin::RETRIEVE, "Ready to sign off boxes"); // std::set<int> projects; projects.insert(job->getProjectID()); frmConfirm->initialise(TfrmSMLogin::RETRIEVE, "Ready to sign off boxes", projects);
	    if (mrOk != frmConfirm->ShowModal()) {
		    Application->MessageBox(L"Signoff cancelled", L"Info", MB_OK); return; // fixme what now?
        }
	}
    saveProgress();
}

void TfrmRetrAsstCollectSamples::saveProgress() {
/** initiate SaveProgressThread */
    info.clear(); warnings.clear(); errors.clear();
    saveProgressThread = new SaveProgressThread();
    saveProgressThread->OnTerminate = &saveProgressThreadTerminated;
}

/** check cryo/store old/new params correct for `LCDbCryovialRetrieval`
`c_box_retrieval`: set `time_stamp`, `status` = 1 (PART_FILLED)
`box_name` (if record): update `time_stamp`, `box_capacity`, `status=1` (IN_USE)
`c_box_name` (if record): update `time_stamp`, `box_capacity`, `status=1` (IN_USE)
check at save and exit if all vials in a box have been saved and if so update box */
void __fastcall SaveProgressThread::Execute() {
/** */
    try {
        updateStorage(); /** update cryo store records */
        findEmpties();   /** calculate if there are any empty boxes */
    } catch (...) {

    }
}

void SaveProgressThread::findEmpties() {
/** updates TfrmRetrAsstCollectSamples::emptyBoxes */
	typedef std::set< SampleRow * > SetOfVials;
	typedef std::map< int, SetOfVials > VialsInBoxesMap;
	VialsInBoxesMap boxes;
	VialsInBoxesMap::iterator found;

    for (auto& sample : collect->combined) {
        int sourceBox = sample->store_record->getBoxID(); // should get id of secondary box as well and add it to map, we are checking for all empty boxes

        // build a map of source box ID to a set of vials contained in that box
        found = boxes.find(sourceBox);
        if (found == boxes.end()) { // not added yet
            SetOfVials setOfVials;
            setOfVials.insert(sample);
            boxes[sourceBox] = setOfVials;
        } else { // already in map
            found->second.insert(sample);
        } ostringstream oss; oss<<sample->cryovial_barcode<<" "<<sample->aliquotName<<" in box with id "<<sourceBox; updateStatus(oss.str());
    }

    collect->emptyBoxes.clear();

    // check for completed boxes
    for (auto &found : boxes) { // for each source box
        bool vialRemains = false;
        SetOfVials & setOfVials = found.second;
        SetOfVials::const_iterator it;
        for (auto &sample : setOfVials) { // for each vial in the box
            switch (sample->lcr_record->getStatus()) { // what about secondaries?
            case LCDbCryovialRetrieval::EXPECTED:
                vialRemains = true;
            default:
                ;
            }
        }
        if (!vialRemains) { // empty/completed box, mark for discard
            collect->emptyBoxes.insert(found.first); // set of int box_cids, used in collectEmpties()
        }
    }
}

void SaveProgressThread::updateStorage() {
    for (auto& sample : collect->combined) {
    //for (int row=0; row < chunk->getSize(); row++) { // update cryo store records
        //SampleRow * sample = chunk->objectAtRel(row);
        LQuery pq(LIMSDatabase::getProjectDb(sample->project_cid));
        updateStorage(sample, pq);
        if (sample->backup) {
            updateStorage(sample->backup, pq); //??? could be different for backup?
        }
    }
}

void SaveProgressThread::updateStorage(SampleRow * aliquot, LQuery & q) {

/* * should be 4 cryovial_store recs/sample: src + dest * primary + secondary
	* new `NOT_FOUND` status (ALLOCATED, CONFIRMED, MOVE_EXPECTED, DESTROYED, ANALYSED, TRANSFERRED, NOT_FOUND, DELETED = 99) (no DEFERRED status?)
	* NOT_FOUND will be 6, not 7 - unless there is supposed to be another new status?

    // handle backups
    // deal with ignored, expected

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

/** update cryovial_store (old and new, primary and secondary) when they enter their password to confirm */
/* LPDbCryovialStore
enum Status { ALLOCATED, CONFIRMED, MOVE_EXPECTED, DESTROYED, ANALYSED, ALIQUOTS_TAKEN, NOT_FOUND, TRANSFERRED, DELETED = 99 };
    void setStatus( Status update ) { status = update; }
	Status getStatus() const { return Status( status ); }
    int getPosition() const { return position; }
    int getBoxID() const { return boxID; }
	bool saveRecord( LQuery query );
	bool setJobRef( LQuery project, int jobRef );    */

    LPDbCryovialStore * current = aliquot->store_record;
    if (NULL != current) {
        switch (aliquot->lcr_record->getStatus()) {
            case LCDbCryovialRetrieval::EXPECTED:
            case LCDbCryovialRetrieval::DEFERRED:
                return; //throw runtime_error("chunk should be complete");
            case LCDbCryovialRetrieval::COLLECTED:
                current->setStatus(LPDbCryovialStore::TRANSFERRED); break; //???
            case LCDbCryovialRetrieval::NOT_FOUND:
                current->setStatus(LPDbCryovialStore::NOT_FOUND); break;
            default:
                throw runtime_error("unexpected status");
        }
        current->saveRecord(q);
    } else throw runtime_error("NULL store_record"); // shouldn't be able to happen

    /* LPDbCryovialStore(int id, int cryovial, int box, short pos) */
    // add to box and use LPDbBoxName methods?
    LPDbBoxNames boxNames;
    boxNames.readFilled(q); // reads CONFIRMED [2], ANALYSED [3]. boxes.readCurrent(qp) reads EMPTY [0], IN_USE [1]
    const LPDbBoxName * pBoxName;

    //int box_cid = *idIt;
    pBoxName = boxNames.findByID(aliquot->dest_box_id);
    if (NULL == pBoxName) { // IN_TANK [4] records not read and findByID for those ids returns a null
        throw runtime_error("box not found"); //???
    } else {
        LPDbBoxName boxName = *(pBoxName); // make mutable object
        //boxes.push_back(boxName);
    }

    //LPDbCryovialStore retrieved(id, cryovial, box, pos); //???
//    LPDbCryovialStore retrieved(0,0,0,0); //???
//    switch (aliquot->lcr_record->getStatus()) {
//        case LCDbCryovialRetrieval::EXPECTED:
//        case LCDbCryovialRetrieval::DEFERRED:
//            throw runtime_error("chunk should be complete");
//        case LCDbCryovialRetrieval::COLLECTED:
//            retrieved.setStatus(LPDbCryovialStore::TRANSFERRED); break; //???
//        case LCDbCryovialRetrieval::NOT_FOUND:
//            retrieved->setStatus(LPDbCryovialStore::NOT_FOUND); break;
//        default:
//            throw runtime_error("unexpected status");
//    }
//    retrieved.saveRecord();
    /*
    LQuery qp(Util::projectQuery(job->getProjectID(), false));
    LPDbBoxNames boxNames;
    boxNames.readFilled(qp); // reads CONFIRMED [2], ANALYSED [3]. boxes.readCurrent(qp) reads EMPTY [0], IN_USE [1]
    const LPDbBoxName * pBoxName;

    int box_cid = *idIt;
    pBoxName = boxNames.findByID(box_cid);
    if (NULL == pBoxName) { // IN_TANK [4] records not read and findByID for those ids returns a null
        throw runtime_error("box not found"); //???
    } else {
        LPDbBoxName boxName = *(pBoxName); // make mutable object
        boxes.push_back(boxName);
    }*/
}

// `cryovial_store`: as above (dealt with already?)

void __fastcall TfrmRetrAsstCollectSamples::saveProgressThreadTerminated(TObject *Sender) {
    DEBUGSTREAM(__FUNC__<<"save plan for job "<<job->getID()<<" finished")
    hideProgressMessage();
	Screen->Cursor = crDefault; Enabled = true;

    if (!emptyBoxes.empty()) {
        if (IDYES != Application->MessageBox(L"There are empty boxes. Would you like to mark these as discarded?", L"Info", MB_YESNO)) {
            throw runtime_error("user did not want to deal with empty boxes");
            // then mark them REFERRED
        } else {
            collectEmpties();
        }
    }

    // were we exiting or just finished a chunk? does it matter?
    // yes, because you don't want to close the form if just finishing a chunk

    if (isJobComplete()) {
        if (IDYES != Application->MessageBox(L"Save job? Are all chunks completed?", L"Info", MB_YESNO)) return;
        closeJob();
        ModalResult = mrOk;
    } else {
        TfrmRetrievalAssistant::msgbox("There are unactioned samples in this retrieval job; will not be marked as finished"); // fixme
        ModalResult = mrCancel;
    }
}

bool TfrmRetrAsstCollectSamples::isJobComplete() { /** are all chunks complete? */
    for (auto &chunk : chunks) { if (Chunk< SampleRow >::Status::DONE != chunk->getStatus()) return false; }
    return true;
}

void TfrmRetrAsstCollectSamples::closeJob() {
    LQuery qp(Util::projectQuery(job->getProjectID(), false));
    LQuery qc(LIMSDatabase::getCentralDb());

    // all boxes must be finished if here (ie. all samples are finished) // is this necessary???
    qc.setSQL("SELECT * FROM c_box_retrieval WHERE retrieval_cid = :rtid");
    qc.setParam("rtid", job->getID());
    qc.open();
    while (!qc.eof()) {
        // `c_box_retrieval`: set `time_stamp`, `status=2` (collected)
        LCDbBoxRetrieval cbr(qc);
        cbr.setStatus(LCDbBoxRetrieval::COLLECTED); //???
        cbr.saveRecord(LIMSDatabase::getCentralDb()); // time_stamp set by default - should be in 2.7.2

        // `box_name`: (if record): update `time_stamp`, `box_capacity`, `status=2` (CONFIRMED)
        // `c_box_name`: (if record): update `time_stamp`, `box_capacity`, `status=2` (CONFIRMED)
    }

    // `c_retrieval_job`: update `finish_date`, `status` = 2
    job->setStatus(LCDbCryoJob::DONE);
    job->saveRecord(qc); // finish date is updated by this method
}

void TfrmRetrAsstCollectSamples::collectEmpties() {
/** if, at the end of processing a chunk, there are any source boxes which have become empty,
    the user may want to discard them instead of replacing them.
    if so, provide an option to discard these empty boxes, recording it in the database
    create tick list or switch list of boxes, empty/otherwise
        ask user to comfirm that empty boxes are in fact empty
        if error, create referred box (INVALID/EXTRA/MISSING CONTENT?) in `c_box_name` and/or `c_slot_allocation`
    does this duplicate findEmpties()?

    // `c_box_name` and `c_slot_allocation` fields together == `l_box_arrival`
    // calculate if there are any empty boxes

    * collect empties (all vials "accepted" or "not found") for discard
    * at the end of processing each chunk, if source boxes are now empty
    * unlikely for test tasks but rat tanks may throw old boxes away
    * all source boxes from a reorganisation task should end up empty
    * ask user to confirm that vessel/structure/slot is now empty
    * otherwise box should be referred */
    Application->MessageBox(L"Handle disposal of empty boxes", L"Info", MB_OK);

    LQuery qp(Util::projectQuery(job->getProjectID(), false)); // fixme project-specific
    LPDbBoxNames boxNames;
    boxNames.readFilled(qp); // reads CONFIRMED [2], ANALYSED [3]. boxes.readCurrent(qp) reads EMPTY [0], IN_USE [1]
    const LPDbBoxName * pBoxName;
    typedef std::vector< LPDbBoxName > VecBoxes;
    VecBoxes boxes;

    if (!emptyBoxes.empty()) {
        if (IDYES != Application->MessageBox(L"There are empty boxes. Would you like to mark these as discarded?", L"Info", MB_YESNO)) {
            throw runtime_error("user did not want to deal with empty boxes");
            // then mark them REFERRED
        }
    } else {
        return;
    }

    std::set< int >::const_iterator idIt;
    for (idIt = emptyBoxes.begin(); idIt != emptyBoxes.end(); idIt++) {
        int box_cid = *idIt;
        pBoxName = boxNames.findByID(box_cid);
        if (NULL == pBoxName) { // IN_TANK [4] records not read and findByID for those ids returns a null
            throw runtime_error("box not found"); //???
        } else {
            LPDbBoxName boxName = *(pBoxName); // make mutable object
            boxes.push_back(boxName);
        }
    }

    VecBoxes::const_iterator boxIt;
    //for (boxIt = boxes.begin(); boxIt != boxes.end(); boxIt++) {
        //LPDbBoxName box = *boxIt;
        // add to tick/switch list

        //boxName.setStatus(LPDbBoxName::Status::IN_TANK);
        //LQuery cq(LIMSDatabase::getCentralDb()); // for extra param to LPDbBoxName::saveRecord()
        //boxName.saveRecord(qp, cq);

    //}
    if (!(boxes.empty())) {
        frmCollectEmpties->ShowModal();
    }
}

// cruft
       /*
    vector<string>::const_iterator strIt;
    if (errors.size() > 0) {
        ostringstream out;
        for (strIt = errors.begin(); strIt != errors.end(); strIt++) { out<<*strIt<<endl; }
        Application->MessageBox(String(out.str().c_str()).c_str(), L"Error", MB_OK);
        return;
	} else {
        delete_referenced< vector <SampleRow * > >(combined);
        delete_referenced< vector< Chunk< SampleRow > * > >(chunks); // chunk objects, not contents of chunks
        Close();
    }*/

//    } catch (std::exception & e) {
//        TfrmRetrievalAssistant::msgbox(e.what());
//    }

            // seems to be repeating job of isJobComplete()
//			int status = sample->lcr_record->getStatus();
//            switch (status) {
//                case LCDbCryovialRetrieval::EXPECTED:
//                case LCDbCryovialRetrieval::DEFERRED:
//                    if (NULL != sample->backup) {
//                        switch (sample->backup->lcr_record->getStatus()) {
//                            case LCDbCryovialRetrieval::EXPECTED:
//                            case LCDbCryovialRetrieval::DEFERRED:
//                                collect->unactionedSamples = true;
//                        }
//                    } else {
//                        collect->unactionedSamples = true;
//                    }
//            }
            //storeSample(sample); // always 'store' even if ignored
/*
catch (std::exception & e) {
        msgbox(e.what());
    }

    if (collect->errors.size() != 0) {
        ostringstream oss;
        oss<<"There were errors:\n\n";
        for (auto &error: collect->errors) {
            oss<<error<<"\n";
        }
        msgbox(oss.str());
    } else if (collect->unactionedSamples) {
        msgbox("There are unactioned samples in this retrieval job; will not be marked as finished"); // fixme
        return;
    } else if (!collect->unactionedSamples) { // job finished
		jobFinished();
	} */

//void __fastcall TfrmRetrAsstCollectSamples::btnAlreadyRetrievedClick(TObject *Sender) { alreadyRetrieved(); }
//     Application->MessageBox(L"Vial has already been retrieved", L"Info", MB_OK);
//    Application->MessageBox(L"What to do?", L"Info", MB_OK);

//void __fastcall TfrmRetrAsstCollectSamples::btnWrongVialClick(TObject *Sender) { wrongVial(); }

//void __fastcall TfrmRetrAsstCollectSamples::btnFoundElsewhereClick(TObject *Sender) { foundElsewhere(); }



void __fastcall TfrmRetrAsstCollectSamples::sgVialsContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled) {
    // bad
}

