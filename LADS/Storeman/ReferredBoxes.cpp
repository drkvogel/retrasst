#include <vcl.h>
#include <Windows.h>
#include <process.h>
#pragma hdrstop
#include "ReferredBoxes.h"
#include "TfrmConfirm.h"
#include "LCDbJob.h"
#include "LCDbObject.h"
#include "ReferredBoxesSummary.h"
#include "SMLogin.h"
#include "LCDbTankMap.h"
#include "StoreDAO.h"
#include "LCDbRack.h"
#include "LCDbTankLayout.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

void TfrmReferred::debugLog(String s) { memoDebug->Lines->Add(s); }

void clearGridSelection(TStringGrid * sg) {
    TGridRect myRect;
    myRect.Left = 0; myRect.Top = 0; myRect.Right = 0; myRect.Bottom = 0;
    sg->Selection = myRect;
}

void clearSG(TStringGrid * sg) {
    clearGridSelection(sg);
    sg->FixedRows = 0;
    sg->RowCount = 0;
    sg->RowCount = 2;
    sg->FixedRows = 1;
    for (int i = 0; i < sg->ColCount; i++) {
        sg->Cells[i][1] = "";
        sg->Objects[i][1] = NULL;
    }
    sg->Cells[0][1] = "No results.";
}

string BoxArrivalRecord::typeFromName() const {
    return box_name.substr(0, box_name.find_last_of(" _-"));
}

const short statusToIndex(const short status) {
    return status; //xxx 1 to 1 for now
}

const short indexToStatus(const short index) {
    return index; //xxx 1 to 1 for now
}

TfrmReferred *frmReferred;

__fastcall TfrmReferred::TfrmReferred(TComponent* Owner) : TForm(Owner) {}

void __fastcall TfrmReferred::FormClose(TObject *Sender, TCloseAction &Action) {
    sgReferredBoxes->RowCount = 1;
    sgMatches->RowCount = 1;
    sgStorage->RowCount = 1;
    delete_referenced<tdvecpBoxArrivalRecord>(totalReferred);
    delete_referenced<tdvecpBoxArrivalRecord>(storageHistory);
    delete_referenced<tdvecpBoxArrivalRecord>(matchingBoxes1);
    delete_referenced<tdvecpBoxArrivalRecord>(matchingBoxes2);
}

__fastcall FindMatchesWorkerThread::FindMatchesWorkerThread(int project, string boxname, string first, string last) : TThread(false), project(project), boxname(boxname), first(first), last(last) {
    FreeOnTerminate = true;
}

__fastcall FindStorageWorkerThread::FindStorageWorkerThread(int project, int box) : TThread(false), project(project), box_cid(box) {
    FreeOnTerminate = true;
}

__fastcall CheckTRSWorkerThread::CheckTRSWorkerThread(BoxArrivalRecord _box) : TThread(false), box(_box) {
    FreeOnTerminate = true;
}

void __fastcall TfrmReferred::btnDoneClick(TObject *Sender) { Close(); }

void __fastcall TfrmReferred::FormShow(TObject *Sender) { init(); }

void __fastcall TfrmReferred::cbLogClick(TObject *Sender) {
    memoDebug->Visible = cbLog->Checked;
}

void TfrmReferred::init() {
    // referred boxes: l_box_arrival
    sgReferredBoxes->ColWidths[0]  = 200;  // Name
    sgReferredBoxes->ColWidths[1]  = 120;  // Status
    sgReferredBoxes->ColWidths[2]  = 30;   // 1stpos
    sgReferredBoxes->ColWidths[3]  = 70;   // 1stID
    sgReferredBoxes->ColWidths[4]  = 30;   // Lstpos
    sgReferredBoxes->ColWidths[5]  = 70;   // LstID
    sgReferredBoxes->ColWidths[6]  = 200;  // Tank/Population/Vessel
    sgReferredBoxes->ColWidths[7]  = 60;   // Rack
    sgReferredBoxes->ColWidths[8]  = 25;   // Slot
    sgReferredBoxes->ColWidths[9]  = 100;  // Project
    sgReferredBoxes->ColWidths[10] = 140;  // Swiped
    sgReferredBoxes->Cells[0] [0] = "Name"; // col, row
    sgReferredBoxes->Cells[1] [0] = "Status";
    sgReferredBoxes->Cells[2] [0] = "1st";
    sgReferredBoxes->Cells[3] [0] = "Barcode";
    sgReferredBoxes->Cells[4] [0] = "Lst";
    sgReferredBoxes->Cells[5] [0] = "Barcode";
    sgReferredBoxes->Cells[6] [0] = "Vessel";
    sgReferredBoxes->Cells[7] [0] = "Struct";
    sgReferredBoxes->Cells[8] [0] = "Slot";
    sgReferredBoxes->Cells[9] [0] = "Project";
    sgReferredBoxes->Cells[10][0] = "Swiped";

    // matched boxes in project database: box_name
    sgMatches->ColWidths[0] = 200;      // name
    sgMatches->ColWidths[1] = 140;      // status
    sgMatches->ColWidths[2] = 40;       // first pos
    sgMatches->ColWidths[3] = 100;      // first barc
    sgMatches->ColWidths[4] = 40;       // last pos
    sgMatches->ColWidths[5] = 100;      // last barc
    sgMatches->ColWidths[6] = 40;      // storage history exists
    //sgMatches->ColWidths[7] = 140;// box2->box_store_status
    sgMatches->Cells[0] [0] = "Name";
    sgMatches->Cells[1] [0] = "Status";
    sgMatches->Cells[2] [0] = "1st";
    sgMatches->Cells[3] [0] = "Barcode";
    sgMatches->Cells[4] [0] = "Lst";
    sgMatches->Cells[5] [0] = "Barcode";
    sgMatches->Cells[6] [0] = "Hist";
    //sgMatches->Cells[7] [0] = "StoreStatus";

    // box storage history: box_store
    sgStorage->ColWidths[0] = 150;      // time
    sgStorage->ColWidths[1] = 200;      // tank
    sgStorage->ColWidths[2] = 70;       // rank
    sgStorage->ColWidths[3] = 30;       // slot
    sgStorage->ColWidths[4] = 140;      // status
    sgStorage->Cells[0] [0] = "Timestamp";
    sgStorage->Cells[1] [0] = "Vessel";
    sgStorage->Cells[2] [0] = "Struct";
    sgStorage->Cells[3] [0] = "Slot";
    sgStorage->Cells[4] [0] = "Status";
    sgStorage->Cells[5] [0] = "Removed";

    // XXX boxes in l_box_arrival are for real projects not dev_<project>
    for (Range<LCDbProject> pr = LCDbProjects::records(); pr.isValid(); ++pr) {
		if (pr->isValid() && !pr->isCentral()) {
			listboxProjects->Items->AddObject(pr->getName().c_str(), (TObject *)pr->getID());
        }
    }
    clearGridSelection(sgReferredBoxes);
    clearGridSelection(sgMatches);
    clearGridSelection(sgStorage);
    loadBoxes();
    showBoxes();
    if (MYDEBUG) cbLog->Visible = true;
}

