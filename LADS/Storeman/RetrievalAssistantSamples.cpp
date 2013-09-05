#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistantSamples.h"
#include "StoreUtil.h"
#include "LCDbRack.h"
#include "RetrievalAssistantAutoChunk.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSamples *frmSamples;

__fastcall LoadVialsWorkerThread::LoadVialsWorkerThread() : TThread(false) {
    FreeOnTerminate = true;
}

void __fastcall LoadVialsWorkerThread::updateStatus() {
    //ostringstream oss; oss<<frmSamples->loadingMessage<<"\n"<<rowCount<<" vials";//<<numerator<<" of "<<denominator;
    //frmSamples->panelLoading->Caption = oss.str().c_str();
    frmSamples->panelLoading->Caption = loadingMessage.c_str(); //oss.str().c_str();
    frmSamples->panelLoading->Repaint();
}

void __fastcall LoadVialsWorkerThread::Execute() {
    delete_referenced<vecpSampleRow>(frmSamples->vials);
    ostringstream oss; oss<<frmSamples->loadingMessage<<" (preparing query)";
    loadingMessage = oss.str().c_str();
    rowCount = 0;
    {
    LQuery qd(Util::projectQuery(frmSamples->job->getProjectID(), true));
    qd.setSQL(
        "SELECT"
        "   cs.cryovial_id, cs.note_exists, cs.retrieval_cid, cs.box_cid, cs.status, cs.cryovial_position,"
        "   c.cryovial_barcode, t.external_name AS aliquot, b.external_name AS box,"
        "   s.external_name AS site, m.position, v.external_full AS vessel,"
        "   shelf_number, r.external_name AS rack, bs.slot_position"
        " FROM"
        "   cryovial c, cryovial_store cs, box_name b, box_store bs, c_rack_number r,"
        "   c_tank_map m, c_object_name s,"   // site
        "   c_object_name v,"                 // vessel
        "   c_object_name t"                  // aliquot?
        " WHERE"
        "   c.cryovial_id = cs.cryovial_id AND"
        "   b.box_cid = cs.box_cid AND"
        "   b.box_cid = bs.box_cid AND"
        "   bs.status = 6 AND"    // 6?
        "   t.object_cid = aliquot_type_cid AND"
        "   bs.rack_cid = r.rack_cid AND"
        "   r.tank_cid = m.tank_cid AND"
        "   s.object_cid = location_cid AND"
        "   v.object_cid = storage_cid AND"
        "   cs.retrieval_cid = :jobID");
    qd.setParam("jobID", frmSamples->job->getID());
    /* -- may have destination box defined, could find with left join:
    but left join on ddb not allowed in ingres
    from
         cryovial_store s1
    left join
        cryovial c on c.cryovial_cid = s1.cryovial_id
    left join
        box_name n1 on n1.box_cid = s1.box_cid
    left join
        cryovial_store s2 on s1.cryovial_id = s2.cryovial_id and
        s2.status = 0
    left join
        box_name n2 on n2.box_cid = s2.box_cid
    where
        s1.retrieval_cid = :jobID*/
    loadingMessage = frmSamples->loadingMessage; // base
    qd.open(); // most time - about 30 seconds - is taken opening the query. Cursoring through 1000+ rows takes 1-2 seconds
    while (!qd.eof()) {
        ostringstream oss; oss<<"Found "<<rowCount<<" vials";//<<numerator<<" of "<<denominator;
        loadingMessage = oss.str().c_str();
        if (0 == rowCount % 10) Synchronize((TThreadMethod)&updateStatus); // don't do graphical things in the thread without Synchronising
            // can't use args for synced method, don't know why
        LPDbCryovialStore * vial = new LPDbCryovialStore(qd);
        pSampleRow  row = new SampleRow(
            vial,
            qd.readString("cryovial_barcode"),
            qd.readString("aliquot"),
            qd.readString("box"),
            qd.readString("site"),
            qd.readInt("position"),
            qd.readString("vessel"),
            qd.readInt("shelf_number"),
            qd.readString("rack"),
            qd.readInt("slot_position")
            );
        frmSamples->vials.push_back(row);
        qd.next();
        rowCount++;
    }
    }
    // look for destination boxes, can't left join in ddb, so do project query per row
    // may be v time-consuming - could do outer join instead and check sequence for gaps
    int rowCount2 = 0;
    for (vecpSampleRow::iterator it = frmSamples->vials.begin(); it != frmSamples->vials.end(); it++) { // vecpDataRow?
        ostringstream oss; oss<<"Finding destinations: "<<rowCount2<<"/"<<rowCount;
        loadingMessage = oss.str().c_str();
        if (0 == rowCount2 % 10) Synchronize((TThreadMethod)&updateStatus);

        pSampleRow sampleRow = (pSampleRow)*it;
        //look in the right database!
        LQuery qp(Util::projectQuery(frmSamples->job->getProjectID(), false)); // project db
        qp.setSQL(
            "SELECT"
            "   n1.box_cid AS boxid, n1.external_name AS boxname, s2.cryovial_position AS pos"
            " FROM"
            "   cryovial_store s1, cryovial c, box_name n1, cryovial_store s2, box_name n2"
            " WHERE"
            "   c.cryovial_id = s1.cryovial_id"
            " AND n1.box_cid = s1.box_cid"
            " AND s1.cryovial_id = s2.cryovial_id"
            " AND s2.status = 0"
            " AND n2.box_cid = s2.box_cid"
            " AND s1.retrieval_cid = :jobID"
            " AND s1.cryovial_id = :vial" // e.g. 1137824 (t_ldb1)
            );
        qp.setParam("jobID", frmSamples->job->getID());
        qp.setParam("vial",  sampleRow->store_record->getID());
        //LPDbCryovialStore * vial = sampleRow->store_record;

        try {
            if (qp.open()) {
                sampleRow->dest_box_id      = qp.readInt("boxid");
                sampleRow->dest_box_name    = qp.readString("boxname");
                sampleRow->dest_box_pos     = qp.readInt("pos");
            } else {
                sampleRow->dest_box_id      = 0;
                sampleRow->dest_box_name    = "";
                sampleRow->dest_box_pos     = 0;
            }

        } catch(Exception & e) {
            //msgbox(String(e.Message.c_str()).c_str());
            String msg = e.Message;
        } catch(...) {
            msgbox("error");

        }
        rowCount2++;
    }
}

