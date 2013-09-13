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

Sorter<SampleRow> sorter[SGVIALS_NUMCOLS] = {
    { SampleRow::sort_asc_barcode,   sgVialColName[0] },
    { SampleRow::sort_asc_destbox,   sgVialColName[1] },
    { SampleRow::sort_asc_destpos,   sgVialColName[2] },
    { SampleRow::sort_asc_currbox,   sgVialColName[3] },
    { SampleRow::sort_asc_currpos,   sgVialColName[4] },
    { SampleRow::sort_asc_site,      sgVialColName[5] },
    { SampleRow::sort_asc_position,  sgVialColName[6] },
    { SampleRow::sort_asc_shelf,     sgVialColName[7] },
    { SampleRow::sort_asc_vessel,    sgVialColName[8] },
    { SampleRow::sort_asc_structure, sgVialColName[9] },
    { SampleRow::sort_asc_slot,      sgVialColName[10] }
};

__fastcall LoadVialsWorkerThread::LoadVialsWorkerThread() : TThread(false) {
    FreeOnTerminate = true;
}

void __fastcall LoadVialsWorkerThread::updateStatus() { // can't use args for synced method, don't know why
    //ostringstream oss; oss<<frmSamples->loadingMessage<<"\n"<<rowCount<<" vials";//<<numerator<<" of "<<denominator;
    //frmSamples->panelLoading->Caption = oss.str().c_str();
    frmSamples->panelLoading->Caption = loadingMessage.c_str(); //oss.str().c_str();
    frmSamples->panelLoading->Repaint();
}

void __fastcall LoadVialsWorkerThread::Execute() {
    delete_referenced<vecpSampleRow>(frmSamples->vials);
    ostringstream oss; oss<<frmSamples->loadingMessage<<" (preparing query)";
    loadingMessage = oss.str().c_str();

/*
destination box and position,
cryovial barcode and current box,
position, structure and location
of the primary and secondary aliquots.
*/

/*    rowCount = 0;
    LQuery qd(Util::projectQuery(frmSamples->job->getProjectID(), true)); // ddb

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

     -- may have destination box defined, could find with left join, e.g.:
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
                s1.retrieval_cid = :jobID

        - but left join on ddb not allowed in ingres

    qd.setParam("jobID", frmSamples->job->getID());
    loadingMessage = frmSamples->loadingMessage; // base
    qd.open(); // most time - about 30 seconds - is taken opening the query. Cursoring through 1000+ rows takes 1-2 seconds
    while (!qd.eof()) {
        ostringstream oss; oss<<"Found "<<rowCount<<" vials";//<<numerator<<" of "<<denominator;
        loadingMessage = oss.str().c_str();
        if (0 == rowCount % 10) Synchronize((TThreadMethod)&updateStatus); // don't do graphical things in the thread without Synchronising
        LPDbCryovialStore * vial = new LPDbCryovialStore(qd); // not query per row - not inefficient?
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
    // then find destinations
    int rowCount2 = 0;
    for (vecpSampleRow::iterator it = frmSamples->vials.begin(); it != frmSamples->vials.end(); it++) { // vecpDataRow?
        ostringstream oss; oss<<"Finding destinations: "<<rowCount2<<"/"<<rowCount;
        loadingMessage = oss.str().c_str();
        if (0 == rowCount2 % 10) Synchronize((TThreadMethod)&updateStatus);
        pSampleRow sampleRow = (pSampleRow)*it;
        try {
            //findDestinationSlowly(sampleRow);
            findDestination(sampleRow);
        } catch(Exception & e) {
            msgbox(e.Message);
        } catch(...) {
            msgbox("error");
        }
        rowCount2++;
    }
    */

    rowCount = 0;
    LQuery qd(Util::projectQuery(frmSamples->job->getProjectID(), true)); // ddb
    qd.setSQL( // from spec 2013-09-11
        "SELECT"
        "  s1.cryovial_id, s1.note_exists, s1.retrieval_cid, s1.box_cid, s1.status, s1.cryovial_position,"
        "  cryovial_barcode,"
        "  b1.external_name as source_box,"
        "  s1.cryovial_position as source_pos,"
        "  b2.external_name as destination_box,"
        "  s2.cryovial_position as dest_pos"
        " FROM"
        "  cryovial_store s1, cryovial c, box_name b1, cryovial_store s2, box_name b2"
        " WHERE"
        "  c.cryovial_id = s1.cryovial_id AND"
        "  b1.box_cid = s1.box_cid AND"
        "  s1.cryovial_id = s2.cryovial_id AND"
        "  s2.status = 0 AND"
        "  b2.box_cid = s2.box_cid AND"
        "  s1.retrieval_cid = :jobID");
    qd.setParam("jobID", frmSamples->job->getID());
    loadingMessage = frmSamples->loadingMessage; // base
    qd.open(); // most time - about 30 seconds - is taken opening the query. Cursoring through 1000+ rows takes 1-2 seconds
    while (!qd.eof()) {
        ostringstream oss; oss<<"Found "<<rowCount<<" vials";//<<numerator<<" of "<<denominator;
        loadingMessage = oss.str().c_str();
        if (0 == rowCount % 10) Synchronize((TThreadMethod)&updateStatus); // don't do graphical things in the thread without Synchronising

        LPDbCryovialStore * vial = new LPDbCryovialStore(qd); // not query per row - not inefficient?
            // expects Cryovial_id, Note_Exists, retrieval_cid, box_cid, status, cryovial_position, cryovial_id
        pSampleRow  row = new SampleRow(
            vial,
            qd.readString("cryovial_barcode"),
            "", //qd.readString("aliquot"),
            qd.readString("source_box"),
            "", //qd.readString("site"),
            0, //qd.readInt("position"),
            "", //qd.readString("vessel"),
            0, //qd.readInt("shelf_number"),
            "", //qd.readString("rack"),
            0 //qd.readInt("slot_position")
            );
        frmSamples->vials.push_back(row);
        qd.next();
        rowCount++;
    }

/* suggested per-box query for finding where each box is stored:
(over ddb but not using left join) - could be cached

findBox( int box_id, int proj_id, ROSETTA & result )
    does this

Select
s.external_name as site,
m.position,
v.external_full as vessel,
shelf_number,
r.external_name as rack,
bs.slot_position
from
box_store bs,
c_rack_number r,
c_tank_map m,
c_object_name s,
c_object_name v
where
bs.status = 6 and
m.status =0 and
bs.rack_cid = r.rack_cid and
r.tank_cid = m.tank_cid and
s.object_cid = location_cid and
v.object_cid = storage_cid and
box_cid = :boxID;

*/

}