void __fastcall TfrmReferred::sgReferredBoxesMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    if (sgReferredBoxes->Row < 1) { return; }
    timerReferredBoxClicked->Enabled = false; // reset
    timerReferredBoxClicked->Enabled = true;
    referredBox = (BoxArrivalRecord *)sgReferredBoxes->Objects[0][sgReferredBoxes->Row];
    if (referredBox->changed) { // reset
        referredBox->status = LCDbBoxStore::Status::REFERRED;
        referredBox->changed = false;
        showBoxes();
    }
    selectedMatch = NULL;
    Screen->Cursor = crSQLWait;
    progressBottom->Style = pbstMarquee;
    clearSG(sgMatches);
    clearSG(sgStorage);
    setBoxDetails(referredBox);
    setTRS(referredBox);
    debugLog(referredBox->str().c_str());
}

void TfrmReferred::setBoxDetails(BoxArrivalRecord * box) {
    editBoxName->Text = box->box_name.c_str();
    editFirstPos->Text = box->first_position;
    editLastPos->Text = box->last_position;
    editFirstID->Text = box->first_barcode.c_str();
    editLastID->Text = box->last_barcode.c_str();
    comboStatus->Clear();
    for (int i = 0; i < 8; i++) { // xxx
        comboStatus->AddItem(boxStoreStatusStrings[i], NULL);
    } comboStatus->ItemIndex = statusToIndex(box->status);
}

void TfrmReferred::setTRS(BoxArrivalRecord * box) {
    comboTank->Clear();
    comboTank->AddItem(box->tank_name.c_str(), (TObject *)box->tank_cid);
    comboTank->ItemIndex = 0;
    comboRack->Clear();
    comboRack->AddItem(box->rack_name.c_str(), (TObject *)0); //xxx(LCDbRack::records().findByName(box.rack_name).getID()));
    comboRack->ItemIndex = 0;
    editSlot->Text = box->slot_position;
    //editSlot->Text = n2s(box->slot_position).c_str();
}

void __fastcall TfrmReferred::timerReferredBoxClickedTimer(TObject *Sender) {
    timerReferredBoxClicked->Enabled = false;
    if (NULL == findMatchesWorkerThread || 1 == findMatchesWorkerThread->WaitFor()) { // not already searching
        findMatches(false);
    }
}

void __fastcall TfrmReferred::editBoxNameChange(TObject *Sender) {
    if (editBoxName->Focused()) {
        Screen->Cursor = crSQLWait;
        progressBottom->Style = pbstMarquee; // seems to be need to kick off animation
        timerBoxNameEdited->Enabled = false; // reset
        timerBoxNameEdited->Enabled = true;
    }
}

void __fastcall TfrmReferred::timerBoxNameEditedTimer(TObject *Sender) {
    timerBoxNameEdited->Enabled = false;
    findMatchesByBoxName();
}

void __fastcall TfrmReferred::editFirstIDChange(TObject *Sender) {
    if (editFirstID->Focused()) {
        timerBarcodeEdited->Enabled = false; // reset
        timerBarcodeEdited->Enabled = true;
    }
}

void __fastcall TfrmReferred::editLastIDChange(TObject *Sender) {
    if (editLastID->Focused()) {
        Screen->Cursor = crSQLWait;
        progressBottom->Style = pbstMarquee; // seems to be need to kick off animation
        timerBarcodeEdited->Enabled = false; // reset
        timerBarcodeEdited->Enabled = true;
    }
}

void __fastcall TfrmReferred::timerBarcodeEditedTimer(TObject *Sender) {
    timerBarcodeEdited->Enabled = false;
    findMatchesByBarcode();
}

void __fastcall TfrmReferred::listboxProjectsClick(TObject *Sender) {
    showBoxesByProject();
}

void __fastcall TfrmReferred::listboxBoxTypesClick(TObject *Sender) {
    filterBoxesByBoxType();
}

void __fastcall TfrmReferred::sgMatchesMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    // use instead of OnSelectCell, which is triggered 3 times per click...
    int row, col; sgMatches->MouseToCell(X, Y, col, row);
    if (row < 1) return;
    selectedMatch = (BoxArrivalRecord *)sgMatches->Objects[0][row];
    if (NULL == selectedMatch) return; // throw "NULL == selectedMatch";
    debugLog(selectedMatch->str().c_str());
    Screen->Cursor = crSQLWait;
    setBoxDetails(selectedMatch);
    setTRS(referredBox); // so TRS can be reset to l_b_a value if selected store record(s) are not correct
    btnSaveBox->Enabled = true;
    sgStorage->RowCount = 1;
    progressBottom->Style = pbstMarquee;
    ostringstream ss; ss << "Searching for storage history of " << selectedMatch->box_name;
    updateStatusBar(ss.str());
    if (NULL == findStorageWorkerThread || 1 == findStorageWorkerThread->WaitFor()) {
        findStorageWorkerThread = new FindStorageWorkerThread(selectedMatch->project_cid, selectedMatch->box_arrival_id);
        findStorageWorkerThread->OnTerminate = &findStorageWorkerThreadTerminated;
    } else {
        debugLog("!(NULL == findStorageWorkerThread || 1 == findStorageWorkerThread->WaitFor())");
    }
}

void __fastcall TfrmReferred::btnSaveBoxClick(TObject *Sender) {
	okOrDiscard(LPDbBoxName::Status::IN_TANK);
}

void __fastcall TfrmReferred::btnDiscardClick(TObject *Sender) {
    okOrDiscard(LPDbBoxName::Status::DELETED);
}

void __fastcall TfrmReferred::btnSignOffClick(TObject *Sender) {
    signOffBoxes();
}

void __fastcall TfrmReferred::groupboxBoxTypesClick(TObject *Sender) {
    listboxBoxTypes->ClearSelection();
    showBoxes();
}

void __fastcall TfrmReferred::groupboxProjectsClick(TObject *Sender) {
    listboxProjects->ClearSelection();
    showBoxes();
}

void __fastcall TfrmReferred::sgReferredBoxesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
	TColor background = clWindow;
    BoxArrivalRecord * drawnBox = (BoxArrivalRecord *)sgReferredBoxes->Objects[0][ARow];
    if (0 == ARow) {
        background = clBtnFace;
    } else if (State.Contains(gdSelected)) { // && sgReferredBoxes->Focused()) {
		background = clActiveCaption; //clHighlight;
	} else if (NULL == drawnBox) {
        background = clWindow;
	} else {
        switch (drawnBox->status) {
        case LPDbBoxName::Status::IN_TANK:          // 4
            background = REFERRED_BOXES_IN_TANK_COLOUR;
            break;
        case LCDbBoxStore::Status::SLOT_ALLOCATED:  // 5
            background = REFERRED_BOXES_ALLOCATED_COLOUR;
            break;
        case LCDbBoxStore::Status::SLOT_CONFIRMED:  // 6
            background = REFERRED_BOXES_CONFIRMED_COLOUR;
            break;
        case LCDbBoxStore::Status::REFERRED:        // 7
            background = REFERRED_BOXES_REFFERED_COLOUR;
            break;
        case LCDbBoxStore::Status::EXPECTED:        // 0
        case LCDbBoxStore::Status::UNCONFIRMED:     // 1
        case LPDbBoxName::Status::CONFIRMED:        // 2
        case LCDbBoxStore::Status::REMOVED:         // 3
        case LCDbBoxStore::Status::DELETED:         // 99
        default:
            background = REFERRED_BOXES_ERROR_COLOUR;
        }
	}
	TCanvas * cnv = sgReferredBoxes->Canvas;
	cnv->Brush->Color = background;
	cnv->FillRect(Rect);
	cnv->TextOut(Rect.Left+5, Rect.Top+5, sgReferredBoxes->Cells[ACol][ARow]);
}

