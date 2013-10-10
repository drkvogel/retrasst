#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistantSamples.h"
#include "StoreUtil.h"
#include "LCDbRack.h"
#include "RetrievalAssistantAutoChunk.h"
#include "StoreDAO.h"
#include "TfrmConfirm.h"
#include "LCDbAuditTrail.h"
#include "LPDbCryovialStore.h"
#include "LPDbBoxes.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmSamples *frmSamples;

__fastcall TfrmSamples::TfrmSamples(TComponent* Owner) : TForm(Owner) {
    sgwChunks = new StringGridWrapper< Chunk< SampleRow > >(sgChunks, &chunks);
    // {87, 70, 304, 150, 66, 242, 160, 87, }
    sgwChunks->addCol("section",  "Section",  87);
    sgwChunks->addCol("start",    "Start",    70);
    sgwChunks->addCol("startbox", "Box",      304);
    sgwChunks->addCol("startvial","Vial",     150);
    sgwChunks->addCol("end",      "End",      66);
    sgwChunks->addCol("endbox",   "Box",      242);
    sgwChunks->addCol("endvial",  "Vial",     160);
    sgwChunks->addCol("size",     "Size",     87);
    sgwChunks->init();

    //{91, 90, 257, 31, 120, 28, 107, 31, 27, 123, 26, 267, 25, }
    sgwVials = new StringGridWrapper<SampleRow>(sgVials, &vials);
    sgwVials->addCol("barcode",  "Barcode",          91,    SampleRow::sort_asc_barcode);
    sgwVials->addCol("aliquot",  "Aliquot",          90,    SampleRow::sort_asc_aliquot);
    sgwVials->addCol("currbox",  "Current box",      257,   SampleRow::sort_asc_currbox);
    sgwVials->addCol("currpos",  "Pos",              31,    SampleRow::sort_asc_currpos);
    sgwVials->addCol("site",     "Site",             120,   SampleRow::sort_asc_site);
    sgwVials->addCol("vesspos",  "Pos",              28,    SampleRow::sort_asc_vesspos);
    sgwVials->addCol("vessel",   "Vessel",           107,   SampleRow::sort_asc_vessel);
    sgwVials->addCol("shelf",    "Shelf",            31,    SampleRow::sort_asc_shelf);
    sgwVials->addCol("structpos","Pos",              27,    SampleRow::sort_asc_structpos);
    sgwVials->addCol("struct",   "Structure",        123,   SampleRow::sort_asc_structure);
    sgwVials->addCol("boxpos",   "Slot",             26,    SampleRow::sort_asc_slot);
    sgwVials->addCol("destbox",  "Destination box",  267,   SampleRow::sort_asc_destbox);
    sgwVials->addCol("destpos",  "Pos",              25,    SampleRow::sort_asc_destpos);
    sgwVials->init();
}

void TfrmSamples::debugLog(String s) {
    frmSamples->memoDebug->Lines->Add(s); // could use varargs: http://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c
}

void __fastcall TfrmSamples::FormCreate(TObject *Sender) {
    cbLog->Checked      = RETRASSTDEBUG;
    cbLog->Visible      = RETRASSTDEBUG;
    memoDebug->Visible  = cbLog->Checked;
    job                 = NULL;
    loadingMessage = "Loading samples, please wait...";
}

void __fastcall TfrmSamples::FormDestroy(TObject *Sender) {
    delete sgwChunks;
    delete sgwVials;
}

void __fastcall TfrmSamples::FormShow(TObject *Sender) {
    ostringstream oss; oss<<job->getName()<<" : "<<job->getDescription();
    Caption = oss.str().c_str();
    btnSave->Enabled = true;
    chunks.clear();
    sgwChunks->clear();
    sgwVials->clear();
    timerLoadVials->Enabled = true;

//	LIMSParams & params = LIMSParams::instance();
//	if( params.openSection( "sorters", true ) )
//		params.setValue( ",,,", categoryID );
//	params.saveSize( this );

//    	regKey = new TRegistry;
//	openSection( "", false );
//	buddyID = findValue( "Buddy ID", 0 );
//    LIMSParams::openSection();

}

