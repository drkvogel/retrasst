#ifndef RetrievalAssistantH
#define RetrievalAssistantH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.CategoryButtons.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <sstream>
#include "LCDbJob.h"
#include "StoreUtil.h"
#include "LPDbCryovialStore.h"
#include "LPDbCryovial.h"
#include "LDbBoxStore.h"

using namespace std;

const bool RETRASSTDEBUG =
#ifdef _DEBUG
    true;
#else
    false;
#endif

#define DEFAULT_NUMROWS 25

#define RETRIEVAL_ASSISTANT_HIGHLIGHT_COLOUR    clActiveCaption
#define RETRIEVAL_ASSISTANT_NEW_JOB_COLOUR      clMoneyGreen
#define RETRIEVAL_ASSISTANT_IN_PROGRESS_COLOUR  clLime
#define RETRIEVAL_ASSISTANT_DONE_COLOUR         clSkyBlue
#define RETRIEVAL_ASSISTANT_ERROR_COLOUR        clRed
#define RETRIEVAL_ASSISTANT_DELETED_COLOUR      clGray

void msgbox(char * main, char * title="Info") {
    Application->MessageBoxW(String(main).w_str(), String(title).w_str(), MB_OK);
}

void msgbox(string main, string title="Info") { // ridiculous contrivance to use stdstring in message box
    Application->MessageBoxW(String(main.c_str()).c_str(), String(title.c_str()).c_str(), MB_OK);
}

void msgbox(String main, string title="Info") {
    Application->MessageBoxW(main.w_str(), String(title.c_str()).c_str(), MB_OK);
}

void clearGridSelection(TStringGrid * sg) { // put this in storeutil?
    TGridRect myRect;
    myRect.Left = 0; myRect.Top = 0; myRect.Right = 0; myRect.Bottom = 0;
    sg->Selection = myRect;
}

void clearSG(TStringGrid * sg) { // put this in storeutil?
    clearGridSelection(sg);
    sg->FixedRows = 0; sg->RowCount = 0; sg->RowCount = 2; sg->FixedRows = 1;
    for (int i = 0; i < sg->ColCount; i++) { sg->Cells[i][1] = ""; sg->Objects[i][1] = NULL; }
    sg->Cells[0][1] = "No results.";
}

void setupStringGrid(TStringGrid * sg, const int cols, const char * colnames[], const int colwidths[]) {
    sg->ColCount = cols;
    for (int i=0; i<cols; i++) { sg->Cells[i][0] = colnames[i]; sg->ColWidths[i] = colwidths[i]; }
}

std::string printColWidths(TStringGrid * sg) {
    std::ostringstream oss;
    oss << sg->Name.c_str() << ": {"; for (int i=0; i<sg->ColCount; i++) { oss << sg->ColWidths[i] << ", "; } oss << "};";
    return oss.str();
}

class DataRow {
    // common fields
};
typedef DataRow * pDataRow;
typedef std::vector<pDataRow> vecpDataRow;

class BoxRow : public DataRow {
public:
    LCDbBoxStore * store_record; // public LPDbID
    //LPDbBoxName ?? getStatus
    //string              cryovial_barcode;
    //string              aliquot_type_name;
    string              box_name;
    string              dest_box;
    string              dest_pos;
    string              site_name;
    int                 position;
    string              vessel_name;
    int                 shelf_number;
    string              rack_name;
    int                 slot_position;
    BoxRow() {}
    ~BoxRow() { if (store_record) delete store_record; }
    BoxRow(  LCDbBoxStore * store_rec, //string barcode, string aliquot,
                string box,
                string site, int pos, string vessel, int shelf, string rack, int slot) :
        store_record(store_rec), box_name(box),
        site_name(site), position(pos), vessel_name(vessel), shelf_number(shelf), rack_name(rack), slot_position(slot) {}