void __fastcall TfrmReferred::sgMatchesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
    TColor background;
    BoxArrivalRecord * drawnBox = (BoxArrivalRecord *)sgMatches->Objects[0][ARow];
    if (0 == ARow) {
        background = clBtnFace;
    } else if (NULL == drawnBox) {
        background = clWindow;
    } else if (drawnBox == probableMatch || State.Contains(gdSelected)) {
        if (   (ACol == 2 && drawnBox->first_position != referredBox->first_position)
            || (ACol == 4 && drawnBox->last_position  != referredBox->last_position)
            || (ACol == 3 && 0 != drawnBox->first_barcode.compare(referredBox->first_barcode))
            || (ACol == 5 && 0 != drawnBox->last_barcode.compare(referredBox->last_barcode))
            || (ACol == 1 && drawnBox->status == LPDbBoxName::Status::IN_TANK)
            || (ACol == 6 && drawnBox->slot_position != 1)// ? "*" : ""; // box_store records exist
        ) {
            background = REFERRED_BOXES_ERROR_COLOUR;
        } else if (State.Contains(gdSelected)) {
            background = REFERRED_BOXES_HIGHLIGHT_COLOUR;
        } else {
            background = REFERRED_BOXES_BOX_MATCH_COLOUR;
        }
	} else {
        background = clWindow;
    }
	TCanvas * cnv = sgMatches->Canvas;
	cnv->Brush->Color = background;
	cnv->FillRect(Rect);
	cnv->TextOut(Rect.Left+5, Rect.Top+5, sgMatches->Cells[ACol][ARow]);
}

void __fastcall TfrmReferred::sgStorageDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
    TColor background;
    BoxArrivalRecord * drawnBox = (BoxArrivalRecord *)sgStorage->Objects[0][ARow];
    try {
        if (0 == ARow) {
            background = clBtnFace;
        } else if (NULL == drawnBox) {
            background = clWindow;
        } else if ( // tank, rack or slot don't match
               (1 == ACol && 0 != String(drawnBox->tank_name.c_str()).CompareIC(referredBox->tank_name.c_str()))
            || (2 == ACol && 0 != String(drawnBox->rack_name.c_str()).CompareIC(referredBox->rack_name.c_str()))
            || (3 == ACol && drawnBox->slot_position != referredBox->slot_position)) {
            background = REFERRED_BOXES_ERROR_COLOUR;
        } else if (State.Contains(gdSelected)) {
            background = REFERRED_BOXES_HIGHLIGHT_COLOUR;
        } else {
            background = clWindow;
        }
    } catch (...) {
        background = clBlack; //Fuchsia;
    }

	TCanvas * cnv = sgStorage->Canvas;
	cnv->Brush->Color = background;
	cnv->FillRect(Rect);
	cnv->TextOut(Rect.Left+5, Rect.Top+5, sgStorage->Cells[ACol][ARow]);
}

void TfrmReferred::loadBoxes() {
    LQuery qc(LIMSDatabase::getCentralDb());
    bool randomStatus = MYDEBUG && false;
    if (randomStatus) {
        qc.setSQL("SELECT * from l_box_arrival WHERE swipe_time > DATE('now') - DATE('90 days')"); // fake
    } else {
        qc.setSQL("SELECT * from l_box_arrival WHERE status <> 99 AND status <> 4 ORDER BY status DESC, box_name ASC"); // real
    }
    // XXX boxes in l_box_arrival are for real projects not dev_<project>
    qc.open();
    delete_referenced<tdvecpBoxArrivalRecord>(totalReferred);
    srand(time(NULL)); // initialize random seed
    while (!qc.eof()) {
        BoxArrivalRecord * box = new BoxArrivalRecord(
            qc.readInt("laptop_cid"),
            qc.readInt("process_cid"),
            qc.readInt("box_arrival_id"),
            qc.readInt("project_cid"),
            qc.readDateTime("swipe_time"),
            qc.readString("box_name"),
            randomStatus ? (rand() % 3 + 5) : // random status between 5 and 7
            qc.readInt("status"), //xxx 5 = allocated, 6 = slot confirmed, 7 = referred
            qc.readString("first_barcode"), qc.readInt("first_position"),
            qc.readString("last_barcode"),  qc.readInt("last_position"),
            qc.readInt("tank_cid"),
            qc.readString("rack_number"),
            qc.readInt("slot_position"),
            qc.readDateTime("time_stamp"));
        //box->tank_name  = getVesselName(box->tank_cid);
//        std::ostringstream out; out << box->slot_position;
//        box->slot_name = out.str();
        totalReferred.push_back(box);
        qc.next();
    }
}

void TfrmReferred::showBoxes() { showBoxesByProject(); }

void TfrmReferred::showBoxesByProject() {
    btnSaveBox->Enabled = false; btnDiscard->Enabled = false;
    projectReferred.clear(); // total boxes filtered by project
        // clear list but don't delete objects pointed to - they come from totalReferred
    tdvecpBoxArrivalRecord::const_iterator it;
    for (it = totalReferred.begin(); it != totalReferred.end(); it++) {
        if (0 == listboxProjects->SelCount) { // no filter
            projectReferred.push_back(*it); continue;
        }
        for (int j=0; j < listboxProjects->Items->Count; j++) {
            if (listboxProjects->Selected[j]) {
                if ((*it)->project_cid == (int)listboxProjects->Items->Objects[j]) {
                    projectReferred.push_back(*it); break;
                }
            }
        }
    }
    showBoxTypes();
    filterBoxesByBoxType();
}

