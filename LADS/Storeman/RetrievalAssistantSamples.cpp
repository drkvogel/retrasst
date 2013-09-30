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
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmSamples *frmSamples;

__fastcall TfrmSamples::TfrmSamples(TComponent* Owner) : TForm(Owner) {
    sgwChunks = new StringGridWrapper< Chunk< SampleRow > >(sgChunks, &chunks);
    sgwChunks->addCol("section",  "Section",  50);
    sgwChunks->addCol("start",    "Start",    80);
    sgwChunks->addCol("startbox", "Box",      80);
    sgwChunks->addCol("startvial","Vial",     150);
    sgwChunks->addCol("end",      "End",      150);
    sgwChunks->addCol("endbox",   "Box",      150);
    sgwChunks->addCol("endvial",  "Vial",     150);
    sgwChunks->addCol("size",     "Size",     80);
    sgwChunks->init();

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
    sgwVials->addCol("boxpos",   "Pos",              40,    SampleRow::sort_asc_slot);
    sgwVials->addCol("destbox",  "Destination box",  213,   SampleRow::sort_asc_destbox);
    sgwVials->addCol("destpos",  "Pos",              37,    SampleRow::sort_asc_destpos);
    sgwVials->init();
}

void TfrmSamples::debugLog(String s) {
    frmSamples->memoDebug->Lines->Add(s); // could use varargs: http://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c
}

void __fastcall TfrmSamples::FormCreate(TObject *Sender) {
    cbLog->Visible      = RETRASSTDEBUG;
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
}

void __fastcall TfrmSamples::FormClose(TObject *Sender, TCloseAction &Action) {
    delete_referenced< vector <SampleRow * > >(frmSamples->vials);
    delete_referenced< vector< Chunk< SampleRow > * > >(chunks); // chunk objects, not contents of chunks
}

void __fastcall TfrmSamples::btnCancelClick(TObject *Sender) {
    //Close();
    ModalResult = mrCancel;
}

