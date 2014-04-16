#include <vcl.h>
#pragma hdrstop
#include "RetrAsstPlanSamples.h"
#include "StoreUtil.h"
#include "LCDbRack.h"
#include "StoreDAO.h"
#include "TfrmConfirm.h"
#include "LCDbAuditTrail.h"
#include "LPDbCryovialStore.h"
#include "LPDbBoxes.h"
#include "LCDbRetrieval.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmSamples *frmSamples;

__fastcall TfrmSamples::TfrmSamples(TComponent* Owner) : TForm(Owner) {
    sgwChunks = new StringGridWrapper< Chunk< SampleRow > >(sgChunks, &chunks);
    sgwChunks->addCol("section",  "Section",            87);
    sgwChunks->addCol("start",    "Start",              70);
    sgwChunks->addCol("startbox", "Destination Box",    304);
    sgwChunks->addCol("startvial","Vial",               150);
    sgwChunks->addCol("end",      "End",                66);
    sgwChunks->addCol("endbox",   "Destination Box",    242);
    sgwChunks->addCol("endvial",  "Vial",               160);
    sgwChunks->addCol("size",     "Size",               87);
    sgwChunks->init();

    sgwVials = new StringGridWrapper<SampleRow>(sgVials, &combined);
    sgwVials->addCol("barcode",  "Barcode",          91,    SampleRow::sort_asc_barcode,    "barcode");
    sgwVials->addCol("site",     "Site",             120,   SampleRow::sort_asc_site,       "site name");
    sgwVials->addCol("vesspos",  "Pos",              28,    SampleRow::sort_asc_vesspos,    "vessel position");
    sgwVials->addCol("vessel",   "Vessel",           107,   SampleRow::sort_asc_vessel,     "vessel name");
    sgwVials->addCol("shelf",    "Shelf",            31,    SampleRow::sort_asc_shelf,      "shelf number");
    sgwVials->addCol("structpos","Pos",              27,    SampleRow::sort_asc_structpos,  "structure position");
    sgwVials->addCol("struct",   "Structure",        123,   SampleRow::sort_asc_structure,  "structure name");
    sgwVials->addCol("boxpos",   "Slot",             26,    SampleRow::sort_asc_slot,       "slot");
    sgwVials->addCol("srcbox",   "Source box",       257,   SampleRow::sort_asc_srcbox,     "source box name");
    sgwVials->addCol("srcpos",   "Pos",              31,    SampleRow::sort_asc_srcpos,     "source box position");
    sgwVials->addCol("destbox",  "Destination box",  267,   SampleRow::sort_asc_destbox,    "dest. box name");
    sgwVials->addCol("destpos",  "Pos",              25,    SampleRow::sort_asc_destpos,    "dest. box position");
    sgwVials->addCol("aliquot",  "Aliquot",          90,    SampleRow::sort_asc_aliquot,    "aliquot type");
    sgwVials->init();

    sgwDebug = new StringGridWrapper<SampleRow>(sgDebug, &combined);
    sgwDebug->addCol("rownum",   "Row",              21);
    sgwDebug->addCol("barcode",  "Barcode",          91);
    sgwDebug->addCol("sample",   "Sample ID",        91);
    sgwDebug->addCol("aliquot",  "Aliquot",          90);
    sgwDebug->addCol("srcbox",   "Source box",      257);
    sgwDebug->addCol("srcpos",   "Pos",              31);
    sgwDebug->addCol("boxpos",   "Slot",             26);
    sgwDebug->addCol("destbox",  "Destination box",  267);
    sgwDebug->addCol("destpos",  "Pos",              25);
    sgwDebug->init();
}

void TfrmSamples::debugLog(String s) {
    String tmp = Now().CurrentDateTime().DateTimeString() + ": " + s;
    memoDebug->Lines->Add(tmp); // could use varargs: http://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c
}

void __fastcall LoadVialsJobThread::debugLog() {
    frmSamples->debugLog(debugMessage.c_str());
}

void __fastcall SavePlanThread::debugLog() {
    frmSamples->debugLog(debugMessage.c_str());
}

void __fastcall TfrmSamples::FormCreate(TObject *Sender) {
    cbLog->Checked      = RETRASSTDEBUG;
    cbLog->Visible      = RETRASSTDEBUG;
    panelDebug->Visible = cbLog->Checked;
    box_size            = DEFAULT_BOX_SIZE;
    job                 = NULL;
    loadingMessage = "Loading samples, please wait...";
}

void __fastcall TfrmSamples::FormDestroy(TObject *Sender) {
    delete sgwChunks;
    delete sgwVials;
    delete sgwDebug;
}

void __fastcall TfrmSamples::FormShow(TObject *Sender) {
    Enabled = false;
    ostringstream oss; oss<<job->getName()<<" : "<<job->getDescription()<<" [id: "<<job->getID()<<"]";
    Caption = oss.str().c_str();
    labelPrimary->Caption   = Util::getAliquotDescription(job->getPrimaryAliquot()).c_str();
    labelSecondary->Caption = Util::getAliquotDescription(job->getSecondaryAliquot()).c_str();
    btnSave->Enabled = true;
    chunks.clear();
    sgwChunks->clear();
    sgwVials->clear();
    frmRetrievalAssistant->clearStorageCache();
    timerLoadVials->Enabled = true;
    editDestBoxSize->Text = box_size;
}