    static bool sort_asc_currbox(const BoxRow *a, const BoxRow *b)    { return Util::numericCompare(a->box_name, b->box_name); }
 //   static bool sort_asc_currpos(const BoxRow *a, const BoxRow *b)    { return a->store_record->getPosition() < b->store_record->getPosition(); }
    static bool sort_asc_destbox(const BoxRow *a, const BoxRow *b)    { return Util::numericCompare(a->dest_box, b->dest_box); }
    static bool sort_asc_destpos(const BoxRow *a, const BoxRow *b)    { return a->dest_pos < b->dest_pos; }
    static bool sort_asc_site(const BoxRow *a, const BoxRow *b)       { return a->site_name.compare(b->site_name) < 0; }
    static bool sort_asc_position(const BoxRow *a, const BoxRow *b)   { return a->position < b->position; }
    static bool sort_asc_shelf(const BoxRow *a, const BoxRow *b)      { return a->shelf_number < b->shelf_number; }
    static bool sort_asc_vessel(const BoxRow *a, const BoxRow *b)     { return a->vessel_name.compare(b->vessel_name) < 0; }
    static bool sort_asc_structure(const BoxRow *a, const BoxRow *b)  { return Util::numericCompare(a->rack_name, b->rack_name); }//return a->rack_name.compare(b->rack_name) > 0; }
    static bool sort_asc_slot(const BoxRow *a, const BoxRow *b)       { return a->slot_position < b->slot_position; }

    string str() {
        ostringstream oss; oss
        //	LCDbBoxStore:
            <<". id: "<<(store_record->getID())<<", "
            <<"box_name: "<<box_name<<", "
            <<"dest_box: "<<dest_box<<", "
            <<"dest_pos: "<<dest_pos<<", "
            <<"site_name: "<<site_name<<", "
            <<"position: "<<position<<", "
            <<"vessel_name: "<<vessel_name<<", "
            <<"shelf_number: "<<shelf_number<<", "
            <<"rack_name: "<<rack_name<<", "
            <<"slot_position: "<<slot_position;
        return oss.str();
    }
};
typedef BoxRow * pBoxRow;
typedef std::vector<pBoxRow> vecpBoxRow;

class SampleRow : public DataRow {
public:
    LPDbCryovialStore * store_record;
    string              cryovial_barcode;
    string              aliquot_type_name;
    string              src_box_name;       // id and cryo pos are in store_record
    int                 dest_box_id;
    string              dest_box_name;
    int                 dest_cryo_pos;      // cryovial_position
    string              site_name;
    int                 vessel_pos;
    string              vessel_name;
    int                 shelf_number;
    int                 structure_pos;      // c_rack_number.position as rack_pos
    string              structure_name;
    int                 box_pos;
    SampleRow() {}
    ~SampleRow() { if (store_record) delete store_record; }
    SampleRow(  LPDbCryovialStore * store_rec,
                string barcode, string aliquot, string src_name, int dest_id, string dest_name, int dest_pos,
                string site, int vssl_pos, string vssl_name, int shelf, int strct_pos, string strct_name, int slot) :
        store_record(store_rec),
        cryovial_barcode(barcode), aliquot_type_name(aliquot),
        src_box_name(src_name),
        dest_box_id(dest_id), dest_box_name(dest_name), dest_cryo_pos(dest_pos),
        site_name(site), vessel_pos(vssl_pos), vessel_name(vssl_name), shelf_number(shelf), structure_pos(strct_pos), structure_name(strct_name), box_pos(slot) {}