__fastcall TfrmSamples::TfrmSamples(TComponent* Owner) : TForm(Owner) { }

void __fastcall TfrmSamples::FormCreate(TObject *Sender) {
/*
// template
    ostringstream oss; oss<<__FUNC__; debugLog(oss.str().c_str());
    LQuery q(LIMSDatabase::getCentralDb());
    //LQuery q(Util::projectQuery(project), true); // get ddb with central and project dbs
    q.setSQL("SELECT * FROM  WHERE status != 99");
    Screen->Cursor = crSQLWait;
    q.open();
    delete_referenced<vecp>(s);
    while (!q.eof()) {
        RetrievalPlan * plan = new RetrievalPlan(q.readString("name"));
        //ob-> = q.readInt("");
        //ob-> = q.readString("");
        s.push_back();
        q.next();
    }
    Screen->Cursor = crDefault;
*/
    cbLog->Visible      = RETRASSTDEBUG;
    maxRows             = DEFAULT_NUMROWS;
    job                 = NULL;
    setupStringGrid(sgChunks, SGCHUNKS_NUMCOLS, sgChunksColName, sgChunksColWidth);
    setupStringGrid(sgVials, SGVIALS_NUMCOLS, sgVialColName, sgVialColWidth);
    radbutDefault->Caption = DEFAULT_NUMROWS;
    loadingMessage = "Loading samples, please wait...";
}

