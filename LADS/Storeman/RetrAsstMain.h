#ifndef RetrAsstMainH
#define RetrAsstMainH
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
#include "LCDbRetrieval.h"
// for setprecision:
#include <iomanip>
#include <boost/date_time.hpp>

#define DEBUGSTREAM(x) {wstringstream oss; oss<<x; debugLog(oss.str().c_str());}

using namespace std;

const bool RETRASSTDEBUG =
#ifdef _DEBUG
    true;
#else
    false;
#endif

#define RETRIEVAL_ASSISTANT_HIGHLIGHT_COLOUR    clActiveCaption
#define RETRIEVAL_ASSISTANT_NEW_COLOUR          clMoneyGreen
#define RETRIEVAL_ASSISTANT_SECONDARY_COLOUR    clYellow
#define RETRIEVAL_ASSISTANT_IN_PROGRESS_COLOUR  clLime
#define RETRIEVAL_ASSISTANT_DONE_COLOUR         clSkyBlue
#define RETRIEVAL_ASSISTANT_NOT_FOUND_COLOUR    clFuchsia
#define RETRIEVAL_ASSISTANT_IGNORED_COLOUR      clGray
#define RETRIEVAL_ASSISTANT_ERROR_COLOUR        clRed
#define RETRIEVAL_ASSISTANT_DELETED_COLOUR      clPurple

// LCDbBoxRetrieval::Status::NEW|PART_FILLED|COLLECTED|NOT_FOUND|DELETED
// LCDbCryovialRetrieval::Status::EXPECTED|IGNORED|COLLECTED|NOT_FOUND|DELETED

#define DEFAULT_BOX_SIZE 100
#define MAX_CHUNK_SIZE 1001

void msgbox(char * main, char * title="Info") {
    Application->MessageBoxW(String(main).w_str(), String(title).w_str(), MB_OK);
}

void msgbox(string main, string title="Info") { // ridiculous contrivance to use stdstring in message box
    Application->MessageBoxW(String(main.c_str()).c_str(), String(title.c_str()).c_str(), MB_OK);
}

void msgbox(String main, string title="Info") {
    Application->MessageBoxW(main.w_str(), String(title.c_str()).c_str(), MB_OK);
}

class RetrievalRow {
public: //protected: ?
    string              src_box_name;       // id and cryo pos are in store_record
    int                 dest_box_id;
    string              dest_box_name;
    string              site_name;
    int                 vessel_pos;
    string              vessel_name;
    int                 shelf_number;
    int                 structure_pos;      // c_rack_number.position as rack_pos
    string              structure_name;
    int                 box_pos;

    RetrievalRow(string srcnm, int dstid, string dstnm, string site, int vsps, string vsnm, int shlf, int stps, string stnm, int bxps) :
        src_box_name(srcnm), dest_box_id(dstid), dest_box_name(dstnm),
        site_name(site), vessel_pos(vsps), vessel_name(vsnm), shelf_number(shlf), structure_pos(stps), structure_name(stnm), box_pos(bxps) {}

    // sort functions could also be factored out; not sure if worth it
    // fun refactoring for a rainy day?
    //static bool sort_asc_currbox(const RetrievalRow *a, const RetrievalRow *b)    { return Util::numericCompare(a->src_box_name, b->src_box_name); }
//    static bool sort_asc_site(const RetrievalRow *a, const RetrievalRow *b)       { return a->site_name.compare(b->site_name) < 0; }
//    static bool sort_asc_vessname(const RetrievalRow *a, const RetrievalRow *b)   { return Util::numericCompare(a->vessel_name, b->vessel_name); }
//    static bool sort_asc_vesspos(const RetrievalRow *a, const RetrievalRow *b)    { return a->vessel_pos < b->vessel_pos; }
//    static bool sort_asc_shelf(const RetrievalRow *a, const RetrievalRow *b)      { return a->shelf_number < b->shelf_number; }
//    static bool sort_asc_vessel(const RetrievalRow *a, const RetrievalRow *b)     { return a->vessel_name.compare(b->vessel_name) < 0; }
//    static bool sort_asc_structure(const RetrievalRow *a, const RetrievalRow *b)  { return Util::numericCompare(a->structure_name, b->structure_name); }//return a->rack_name.compare(b->rack_name) > 0; }
//    static bool sort_asc_slot(const RetrievalRow *a, const RetrievalRow *b)       { return a->box_pos < b->box_pos; }