void LoadVialsWorkerThread::findDestination(pSampleRow row) {
/*
	std::map<int, const GridEntry *> boxes;
	ROSETTA result;
	StoreDAO dao;
	progress -> Max = rows.size();
	progress -> Position = 0;
	for( std::vector<GridEntry>::iterator ge = rows.begin(); ge != rows.end(); ++ ge ) {
		std::map<int, const GridEntry *>::const_iterator found = boxes.find( ge->bid );
		if( found != boxes.end() ) {
			ge->copyLocation( *(found->second) );
		} else {
			if( dao.findBox( ge->bid, LCDbProjects::getCurrentID(), result ) ) {
				ge->copyLocation( result );
			}
			boxes[ ge->bid ] = &(*ge);
		}
		progress -> StepIt();
		drawGrid();
		Application -> ProcessMessages();
	}*/

	static std::map<int, SampleRow *> boxes;
	ROSETTA result;
	StoreDAO dao;
	std::map<int, SampleRow *>::const_iterator found = boxes.find(row->store_record->getBoxID());
    if (found != boxes.end()) {
        row->dest_box_id      = (*(found->second)).dest_box_id;
        row->dest_box_name    = (*(found->second)).box_name;
        row->dest_box_pos     = (*(found->second)).dest_box_pos;
    } else {
        //if (dao.findBox(row->store_record->getBoxID(), LCDbProjects::getCurrentID(), result)) { finds t/r/s | v/s/s
        dao.loadBoxDetails(row->store_record->getBoxID(), LCDbProjects::getCurrentID(), result);
        row->dest_box_id      = result.getInt("box_cid");
        row->dest_box_name    = result.getString("external_name");
        row->dest_box_pos     = result.getInt("slot_position");
        //} // isn't this just getting details of the current box, not the destination?
        boxes[row->dest_box_id] = &(*row);
    }
}

