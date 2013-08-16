#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistantSamples.h"
#include "StoreUtil.h"
#include "LCDbRack.h"
#include "RetrievalAssistantAutoChunk.h"
//#include "ReferredBoxes.h"
    // for MYDEBUG - should move somewhere else
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmSamples *frmSamples;

void TfrmSamples::debugLog(String s) {
    frmSamples->memoDebug->Lines->Add(s);
}

__fastcall LoadVialsWorkerThread::LoadVialsWorkerThread() : TThread(false) {
    FreeOnTerminate = true;
}

//void __fastcall LoadVialsWorkerThread::updateStatus(int numerator, int denominator) {
void __fastcall LoadVialsWorkerThread::updateStatus() {
    //ostringstream oss; oss<<frmSamples->loadingMessage<<"\n"<<numerator<<" of "<<denominator;
    ostringstream oss; oss<<frmSamples->loadingMessage<<"\n"<<rowCount<<" vials";//<<denominator;
    frmSamples->panelLoading->Caption = oss.str().c_str();
    frmSamples->panelLoading->Repaint();
}

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

__fastcall TfrmSamples::TfrmSamples(TComponent* Owner) : TForm(Owner) { }

void __fastcall TfrmSamples::FormCreate(TObject *Sender) {
    cbLog->Visible      = RETRASSTDEBUG;
    //memoDebug->Visible  = RETRASSTDEBUG;
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
    addChunk();
    showChunks();
    clearSG(sgVials);
    //loadRows();
    timerLoadVials->Enabled = true;
    //showRows();
    //progressBottom->Visible = false;
    //if (IDYES == Application->MessageBox(L"Do you want to automatically create chunks for this list?", L"Question", MB_YESNO)) {autoChunk();}
}

void __fastcall TfrmSamples::btnCancelClick(TObject *Sender) { Close(); }

