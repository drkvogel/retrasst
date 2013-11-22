#ifndef RetrievalAssistantProcessH
#define RetrievalAssistantProcessH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include "LCDbJob.h"
#include "RetrievalAssistant.h"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

#define DEFAULT_NUMROWS 25

class LoadPlanWorkerThread : public TThread {
protected:
    void __fastcall Execute();
public:
    __fastcall LoadPlanWorkerThread();
    int             rowCount;
    string          loadingMessage;
    string          debugMessage;
    void __fastcall updateStatus(); // synchronized methods can't have args
    void __fastcall debugLog(); // synchronized methods can't have args
};

class TfrmProcess : public TForm {
    friend class LoadPlanWorkerThread;
__published:
    TGroupBox *List;
    TGroupBox *GroupBox2;
    TButton *btnAccept;
    TButton *btnSkip;
    TEdit *editBarcode;
    TLabel *Label1;
    TMainMenu *mainMenuMain;
    TMenuItem *menuItemExit;
    TGroupBox *Chunks;
    TStringGrid *sgChunks;
    TStringGrid *sgVials;
    TCheckBox *cbLog;
    TPanel *panelLoading;
    TProgressBar *progressBottom;
    TTimer *timerLoadPlan;
    TSplitter *Splitter1;
    TGroupBox *GroupBox3;
    TLabel *Label3;
    TButton *btnExit;
    TLabel *labelStorage;
    TLabel *labelSampleID;
    TPanel *panelDebug;
    TMemo *memoDebug;
    TButton *btnSimAccept;
    TButton *btnNotFound;
    TLabel *Label2;
    TLabel *Label4;
    TLabel *labelDestbox;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall menuItemExitClick(TObject *Sender);
    void __fastcall cbLogClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall timerLoadPlanTimer(TObject *Sender);
    void __fastcall btnAcceptClick(TObject *Sender);
    void __fastcall sgChunksFixedCellClick(TObject *Sender, int ACol, int ARow);
    void __fastcall sgChunksClick(TObject *Sender);
    void __fastcall sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall sgVialsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall btnSkipClick(TObject *Sender);
    void __fastcall btnSimAcceptClick(TObject *Sender);
    void __fastcall btnNotFoundClick(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:
    LoadPlanWorkerThread *                      loadPlanWorkerThread;
    void __fastcall                             loadPlanWorkerThreadTerminated(TObject *Sender);
    LCDbCryoJob *                               job;
    vector< Chunk< SampleRow > *>               chunks;
    vecpSampleRow                               vials;
    StringGridWrapper< Chunk< SampleRow > > *   sgwChunks;
    StringGridWrapper<SampleRow> *              sgwVials;
    void                                        showChunks();
    Chunk< SampleRow > *                        currentChunk();
    SampleRow *                                 currentSample();
    void                                        showChunk(Chunk< SampleRow > * chunk=NULL);
    //Chunk< SampleRow >::Status                  chunkStatus(Chunk< SampleRow > * chunk);
    void                                        loadRows();
    void                                        addChunk(int row);
    //void                                        addChunks();
    void                                        process();
    void                                        showCurrentRow();
    void                                        showRowDetails(SampleRow * sample);
    void                                        accept(String barcode);
    void                                        nextRow();
    void                                        exit();
    //int                                         currentRow;
    //int                                         currentChunk;
    //int maxRows;
    const char *                                loadingMessage;
    void                                        debugLog(String s);
public:
    void setJob(LCDbCryoJob * ajob) { job = ajob; }
    __fastcall TfrmProcess(TComponent* Owner);
};

extern PACKAGE TfrmProcess *frmProcess;
#endif