void __fastcall TfrmSamples::btnSaveClick(TObject *Sender) {
    /** Insert an entry into c_box_retrieval for each destination box, recording the chunk it is in,
    and a record into l_cryovial_retrieval for each cryovial, recording its position in the list. */
    if (IDYES == Application->MessageBox(L"Save changes? Press 'No' to go back and re-order", L"Question", MB_YESNO)) {
        std::set<int> projects; projects.insert(job->getProjectID());
        frmConfirm->initialise(LCDbCryoJob::Status::DONE, "Confirm retrieval plan", projects);  //status???
        if (mrOk != frmConfirm->ShowModal()) return;
        LQuery qc(LIMSDatabase::getCentralDb());
        map<int, const SampleRow *> boxes;
        for (vector< Chunk< SampleRow > * >::const_iterator it = chunks.begin(); it != chunks.end(); it++) { // for chunks
            Chunk< SampleRow > * chunk = *it;
            for (int i = 1; i < chunk->getSize(); i++) {
                SampleRow *         sampleRow = chunk->rowAt(i);
                LPDbCryovial *      cryo  = sampleRow->cryo_record;
                LPDbCryovialStore * store = sampleRow->store_record;
                map<int, const SampleRow *>::iterator found = boxes.find(sampleRow->store_record->getBoxID());
                if (found != boxes.end()) {
                    // already added record //cached = (*(found->second));
                } else {
                    // add record
        /*  retrieval_cid	c_retrieval_job	    The retrieval task this entry is part of
            section	                            Which chunk of the retrieval plan this entry belongs to (0 = retrieve all boxes in parallel)
            box_id          box_name	        The box being retrieved (for box retrieval/disposal) or retrieved into (for sample retrieval/disposal)
            rj_box_cid                          Unique ID for this retrieval list entry (also determines retrieval order for box retrievals)
            status                              0: new record; 1: part-filled, 2: collected; 3: not found; 99: record deleted
            time_stamp                          When this record was inserted or updated */
                    qc.setSQL(
                        "INSERT INTO c_box_retrieval"
                        " (retrieval_cid, section, box_id, rj_box_cid, status)"
                        " VALUES"
                        " (:rtid, :sect, :bid, :rjbid, :stat)" // box_id, rj_box_cid - which is which?
                    );
                    qc.setParam("rtid", job->getID());
                    qc.setParam("sect", chunk->getSection());
                    qc.setParam("bid",  sampleRow->store_record->getBoxID());
                    qc.setParam("rjbid",sampleRow->dest_box_id); //??
                    qc.setParam("stat", LCDbBoxStore::Status::SLOT_ALLOCATED); //??
                    qc.execSQL();
                    boxes[sampleRow->store_record->getBoxID()] = (sampleRow); // cache result
                }
                qc.setSQL(
                    "INSERT INTO l_cryovial_retrieval"
                    " (rj_box_cid, position, cryovial_barcode, aliquot_type_cid, process_cid, time_stamp, status)"
                    " VALUES"
                    " (:rjid, :pos, :barc, :aliq, :pid, 'now', :st)"
                ); //status?
                qc.setParam("rjid", sampleRow->dest_box_id); //??
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
        ModalResult = mrOk; // update c_retrieval_job (in progress)
    } else { // start again
        chunks.clear();
        addChunk(); // start again
    }
}

void __fastcall TfrmSamples::btnAddChunkClick(TObject *Sender) {
    addChunk();
}

void __fastcall TfrmSamples::cbLogClick(TObject *Sender) {
    memoDebug->Visible = cbLog->Checked;
    splitterDebug->Visible  = cbLog->Checked;
}

void __fastcall TfrmSamples::btnDelChunkClick(TObject *Sender) {
    if (RETRASSTDEBUG || IDYES == Application->MessageBox(L"Are you sure you want to delete the last chunk?", L"Question", MB_YESNO)) {
        delete chunks.back();
        chunks.pop_back();
        (*(chunks.end()))->setEnd(vials.size());
        showChunks();
    }
    if (chunks.size() == 1) btnDelChunk->Enabled = false;
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
            background = RETRIEVAL_ASSISTANT_DONE_COLOUR; //background = RETRIEVAL_ASSISTANT_ERROR_COLOUR;
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
    ostringstream oss; oss << __FUNC__; oss<<sgwVials->printColWidths(); debugLog(oss.str().c_str());
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
        //Close();
        ModalResult = mrCancel; //??? don't use modalresult
    }
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

void __fastcall TfrmSamples::sgVialsDblClick(TObject *Sender) {
    // mark chunk boundary
    //msgbox("chunk split");
    addChunk();
    showChunks();
}

void TfrmSamples::addChunk() {
    Chunk< SampleRow > * chunk;// = new Chunk< SampleRow >;
    if (chunks.size() == 0) { // first chunk, make default chunk from entire listrows
        chunk = new Chunk< SampleRow >(
            sgwVials, chunks.size() + 1,
            //vector< SampleRow * > * rows;
//            1, ((*(sgwVials->rows))[0]->src_box_name), ((*(sgwVials->rows))[0]->cryo_record->getSampleID()),
//            (*(sgwVials->rows)).size(), ((*(sgwVials->rows))[(*(sgwVials->rows)).size()-1]->src_box_name), ((*(sgwVials->rows))[(*(sgwVials->rows)).size()-1]->cryo_record->getSampleID()),
            1,              vials[0]->src_box_name,                vials[0]->cryo_record->getBarcode(),
            vials.size(),   vials[vials.size()-1]->src_box_name,   vials[vials.size()-1]->cryo_record->getBarcode()
        ); // 1-indexed // size is calculated
        chunk->setEnd(vials.size());
        chunk->setStart(1);
    } else {
        // new chunk starting one after the end of the last one...
        // should be starting where you chose the division point, end of last one will always be end of list!
        // with my current idiom, 'the division point' is the point you've chosen... in the current chunk!
                                                        // section num    // start                      // end
        chunk = new Chunk< SampleRow >(
            sgwVials, chunks.size() + 1,
            currentChunk()->getSize()+1,    vials[0]->src_box_name,                vials[0]->cryo_record->getBarcode(), // first
            vials.size(),                   vials[vials.size()-1]->src_box_name,   vials[vials.size()-1]->cryo_record->getBarcode() // last
        );
    }

    // fixme make it the current chunk
    sgChunks->Row = sgChunks->RowCount - 1;
    chunks.push_back(chunk);
    btnDelChunk->Enabled = true;
    showChunks();
}

void TfrmSamples::showChunks() {
    if (0 == chunks.size()) { // must always have one chunk anyway
        throw Exception("No chunks");
    } else {
        sgChunks->RowCount = chunks.size() + 1;
        sgChunks->FixedRows = 1; // "Fixed row count must be LESS than row count"
    }
    int row = 1;
    for (vector< Chunk< SampleRow > * >::const_iterator it = chunks.begin(); it != chunks.end(); it++, row++) {
        Chunk< SampleRow > * chunk = *it;
/*    sgwChunks->addCol("section",  "Section",  50);
    sgwChunks->addCol("start",    "Start",    80);
    sgwChunks->addCol("startbox", "Box",      80);
    sgwChunks->addCol("startvial","Vial",     150);
    sgwChunks->addCol("end",      "End",      150);
    sgwChunks->addCol("endbox",   "Box",      150);
    sgwChunks->addCol("endvial",  "Vial",     150);
    sgwChunks->addCol("size",     "Size",     80);*/
        sgChunks->Cells[sgwChunks->colNameToInt("section")]   [row] = chunk->getSection();
        sgChunks->Cells[sgwChunks->colNameToInt("start")]     [row] = chunk->getStart();
        sgChunks->Cells[sgwChunks->colNameToInt("end")]       [row] = chunk->getEnd(); //end.c_str();
        sgChunks->Cells[sgwChunks->colNameToInt("size")]      [row] = chunk->getSize();//chunk->end - chunk->start;
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
    Screen->Cursor = crSQLWait; // disable mouse? //ShowCursor(false);
    Enabled = false;

    if (NULL == chunk) { chunk = currentChunk(); } // default

    if (chunk->getSize() <= 0) {
        sgwVials->clear();
    } else {
        sgVials->RowCount = chunk->getSize(); //getSize() works, ie. returns debug value.
        sgVials->FixedRows = 1;
    }

    for (int row = 1; row < chunk->getSize(); row++) {
        SampleRow * sampleRow = chunk->rowAt(row); // does OutputDebugString work? yes, in Event Log tab.
            // start + pos is probably wrong. it is being called. no debug symbols for templates though. way to enable this?
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

    Screen->Cursor = crDefault;
    Enabled = true;
}

void __fastcall TfrmSamples::sgChunksSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value) {
    //ostringstream oss; oss<<__FUNC__<<String(sgChunks->Cells[ACol][ARow].c_str()); debugLog(oss.str().c_str());
}

void __fastcall TfrmSamples::sgChunksGetEditText(TObject *Sender, int ACol, int ARow, UnicodeString &Value) {
    //ostringstream oss; oss<<__FUNC__<<String(sgChunks->Cells[ACol][ARow].c_str());
    //debugLog(oss.str().c_str());
}

void TfrmSamples::autoChunk() {
    frmAutoChunk->ShowModal();
}

//-------------- sorters --------------

void TfrmSamples::addSorter() {
    ostringstream oss; oss << __FUNC__ << groupSort->ControlCount; debugLog(oss.str().c_str());
    TComboBox * combo = new TComboBox(this);
    combo->Parent = groupSort; // new combo is last created, aligned to left
        // put in right order: take them all out, sort and put back in in reverse order?
    combo->Width = 250;
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
    for (int i=groupSort->ControlCount-1; i>=0; i--) { // work backwards through controls to find last combo box // controls are in creation order, ie. buttons first from design, and last added combo is last
        TControl * control = groupSort->Controls[i];
        TComboBox * combo = dynamic_cast<TComboBox *>(control);
        if (combo != NULL) {
            if (-1 != combo->ItemIndex) {
                debugLog("sorting: ");
                debugLog(combo->Items->Strings[combo->ItemIndex].c_str());
                //chunk->;
            }
        } else {
            debugLog("not a combo box, finish sorting.");
            break; // finished sorting
        }
    }
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
    loadingMessage = oss.str().c_str(); //return;

    rowCount = 0;
    LQuery qd(Util::projectQuery(frmSamples->job->getProjectID(), true)); // ddb
    qd.setSQL( // from spec 2013-09-11
        "SELECT"
        "  s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.cryovial_position," // for LPDbCryovialStore
        "  s1.record_id, c.sample_id, c.aliquot_type_cid, " // for LPDbCryovial
            // LPDbCryovial::storeID( query.readInt( "record_id" ) ) <-- record_id comes from cryovial_store?
        "  c.cryovial_barcode, t.external_name AS aliquot,"
        "  b1.box_cid as source_id,"
        "  b1.external_name as source_name,"
        "  s1.cryovial_position as source_pos,"
        "  s2.box_cid as dest_id,"
        "  b2.external_name as dest_name,"
        "  s2.cryovial_position as dest_pos"
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
        frmSamples->vials.push_back(row);
        qd.next();
        rowCount++;
    }

    // find locations of source boxes
    map<int, const SampleRow *> samples; ROSETTA result; StoreDAO dao;
    int rowCount2 = 0;
	for (vector<SampleRow *>::iterator it = frmSamples->vials.begin(); it != frmSamples->vials.end(); ++it, rowCount2++) {
        SampleRow * sample = *it;
        ostringstream oss; oss<<"Finding storage for "<<sample->cryovial_barcode<<"["<<rowCount2<<"/"<<rowCount<<"]";
        try {
            map<int, const SampleRow *>::iterator found = samples.find(sample->store_record->getBoxID());
            if (found != samples.end()) { // fill in box location from cache map
                sample->copyLocation(*(found->second)); //oss<<"(cached)";
            } else {
                if (dao.findBox(sample->store_record->getBoxID(), LCDbProjects::getCurrentID(), result)) {
                    sample->copyLocation(result); //oss<<"(db)";
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
        addChunk(); // default chunk
    }
    showChunks();
}

