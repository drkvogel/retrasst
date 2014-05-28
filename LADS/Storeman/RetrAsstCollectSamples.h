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

#define PlanThread 25

class LoadPlanThread : public TThread {
protected:
    void __fastcall Execute();
public:
    __fastcall LoadPlanThread();
    //Chunk< SampleRow > * loadingChunk;
    //LCDbCryoJob *   job;
    int             rowCount; // class variable needed for synchronise
    string          loadingMessage;
    string          debugMessage;
    void            addSampleDetails(SampleRow * row);
    void __fastcall updateStatus(); // synchronized methods can't have args
    void __fastcall debugLog();
    void __fastcall msgbox();
};

class SaveProgressThread : public TThread {
protected:
    void __fastcall Execute();
public:
    __fastcall SaveProgressThread();
    void            storeSample(SampleRow * sample);
    void            jobFinished();
    //int             rowCount;
    string          loadingMessage;
    string          debugMessage;
    void __fastcall updateStatus();
    void __fastcall debugLog();
    void __fastcall msgbox();
};

class TfrmRetrAsstCollectSamples : public TForm {
    friend class LoadPlanThread;
    friend class SaveProgressThread;
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
    TLabel *labelDestype;
    TLabel *Label6;
    TGroupBox *groupChunkKey;
    TLabel *labelNew;
    TLabel *labelInProgress;
    TLabel *labelDone;
    TGroupBox *groupVialKey;
    TLabel *labelVialKeyCollected;
    TLabel *labelVialKeyExpectedBackup;
    TLabel *labelVialKeyExpectedPrimary;
    TLabel *labelVialKeyIgnored;
    TLabel *labelVialKeyNotFound;
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
	vector<string> 								info;
	vector<string> 								warnings;
	vector<string> 								errors;
    LoadPlanThread *                            loadPlanThread;
    void __fastcall                             loadPlanThreadTerminated(TObject *Sender);
    SaveProgressThread *                        saveProgressThread;
    void __fastcall                             saveProgressThreadTerminated(TObject *Sender);
    LCDbCryoJob *                               job;
    vector< Chunk< SampleRow > *>               chunks;
    //vecpSampleRow                               vials;
    vecpSampleRow                               primaries;
    vecpSampleRow                               secondaries;
    vecpSampleRow                               combined;
    StringGridWrapper< Chunk< SampleRow > > *   sgwChunks; //std::auto_ptr< StringGridWrapper< Chunk< SampleRow > > >  sgwChunks;
    StringGridWrapper<SampleRow> *              sgwVials;
    void                                        showChunks();
    void                                        loadPlan();
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
    const char *                                progressMessage;
    void                                        prepareProgressMessage(const char * loadingMessage);
    void                                        debugLog(String s);
    //bool                                        isChunkComplete(Chunk< SampleRow > * chunk);
    void                                        chunkComplete(Chunk< SampleRow > * chunk);
    bool                                        isJobComplete();
    std::set< int >                             emptyBoxes;
    void                                        discardBoxes();
    void                                        checkExit();
	void                                        exit();
	void 										storeSample(SampleRow * sample);
	void										jobFinished();
    bool                                        destroying;  // for FormResize
    bool                                        unactionedSamples;
public:
    void                                        setJob(LCDbCryoJob * ajob) { job = ajob; }
    __fastcall TfrmRetrAsstCollectSamples(TComponent* Owner);
    __fastcall ~TfrmRetrAsstCollectSamples();
};

extern PACKAGE TfrmRetrAsstCollectSamples *frmRetrAsstCollectSamples;
#endif