void LoadVialsWorkerThread::findDestinationSlowly(pSampleRow sampleRow) {
    // look for destination boxes, can't left join in ddb, so do project query per row
    // may be v time-consuming - could do outer join instead and check sequence for gaps
    // p.s. *is* v time-consuming...
    //look in the right database
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
    if (qp.open()) {
        sampleRow->dest_box_id      = qp.readInt("boxid");
        sampleRow->dest_box_name    = qp.readString("boxname");
        sampleRow->dest_box_pos     = qp.readInt("pos");
    } else {
        sampleRow->dest_box_id      = 0;
        sampleRow->dest_box_name    = "";
        sampleRow->dest_box_pos     = 0;
    }
}

__fastcall TfrmSamples::TfrmSamples(TComponent* Owner) : TForm(Owner) { }

void TfrmSamples::debugLog(String s) {
    frmSamples->memoDebug->Lines->Add(s); // could use varargs: http://stackoverflow.com/questions/1657883/variable-number-of-arguments-in-c
}

void __fastcall TfrmSamples::FormCreate(TObject *Sender) {
    cbLog->Visible      = RETRASSTDEBUG;
    //maxRows             = DEFAULT_NUMROWS;
    job                 = NULL;
    setupStringGrid(sgChunks, SGCHUNKS_NUMCOLS, sgChunksColName, sgChunksColWidth);
    setupStringGrid(sgVials, SGVIALS_NUMCOLS, sgVialColName, sgVialColWidth);
    loadingMessage = "Loading samples, please wait...";
}

void __fastcall TfrmSamples::FormShow(TObject *Sender) {
    btnSave->Enabled = true;
    chunks.clear();
    clearSG(sgChunks);
    clearSG(sgVials);
    timerLoadVials->Enabled = true;
    //if (IDYES == Application->MessageBox(L"Do you want to automatically create chunks for this list?", L"Question", MB_YESNO)) {autoChunk();}
}

