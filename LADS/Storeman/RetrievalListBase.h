#ifndef RetrievalListBaseH
#define RetrievalListBaseH
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
//#include "RetrAsstCollectSamples.h"
// for setprecision:
#include <iomanip>
#include <boost/date_time.hpp>

//#define RETRIEVAL_ASSISTANT_HIGHLIGHT_COLOUR    clActiveCaption

// job colours
#define RETRIEVAL_ASSISTANT_JOB_NEW_COLOUR          clMoneyGreen
#define RETRIEVAL_ASSISTANT_JOB_INPROGRESS_COLOUR   clLime
#define RETRIEVAL_ASSISTANT_JOB_COMPLETED_COLOUR    clSkyBlue
#define RETRIEVAL_ASSISTANT_JOB_DELETED_COLOUR      clPurple

// chunk colours
#define RETRIEVAL_ASSISTANT_CHUNK_NEW_COLOUR        clMoneyGreen
#define RETRIEVAL_ASSISTANT_CHUNK_INPROGRESS_COLOUR clLime
#define RETRIEVAL_ASSISTANT_CHUNK_COMPLETED_COLOUR  clSkyBlue

// vial aliquot types
#define RETRIEVAL_ASSISTANT_PRIMARY_COLOUR          clMoneyGreen
#define RETRIEVAL_ASSISTANT_SECONDARY_COLOUR        clYellow
#define RETRIEVAL_ASSISTANT_COMBINED_COLOUR         clWebCyan
#define RETRIEVAL_ASSISTANT_EXTRA_COLOUR            clWebChartreuse
    // neither primary nor secondary
    // Chartreuse is a tertiary colour, halfway between green and yellow
    // http://en.wikipedia.org/wiki/Chartreuse_(color)

// vial statuses
#define RETRIEVAL_ASSISTANT_EXPECTED_COLOUR         clMoneyGreen
#define RETRIEVAL_ASSISTANT_NOT_FOUND_COLOUR        clFuchsia
#define RETRIEVAL_ASSISTANT_COLLECTED_COLOUR        clSkyBlue
#define RETRIEVAL_ASSISTANT_DEFERRED_COLOUR          clGray

// other
#define RETRIEVAL_ASSISTANT_ERROR_COLOUR            clRed

#define DEFAULT_BOX_SIZE 100
#define MAX_CHUNK_SIZE 1001
#define DEBUGSTREAM(x) {wstringstream oss; oss<<x; debugLog(oss.str().c_str());}

const bool RETRASSTDEBUG =
#ifdef _DEBUG
    true;
#else
    false;
#endif

using namespace std;

class RetrievalRow {
public: //protected: ?
    int                 project_cid;
    LCDbBoxRetrieval *  cbr_record;
    string              src_box_name;       // id and cryo pos are in store_record
    int                 dest_box_id;
    int                 dest_box_type;
    string              dest_box_name;
    string              dest_type_name;
    string              site_name;
    int                 vessel_pos;
    string              vessel_name;
    int                 shelf_number;
    int                 structure_pos;      // c_rack_number.position as rack_pos
    string              structure_name;
    int                 box_pos;

    RetrievalRow(int proj, LCDbBoxRetrieval * cbr_rec, string srcnm,
                int dstid, string dstnm, int dstyp,
                string site, int vsps, string vsnm, int shlf, int stps, string stnm,
                int bxps) :
        project_cid(proj), cbr_record(cbr_rec), src_box_name(srcnm),
        dest_box_id(dstid), dest_box_name(dstnm), dest_box_type(dstyp),
        site_name(site), vessel_pos(vsps), vessel_name(vsnm), shelf_number(shlf), structure_pos(stps), structure_name(stnm),
        box_pos(bxps) {}

    // sort functions could also be factored out; not sure if worth it
    // fun refactoring for a rainy day?
    //static bool sort_asc_srcbox(const RetrievalRow *a, const RetrievalRow *b)    { return Util::numericCompare(a->src_box_name, b->src_box_name); }
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