void __fastcall TfrmSamples::FormClose(TObject *Sender, TCloseAction &Action) {
    delete_referenced< vector <SampleRow * > >(frmSamples->vials);
    delete_referenced< vector< Chunk< SampleRow > * > >(chunks); // chunk objects, not contents of chunks
}

void __fastcall TfrmSamples::btnCancelClick(TObject *Sender) {
    ModalResult = mrCancel;
}

void __fastcall TfrmSamples::cbLogClick(TObject *Sender) {
    memoDebug->Visible = cbLog->Checked;
    splitterDebug->Visible  = cbLog->Checked;
}

void __fastcall TfrmSamples::sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
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

void __fastcall TfrmSamples::sgVialsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
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

void __fastcall TfrmSamples::sgChunksFixedCellClick(TObject *Sender, int ACol, int ARow) {
    ostringstream oss; oss << __FUNC__;
    oss<<sgwChunks->printColWidths()<<" clicked on col: "<<ACol<<".";
    debugLog(oss.str().c_str());
}

/* delete from l_cryovial_retrieval; delete from c_box_retrieval -- as user lust */

void __fastcall TfrmSamples::btnSaveClick(TObject *Sender) {
    /** Insert an entry into c_box_retrieval for each destination box, recording the chunk it is in,
    and a record into l_cryovial_retrieval for each cryovial, recording its position in the list. */
    if (IDYES == Application->MessageBox(L"Save changes? Press 'No' to go back and re-order", L"Question", MB_YESNO)) {

        std::set<int> projects; projects.insert(job->getProjectID());
        frmConfirm->initialise(LCDbCryoJob::Status::DONE, "Confirm retrieval plan", projects);  //status???
        //if (mrOk != frmConfirm->ShowModal()) return;
        Screen->Cursor = crSQLWait; Enabled = false;
        LQuery qc(LIMSDatabase::getCentralDb());
        map<int, int> boxes; // box_id to rj_box_id
        int rj_box_cid;
        for (vector< Chunk< SampleRow > * >::const_iterator it = chunks.begin(); it != chunks.end(); it++) {
            Chunk< SampleRow > * chunk = *it;
            for (int i = 1; i < chunk->getSize(); i++) {
                SampleRow *         sampleRow = chunk->rowAt(i);
                LPDbCryovial *      cryo  = sampleRow->cryo_record;
                LPDbCryovialStore * store = sampleRow->store_record;
                map<int, int>::iterator found = boxes.find(sampleRow->store_record->getBoxID());
                if (found == boxes.end()) { // not added yet, add record and cache
                    { // must go out of scope otherwise read locks db with "no mst..."
                        LQuery qt(LIMSDatabase::getCentralDb()); LCDbID myLCDbID;
                        rj_box_cid = myLCDbID.claimNextID(qt); // SQL: "next value for c_id_sequence"
                    }
                    int rtid = job->getID();
                    int sect = chunk->getSection();
                    int bid  = sampleRow->store_record->getBoxID();
                    int stat = LCDbBoxStore::Status::SLOT_ALLOCATED;
                    qc.setSQL(
                        "INSERT INTO c_box_retrieval (retrieval_cid, section, box_id, rj_box_cid, status)"
                        " VALUES (:rtid, :sect, :bid, :rjbid, :stat)"
                    );
                    qc.setParam("rtid", job->getID());
                    qc.setParam("rjbid",rj_box_cid); // Unique ID for this retrieval list entry (also determines retrieval order for box retrievals)
                    qc.setParam("sect", chunk->getSection()); // 0 = retrieve all boxes in parallel
                    qc.setParam("bid",  sampleRow->store_record->getBoxID()); // The box being retrieved (for box retrieval/disposal) or retrieved into (for sample retrieval/disposal)
                    qc.setParam("stat", LCDbBoxStore::Status::SLOT_ALLOCATED); // 0: new record; 1: part-filled, 2: collected; 3: not found; 99: record deleted
                    qc.execSQL();
                    boxes[sampleRow->store_record->getBoxID()] = rj_box_cid; // cache result
                } else {
                    rj_box_cid = found->second;
                }
                qc.setSQL(
                    "INSERT INTO l_cryovial_retrieval"
                    " (rj_box_cid, position, cryovial_barcode, aliquot_type_cid, process_cid, time_stamp, status)"
                    " VALUES"
                    " (:rjid, :pos, :barc, :aliq, :pid, 'now', :st)"
                );
                qc.setParam("rjid", rj_box_cid);
                qc.setParam("pos",  sampleRow->store_record->getPosition()); //??
                qc.setParam("barc", sampleRow->cryo_record->getBarcode()); //??
                qc.setParam("aliq", sampleRow->cryo_record->getAliquotType());
                const int pid = LCDbAuditTrail::getCurrent().getProcessID();
                qc.setParam("pid",  pid);
                qc.setParam("st",   LPDbCryovialStore::Status::ALLOCATED); //??
                qc.execSQL();
            }
        }
        btnSave->Enabled = false;
        job->setStatus(LCDbCryoJob::INPROGRESS);
        job->saveRecord(LIMSDatabase::getCentralDb());
        Screen->Cursor = crDefault; Enabled = true;
        ModalResult = mrOk;
    } else { // start again
        chunks.clear();
        addChunk(0); // start again
    }
}