void __fastcall TfrmSamples::FormClose(TObject *Sender, TCloseAction &Action) {
    //delete_referenced< vector <SampleRow * > >(vials);
    //delete_referenced< vector <SampleRow * > >(combined);
    delete_referenced< vector <SampleRow * > >(primaries);
    delete_referenced< vector <SampleRow * > >(secondaries);
    delete_referenced< vector< Chunk< SampleRow > * > >(chunks); // chunk objects, not contents of chunks
}

void __fastcall TfrmSamples::FormResize(TObject *Sender) {
    sgwChunks->resize();
    sgwVials->resize();
}

void __fastcall TfrmSamples::btnCancelClick(TObject *Sender) {
    ModalResult = mrCancel;
}

void __fastcall TfrmSamples::cbLogClick(TObject *Sender) {
    panelDebug->Visible = cbLog->Checked;
    splitterDebug->Visible  = cbLog->Checked;
}

void __fastcall TfrmSamples::sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
    TColor background = clWindow;
    if (0 == ARow) {
        background = clBtnFace;
    } else {
        Chunk< SampleRow > * chunk = (Chunk< SampleRow > *)sgChunks->Objects[0][ARow];
        if (NULL == chunk) {
            background = clWindow; // for when loading, not RETRIEVAL_ASSISTANT_ERROR_COLOUR;
        } else {
            background = RETRIEVAL_ASSISTANT_PLANNED_COLOUR;
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

void __fastcall TfrmSamples::sgVialsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
    TColor background = clWindow;
    if (0 == ARow) {
        background = clBtnFace;
    } else {
        SampleRow * row = (SampleRow *)sgVials->Objects[0][ARow];
        if (NULL == row) {
            background = clWindow;//RETRIEVAL_ASSISTANT_ERROR_COLOUR???
        } else {
            background = RETRIEVAL_ASSISTANT_PLANNED_COLOUR;
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

void __fastcall TfrmSamples::sgChunksFixedCellClick(TObject *Sender, int ACol, int ARow) {
    ostringstream oss; oss << __FUNC__;
    oss<<sgwChunks->printColWidths()<<" clicked on col: "<<ACol<<".";
    debugLog(oss.str().c_str());
}

void __fastcall TfrmSamples::sgChunksClick(TObject *Sender) {
    Chunk< SampleRow > * chunk;
    int row = sgChunks->Row;
    if (0 == row) {
        return;
    } else {
        chunk = (Chunk< SampleRow > * )(sgChunks->Objects[0][row]);
        showChunk(chunk); // default is 1st
    }
}

void __fastcall TfrmSamples::sgVialsFixedCellClick(TObject *Sender, int ACol, int ARow) { // sort by column
    Enabled = false; //ostringstream oss; oss << __FUNC__; oss<<sgwVials->printColWidths()<<" sorting by col: "<<ACol<<"."; debugLog(oss.str().c_str());
    if (chunks.size() == 0) return; // fix bug where double-click on main screen leaks through to this form on show
    currentChunk()->sortToggle(ACol);
    showChunk();
    Enabled = true;
}

void __fastcall TfrmSamples::sgVialsClick(TObject *Sender) {
    SampleRow * sample  = (SampleRow *)sgVials->Objects[0][sgVials->Row];
    sample?debugLog(sample->str().c_str()):debugLog("NULL sample");
    sample->backup?debugLog(sample->backup->str().c_str()):debugLog("NULL backup");
}

void __fastcall TfrmSamples::timerLoadVialsTimer(TObject *Sender) {
    timerLoadVials->Enabled = false;
    loadRows(); // so that gui can be updated
}

void TfrmSamples::rejectList() { // could be abstracted
    job->setStatus(LCDbCryoJob::Status::REJECTED);
    job->saveRecord(LIMSDatabase::getCentralDb());
    ModalResult = mrCancel; //??? don't use modalresult
}

void __fastcall TfrmSamples::btnRejectClick(TObject *Sender) {
    if (IDYES == Application->MessageBox(L"Are you sure you want to reject this list?", L"Question", MB_YESNO)) {
        rejectList();
    }
}

void __fastcall TfrmSamples::sgVialsDblClick(TObject *Sender) {
    if (sgVials->Row <= 1)
        return; // header or silly chunk
    if (sgChunks->Row < sgChunks->RowCount-1) {
		TfrmRetrievalAssistant::msgbox("Only the last chunk can be split");
        return;
    }
    addChunk(sgVials->Row-1); // allowing for fixed header row
    showChunks();
    showChunk();
}

void __fastcall TfrmSamples::btnAddChunkClick(TObject *Sender) {
    int selectedChunkSize = comboSectionSize->Items->Strings[comboSectionSize->ItemIndex].ToIntDef(0);
    if (addChunk(selectedChunkSize)) {
        showChunks();
        showChunk();
    } else {
        TfrmRetrievalAssistant::msgbox("Chosen chunk size is too big for current list");
    }
}

bool TfrmSamples::addChunk(unsigned int offset) {
/** Add chunk starting at specified row [of the specified size?]
    offset: number of rows after beginning of previous chunk at which to cut off new chunk
    return: is there any space for more? */
    if (combined.size() == 0) return false; //throw "vials.size() == 0"; // not an error strictly; not by my program anyway!
    int numvials  = combined.size(); int numchunks = chunks.size();

    Chunk< SampleRow > * curchunk, * newchunk;
    if (chunks.size() == 0) { // first chunk, make default chunk from entire listrows
        newchunk = new Chunk< SampleRow >(sgwVials, chunks.size()+1, 0, combined.size()-1); // 0-indexed // size is calculated
    } else {
        if (offset <= 0 || offset > combined.size()) { // ok only for first chunk
            Application->MessageBox(L"Invalid chunk size", L"Info", MB_OK);
            return false;
        }
        curchunk = chunks[chunks.size()-1];
        int currentchunksize = curchunk->getSize(); // no chunks until first added
        if (curchunk->getStartAbs()+offset > combined.size()) { // current last chunk is too small to be split at this offset
            return false; // e.g. for auto-chunk to stop chunking
        }
        curchunk->setEndAbs(curchunk->getStartAbs()+offset-1); // row above start of new chunk - oldrowscheme
        newchunk = new Chunk< SampleRow >(sgwVials, chunks.size()+1, curchunk->getStartAbs()+offset, combined.size()-1);
    }
    chunks.push_back(newchunk);
    showChunk(newchunk);
    sgChunks->Row = sgChunks->RowCount-1;
    return true;
}

void TfrmSamples::showChunks() {
    if (0 == chunks.size()) { // must always have one chunk anyway
        if (RETRASSTDEBUG) {
            if (IDYES == Application->MessageBox(L"This list is empty. Do you want to reject it?", L"Question", MB_YESNO)) {
                rejectList();
            } // option not to reject for testing purposes
        } else {
            Application->MessageBox(L"This list is empty. It will now be rejected", L"Info", MB_OK); // don't want to enable save of empty list
            rejectList(); // don't want to risk data corruption on live system
        }
        return;
    } else {
        sgChunks->RowCount = chunks.size() + 1;
        sgChunks->FixedRows = 1; // "Fixed row count must be LESS than row count"
    }
    int row = 1;
    for (vector< Chunk< SampleRow > * >::const_iterator it = chunks.begin(); it != chunks.end(); it++, row++) {
        Chunk< SampleRow > * chunk = *it;
        sgChunks->Cells[sgwChunks->colNameToInt("section")]   [row] = chunk->getSection();
        sgChunks->Cells[sgwChunks->colNameToInt("start")]     [row] = chunk->getStartAbs()+1; // human-readable
        sgChunks->Cells[sgwChunks->colNameToInt("startbox")]  [row] = chunk->getStartBox().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("startvial")] [row] = chunk->getStartVial().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("end")]       [row] = chunk->getEndAbs()+1; // human-readable
        sgChunks->Cells[sgwChunks->colNameToInt("endbox")]    [row] = chunk->getEndBox().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("endvial")]   [row] = chunk->getEndVial().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("size")]      [row] = chunk->getSize();
        sgChunks->Objects[0][row] = (TObject *)chunk;
    }
    sgwVials->clearSelection();
}