    static bool sort_asc_barcode(const SampleRow *a, const SampleRow *b)    { return a->cryovial_barcode.compare(b->cryovial_barcode) > 0; }
    static bool sort_asc_aliquot(const SampleRow *a, const SampleRow *b)    { return a->aliquot_type_name.compare(b->aliquot_type_name); }
    static bool sort_asc_currbox(const SampleRow *a, const SampleRow *b)    { return Util::numericCompare(a->src_box_name, b->src_box_name); }
    static bool sort_asc_currpos(const SampleRow *a, const SampleRow *b)    { return a->store_record->getPosition() < b->store_record->getPosition(); }
    static bool sort_asc_destbox(const SampleRow *a, const SampleRow *b)    { return Util::numericCompare(a->dest_box_name, b->dest_box_name); }
    static bool sort_asc_destpos(const SampleRow *a, const SampleRow *b)    { return a->dest_cryo_pos < b->dest_cryo_pos; }
    static bool sort_asc_site(const SampleRow *a, const SampleRow *b)       { return a->site_name.compare(b->site_name) < 0; }
    static bool sort_asc_position(const SampleRow *a, const SampleRow *b)   { return a->structure_pos < b->structure_pos; }
    static bool sort_asc_shelf(const SampleRow *a, const SampleRow *b)      { return a->shelf_number < b->shelf_number; }
    static bool sort_asc_vessel(const SampleRow *a, const SampleRow *b)     { return a->vessel_name.compare(b->vessel_name) < 0; }
    static bool sort_asc_structure(const SampleRow *a, const SampleRow *b)  { return Util::numericCompare(a->structure_name, b->structure_name); }//return a->rack_name.compare(b->rack_name) > 0; }
    static bool sort_asc_slot(const SampleRow *a, const SampleRow *b)       { return a->box_pos < b->box_pos; }

    void setLocation(string site, int vssl_pos, string vssl_name, int shelf, int strctr_pos, string strctr_name, int boxpos) {
        site_name       = site;
        vessel_pos      = vssl_pos;
        vessel_name     = vssl_name;
        shelf_number    = shelf;
        structure_pos   = strctr_pos;
        structure_name  = strctr_name;
        box_pos         = boxpos;
    }

    void copyLocation(const SampleRow & other) {
        site_name       = other.site_name;
        vessel_pos      = other.vessel_pos;
        vessel_name     = other.vessel_name;
        shelf_number    = other.shelf_number;
        structure_pos   = other.structure_pos;
        structure_name  = other.structure_name;
        box_pos         = other.box_pos;
    }

    void copyLocation(const ROSETTA & row) {
        site_name       = row.getString("site_name");
        vessel_pos      = row.getInt("tank_pos");
        vessel_name     = row.getString("vessel_name");
        shelf_number    = row.getInt("shelf_number");
        structure_pos   = row.getInt("rack_pos");
        structure_name  = row.getString("structure");
        box_pos         = row.getInt("slot_position");
    }

    string str() {
        ostringstream oss; oss<<__FUNC__
        //	LPDbCryovialStore: cryovialID, boxID, retrievalID, status, position
            <<"id: "<<(store_record->getID())<<", "
            <<"status: "<<(store_record->getStatus())<<", "
        // LPDbCryovial: barcode, boxID, sampleID, typeID, storeID, retrievalID, status, position
            //<<"barcode: "<<store_record->getBarcode()
            //<<"sampleID"<<cryo_record->getSampleID()
            //<<"aliquot type ID"<<cryo_record->getAliquotType()
            <<"status"<<store_record->getStatus()<<", "
            <<"position"<<store_record->getPosition()<<", "
            <<"barc: "<<cryovial_barcode<<", "<<"aliq: "<<aliquot_type_name<<", "
            <<"src: {"<<store_record->getBoxID()<<", "<<src_box_name<<"["<<store_record->getPosition()<<"]}, "
            <<"dst: {"<<dest_box_id<<", "<<dest_box_name<<"["<<dest_cryo_pos<<"]}, "
            <<"loc: {"<<storage_str()<<"}";
        return oss.str();
    };
    string storage_str() {
        ostringstream oss;
            oss<<site_name<<"["<<vessel_pos<<"]: "<<vessel_name<<":"<<shelf_number
            <<"["<<structure_pos<<"]/"<<structure_name<<"["<<box_pos<<"]";
        return oss.str();
    }
};

typedef std::vector<SampleRow *> vecpSampleRow;