    BoxRow(int proj, LCDbBoxRetrieval * cbr_rec, LCDbBoxStore * rec, string srcnm, int dstid, string dstnm, int dstyp, int dstps, string site, int vsps, string vsnm, int shlf, int stps, string stnm, int bxps) :
        store_record(rec), RetrievalRow(proj, cbr_rec, srcnm, dstid, dstnm, dstyp, site, vsps, vsnm, shlf, stps, stnm, bxps) {
    }

    ~BoxRow() { if (store_record) delete store_record; }

    static bool sort_asc_srcbox(const BoxRow *a, const BoxRow *b)     { return Util::numericCompare(a->src_box_name, b->src_box_name); }
    static bool sort_asc_destbox(const BoxRow *a, const BoxRow *b)    { return Util::numericCompare(a->dest_box_name, b->dest_box_name); }
    static bool sort_asc_destype(const BoxRow *a, const BoxRow *b)    { return a->dest_box_type < b->dest_box_type; } // should do by type name, not id
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
    LPDbCryovial *          cryo_record; // auto_ptr for these?
    LPDbCryovialStore *     store_record;
    LCDbCryovialRetrieval * lcr_record;
    string                  cryovial_barcode;
    int                     dest_cryo_pos;      // cryovial_position/tube_position
    SampleRow *             backup; //unique_ptr< SampleRow > backup; //shared_ptr< SampleRow > backup;
    ~SampleRow() {
        if (store_record) delete store_record;
        if (cryo_record) delete cryo_record;
        if (lcr_record) delete lcr_record;
        if (backup) delete backup; // make sure this is NULLed before deleting object
    }
    SampleRow(  int proj, LCDbBoxRetrieval * cbr_rec, LPDbCryovial * cryo_rec, LPDbCryovialStore * store_rec, LCDbCryovialRetrieval * lcr_rec,
                string barc, string srcnm, int dstid, string dstnm, int dstyp, int dstps,
                string site, int vsps, string vsnm, int shlf, int stps, string stnm, int bxps) :
                RetrievalRow(proj, cbr_rec, srcnm, dstid, dstnm, dstyp, site, vsps, vsnm, shlf, stps, stnm, bxps),
                cryo_record(cryo_rec),
                store_record(store_rec),
                lcr_record(lcr_rec),
                //cryovial_barcode(barc), dest_cryo_pos(dstps), backup(NULL) {}
                cryovial_barcode(barc), dest_cryo_pos(dstps), backup() {}
    static bool sort_asc_barcode(const SampleRow *a, const SampleRow *b)    { return a->cryovial_barcode.compare(b->cryovial_barcode) < 0; }
    static bool sort_asc_srcbox(const SampleRow *a, const SampleRow *b)     { return Util::numericCompare(a->src_box_name, b->src_box_name); }
    static bool sort_asc_srcpos(const SampleRow *a, const SampleRow *b)     { return a->store_record->getPosition() < b->store_record->getPosition(); }
    static bool sort_asc_destbox(const SampleRow *a, const SampleRow *b)    { return Util::numericCompare(a->dest_box_name, b->dest_box_name); }
    static bool sort_asc_destype(const SampleRow *a, const SampleRow *b)    { return a->dest_box_type < b->dest_box_type; } // should do by type name, not id
    static bool sort_asc_destpos(const SampleRow *a, const SampleRow *b)    { return a->dest_cryo_pos < b->dest_cryo_pos; }
    static bool sort_asc_site(const SampleRow *a, const SampleRow *b)       { return a->site_name.compare(b->site_name) < 0; }
    static bool sort_asc_vesspos(const SampleRow *a, const SampleRow *b)    { return a->vessel_pos < b->vessel_pos; }
    static bool sort_asc_vessel(const SampleRow *a, const SampleRow *b)     { return a->vessel_name.compare(b->vessel_name) < 0; }
    static bool sort_asc_shelf(const SampleRow *a, const SampleRow *b)      { return a->shelf_number < b->shelf_number; }
    static bool sort_asc_structpos(const SampleRow *a, const SampleRow *b)  { return a->structure_pos < b->structure_pos; }
    static bool sort_asc_structure(const SampleRow *a, const SampleRow *b)  { return Util::numericCompare(a->structure_name, b->structure_name); }//return a->rack_name.compare(b->rack_name) > 0; }
    static bool sort_asc_slot(const SampleRow *a, const SampleRow *b)       { return a->box_pos < b->box_pos; }
    static bool sort_asc_aliquot(const SampleRow *a, const SampleRow *b)    { return a->cryo_record->getAliquotType() < b->cryo_record->getAliquotType(); }

