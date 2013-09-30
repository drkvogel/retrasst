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
    void __fastcall updateStatus(); // synchronized methods can't have args
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
    TGroupBox *GroupBox3;
    TLabel *Label3;
    TLabel *Label2;
    TMainMenu *mainMenuMain;
    TMenuItem *menuItemExit;
    TGroupBox *Chunks;
    TStringGrid *sgChunks;
    TStringGrid *sgVials;
    TCheckBox *cbLog;
    TMemo *memoDebug;
    TPanel *panelLoading;
    TProgressBar *progressBottom;
    TTimer *timerLoadPlan;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall menuItemExitClick(TObject *Sender);
    void __fastcall cbLogClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall timerLoadPlanTimer(TObject *Sender);
private:
    LoadPlanWorkerThread * loadPlanWorkerThread;
    void __fastcall loadPlanWorkerThreadTerminated(TObject *Sender);
    LCDbCryoJob * job;
    vector< Chunk< SampleRow > *>               chunks;
    vecpSampleRow                               vials;
    StringGridWrapper< Chunk< SampleRow > > *   sgwChunks;
    StringGridWrapper<SampleRow> *              sgwVials;
    void                                        showChunks();
    Chunk< SampleRow > *                        currentChunk();
    void                                        showChunk(Chunk< SampleRow > * chunk=NULL);
    void                                        loadRows();
    void                                        addChunk(); // code smell - should be part of loadRows?
    //void                                        showRows();
    void                                        process();
    //int maxRows;
    const char *        loadingMessage;
public:
    void setJob(LCDbCryoJob * ajob) { job = ajob; }
    __fastcall TfrmProcess(TComponent* Owner);
};

extern PACKAGE TfrmProcess *frmProcess;
#endif