void __fastcall TfrmSamples::FormShow(TObject *Sender) {
    btnSave->Enabled = true;
    chunks.clear();
    clearSG(sgChunks);
    //addChunk(); // no - not before list loaded
    //showChunks();
    clearSG(sgVials);
    //timerLoadVials->Enabled = true;
    //if (IDYES == Application->MessageBox(L"Do you want to automatically create chunks for this list?", L"Question", MB_YESNO)) {autoChunk();}
}

void __fastcall TfrmSamples::FormClose(TObject *Sender, TCloseAction &Action) {
    delete_referenced<vecpSampleRow>(frmSamples->vials);
    delete_referenced<vecpSampleChunk>(chunks);
}

void __fastcall TfrmSamples::btnCancelClick(TObject *Sender) { Close(); }

void __fastcall TfrmSamples::btnSaveClick(TObject *Sender) {
    if (IDYES == Application->MessageBox(L"Save changes? Press 'No' to go back and re-order", L"Question", MB_YESNO)) {
        // save stuff
        // ie, create the retrieval plan by inserting into c_box_retrieval and l_sample_retrieval
        Close();
    } else { // start again
        chunks.clear();
        addChunk();
    }
    //btnSave->Enabled = false;
    // TODO insert rows into c_box_retrieval and l_cryovial_retrieval
    // TODO update c_retrieval_job (in progress)
}

void __fastcall TfrmSamples::btnAddChunkClick(TObject *Sender) {
    addChunk();
}

void __fastcall TfrmSamples::cbLogClick(TObject *Sender) {
    memoDebug->Visible = cbLog->Checked;
}

void __fastcall TfrmSamples::btnDelChunkClick(TObject *Sender) {
    if (RETRASSTDEBUG || IDYES == Application->MessageBox(L"Are you sure you want to delete the last chunk?", L"Question", MB_YESNO)) {
        delete chunks.back();
        chunks.pop_back();
        showChunks();
    }
    if (chunks.size() == 1) btnDelChunk->Enabled = false;
}

void __fastcall TfrmSamples::radbutDefaultClick(TObject *Sender) { radgrpRowsChange(); }

void __fastcall TfrmSamples::radbutAllClick(TObject *Sender) { radgrpRowsChange(); }

void __fastcall TfrmSamples::radbutCustomClick(TObject *Sender) { radgrpRowsChange(); }

void __fastcall TfrmSamples::timerCustomRowsTimer(TObject *Sender) {
    std::ostringstream oss; oss <<__FUNC__<<": load"<<": numrows: "<<maxRows; debugLog(oss.str().c_str());
    timerCustomRows->Enabled = false;
    maxRows = editCustomRows->Text.ToIntDef(0);
    showChunk();
}

void __fastcall TfrmSamples::editCustomRowsChange(TObject *Sender) {
    timerCustomRows->Enabled = false; // reset
    timerCustomRows->Enabled = true;;
}

void __fastcall TfrmSamples::sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
    TColor background = clWindow;
    if (0 == ARow) {
        background = clBtnFace;
    } else {
        SampleChunk * chunk = NULL;
        chunk = (SampleChunk *)sgChunks->Objects[0][ARow];
        background = RETRIEVAL_ASSISTANT_DONE_COLOUR; //break;
        if (NULL == chunk) {
            background = clBtnFace; //RETRIEVAL_ASSISTANT_ERROR_COLOUR;
        } else {
            background = RETRIEVAL_ASSISTANT_DONE_COLOUR; //background = RETRIEVAL_ASSISTANT_ERROR_COLOUR;
        }
        //else if (chunk->
    }
    TCanvas * cnv = sgChunks->Canvas;
	cnv->Brush->Color = background;
	cnv->FillRect(Rect);
    if (State.Contains(gdSelected)) {
        TFontStyles oldFontStyle = cnv->Font->Style;
        TPenStyle oldPenStyle = cnv->Pen->Style;
        cnv->Pen->Style = psDot;
        cnv->Rectangle(Rect.Left+1, Rect.Top+1, Rect.Right-1, Rect.Bottom-1);
        cnv->Font->Style = TFontStyles() << fsBold;
    	cnv->TextOut(Rect.Left+5, Rect.Top+5, sgChunks->Cells[ACol][ARow]);
        cnv->Pen->Style     = oldPenStyle;
        cnv->Font->Style    = oldFontStyle;
	} else {
        cnv->TextOut(Rect.Left+5, Rect.Top+5, sgChunks->Cells[ACol][ARow]);
    }
}