Chunk< SampleRow > * TfrmSamples::currentChunk() {
    if (sgChunks->Row < 1) sgChunks->Row = 1; // force selection of 1st row
    Chunk< SampleRow > * chunk = (Chunk< SampleRow > *)sgChunks->Objects[0][sgChunks->Row];
    if (NULL == chunk) {// still null
        ostringstream oss; oss<<__FUNC__<<": Null chunk"; debugLog(oss.str().c_str());
        throw Exception("null chunk");
    }
    return chunk;
}

void TfrmSamples::showChunk(Chunk< SampleRow > * chunk) {
    Screen->Cursor = crSQLWait; Enabled = false;
    debugLog("showChunk");

    if (NULL == chunk) { chunk = currentChunk(); }

    if (chunk->getSize() <= 0) { //?? error surely
        sgwVials->clear();
    } else {
        sgVials->RowCount = chunk->getSize()+1;
        sgVials->FixedRows = 1;
    }
    for (int row=0; row < chunk->getSize(); row++) {
        SampleRow *         sampleRow = chunk->objectAtRel(row);
        LPDbCryovial *      vial    = sampleRow->cryo_record;
        LPDbCryovialStore * store   = sampleRow->store_record;
        int rw = row+1; // for stringgrid
        sgVials->Cells[sgwVials->colNameToInt("barcode")]  [rw] = sampleRow->cryovial_barcode.c_str();
        sgVials->Cells[sgwVials->colNameToInt("aliquot")]  [rw] = sampleRow->aliquotName().c_str();
        sgVials->Cells[sgwVials->colNameToInt("srcbox")]   [rw] = sampleRow->src_box_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("srcpos")]   [rw] = sampleRow->store_record->getPosition();
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

    if (RETRASSTDEBUG) { // sgDebug - all vials
        sgDebug->RowCount = combined.size()+1;
        sgDebug->FixedRows = 1;
        for (unsigned row=0; row < combined.size(); row++) {
            SampleRow *         sampleRow = combined[row];
            LPDbCryovial *      vial    = sampleRow->cryo_record;
            LPDbCryovialStore * store   = sampleRow->store_record;
            int rw = row+1; // for stringgrid
            sgDebug->Cells[sgwDebug->colNameToInt("rownum")]   [rw] = row;
            sgDebug->Cells[sgwDebug->colNameToInt("barcode")]  [rw] = sampleRow->cryovial_barcode.c_str();
            sgDebug->Cells[sgwDebug->colNameToInt("sample" )]  [rw] = sampleRow->cryo_record->getSampleID();
            sgDebug->Cells[sgwDebug->colNameToInt("aliquot")]  [rw] = sampleRow->aliquotName().c_str();
            sgDebug->Cells[sgwDebug->colNameToInt("srcbox")]   [rw] = sampleRow->src_box_name.c_str();
            sgDebug->Cells[sgwDebug->colNameToInt("srcpos")]   [rw] = sampleRow->store_record->getPosition();
            sgDebug->Cells[sgwDebug->colNameToInt("boxpos" )]  [rw] = sampleRow->box_pos;
            sgDebug->Cells[sgwDebug->colNameToInt("destbox")]  [rw] = sampleRow->dest_box_name.c_str();
            sgDebug->Cells[sgwDebug->colNameToInt("destpos")]  [rw] = sampleRow->dest_cryo_pos;
            sgDebug->Objects[0][rw] = (TObject *)sampleRow;
        }
    }
    showChunks(); // to refrest start/end boxes
    Screen->Cursor = crDefault; Enabled = true;
}

