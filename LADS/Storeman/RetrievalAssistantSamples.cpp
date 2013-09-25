#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistantSamples.h"
#include "StoreUtil.h"
#include "LCDbRack.h"
#include "RetrievalAssistantAutoChunk.h"
#include "StoreDAO.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmSamples *frmSamples;

static bool sort_asc_barcode(const SampleRow *a, const SampleRow *b) { return a->cryovial_barcode.compare(b->cryovial_barcode) > 0; }

__fastcall TfrmSamples::TfrmSamples(TComponent* Owner) : TForm(Owner) {
    sgwChunks = new StringGridWrapper<SampleChunk>(sgChunks, &chunks);
    sgwChunks->addCol("section",  "Section",  200);
    sgwChunks->addCol("start",    "Start",    200);
    sgwChunks->addCol("end",      "End",      200);
    sgwChunks->addCol("size",     "Size",     200);
    sgwChunks->init();

    sgwVials = new StringGridWrapper<SampleRow>(sgVials, &vials);
    sgwVials->addCol("barcode",  "Barcode",          102,   SampleRow::sort_asc_barcode);
    sgwVials->addCol("aliquot",  "Aliquot",          100,   SampleRow::sort_asc_aliquot);
    sgwVials->addCol("currbox",  "Current box",      275,   SampleRow::sort_asc_currbox);
    sgwVials->addCol("currpos",  "Pos",              43,    SampleRow::sort_asc_currpos);
    sgwVials->addCol("site",     "Site",             116,   SampleRow::sort_asc_site);
    sgwVials->addCol("vesspos",  "Position",         50,    SampleRow::sort_asc_position); // vessel pos in site
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

void __fastcall TfrmSamples::FormShow(TObject *Sender) {
    ostringstream oss; oss<<job->getName()<<" : "<<job->getDescription();
    Caption = oss.str().c_str();
    btnSave->Enabled = true;
    chunks.clear();
    sgwChunks->clear();
    sgwVials->clear();
    timerLoadVials->Enabled = true;
    //if (IDYES == Application->MessageBox(L"Do you want to automatically create chunks for this list?", L"Question", MB_YESNO)) {autoChunk();}
}

void __fastcall TfrmSamples::FormClose(TObject *Sender, TCloseAction &Action) {
    delete_referenced< std::vector <SampleRow * > >(frmSamples->vials);
    delete_referenced<vecpSampleChunk>(chunks); // chunk objects, not contents of chunks
}

void __fastcall TfrmSamples::btnCancelClick(TObject *Sender) { Close(); }

void __fastcall TfrmSamples::btnSaveClick(TObject *Sender) {
    if (IDYES == Application->MessageBox(L"Save changes? Press 'No' to go back and re-order", L"Question", MB_YESNO)) {
        // sign off?
        // create the retrieval plan by inserting into c_box_retrieval and l_sample_retrieval
        for (vecpSampleChunk::const_iterator it = chunks.begin(); it != chunks.end(); it++) { // for chunks
            // TODO insert rows into c_box_retrieval and l_cryovial_retrieval
            SampleChunk * chunk = *it;
            //      for samples
            //          insert destination box into C_BOX_RETRIEVAL with current section (chunk) number
            for (std::vector <SampleRow * >::const_iterator it = chunk->rows.begin(); it != chunk->rows.end(); it++) { // vecpDataRow?
                SampleRow * sampleRow = (SampleRow *)*it;
                LPDbCryovialStore * vial = sampleRow->store_record;
                // insert into l_sample_retrieval
            }

            /*
            retrieval_cid	 i4		c_retrieval_job	 The retrieval task this entry is part of
            retrieval_type	 i2			obsolete - see c_retrieval_job
            box_id	 i4		box_name	 The box being retrieved (for box retrieval/disposal) or retrieved into (for sample retrieval/disposal)
            section	 i2			 Which chunk of the retrieval plan this entry belongs to (0 = retrieve all boxes in parallel)
            position	 i2			obsolete
            box_name	 v32			obsolete
            rj_box_cid	 i4	 1		 Unique ID for this retrieval list entry (also determines retrieval order for box retrievals)
            status	 i2			 0: new record; 1: part-filled, 2: collected; 3: not found; 99: record deleted
            time_stamp	 d/t			 When this record was inserted or updated*/
        }
        return;
        // update c_retrieval_job (in progress)
        job->setStatus(LCDbCryoJob::INPROGRESS);
        job->saveRecord(LIMSDatabase::getCentralDb());
        btnSave->Enabled = false;
        Close();
    } else { // start again
        chunks.clear(); // delete memory
        addChunk();
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
        //fixme move contents into preceding chunk
        //vecpSampleRow::back_insert_iterator bit;
        //vecpSampleRow::back_inserter bit;
        delete chunks.back();
        chunks.pop_back();
        showChunks();
    }
    if (chunks.size() == 1) btnDelChunk->Enabled = false;
}

void __fastcall TfrmSamples::sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
    TColor background = clWindow;
    if (0 == ARow) {
        background = clBtnFace;
    } else {
        SampleChunk * chunk = (SampleChunk *)sgChunks->Objects[0][ARow];
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
    ostringstream oss; oss << __FUNC__;
    oss<<sgwVials->printColWidths(); debugLog(oss.str().c_str()); // print column widths so we can copy them into the source
    //sortChunk(currentChunk(), ACol, Sorter<SampleRow *>::TOGGLE);
    //currentChunk()->sortToggle(ACol);
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
        Close();
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

void TfrmSamples::showChunks() {
    if (0 == chunks.size()) { // must always have one chunk anyway
        throw Exception("No chunks");
        sgwChunks->clear();
    } else {
        sgChunks->RowCount = chunks.size() + 1;
        sgChunks->FixedRows = 1; // "Fixed row count must be LESS than row count"
    }
    int row = 1;
    for (vecpSampleChunk::const_iterator it = chunks.begin(); it != chunks.end(); it++, row++) {
        SampleChunk * chunk = *it;
        // sgwChunks->colNameToInt
//        sgChunks->Cells[SGCHUNKS_SECTION]   [row] = chunk->section;
//        sgChunks->Cells[SGCHUNKS_START]     [row] = chunk->start.c_str();
//        sgChunks->Cells[SGCHUNKS_END]       [row] = chunk->end.c_str();
//        sgChunks->Cells[SGCHUNKS_SIZE]      [row] = 0;//chunk->end - chunk->start;
        sgChunks->Objects[0][row] = (TObject *)chunk;
    }
    showChunk();
}

void TfrmSamples::addChunk() {
    SampleChunk * chunk = new SampleChunk;
    chunk->setSection(chunks.size() + 1);
    if (chunks.size() == 0) { // first chunk, make default chunk from entire listrows
//        for (vecpSampleRow::const_iterator it = vials.begin(); it != vials.end(); it++) {
//            chunk->rows.push_back((SampleRow *)*(it));
//        }
        chunk->rows = vials;
    } else {
        //chunk->rows.push_back(*(vials.begin()));
    }
    chunks.push_back(chunk);
    btnDelChunk->Enabled = true;
    showChunks();
}

void TfrmSamples::autoChunk() {
    frmAutoChunk->ShowModal();
}

SampleChunk * TfrmSamples::currentChunk() {
    if (sgChunks->Row < 1) sgChunks->Row = 1; // force selection of 1st row
    SampleChunk * chunk = (SampleChunk *)sgChunks->Objects[0][sgChunks->Row];
    if (NULL == chunk) {// still null
        ostringstream oss; oss<<__FUNC__<<": Null chunk"; debugLog(oss.str().c_str());
        throw Exception("null chunk");
    }
    return chunk;
}

void TfrmSamples::showChunk(SampleChunk * chunk) {
    if (NULL == chunk) { // default
        chunk = currentChunk();
    }
    if (chunk->rows.size() <= 0) {
        sgwVials->clear();
    } else {
        sgVials->RowCount = chunk->rows.size();
        sgVials->FixedRows = 1;
    }
    int row = 1;
    for (std::vector<SampleRow * >::const_iterator it = chunk->rows.begin(); it != chunk->rows.end(); it++, row++) { // vecpDataRow?
        SampleRow * sampleRow = (SampleRow *)*it;
        LPDbCryovialStore * vial = sampleRow->store_record;

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
}

void TfrmSamples::addSorter() {
    ostringstream oss; oss << __FUNC__ << groupSort->ControlCount; debugLog(oss.str().c_str());
    TComboBox * combo = new TComboBox(this);
    combo->Parent = groupSort; // new combo is last created, aligned to left
        // to put in right order: take them all out, sort and put back in in reverse order?
    combo->Align = alLeft;
    combo->Style = csDropDown; // csDropDownList
    //for (int i=0; i<SGVIALS_NUMCOLS; i++) {
    for (int i=0; i<sgwVials->colCount(); i++) {
        //combo->AddItem(sorter[i].description.c_str(), (TObject *)&sorter[i]);
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
    SampleChunk * chunk = currentChunk();
    for (int i=groupSort->ControlCount-1; i>=0; i--) { // work backwards through controls to find last combo box // controls are in creation order, ie. buttons first from design, and last added combo is last
        TControl * control = groupSort->Controls[i];
        TComboBox * combo = dynamic_cast<TComboBox *>(control);
        if (combo != NULL) {
            if (-1 != combo->ItemIndex) {
                debugLog("sorting: ");
                debugLog(combo->Items->Strings[combo->ItemIndex].c_str());
                //sortChunk(chunk, combo->ItemIndex, Sorter<SampleRow *>::ASCENDING);
            }
        } else {
            debugLog("not a combo box, finish sorting.");
            break; // finished sorting
        }
    }
}

//void TfrmSamples::sortChunk(SampleChunk * chunk, int col, Sorter<SampleRow *>::SortOrder order) {
//void TfrmSamples::sortChunk(SampleChunk * chunk, int col) {
//    Screen->Cursor = crSQLWait;
//    switch (order) {
//        case Sorter<SampleRow *>::ASCENDING:
//            sgwVials->sort_asc(col);
//            //chunk->rows
//            break;
//        case Sorter<SampleRow *>::DESCENDING:
//            sgwVials->sort_dsc(col);
//            break;
//        case Sorter<SampleRow *>::TOGGLE:
//            sgwVials->sort_toggle(col);
//            break;
//    }
//    showChunk(chunk);
//    Screen->Cursor = crDefault;
//}

void __fastcall TfrmSamples::sgChunksSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value) {
    ostringstream oss;
    //oss<<__FUNC__<<String(sgChunks->Cells[ACol][ARow].c_str())<endl;
    //debugLog(oss.str().c_str());
    debugLog(sgChunks->Cells[ACol][ARow]);
}

void __fastcall TfrmSamples::sgChunksGetEditText(TObject *Sender, int ACol, int ARow, UnicodeString &Value) {
    ostringstream oss;
    oss<<__FUNC__; debugLog(oss.str().c_str()); //String(sgChunks->Cells[ACol][ARow].c_str())<endl;
    //debugLog(oss.str().c_str());
    debugLog(sgChunks->Cells[ACol][ARow]);
}

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
    delete_referenced< std::vector<SampleRow * > >(frmSamples->vials);
    ostringstream oss; oss<<frmSamples->loadingMessage<<" (preparing query)";
    loadingMessage = oss.str().c_str(); //return;

    rowCount = 0;
    LQuery qd(Util::projectQuery(frmSamples->job->getProjectID(), true)); // ddb
    qd.setSQL( // from spec 2013-09-11
    /* destination box and position, cryovial barcode and current box, position,
       structure and location of the primary and secondary aliquots */
        "SELECT"
        "  s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.cryovial_position," // for LPDbCryovialStore
        "  cryovial_barcode, t.external_name AS aliquot,"
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

    // find the locations of the source boxes
    std::map<int, const SampleRow *> samples;
	ROSETTA result;
	StoreDAO dao;
    int rowCount2 = 0;
	for (std::vector<SampleRow *>::iterator it = frmSamples->vials.begin(); it != frmSamples->vials.end(); ++it, rowCount2++) {
        SampleRow * sample = *it;
        ostringstream oss; oss<<"Finding storage for "<<sample->cryovial_barcode<<"["<<rowCount2<<"/"<<rowCount<<"]";
        try {
            std::map<int, const SampleRow *>::iterator found = samples.find(sample->store_record->getBoxID());
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
    chunks.clear();
    sgwChunks->clear();
    addChunk(); // default chunk
    showChunks();
    Screen->Cursor = crDefault;
    Enabled = true;
}

void __fastcall TfrmSamples::sgVialsDblClick(TObject *Sender) {
    // mark chunk boundary
    msgbox("chunk split");
}

