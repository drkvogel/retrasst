#ifndef RetrievalAssistantBoxesH
#define RetrievalAssistantBoxesH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
#include <sstream>
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

enum {  SGBOXES_BOXNAME, SGBOXES_SITE, SGBOXES_POSITION, SGBOXES_SHELF, SGBOXES_VESSEL, SGBOXES_STRUCTURE, SGBOXES_SLOT, // location in "Russian Doll order"
        SGBOXES_NUMCOLS };
static const char * sgBoxesColName[SGBOXES_NUMCOLS] = {"Box name", "Site", "Position", "Shelf", "Vessel", "Structure", "Slot"};
static int sgBoxesColWidth[SGBOXES_NUMCOLS] = {266, 156, 74, 74, 262, 200, 78 };

//extern Sorter<BoxRow> boxSorter[SGBOXES_NUMCOLS];

class LoadBoxesWorkerThread : public TThread {
private:
protected:
    void __fastcall Execute();
public:
    __fastcall LoadBoxesWorkerThread();
    int             rowCount;       // current rows loaded, for thread sync
    void __fastcall updateStatus(); // syncronized method can't have args (?)
};

class TfrmBoxes : public TForm {
friend class LoadBoxesWorkerThread;
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
    TStringGrid *sgBoxes;
    TButton *btnSaveChunk;
    TButton *btnIncr;
    TButton *btnDecr;
    TMemo *memoDebug;
    TTimer *timerLoadBoxes;
    TPanel *panelLoading;
    TProgressBar *progressBottom;
    TButton *btnReject;
    TButton *Button1;
    TButton *Button2;
    TCheckBox *cbLog;
    TButton *btnAutoChunk;
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
    void __fastcall btnDecrClick(TObject *Sender);
    void __fastcall btnIncrClick(TObject *Sender);
    void __fastcall sgBoxesFixedCellClick(TObject *Sender, int ACol, int ARow);
    void __fastcall timerLoadBoxesTimer(TObject *Sender);
    void __fastcall sgBoxesClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall sgBoxesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall btnRejectClick(TObject *Sender);
    void __fastcall btnAutoChunkClick(TObject *Sender);
private:
    const char *        loadingMessage;
    LoadBoxesWorkerThread * loadBoxesWorkerThread;
    void __fastcall loadBoxesWorkerThreadTerminated(TObject *Sender);
    //int                         maxRows; // rows to show at a time
    LCDbCryoJob *               job;
    vecpBoxChunk                chunks;
    vecpBoxRow                  boxes;
    StringGridWrapper<BoxRow> *     sgwBoxes;
    StringGridWrapper<BoxChunk> *   sgwChunks;
    void                        sortList(int col);
    void                        addChunk();
    void                        showChunks();
    void                        autoChunk();
    void                        loadRows();
    void                        showRows();
    //void                        radgrpRowsChange();
    UINT TimerId;
    static VOID CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime);
        // Declaring the function static stops the __closure keyword being included
        // http://vogelworks.wordpress.com/?s=closure
public:
    void                setJob(LCDbCryoJob * ajob) { job = ajob; }; // this was not being called. why? it was declared in the defunct RetrievalManager.h
    __fastcall          TfrmBoxes(TComponent* Owner);
    void                debugLog(String s);
};

extern PACKAGE TfrmBoxes *frmBoxes;
#endif