    string aliquotName() {
        return Util::getAliquotDescription(cryo_record->getAliquotType());
    }
    string debug_str() {
        ostringstream oss;
        try {
            oss<<"id: "<<(store_record->getID())<<", "; //	LPDbCryovialStore: cryovialID, boxID, retrievalID, status, position// <<"status: "<<(store_record->getStatus())<<", " // LPDbCryovial: barcode, boxID, sampleID, typeID, storeID, retrievalID, status, position //<<"barcode: "<<store_record->getBarcode() //<<"sampleID"<<cryo_record->getSampleID() //<<"aliquot type ID"<<cryo_record->getAliquotType()
            if (NULL != lcr_record) {
                oss<<"retrieval_status: "<<lcr_record->getStatus()
                <<" ("<<lcr_record->statusString(lcr_record->getStatus())<<"), ";
            }
            oss<<"proj: "<<(project_cid)<<", "
            //oss<<"proj: "<<(cbr_record->getProjId())<<", "
            <<"status: "<<store_record->getStatus()<<", "
            <<"barc: \""<<cryovial_barcode<<"\", "<<"aliq: "<<cryo_record->getAliquotType()<<" \""<<aliquotName()<<"\", "
            <<"cryo_status: "<<cryo_record->getStatus()<<", "
            <<"src: {"<<store_record->getBoxID()<<", \""<<src_box_name<<"\", pos: "<<store_record->getPosition()<<"}, "
            <<"dst: {"<<dest_box_id<<" \""<<dest_box_name<<"\", pos: "<<dest_cryo_pos<<", type: "<<dest_box_type<<" \""<<dest_type_name<<"\"}, "
            <<"loc: {"<<storage_str()<<"}";
        } catch (...) {
            oss<<"error in "<<__FUNC__;
        }
        return oss.str();
    }
    string storage_str() { // for public consumption
        ostringstream oss;
        oss<<site_name<<"["<<vessel_pos<<"]: "
            <<vessel_name<<":"<<shelf_number<<"["<<structure_pos<<"]/"<<structure_name<<"["<<box_pos<<"]";
        // fixme doesn't include box name and cryo position!
        return oss.str();
    }
    string dest_str() { // for public consumption
        ostringstream oss;
        oss<<dest_box_name<<" ["<<dest_cryo_pos<<"]";
        return oss.str();
    }
};

typedef std::vector<SampleRow *> vecpSampleRow;

template < class T >
class StringGridWrapper {
/** Wrapper for TStringGrid, provides sorting functions. T is type of data in each row */
    TStringGrid *       sg;
    map< string, int >  mapColNameToInt;
    bool                initialised;
public:
    class Col {
    public:
        Col(string n, string t, int w, bool (*f)(const T *, const T *)=NULL, string d="")
            : name(n), title(t), width(w), sort_func_asc(f), descrip(d), sortAsc(true) {}
        string  sortDescription() {
            ostringstream oss; oss<<(descrip.length() > 0 ? descrip : title); return oss.str();
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
		if (initialised) throw runtime_error("Already initialised");
		mapColNameToInt[c.name] = cols.size();
		cols.push_back(c);
	}

	void addCol(string n, string t, int w, bool (*f)(const T *, const T *)=NULL, string d="") {
		addCol(StringGridWrapper< T >::Col(n, t, w, f, d));
	}