void TfrmReferred::filterBoxesByBoxType() {
    boxtypeReferred.clear(); // but don't delete objects pointed to - ie. in totalReferred
    tdvecpBoxArrivalRecord::const_iterator it;
    for (it = projectReferred.begin(); it != projectReferred.end(); it++) {
        if (0 == listboxBoxTypes->SelCount) { // no filter
            boxtypeReferred.push_back(*it); continue;
        }
        for (int j=0; j < listboxBoxTypes->Items->Count; j++) {
            if (listboxBoxTypes->Selected[j]) {
                if (String((*it)->typeFromName().c_str()).CompareIC(listboxBoxTypes->Items->Strings[j]) == 0) {
                    boxtypeReferred.push_back(*it); break;
                }
            }
        }
    }
    if (boxtypeReferred.size() < 1) {
        clearSG(sgReferredBoxes);
    } else {
        sgReferredBoxes->RowCount = boxtypeReferred.size() + 1;
    }
    sgReferredBoxes->FixedRows = 1;
    int i;
    for (i = 1, it = boxtypeReferred.begin(); it != boxtypeReferred.end(); it++, i++) {
        BoxArrivalRecord * box = *it;
        sgReferredBoxes->Cells[0] [i] = box->box_name.c_str();
        sgReferredBoxes->Cells[1] [i] = statusString(box->status);
        sgReferredBoxes->Cells[2] [i] = box->first_position;
        sgReferredBoxes->Cells[3] [i] = box->first_barcode.c_str();
        sgReferredBoxes->Cells[4] [i] = box->last_position;
        sgReferredBoxes->Cells[5] [i] = box->last_barcode.c_str();
        box->tank_name  = getVesselName(box->tank_cid);
        sgReferredBoxes->Cells[6] [i] = box->tank_name.c_str();
        sgReferredBoxes->Cells[7] [i] = box->rack_name.c_str();
        std::ostringstream out; out << box->slot_position; box->slot_name = out.str();
        sgReferredBoxes->Cells[8] [i] = box->slot_name.c_str();
        sgReferredBoxes->Cells[9] [i] = LCDbProjects::records().get(box->project_cid).getName().c_str();
        sgReferredBoxes->Cells[10][i] = box->swipe_time.DateTimeString();
        sgReferredBoxes->Objects[0][i] = (TObject *)(box);
    }
    editBoxName->Clear();           editBoxName->Enabled = true;
    comboStatus->ItemIndex = -1;    comboStatus->Enabled = false;
    editFirstPos->Clear();          editFirstPos->Enabled = false;
    editLastPos->Clear();           editLastPos->Enabled = false;
    editFirstID->Clear();           editFirstID->Enabled = false;
    editLastID->Clear();            editLastID->Enabled = false;
    comboTank->Clear(); comboRack->Clear(); editSlot->Clear();
    comboEventHistory->Clear();
    clearGridSelection(sgReferredBoxes);
    clearSG(sgMatches);
    clearSG(sgStorage);
    btnSaveBox->Enabled = false;    btnDiscard->Enabled = false;
}

/** build list of box types from boxes currently filtered by project (or not) */
void TfrmReferred::showBoxTypes() {
    boxTypes.clear();
    for (tdvecpBoxArrivalRecord::const_iterator it = projectReferred.begin(); it != projectReferred.end(); it++) {
        BoxArrivalRecord * box = *it;
        string boxType = box->typeFromName(); // strip off numbers at end, gives box type
        boxTypes.insert(boxType);
    }
    listboxBoxTypes->Clear();
    for (set<string>::const_iterator it = boxTypes.begin(); it != boxTypes.end(); it++) {
        String boxType((*it).c_str());
        listboxBoxTypes->Items->Add(boxType);
    }
    if (listboxBoxTypes->Items->Count == 0) listboxBoxTypes->Items->Add(" - none - ");
}

// enum Status {NEW_JOB, INPROGRESS, DONE, DELETED = 99};
// enum JobKind {UNKNOWN, MOVEJOB, RETRIEVALJOB, DISCARDJOB, CONFIGURE, NUM_TYPES};
void TfrmReferred::signOffBoxes() {
    std::set<int> projects;
    frmReferredBoxesSummary->summaryBoxes.clear();
    tdvecpBoxArrivalRecord::const_iterator it;
    for (it = totalReferred.begin(); it != totalReferred.end(); it++) {
        BoxArrivalRecord * box = *it;
        if (box->changed) { // if (box->status != LCDbBoxStore::Status::REFERRED/* ??? */)
            frmReferredBoxesSummary->summaryBoxes.push_back(box);
            projects.insert(box->project_cid);
        }
    }
    frmConfirm->initialise(TfrmLogin::REFERRED, "Ready to sign off boxes", projects);  //???
    if (mrOk == frmReferredBoxesSummary->ShowModal()) {
        loadBoxes();
        showBoxes();
    }
    // else, they cancelled - don't refresh???
}

