#include <vcl.h>
#pragma hdrstop
#include <sstream>
#include "ReferredBoxesSummary.h"
#include "LQuery.h"
#include "LIMSDatabase.h"
#include "TfrmConfirm.h"
#include "LCDbJob.h"
#include "StoreUtil.h"
#include "LCDbObject.h"
#include "LCDbRack.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmReferredBoxesSummary *frmReferredBoxesSummary;

__fastcall TfrmReferredBoxesSummary::TfrmReferredBoxesSummary(TComponent* Owner) : TForm(Owner) {}

void TfrmReferredBoxesSummary::debugLog(string s) { memoLog->Lines->Add(s.c_str()); }

void __fastcall TfrmReferredBoxesSummary::FormCreate(TObject *Sender) {
    // col, row
    sgBoxes->ColWidths[0]  = 200;   // Name
    sgBoxes->ColWidths[1]  = 100;   // Project
    sgBoxes->ColWidths[2]  = 140;   // Swiped
    sgBoxes->ColWidths[3]  = 140;   // Status
    sgBoxes->ColWidths[4]  = 30;   // 1stpos
    sgBoxes->ColWidths[5]  = 80;   // 1stID
    sgBoxes->ColWidths[6]  = 30;   // Lstpos
    sgBoxes->ColWidths[7]  = 80;   // LstID
    sgBoxes->ColWidths[8]  = 40;   // Tank
    sgBoxes->ColWidths[9]  = 70;   // Rack
    sgBoxes->ColWidths[10] = 30;   // Slot
    sgBoxes->Cells[0] [0] = "Name";
    sgBoxes->Cells[1] [0] = "Project";
    sgBoxes->Cells[2] [0] = "Swiped";
    sgBoxes->Cells[3] [0] = "Status";
    sgBoxes->Cells[4] [0] = "1st";
    sgBoxes->Cells[5] [0] = "Barcode";
    sgBoxes->Cells[6] [0] = "Lst";
    sgBoxes->Cells[7] [0] = "Barcode";
    sgBoxes->Cells[8] [0] = "Tank";
    sgBoxes->Cells[9] [0] = "Rack";
    sgBoxes->Cells[10][0] = "Slot";
    dummyRun = REFBOXESDEBUG;
}

void __fastcall TfrmReferredBoxesSummary::FormShow(TObject *Sender) {
    memoLog->Clear();
    displayBoxes();
    errors.clear();
    warnings.clear();
    info.clear();
    btnConfirm->Enabled = true;
}

void TfrmReferredBoxesSummary::displayBoxes() {
    tdvecpBoxArrivalRecord::const_iterator it;
    int size = 0;
    for (it = summaryBoxes.begin(); it != summaryBoxes.end(); it++) { if ((*it)->changed) size++; }
    sgBoxes->RowCount = size + 1; //ostringstream oss; oss << "number of changed boxes: "<<size; debugLog(oss.str());
    int i = 1;
    for (it = summaryBoxes.begin(); it != summaryBoxes.end(); it++, i++) {
        BoxArrivalRecord * box = *it;
        if (!box->changed) continue;
        sgBoxes->Cells[0] [i] = box->box_name.c_str();
        sgBoxes->Cells[1] [i] = LCDbProjects::records().get(box->project_cid).getName().c_str();
        sgBoxes->Cells[2] [i] = box->swipe_time.DateTimeString();
        sgBoxes->Cells[3] [i] = statusString(box->status);
        sgBoxes->Cells[4] [i] = box->first_position;
        sgBoxes->Cells[5] [i] = box->first_barcode.c_str();
        sgBoxes->Cells[6] [i] = box->last_position;
        sgBoxes->Cells[7] [i] = box->last_barcode.c_str();
        sgBoxes->Cells[8] [i] = box->tank_cid == 0 ? "---" : LCDbObjects::records().get(box->tank_cid).getName().c_str();
        sgBoxes->Cells[9] [i] = box->rack_name.c_str();
        sgBoxes->Cells[10][i] = box->slot_position;
        sgBoxes->Objects[0][i] = (TObject *)box;
    }
}

void __fastcall TfrmReferredBoxesSummary::btnCancelClick(TObject *Sender) {
    if (btnConfirm->Enabled)
        ModalResult = mrCancel;
    else
        ModalResult = mrOk;
}

void __fastcall TfrmReferredBoxesSummary::btnConfirmClick(TObject *Sender) {
    Screen->Cursor = crSQLWait;
    debugLog(dummyRun ? "*** dummy run ***" : "*** live run ***");
    if (!dummyRun && mrOk != frmConfirm->ShowModal()) return; // require re-login
    if (!dummyRun) btnConfirm->Enabled = false;
    try {
        signOffBoxes();
	} catch(Exception & e) {
		AnsiString msg = e.Message;
		errors.push_back(msg.c_str());
    } catch(char * e) {
        errors.push_back(e);
    } catch (...) {
        errors.push_back("Unknown error processing box");
    }
    Screen->Cursor = crDefault;
    vector<string>::const_iterator strIt;
    if (errors.size() > 0) {
        ostringstream out;
        for (strIt = errors.begin(); strIt != errors.end(); strIt++) { out<<*strIt<<endl; }
        Application->MessageBox(String(out.str().c_str()).c_str(), L"Error", MB_OK);
        return;
    }
}

