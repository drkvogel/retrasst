#ifndef RetrievalAssistantSamplesH
#define RetrievalAssistantSamplesH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <sstream>
#include "LCDbJob.h"
#include "RetrievalAssistant.h"

using namespace std;

//class Chunk { // not recorded in database
//public:
//    //Chunk() : section(0), retrieval_cid(0), name(""), start(0), end(0) { }
//    Chunk() : section(0), start(0), end(0) { }
//    int         section;
//    //int         retrieval_cid;
//    //int         exercise_cid;
//    //std::string name;
//    int         start;
//    int         end;
//    //string      descrip;
//    //int         job_type;
//    //int         project_cid;
//    //int         primary_aliquot;
//};
//
//typedef std::vector< Chunk * >  vecpChunk;

// spec: show
// cryovial barcode, destination box, position, current box, position, structure and location of the primary and secondary
/*
    cs.Cryovial_id, cs.Note_Exists, cs.retrieval_cid, cs.box_cid, cs.status, cs.cryovial_position,
SampleRow(  LPDbCryovialStore * store_rec, string barcode, string aliquot, string box,
                string site, int pos, string vessel, int shelf, string rack, int slot) :
        "   c.cryovial_barcode, t.external_name AS aliquot, b.external_name AS box,"
        "   s.external_name AS site, m.position, v.external_full AS vessel,"
        "   shelf_number, r.external_name AS rack, bs.slot_position"
                */

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

// vials
enum {
    SGVIALS_BARCODE, SGVIALS_DESTBOX, SGVIALS_DESTPOS, SGVIALS_CURRBOX, SGVIALS_LOCATION,
    // site/vessel/
    // secondary aliquots if defined?
    SGVIALS_NUMCOLS
};
static const char * sgVialColName[SGVIALS_NUMCOLS] = {
    "Barcode",
    "Dest box",
    "Pos",
    "Curr box",
    //"Pos",
    //"Structure",
    "Location"
};
//static int sgVialColWidth[SGVIALS_NUMCOLS] = { 100, 100, 30, 100, 30, 100, 100 };
static int sgVialColWidth[SGVIALS_NUMCOLS] = {102, 156, 43, 195, 461 };
//typedef std::vector< LPDbCryovialStore *> vecpVial;

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

class TfrmSamples : public TForm
{
__published:	// IDE-managed Components
    TSplitter *Splitter1;
    TGroupBox *groupList;
    TPanel *Panel2;
    TButton *btnAddChunk;
    TButton *btnDelChunk;
    TButton *btnIncr;
    TButton *btnDecr;
    TStringGrid *sgChunks;
    TPanel *Panel1;
    TButton *btnSave;
    TButton *btnCancel;
    TCheckBox *cbLog;
    TRadioGroup *radgrpRows;
    TRadioButton *radbutAll;
    TRadioButton *radbutDefault;
    TRadioButton *radbutCustom;
    TEdit *editCustomRows;
    TGroupBox *groupVials;
    TStringGrid *sgVials;
    TMemo *memoDebug;
    TTimer *timerCustomRows;
    TButton *btnAutoChunk;
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
private:
    LCDbCryoJob * job;
    int                 maxRows; // rows to show at a time
    vecpChunk           chunks;
    //vecpSampleRow       vials;
    std::vector<SampleRow *> vials;
    void                autoChunk();
    void                showChunks();
    void                loadRows();
    void                showRows();
    void                radgrpRowsChange();
    //void                sortList(enum SampleRow::SortType);
    //void                sortList(void *); // function argument
    //void                sortList(Sorter sorter); // struct argument - structs could be encapsulated in e.g. SampleRow
    void                sortList(int sortType); //
public:
    __fastcall          TfrmSamples(TComponent* Owner);
    void                debugLog(String s);
    void                setJob(LCDbCryoJob * ajob) { job = ajob; }
    bool                autochunk;
    void                addChunk();
};

extern PACKAGE TfrmSamples *frmSamples;
#endif