void __fastcall FindMatchesWorkerThread::Execute() {
    delete_referenced<tdvecpBoxArrivalRecord>(frmReferred->matchingBoxes1);
    delete_referenced<tdvecpBoxArrivalRecord>(frmReferred->matchingBoxes2);
    LQuery qp(Util::projectQuery(project));
    string projectName = LCDbProjects::records().get(project).getName();
    string dbName = qp.getDatabase().getDbName();
    int box_id, pos_min, pos_max;

    // populate event history dropdown
	frmReferred->comboEventHistory->Clear();
    LQuery qc(LIMSDatabase::getCentralDb());
    qc.setSQL(
        "SELECT event_cid, operator_cid, event_date, process_cid, project_cid"
        " FROM l_box_arrival_event_history WHERE box_arrival_id = :baid"
        " ORDER BY event_date DESC");
    qc.setParam("baid", frmReferred->referredBox->box_arrival_id);
    if (!qc.open()) {
        frmReferred->comboEventHistory->Items->Add("No event history");
    } else {
        while (!qc.eof()) {
            std::stringstream ss;
            ss << bcsToStd(qc.readDateTime("event_date").DateTimeString()) << ": " <<
            LCDbObjects::records().get(qc.readInt("event_cid")).getName().c_str();
            frmReferred->comboEventHistory->Items->Add(ss.str().c_str());
            qc.next();
        }
    }
    frmReferred->comboEventHistory->ItemIndex = 0;

    // determine whether stats are set on cryovial_store table
    bool stat_record_id, stat_cryovial_id, stat_box_cid, have_stats;
    const char * select_stats_on_column =
        "SELECT COUNT(*) FROM iistatistics s JOIN iirelation r"
        " ON s.stabbase = r.reltid AND s.stabindex = r.reltidx"
        " JOIN iiattribute a ON a.attrelid = r.reltid"
        " AND a.attrelidx = r.reltidx AND a.attid = s.satno"
        " WHERE r.relid = 'cryovial_store'"
        " AND a.attname = :column_name";

	{   LQuery qt(Util::projectQuery(project));
        qt.setSQL(select_stats_on_column);
		qt.setParam("column_name", "record_id"); qt.open();
        stat_record_id = qt.readInt(0); } // LQuery::close(), only by going out of scope

    {   LQuery qt(Util::projectQuery(project));
        qt.setSQL(select_stats_on_column);
		qt.setParam("column_name", "cryovial_id"); qt.open();
        stat_cryovial_id = qt.readInt(0); }

    {   LQuery qt(Util::projectQuery(project));
        qt.setSQL(select_stats_on_column);
		qt.setParam("column_name", "box_cid"); qt.open();
        stat_box_cid = qt.readInt(0); }

    have_stats = stat_record_id && stat_cryovial_id && stat_box_cid;

    /* There's a bug in the ASCEND data: cryovial_store.removed is empty rather than null
    so testing for "cryovial_store.removed IS NULL" gives no matching boxes because it
    thinks the cryovials have all been removed. Use cryovial_store.status instead */

    const char * original_query = // quicker when table has stats
        "SELECT box_cid, MIN(cryovial_position) AS minpos, MAX(cryovial_position) AS maxpos"
        " FROM cryovial_store"
        " WHERE status= 1"
        " AND box_cid IN ("
        "   SELECT box_cid FROM cryovial c, cryovial_store s"
        "   WHERE c.cryovial_id = s.cryovial_id AND cryovial_barcode IN (:first, :last)"
        " )"
        " GROUP BY box_cid";

    const char * recast_query = // quicker on tables with no stats
        "SELECT s.box_cid, MIN(cryovial_position) AS minpos, MAX(cryovial_position) AS maxpos"
        " FROM cryovial_store s JOIN cryovial c ON s.cryovial_id = c.cryovial_id"
        " WHERE s.status= 1"
        " AND cryovial_barcode IN (:first, :last)"
        " GROUP BY box_cid";

    const char * box_name_query =
        "SELECT box_cid, MIN(cryovial_position) AS minpos, MAX(cryovial_position) AS maxpos"
        " FROM cryovial_store"
        " WHERE status= 1"
        " AND box_cid IN ("
        "   SELECT box_cid FROM box_name bn"
        "   WHERE UPPER(bn.external_name) = UPPER(:boxname)"
        " )"
        " GROUP BY box_cid";

    if (boxname.empty()) { // search by barcodes
        qp.setSQL(have_stats ? original_query : recast_query);
        qp.setParam("first", first); qp.setParam("last", last);
    } else { // search by box name
        qp.setSQL(box_name_query);
        qp.setParam("boxname", boxname);
    }

    if (!qp.open()) return; // xxx no results [open() returns bool, not int count]
    while (!qp.eof()) { // Find boxes with either cryovial in
        box_id  = qp.readInt("box_cid");
        pos_min = qp.readInt("minpos"); // first_position
        pos_max = qp.readInt("maxpos"); // last_position
        BoxArrivalRecord * box = new BoxArrivalRecord( // just using this as a similar data structure
            0, 0, box_id, // actually project box id, not box_arrival_id
            project, Now(), "", 0,  "", pos_min, "", pos_max, 0, "", 0, Now());
        frmReferred->matchingBoxes1.push_back(box);
        qp.next();
    }

    // Now find the details you need to display for each box
    tdvecpBoxArrivalRecord::const_iterator it;
    int new_cid, last_cid = 0;
    for (it = frmReferred->matchingBoxes1.begin(); it != frmReferred->matchingBoxes1.end(); it++) {
        BoxArrivalRecord * box = *it;
        qp.setSQL( // Join includes rack and slot, which we can use with central db to find tank
            "SELECT b.box_cid, b.external_name, b.status, bs.status AS storestatus, bs.slot_position AS slot,"
            " s1.cryovial_position AS pos1, c1.cryovial_barcode AS barc1,"
            " s2.cryovial_position AS pos2, c2.cryovial_barcode AS barc2"
            " FROM box_name b LEFT JOIN box_store bs"
            " ON b.box_cid = bs.box_cid AND bs.status != 99, "
            " cryovial c1, cryovial_store s1, cryovial c2, cryovial_store s2"
            " WHERE b.box_cid = s1.box_cid AND b.box_cid = s2.box_cid"
            " AND c1.cryovial_id = s1.cryovial_id AND c2.cryovial_id = s2.cryovial_id"
            " AND b.box_cid = :bid AND s1.cryovial_position = :fpos AND s2.cryovial_position = :lpos"
            " ORDER BY b.box_cid" // to remove dupes
            //" AND bs.status = 6" // cut out dupes?
            );
        qp.setParam("bid",  box->box_arrival_id);
        qp.setParam("fpos", box->first_position);
        qp.setParam("lpos", box->last_position);
        if (!qp.open()) return; // no results
            BoxArrivalRecord * box2 = new BoxArrivalRecord(
                0, 0, box->box_arrival_id, box->project_cid, Now(), // actually project box id, not box_arrival_id
                qp.readString("external_name"), qp.readInt("status"),
                qp.readString("barc1"), qp.readInt("pos1"),
                qp.readString("barc2"), qp.readInt("pos2"),
                0, "", qp.fieldExists("slot") ? 1 : 0, Now());
            box2->box_store_status = qp.readInt("storestatus");
                //xxx make slot_position non-zero to mark existence of box_store record(s)
            frmReferred->matchingBoxes2.push_back(box2);
            qp.next();
//        }
    }
    ReturnValue = 1;
}

void __fastcall TfrmReferred::findMatchesWorkerThreadTerminated(TObject *Sender) {
    if (0 >= matchingBoxes2.size()) {
        clearSG(sgMatches);
    } else {
        sgMatches->RowCount = matchingBoxes2.size() + 1;
        int i = 1;
        tdvecpBoxArrivalRecord::const_iterator it;
        for (it = matchingBoxes2.begin(); it != matchingBoxes2.end(); it++, i++) {
            BoxArrivalRecord * box = *it;
            sgMatches->Cells[0][i] = box->box_name.c_str();
            sgMatches->Cells[1][i] = statusString(box->status);
            sgMatches->Cells[2][i] = box->first_position;
            sgMatches->Cells[3][i] = box->first_barcode.c_str();
            sgMatches->Cells[4][i] = box->last_position;
            sgMatches->Cells[5][i] = box->last_barcode.c_str();
            sgMatches->Cells[6][i] = box->slot_position == 1 ? "*" : ""; //xxx box_store records exist
            //sgMatches->Cells[7][i] = boxStoreStatusString(box->box_store_status);
            sgMatches->Objects[0][i] = (TObject *)box;
            if (0 == String(box->box_name.c_str()).CompareIC(editBoxName->Text)) {
                probableMatch = box; // save match on box name
            }
        }
    }
    btnSaveBox->Enabled = false; btnDiscard->Enabled = true;
    editFirstID->Enabled = true; editLastID->Enabled = true;
    progressBottom->Style = pbstNormal;
    Screen->Cursor = crDefault;
    sgMatches->FixedRows = 1; // otherwise cannot resize columns [4977b131b8]
    findMatchesWorkerThread = NULL;
    clearGridSelection(sgMatches);
    updateStatusBar("OK");
}