void __fastcall TfrmSamples::sgChunksClick(TObject *Sender) {
    showChunk(); // default is 1st
}

void __fastcall TfrmSamples::btnAutoChunkClick(TObject *Sender) {
    autoChunk();
}

void __fastcall TfrmSamples::btnIncrClick(TObject *Sender) {
    // increase end of current chunk
}

void __fastcall TfrmSamples::btnDecrClick(TObject *Sender) {
    // decrease end of current chunk
}

void __fastcall TfrmSamples::sgVialsFixedCellClick(TObject *Sender, int ACol, int ARow) { // sort by column
    ostringstream oss; oss << __FUNC__;
    oss<<sgwVials->printColWidths()<<" sorting by col: "<<ACol<<".";
    debugLog(oss.str().c_str());
    currentChunk()->sortToggle(ACol);
    showChunk();
}

void __fastcall TfrmSamples::sgVialsClick(TObject *Sender) {
    SampleRow * sample  = (SampleRow *)sgVials->Objects[0][sgVials->Row];
    sample?debugLog(sample->str().c_str()):debugLog("NULL sample");
}

void __fastcall TfrmSamples::timerLoadVialsTimer(TObject *Sender) {
    timerLoadVials->Enabled = false;
    loadRows(); // so that gui can be updated
}

void __fastcall TfrmSamples::btnRejectClick(TObject *Sender) {
    if (IDYES == Application->MessageBox(L"Are you sure you want to reject this list?", L"Question", MB_YESNO)) {
        job->setStatus(LCDbCryoJob::Status::REJECTED);
        job->saveRecord(LIMSDatabase::getCentralDb());
        ModalResult = mrCancel; //??? don't use modalresult
    }
}

//-----chunks-------

void __fastcall TfrmSamples::sgVialsDblClick(TObject *Sender) {
    // mark chunk boundary //msgbox("chunk split");
    addChunk(sgVials->Row); //??
    showChunks();
}

void __fastcall TfrmSamples::btnAddChunkClick(TObject *Sender) {
    if (sgVials->Row < 2) return;
    addChunk(sgVials->Row);
}

void __fastcall TfrmSamples::btnDelChunkClick(TObject *Sender) {
    ostringstream oss; oss << __FUNC__; oss<<chunks.size();
    if (RETRASSTDEBUG || IDYES == Application->MessageBox(L"Are you sure you want to delete the last chunk?", L"Question", MB_YESNO)) {
        oss<<" before delete: "<<chunks.size();
        delete chunks.back();
        oss<<" before pop: "<<chunks.size();
        chunks.pop_back();
        oss<<" after pop: "<<chunks.size();
        debugLog(oss.str().c_str());
        (*(chunks.end()))->setEnd(vials.size());
        showChunks();
    }
    if (chunks.size() == 1) btnDelChunk->Enabled = false;
}