    void setLocation(string site, int vssl_pos, string vssl_name, int shelf, int strctr_pos, string strctr_name, int boxpos) {
        site_name       = site;
        vessel_pos      = vssl_pos;
        vessel_name     = vssl_name;
        shelf_number    = shelf;
        structure_pos   = strctr_pos;
        structure_name  = strctr_name;
        box_pos         = boxpos;
    }

    void copyLocation(const RetrievalRow & other) {
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

    string storage_str() {
        ostringstream oss;
            oss<<site_name<<"["<<vessel_pos<<"]: "<<vessel_name<<":"<<shelf_number
            <<"["<<structure_pos<<"]/"<<structure_name<<"["<<box_pos<<"]";
        return oss.str();
    }

};
typedef RetrievalRow * pRetrievalRow;
typedef std::vector<pRetrievalRow> vecpRetrievalRow;

class BoxRow : public RetrievalRow {
public:
    LCDbBoxStore * store_record; // public LPDbID //LPDbBoxName ?? getStatus

    BoxRow(LCDbBoxStore * rec, string srcnm, int dstid, string dstnm, int dstps, string site, int vsps, string vsnm, int shlf, int stps, string stnm, int bxps) :
        store_record(rec), RetrievalRow(srcnm, dstid, dstnm, site, vsps, vsnm, shlf, stps, stnm, bxps) {
    }
    ~BoxRow() { if (store_record) delete store_record; }

    static bool sort_asc_currbox(const BoxRow *a, const BoxRow *b)    { return Util::numericCompare(a->src_box_name, b->src_box_name); }
    static bool sort_asc_destbox(const BoxRow *a, const BoxRow *b)    { return Util::numericCompare(a->dest_box_name, b->dest_box_name); }
    static bool sort_asc_site(const BoxRow *a, const BoxRow *b)       { return a->site_name.compare(b->site_name) < 0; }
    static bool sort_asc_vessname(const BoxRow *a, const BoxRow *b)   { return Util::numericCompare(a->vessel_name, b->vessel_name); }
    static bool sort_asc_vesspos(const BoxRow *a, const BoxRow *b)    { return a->vessel_pos < b->vessel_pos; }
    static bool sort_asc_shelf(const BoxRow *a, const BoxRow *b)      { return a->shelf_number < b->shelf_number; }
    static bool sort_asc_vessel(const BoxRow *a, const BoxRow *b)     { return a->vessel_name.compare(b->vessel_name) < 0; }
    static bool sort_asc_structure(const BoxRow *a, const BoxRow *b)  { return Util::numericCompare(a->structure_name, b->structure_name); }//return a->rack_name.compare(b->rack_name) > 0; }
    static bool sort_asc_slot(const BoxRow *a, const BoxRow *b)       { return a->box_pos < b->box_pos; }

    string str() {
        ostringstream oss; oss
            <<". id: "<<(store_record->getID())<<", "<<"src_box_name: "<<src_box_name<<", "
            <<"dest_box_name: "<<dest_box_name<<", "
            <<"loc: {"<<storage_str()<<"}";
        return oss.str();
    }
};
typedef BoxRow * pBoxRow;
typedef std::vector<pBoxRow> vecpBoxRow;

class SampleRow : public RetrievalRow {
public:
    LPDbCryovial            * cryo_record; // auto_ptr for these?
    LPDbCryovialStore       * store_record;
    LCDbCryovialRetrieval   * retrieval_record;
    string                  cryovial_barcode;
    //string                  aliquot_type_name;  // not in LPDbCryovial
    int                     dest_cryo_pos;      // cryovial_position/tube_position
    SampleRow               * secondary;
    ~SampleRow() {
        if (store_record) delete store_record;
        if (cryo_record) delete cryo_record;
        if (secondary) delete secondary;
        if (retrieval_record) delete retrieval_record;
    }
    SampleRow(  LPDbCryovial * cryo_rec, LPDbCryovialStore * store_rec, LCDbCryovialRetrieval * retrieval_rec,
                string barc, string srcnm, int dstid, string dstnm, int dstps,
                string site, int vsps, string vsnm, int shlf, int stps, string stnm, int bxps) :
                RetrievalRow(srcnm, dstid, dstnm, site, vsps, vsnm, shlf, stps, stnm, bxps),
                cryo_record(cryo_rec), store_record(store_rec), retrieval_record(retrieval_rec), cryovial_barcode(barc), dest_cryo_pos(dstps), secondary(NULL) {
    }

