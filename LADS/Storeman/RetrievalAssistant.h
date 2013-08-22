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

// utilities

void msgbox(string main, string title="Info") { // ridiculous contrivance to use stdstring in message box
    Application->MessageBoxW(String(main.c_str()).c_str(), String(title.c_str()).c_str(), MB_OK);
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

// end utilities

class BoxRow {
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
    BoxRow(  LCDbBoxStore * store_rec, //string barcode, string aliquot,
                string box,
                string site, int pos, string vessel, int shelf, string rack, int slot) :
        store_record(store_rec), box_name(box),
        site_name(site), position(pos), vessel_name(vessel), shelf_number(shelf), rack_name(rack), slot_position(slot) {}

    // box_name vs store_record->getID()?
    static bool sort_asc_currbox(const BoxRow *a, const BoxRow *b)    { return numeric_compare(a->box_name, b->box_name); }
    static bool sort_desc_currbox(const BoxRow *a, const BoxRow *b)   { return numeric_compare(b->box_name, a->box_name); }
 //   static bool sort_asc_currpos(const BoxRow *a, const BoxRow *b)    { return a->store_record->getPosition() < b->store_record->getPosition(); }
 //   static bool sort_desc_currpos(const BoxRow *a, const BoxRow *b)   { return a->store_record->getPosition() > b->store_record->getPosition(); }
    static bool sort_asc_destbox(const BoxRow *a, const BoxRow *b)    { return numeric_compare(a->dest_box, b->dest_box); }
    static bool sort_desc_destbox(const BoxRow *a, const BoxRow *b)   { return numeric_compare(b->dest_box, a->dest_box); }
    static bool sort_asc_destpos(const BoxRow *a, const BoxRow *b)    { return a->dest_pos < b->dest_pos; }
    static bool sort_desc_destpos(const BoxRow *a, const BoxRow *b)   { return a->dest_pos > b->dest_pos; }
    static bool sort_asc_site(const BoxRow *a, const BoxRow *b)       { return a->site_name.compare(b->site_name) < 0; }
    static bool sort_desc_site(const BoxRow *a, const BoxRow *b)      { return a->site_name.compare(b->site_name) > 0; }
    static bool sort_asc_position(const BoxRow *a, const BoxRow *b)   { return a->position < b->position; }
    static bool sort_desc_position(const BoxRow *a, const BoxRow *b)  { return a->position > b->position; }
    static bool sort_asc_shelf(const BoxRow *a, const BoxRow *b)      { return a->shelf_number < b->shelf_number; }
    static bool sort_desc_shelf(const BoxRow *a, const BoxRow *b)     { return a->shelf_number > b->shelf_number; }
    static bool sort_asc_vessel(const BoxRow *a, const BoxRow *b)     { return a->vessel_name.compare(b->vessel_name) < 0; }
    static bool sort_desc_vessel(const BoxRow *a, const BoxRow *b)    { return a->vessel_name.compare(b->vessel_name) > 0; }
    static bool sort_asc_structure(const BoxRow *a, const BoxRow *b)  { return numeric_compare(a->rack_name, b->rack_name); }//return a->rack_name.compare(b->rack_name) > 0; }
    static bool sort_desc_structure(const BoxRow *a, const BoxRow *b) { return a->rack_name.compare(b->rack_name) < 0; }
    static bool sort_asc_slot(const BoxRow *a, const BoxRow *b)       { return a->slot_position < b->slot_position; }
    static bool sort_desc_slot(const BoxRow *a, const BoxRow *b)      { return a->slot_position > b->slot_position; }

    // search func: strip out numeric chars from name, concatenate, compare as ints
    static bool numeric_compare(const string a, const string b) {
        struct temp { // Local functions are not allowed in C++, but local classes are and functions are allowed in local classes
            static int alpha_to_int(string a) {
                ostringstream numerics;
                for (int i=0; i<a.length(); i++) {
                    char ch = a.at(i); if (ch >= 0x30 && ch < 0x3A) { numerics << ch; } // pull out the numerics
                }
                return atoi(numerics.str().c_str());
            }
        } local;
        return local.alpha_to_int(a) < local.alpha_to_int(b);
        // what if there are no numerics?
    }
    string str() {
        ostringstream oss; oss<<__FUNC__
        //	LPDbCryovialStore: cryovialID, boxID, retrievalID, status, position
            <<". id: "<<(store_record->getID())<<", "
            //<<"status: "<<(store_record->getStatus())<<", "
            //storeID       storeID
            //retrievalID   retrievalID
        // LPDbCryovial: barcode, boxID, sampleID, typeID, storeID, retrievalID, status, position
            // ?
            //<<"barcode: "<<store_record->getBarcode()
            //<<"boxID"<<store_record->getBoxID()
            //<<"sampleID"<<store_record->getSampleID()
            //<<"aliquot type ID"<<store_record->getAliquotType()
            //<<"status"<<store_record->getStatus()<<", "
            //<<"position"<<store_record->getPosition()<<", "
        // BoxRow
            //<<"cryovial_barcode: "<<cryovial_barcode<<", "
            //<<"aliquot_type_name: "<<aliquot_type_name<<", "
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

class SampleRow {
public:
    LPDbCryovialStore * store_record;
    string              cryovial_barcode;
    string              aliquot_type_name;
    string              box_name;
    string              dest_box;
    string              dest_pos;
    string              site_name;
    int                 position;
    string              vessel_name;
    int                 shelf_number;
    string              rack_name;
    int                 slot_position;
    SampleRow() {}
    SampleRow(  LPDbCryovialStore * store_rec, string barcode, string aliquot, string box,
                string site, int pos, string vessel, int shelf, string rack, int slot) :
        store_record(store_rec), cryovial_barcode(barcode), aliquot_type_name(aliquot), box_name(box),
        site_name(site), position(pos), vessel_name(vessel), shelf_number(shelf), rack_name(rack), slot_position(slot) {}

    // box_name vs store_record->getID()?
    static bool sort_asc_barcode(const SampleRow *a, const SampleRow *b)    { return a->cryovial_barcode.compare(b->cryovial_barcode) > 0; }
    static bool sort_desc_barcode(const SampleRow *a, const SampleRow *b)   { return a->cryovial_barcode.compare(b->cryovial_barcode) < 0; }
    static bool sort_asc_currbox(const SampleRow *a, const SampleRow *b)    { return numeric_compare(a->box_name, b->box_name); }
    static bool sort_desc_currbox(const SampleRow *a, const SampleRow *b)   { return numeric_compare(b->box_name, a->box_name); }
    static bool sort_asc_currpos(const SampleRow *a, const SampleRow *b)    { return a->store_record->getPosition() < b->store_record->getPosition(); }
    static bool sort_desc_currpos(const SampleRow *a, const SampleRow *b)   { return a->store_record->getPosition() > b->store_record->getPosition(); }
    static bool sort_asc_destbox(const SampleRow *a, const SampleRow *b)    { return numeric_compare(a->dest_box, b->dest_box); }
    static bool sort_desc_destbox(const SampleRow *a, const SampleRow *b)   { return numeric_compare(b->dest_box, a->dest_box); }
    static bool sort_asc_destpos(const SampleRow *a, const SampleRow *b)    { return a->dest_pos < b->dest_pos; }
    static bool sort_desc_destpos(const SampleRow *a, const SampleRow *b)   { return a->dest_pos > b->dest_pos; }
    static bool sort_asc_site(const SampleRow *a, const SampleRow *b)       { return a->site_name.compare(b->site_name) < 0; }
    static bool sort_desc_site(const SampleRow *a, const SampleRow *b)      { return a->site_name.compare(b->site_name) > 0; }
    static bool sort_asc_position(const SampleRow *a, const SampleRow *b)   { return a->position < b->position; }
    static bool sort_desc_position(const SampleRow *a, const SampleRow *b)  { return a->position > b->position; }
    static bool sort_asc_shelf(const SampleRow *a, const SampleRow *b)      { return a->shelf_number < b->shelf_number; }
    static bool sort_desc_shelf(const SampleRow *a, const SampleRow *b)     { return a->shelf_number > b->shelf_number; }
    static bool sort_asc_vessel(const SampleRow *a, const SampleRow *b)     { return a->vessel_name.compare(b->vessel_name) < 0; }
    static bool sort_desc_vessel(const SampleRow *a, const SampleRow *b)    { return a->vessel_name.compare(b->vessel_name) > 0; }
    static bool sort_asc_structure(const SampleRow *a, const SampleRow *b)  { return numeric_compare(a->rack_name, b->rack_name); }//return a->rack_name.compare(b->rack_name) > 0; }
    static bool sort_desc_structure(const SampleRow *a, const SampleRow *b) { return a->rack_name.compare(b->rack_name) < 0; }
    static bool sort_asc_slot(const SampleRow *a, const SampleRow *b)       { return a->slot_position < b->slot_position; }
    static bool sort_desc_slot(const SampleRow *a, const SampleRow *b)      { return a->slot_position > b->slot_position; }

    // search func: strip out numeric chars from name, concatenate, compare as ints
    static bool numeric_compare(const string a, const string b) {
        struct temp { // Local functions are not allowed in C++, but local classes are and functions are allowed in local classes
            static int alpha_to_int(string a) {
                ostringstream numerics;
                for (int i=0; i<a.length(); i++) {
                    char ch = a.at(i); if (ch >= 0x30 && ch < 0x3A) { numerics << ch; } // pull out the numerics
                }
                return atoi(numerics.str().c_str());
            }
        } local;
        return local.alpha_to_int(a) < local.alpha_to_int(b);
        // what if there are no numerics?
    }
    string str() {
        ostringstream oss; oss<<__FUNC__
        //	LPDbCryovialStore: cryovialID, boxID, retrievalID, status, position
            <<". id: "<<(store_record->getID())<<", "
            <<"status: "<<(store_record->getStatus())<<", "
            //storeID       storeID
            //retrievalID   retrievalID
        // LPDbCryovial: barcode, boxID, sampleID, typeID, storeID, retrievalID, status, position
            // ?
            //<<"barcode: "<<store_record->getBarcode()
            //<<"boxID"<<store_record->getBoxID()
            //<<"sampleID"<<store_record->getSampleID()
            //<<"aliquot type ID"<<store_record->getAliquotType()
            //<<"status"<<store_record->getStatus()<<", "
            //<<"position"<<store_record->getPosition()<<", "
        // SampleRow
            <<"cryovial_barcode: "<<cryovial_barcode<<", "
            <<"aliquot_type_name: "<<aliquot_type_name<<", "
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
typedef SampleRow * pSampleRow;
typedef std::vector<pSampleRow> vecpSampleRow;

/** helper class to sort rows of type T via sort functions defined in intializer
    sort explicitly ascending or descending, or toggle last sort order */
template <class T>
class Sorter {
public:
    bool (*sort_func_asc)(const T *, const T *); // ascending sort function
    bool (*sort_func_dsc)(const T *, const T *); // descending sort function
    string description;
    void sort_asc(std::vector<T *> & vec) { sort(vec, ASCENDING);  }
    void sort_dsc(std::vector<T *> & vec) { sort(vec, DESCENDING); }
    void sort_toggle(std::vector<T *> & vec) {
        sort(vec, sortOrder);
        sortOrder = (sortOrder == ASCENDING) ? DESCENDING : ASCENDING; // toggle
    }
    //Sorter() { sortOrder = ASCENDING; } // compiler expects this ctor to be initialized??
private:
    enum SortOrder { ASCENDING, DESCENDING } sortOrder;
    void sort(std::vector<T *> & vec, SortOrder order) {
        switch (order) {
            case ASCENDING:     std::sort(vec.begin(), vec.end(), sort_func_asc); break;
            case DESCENDING:    std::sort(vec.begin(), vec.end(), sort_func_dsc); break;
            default:            throw Exception("Invalid sort order");
        }
    }
};

// chunk stringgrid setup
enum { SGCHUNKS_SECTION, SGCHUNKS_START,  SGCHUNKS_END, SGCHUNKS_SIZE, SGCHUNKS_NUMCOLS };// sgChunks_cols;
static const char * sgChunksColName[SGCHUNKS_NUMCOLS]   = { "Section", "Start", "End", "Size" };
static const int    sgChunksColWidth[SGCHUNKS_NUMCOLS]  = { 200, 200, 200, 200 };
class Chunk { // not recorded in database
public:
    Chunk() : section(0), start(0), end(0) { }
    int         section;
    //std::string name;
    int         start;
    int         end;
};
typedef std::vector< Chunk * >  vecpChunk;

// jobs grid setup
enum { SGJOBS_DESCRIP, SGJOBS_JOBTYPE, SGJOBS_STATUS, SGJOBS_PRIMARY, SGJOBS_PROJECT, SGJOBS_REASON, SGJOBS_TIMESTAMP, SGJOBS_NUMCOLS };
static const char * sgJobsColName[SGJOBS_NUMCOLS]   = { "Description", "Job type", "Status", "Primary Aliquot", "Project", "Reason", "Timestamp" };
static const int    sgJobsColWidth[SGJOBS_NUMCOLS]  = {401, 113, 72, 109, 100, 229, 127 };

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
private:
    //LCDbCryoJob * selectedJob;
    void debugLog(String s);
    tdvecpJob vecJobs;
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

//struct SampleLocation { // to include in SampleRow for each aliquot?
//    int dummy;
//};

//class Sorter {
//    int dummy;
//};
//    struct Sort1 : public Sorter {
//        bool operator()(const SampleRow &a, const SampleRow &b) const {
//            return a.position < b.position;
//        }
//    } sort1;

//     SGVIALS_BARCODE, SGVIALS_DESTBOX, SGVIALS_DESTPOS, SGVIALS_CURRBOX, SGVIALS_CURRPOS,
//    SGVIALS_SITE, SGVIALS_POSITION, SGVIALS_SHELF, SGVIALS_VESSEL, SGVIALS_STRUCTURE, SGVIALS_SLOT,
    //static bool less_than_(const SampleRow *a, const SampleRow *b) { return a-> < b->; }

//class RetrievalPlan : public LCDbID { // c_retrieval_plan
//    int                     retrieval_plan_cid;
//    std::string             name;
//    vecpChunk               chunks;
//    int                     status;
//    LCDbCryoJob::JobKind    jobType;
//public:
//    RetrievalPlan(std::string nm) : retrieval_plan_cid(0), name(nm) { }
//    void readChunks();
//    void deletePlan() { /* set 99 */ }
//    void setCID(int id) { retrieval_plan_cid = id; }
//    void setStatus(int st) { status = st; }
//    int getStatus() { return status; }
//    void setName(std::string nm) { name = nm; }
//    std::string getName() { return name; }
//    void addChunk(Chunk * ch) { chunks.push_back(ch); }
//    void popChunk() { delete chunks.back(); chunks.pop_back(); }
//};
//
//typedef std::vector<RetrievalPlan *> vecpRetrievalPlan;

//class RetrievalPlans : public LCDbID {
//class RetrievalPlans : public LDbCache< RetrievalPlan >, public LDbSingleton< RetrievalPlan > {
//    vecpRetrievalPlan plans;
//public:
//    void read();
//};