void __fastcall TfrmSamples::sgChunksSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value) {
    //fixme ostringstream oss; oss<<__FUNC__<<String(sgChunks->Cells[ACol][ARow].c_str()); debugLog(oss.str().c_str());
}

void __fastcall TfrmSamples::sgChunksGetEditText(TObject *Sender, int ACol, int ARow, UnicodeString &Value) {
    //fixme ostringstream oss; oss<<__FUNC__<<String(sgChunks->Cells[ACol][ARow].c_str()); //debugLog(oss.str().c_str());
}

void TfrmSamples::autoChunk() {
/** initialise box size with size of first box in list
    box_name.box_type_cid -> box_content.box_size_cid -> c_box_size.box_capacity */
    LQuery qd(Util::projectQuery(frmSamples->job->getProjectID(), true)); LPDbBoxNames boxes;
    int box_id = combined[0]->dest_box_id;//->getBoxID(); // ???look at base list, chunk might not have been created
    const LPDbBoxName * found = boxes.readRecord(LIMSDatabase::getProjectDb(), box_id);
    if (found == NULL)
        throw "box not found";
}

void __fastcall TfrmSamples::btnAddSortClick(TObject *Sender) {
    addSorter();
}

void __fastcall TfrmSamples::btnDelSortClick(TObject *Sender) {
    removeSorter();
}

void __fastcall TfrmSamples::btnApplySortClick(TObject *Sender) {
    applySort();
}

void TfrmSamples::addSorter() {
    ostringstream oss; oss << __FUNC__ << groupSort->ControlCount; debugLog(oss.str().c_str());
    TComboBox * combo = new TComboBox(this);
    combo->Parent = groupSort;      // new combo is last created, aligned to left. put in right order: take them all out, sort and put back in in reverse order?
    combo->Width = 170;             //
    combo->Align = alRight;         // bodge
    combo->Align = alLeft;          // now the new combo is last (rightmost) in the list, rather than first (leftmost)
    combo->Style = csDropDownList;  // csDropDownList
    for (int i=0; i<sgwVials->colCount(); i++) {
        combo->AddItem(sgwVials->cols[i].sortDescription().c_str(), (TObject *)&sgwVials->cols[i]);
    }
    combo->ItemIndex = 0;
}

void TfrmSamples::removeSorter() {
    TComponent * component = groupSort->Controls[groupSort->ControlCount-1];
    TComboBox * combo = dynamic_cast<TComboBox *>(component);
    if (combo != NULL) {
        debugLog("found a combo box, text:");
        debugLog(combo->Text);
        delete component; // not RemoveComponent(component); and not groupSort->RemoveComponent(component); // remove TComponent, not TComboBox
    } else {
        debugLog("not a combo box");
    }
}

void TfrmSamples::applySort() { // loop through sorters and apply each selected sort
    Chunk< SampleRow > * chunk = currentChunk();
    bool changed = false;
    for (int i=groupSort->ControlCount-1; i>=0; i--) { // controls are in creation order, ie. buttons first from design, last added combo is last
        TControl * control = groupSort->Controls[i];
        TComboBox * combo = dynamic_cast<TComboBox *>(control);
        if (combo != NULL) {
            if (-1 != combo->ItemIndex) {
                StringGridWrapper< SampleRow >::Col * col = (StringGridWrapper< SampleRow >::Col *)combo->Items->Objects[combo->ItemIndex];
                ostringstream ss; ss<<"sorting by: "<<col->sortDescription().c_str(); debugLog(ss.str().c_str());
                chunk->sort_asc(col->name);
                changed = true;
            }
        } else {
            debugLog("not a combo box"); //break;
        }
    }
    if (changed) showChunk();
}