    static bool sort_asc_barcode(const SampleRow *a, const SampleRow *b)    { return a->cryovial_barcode.compare(b->cryovial_barcode) < 0; }
    static bool sort_asc_currbox(const SampleRow *a, const SampleRow *b)    { return Util::numericCompare(a->src_box_name, b->src_box_name); }
    static bool sort_asc_currpos(const SampleRow *a, const SampleRow *b)    { return a->store_record->getPosition() < b->store_record->getPosition(); }
    static bool sort_asc_destbox(const SampleRow *a, const SampleRow *b)    { return Util::numericCompare(a->dest_box_name, b->dest_box_name); }
    static bool sort_asc_destpos(const SampleRow *a, const SampleRow *b)    { return a->dest_cryo_pos < b->dest_cryo_pos; }
    static bool sort_asc_site(const SampleRow *a, const SampleRow *b)       { return a->site_name.compare(b->site_name) < 0; }
    static bool sort_asc_vesspos(const SampleRow *a, const SampleRow *b)    { return a->vessel_pos < b->vessel_pos; }
    static bool sort_asc_vessel(const SampleRow *a, const SampleRow *b)     { return a->vessel_name.compare(b->vessel_name) < 0; }
    static bool sort_asc_shelf(const SampleRow *a, const SampleRow *b)      { return a->shelf_number < b->shelf_number; }    
    static bool sort_asc_structpos(const SampleRow *a, const SampleRow *b)  { return a->structure_pos < b->structure_pos; }
    static bool sort_asc_structure(const SampleRow *a, const SampleRow *b)  { return Util::numericCompare(a->structure_name, b->structure_name); }//return a->rack_name.compare(b->rack_name) > 0; }
    static bool sort_asc_slot(const SampleRow *a, const SampleRow *b)       { return a->box_pos < b->box_pos; }
    string aliquotName() {
        return Util::getAliquotDescription(cryo_record->getAliquotType());
    }
    string str() {
        ostringstream oss; oss<<__FUNC__
            <<"id: "<<(store_record->getID())<<", " //	LPDbCryovialStore: cryovialID, boxID, retrievalID, status, position// <<"status: "<<(store_record->getStatus())<<", " // LPDbCryovial: barcode, boxID, sampleID, typeID, storeID, retrievalID, status, position //<<"barcode: "<<store_record->getBarcode() //<<"sampleID"<<cryo_record->getSampleID() //<<"aliquot type ID"<<cryo_record->getAliquotType()
            <<"status"<<store_record->getStatus()<<", "
            <<"barc: "<<cryovial_barcode<<", "<<"aliq: "<<aliquotName()<<", "
            <<"src: {"<<store_record->getBoxID()<<", "<<src_box_name<<"["<<store_record->getPosition()<<"]}, "
            <<"dst: {"<<dest_box_id<<", "<<dest_box_name<<"["<<dest_cryo_pos<<"]}, "
            <<"loc: {"<<storage_str()<<"}";
        return oss.str();
    }

    string storage_str() {
        ostringstream oss;
        oss<<site_name<<"["<<vessel_pos<<"]: "
            <<vessel_name<<":"<<shelf_number<<"["<<structure_pos<<"]/"<<structure_name<<"["<<box_pos<<"]";
        return oss.str();
    }

    string dest_str() {
        ostringstream oss;
        oss<<dest_box_name<<" ["<<dest_cryo_pos<<"]";
        return oss.str();
    }
};

typedef std::vector<SampleRow *> vecpSampleRow;

/** StringGridWrapper
Wrapper for TStringGrid, provides sorting functions. T is type of data in each row */
template < class T >
class StringGridWrapper {
    TStringGrid *       sg;
    map< string, int >  mapColNameToInt;
    bool                initialised;
public:
    class Col {
    public:
        Col() : sort_func_asc(NULL), name(""), description(""), width(0), sortAsc(true), vec(NULL), initialised(false) { }
        Col(string n, string t, int w, bool (*f)(const T *, const T *)=NULL, string d="")
            : name(n), title(t), width(w), sort_func_asc(f), descrip(d), sortAsc(true) {}
        string  sortDescription() {
            ostringstream oss; oss<<(descrip.length() > 0 ? descrip : title)<<" ascending"; return oss.str();
        }
        bool    (*sort_func_asc)(const T *, const T *); // ascending sort function
        bool    (*sort_func_dsc)(const T *, const T *); //{ return (!sort_func_asc()); } // descending sort function
        string  name;           // internal identifier string
        string  title;          // text to display in stringgrid header
        string  descrip;        // longer description e.g. to avoid ambiguity between cols with similar titles
        int     width;          // for StringGrid::ColWidths[]  // minWidth?
        float   percentageWidth;// width as a percentage of total
        bool    sortAsc;        // sort toggle
    };
    vector< T * > *     rows;
    vector< Col >       cols;
    ~StringGridWrapper() { /* delete cols */ }