void __fastcall TfrmSamples::FormClose(TObject *Sender, TCloseAction &Action) {
    delete_referenced<vecpSampleRow>(frmSamples->vials);
    delete_referenced<vecpSampleChunk>(chunks);
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
            for (vecpSampleRow::const_iterator it = chunk->rows.begin(); it != chunk->rows.end(); it++) { // vecpDataRow?
                pSampleRow sampleRow = (pSampleRow)*it;
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
}

void __fastcall TfrmSamples::btnDelChunkClick(TObject *Sender) {
    if (RETRASSTDEBUG || IDYES == Application->MessageBox(L"Are you sure you want to delete the last chunk?", L"Question", MB_YESNO)) {
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
        //else if (chunk->
    }
    TCanvas * cnv = sgVials->Canvas;
	cnv->Brush->Color = background;
	cnv->FillRect(Rect);
    if (State.Contains(gdSelected)) {
        TFontStyles oldFontStyle = cnv->Font->Style;
        TPenStyle oldPenStyle = cnv->Pen->Style;
        cnv->Pen->Style = psDot;
        cnv->Rectangle(Rect.Left+1, Rect.Top+1, Rect.Right-1, Rect.Bottom-1);
        cnv->Font->Style = TFontStyles() << fsBold;
    	cnv->TextOut(Rect.Left+5, Rect.Top+5, sgVials->Cells[ACol][ARow]);
        cnv->Pen->Style     = oldPenStyle;
        cnv->Font->Style    = oldFontStyle;
	} else {
        cnv->TextOut(Rect.Left+5, Rect.Top+5, sgVials->Cells[ACol][ARow]);
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
    //ShowCursor(true);
    Enabled = true;
}

void __fastcall TfrmSamples::sgChunksClick(TObject *Sender) {
    showChunk(); // default is 1st
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
    sortChunk(currentChunk(), ACol, Sorter<SampleRow *>::TOGGLE);
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

void TfrmSamples::loadRows() {
    panelLoading->Caption = loadingMessage;
    panelLoading->Visible = true; // appearing in wrong place because called in OnShow, form not yet maximized
    panelLoading->Top = (sgVials->Height / 2) - (panelLoading->Height / 2);
    panelLoading->Left = (sgVials->Width / 2) - (panelLoading->Width / 2);
    progressBottom->Style = pbstMarquee; progressBottom->Visible = true;
    Screen->Cursor = crSQLWait; // disable mouse?
    //ShowCursor(false);
    Enabled = false;
    loadVialsWorkerThread = new LoadVialsWorkerThread();
    loadVialsWorkerThread->OnTerminate = &loadVialsWorkerThreadTerminated;
}

void TfrmSamples::showChunks() {
    if (0 == chunks.size()) { // must always have one chunk anyway
        throw Exception("No chunks");
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
        clearSG(sgVials);
    } else {
        sgVials->RowCount = chunk->rows.size();
        sgVials->FixedRows = 1;
    }
    int row = 1;
    for (vecpSampleRow::const_iterator it = chunk->rows.begin(); it != chunk->rows.end(); it++, row++) { // vecpDataRow?
        pSampleRow sampleRow = (pSampleRow)*it;
        LPDbCryovialStore * vial = sampleRow->store_record;
        sgVials->Cells[SGVIALS_BARCODE] [row]    = sampleRow->cryovial_barcode.c_str();
        sgVials->Cells[SGVIALS_DESTBOX] [row]    = sampleRow->dest_box_name.c_str();
        sgVials->Cells[SGVIALS_DESTPOS] [row]    = sampleRow->dest_box_pos;
        sgVials->Cells[SGVIALS_CURRBOX] [row]    = sampleRow->box_name.c_str();
        sgVials->Cells[SGVIALS_CURRPOS] [row]    = sampleRow->slot_position;
        sgVials->Cells[SGVIALS_SITE]    [row]    = sampleRow->site_name.c_str();
        sgVials->Cells[SGVIALS_POSITION][row]    = sampleRow->position;
        sgVials->Cells[SGVIALS_SHELF]   [row]    = sampleRow->shelf_number;
        sgVials->Cells[SGVIALS_VESSEL]  [row]    = sampleRow->vessel_name.c_str();
        sgVials->Cells[SGVIALS_STRUCTURE][row]   = sampleRow->rack_name.c_str();
        sgVials->Cells[SGVIALS_SLOT]    [row]    = sampleRow->slot_position;
        sgVials->Objects[0][row] = (TObject *)sampleRow;
    }
    //groupVials->Caption = oss.str().c_str();
}

void TfrmSamples::addSorter() {
    ostringstream oss; oss << __FUNC__ << groupSort->ControlCount; debugLog(oss.str().c_str());
    TComboBox * combo = new TComboBox(this);
    //groupSort->InsertControl(combo); // don't call this directly, set the parent as above
    combo->Parent = groupSort; // new combo is last created, aligned to left
        // to put in right order: take them all out, sort and put back in in reverse order?
    combo->Align = alLeft;
    combo->Style = csDropDown; // csDropDownList
    for (int i=0; i<SGVIALS_NUMCOLS; i++) {
        combo->AddItem(sorter[i].description.c_str(), (TObject *)&sorter[i]);
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
                sortChunk(chunk, combo->ItemIndex, Sorter<SampleRow *>::ASCENDING);
            }
        } else {
            debugLog("not a combo box, finish sorting.");
            break; // finished sorting
        }
    }
}

void TfrmSamples::sortChunk(SampleChunk * chunk, int col, Sorter<SampleRow *>::SortOrder order) {
    Screen->Cursor = crSQLWait;
    switch (order) {
        case Sorter<SampleRow *>::ASCENDING:
            sorter[col].sort_asc(chunk->rows);
            break;
        case Sorter<SampleRow *>::DESCENDING:
            sorter[col].sort_dsc(chunk->rows);
            break;
        case Sorter<SampleRow *>::TOGGLE:
            sorter[col].sort_toggle(chunk->rows);
            break;
    }
    //sorter[col].sort_toggle(chunk->rows); //partial_sort
    showChunk(chunk);
    Screen->Cursor = crDefault;
}

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