/** SGWrapper

Wrapper for TStringGrid, provides sorting functions. T is type of data in each row */
template < class T >
class SGWrapper {
    class Col {
    public:
        Col() : sort_func_asc(NULL), name(""), description(""), width(0), sortAsc(false), vec(NULL), initialised(false) { }
        Col(bool (*f)(const T *, const T *), std::string n, std::string d, int w) :
            sort_func_asc(f), name(n), title(d), width(w), sortAsc(false) {}
        std::string sortDescription() { ostringstream oss; oss<<"Sort by "<<title<<" ascending"; return oss.str(); }
        bool (*sort_func_asc)(const T *, const T *); // ascending sort function
        std::string name;           // internal identifier string
        std::string title;          // text to display in stringgrid header
        int width;                  // for StringGrid::ColWidths[]
        bool sortAsc;
    };
    TStringGrid * sg;
    std::vector< T * > * rows;
    std::vector< Col >cols;
    std::map< std::string, int > mapColNameToInt;
    bool initialised;
public:
    void init() {
        sg->ColCount = cols.size(); // was setupStringGrid
        for (int i=0; i<cols.size(); i++) {
            sg->Cells[i][0]     = cols[i].title;
            sg->ColWidths[i]    = cols[i].width;
            initialised = true;
        }
    }
    SGWrapper(TStringGrid * g, std::vector<T *> * v) : sg(g), rows(v) {}
    void addCol(Col c) {
        if (initialised) throw "Already initialised";
        mapColNameToInt[c.name] = cols.size();
        cols.push_back(c);
    }
    void addCol(bool (*f)(const T *, const T *), std::string n, std::string d, int w) {
        addCol(SGWrapper<SampleRow>::Col(f, n, d, w));
    }
    int colNameToInt(std::string colName) {
        if (mapColNameToInt.find(colName) == mapColNameToInt.end()) throw "column name not found";
        return mapColNameToInt[colName];
    }
    //int size() { return rows.size(); }
    std::string printColWidths() {
        std::ostringstream oss; oss << sg->Name.c_str() << ": {";
        for (int i=0; i<sg->ColCount; i++) { oss << sg->ColWidths[i] << ", "; }
        oss << "};"; return oss.str();
    }
    void clearSelection() {
        TGridRect myRect;
        myRect.Left = 0; myRect.Top = 0; myRect.Right = 0; myRect.Bottom = 0;
        sg->Selection = myRect;
    }
    void clear() {
        clearSelection();
        sg->FixedRows = 0; sg->RowCount = 0; sg->RowCount = 2; sg->FixedRows = 1;
        for (int i = 0; i < sg->ColCount; i++) { sg->Cells[i][1] = ""; sg->Objects[i][1] = NULL; }
        sg->Cells[0][1] = "No results.";
    }
    void sort_asc(std::string colName) {
        int col = colNameToInt(colName);
        std::sort(rows->begin(), rows->end(), cols[col].sort_func_asc); // dot notation: vec.begin() also seems to work - how?
    }
    void sort_dsc(std::string colName) {
        int col = colNameToInt(colName);
        std::sort(rows->rbegin(), rows->rend(), cols[col].sort_func_asc);
    }
    void sort_toggle(std::string colName) {
        int col = colNameToInt(colName);
        cols[col].sortAsc ? sort_asc(col) : sort_dsc(col); cols[col].sortAsc = !cols[col].sortAsc;
    }
};

enum { SGCHUNKS_SECTION, SGCHUNKS_START,  SGCHUNKS_END, SGCHUNKS_SIZE, SGCHUNKS_NUMCOLS };// sgChunks_cols;

static const char * sgChunksColName[SGCHUNKS_NUMCOLS]   = { "Section", "Start", "End", "Size" };

static const int    sgChunksColWidth[SGCHUNKS_NUMCOLS]  = { 200, 200, 200, 200 };

class Chunk { // not recorded in database
public:
    Chunk() : section(0), start("start"), end("end") { }
    Chunk(string name, int section, string start, string end) : section(section), start(start), end(end) { }
    string      name;
    int         section;
    string      start;
    string      end;
};

typedef std::vector< Chunk * > vecpChunk;