    int colNameToInt(string colName) {
        if (mapColNameToInt.find(colName) == mapColNameToInt.end()) throw runtime_error("column name not found");
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
		stable_sort(rows->begin()+start, rows->begin()+end+1, cols[col].sort_func_asc);
	}
	void sort_dsc(int col, int start, int end) {
		stable_sort(rows->rend()-end-1, rows->rend()-start, cols[col].sort_func_asc);
    }
    void sort_asc(string colName, int start, int end) {
        sort_asc(colNameToInt(colName), start, end);
    }
//    void sort_dsc(string colName, int start, int end) {
//        sort_dsc(colNameToInt(colName), start, end);
//    }
	void sort_toggle(int col, int start, int end) {
		cols[col].sortAsc ? sort_asc(col, start, end) : sort_dsc(col, start, end);
        cols[col].sortAsc = !cols[col].sortAsc; // toggle
	}
};

template < class T >
class Chunk { // not recorded in database
	StringGridWrapper< T > * sgw;
	int                 section;
    int                 startAbs;
	string              startVial;
	string              startBox;
    int                 endAbs;
    string              endVial;
    string              endBox;
    string              endDescrip;
    int                 rowRel;
    int                 processed;
public:
    Chunk(StringGridWrapper< T > * w, int sc, int s, int e) : sgw(w), section(sc), startAbs(s), endAbs(e), rowRel(0) { }
    enum Status { NOT_STARTED, INPROGRESS, DONE, /*REJECTED, DELETED = 99,*/ NUM_STATUSES };// status;
    int     getSection()    { return section; }
    int     getStartAbs()   { return startAbs; }
    int     getStartRel()   { return 0; }
    T *     getStartRow()   { return sgw->rows->at(startAbs); }
    string  getStartBox()   { return sgw->rows->at(startAbs)->dest_box_name; }
    string  getStartVial()  { return sgw->rows->at(startAbs)->cryo_record->getBarcode(); }
    int     getEndAbs()     { return endAbs; }
    int     getEndRel()     { return endAbs - startAbs; }
    string  getEndBox()     { return sgw->rows->at(endAbs)->dest_box_name; }
    string  getEndVial()    { return sgw->rows->at(endAbs)->cryo_record->getBarcode(); }
    int     getRowRel()     { return rowRel; }
    int     getRowAbs()     { return startAbs + rowRel; }
    int     getSize()       { return endAbs - startAbs + 1; }
    void    setStartAbs(int s) { if (s < 0 || s > endAbs) throw runtime_error("invalid chunk start value"); startAbs = s; }
    void    setStartBox(string s) { startBox = s; }
    void    setStartVial(string v) { startVial = v; }
    void    setEndAbs(int e) { if (e >= sgw->rowCount()) throw runtime_error("invalid chunk end value"); endAbs = e; }
    void    setEndBox(string s) { endBox = s; }
    void    setEndVial(string v) { endVial = v; }
    void    setRowRel(int i) { rowRel = i; }
    void    setRowAbs(int i) { rowRel = i - startAbs; }
    T *     currentObject() { return objectAtRel(rowRel); }
    T *     objectAtRel(int posRel) {
        if (posRel >= getSize()) throw runtime_error("out of range"); return objectAtAbs((startAbs)+(posRel)); }
    T *     objectAtAbs(int posAbs) {
        if (posAbs >= sgw->rowCount()) throw runtime_error("out of range"); return sgw->rows->at(posAbs); }
    void    sort_asc(string colName) { sgw->sort_asc(colName, startAbs, endAbs); }
    void    sort_dsc(string colName) { sgw->sort_dsc(colName, startAbs, endAbs); }
    void    sortToggle(int col) { sgw->sort_toggle(col, startAbs, endAbs); }
    void    sortToggle(string colName) { sgw->sort_toggle(colName, startAbs, endAbs); } // n.b. uninstantiated code in templates is not compiled
    enum NextUnresolvedStatus { NONE_FOUND = -1 };
    int     nextUnresolvedAbs() {
        int rowAbs;
        SampleRow * sample;
        for (rowAbs = startAbs; rowAbs <= endAbs; rowAbs++) {
            sample = objectAtAbs(rowAbs);
            if (sample->lcr_record->getStatus() == LCDbCryovialRetrieval::Status::EXPECTED) {
                return rowAbs;
            } else if (
                    sample->lcr_record->getStatus() == LCDbCryovialRetrieval::Status::NOT_FOUND &&
                    sample->backup != NULL &&
                    sample->backup->lcr_record->getStatus() == LCDbCryovialRetrieval::Status::EXPECTED) {
                return rowAbs; // fixme oo-err
            }
        }
        return NONE_FOUND;
    }
    string statusString() {
        switch (getStatus()) {
            case NOT_STARTED:
                return "Not started";
            case INPROGRESS:
                return "In progress";
            case DONE:
                return "Completed";
            default:
                throw runtime_error("unknown status");
        }
    }
    //bool isProcessed()
    float getProgress() {
        processed = 0; int size = getSize(); if (0 == size) return 0;
        for (int i=0; i<size; i++) { // there could be gaps (previously deferred vials). Gotta count 'em.
            SampleRow * sample = objectAtRel(i);
            switch (sample->lcr_record->getStatus()) { // EXPECTED, DEFERRED, COLLECTED, DISPOSED, NOT_FOUND
                case LCDbCryovialRetrieval::EXPECTED:
                case LCDbCryovialRetrieval::DEFERRED:
                    break;
                case LCDbCryovialRetrieval::COLLECTED:
                case LCDbCryovialRetrieval::DISPOSED:
                    processed++; break;
                case LCDbCryovialRetrieval::NOT_FOUND:
                    if (sample->backup) {
                        switch (sample->backup->lcr_record->getStatus()) {
                            case LCDbCryovialRetrieval::EXPECTED:
                            case LCDbCryovialRetrieval::DEFERRED:
                                break;
                            case LCDbCryovialRetrieval::COLLECTED:
                            case LCDbCryovialRetrieval::DISPOSED:
                            case LCDbCryovialRetrieval::NOT_FOUND:
                                processed++; break;
                            default:
                                int backupStatus = sample->backup->lcr_record->getStatus();
                                throw runtime_error("unexpected LCDbCryovialRetrieval status"+to_string((long long)backupStatus));
                        }
                    } else {
                        processed++;
                    }
                    break;
                default:
                    int status = sample->lcr_record->getStatus();
                    throw runtime_error("unexpected LCDbCryovialRetrieval status: "+to_string((long long)status));
                        // e.g. if status enum in LCDbCryovialRetrieval changed and plan using old scheme loaded
            }
        }
        return ((float)processed/((float)size));
    }
    string progressString() {
        ostringstream oss;
        float percent = getProgress()*100;
        oss<<processed<<"/"<<getSize();//<<" ("<<std::setprecision(0)<<std::fixed<<percent<<"%)";
        return oss.str();
    }
    int getStatus() { // return DONE|NOT_STARTED|INPROGRESS
        bool complete = true;
        bool not_started = true;
        for (int i=0; i<getSize(); i++) {
            int status = objectAtRel(i)->lcr_record->getStatus();
            switch (status) {
                case LCDbCryovialRetrieval::EXPECTED:
                case LCDbCryovialRetrieval::DEFERRED:
                    complete = false; break;
                case LCDbCryovialRetrieval::COLLECTED:
                case LCDbCryovialRetrieval::NOT_FOUND:
                case LCDbCryovialRetrieval::DISPOSED:
                    not_started = false; break;
                default:
                    throw runtime_error("unexpected LCDbCryovialRetrieval status");
            }
        }
        if (complete) { return DONE; } else if (not_started) { return NOT_STARTED; } else { return INPROGRESS; }
    }
};

static const char * jobStatusString(short status) {
    // enum Status { NEW_JOB, INPROGRESS, DONE, REJECTED, DELETED = 99, NUM_STATUSES };
    static const char * jobStatusStrings[] = { "New job", "In progress", "Done", "Rejected" };
    if (LCDbCryoJob::Status::DELETED == status) {
        return "Deleted";
    } else {
        return status < LCDbCryoJob::Status::NUM_STATUSES ? jobStatusStrings[status] : "Invalid";
    }
};

static const char * jobTypeString(short status) {
    static const char * jobTypeStrings[] = { "Unknown", "Box move", "Box retrieval", "Box discard", "Sample retrieval", "Sample discard", "NUM_TYPES" };
    return status < LCDbCryoJob::JobKind::NUM_TYPES ? jobTypeStrings[status] : "Invalid";
};

typedef std::vector<LCDbCryoJob *> tdvecpJob;

class TfrmRetrievalJobList : public TForm {
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
    TPanel *Panel1;
    TButton *btnResetJobs;
    TSplitter *splitterDebug;
    void __fastcall FormResize(TObject *Sender);
    void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall sgJobsDblClick(TObject *Sender);
    void __fastcall sgJobsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall sgJobsClick(TObject *Sender);
    void __fastcall cbNewJobClick(TObject *Sender);
    void __fastcall cbInProgressClick(TObject *Sender);
    void __fastcall cbDoneClick(TObject *Sender);
    void __fastcall cbDeletedClick(TObject *Sender);
    void __fastcall cbBoxRetrievalClick(TObject *Sender);
    void __fastcall cbSampleRetrievalClick(TObject *Sender);
    void __fastcall cbBoxMoveClick(TObject *Sender);
    void __fastcall cbBoxDiscardClick(TObject *Sender);
    void __fastcall cbSampleDiscardClick(TObject *Sender);
    void __fastcall cbLogClick(TObject *Sender);
    void __fastcall cbRejectedClick(TObject *Sender);
    void __fastcall btnResetJobsClick(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
private:
    StringGridWrapper<LCDbCryoJob> *  sgwJobs;
    tdvecpJob           vecJobs;
    LCDbCryoJobs        jobs;
    string              getExerciseDescription(int exercise_cid);
    string              getProjectDescription(int project_cid);
    string              getAliquotDescription(int primary_aliquot);
    string              getAuditInfo(int process_cid);
    void                debugLog(String s);
    void                toggleLog();
    map<int, const SampleRow *> storageCache;
protected:
    void                loadJobs();
    virtual void        initCustom(); // to override if necessary
public:
    __fastcall TfrmRetrievalJobList(TComponent* Owner);
    void                init();
    void                clearStorageCache();
	void                getStorage(SampleRow * sample);
    //void                combineAliquots(const vecpSampleRow & primaries, const vecpSampleRow & secondaries, vecpSampleRow & combined);

	// made into class methods, NG, 17/3/14, to avoid linker error
	static int msgbox(string main, string title="Info", int buttons=MB_OK);
};

//class TfrmRetrievalAssistant : public TfrmRetrievalJobList {
//public:
//    __fastcall TfrmRetrievalAssistant(TComponent* Owner);
//};

//class TfrmRetrAsstCollectSamples : public TForm;
//class TfrmRetrAsstCollectSamples;

//class RetrAsstThread : public TThread {
//protected:
//    RetrAsstThread(); // : TThread(false);
//    TfrmRetrievalAssistant      * main;
//    TfrmRetrAsstCollectSamples  * collect;
//    string          messageTitle; // can't use args for synced method, don't know why
//    string          messageBody;  // so need these temp strings
//public:
//    //virtual
//    void            debugLog(string body);
//    void            _debugLog( ); // synchronized methods can't have args
//    void            updateStatus(string body, string title);
//    void            _updateStatus();
//    void            msgbox(string body, string title);
//    void            _msgbox();
//};

//extern PACKAGE TfrmRetrievalAssistant *frmRetrievalAssistant;
extern PACKAGE TfrmRetrievalJobList *frmRetrievalJobList;
#endif

//wstringstream oss; oss << __FUNC__; oss<<"sorting "<<rows->size()<<" rows: start:"<<start<<", end: "<<end; //debugLog(oss.str().c_str());
//OutputDebugString(L"test"); //OutputDebugString(oss.str().c_str());
//frmSamples->debugLog(oss.str().c_str());

 //	FIXME - C++ compiler error:	  typedef std::vector< Chunk * > vecpChunk;

//???
//class RetrievalList {
//    RetrievalRow rows[];
//    Chunk chunks[];
//};