void __fastcall TfrmSamples::loadVialsWorkerThreadTerminated(TObject *Sender) {
    progressBottom->Style = pbstNormal; progressBottom->Visible = false;
    panelLoading->Visible = false;
    chunks.clear();
    clearSG(sgChunks);
    addChunk(); // create a default chunk // no - not before list loaded
    showChunks(); //showChunk(); // must do this outside thread, unless synchronised - does gui stuff
    Screen->Cursor = crDefault;
}

void __fastcall TfrmSamples::btnAutoChunkClick(TObject *Sender) {
    autoChunk();
}

void __fastcall TfrmSamples::btnIncrClick(TObject *Sender) {
    //
}

void __fastcall TfrmSamples::btnDecrClick(TObject *Sender) {
    //
}

void __fastcall TfrmSamples::sgVialsFixedCellClick(TObject *Sender, int ACol, int ARow) { // sort by column
    ostringstream oss; oss << __FUNC__; oss << printColWidths(sgVials); debugLog(oss.str().c_str()); // print column widths so we can copy them into the source
    sortList(ACol);
}

void __fastcall TfrmSamples::sgVialsClick(TObject *Sender) {
    SampleRow * sample  = (SampleRow *)sgVials->Objects[0][sgVials->Row];
    sample?debugLog(sample->str().c_str()):debugLog("NULL sample");
    job?debugLog(job->getName().c_str()):debugLog("NULL job");
}

void __fastcall TfrmSamples::timerLoadVialsTimer(TObject *Sender) {
    timerLoadVials->Enabled = false;
    loadRows(); // so that gui can be updated
}

void __fastcall TfrmSamples::btnRejectClick(TObject *Sender) {
    if (IDYES == Application->MessageBox(L"Are you sure you want to reject this list?", L"Question", MB_YESNO)) {
        //xxxxrejectList();
        Close();
    }
}

void __fastcall TfrmSamples::btnAddSortClick(TObject *Sender) {
    addSorter();
}

void TfrmSamples::addSorter() {
    ostringstream oss; oss << __FUNC__ << groupSort->ControlCount; debugLog(oss.str().c_str());
    TComboBox * combo = new TComboBox(this);
    combo->Parent = groupSort;
    combo->Align = alLeft;
    combo->Items->AddObject(groupSort->ControlCount, NULL);
    // new combo is last created,
    //groupSort->InsertControl(combo);
    //ostringstream oss;
    //oss.str(""); oss << __FUNC__ << groupSort->ControlCount; debugLog(oss.str().c_str());
}

void __fastcall TfrmSamples::btnDelSortClick(TObject *Sender) {
    //groupSort->Controls[groupSort->ControlCount-1]
    //if (dynamic_cast<TComboBox *>() != NULL) {
    //    groupSort->RemoveComponent();
    //}

    //for (int i=groupSort->ControlCount-1; i>=0; i--) {
        // work backwards through controls to find last combo box
    for (int i=0; i<groupSort->ControlCount; i++) {
        // controls are in creation order, ie. buttons first from design, and last added combo is last
        TControl * control = groupSort->Controls[i];
        TButton * button = dynamic_cast<TButton *>(control);
        if (button != NULL) {
            debugLog("found a button, caption: ");
            debugLog(button->Caption);
            continue; // skip
        }
        TComboBox * combo = dynamic_cast<TComboBox *>(control);
        if (combo != NULL) {
            debugLog("found a combo box, text:");
            debugLog(combo->Text);
        }
        //groupSort->Controls[i]->RemoveComponent();
        //groupSort->Controls
    }

    //TComboBox * combo = dynamic_cast<TComboBox *>(groupSort->Controls[groupSort->ControlCount-1]);
    //if (NULL != combo) groupSort->RemoveComponent(combo);
}