bool TfrmSamples::addChunk(unsigned int offset) {//, unsigned int size) {
/** Add a chunk starting at the specified row [of the specified size?]
    offset: number of rows after beginning of previous chunk at which to cut off new chunk
    return: is there any space for more? */

    if (vials.size() == 0) return false; //throw "vials.size() == 0";
    if (offset > vials.size()) return false; //throw "startrow > vials.size()";

    Chunk< SampleRow > * chunk;
    int test5 = vials.size();
    int test1 = chunks.size() + 1;

    if (chunks.size() == 0) { // first chunk, make default chunk from entire listrows
        chunk = new Chunk< SampleRow >(sgwVials, chunks.size() + 1, 0, vials.size()-1); // 0-indexed // size is calculated
    } else {
        if (offset <= 0) return false; //??
        int test2 = currentChunk()->getSize()+1; //make end of previous chunk start - 1
        //currentChunk()->getStart()
        if (currentChunk()->getStart() + offset > vials.size()) { // current last chunk is too small to be split at this offset
            return false; // e.g. for auto-chunk to stop chunking
        }
        currentChunk()->setEnd(currentChunk()->getStart() + offset-1); // row above start of new chunk
        chunk = new Chunk< SampleRow >(sgwVials, chunks.size() + 1, currentChunk()->getStart()+offset, vials.size()-1);
    }
    chunks.push_back(chunk); btnDelChunk->Enabled = true;
    showChunks();
    sgChunks->Row = sgChunks->RowCount-1; // fixme make it the current chunk
    sgwVials->clearSelection();
    return true;
}

void TfrmSamples::showChunks() {
    if (0 == chunks.size()) { throw Exception("No chunks"); } // must always have one chunk anyway
    else { sgChunks->RowCount = chunks.size() + 1; sgChunks->FixedRows = 1; } // "Fixed row count must be LESS than row count"

    int row = 1;
    for (vector< Chunk< SampleRow > * >::const_iterator it = chunks.begin(); it != chunks.end(); it++, row++) {
        Chunk< SampleRow > * chunk = *it;
        sgChunks->Cells[sgwChunks->colNameToInt("section")]   [row] = chunk->getSection();
        sgChunks->Cells[sgwChunks->colNameToInt("start")]     [row] = chunk->getStartPos();
        sgChunks->Cells[sgwChunks->colNameToInt("startbox")]  [row] = chunk->getStartBox().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("startvial")] [row] = chunk->getStartVial().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("end")]       [row] = chunk->getEndPos();
        sgChunks->Cells[sgwChunks->colNameToInt("endbox")]    [row] = chunk->getEndBox().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("endvial")]   [row] = chunk->getEndVial().c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("size")]      [row] = chunk->getSize();
        sgChunks->Objects[0][row] = (TObject *)chunk;
    }
    showChunk();
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

    if (NULL == chunk) { chunk = currentChunk(); }
    if (chunk->getSize() <= 0) { sgwVials->clear(); }
    else { sgVials->RowCount = chunk->getSize(); sgVials->FixedRows = 1; }

    for (int row = 1; row < chunk->getSize(); row++) {
        SampleRow *         sampleRow = chunk->rowAt(row);
        LPDbCryovial *      vial    = sampleRow->cryo_record;
        LPDbCryovialStore * store   = sampleRow->store_record;
        sgVials->Cells[sgwVials->colNameToInt("barcode")]  [row] = sampleRow->cryovial_barcode.c_str();
        sgVials->Cells[sgwVials->colNameToInt("aliquot")]  [row] = sampleRow->aliquot_type_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("currbox")]  [row] = sampleRow->src_box_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("currpos")]  [row] = sampleRow->store_record->getPosition();
        sgVials->Cells[sgwVials->colNameToInt("site"   )]  [row] = sampleRow->site_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("vesspos")]  [row] = sampleRow->vessel_pos;
        sgVials->Cells[sgwVials->colNameToInt("vessel" )]  [row] = sampleRow->vessel_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("shelf"  )]  [row] = sampleRow->shelf_number;
        sgVials->Cells[sgwVials->colNameToInt("structpos")][row] = sampleRow->structure_pos;
        sgVials->Cells[sgwVials->colNameToInt("struct" )]  [row] = sampleRow->structure_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("boxpos" )]  [row] = sampleRow->box_pos;
        sgVials->Cells[sgwVials->colNameToInt("destbox")]  [row] = sampleRow->dest_box_name.c_str();
        sgVials->Cells[sgwVials->colNameToInt("destpos")]  [row] = sampleRow->dest_cryo_pos;
        sgVials->Objects[0][row] = (TObject *)sampleRow;
    }
    Screen->Cursor = crDefault; Enabled = true;
}

