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
    static bool sort_asc_vesspos(const BoxRow *a, const BoxRow *b)   { return a->vessel_pos < b->vessel_pos; }
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
    LPDbCryovialStore * store_record;
    string              cryovial_barcode;
    string              aliquot_type_name;
    int                 dest_cryo_pos;      // cryovial_position
    ~SampleRow() { if (store_record) delete store_record; }
    SampleRow(  LPDbCryovialStore * store_rec,
                string barc, string aliq, string srcnm, int dstid, string dstnm, int dstps,
                string site, int vsps, string vsnm, int shlf, int stps, string stnm, int bxps) :
                RetrievalRow(srcnm, dstid, dstnm, site, vsps, vsnm, shlf, stps, stnm, bxps),
                store_record(store_rec), cryovial_barcode(barc), aliquot_type_name(aliq), dest_cryo_pos(dstps) {
            // why can't derived members be initialised in constructor's initialization list?
            // because they can't - but you can delegate to the base class' constructor
            // http://stackoverflow.com/questions/17196495/initialise-protected-data-members-from-derived-class-constructor
    }

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

    string str() {
        ostringstream oss; oss<<__FUNC__
            <<"id: "<<(store_record->getID())<<", " //	LPDbCryovi alStore: cryovialID, boxID, retrievalID, status, position// <<"status: "<<(store_record->getStatus())<<", " // LPDbCryovial: barcode, boxID, sampleID, typeID, storeID, retrievalID, status, position //<<"barcode: "<<store_record->getBarcode() //<<"sampleID"<<cryo_record->getSampleID() //<<"aliquot type ID"<<cryo_record->getAliquotType()
            <<"status"<<store_record->getStatus()<<", "
            <<"barc: "<<cryovial_barcode<<", "<<"aliq: "<<aliquot_type_name<<", "
            <<"src: {"<<store_record->getBoxID()<<", "<<src_box_name<<"["<<store_record->getPosition()<<"]}, "
            <<"dst: {"<<dest_box_id<<", "<<dest_box_name<<"["<<dest_cryo_pos<<"]}, "
            <<"loc: {"<<storage_str()<<"}";
        return oss.str();
    };
};

typedef std::vector<SampleRow *> vecpSampleRow;

/** StringGridWrapper

Wrapper for TStringGrid, provides sorting functions. T is type of data in each row */
template < class T >
class StringGridWrapper {
    class Col {
    public:
        Col() : sort_func_asc(NULL), name(""), description(""), width(0), sortAsc(false), vec(NULL), initialised(false) { }
        Col(std::string n, std::string t, int w, bool (*f)(const T *, const T *)=NULL) :
            name(n), title(t), width(w), sort_func_asc(f), sortAsc(false) {}
        std::string sortDescription() { ostringstream oss; oss<<"Sort by "<<title<<" ascending"; return oss.str(); }
        bool (*sort_func_asc)(const T *, const T *); // ascending sort function
        std::string name;           // internal identifier string
        std::string title;          // text to display in stringgrid header
        int width;                  // for StringGrid::ColWidths[]
        bool sortAsc;
    };
    TStringGrid * sg;
    std::map< std::string, int > mapColNameToInt;
    bool initialised;
public:
    std::vector< T * > * rows;
    std::vector< Col >cols;

    StringGridWrapper(TStringGrid * g, std::vector<T *> * v) : sg(g), rows(v), initialised(false) {}
    void init() {
        sg->ColCount = cols.size(); // was setupStringGrid
        for (int i=0; i<cols.size(); i++) {
            sg->Cells[i][0]     = cols[i].title.c_str();
            sg->ColWidths[i]    = cols[i].width;
        }
        initialised = true;
    }
    void addCol(Col c) {
        if (initialised) throw "Already initialised";
        mapColNameToInt[c.name] = cols.size();
        cols.push_back(c);
    }
    void addCol(std::string n, std::string d, int w, bool (*f)(const T *, const T *)=NULL) {
        addCol(StringGridWrapper< T >::Col(n, d, w, f));
    }
    int colNameToInt(std::string colName) {
        if (mapColNameToInt.find(colName) == mapColNameToInt.end()) throw "column name not found";
        return mapColNameToInt[colName];
    }
    int colCount() { return cols.size(); }
    int rowCount() { return rows.size(); }
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
    void sort_asc(int col) {
        std::sort(rows->begin(), rows->end(), cols[col].sort_func_asc); // dot notation: vec.begin() also seems to work - how?
    }
    void sort_dsc(int col) {
        std::sort(rows->rbegin(), rows->rend(), cols[col].sort_func_asc);
    }
    void sort_asc(std::string colName) {
        sort_asc(colNameToInt(colName));
    }
    void sort_dsc(std::string colName) {
        sort_dsc(colNameToInt(colName));
    }
    void sort_toggle(int col) {
        cols[col].sortAsc ? sort_asc(col) : sort_dsc(col); cols[col].sortAsc = !cols[col].sortAsc;
    }
    void sort_toggle(std::string colName) {
        sort_toggle(colNameToInt(colName));
    }
};

/**
http://stackoverflow.com/questions/18311149/ho-to-get-rid-of-cbuilder-warning-virtual-function-hides
*/
struct Base {
  virtual void foo(int) {}
  virtual void foo(int, double) {}

};

struct Derived : Base {
  virtual void foo(int) {}
  //using Base::foo;
};
/** end */

class Chunk { // not recorded in database
    string          name;
    int             section;
//    string          start;
//    string          end;
    int             start;
    int             end;
public:
    //Chunk() : section(0), start("start"), end("end") { }
    Chunk() : section(0), start(0), end(0) { }
    //Chunk(string name, int section, string start, string end) : section(section), start(start), end(end) {
    Chunk(string name, int sc, int st, int end) : section(sc), start(st), end(end) {
        //Derived d;
        //d.foo(1, 1.2);
    }
    setSection(int s) { section = s; }
//    incrStart();
//    decrStart();
    incrEnd();
    decrEnd();

    //virtual void    sortAsc(int col) = 0;
    //virtual void    sortDesc(int col) = 0;
    //virtual void    sortToggle(int col) = 0;
    //virtual void    setRows() = 0;
};

typedef std::vector< Chunk * > vecpChunk;

class SampleChunk : public Chunk {
public:
    vecpSampleRow   rows;
    void    sortAsc(int col);
    void    sortDesc(int col);
    void    sortToggle(int col);
};

typedef std::vector< SampleChunk * >  vecpSampleChunk;

class BoxChunk : public Chunk {
public:
    vecpBoxRow      rows;
    void    sortAsc() { return; };
    void    sortDesc();
    void    sortToggle();
};

typedef std::vector< BoxChunk * >  vecpBoxChunk;

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
    void                    debugLog(String s);
    tdvecpJob               vecJobs;
    LCDbCryoJobs            jobs;
    StringGridWrapper<LCDbCryoJob> *  sgwJobs;
    void                    loadJobs();
    void                    showJobs();
    std::string             getExerciseDescription(int exercise_cid);
    std::string             getProjectDescription(int project_cid);
    std::string             getAliquotDescription(int primary_aliquot);
    std::string             getAuditInfo(int process_cid);
public:
    __fastcall TfrmRetrievalAssistant(TComponent* Owner);
    void init();
};

extern PACKAGE TfrmRetrievalAssistant *frmRetrievalAssistant;
#endif