void __fastcall TfrmSamples::sgChunksClick(TObject *Sender) {
    // show current chunk
    showChunk(); // default is 1st
}

void TfrmSamples::debugLog(String s) {
    frmSamples->memoDebug->Lines->Add(s);
}

void TfrmSamples::radgrpRowsChange() {
    maxRows = 0;
    if (radbutCustom->Checked) {
        editCustomRows->Enabled  = true;
        timerCustomRows->Enabled = true;
        return; // allow user to edit value
    } else {
        editCustomRows->Enabled = false;
        if (radbutDefault->Checked) {
            maxRows = DEFAULT_NUMROWS;
        } else if (radbutAll->Checked) {
            maxRows = -1;
        }
    }
    std::ostringstream oss; oss <<__FUNC__<<": numrows: "<<maxRows; debugLog(oss.str().c_str());
    showChunk();
}

void TfrmSamples::showChunks() {
    if (0 == chunks.size()) { // must always have one chunk anyway
        clearSG(sgChunks);
    } else {
        sgChunks->RowCount = chunks.size() + 1;
        sgChunks->FixedRows = 1; // "Fixed row count must be LESS than row count"
    }
    int row = 1;
    for (vecpSampleChunk::const_iterator it = chunks.begin(); it != chunks.end(); it++, row++) {
        SampleChunk * chunk = *it;
        sgChunks->Cells[SGCHUNKS_SECTION]   [row] = chunk->section;
        sgChunks->Cells[SGCHUNKS_START]     [row] = chunk->start.c_str();
        sgChunks->Cells[SGCHUNKS_END]       [row] = chunk->end.c_str();
        sgChunks->Cells[SGCHUNKS_SIZE]      [row] = 0;//chunk->end - chunk->start;
        sgChunks->Objects[0][row] = (TObject *)chunk;
    }
    showChunk();
}

void TfrmSamples::addChunk() {
    SampleChunk * chunk = new SampleChunk;
    chunk->section = chunks.size() + 1;
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
/*
box_content.box_type_cid
18  EDTA_1(UK)  HPS2-THRIVE EDTA 1 UK samples
c_box_size.box_type_cid
Display the size of the job and ask user if they want to divide up the list.  If they do:
1.	Ask them the maximum section size (default = 500 cryovials)
2.	Calculate slot/box (where `c_box_size.box_size_cid = box_content.box_size_cid`)
3.	Ask them to select the size of first section from a list – it must be a multiple of the box size (from 2) and no more than the maximum (from 1)
4.	Allocate the appropriate number of destination boxes to the first section
5.	Repeat steps (2) and (3) until every entry has been allocated to a section
*/
}

void TfrmSamples::loadRows() {
    std::ostringstream oss; oss<<__FUNC__<<": numrows: "<<maxRows; debugLog(oss.str().c_str());
    panelLoading->Caption = loadingMessage;
    panelLoading->Visible = true; // appearing in wrong place because called in OnShow, form not yet maximized
    panelLoading->Top = (sgVials->Height / 2) - (panelLoading->Height / 2);
    panelLoading->Left = (sgVials->Width / 2) - (panelLoading->Width / 2);
    progressBottom->Style = pbstMarquee; progressBottom->Visible = true;
    Screen->Cursor = crSQLWait; // disable mouse?
    loadVialsWorkerThread = new LoadVialsWorkerThread();
    loadVialsWorkerThread->OnTerminate = &loadVialsWorkerThreadTerminated;
}

SampleChunk * TfrmSamples::currentChunk() {
    if (sgChunks->Row < 1) sgChunks->Row = 1; // force selection of 1st row
    SampleChunk * chunk = (SampleChunk *)sgChunks->Objects[0][sgChunks->Row];
    if (NULL == chunk) {// still null
        ostringstream oss; oss<<__FUNC__<<": Null chunk"; debugLog(oss.str().c_str());
        throw Exception("null chunk"); // msgbox("null chunk");
    }
    return chunk;
    //return (SampleChunk *)sgChunks->Objects[0][sgChunks->Row];
}

