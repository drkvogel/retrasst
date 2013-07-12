#ifndef ReferredBoxesH
#define ReferredBoxesH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <System.DateUtils.hpp>
#include "LDbBoxType.h"
#include "LDbBoxSize.h"
#include "CritSec.h"
#include "LPDbBoxes.h"
#include "LDbBoxStore.h"
#include "BoxSummary.h"
#include "StoreUtil.h"
#include "StringUtil.h"
#include <Vcl.ComCtrls.hpp>
#include <vector>
#include <set>
#include <algorithm>
#include <sstream>

using namespace std;

const bool MYDEBUG =
#ifdef _DEBUG
    true;
#else
    false;
#endif

#define REFERRED_BOXES_HIGHLIGHT_COLOUR clActiveCaption
#define REFERRED_BOXES_BOX_MATCH_COLOUR clMoneyGreen
#define REFERRED_BOXES_ERROR_COLOUR     clRed
#define REFERRED_BOXES_IN_TANK_COLOUR   clGreen
#define REFERRED_BOXES_ALLOCATED_COLOUR clSilver
#define REFERRED_BOXES_CONFIRMED_COLOUR clGray
#define REFERRED_BOXES_REFFERED_COLOUR  clSkyBlue

class BoxRecord {
public:
    enum ErrorState { NULL_ERROR, UNKNOWN_ERROR, CONFIG_ERROR, BOX_NOT_MATCHED, UNKNOWN_PROJECT, DATABASE_ERROR, BOX_DUPLICATE, SLOT_OCCUPIED, SLOT_INVALID, INVALID_PROJECT, UNEXPECTED };
	string      box_name;
	int         project_cid;
	string      first_barcode;
	int         first_position;
	string      last_barcode;
	int         last_position;
	// encryption/decryption failed, not matched in some way, duplicate, ... ? xxx
	ErrorState  errorState;
	String      errorMessage;
    virtual std::string str() = 0;
};

class BoxArrivalRecord : public BoxRecord {
public:
	int         laptop_cid;
	int         process_cid;
	int         box_arrival_id;
	TDateTime   swipe_time;
	int         status;
    int         box_store_status;
/* LCDbBoxStore::Status {
    EXPECTED = 0, MOVE_EXPECTED = 1, REMOVED = 3, SLOT_ALLOCATED = 5,
	SLOT_CONFIRMED = 6, REFERRED = 7, DELETED = 99 }; */
	int         tank_cid;
    int         rack_cid;       // for t_ldbx::box_store.rack_cid
	string      rack_name;      // for t_ldbc::l_b_a.rack_number - which is a varchar
    int         slot_position;  // for t_ldbc::l_b_a.slot_position or t_ldbx::box_store.slot_position
    string      tank_name;
    string      slot_name;      // to avoid converting int to string more than once: it's painful!
    TDateTime   time_stamp;
    TDateTime   removed;
    bool        changed;
    string      typeFromName() const;

    BoxArrivalRecord() {}
    BoxArrivalRecord
		(int ltid, int psid, int baid, int pjid, TDateTime stime, string bname, int st,
		string firstbar, int firstpos, string lastbar, int lastpos,
		int tankid, string racknum, int rackslot, TDateTime tstamp) :
        laptop_cid(ltid), process_cid(psid), box_arrival_id (baid), swipe_time(stime), status(st),
        tank_cid(tankid), rack_name(racknum), slot_position(rackslot), time_stamp(tstamp), changed(false)
    {
        box_name = bname; project_cid = pjid;
        first_barcode = firstbar; first_position = firstpos;
        last_barcode = lastbar;   last_position = lastpos;
	}
    std::string str() {
        std::stringstream out;
        out
        //<<"laptop_cid: "<<laptop_cid<<", process_cid: "<<process_cid
        <<" box_arrival_id: "<<box_arrival_id<<", status: "<<status
        <<", box_name: "<<box_name
        <<", project_cid: "<<project_cid<<", first_barcode: "<<first_barcode
	    <<", first_position: "<<first_position<<", last_barcode: "<<last_barcode<<", last_position: "<<last_position
        <<", swipe_time: "<<bcsToStd(swipe_time.DateTimeString())
        <<", tank_cid: "<<tank_cid<<", rack_cid: "<<rack_cid<<", rack_name: "<<rack_name<<", slot_position: "<<slot_position<<", tank_name: "<<tank_name
        //<<", time_stamp: "<<time_stamp.DateTimeString()
        <<", changed: "<<changed<<", typeFromName(): "<<typeFromName();
        return out.str();
    }
};