void __fastcall TfrmSamples::sgChunksSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value) {
    //ostringstream oss; oss<<__FUNC__<<String(sgChunks->Cells[ACol][ARow].c_str()); debugLog(oss.str().c_str());
}

void __fastcall TfrmSamples::sgChunksGetEditText(TObject *Sender, int ACol, int ARow, UnicodeString &Value) {
    //ostringstream oss; oss<<__FUNC__<<String(sgChunks->Cells[ACol][ARow].c_str());
    //debugLog(oss.str().c_str());
}

void TfrmSamples::autoChunk() {
/** initialise box size with size of first box in first chunk
    box_name.box_type_cid -> box_content.box_size_cid -> c_box_size.box_capacity */
    LQuery qd(Util::projectQuery(frmSamples->job->getProjectID(), true));
    LPDbBoxNames boxes;

    //int box_id = chunks[0]->getStartRow()->store_record->getBoxID();
    int box_id = vials[0]->store_record->getBoxID();
    const LPDbBoxName * found = boxes.readRecord(LIMSDatabase::getProjectDb(), box_id);
    if (found == NULL)
        throw "box not found";
    frmAutoChunk->setBoxSize(found->getSize());
    frmAutoChunk->ShowModal();
}

//-------------- sorters --------------

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
    combo->Parent = groupSort; // new combo is last created, aligned to left
        // put in right order: take them all out, sort and put back in in reverse order?
    combo->Width = 200;
    combo->Align = alLeft;
    combo->Style = csDropDown; // csDropDownList
    for (int i=0; i<sgwVials->colCount(); i++) {
        combo->AddItem(sgwVials->cols[i].sortDescription().c_str(), (TObject *)&sgwVials->cols[i]);
    }
    //combo->OnChange = &comboSortOnChange;
}

void TfrmSamples::removeSorter() {
    //for (int i=groupSort->ControlCount-1; i>=0; i--) { // work backwards through controls to find last combo box
    //for (int i=0; i<groupSort->ControlCount; i++) { // controls are in creation order, ie. buttons first from design, and last added combo is last
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
    ostringstream oss; oss<<__FUNC__<<groupSort->ControlCount<<" controls"<<endl; debugLog(oss.str().c_str());
    Chunk< SampleRow > * chunk = currentChunk();
    bool changed = false;
    for (int i=groupSort->ControlCount-1; i>=0; i--) { // work backwards through controls to find last combo box // controls are in creation order, ie. buttons first from design, and last added combo is last
        TControl * control = groupSort->Controls[i];
        TComboBox * combo = dynamic_cast<TComboBox *>(control);
        if (combo != NULL) {
            if (-1 != combo->ItemIndex) {
                changed = true;
                debugLog("sorting: ");
                debugLog(combo->Items->Strings[combo->ItemIndex].c_str());
                StringGridWrapper< SampleRow >::Col * col = (StringGridWrapper< SampleRow >::Col *)combo->Items->Objects[combo->ItemIndex];
                debugLog(col->sortDescription().c_str());
                chunk->sort_asc(col->name);
            }
        } else {
            debugLog("not a combo box, finish sorting."); break;
        }
    }
    if (changed) showChunk();
}

//-------------- samples --------------

void TfrmSamples::loadRows() {
    panelLoading->Caption = loadingMessage;
    panelLoading->Visible = true; // appearing in wrong place because called in OnShow, form not yet maximized
    panelLoading->Top = (sgVials->Height / 2) - (panelLoading->Height / 2);
    panelLoading->Left = (sgVials->Width / 2) - (panelLoading->Width / 2);
    progressBottom->Style = pbstMarquee; progressBottom->Visible = true;
    Screen->Cursor = crSQLWait; // disable mouse? //ShowCursor(false);
    Enabled = false;
    loadVialsWorkerThread = new LoadVialsWorkerThread();
    loadVialsWorkerThread->OnTerminate = &loadVialsWorkerThreadTerminated;
}

