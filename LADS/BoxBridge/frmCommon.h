//---------------------------------------------------------------------------

#ifndef frmCommonH
#define frmCommonH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <DB.hpp>

#include <vector>
#include <map>
#include <fstream>

#include "LCDbProject.h"

#define APPMSG(x, y, z) Application->MessageBox(String(x).c_str(), y, z)

#define PROGRAM_NAME "Storage Sync"
#define PROGRAM_SYSTEM_VERSION ""
#define PROGRAM_SUPPORT "Core Programmers"
                                                                      
#define DB_MAJOR_VERSION 2
#define DB_MINOR_VERSION 3

#define ESCQUOTES(x) fmCommon->fnEscapeQuotes(x)

// #define UNKNOWN -1
#define ISNULL 255
#define NULLDATE "30/12/1899"
#define ILLEGAL_CHARS "\\/:*?\"<>|.\'"

static String strTemp;

//---------------------------------------------------------------------------

namespace Config {
    //static const int EVENT_CODE_SYNC = -197500;
    static const int numIDs = 50;
    static const int numTables = 11;
    static const String otherTables[] = {
        "c_permission",
        "c_operator",
        "archive_c_operator",
        "c_project",
        "c_box_size",
        "c_storage_details",
        "c_tank_map",
        //"c_rack_number",      // xxx big
        //"c_slot_allocation",  // obsolete
        "c_object_name",
        "c_tank_layout",
        "c_block",
		"c_machine",
		"c_cluster_machine"
	};
};

class BoxRecord {
public:
    enum ErrorState { NULL_ERROR, UNKNOWN_ERROR, CONFIG_ERROR, BOX_NOT_MATCHED, UNKNOWN_PROJECT, DATABASE_ERROR, BOX_DUPLICATE, SLOT_OCCUPIED, SLOT_INVALID, INVALID_PROJECT, UNEXPECTED };
    String      box_name;
    int         project_cid;
    String      first_barcode;
    int         first_position;
    String      last_barcode;
    int         last_position;
    // encryption/decryption failed, not matched in some way, duplicate, ... ? xxx
    ErrorState  errorState;
    String      errorMessage;

    virtual String print() = 0;
};

class BoxArrivalRecord : public BoxRecord {
public:
    int         laptop_cid;
    int         process_cid;
    int         box_arrival_id;
//    int         project_cid;
    TDateTime   swipe_time;
//    String      box_name;
    int         status;         
//    String      first_barcode;
//    int         first_position;
//    String      last_barcode;
//    int         last_position;
    int         tank_cid;
    String      rack_number;
    int         slot_position;
    //int         slot_cid;
    //String      key_hash;
    TDateTime   time_stamp;

    BoxArrivalRecord() {}
    BoxArrivalRecord
        (int ltid, int psid, int baid, int pjid, TDateTime stime, String bname, int st,
        String firstbar, int firstpos, String lastbar, int lastpos,
        int tankid, String racknum, int rackslot, TDateTime tstamp) :
        laptop_cid(ltid), process_cid(psid), box_arrival_id (baid), swipe_time(stime), status(st),
        tank_cid(tankid), rack_number(racknum), slot_position(rackslot), time_stamp(tstamp)
    {
        box_name = bname; project_cid = pjid;
        first_barcode = firstbar; first_position = firstpos;
        last_barcode = lastbar;   last_position = lastpos;
    }
    String print() {
        String temp; temp.sprintf(
			"laptop_cid=%d, process_cid=%d, box_arrival_id=%d, project_cid=%d, swipe_time=%s, box_name=%s, status=%d, "
//			" first_barcode=, first_position=%d, last_barcode=, last_position=%d,
			" tank_cid=%d, rack_number=%s, slot_position=%d, time_stamp=%s",
			laptop_cid, process_cid, box_arrival_id, project_cid, swipe_time.DateTimeString(), box_name, status,
//			first_position, last_position,
			 tank_cid, rack_number, slot_position, time_stamp.DateTimeString());
		return temp;
	}
};

typedef std::vector< BoxArrivalRecord > tdvecBoxArrivalRecord;

class LBoxExpectedRecord : public BoxRecord {
public:
	int     box_expected_id;
//    String      box_name;
//    int         project_cid;
//    String      first_barcode;
//    int         first_position;
//    String      last_barcode;
//    int         last_position;

    LBoxExpectedRecord() {}
    LBoxExpectedRecord(int bid, int pcid, String bn)
    {
        box_expected_id = bid;
        project_cid = pcid;
        box_name = bn;
    }
    LBoxExpectedRecord
        (int bid, int pcid, String bn, String fb, int fp, String lb, int lp)
    {
        box_expected_id = bid; project_cid = pcid;
        box_name = bn;
        first_barcode = fb; first_position = fp; last_barcode = lb; last_position = lp;
    }
    String print() {
        String temp;
        temp.sprintf(
//            "box_expected_id=%d, project_cid=%d, box_name='%s', "
//            " first_barcode='%s', first_position=%d, last_barcode='%s' last_position=%d",
//            box_expected_id, project_cid, box_name, first_barcode, first_position,
//            last_barcode, last_position);
			"box_expected_id=%d, project_cid=%d, box_name='%s' "
//            " first_barcode=, first_position=%d, last_barcode=, last_position=%d",
			, box_expected_id, project_cid, box_name    );
//			, first_position,       last_position

        return temp;
    }
};

typedef std::vector< LBoxExpectedRecord > tdvecLBoxExpectedRecord;

class SSException : public Exception {
public:
    BoxRecord::ErrorState errorState;
protected:
    SSException(BoxRecord::ErrorState es, String mess) : Exception(mess), errorState(es) {}
};

class SSMinorException : public SSException {
public:
    SSMinorException(BoxRecord::ErrorState es, String mess) : SSException(es, mess) {}
};

typedef std::map<String, String> tdmapFields;

class Operator {
public:
    int         cid;
    int         status;
    TDateTime   valid_from;
    TDateTime   valid_to;
    String      password;
};

typedef std::vector< Operator > tdvecOperator;

String EscapeQuotes(String TheString);

class TfmCommon : public TForm
{
__published:	// IDE-managed Components
    TMemo *MemoDebug;
    TPanel *Panel1;
    TMemo *MemoErr;
    TButton *btnMark;
    void __fastcall FormCreate(TObject *Sender);
    //void __fastcall btnMarkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfmCommon(TComponent* Owner);
    void ShowText(AnsiString text);
    void LogError(String text, String source);
    void clearStringGrid(String contents, TStringGrid *tsg, int vecsize);
};
//---------------------------------------------------------------------------
extern PACKAGE TfmCommon *fmCommon;
//---------------------------------------------------------------------------
#endif