typedef std::vector< BoxArrivalRecord > tdvecBoxArrivalRecord;
typedef std::vector< BoxArrivalRecord*> tdvecpBoxArrivalRecord;

template<typename Container>
void delete_referenced(Container& c) {
    while (!c.empty()) delete c.back(), c.pop_back();
}

static const char * boxStoreStatusStrings[] = {
//    case LCDbBoxStore::Status::EXPECTED:        // 0
//    case LPDbBoxName::Status::EMPTY:            // 0
//    case LCDbBoxStore::Status::UNCONFIRMED:     // 1
//    case LPDbBoxName::Status::IN_USE:           // 1
//    case LCDbBoxStore::Status::MOVE_EXPECTED    // 2
//    case LPDbBoxName::Status::CONFIRMED:        // 2
//    case LCDbBoxStore::Status::REMOVED:         // 3
//    case LPDbBoxName::Status::IN_TANK:          // 4
//    case LCDbBoxStore::Status::SLOT_ALLOCATED:  // 5
//    case LCDbBoxStore::Status::SLOT_CONFIRMED:  // 6
//    case LCDbBoxStore::Status::REFERRED:        // 7
//    case LPDbBoxName::Status::DESTROYED         // 8
//    case LCDbBoxStore::Status::DELETED          // 99
//    case LPDbBoxName::Status::DELETED           // 99
    "EXPECTED [0]", "UNCONFIRMED [1]", "CONFIRMED [2]", "REMOVED [3]", "IN_TANK [4]",
    "SLOT_ALLOCATED [5]", "SLOT_CONFIRMED [6]", "REFERRED [7]", "DESTROYED [8]", "DELETED [99]"
};

static const char * boxNameStatusStrings[] = {
    "EMPTY [0]", "IN_USE [1]", "CONFIRMED [2]", "[undefined] [3]", "IN_TANK [4]",
    "[undefined] [5]", "[undefined] [6]", "[undefined] [7]", "DESTROYED [8]", "DELETED [99]"
    // { EMPTY = 0, IN_USE = 1, CONFIRMED = 2, ANALYSED = 3, IN_TANK = 4, DESTROYED = 8, DELETED = 99 };
};

/** number to string */
template <typename T>
string n2s(T Number) {
    ostringstream ss; ss << Number; return ss.str();
}

static const char * statusString(int status) {
    return status < LPDbBoxName::Status::DESTROYED ?
        boxStoreStatusStrings[status]
        : boxStoreStatusStrings[9]; // DELETED
}

static const char * boxStoreStatusString(int status) {
    return status < LCDbBoxStore::Status::DELETED ?
        boxStoreStatusStrings[status]
        : boxStoreStatusStrings[9]; // DELETED
}

class FindMatchesWorkerThread : public TThread
{
private:
    int project;
    string boxname, first, last;
protected:
    void __fastcall Execute();
public:
    __fastcall FindMatchesWorkerThread(int project, string boxname, string first, string last);
};

class FindStorageWorkerThread : public TThread {
private:
    int project;
    int box_cid;
protected:
    void __fastcall Execute();
public:
    __fastcall FindStorageWorkerThread(int project, int box_cid);
};

class CheckTRSWorkerThread : public TThread {
private:
    LCDbProject curProj;
    BoxArrivalRecord box;
protected:
    void __fastcall Execute();
    void __fastcall updateStatus();
public:
    __fastcall CheckTRSWorkerThread(BoxArrivalRecord box);
};