void __fastcall FindStorageWorkerThread::Execute() {
    LQuery qp(Util::projectQuery(project));
    delete_referenced<tdvecpBoxArrivalRecord>(frmReferred->storageHistory);

    // get rack_cids from box_store
    qp.setSQL(
        "SELECT record_id, rack_cid, slot_position, status, time_stamp, removed"
        " FROM box_store WHERE box_cid = :bcid AND status <> 99"
        " ORDER BY time_stamp ASC");
    qp.setParam("bcid", box_cid);
    if (!qp.open()) { // no results
        return;
    }
    while (!qp.eof()) {
        BoxArrivalRecord * box = new BoxArrivalRecord(
            0, 0, box_cid, project, Now(), // actually project box id, not box_arrival_id
            "", qp.readInt("status"), "", 0, "", 0,
            0, "", qp.readInt("slot_position"), qp.readDateTime("time_stamp")); // Now()); // tank_cid, rack_name, slot_position, time_stamp
        box->rack_cid   = qp.readInt("rack_cid");
        box->removed    = qp.readDateTime("removed");
        frmReferred->storageHistory.push_back(box);
        qp.next();
    }

    // show event history
    frmReferred->comboEventHistory->Clear();
    qp.setSQL(
        "SELECT event_cid, operator_cid, event_date, process_cid, comment"
        " FROM box_event_history WHERE box_id = :bid"
        " ORDER BY event_date DESC");
    qp.setParam("bid", box_cid);
    if (!qp.open()) {
        frmReferred->comboEventHistory->Items->Add("No event history");
    } else {
        while (!qp.eof()) {
            std::stringstream ss;
            ss << bcsToStd(qp.readDateTime("event_date").DateTimeString()) << ": " <<
            LCDbObjects::records().get(qp.readInt("event_cid")).getName();
            frmReferred->comboEventHistory->Items->Add(ss.str().c_str());
            qp.next();
        }
    }
    frmReferred->comboEventHistory->ItemIndex = 0;

    // find tank name, rack name, and display stringgrid
    tdvecpBoxArrivalRecord::iterator it; // not const as want to modify box records to include tank and rack names
    for (it = frmReferred->storageHistory.begin(); it != frmReferred->storageHistory.end(); it++) {
        BoxArrivalRecord * box = *it;
        LQuery qc(LIMSDatabase::getCentralDb());
        qc.setSQL(
            "SELECT rn.external_name AS rackname,"  // rack name
            " rn.tank_cid as tankcid"               // tank cid
            " FROM c_rack_number rn, c_object_name ob"
            " WHERE ob.object_cid = rn.tank_cid"
            " AND ob.status != 99"
            " AND rn.rack_cid = :rack"); // eg -12589136
        qc.setParam("rack", box->rack_cid);
        if (!qc.open()) { //xxx does this ever happen? yes: ...8146
            box->tank_cid   = 0;
            box->rack_name  = n2s(box->rack_cid);
            box->tank_name  = "unknown";
        } else {
            box->tank_cid   = qc.readInt("tankcid");
            box->rack_name  = qc.readString("rackname");
            box->tank_name  = frmReferred->getVesselName(box->tank_cid);
		}
    }
}

const string TfrmReferred::getVesselName(int population_cid) {
    if (0 != population_cid) {
        const LCDbObject pop = LCDbObjects::records().get(population_cid);
        if (pop.isActive()) { //pop != NULL &&
            if (pop.getObjectType() != LCDbObject::STORAGE_POPULATION) throw "Wrong object type";
            for (Range< LCDbTankMap > tmi = LCDbTankMaps::records(); tmi.isValid(); ++tmi) {
                if (tmi->isActive() && tmi->getTankCID() == pop.getID()) {
                    int vesselCID = tmi->getStorageCID();
                    static const LCDbObjects & names = LCDbObjects::records();
                    const LCDbObject * vessel = names.findByID(vesselCID);
                    ostringstream oss;
                    string site = LCDbObjects::records().get(tmi->getLocationCID()).getName();
                    oss << "["<<LCDbObjects::records().get(population_cid).getName().c_str()<<"] ";
                    oss << vessel->getDescription() << " at " << site << " " << tmi->getPosition();
                    if (tmi->getPopulation() != 0) // it's in a shelf
                        oss<<" [shelf "<<tmi->getPopulation()<<"]";
                    return oss.str(); // e.g. "Selenium at Cowley pos x [shelf y]"
                }
            }
        }
    }
    return "Unknown";
}

void __fastcall TfrmReferred::findStorageWorkerThreadTerminated(TObject *Sender) {
    if (storageHistory.size() <= 0) {
        clearSG(sgStorage);
    } else {
        sgStorage->RowCount = storageHistory.size() + 1;
        tdvecpBoxArrivalRecord::iterator it; int i = 1;
        for (it = storageHistory.begin(); it != storageHistory.end(); it++, i++) {
            BoxArrivalRecord * box = *it;
            sgStorage->Cells[0][i] = box->time_stamp;
            sgStorage->Cells[1][i] = box->tank_name.c_str();
            sgStorage->Cells[2][i] = box->rack_name.c_str();
            sgStorage->Cells[3][i] = box->slot_position;
            sgStorage->Cells[4][i] = boxStoreStatusString(box->status);
            sgStorage->Cells[5][i] = box->removed;
            sgStorage->Objects[0][i] = (TObject *)box;
        }
    }
    progressBottom->Style = pbstNormal;
    Screen->Cursor = crDefault;
    sgStorage->FixedRows = 1; // otherwise cannot resize columns [4977b131b8]
    clearGridSelection(sgStorage);
    findStorageWorkerThread = NULL;
    updateStatusBar("");
}