__fastcall LoadVialsWorkerThread::LoadVialsWorkerThread() : TThread(false) {
    FreeOnTerminate = true;
}

void __fastcall LoadVialsWorkerThread::updateStatus() { // can't use args for synced method, don't know why
    frmSamples->panelLoading->Caption = loadingMessage.c_str();
    frmSamples->panelLoading->Repaint();
}

void __fastcall LoadVialsWorkerThread::Execute() {
    delete_referenced< vector<SampleRow * > >(frmSamples->vials);
    ostringstream oss; oss<<frmSamples->loadingMessage<<" (preparing query)";
    loadingMessage = oss.str().c_str();

    LQuery qd(Util::projectQuery(frmSamples->job->getProjectID(), true)); // ddb
    qd.setSQL( // from spec 2013-09-11
        "SELECT"
        "  s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.tube_position," // for LPDbCryovialStore
        "  s1.record_id, c.sample_id, c.aliquot_type_cid, " // for LPDbCryovial
            // LPDbCryovial::storeID( query.readInt( "record_id" ) ) <-- record_id comes from cryovial_store?
        "  c.cryovial_barcode, t.external_name AS aliquot,"
        "  b1.box_cid as source_id,"
        "  b1.external_name as source_name,"
        "  s1.tube_position as source_pos,"
        "  s2.box_cid as dest_id,"
        "  b2.external_name as dest_name,"
        "  s2.tube_position as dest_pos"
        " FROM"
        "  cryovial c, cryovial_store s1, box_name b1,"
        "  cryovial_store s2, box_name b2,"
        "  c_object_name t"
        " WHERE"
        "  c.cryovial_id = s1.cryovial_id AND"
        "  b1.box_cid = s1.box_cid AND"
        "  s1.cryovial_id = s2.cryovial_id AND"
        "  s2.status = 0 AND"
        "  b2.box_cid = s2.box_cid AND"
        "  t.object_cid = aliquot_type_cid AND"
        "  s1.retrieval_cid = :jobID"
        " ORDER BY"
        "  cryovial_barcode"
        );
    qd.setParam("jobID", frmSamples->job->getID());
    loadingMessage = frmSamples->loadingMessage;
    rowCount = 0; qd.open();
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
        frmSamples->vials.push_back(row);
        qd.next();
        rowCount++;
    }

    // find locations of source boxes
    map<int, const SampleRow *> samples; ROSETTA result; StoreDAO dao; int rowCount2 = 0;
	for (vector<SampleRow *>::iterator it = frmSamples->vials.begin(); it != frmSamples->vials.end(); ++it, rowCount2++) {
        SampleRow * sample = *it;
        ostringstream oss; oss<<"Finding storage for "<<sample->cryovial_barcode<<"["<<rowCount2<<"/"<<rowCount<<"]: ";

        map<int, const SampleRow *>::iterator found = samples.find(sample->store_record->getBoxID());
        if (found != samples.end()) { // fill in box location from cache map
            sample->copyLocation(*(found->second)); oss<<"(cached)";
        } else {
            if (dao.findBox(sample->store_record->getBoxID(), LCDbProjects::getCurrentID(), result)) {
                sample->copyLocation(result); //oss<<"(db)";
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

void __fastcall TfrmSamples::loadVialsWorkerThreadTerminated(TObject *Sender) {
    progressBottom->Style = pbstNormal; progressBottom->Visible = false;
    panelLoading->Visible = false;
    Screen->Cursor = crDefault;
    Enabled = true;
    chunks.clear();
    sgwChunks->clear();
    if (IDYES == Application->MessageBox(L"Do you want to automatically create chunks for this list?", L"Question", MB_YESNO)) {
        autoChunk();
    } else {
        addChunk(0); // default chunk
    }
    //showChunks();
}