class TfrmReferred : public TForm
{
friend class FindMatchesWorkerThread;
friend class FindStorageWorkerThread;
friend class CheckTRSWorkerThread;
__published:	// IDE-managed Components
    TPanel *panelProjectsAndBoxTypes;
    TGroupBox *groupboxProjects;
    TListBox *listboxProjects;
    TSplitter *Splitter1;
    TGroupBox *groupboxBoxTypes;
    TListBox *listboxBoxTypes;
    TPanel *panelBoxes;
    TGroupBox *groupboxReferredBoxes;
    TStringGrid *sgReferredBoxes;
    TGroupBox *groupboxBoxDetails;
    TLabel *Label2;
    TLabel *Label4;
    TEdit *editBoxName;
    TEdit *editFirstPos;
    TEdit *editLastPos;
    TEdit *editFirstID;
    TEdit *editLastID;
    TButton *btnSaveBox;
    TSplitter *Splitter2;
    TGroupBox *groupboxFoundIn;
    TStringGrid *sgMatches;
    TSplitter *Splitter3;
    TComboBox *comboStatus;
    TButton *btnDiscard;
    TLabel *Label1;
    TLabel *Label3;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TEdit *editSlot;
    TProgressBar *progressBottom;
    TPanel *panelBottom;
    TBitBtn *btnSignOff;
    TButton *btnDone;
    TGroupBox *groupboxStorageHistory;
    TSplitter *Splitter4;
    TStringGrid *sgStorage;
    TTimer *timerReferredBoxClicked;
    TMemo *memoDebug;
    TCheckBox *cbLog;
    TComboBox *comboEventHistory;
    TTimer *timerBoxNameEdited;
    TTimer *timerBarcodeEdited;
    TComboBox *comboRack;
    TComboBox *comboTank;
    TUpDown *updownSlot;
    TStatusBar *statusBar;
    TLabel *Label8;
    void __fastcall listboxProjectsClick(TObject *Sender);
    void __fastcall listboxBoxTypesClick(TObject *Sender);
    void __fastcall btnSaveBoxClick(TObject *Sender);
    void __fastcall sgReferredBoxesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall editBoxNameChange(TObject *Sender);
    void __fastcall editFirstIDChange(TObject *Sender);
    void __fastcall editLastIDChange(TObject *Sender);
    void __fastcall sgMatchesMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall sgMatchesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall btnSignOffClick(TObject *Sender);
    void __fastcall groupboxBoxTypesClick(TObject *Sender);
    void __fastcall groupboxProjectsClick(TObject *Sender);
    void __fastcall btnDiscardClick(TObject *Sender);
    void __fastcall btnDoneClick(TObject *Sender);
    void __fastcall sgStorageDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall timerReferredBoxClickedTimer(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall sgStorageMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall cbLogClick(TObject *Sender);
    void __fastcall timerBarcodeEditedTimer(TObject *Sender);
    void __fastcall timerBoxNameEditedTimer(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall comboTankDropDown(TObject *Sender);
    void __fastcall comboRackDropDown(TObject *Sender);
    void __fastcall sgReferredBoxesMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
    set<std::string> boxTypes;
    std::vector<BoxArrivalRecord*> totalReferred;
    std::vector<BoxArrivalRecord*> projectReferred;
    std::vector<BoxArrivalRecord*> boxtypeReferred;

    void loadBoxes();
    void showBoxes();
    void showBoxTypes();
    void showBoxesByProject();
    void filterBoxesByBoxType();
    void setTRS(BoxArrivalRecord * box);
    void setBoxDetails(BoxArrivalRecord * box);

    BoxArrivalRecord * referredBox;
    BoxArrivalRecord editedBox;
    FindMatchesWorkerThread * findMatchesWorkerThread;
    void __fastcall findMatchesWorkerThreadTerminated(TObject *Sender);

    BoxArrivalRecord * probableMatch;
    BoxArrivalRecord * selectedMatch;
    FindStorageWorkerThread * findStorageWorkerThread;
    void __fastcall findStorageWorkerThreadTerminated(TObject *Sender);

    CheckTRSWorkerThread * checkTRSWorkerThread;
    void __fastcall checkTRSWorkerThreadTerminated(TObject *Sender);

    void findMatches(bool byBoxName);
    void findMatchesByBarcode();
    void findMatchesByBoxName();
    void okOrDiscard(int status);
    void finishOKorDiscard();
    void signOffBoxes();
    //const LCDbObject * getVessel(int tank_cid);
    const string getVesselName(int population_cid);
protected:
    vector<string> info;
    vector<string> warnings;
    vector<string> errors;
    tdvecpBoxArrivalRecord matchingBoxes1; // public for thread
    tdvecpBoxArrivalRecord matchingBoxes2;
    tdvecpBoxArrivalRecord storageHistory;
    void updateStatusBar(string status);
    void debugLog(String s);
public:		// User declarations
	__fastcall TfrmReferred(TComponent* Owner);
	void init();
};

extern PACKAGE TfrmReferred *frmReferred;

#endif