void TfrmReferred::okOrDiscard(int status) {
    errors.clear(); warnings.clear(); info.clear();

    if (LPDbBoxName::Status::DELETED == status) { // discard
        referredBox->status = status;
        referredBox->changed = true;
        showBoxes();
        return;
    }

    //editedBox = *referredBox; // this has the old box_arrival_id, may be wrong
    if (selectedMatch->box_arrival_id != referredBox->box_arrival_id) { // the ID was wrong in l_b_a - correct it now
        LQuery qp = Util::projectQuery(referredBox->project_cid, true);
        ostringstream out;
        out<<"Correcting l_box_arrival ID from "<<referredBox->box_arrival_id<<" to "<<selectedMatch->box_arrival_id;
        qp.setSQL("SELECT COUNT(*) FROM l_box_arrival WHERE box_arrival_id = :baid");
        qp.setParam("baid", selectedMatch->box_arrival_id);
        qp.open();
        if (0 != qp.readInt(0)) {
            out<<" would clash with existing record with ID "<<selectedMatch->box_arrival_id;//"ERROR: "existing l_box_arrival record with id "<<selectedMatch->box_arrival_id;
            Application->MessageBox(String(out.str().c_str()).c_str(), L"Error", MB_OK);
            return;
        } else { // else { while (!qp.eof()) { qp.next();
            debugLog(out.str().c_str());
        }
        qp.setSQL("UPDATE l_box_arrival SET status = :stat WHERE box_arrival_id = :baid");
        qp.setParam("stat", LPDbBoxName::Status::DELETED);
        qp.setParam("baid", referredBox->box_arrival_id);
        qp.execSQL();
        // bit of a bodge - user could quit before signing off and then the lba record has been 99'd and not seen again.
        // also in signoff, updateLBA will fail. Should create a corrected one as well and update that
        qp.setSQL("INSERT INTO l_box_arrival"
				" (laptop_cid, process_cid, box_arrival_id, project_cid, swipe_time, box_name, status,"
				" first_barcode, first_position, last_barcode, last_position, tank_cid, rack_number, slot_position)"
                " VALUES (:ltid, :prid, :baid, :pjid, :swip, :name, :stat, :bar1, :pos1, :bar2, :pos2, :tank, :rack, :slot)");
        qp.setParam("ltid", referredBox->laptop_cid);
        qp.setParam("prid", referredBox->process_cid);
        qp.setParam("baid", selectedMatch->box_arrival_id);
        qp.setParam("pjid", referredBox->project_cid);
        qp.setParam("swip", XTIME(referredBox->swipe_time));
        qp.setParam("name", referredBox->box_name);
        qp.setParam("stat", status);
        qp.setParam("bar1", referredBox->first_barcode);
        qp.setParam("pos1", referredBox->first_position);
        qp.setParam("bar2", referredBox->last_barcode);
        qp.setParam("pos2", referredBox->last_position);
        qp.setParam("tank", referredBox->tank_cid);
        qp.setParam("rack", referredBox->rack_name);
        qp.setParam("slot", referredBox->slot_position);
        qp.execSQL();
    }

    LQuery qc(LIMSDatabase::getCentralDb());
    editedBox = *selectedMatch; // initialise from canonical box_name record in case box name or ID was wrong in l_b_a
    debugLog("referredBox->str():");
    debugLog(referredBox->str().c_str());
    debugLog("selectedMatch->str():");
    debugLog(selectedMatch->str().c_str());
    editedBox.box_name      = bcsToStd(editBoxName->Text);
    editedBox.status        = status;
    editedBox.first_position= editFirstPos->Text.ToInt();;
    editedBox.last_position = editLastPos->Text.ToInt();
    editedBox.first_barcode = bcsToStd(editFirstID->Text);
    editedBox.last_barcode  = bcsToStd(editLastID->Text);
    if (0 <= comboTank->ItemIndex) {
        editedBox.tank_cid = (int)comboTank->Items->Objects[comboTank->ItemIndex];
    } else {
        errors.push_back("Vessel not selected");
    }
    if (0 <= comboRack->ItemIndex) {
        editedBox.rack_name = bcsToStd(comboRack->Items->Strings[comboRack->ItemIndex]); // comboRack shouldn't be relied to have correct name?
        LCDbRack rackData("", editedBox.tank_cid, editedBox.rack_name);
        if (!rackData.findRack(qc)) {
            ostringstream oss;
            oss <<"Rack not found.\n\ntank: "<<editedBox.tank_name<<"["<<editedBox.tank_cid<<"]\nrack: '"<<editedBox.rack_name<<"'";
            oss <<"\n(Is tank correct?)";
            errors.push_back(oss.str());
        }
        editedBox.rack_cid = rackData.getID();
    } else {
        errors.push_back("Rack not selected");
    }
    editedBox.slot_position = editSlot->Text.ToInt();
    if (editedBox.slot_position <= 0) {
        warnings.push_back("Slot position is zero");
    }
    debugLog("original box:"); debugLog(referredBox->str().c_str());
    debugLog("edited box:");   debugLog(editedBox.str().c_str());

    // warn if not probable match
    if (NULL == selectedMatch) {
        Application->MessageBox(L"No match selected", L"Error", MB_OK); return;
    } else {
        if (NULL != probableMatch) { // there is a matched box
            if (selectedMatch == probableMatch) { // it's been selected
                info.push_back("Selected box is exact match");
            } else { // clicked on a different box
                info.push_back("Selected box is not exact match");
                warnings.push_back("Selected box is not exact match");
            }
        } else { // there wasn't a matched box
            info.push_back("No exact match existed");
        }
    }

    if (errors.size() > 0) {
        finishOKorDiscard();
        return;
    }
    Screen->Cursor = crSQLWait;
    progressBottom->Style = pbstMarquee; progressBottom->Visible = true;
    checkTRSWorkerThread = new CheckTRSWorkerThread(editedBox);
    checkTRSWorkerThread->OnTerminate = &checkTRSWorkerThreadTerminated;
}

void __fastcall CheckTRSWorkerThread::updateStatus() {
    ostringstream ss;
    ss <<"Searching project '"<<curProj.getName()<<"' ["<<curProj.getDbName()<< "("<<curProj.getID()<<")] storage"<<endl;
    frmReferred->updateStatusBar(ss.str());
}

void __fastcall CheckTRSWorkerThread::Execute() {
    LQuery qc(LIMSDatabase::getCentralDb());
    LCDbRack rackData("", box.tank_cid, box.rack_name);

    /// FIXME: NG - the slot is valid if the rack exists and layout exists and 1 <= slot <= layout.rack_capacity
	if (rackData.findRack(qc)) {
        ostringstream out; out << "Slot "<<box.slot_position<<" in structure "<<box.rack_cid;
        const LCDbSectionDef * layout = LCDbSectionDefs::records().findByID(rackData.getSectionType());
        if (NULL == layout) {
            out <<"layout is NULL";  frmReferred->errors.push_back(out.str()); return;
        } else if (1 <= box.slot_position <= layout->getRackCapacity()) {
            out <<" is ok"<<endl;    frmReferred->info.push_back(out.str());
        } else {
            out <<" is not valid";   frmReferred->errors.push_back(out.str()); return;
        }
    }

    // check that tank/rack/slot is available (or still in use for selected box) and give an appropriate error message if necessary
    // repeat query for each project database to find out if the slot is occupied
    for (Range< LCDbProject > pr = LCDbProjects::records(); pr.isValid(); ++ pr) { //&& !pr->isCentral())
        curProj = *pr;
        Synchronize((TThreadMethod)&updateStatus); // don't do graphical things in the thread without Synchronising
        LQuery qi(Util::projectQuery(pr->getID()));;
        qi.setSQL(
            "SELECT box_cid, status, time_stamp, removed, process_cid, retrieval_cid"
            " FROM box_store WHERE rack_cid = :rack AND slot_position = :slot AND status <> 99");
        qi.setParam("rack", box.rack_cid);
        qi.setParam("slot", box.slot_position);
        if (qi.open()) { // results
            int box_in_slot = qi.readInt("box_cid");
            ostringstream out;
            if (box_in_slot != box.box_arrival_id) { // not in use for this box
                                        //xxx box.box_arrival_id has been set correctly?
                int status = qi.readInt("status");
                switch (status) {
                case LCDbBoxStore::SLOT_CONFIRMED:
                    out <<"Position is in use.\n\nbox_cid '"<<qi.readInt("box_cid")<<"' is in "//<<endl
                        <<"structure '"<<box.rack_name<<"' ["<<box.rack_cid<<"], "//<<endl
                        <<"slot "<<box.slot_position//<<endl
                        <<" since "<<bcsToStd(qi.readDateTime("time_stamp").DateTimeString());
                    frmReferred->errors.push_back(out.str());
                    return; // abort
                case LCDbBoxStore::EXPECTED:
                case LCDbBoxStore::UNCONFIRMED:
                case LCDbBoxStore::MOVE_EXPECTED:
                case LCDbBoxStore::REMOVED:
                case LCDbBoxStore::SLOT_ALLOCATED:
                case LCDbBoxStore::REFERRED:
                case LCDbBoxStore::DELETED:
                    // OK?
                default:
                    ;
                }
            } else {
                out <<"Box "<<qi.readInt("box_cid")<<" is already marked as stored in "//<<endl
                    <<"structure '"<<box.rack_name<<"' ["<<box.rack_cid<<"], "//<<endl
                    <<"slot "<<box.slot_position//<<endl
                    <<" since "<<bcsToStd(qi.readDateTime("time_stamp").DateTimeString())<<endl;
                frmReferred->warnings.push_back(out.str());
                break;
            }
            qi.next(); // unlikely
        }
    }

    // compare t/r/s with those in l_b_a
    qc.setSQL("SELECT * FROM l_box_arrival WHERE box_arrival_id = :baid AND status <> 99");
    //qc.setSQL("SELECT * FROM l_box_arrival WHERE box_arrival_id = :baid"); // including 99'd records, ID might have been corrected
    qc.setParam("baid", box.box_arrival_id);
    if (!qc.open()) { // results
        throw Exception("Box arrival record not found");
    } else {
        int orig_tank = qc.readInt("tank_cid");
        string orig_rackname(qc.readString("rack_number"));
        int orig_slot = qc.readInt("slot_position");
        if (0 == box.tank_cid) {
            stringstream ss; ss << "No vessel selected";
            frmReferred->errors.push_back(ss.str());
            return;
        } else if (orig_tank != box.tank_cid) {
            stringstream ss; ss << "Vessel has changed from '"<<(orig_tank == 0 ? string("NULL") : frmReferred->getVesselName(orig_tank)) //LCDbObjects::records().get(orig_tank).getName())
                <<"' to '"<<LCDbObjects::records().get(box.tank_cid).getName()<<"'";
            frmReferred->warnings.push_back(ss.str());
        }
        if (0 != orig_rackname.compare(box.rack_name)) {
            stringstream ss; ss << "Structure has changed from '"<<orig_rackname<<"' to '"<<box.rack_name<<"'";
            frmReferred->warnings.push_back(ss.str());
        }
        if (orig_slot != box.slot_position) {
            stringstream ss; ss << "Slot has changed from "<<orig_slot<<" to "<<box.slot_position;
            frmReferred->warnings.push_back(ss.str());
        }
    }
}
void __fastcall TfrmReferred::checkTRSWorkerThreadTerminated(TObject *Sender) {
    progressBottom->Style = pbstNormal; Screen->Cursor = crDefault;
    updateStatusBar("");
    finishOKorDiscard();
}