void TfrmSamples::loadRows() {
    panelLoading->Caption = loadingMessage;
    panelLoading->Visible = true; // appearing in wrong place because called in OnShow, form not yet maximized
    panelLoading->Top = (sgVials->Height / 2) - (panelLoading->Height / 2);
    panelLoading->Left = (sgVials->Width / 2) - (panelLoading->Width / 2);
    progressBottom->Style = pbstMarquee; progressBottom->Visible = true;
    Screen->Cursor = crSQLWait; // disable mouse? //ShowCursor(false);
    Enabled = false;
    loadVialsJobThread = new LoadVialsJobThread();
    loadVialsJobThread->OnTerminate = &loadVialsJobThreadTerminated;
}

__fastcall LoadVialsJobThread::LoadVialsJobThread() : TThread(false) {
    FreeOnTerminate = true;
}

void __fastcall LoadVialsJobThread::updateStatus() { // can't use args for synced method, don't know why
    frmSamples->panelLoading->Caption = loadingMessage.c_str();
    frmSamples->panelLoading->Repaint();
}

void __fastcall LoadVialsJobThread::Execute() {
    try {
        load();
    } catch (Exception & e) {
        debugMessage = AnsiString(e.Message).c_str(); Synchronize((TThreadMethod)&debugLog);
    } catch (...) {
        debugMessage = "unknown error"; Synchronize((TThreadMethod)&debugLog);
    }
}

void LoadVialsJobThread::load() {
    //delete_referenced< vector<SampleRow * > >(frmSamples->vials);
    delete_referenced< vector<SampleRow * > >(frmSamples->combined);
    delete_referenced< vector<SampleRow * > >(frmSamples->primaries);
    delete_referenced< vector<SampleRow * > >(frmSamples->secondaries);

    ostringstream oss; oss<<frmSamples->loadingMessage<<" (preparing query)"; loadingMessage = oss.str().c_str();
    debugMessage = "preparing query"; Synchronize((TThreadMethod)&debugLog);
    loadingMessage = frmSamples->loadingMessage;
    job = frmSamples->job;
    const int primary_aliquot     = job->getPrimaryAliquot();
    const int secondary_aliquot   = job->getSecondaryAliquot();

    LQuery qd(Util::projectQuery(job->getProjectID(), true)); // ddb

    oss.str(""); oss << // quick check to avoid wasting time
        "SELECT COUNT(*) FROM cryovial_store s1, cryovial_store s2"
        " WHERE s1.cryovial_id = s2.cryovial_id AND s2.status = 0 AND s1.retrieval_cid = :jobID";
    qd.setSQL(oss.str());
    qd.setParam("jobID", job->getID());
    qd.open();
    rowCount = qd.readInt(0);
    if (0 == rowCount) return;
    oss.str(""); oss << // actual query now we know there are some rows
        "SELECT"
		"  s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.cryovial_position," // for LPDbCryovialStore
        "  s1.record_id, c.sample_id, c.aliquot_type_cid, " // for LPDbCryovial
        "  c.cryovial_barcode,"
        "  b1.box_cid as source_id,"
        "  b1.external_name as source_name,"
		"  s1.cryovial_position as source_pos,"
        "  s2.box_cid as dest_id,"
        "  b2.external_name as dest_name,"
        "  s2.cryovial_position as dest_pos"
        " FROM"
        "  cryovial c, cryovial_store s1, box_name b1,"
        "  cryovial_store s2, box_name b2"
        " WHERE"
        "  c.cryovial_id = s1.cryovial_id AND"
        "  b1.box_cid = s1.box_cid AND"
        "  s1.cryovial_id = s2.cryovial_id AND"
        "  s2.status = 0 AND"
        //"  b1.status != 99 AND b2.status != 99 AND"
        "  b2.box_cid = s2.box_cid AND" //"  aliquot_type_cid = :aliquotID AND"
        "  s1.retrieval_cid = :jobID"
        " ORDER BY"
        "  cryovial_barcode, aliquot_type_cid "
        << (primary_aliquot < secondary_aliquot ? "ASC" : "DESC");

    qd.setSQL(oss.str()); debugMessage = qd.getSQL(); Synchronize((TThreadMethod)&debugLog);
    qd.setParam("jobID", job->getID());
    qd.open(); debugMessage = "query open"; Synchronize((TThreadMethod)&debugLog);
    rowCount = 0; SampleRow * previous = NULL;
    while (!qd.eof()) {
        if (0 == rowCount % 10) {
            ostringstream oss; oss<<"Found "<<rowCount<<" vials";
            loadingMessage = oss.str().c_str();
            Synchronize((TThreadMethod)&updateStatus);
        }
        SampleRow * row = new SampleRow(
            new LPDbCryovial(qd),
            new LPDbCryovialStore(qd),
            NULL,
                //new LCDbCryovialRetrieval(qd) would be needed for SampleRow::sort_asc_aliquot(...) { return a->retrieval_record->getAliType() ... }
                // but no retrieval plan has been created at this point
                // can use row->cryo_record->getAliquotType()
            qd.readString(  "cryovial_barcode"),
            qd.readString(  "source_name"),
            qd.readInt(     "dest_id"),
            qd.readString(  "dest_name"),
            qd.readInt(     "dest_pos"),
            "", 0, "", 0, 0, "", 0 ); // no storage details yet

        const int aliquotType = row->cryo_record->getAliquotType();
        if (aliquotType == primary_aliquot) {
            frmSamples->primaries.push_back(row);
        } else if (aliquotType == secondary_aliquot) {
            frmSamples->secondaries.push_back(row);
        } else {
            throw "unknown aliquot type for this job";
        }
        qd.next();
        rowCount++;
    }
    debugMessage = "finished retrieving rows, getting storage details"; Synchronize((TThreadMethod)&debugLog);

    combineAliquots(frmSamples->primaries, frmSamples->secondaries, frmSamples->combined);

    // find locations of source boxes
    map<int, const SampleRow *> samples; ROSETTA result; StoreDAO dao; int rowCount2 = 0;
	for (vector<SampleRow *>::iterator it = frmSamples->combined.begin(); it != frmSamples->combined.end(); ++it, rowCount2++) {
        SampleRow * sample = *it;
        ostringstream oss; oss<<"Finding storage for "<<sample->cryovial_barcode<<" ["<<rowCount2<<"/"<<rowCount<<"]: ";
        map<int, const SampleRow *>::iterator found = samples.find(sample->store_record->getBoxID());
        if (found != samples.end()) { // fill in box location from cache map
            sample->copyLocation(*(found->second));
            oss<<sample->storage_str(); oss<<" (cached)";
        } else {
            if (dao.findBox(sample->store_record->getBoxID(), LCDbProjects::getCurrentID(), result)) {
                sample->copyLocation(result);
            } else {
                sample->setLocation("not found", 0, "not found", 0, 0, "not found", 0); //oss<<"(not found)";
            }
            samples[sample->store_record->getBoxID()] = (*it); // cache result
            oss<<sample->storage_str(); oss<<"         ";
        }
        loadingMessage = oss.str().c_str(); Synchronize((TThreadMethod)&updateStatus);
	}
    debugMessage = "finished getting storage details"; Synchronize((TThreadMethod)&debugLog);
}

