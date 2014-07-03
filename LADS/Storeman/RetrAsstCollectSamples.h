#ifndef RetrAsstCollectSamplesH
#define RetrAsstCollectSamplesH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "LCDbJob.h"
#include "RetrAsstMain.h"

class TfrmRetrAsstCollectSamples : public TForm {
    friend class RetrAsstThread;
    friend class LoadPlanThread;
    friend class SaveProgressThread;
__published:
    TGroupBox *groupRetrievalList;
    TGroupBox *groupSignOff;
    TButton *btnAccept;
    TButton *btnSkip;
    TEdit *editBarcode;
    TMainMenu *mainMenuMain;
    TMenuItem *menuItemExit;
    TGroupBox *Chunks;
    TStringGrid *sgChunks;
    TStringGrid *sgVials;
    TPanel *panelLoading;
    TProgressBar *progressBottom;
    TTimer *timerLoadPlan;
    TSplitter *Splitter1;
    TGroupBox *groupDetails;
    TPanel *panelDebug;
    TMemo *memoDebug;
    TButton *btnSimAccept;
    TButton *btnNotFound;
    TPanel *panelBarcode;
    TSplitter *Splitter2;
    TSplitter *splitterDebug;
    TPanel *panelLegends;
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
    TGroupBox *groupSampleID;
    TLabel *labelSampleID;
    TPanel *panelAliquots;
    TGroupBox *groupPrimary;
    TLabel *labelPrimary;
    TGroupBox *groupSecondary;
    TLabel *labelSecondary;
    TGroupBox *groupSource;
    TGroupBox *groupDest;
    TLabel *labelDestString;
    TLabel *labelStorage;
    TPanel *Panel1;
    TPanel *Panel2;
    TLabel *Label1;
    TLabel *Label2;
    TButton *btnExit;
    TCheckBox *cbLog;
    TLabel *labelDestPos;
    TLabel *labelDestBox;
    TLabel *labelSite;
    TLabel *labelVessel;
    TLabel *labelStructure;
    TLabel *labelSlot;
    TLabel *labelPosInBox;
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
    void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
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
    void                                        saveProgress();
    void                                        showProgressMessage(const char * loadingMessage);
    void                                        debugLog(String s);
    void                                        chunkCompleted(Chunk< SampleRow > * chunk);
    bool                                        isJobComplete();
    void                                        collectEmpties();
    std::set< int >                             emptyBoxes;
    bool                                        unactionedSamples;
    void                                        checkExit();
	void                                        exit();
	void 										storeSample(SampleRow * sample);
	void										jobFinished();
    bool                                        destroying;  // for FormResize
    void                                        toggleLog();
    void                                        flash(TGroupBox *box, TColor other);
public:
    void                                        setJob(LCDbCryoJob * ajob) { job = ajob; }
    __fastcall TfrmRetrAsstCollectSamples(TComponent* Owner);
    __fastcall ~TfrmRetrAsstCollectSamples();
};


class RetrAsstThread : public TThread {
protected:
    RetrAsstThread(); // : TThread(false);
    TfrmRetrievalAssistant      * main;
    TfrmRetrAsstCollectSamples  * collect;
    string          messageTitle; // can't use args for synced method, don't know why
    string          messageBody;  // so need these temp strings
public:
    //virtual
    void            debugLog(string body);
    void            _debugLog( ); // synchronized methods can't have args
    void            updateStatus(string body, string title);
    void            _updateStatus();
    void            msgbox(string body, string title);
    void            _msgbox();
};

class LoadPlanThread : public RetrAsstThread {
    void __fastcall Execute();
public:
    int             rowCount; // class variable needed for synchronise
    void            addSampleDetails(SampleRow * row);
};

class SaveProgressThread : public RetrAsstThread {
    void __fastcall Execute();
    void            storeSample(SampleRow * sample);
    void            jobFinished();
};

extern PACKAGE TfrmRetrAsstCollectSamples *frmRetrAsstCollectSamples;
#endif

    //Chunk< SampleRow > * loadingChunk;
    //__fastcall SaveProgressThread();
        //__fastcall LoadPlanThread();