void TfrmReferred::finishOKorDiscard() {
    vector<string>::const_iterator it;
    ostringstream out;
    for (it = info.begin(); it != info.end(); it++) { out<<*it<<endl; }
    debugLog(out.str().c_str());

    // display errors/warnings and quit if errors or not sure
    if (errors.size() > 0) {
        ostringstream out;
        for (it = errors.begin(); it != errors.end(); it++) { out<<*it<<endl; }
        Application->MessageBox(String(out.str().c_str()).c_str(), L"Error", MB_OK);
        return;
    }
    if (warnings.size() > 0) {
        ostringstream out;
        for (it = warnings.begin(); it != warnings.end(); it++) { out<<*it<<endl; }
        out << endl << "Are you sure you want to sign off this box?";
        if (mrYes != Application->MessageBox(String(out.str().c_str()).c_str(), L"Warning", MB_YESNO))
            return;
    }

    // mark for sign-off
    *referredBox = editedBox;       // save changes in grid
    referredBox->changed = true;    // debugLog(selectedBox->str().c_str());
    showBoxes();                    // refresh to reflect changes
}

void __fastcall TfrmReferred::sgStorageMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y) {
    int row, col;
    sgStorage->MouseToCell(X, Y, col, row);
    if (row < 1) {
        clearGridSelection(sgStorage);
        return;
    }
    BoxArrivalRecord * selectedBox = (BoxArrivalRecord *)sgStorage->Objects[0][row];
    if (NULL == selectedBox) return;
    setTRS(selectedBox);
    btnSaveBox->Enabled = true;
}

void TfrmReferred::findMatchesByBarcode() { findMatches(false); }

void TfrmReferred::findMatchesByBoxName() { findMatches(true); }

/** find possible matching boxes, by either name or barcodes */
void TfrmReferred::findMatches(bool byBoxName) {
    if (NULL != findMatchesWorkerThread) return; // already searching
    if (sgReferredBoxes->Row < 1) return; //xxx shouldn't happen?
    Screen->Cursor = crSQLWait;
    string dbName, boxname;
    if (byBoxName) { // box name edited, search for it
        boxname.assign(bcsToStd(editBoxName->Text));
    }
    findMatchesWorkerThread = new FindMatchesWorkerThread(
        referredBox->project_cid, boxname, bcsToStd(editFirstID->Text), bcsToStd(editLastID->Text));
    findMatchesWorkerThread->OnTerminate = &findMatchesWorkerThreadTerminated;

    sgMatches->RowCount = 1; sgStorage->RowCount = 1;
    probableMatch = NULL;
    {   LQuery qp(Util::projectQuery(referredBox->project_cid));
        dbName = qp.getDatabase().getDbName(); }
    std::stringstream out;
    out << "Possible matches found in " << dbName;
    groupboxFoundIn->Caption = out.str().c_str();
    ostringstream ss; ss << "Searching for matches in " << dbName;
    updateStatusBar(ss.str());
}

void TfrmReferred::updateStatusBar(string status) {
    statusBar->SimpleText = status.c_str();
}

/** load all tanks (not just for current project) */
void __fastcall TfrmReferred::comboTankDropDown(TObject *Sender) {
    comboTank->Clear(); comboRack->Clear();
    Screen->Cursor = crSQLWait;

    // object_type 9 [STORAGE_POPULATION] or 16 [STORAGE_VESSEL]?

	const LCDbObjects & names = LCDbObjects::records();
    for (Range< LCDbTankMap >tmi = LCDbTankMaps::records(); tmi.isValid(); ++tmi) {
        if (tmi->getStatus() < LCDbTankMap::OFFLINE) {
            const LCDbObject * vessel = names.findByID(tmi->getStorageCID());
            if (vessel != NULL) {
                comboTank->AddItem(getVesselName(tmi->getTankCID()).c_str(), (TObject *)tmi->getTankCID());
            }
        }
    }
    Screen->Cursor = crDefault;
}

/** load racks for current tank */
void __fastcall TfrmReferred::comboRackDropDown(TObject *Sender) {
    comboRack->Clear();
    if (-1 == comboTank->ItemIndex) {
        comboRack->AddItem("No vessel", (TObject *)0);
        comboRack->ItemIndex = 0;
        comboRack->ClearSelection();
        return;
    }
    int tank_cid = (int)comboTank->Items->Objects[comboTank->ItemIndex];
    LQuery qc(LIMSDatabase::getCentralDb());
    qc.setSQL("SELECT * FROM c_rack_number WHERE tank_cid = :tank ORDER BY project_cid, position");
    qc.setParam("tank", tank_cid);
    Screen->Cursor = crSQLWait;
    if (qc.open()) {
        while (!qc.eof()) {
            comboRack->AddItem(qc.readString("external_name").c_str(), (TObject *)qc.readInt("rack_cid"));
            qc.next();
        }
    } else {
        comboRack->AddItem("No structures", (TObject *)0);
    }
    Screen->Cursor = crDefault;
}