void LoadVialsJobThread::combineAliquots(const vecpSampleRow & primaries, const vecpSampleRow & secondaries, vecpSampleRow & combined) {

    struct PosKey {
        PosKey(int b, int p) : box(b), pos(p) { }
        PosKey(SampleRow * s) : box(s->dest_cryo_pos), pos(s->dest_box_id) { }
        int box, pos; //SampleRow * row1, * row2;
        bool operator <(const PosKey &other) const {
            if (box < other.box) { //if (row1->S);
                return true;
            } else if (box == other.box) {
                return pos < other.pos;
            } else {
                return false;
            }
        }
    };

    typedef std::map< PosKey, SampleRow * > posCache;
    posCache cache;

    combined.clear();

    for (auto &it : primaries) {
    //for (vecpSampleRow::const_iterator it = primaries.begin(); it != primaries.end(); it++) {
        //PosKey key(it->dest_cryo_pos, it->retrieval_record->getRJBId());
        PosKey key(it);
        cache[key] = it; // cache combination of dest box and pos
        combined.push_back(it);
//        SampleRow * row = *it;
//        PosKey key(row);
//        cache[key] = row; // cache combination of dest box and pos
//        combined.push_back(row);
    }

    for (auto &it : secondaries) {
        //PosKey key(it->dest_cryo_pos, it->retrieval_record->getRJBId());
        PosKey key(it);
        posCache::iterator found = cache.find(key);
        if (found != cache.end()) { // destination box and position already used (by primary)
            if (NULL == it)
                throw "null in cache";
            found->second->backup = it; // add as backup to primary
        } else {
            combined.push_back(it);     // add to list in its own right
        }
    }
}

void __fastcall TfrmSamples::loadVialsJobThreadTerminated(TObject *Sender) {
    //combineAliquots(primaries, secondaries, combined);
    progressBottom->Style = pbstNormal; progressBottom->Visible = false;
    panelLoading->Visible = false;
    Screen->Cursor = crDefault;
    Enabled = true;
    chunks.clear();
    sgwChunks->clear();
    LQuery qd(Util::projectQuery(frmSamples->job->getProjectID(), true)); LPDbBoxNames boxes;
    if (0 == combined.size()) {
        Application->MessageBox(L"No samples found, exiting", L"Info", MB_OK); Close();
        return;
    }
    int box_id = combined[0]->dest_box_id; // look at base list, chunk might not have been created
    const LPDbBoxName * found = boxes.readRecord(LIMSDatabase::getProjectDb(), box_id);
    if (found == NULL) {
        throw "box not found"; //Application->MessageBox(L"Box not found, exiting", L"Info", MB_OK); Close(); return;
    }
    box_size = found->getSize();
    editDestBoxSize->Text = box_size;
    addChunk(0); // default chunk
    showChunks();
    showChunk();
    Application->MessageBox(L"Use the 'Auto-Chunk' controls to automatically divide this list, or double click on a row to manually create chunks", L"Info", MB_OK);
    Enabled = true;
}

