#ifndef RetrAsstCollectSamplesH
#define RetrAsstCollectSamplesH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include "LCDbJob.h"
#include "RetrAsstMain.h"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

#define DEFAULT_NUMROWS 25

//class LoadSamplesWorkerThread : public TThread {
class LoadPlanWorkerThread : public TThread {
protected:
    void __fastcall Execute();
public:
    __fastcall LoadPlanWorkerThread();
    void NotUsingTempTable();
    //void UsingTempTable();
    Chunk< SampleRow > * loadingChunk;
    LCDbCryoJob *   job;
    int             rowCount; // class variable needed for synchronise
    string          loadingMessage;
    string          debugMessage;
    void __fastcall updateStatus(); // synchronized methods can't have args
    void __fastcall debugLog();
    void __fastcall msgbox();
    bool stats;
};

class LoadChunksWorkerThread : public TThread {
protected:
    void __fastcall Execute();
public:
    __fastcall LoadChunksWorkerThread();
    Chunk< SampleRow > * loadingChunk;
    int             rowCount;
    string          loadingMessage;
    string          debugMessage;
    void __fastcall updateStatus(); // synchronized methods can't have args
    void __fastcall debugLog();
    void __fastcall msgbox();
    bool stats;
};

class TfrmProcess : public TForm {
    friend class LoadPlanWorkerThread;
__published:
    TGroupBox *groupRetrievalList;
    TGroupBox *groupSignOff;
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
    TGroupBox *groupDetails;
    TButton *btnExit;
    TPanel *panelDebug;
    TMemo *memoDebug;
    TButton *btnSimAccept;
    TButton *btnNotFound;
    TPanel *panelAliquots;
    TGroupBox *groupPrimary;
    TLabel *labelPrimary;
    TGroupBox *groupSecondary;
    TLabel *labelSecondary;
    TPanel *Panel1;
    TLabel *Label2;
    TLabel *labelStorage;
    TLabel *labelDestbox;
    TLabel *Label4;
    TPanel *Panel2;
    TLabel *Label3;
    TLabel *labelSampleID;
    TSplitter *Splitter2;
    TSplitter *splitterDebug;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall menuItemExitClick(TObject *Sender);
    void __fastcall cbLogClick(TObject *Sender);
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
    void __fastcall editBarcodeKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall sgVialsClick(TObject *Sender);
private:
    LoadPlanWorkerThread *                      loadPlanWorkerThread;
    void __fastcall                             loadPlanWorkerThreadTerminated(TObject *Sender);
    LCDbCryoJob *                               job;
    vector< Chunk< SampleRow > *>               chunks;
    vecpSampleRow                               vials;
    StringGridWrapper< Chunk< SampleRow > > *   sgwChunks; //std::auto_ptr< StringGridWrapper< Chunk< SampleRow > > >  sgwChunks;
    StringGridWrapper<SampleRow> *              sgwVials;
    void                                        getStorage(SampleRow * sample);
    void                                        showChunks();
    void                                        loadChunk();
    Chunk< SampleRow > *                        currentChunk();
    SampleRow *                                 currentSample();
    SampleRow *                                 currentAliquot();
    void                                        showChunk(Chunk< SampleRow > * chunk=NULL);
    void                                        fillRow(SampleRow * sampleRow, int rw);
    void                                        loadRows();
    void                                        addChunk(int number, int row);
    void                                        showCurrentRow();
    void                                        showDetails(SampleRow * sample);
    void                                        accept(String barcode);
    void                                        notFound();
    void                                        skip();
    void                                        nextRow();
    void                                        collectEmpties();
    void                                        exit();
    const char *                                loadingMessage;
    void                                        debugLog(String s);
    bool destroying;
    string tempTableName;
public:
    void setJob(LCDbCryoJob * ajob) { job = ajob; }
    __fastcall TfrmProcess(TComponent* Owner);
    __fastcall ~TfrmProcess();
};

extern PACKAGE TfrmProcess *frmProcess;
#endif