void TfrmSamples::showChunk(SampleChunk * chunk) {
    if (NULL == chunk) { // default
        chunk = currentChunk();
    }
    if (chunk->rows.size() <= 0) {
        clearSG(sgVials);
    } else {
        sgVials->RowCount = (-1 == maxRows) ? chunk->rows.size() + 1 : maxRows + 1;
        sgVials->FixedRows = 1;
    }
    int row = 1;
    for (vecpSampleRow::const_iterator it = chunk->rows.begin(); it != chunk->rows.end(); it++, row++) { // vecpDataRow?
        pSampleRow sampleRow = (pSampleRow)*it;
        LPDbCryovialStore * vial = sampleRow->store_record;
        sgVials->Cells[SGVIALS_BARCODE] [row]    = sampleRow->cryovial_barcode.c_str();
        sgVials->Cells[SGVIALS_DESTBOX] [row]    = "tba"; //sampleRow->;
        sgVials->Cells[SGVIALS_DESTPOS] [row]    = "tba"; //sampleRow->;
        sgVials->Cells[SGVIALS_CURRBOX] [row]    = sampleRow->box_name.c_str();
        sgVials->Cells[SGVIALS_CURRPOS] [row]    = "tba"; //sampleRow->;
        sgVials->Cells[SGVIALS_SITE]    [row]    = sampleRow->site_name.c_str();
        sgVials->Cells[SGVIALS_POSITION][row]    = sampleRow->position;
        sgVials->Cells[SGVIALS_SHELF]   [row]    = sampleRow->shelf_number;
        sgVials->Cells[SGVIALS_VESSEL]  [row]    = sampleRow->vessel_name.c_str();
        sgVials->Cells[SGVIALS_STRUCTURE][row]   = sampleRow->rack_name.c_str();
        sgVials->Cells[SGVIALS_SLOT]    [row]    = sampleRow->slot_position;
        sgVials->Objects[0][row] = (TObject *)sampleRow;
        if (-1 != maxRows && row >= maxRows) break;
    }
    ostringstream oss; oss<<((-1 == maxRows) ? vials.size() : maxRows)<<" of "<<vials.size()<<" vials";
    groupVials->Caption = oss.str().c_str();
}

void TfrmSamples::sortList(int col) {
    //partial_sort
    Screen->Cursor = crSQLWait;
    static Sorter<SampleRow> sorter[SGVIALS_NUMCOLS] = {
        { SampleRow::sort_asc_barcode,   SampleRow::sort_desc_barcode,  sgVialColName[0] },
        { SampleRow::sort_asc_destbox,   SampleRow::sort_desc_destbox,  sgVialColName[1] },
        { SampleRow::sort_asc_destpos,   SampleRow::sort_desc_destpos,  sgVialColName[2] },
        { SampleRow::sort_asc_currbox,   SampleRow::sort_desc_currbox,  sgVialColName[3] },
        { SampleRow::sort_asc_currpos,   SampleRow::sort_desc_currpos,  sgVialColName[4] },
        { SampleRow::sort_asc_site,      SampleRow::sort_desc_site,     sgVialColName[5] },
        { SampleRow::sort_asc_position,  SampleRow::sort_desc_position, sgVialColName[6] },
        { SampleRow::sort_asc_shelf,     SampleRow::sort_desc_shelf,    sgVialColName[7] },
        { SampleRow::sort_asc_vessel,    SampleRow::sort_desc_vessel,   sgVialColName[8] },
        { SampleRow::sort_asc_structure, SampleRow::sort_desc_structure,sgVialColName[9] },
        { SampleRow::sort_asc_slot,      SampleRow::sort_desc_slot,     sgVialColName[10] },
    };
    sorter[col].sort_toggle(currentChunk()->rows);
    showChunk(currentChunk());
    Screen->Cursor = crDefault;
}