void __fastcall TfrmSamples::btnDelChunkClick(TObject *Sender) {
    ostringstream oss; oss << __FUNC__;
    if (chunks.size() > 1 && (RETRASSTDEBUG || IDYES == Application->MessageBox(L"Are you sure you want to delete the last chunk?", L"Question", MB_YESNO))) {
        oss<<" before delete: "<<chunks.size();
        delete chunks.back();
        oss<<" before pop: "<<chunks.size();
        chunks.pop_back();
        oss<<" after pop: "<<chunks.size();
        debugLog(oss.str().c_str());
        (*(chunks.end()-1))->setEndAbs(combined.size()-1); // oldrowscheme
        showChunks();
        showChunk();
    }
}

void __fastcall TfrmSamples::editDestBoxSizeChange(TObject *Sender) {
    timerCalculate->Enabled = true;
}

void __fastcall TfrmSamples::btnAddAllChunksClick(TObject *Sender) {
    Screen->Cursor = crSQLWait; Enabled = false;
    int selectedChunkSize = comboSectionSize->Items->Strings[comboSectionSize->ItemIndex].ToIntDef(0);
    float result = float(frmSamples->combined.size()) / float(selectedChunkSize);
    int numChunks = ceil(result);
    for (int i=0; i < numChunks; i++) {
        showChunks();
        if (!addChunk(selectedChunkSize))
            break;
    }
    showChunks();
    showChunk();
    Screen->Cursor = crDefault; Enabled = true;
}

void __fastcall TfrmSamples::timerCalculateTimer(TObject *Sender) {
    timerCalculate->Enabled = false;
    box_size = editDestBoxSize->Text.ToIntDef(0);
    calcSizes();
}

void TfrmSamples::calcSizes() {
    /** calculate possible chunk (section) sizes
    slot/box (where c_box_size.box_size_cid = c_box_content.box_size_cid) (where does box_content come from?)
    As retrieval lists will always specify destination boxes, chunk size can be based on the number of cryovials allocated to each box */
    comboSectionSize->Clear();
    int possibleChunkSize = box_size; // smallest chunk
    while (possibleChunkSize <= editMaxSize->Text.ToIntDef(0)) {
        comboSectionSize->Items->Add(String(possibleChunkSize));
        possibleChunkSize += box_size;
    }
    comboSectionSize->ItemIndex = comboSectionSize->Items->Count-1;
}

void __fastcall TfrmSamples::btnSaveClick(TObject *Sender) {
    for (unsigned   int i=0; i<chunks.size(); i++) { // check chunk sizes?
        if (chunks[i]->getSize() > MAX_CHUNK_SIZE) {
            wstringstream oss; oss<<"Maximum chunk size is "<<MAX_CHUNK_SIZE; Application->MessageBox(oss.str().c_str(), L"Error", MB_OK); return;
        }
    }
    frmConfirm->initialise(LCDbCryoJob::Status::DONE, "Confirm retrieval plan");  // , projects); don't need project ids??? //status??? //std::set<int> projects; projects.insert(job->getProjectID());
    if (IDYES == Application->MessageBox(L"Save changes? Press 'No' to go back and re-order", L"Question", MB_YESNO)
            && (RETRASSTDEBUG || mrOk == frmConfirm->ShowModal())) {
        Screen->Cursor = crSQLWait;
        Enabled = false;
        debugLog("starting save plan");
        panelLoading->Caption = loadingMessage;
        panelLoading->Visible = true; // appearing in wrong place because called in OnShow, form not yet maximized
        panelLoading->Top = (sgVials->Height / 2) - (panelLoading->Height / 2);
        panelLoading->Left = (sgVials->Width / 2) - (panelLoading->Width / 2);
        progressBottom->Style = pbstMarquee; progressBottom->Visible = true;

        savePlanThread = new SavePlanThread();
        savePlanThread->OnTerminate = &savePlanThreadTerminated;
    } else { // start again
        chunks.clear();
        addChunk(0);
        showChunks();
        showChunk();
    }
}

__fastcall SavePlanThread::SavePlanThread() : TThread(false) {
    FreeOnTerminate = true;
}

void __fastcall SavePlanThread::updateStatus() { // can't use args for synced method, don't know why
    frmSamples->panelLoading->Caption = loadingMessage.c_str();
    frmSamples->panelLoading->Repaint();
}

void __fastcall SavePlanThread::Execute() {
/** Insert an entry into c_box_retrieval for each destination box, recording the chunk it is in,
and a record into l_cryovial_retrieval for each cryovial, recording its position in the list. */
    try {
        save();
        frmSamples->job->setStatus(LCDbCryoJob::INPROGRESS);
        frmSamples->job->saveRecord(LIMSDatabase::getCentralDb());
        frmSamples->ModalResult = mrOk; // save and close here rather than OnTerminate in case of exception
    } catch (Exception & e) {
        debugMessage = AnsiString(e.Message).c_str(); Synchronize((TThreadMethod)&debugLog);
    } catch (...) {
        debugMessage = "unknown error"; Synchronize((TThreadMethod)&debugLog);
    }
}

