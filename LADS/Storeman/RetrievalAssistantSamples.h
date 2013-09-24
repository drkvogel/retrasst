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
#include <iterator>
#include "LCDbJob.h"
#include "RetrievalAssistant.h"

using namespace std;

// cryovial barcode, destination box, position, current box, position, structure and location of the primary and secondary
// secondary aliquots: if defined, will be separate rows after all primary aliquots
// if any primaries fail, these will be marked to make a new chunk of replacements

class LoadVialsWorkerThread : public TThread {
protected:
    void __fastcall Execute();
public:
    __fastcall LoadVialsWorkerThread();
    int             rowCount;
    string          loadingMessage;
    void __fastcall updateStatus(); // synchronized methods can't have args
};

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
    TButton *btnDelSort;
    TButton *btnAddSort;
    TButton *btnApplySort;
    TSplitter *splitterDebug;
    TMemo *memoDebug;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall btnAddChunkClick(TObject *Sender);
    void __fastcall cbLogClick(TObject *Sender);
    void __fastcall btnDelChunkClick(TObject *Sender);
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
    void __fastcall btnApplySortClick(TObject *Sender);
    void __fastcall sgVialsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall sgChunksSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
    void __fastcall sgChunksGetEditText(TObject *Sender, int ACol, int ARow, UnicodeString &Value);
    void __fastcall sgVialsDblClick(TObject *Sender);
private:
    LoadVialsWorkerThread *     loadVialsWorkerThread;
    void __fastcall             loadVialsWorkerThreadTerminated(TObject *Sender);
    LCDbCryoJob *               job;
    std::vector<SampleChunk *>  chunks;
    vecpSampleRow               vials;      // all vials in job
    StringGridWrapper<SampleRow> *      sgwVials;
    StringGridWrapper<SampleChunk> *    sgwChunks;
    void                        addSorter();
    void                        removeSorter();
    void                        applySort();
    void                        autoChunk();
    SampleChunk *               currentChunk();
    void                        showChunks();
    void                        loadRows();
    void                        showChunk(SampleChunk * chunk=NULL);
    //void                        radgrpRowsChange();
    //void                        sortChunk(SampleChunk * chunk, int col, Sorter<SampleRow *>::SortOrder order);
    const char *                loadingMessage;
public:
    __fastcall                  TfrmSamples(TComponent* Owner);
    void                        debugLog(String s);
    void                        setJob(LCDbCryoJob * ajob) { job = ajob; };
    void                        addChunk();
};

extern PACKAGE TfrmSamples *frmSamples;
#endif
