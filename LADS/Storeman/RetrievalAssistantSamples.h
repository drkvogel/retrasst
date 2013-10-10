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
    //friend class TfrmAutoChunk;
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
    TCheckBox *cbSnapToBoxes;
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
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall sgChunksFixedCellClick(TObject *Sender, int ACol, int ARow);

private:
    LoadVialsWorkerThread *                     loadVialsWorkerThread;
    void __fastcall                             loadVialsWorkerThreadTerminated(TObject *Sender);
    LCDbCryoJob *                               job;
    vector< Chunk< SampleRow > *>               chunks;
    StringGridWrapper<SampleRow> *              sgwVials;
    StringGridWrapper< Chunk< SampleRow > > *   sgwChunks;
    void                                        addSorter();
    void                                        removeSorter();
    void                                        applySort();
    void                                        autoChunk();
    Chunk< SampleRow > *                        currentChunk();
    void                                        showChunks();
    void                                        loadRows();
    void                                        showChunk(Chunk< SampleRow > * chunk=NULL);
    const char *                                loadingMessage;
public:
    vecpSampleRow                               vials;      // all vials in job - why public??
    __fastcall                                  TfrmSamples(TComponent* Owner);
    void                                        debugLog(String s);
    void                                        setJob(LCDbCryoJob * ajob) { job = ajob; };
    bool                                        addChunk(unsigned int startrow);//, unsigned int size=0);
};

extern PACKAGE TfrmSamples *frmSamples;
#endif