class SampleChunk : public Chunk {
public:
    //~SampleChunk() { delete_referenced<vecpSampleRow>(rows); } // rows allocated in 'vials' ie. vector of all rows -
    vecpSampleRow   rows;
    //std::vector<SampleRow *> rows;
};

typedef std::vector< SampleChunk * >  vecpSampleChunk;

class BoxChunk : public Chunk {
public:
    ~BoxChunk() { delete_referenced<vecpBoxRow>(rows); }
    vecpBoxRow      rows;
};

typedef std::vector< BoxChunk * >  vecpBoxChunk;

enum { SGJOBS_DESCRIP, SGJOBS_JOBTYPE, SGJOBS_STATUS, SGJOBS_PRIMARY, SGJOBS_SECONDARY, SGJOBS_PROJECT, SGJOBS_REASON, SGJOBS_TIMESTAMP, SGJOBS_NUMCOLS };

static const char * sgJobsColName[SGJOBS_NUMCOLS]   = { "Description", "Job type", "Status", "Primary Aliquot", "Secondary Aliquot", "Project", "Reason", "Timestamp" };

static const int    sgJobsColWidth[SGJOBS_NUMCOLS]  =  {359, 105, 88, 88, 134, 103, 177, 127 };

static const char * jobStatusString(short status) {
    static const char * jobStatusStrings[] = { "New job", "In progress", "Done", "Deleted" };
    return status < LCDbCryoJob::Status::DELETED ? jobStatusStrings[status] : "Invalid";
};

static const char * jobTypeString(short status) {
    static const char * jobTypeStrings[] = { "Unknown", "Box move", "Box retrieval", "Box discard", "Sample retrieval", "Sample discard", "NUM_TYPES" };
    return status < LCDbCryoJob::JobKind::NUM_TYPES ? jobTypeStrings[status] : "Invalid";
};

typedef std::vector<LCDbCryoJob *> tdvecpJob;

class TfrmRetrievalAssistant : public TForm {
__published:
    TGroupBox *GroupBox1;
    TStringGrid *sgJobs;
    TPanel *Panel2;
    TGroupBox *groupStatusCheck;
    TCheckBox *cbNewJob;
    TCheckBox *cbInProgress;
    TCheckBox *cbDone;
    TCheckBox *cbDeleted;
    TGroupBox *groupRetrievalTypeCheck;
    TCheckBox *cbBoxRetrieval;
    TCheckBox *cbSampleRetrieval;
    TCheckBox *cbBoxMove;
    TCheckBox *cbBoxDiscard;
    TCheckBox *cbSampleDiscard;
    TButton *btnExit;
    TCheckBox *cbLog;
    TMemo *memoDebug;
    TTimer *Timer1;
    TCheckBox *cbRejected;
    void __fastcall sgJobsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall cbNewJobClick(TObject *Sender);
    void __fastcall cbInProgressClick(TObject *Sender);
    void __fastcall cbDoneClick(TObject *Sender);
    void __fastcall cbDeletedClick(TObject *Sender);
    void __fastcall cbBoxRetrievalClick(TObject *Sender);
    void __fastcall cbSampleRetrievalClick(TObject *Sender);
    void __fastcall sgJobsDblClick(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall cbBoxMoveClick(TObject *Sender);
    void __fastcall cbBoxDiscardClick(TObject *Sender);
    void __fastcall cbSampleDiscardClick(TObject *Sender);
    void __fastcall cbLogClick(TObject *Sender);
    void __fastcall sgJobsClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall cbRejectedClick(TObject *Sender);
private:
    void debugLog(String s);
    tdvecpJob vecJobs;
    LCDbCryoJobs jobs;
    void loadJobs();
    void showJobs();
    std::string getExerciseDescription(int exercise_cid);
    std::string getProjectDescription(int project_cid);
    std::string getAliquotDescription(int primary_aliquot);
    std::string getAuditInfo(int process_cid);
public:
    __fastcall TfrmRetrievalAssistant(TComponent* Owner);
    void init();
};

extern PACKAGE TfrmRetrievalAssistant *frmRetrievalAssistant;
#endif
