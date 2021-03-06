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
#include <Vcl.Buttons.hpp>
#include "LCDbJob.h"
#include "RetrAsstMain.h"
#include "RetrAsstWrongBarcode.h"


class TfrmRetrAsstCollectSamples : public TForm {
    friend class RetrAsstThread;
    friend class LoadPlanThread;
    friend class SaveProgressThread;
    friend class TfrmWrongBarcode;
__published:
    TGroupBox *groupRetrievalList;
    TGroupBox *groupSignOff;
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
    TPanel *panelDebug;
    TMemo *memoDebug;
    TButton *btnSimAccept;
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
    TPanel *Panel1;
    TPanel *Panel2;
    TLabel *Label1;
    TPanel *panelDetails;
    TGroupBox *groupSource;
    TLabel *labelSite;
    TLabel *labelVessel;
    TLabel *labelStructure;
    TLabel *labelSlot;
    TLabel *labelSrcPos;
    TLabel *labelSrcBox;
    TGroupBox *groupSampleID;
    TLabel *labelSampleID;
    TPanel *panelAliquots;
    TGroupBox *groupPrimary;
    TLabel *labelPrimary;
    TGroupBox *groupSecondary;
    TLabel *labelSecondary;
    TGroupBox *groupDest;
    TLabel *labelDestString;
    TLabel *labelDestPos;
    TLabel *labelDestBox;
    TLabel *labelStorage;
    TBitBtn *btnAccept;
    TBitBtn *btnNotFound;
    TBitBtn *btnDefer;
    TCheckBox *cbLog;
    TBitBtn *btnAddNote;
    TBitBtn *btnSignOff;
    TBitBtn *btnSave;
    TBitBtn *btnBadVial;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall sgChunksFixedCellClick(TObject *Sender, int ACol, int ARow);
    void __fastcall sgChunksClick(TObject *Sender);
    void __fastcall sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall sgVialsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall sgVialsClick(TObject *Sender);
    void __fastcall menuItemExitClick(TObject *Sender);
    void __fastcall cbLogClick(TObject *Sender);
    void __fastcall timerLoadPlanTimer(TObject *Sender);
    void __fastcall btnAcceptClick(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall btnDeferClick(TObject *Sender);
    void __fastcall btnSimAcceptClick(TObject *Sender);
    void __fastcall btnNotFoundClick(TObject *Sender);
    void __fastcall btnBadVialClick(TObject *Sender);
    void __fastcall btnAddNoteClick(TObject *Sender);
    void __fastcall editBarcodeKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
    void __fastcall btnSignOffClick(TObject *Sender);
    void __fastcall sgVialsContextPopup(TObject *Sender, TPoint &MousePos, bool &Handled);

    //void __fastcall btnWrongVialClick(TObject *Sender);
    //void __fastcall btnFoundElsewhereClick(TObject *Sender);
    //void __fastcall btnAlreadyRetrievedClick(TObject *Sender);
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
    void                                        fillRow(SampleRow * sampleRow, int rw); // shared_ptr < SampleRow > row,
    void                                        loadRows();
    void                                        addChunk(int number, int row);
    void                                        showCurrentRow();
    void                                        showDetails(SampleRow * sample);
    void                                        accept(string barcode);
    void                                        replace(string barcode);
    void                                        notFound();
    void                                        addNote();
    //void                                        alreadyRetrieved();
    void                                        badVial();
    void                                        skip();
    //void                                        foundElsewhere();
    //void                                        wrongVial();
    void                                        nextRow();
    void                                        saveProgress();
    void                                        showProgressMessage(const char * loadingMessage);
    void                                        hideProgressMessage();
    void                                        debugLog(String s);
    void                                        chunkCompleted(Chunk< SampleRow > * chunk);
    bool                                        isJobComplete();
    void                                        collectEmpties();
    std::set< int >                             emptyBoxes;
    //bool                                        unactionedSamples;
    void                                        checkExit();
	void                                        exit();
	void										closeJob();
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
    void            updateStorage();
    void            updateStorage(SampleRow * aliquot, LQuery & q);
    void            findEmpties();
};

extern PACKAGE TfrmRetrAsstCollectSamples *frmRetrAsstCollectSamples;
#endif

    //Chunk< SampleRow > * loadingChunk;
    //__fastcall SaveProgressThread();
        //__fastcall LoadPlanThread();