void TfrmReferredBoxesSummary::signOffBoxes() {
    LQuery qc(LIMSDatabase::getCentralDb());
    std::ostringstream out;

    tdvecpBoxArrivalRecord::const_iterator boxIt;
    BoxArrivalRecord * box;
    for (boxIt = summaryBoxes.begin(); boxIt != summaryBoxes.end(); boxIt++) {
        box = *boxIt;
        out.str(""); out<<"Processing box "<<box->box_name<<" ["<<box->box_arrival_id<<"]"; debugLog(out.str());
        LQuery qp = Util::projectQuery(box->project_cid, true); // true 2nd param gets ddb
        switch (box->status) {
		case LPDbBoxName::Status::IN_TANK:
			storeTRS(qp, box);        // update/insert into box_store
			/* REMOVED 18/9/2013, NG: new Box Reception won't create l_box_arrival_event_history records
				copyHistory(qp, box);     // copy l_box_arrival_event_history into box_event_history
				addEvent(qp, box);        // add new event
			*/
        case LPDbBoxName::Status::DELETED:
            updateLBA(qc, box);       // update l_box_arrival
            break;
        default:
            out.str(""); out<<"Status "<<box->status<<" occurred for box id "<<box->box_arrival_id<<" in "<<__FUNC__;
            throw new Exception(out.str().c_str());
        }
        out.str(""); out<<"Finished processing box "<<box->box_name; debugLog(out.str()); debugLog(""); // newline
        if (!dummyRun) box->changed = false;
        displayBoxes(); // eye candy
    }

    // display warnings and info
    vector<string>::const_iterator strIt;
    if (warnings.size() > 0) {
        ostringstream out;
        for (strIt = warnings.begin(); strIt != warnings.end(); strIt++) { out<<*strIt<<endl; }
        Application->MessageBox(String(out.str().c_str()).c_str(), L"Warning", MB_OK);
    }
    if (info.size() > 0) {
        ostringstream out;
        for (strIt = info.begin(); strIt != info.end(); strIt++) { out<<*strIt<<endl; }
        Application->MessageBox(String(out.str().c_str()).c_str(), L"Info", MB_OK);
    }
}

void TfrmReferredBoxesSummary::updateLBA(LQuery & qc, BoxArrivalRecord * box) {
    std::ostringstream out;
    out<<"Updating l_box_arrival: ";
    if (LPDbBoxName::Status::IN_TANK == box->status) {
        out<<"IN_TANK";
        qc.setSQL(
            "UPDATE l_box_arrival SET "
            " box_name = :bnam, first_position = :fpos, last_position = :lpos,"
            " first_barcode = :fbar, last_barcode = :lbar, status = :stat,"
            " tank_cid = :tank, rack_number = :rack, slot_position = :slot"
            " WHERE box_arrival_id = :baid and project_cid = :pid");
        qc.setParam("bnam", box->box_name);
        qc.setParam("fpos", box->first_position);
        qc.setParam("lpos", box->last_position);
        qc.setParam("fbar", box->first_barcode);
        qc.setParam("lbar", box->last_barcode);
        qc.setParam("tank", box->tank_cid);
        qc.setParam("rack", box->rack_name);
        qc.setParam("slot", box->slot_position);
    } else {
        out<<" status "<<box->status;
        qc.setSQL(
            "UPDATE l_box_arrival SET status = :stat"
            " WHERE box_arrival_id = :baid AND project_cid = :pid");
    }
    qc.setParam("stat", box->status);
    qc.setParam("baid", box->box_arrival_id);
    qc.setParam("pid", box->project_cid);
    if (!dummyRun) qc.execSQL();
    debugLog(out.str());
    // rfp should add to l_box_arrival_event_history
}

