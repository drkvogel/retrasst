#ifndef RetrievalAssistantBoxesH
#define RetrievalAssistantBoxesH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.StdCtrls.hpp>
#include <sstream>
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "RetrievalAssistant.h"
#include "LCDbJob.h"
#include "LDbBoxStore.h"
//#include "LPDbCryovialStore.h"
//#include "LPDbCryovial.h"

/*
select * from box_content bc, c_box_size bs where bc.box_size_cid = bs.box_size_cid

select * from c_retrieval_job rj, cryovial_store cs where rj.retrieval_cid = cs.retrieval_cid

c_retrieval_job

c_box_retrieval
    box_id          # The box being retrieved (for box retrieval/disposal) or retrieved into (for sample retrieval/disposal)
    retrieval_cid   # The retrieval task this entry is part of
    retrieval_type  # obsolete
    section         # Which chunk of the retrieval plan this entry belongs to (0 = retrieve all boxes)
    position        # The position of this entry in that chunk (may be 0 for sample retrieval, i.e. use l_cryovial_retrieval position)
    box_name        # obsolete
    rj_box_cid      # Unique ID for this retrieval list entry (what does rj stand for?)
    status          # 0: new record; 1: part-filled, 2: collected; 3: not found; 99: record deleted

l_cryovial_retrieval
    rj_box_cid      # record id of c_box_retrieval entry for the box the sample should be placed into?
    position        # Where this sample appears in the current chunk of the retrieval plan
    cryovial_barcode# The barcode on the sample
    aliquot_type_cid# The aliquot type (cryovial_barcode + aliquot_type_cid should uniquely identify the cryovial within the project)
    slot_number     # The expected position of the cryovial in the destination box (if two records suggest the same position in the same box, the first should be the primary aliquot; the second will be ignored if the first is found)
    process_cid     # Who stored it, which program etc.
    status          # 0: expected; 1: ignored, 2: collected; 3: not found; 99: record deleted

LCDbCryoJob new fields
    int exercise, primary, secondary;
public:
	int getExerciseID() const { return exercise; }
	int getPrimaryAliquot() const { return primary; }
	void setPrimaryAliquot( int typeID ) { primary = typeID; }
	int getSecondaryAliquot() const { return secondary; }
	TDateTime getStartDate() const { return start_date; }
	TDateTime getClaimedUntil() const { return claimed_until; }
	TDateTime getFinishDate() const { return finish_date; }
*/


//	enum Status { NEW_JOB, INPROGRESS, DONE, DELETED = 99 };
//	enum JobKind { UNKNOWN, BOX_MOVE, BOX_RETRIEVAL, BOX_DISCARD, SAMPLE_RETRIEVAL, SAMPLE_DISCARD, NUM_TYPES };
class RetrievalPlan : public LCDbID { // c_retrieval_plan
    int                     retrieval_plan_cid;
    std::string             name;
    vecpChunk               chunks;
    int                     status;
    LCDbCryoJob::JobKind    jobType;
public:
    RetrievalPlan(std::string nm) : retrieval_plan_cid(0), name(nm) { }
    void readChunks();
    void deletePlan() { /* set 99 */ }
    void setCID(int id) { retrieval_plan_cid = id; }
    void setStatus(int st) { status = st; }
    int getStatus() { return status; }
    void setName(std::string nm) { name = nm; }
    std::string getName() { return name; }
    void addChunk(Chunk * ch) { chunks.push_back(ch); }
    void popChunk() { delete chunks.back(); chunks.pop_back(); }
};

typedef std::vector<RetrievalPlan *> vecpRetrievalPlan;

//class RetrievalPlans : public LCDbID {
//class RetrievalPlans : public LDbCache< RetrievalPlan >, public LDbSingleton< RetrievalPlan > {
//    vecpRetrievalPlan plans;
//public:
//    void read();
//};

typedef std::vector< LCDbBoxStore *> vecpBox;
//typedef std::vector< LPDbCryovialStore *> vecpVial;

class TfrmBoxes : public TForm {
__published:
    TGroupBox *groupList;
    TPanel *Panel1;
    TButton *btnSave;
    TButton *btnCancel;
    TSplitter *Splitter1;
    TPanel *Panel2;
    TButton *btnAddChunk;
    TButton *btnDelChunk;
    TStringGrid *sgChunks;
    TGroupBox *GroupBox1;
    TStringGrid *sgChunk;
    TButton *btnSaveChunk;
    TButton *btnIncr;
    TButton *btnDecr;
    TMemo *memoDebug;
    TCheckBox *cbLog;
    TRadioGroup *radgrpRows;
    TRadioButton *radbutAll;
    TRadioButton *radbutDefault;
    TRadioButton *radbutCustom;
    TEdit *editCustomRows;
    TTimer *timerCustomRows;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall btnAddChunkClick(TObject *Sender);
    void __fastcall btnDelChunkClick(TObject *Sender);
    void __fastcall sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall sgChunksSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
    void __fastcall cbLogClick(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall sgChunksFixedCellClick(TObject *Sender, int ACol, int ARow);
    void __fastcall editCustomRowsChange(TObject *Sender);
    void __fastcall timerCustomRowsTimer(TObject *Sender);
    void __fastcall radbutDefaultClick(TObject *Sender);
    void __fastcall radbutAllClick(TObject *Sender);
    void __fastcall radbutCustomClick(TObject *Sender);
    void __fastcall btnDecrClick(TObject *Sender);
    void __fastcall btnIncrClick(TObject *Sender);
private:
    //vecpRetrievalPlan plans;
    LCDbCryoJob * job;
    vecpChunk chunks;
    vecpBox boxes;
    //vecpVial vials;
    void autoChunk();
    //void loadChunks();
    void showChunks();
    void loadRows(int numrows);
    void radgrpRowsChange();
//    void loadPlans();
//    void showPlans();
public:
    __fastcall TfrmBoxes(TComponent* Owner);
    void setJob(LCDbCryoJob * ajob) { job = ajob; }
};

extern PACKAGE TfrmBoxes *frmBoxes;
#endif