void SavePlanThread::save() {
/** Insert an entry into c_box_retrieval for each destination box, recording the chunk it is in,
and a record into l_cryovial_retrieval for each cryovial, recording its position in the list. */
    struct Saver { // encapsulate in order to re-use for secondary aliquot
        LCDbCryoJob * job;
        LQuery & qc;
        int pid;
        int pos = 0; // position in chunk
        Saver(LCDbCryoJob * _job, LQuery & _qc, int _pid) : job(_job), qc(_qc), pid(_pid) {} //, chunk(chunk), sampleRow(sampleRow) { }
        int saveBox(Chunk< SampleRow > * chunk, map<int, int> & boxes, int dest_box_id) {
            int rj_box_cid;
            map<int, int>::iterator found = boxes.find(dest_box_id);
            if (found == boxes.end()) { // not added yet, add record and cache
                LCDbBoxRetrieval box( //rj_box_cid created by DAO
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
            return rj_box_cid;
        }
        void saveSample(Chunk< SampleRow > * chunk, SampleRow * sampleRow, int rj_box_cid) {
            LCDbCryovialRetrieval vial(
                rj_box_cid,
                ++pos,
                // wrong! this should be the position in the plan and auto-incremented (in the DAO?) sampleRow->dest_cryo_pos,
                sampleRow->cryo_record->getBarcode(),
                sampleRow->cryo_record->getAliquotType(),
                sampleRow->store_record->getBoxID(),    //??? old_box_cid
                sampleRow->store_record->getPosition(), //??? old_position
                sampleRow->dest_cryo_pos,               //??? new_position
                pid,
                LCDbCryovialRetrieval::Status::EXPECTED
            );
            vial.saveRecord(qc);
        }
    };

    const int pid = LCDbAuditTrail::getCurrent().getProcessID();

    LQuery qc(LIMSDatabase::getCentralDb());
    //Saver s(frmSamples->job, qc, pid);
    //for (vector< Chunk< SampleRow > * >::const_iterator it = frmSamples->chunks.begin(); it != frmSamples->chunks.end(); it++) {
    for (auto &it : frmSamples->chunks) {
        map<int, int> boxes; // box_id to rj_box_id, per chunk
        int rj_box_cid;
        //Chunk< SampleRow > * chunk = *it;
        Chunk< SampleRow > * chunk = it;
        Saver s(frmSamples->job, qc, pid); // to start lcr.position at 1 for each chunk
        for (int i = 0; i < chunk->getSize(); i++) {
            SampleRow * sampleRow = chunk->objectAtRel(i);
            rj_box_cid = s.saveBox(chunk, boxes, sampleRow->dest_box_id); // crash here
            s.saveSample(chunk, sampleRow, rj_box_cid);
            ostringstream oss; oss<<"Saving chunk "<<chunk->getSection()
            <<" - Primary: '"<<sampleRow->cryovial_barcode<<"'";
            if (NULL != sampleRow->backup) {
                rj_box_cid = s.saveBox(chunk, boxes, sampleRow->backup->dest_box_id);
                s.saveSample(chunk, sampleRow->backup, rj_box_cid);
                oss<<", backup: '"<<sampleRow->backup->cryovial_barcode<<"'";
            }
            oss <<" ["<<i<<"/"<<chunk->getSize()<<"]";
            loadingMessage = oss.str().c_str(); Synchronize((TThreadMethod)&updateStatus);
        }
    }
}

void __fastcall TfrmSamples::savePlanThreadTerminated(TObject *Sender) {
    debugLog("finished save plan");
    Screen->Cursor = crDefault;
    btnSave->Enabled = false;
    Enabled = true;
}

//                { // must go out of scope otherwise read locks db with "no mst..."
//                    LQuery qt(LIMSDatabase::getCentralDb()); LCDbID myLCDbID;
//                    rj_box_cid = myLCDbID.claimNextID(qt); // SQL: "next value for c_id_sequence"
//                } //int rtid = job->getID(); int sect = chunk->getSection(); int bid  = sampleRow->store_record->getBoxID(); // debug
                //should use saveRecord
//    frmAutoChunk->setBoxSize(found->getSize());
//    frmAutoChunk->Visible = false; // http://www.delphipages.com/forum/showthread.php?t=69616
//    frmAutoChunk->ShowModal();

//            qc.setSQL(
//                "INSERT INTO l_cryovial_retrieval (rj_box_cid, position, cryovial_barcode, aliquot_type_cid, slot_number, process_cid, time_stamp, status)"
//                " VALUES (:rjid, :pos, :barc, :aliq, :slot, :pid, 'now', :st)"
//            );
//            qc.setParam("rjid", rj_box_cid);
//            qc.setParam("pos",  sampleRow->dest_cryo_pos); //?? //qc.setParam("pos",  sampleRow->store_record->getPosition()); //??
//            qc.setParam("barc", sampleRow->cryo_record->getBarcode()); //??
//            qc.setParam("aliq", sampleRow->cryo_record->getAliquotType());
//            qc.setParam("slot", sampleRow->box_pos); //??? // rename box_pos to dest_pos?
//            qc.setParam("pid",  pid);
//            qc.setParam("st",   LCDbCryovialRetrieval::Status::EXPECTED); //??
//            qc.execSQL();

//	LIMSParams & params = LIMSParams::instance();
//	if( params.openSection( "sorters", true ) )
//		params.setValue( ",,,", categoryID );
//	params.saveSize( this );

//    	regKey = new TRegistry;
//	openSection( "", false );
//	buddyID = findValue( "Buddy ID", 0 );
//    LIMSParams::openSection();