    StringGridWrapper(TStringGrid * g, vector<T *> * v) : sg(g), rows(v), initialised(false) {}
    void init() {
        sg->ColCount = cols.size(); // was setupStringGrid
        int totalWidth = 0;
        for (int i=0; i<cols.size(); i++) {
            totalWidth += cols[i].width;
        }
        for (int i=0; i<cols.size(); i++) {
            cols[i].percentageWidth = ((float)100/(float)totalWidth) * (float)cols[i].width;
        }
        resize();
        initialised = true;
    }
    void resize() {
        for (int i=0; i<cols.size(); i++) {
            sg->Cells[i][0] = cols[i].title.c_str();
            const int MARGIN = cols.size() + 10; //5; // even ClientWidth seems not big enough to hold all columns without scrolling horizontally
            sg->ColWidths[i] = cols[i].width = (int)((cols[i].percentageWidth * (float)(sg->ClientWidth-MARGIN)) / (float)100);
        }
    }
    void addCol(Col c) {
        if (initialised) throw "Already initialised";
        mapColNameToInt[c.name] = cols.size();
        cols.push_back(c);
    }
    void addCol(string n, string t, int w, bool (*f)(const T *, const T *)=NULL, string d="") {
        addCol(StringGridWrapper< T >::Col(n, t, w, f, d));
    }
    int colNameToInt(string colName) {
        if (mapColNameToInt.find(colName) == mapColNameToInt.end()) throw "column name not found";
        return mapColNameToInt[colName];
    }
    int colCount() { return cols.size(); }
    int rowCount() { return rows->size(); }
    string printColWidths() {
        ostringstream oss; oss << sg->Name.c_str() << ": {";
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
    void sort_asc(int col, int start, int end) {
        frmSamples->debugLog("ascending");
        stable_sort(rows->begin()+start, rows->begin()+end+1, cols[col].sort_func_asc);
    }
    void sort_dsc(int col, int start, int end) {
        frmSamples->debugLog("descending");
        stable_sort(rows->rend()-end-1, rows->rend()-start, cols[col].sort_func_asc);
    }
    void sort_asc(string colName, int start, int end) {
        sort_asc(colNameToInt(colName), start, end);
    }
//    void sort_dsc(string colName, int start, int end) {
//        sort_dsc(colNameToInt(colName), start, end);
//    }
    void sort_toggle(int col, int start, int end) {
        //wstringstream oss; oss << __FUNC__; oss<<"sorting "<<rows->size()<<" rows: start:"<<start<<", end: "<<end; //debugLog(oss.str().c_str());
        //OutputDebugString(L"test"); //OutputDebugString(oss.str().c_str());
        //frmSamples->debugLog(oss.str().c_str());
        cols[col].sortAsc ? sort_asc(col, start, end) : sort_dsc(col, start, end);
        cols[col].sortAsc = !cols[col].sortAsc; // toggle
    }
//    void sort_toggle(string colName, int start, int end) {
//        sort_toggle(colNameToInt(colName), start, end);
//    }
};

template < class T >
class Chunk { // not recorded in database
    StringGridWrapper< T > * sgw;
    int                 section;
    int                 start;
    string              startVial;
    string              startBox;
    int                 end;
    string              endVial;
    string              endBox;
    string              endDescrip;
    int                 currentRowIdx;
    //char *              statusString[] = {"NOT_STARTED, INPROGRESS, DONE, REJECTED, DELETED"};
public:
    Chunk(StringGridWrapper< T > * w, int sc, int s, int e) : sgw(w), section(sc), start(s), end(e), currentRowIdx(0) { }
    enum Status { NOT_STARTED, INPROGRESS, DONE, /*REJECTED, DELETED = 99,*/ NUM_STATUSES };// status;
    string statusString() {
        switch (getStatus()) {
            case NOT_STARTED:
                return "Not started";
            case INPROGRESS:
                return "In progress";
            case DONE:
                return "Completed";
            default:
                throw "unknown status";
        }
    }
    float getProgress() {
        return ((float)currentRowIdx/((float)getSize()));
    }
    string progressString() {
        ostringstream oss;
        //float percent = ((float)currentRowIdx/((float)getSize()))*100;
        float percent = getProgress()*100;
        oss<<currentRowIdx<<"/"<<getSize()<<" ("<<std::setprecision(0)<<std::fixed<<percent<<"%)";
        return oss.str();
    }
    int getStatus() { // http://stackoverflow.com/questions/456713/why-do-i-get-unresolved-external-symbol-errors-when-using-templates
        bool complete = true;
        bool not_started = true;
        for (int i=0; i<getSize(); i++) {
            int status = rowAt(i)->retrieval_record->getStatus();
            switch (status) {
                case LCDbCryovialRetrieval::EXPECTED:
                    complete = false; break;
                case LCDbCryovialRetrieval::IGNORED:
                case LCDbCryovialRetrieval::COLLECTED:
                case LCDbCryovialRetrieval::NOT_FOUND:
                    not_started = false; break;
                default:
                    throw "unexpected LCDbCryovialRetrieval status";
            }
        }
        if (complete) {
            return DONE;
        } else if (not_started) {
            return NOT_STARTED;
        } else {
            return INPROGRESS;
        }
    }
    int     getSection()    { return section; }
    int     getStart()      { return start; }
    int     getStartPos()   { return start+1; } // 1-indexed, human-readable
    T *     getStartRow()   { return sgw->rows->at(start); }
    string  getStartBox()   { return sgw->rows->at(start)->dest_box_name; }
    string  getStartVial()  { return sgw->rows->at(start)->cryo_record->getBarcode(); }
    int     getEnd()        { return end; }
    int     getEndPos()     { return end+1; }   // 1-indexed, human-readable
    string  getEndBox()     { return sgw->rows->at(end)->dest_box_name; }
    string  getEndVial()    { return sgw->rows->at(end)->cryo_record->getBarcode(); }
    int     getCurrentRow() { return currentRowIdx; }
    int     getSize()       { return end - start + 1; } //OutputDebugString(L"test");
    void    setStart(int s) {
        if (s < 0 || s > end)
            throw "invalid chunk start value";
        start = s;
    }
    void    setStartBox(string s) { startBox = s; }
    void    setStartVial(string v) { startVial = v; }
    void    setEnd(int e) {
        if (e > sgw->rowCount()-1)
            throw "invalid chunk end value";
        end = e;
    }
    void    setEndBox(string s) { endBox = s; }
    void    setEndVial(string v) { endVial = v; }
    void    setCurrentRow(int row) { currentRowIdx = row; }
    T *     currentRow() { return rowAt(currentRowIdx); }
    T *     rowAt(int pos) {
        if (pos > getSize())
            throw "out of range";
        return sgw->rows->at((start)+(pos)); }
    void sort_asc(string colName) { sgw->sort_asc(colName, start, end); }
    void sort_dsc(string colName) { sgw->sort_dsc(colName, start, end); }
    void sortToggle(int col) { sgw->sort_toggle(col, start, end); }
    void sortToggle(string colName) { sgw->sort_toggle(colName, start, end); } // n.b. uninstantiated code in templates is not compiled
};

typedef std::vector< Chunk * > vecpChunk;

//???
//class RetrievalList {
//    RetrievalRow rows[];
//    Chunk chunks[];
//};

static const char * jobStatusString(short status) {
    static const char * jobStatusStrings[] = { "New job", "In progress", "Done", "Rejected", "Deleted" };
    return status < LCDbCryoJob::Status::NUM_STATUSES ? jobStatusStrings[status] : "Invalid";
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
    TTimer *Timer1;
    TCheckBox *cbRejected;
    TPanel *panelDebug;
    TMemo *memoDebug;
    TButton *btnResetJobs;
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
    void __fastcall btnResetJobsClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
private:
    StringGridWrapper<LCDbCryoJob> *  sgwJobs;
    void               loadJobs();
    tdvecpJob          vecJobs;
    LCDbCryoJobs       jobs;
    string             getExerciseDescription(int exercise_cid);
    string             getProjectDescription(int project_cid);
    string             getAliquotDescription(int primary_aliquot);
    string             getAuditInfo(int process_cid);
    void               debugLog(String s);
    map<int, const SampleRow *> storageCache;
protected:

public:
    __fastcall TfrmRetrievalAssistant(TComponent* Owner);
    void init();
    void               clearStorageCache();
    void               getStorage(SampleRow * sample);
};

extern PACKAGE TfrmRetrievalAssistant *frmRetrievalAssistant;
#endif