void __fastcall TfrmSamples::btnSaveClick(TObject *Sender) {
    if (IDYES == Application->MessageBox(L"Save changes? Press 'No' to go back and re-order", L"Question", MB_YESNO)) {
        // save stuff
        // ie, create the retrieval plan by inserting into c_box_retrieval and l_sample_retrieval
        Close();
    } else {
        // delete chunks, start again
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

void TfrmSamples::addChunk() {
    if (chunks.size() == 0) {
        // first chunk, make default chunk from entire list
    }
    Chunk * chunk = new Chunk;
    chunk->section = chunks.size() + 1;
    chunks.push_back(chunk);
    btnDelChunk->Enabled = true;
    showChunks();
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
    if (chunks.size() == 0) btnDelChunk->Enabled = false;
}

void __fastcall TfrmSamples::radbutDefaultClick(TObject *Sender) { radgrpRowsChange(); }
void __fastcall TfrmSamples::radbutAllClick(TObject *Sender) { radgrpRowsChange(); }
void __fastcall TfrmSamples::radbutCustomClick(TObject *Sender) { radgrpRowsChange(); }

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
    showRows();
}

void __fastcall TfrmSamples::timerCustomRowsTimer(TObject *Sender) {
    std::ostringstream oss; oss <<__FUNC__<<": load"<<": numrows: "<<maxRows; debugLog(oss.str().c_str());
    timerCustomRows->Enabled = false;
    maxRows = editCustomRows->Text.ToIntDef(0);
    showRows();
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
        Chunk * chunk = NULL;
        chunk = (Chunk *)sgChunks->Objects[0][ARow];
        background = RETRIEVAL_ASSISTANT_DONE_COLOUR; //break;
        if (NULL == chunk) {
            background = RETRIEVAL_ASSISTANT_DONE_COLOUR;
            //background = RETRIEVAL_ASSISTANT_ERROR_COLOUR;
        } else {
            background = RETRIEVAL_ASSISTANT_DONE_COLOUR;
            //background = RETRIEVAL_ASSISTANT_ERROR_COLOUR;
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

void TfrmSamples::showChunks() {
    if (0 == chunks.size()) { // must always have one chunk anyway
        clearSG(sgChunks);
    } else {
        sgChunks->RowCount = chunks.size() + 1;
        sgChunks->FixedRows = 1; // "Fixed row count must be LESS than row count"
    }
    vecpChunk::const_iterator it;
    int row = 1;
    for (it = chunks.begin(); it != chunks.end(); it++, row++) {
        Chunk * chunk = *it;
        sgChunks->Cells[SGCHUNKS_SECTION]   [row] = chunk->section;
        sgChunks->Cells[SGCHUNKS_START]     [row] = chunk->start;
        sgChunks->Cells[SGCHUNKS_END]       [row] = chunk->end;
        sgChunks->Cells[SGCHUNKS_SIZE]      [row] = chunk->end - chunk->start;
        sgChunks->Objects[0][row] = (TObject *)chunk;
    }
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

/*
    //"SELECT br.box_id FROM c_box_retrieval br WHERE br.retrieval_cid = :rtid AND br.section = :sect AND status != 99");
//    // no 'chunks' yet, we haven't created them!
//    // they will exist in c_box_retrieval, but don't already exist in cryovial_store where the job comes from
//    q.setSQL("SELECT * FROM c_retrieval_job rj, cryovial_store cs WHERE rj.retrieval_cid = cs.retrieval_cid ORDER BY cs.box_cid");
*/

void __fastcall  LoadVialsWorkerThread::Execute() {
    // do stuff
    delete_referenced<vecpSampleRow>(frmSamples->vials);
    LQuery q(Util::projectQuery(frmSamples->job->getProjectID(), true));
    q.setSQL(
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
    q.setParam("jobID", frmSamples->job->getID());
    /* -- may have destination box defined, could find with left join:
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

    //int count = 0;
    //return;
    q.open(); // most time - about 30 seconds - is taken opening the query. Cursoring through 1000+ rows takes 1-2 seconds
    while (!q.eof()) {
        LPDbCryovialStore * vial = new LPDbCryovialStore(q);
        pSampleRow  row = new SampleRow(
            vial,
            q.readString("cryovial_barcode"),
            q.readString("aliquot"),
            q.readString("box"),
            q.readString("site"),
            q.readInt("position"),
            q.readString("vessel"),
            q.readInt("shelf_number"),
            q.readString("rack"),
            q.readInt("slot_position")
            );
        frmSamples->vials.push_back(row);
        q.next();
        rowCount++;
        if (0 == rowCount % 10) Synchronize((TThreadMethod)&updateStatus); // don't do graphical things in the thread without Synchronising
            // can't use args for synced method, don't know why
    }
}

void __fastcall TfrmSamples::loadVialsWorkerThreadTerminated(TObject *Sender) {
    // finish up
    showRows(); // must do this outside thread, unless synchronised - does gui stuff
    progressBottom->Style = pbstNormal; progressBottom->Visible = false;
    panelLoading->Visible = false;
    Screen->Cursor = crDefault;
}

/* SampleRow(  LPDbCryovialStore * store_rec, string barcode, string aliquot, string box,
                string site, int pos, string vessel, int shelf, string rack, int slot) :
        "   c.cryovial_barcode, t.external_name AS aliquot, b.external_name AS box,"
        "   s.external_name AS site, m.position, v.external_full AS vessel,"
        "   shelf_number, r.external_name AS rack, bs.slot_position" */
    /*  LPDbCryovialStore * store_record;
        std::string     cryovial_barcode;
        std::string     aliquote_type_name;
        std::string     box_name;
        std::string     site_name;
        int             position;
        std::string     vessel_name;
        int             shelf_number;
        std::string     rack_name;
        int             slot_position;*/

void TfrmSamples::loadRows() {
    std::ostringstream oss; oss<<__FUNC__<<": numrows: "<<maxRows; debugLog(oss.str().c_str());
    panelLoading->Caption = loadingMessage;
    panelLoading->Visible = true; // appearing in wrong place because called in OnShow, form not yet maximized
    panelLoading->Top = (sgVials->Height / 2) - (panelLoading->Height / 2);
    panelLoading->Left = (sgVials->Width / 2) - (panelLoading->Width / 2);
    progressBottom->Style = pbstMarquee; progressBottom->Visible = true;
    // Screen-> // disable mouse?
    Screen->Cursor = crSQLWait;
    //Repaint();
    loadVialsWorkerThread = new LoadVialsWorkerThread();
    loadVialsWorkerThread->OnTerminate = &loadVialsWorkerThreadTerminated;
}

/* SampleRow(  LPDbCryovialStore * store_rec, string barcode, string aliquot, string box,
               string site, int pos, string vessel, int shelf, string rack, int slot) :  */
void TfrmSamples::showRows() {
    if (vials.size() <= 0) {
        clearSG(sgVials);
    } else {
        sgVials->RowCount = (-1 == maxRows) ? vials.size() + 1 : maxRows + 1;
        sgVials->FixedRows = 1;
    }
    int row = 1;
    vecpSampleRow::const_iterator it;
    for (it = vials.begin(); it != vials.end(); it++, row++) {
        pSampleRow sampleRow = *it;
        LPDbCryovialStore * vial = sampleRow->store_record;
        sgVials->Cells[SGVIALS_BARCODE] [row]    = sampleRow->cryovial_barcode.c_str();
        sgVials->Cells[SGVIALS_DESTBOX] [row]    = "tba"; //sampleRow->;
        sgVials->Cells[SGVIALS_DESTPOS] [row]    = "tba"; //sampleRow->;
        sgVials->Cells[SGVIALS_CURRBOX] [row]    = sampleRow->box_name.c_str();
        sgVials->Cells[SGVIALS_CURRPOS] [row]    = "tba"; //sampleRow->;
        sgVials->Cells[SGVIALS_SITE]    [row]    = sampleRow->site_name.c_str(); // Russian doll order
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
    ostringstream oss; oss << __FUNC__; oss << printColWidths(sgVials); // print column widths so we can copy them into the source
    debugLog(oss.str().c_str());
    sortList(ACol);
}

void __fastcall TfrmSamples::sgVialsClick(TObject *Sender) {
    // ostringstream oss; oss << __FUNC__; oss << printColWidths(sgVials); debugLog(oss.str().c_str());
    SampleRow * sample  = (SampleRow *)sgVials->Objects[0][sgVials->Row];
    if (NULL != sample) {
        debugLog(sample->str().c_str());
    }
}

void TfrmSamples::sortList(int col) {
    static bool sort_desc[SGVIALS_NUMCOLS]; // reverse sort toggle
    //using namespace SampleRow;
    //partial_sort

/*
sort box names properly:

alpha sort:
SEARCH Buffy 1
SEARCH Buffy 103
SEARCH Buffy 109
SEARCH Buffy 11

should be:

SEARCH Buffy 1
SEARCH Buffy 2
SEARCH Buffy 3
etc.

search func: strip out numeric chars from name, concatenate, compare as ints


*/
    //
    bool (*sort_func)(const SampleRow *, const SampleRow *);
    switch (col) {

    case SGVIALS_BARCODE:   sort_func = sort_desc[col] ? SampleRow::sort_asc_barcode : SampleRow::sort_desc_barcode;   break;
    //case SGVIALS_DESTBOX:   sort_func = SampleRow::less_than_; break;
    //case SGVIALS_DESTPOS:   sort_func = SampleRow::less_than_; break;
    case SGVIALS_CURRBOX:   sort_func = sort_desc[col] ? SampleRow::sort_asc_currbox : SampleRow::sort_desc_currbox;   break;
    //case SGVIALS_CURRPOS:   sort_func = SampleRow::less_than_; break;
    case SGVIALS_SITE:      sort_func = sort_desc[col] ? SampleRow::sort_asc_site : SampleRow::sort_desc_site;      break;
    case SGVIALS_POSITION:  sort_func = sort_desc[col] ? SampleRow::sort_asc_position : SampleRow::sort_desc_position;  break;
    case SGVIALS_SHELF:     sort_func = sort_desc[col] ? SampleRow::sort_asc_shelf : SampleRow::sort_desc_shelf;     break;
    case SGVIALS_VESSEL:    sort_func = sort_desc[col] ? SampleRow::sort_asc_vessel : SampleRow::sort_desc_vessel;    break;
    case SGVIALS_STRUCTURE: sort_func = sort_desc[col] ? SampleRow::sort_asc_structure : SampleRow::sort_desc_structure; break;
    case SGVIALS_SLOT:      sort_func = sort_desc[col] ? SampleRow::sort_asc_slot : SampleRow::sort_desc_slot;      break;
    default:
        return;
        //throw Exception("Unknown sortType");
    }
    sort_desc[col] = !sort_desc[col]; // toggle sort order
    std::sort(vials.begin(), vials.end(), sort_func);
    showRows();
}

void __fastcall TfrmSamples::timerLoadVialsTimer(TObject *Sender) {
    timerLoadVials->Enabled = false;
    loadRows(); // so that gui can be updated
}

void __fastcall TfrmSamples::btnRejectClick(TObject *Sender) {
    if (IDYES == Application->MessageBox(L"Are you sure you want to reject this list?", L"Question", MB_YESNO)) {
        //autoChunk();
        //rejectList();
        Close();
    }
}

void __fastcall TfrmSamples::btnAddSortClick(TObject *Sender) {
    TComboBox * combo = new TComboBox(this);
    combo->Parent = groupSort;
    combo->Align = alLeft;
    // new combo is last created,
    //groupSort->InsertControl(combo);
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