void TfrmReferredBoxesSummary::storeTRS(LQuery & qp, BoxArrivalRecord * box) {
    std::ostringstream out;
    out.str(""); out<<"Updating box_store: ";
    LCDbRack rack("", box->tank_cid, box->rack_name);
    if (!rack.findRack(qp)) { // belt and braces
        throw "findRack failed"; // should never happen
    } // box->rack_cid should have been set correctly in okOrDiscard()

    LCDbBoxStore boxStore(0, box->tank_cid, box->rack_name, box->slot_position); // boxID is set to 0 anyway
    boxStore.setBox(box->project_cid, box->box_arrival_id); // just sets projectID = proj, boxID = box; projectID is not used, boxID is used by findRecord
	bool createNew = true;
	LQuery cq(LIMSDatabase::getCentralDb()); // for extra param to LPDbBoxName::saveRecord()
	if (boxStore.findBoxRecord(qp)) { // find by id. copies over fields
		out<<"Found existing box_store record. ";
		if (    box->rack_cid != boxStore.getRackID()
			||  box->slot_position != boxStore.getSlotPosition()) { // box was stored but has moved
			out<<"Set existing record REMOVED";
			boxStore.setStatus(LCDbBoxStore::Status::REMOVED);
			if (!dummyRun) boxStore.saveRecord(qp,cq); // update current record - id should be non-zero here
		} else { // do nothing - status is already SLOT_CONFIRMED due to WHERE clause in findBoxRecord
			out<<"Existing record is correct";
			createNew = false;
		}
	}
	if (createNew) {
        out<<"Creating new box_store record. ";
        LCDbBoxStore newBoxStore(0, box->tank_cid, box->rack_name, box->slot_position);
        newBoxStore.setBox(box->project_cid, box->box_arrival_id);
        newBoxStore.setRackID(box->rack_cid);
        newBoxStore.setStatus(LCDbBoxStore::Status::SLOT_CONFIRMED);
        if (!dummyRun) newBoxStore.saveRecord(qp,cq);
    }
    debugLog(out.str());

    out.str(""); out<<"Updating box_name: ";
    LPDbBoxNames boxes; boxes.readFilled(qp); // reads CONFIRMED [2], ANALYSED [3]. boxes.readCurrent(qp) reads EMPTY [0], IN_USE [1]
    const LPDbBoxName * pboxName;
    pboxName = boxes.findByID(box->box_arrival_id);
    if (NULL == pboxName) { // IN_TANK [4] records not read and findByID for those ids returns a null
        qp.setSQL(
            "SELECT COUNT(*) FROM box_name WHERE box_cid = :bid AND status = :st");
        qp.setParam("bid", box->box_arrival_id);
        qp.setParam("st", LPDbBoxName::Status::IN_TANK);
        if (qp.open() && qp.readInt(0) > 0) { // already marked IN_TANK
            out <<"box_name record with box_id "<<box->box_arrival_id<<" already IN_TANK"; //info.push_back(out.str());
        } else {
            out <<"box_name record not found for box "<<box->box_arrival_id;
            throw new Exception(out.str().c_str()); //xxx is this worthy of an exception?
        }
    } else {
        out<<"Record found. Setting IN_TANK";
        LPDbBoxName boxName = *(pboxName);
        boxName.setStatus(LPDbBoxName::Status::IN_TANK);
		if (!dummyRun) boxName.saveRecord(qp, cq);
    }
    debugLog(out.str());
}

void TfrmReferredBoxesSummary::addEvent(LQuery & qp, BoxArrivalRecord * box) { // object_type = 4
    debugLog("Adding event to box_event_history.");
    const LCDbObject * event;
    switch (box->status) {
        case LPDbBoxName::Status::IN_TANK:
            event = LPDbBoxName::findEvent("BoxConfirmed");
            break;
    default:
        throw new Exception("unexpected box status"); // only add events for confirmed boxes
    }
//	if (!dummyRun) LPDbBoxName(box->box_arrival_id).addEventRecord(qp, event, "added by Referred Boxes");
}

void TfrmReferredBoxesSummary::copyHistory(LQuery & qp, BoxArrivalRecord * box) {
    //debugLog("[DISABLED] Copying l_box_arrival_event_history into box_event_history.");
    return;
    debugLog("Copying l_box_arrival_event_history into box_event_history.");
    qp.setSQL(
        "SELECT event_cid, operator_cid, event_date, process_cid, project_cid"
		" FROM l_box_arrival_event_history WHERE box_arrival_id = :baid"
		" ORDER BY event_date DESC");
    qp.setParam("baid", box->box_arrival_id);
    if (!qp.open()) {
        // nothing in l_box_arrival_event_history
    } else {
        while (!qp.eof()) {
//            if (!dummyRun) LPDbBoxName(box->box_arrival_id).addEventRecord(qp, LCDbObjects::records().findByID(qp.readInt("event_cid")), "");
            qp.next();
        }
    }
}

void __fastcall TfrmReferredBoxesSummary::sgBoxesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State) {
	TColor background = clWindow;
    BoxArrivalRecord * drawnBox = (BoxArrivalRecord *)sgBoxes->Objects[0][ARow];
    if (0 == ARow) {
        background = clBtnFace;
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
	TCanvas * cnv = sgBoxes->Canvas; cnv->Brush->Color = background; cnv->FillRect(Rect);
	cnv->TextOut(Rect.Left+5, Rect.Top+5, sgBoxes->Cells[ACol][ARow]);
}


void __fastcall TfrmReferredBoxesSummary::cbDummyClick(TObject *Sender) {
    dummyRun = cbDummy->Checked;
}


