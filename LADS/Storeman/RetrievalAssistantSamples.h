#ifndef RetrievalAssistantSamplesH
#define RetrievalAssistantSamplesH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
#include <sstream>
#include "LCDbJob.h"
#include "RetrievalAssistant.h"

using namespace std;

// spec: show
// cryovial barcode, destination box, position, current box, position, structure and location of the primary and secondary
/*
    cs.Cryovial_id, cs.Note_Exists, cs.retrieval_cid, cs.box_cid, cs.status, cs.cryovial_position,
SampleRow(  LPDbCryovialStore * store_rec, string barcode, string aliquot, string box,
                string site, int pos, string vessel, int shelf, string rack, int slot) :
        "   c.cryovial_barcode, t.external_name AS aliquot, b.external_name AS box,"
        "   s.external_name AS site, m.position, v.external_full AS vessel,"
        "   shelf_number, r.external_name AS rack, bs.slot_position" */

// encapsulate data about a stringgrid in a class?
class SgData {
    // key-value (stl?) lookup func instead of enums; map eg. "barcode" to (column) 4
    int colNum(std::string colName);
    TStringGrid *   sg;
    String          caption;
    enum            cols {};
    char *          colnames[];
    int             colwidths[];
};

// secondary aliquots: if defined, will be separate rows after all primary aliquots
// if any primaries fail, these will be marked to make a new chunk of replacements

enum {  SGVIALS_BARCODE, SGVIALS_DESTBOX, SGVIALS_DESTPOS, SGVIALS_CURRBOX, SGVIALS_CURRPOS,
        SGVIALS_SITE, SGVIALS_POSITION, SGVIALS_SHELF, SGVIALS_VESSEL, SGVIALS_STRUCTURE, SGVIALS_SLOT, // location in "Russian Doll order"
        SGVIALS_NUMCOLS};
static const char * sgVialColName[SGVIALS_NUMCOLS] = {"Barcode", "Dest box", "Pos", "Curr box", "Pos", "Site", "Position", "Shelf", "Vessel", "Structure", "Slot"};
static int sgVialColWidth[SGVIALS_NUMCOLS] = {102, 147, 43, 275, 37, 64, 50, 43, 100, 121, 40};

/*
"   cs.cryovial_id, cs.note_exists, cs.retrieval_cid, cs.box_cid, cs.status, cs.cryovial_position,"
"   c.cryovial_barcode, t.external_name AS aliquot, b.external_name AS box,"
"   s.external_name AS site, m.position, v.external_full AS vessel,"
"   shelf_number, r.external_name AS rack, bs.slot_position"
" FROM"
"   cryovial c, cryovial_store cs, box_name b, box_store bs, c_rack_number r,"
"   c_tank_map m, c_object_name s,"   // site
"   c_object_name v,"                 // vessel
"   c_object_name t"                  // aliquot? */

class LoadVialsWorkerThread : public TThread {
private:
protected:
    void __fastcall Execute();
public:
    __fastcall LoadVialsWorkerThread();
    int             rowCount;       // current rows loaded, for thread sync
    string          loadingMessage;
    void __fastcall updateStatus(); // syncronized method can't have args (?) - was going to use (int numerator, int denominator)
};

extern Sorter<SampleRow> sorter[SGVIALS_NUMCOLS];

class TfrmSamples : public TForm {
    friend class LoadVialsWorkerThread;
__published:
    TSplitter *Splitter1;
    TGroupBox *groupList;
    TPanel *Panel2;
    TButton *btnAddChunk;
    TButton *btnDelChunk;
    TButton *btnIncr;
    TButton *btnDecr;
    TStringGrid *sgChunks;
    TGroupBox *groupVials;
    TStringGrid *sgVials;
    TMemo *memoDebug;
    TTimer *timerCustomRows;
    TButton *btnAutoChunk;
    TPanel *panelLoading;
    TProgressBar *progressBottom;
    TTimer *timerLoadVials;
    TPanel *Panel3;
    TGroupBox *groupSort;
    TButton *btnCancel;
    TButton *btnSave;
    TButton *btnReject;
    TCheckBox *cbLog;
    TGroupBox *GroupBox2;
    TRadioButton *radbutDefault;
    TRadioButton *radbutAll;
    TRadioButton *radbutCustom;
    TEdit *editCustomRows;
    TButton *btnDelSort;
    TButton *btnAddSort;
    TButton *btnApplySort;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall btnAddChunkClick(TObject *Sender);
    void __fastcall cbLogClick(TObject *Sender);
    void __fastcall btnDelChunkClick(TObject *Sender);
    void __fastcall radbutDefaultClick(TObject *Sender);
    void __fastcall radbutAllClick(TObject *Sender);
    void __fastcall radbutCustomClick(TObject *Sender);
    void __fastcall timerCustomRowsTimer(TObject *Sender);
    void __fastcall editCustomRowsChange(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall btnAutoChunkClick(TObject *Sender);
    void __fastcall btnIncrClick(TObject *Sender);
    void __fastcall btnDecrClick(TObject *Sender);
    void __fastcall sgVialsFixedCellClick(TObject *Sender, int ACol, int ARow);
    void __fastcall sgVialsClick(TObject *Sender);
    void __fastcall timerLoadVialsTimer(TObject *Sender);
    void __fastcall btnRejectClick(TObject *Sender);
    void __fastcall btnAddSortClick(TObject *Sender);
    void __fastcall btnDelSortClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall sgChunksClick(TObject *Sender);
private:
    void __fastcall             comboSortOnChange(TObject *Sender);
    LoadVialsWorkerThread *     loadVialsWorkerThread;
    void __fastcall             loadVialsWorkerThreadTerminated(TObject *Sender);
    LCDbCryoJob *               job;
    std::vector<SampleChunk *>  chunks;
    std::vector<SampleRow *>    vials;      // all vials loaded
    int                         maxRows;    // rows to show at a time
    void                        addSorter();
    void                        applySort();
    void                        autoChunk();
    SampleChunk *               currentChunk();
    void                        showChunks();
    void                        loadRows();
    void                        showChunk(SampleChunk * chunk=NULL);
    void                        radgrpRowsChange();
    void                        sortChunk(SampleChunk * chunk, int col);
    const char *                loadingMessage;
public:
    __fastcall          TfrmSamples(TComponent* Owner);
    void                debugLog(String s);
    void                setJob(LCDbCryoJob * ajob) { job = ajob; };
    void                addChunk();
};
    //void                sortList(enum SampleRow::SortType);
    //void                sortList(void *); // function argument
    //void                sortList(Sorter sorter); // struct argument - structs could be encapsulated in e.g. SampleRow
    //void                sortList(int sortType); //

extern PACKAGE TfrmSamples *frmSamples;
#endif
