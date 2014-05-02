#ifndef RetrAsstPlanSamplesH
#define RetrAsstPlanSamplesH
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
#include "RetrAsstMain.h"

using namespace std;

class LoadVialsJobThread : public TThread {
protected:
    void __fastcall Execute();
    void load();
    LCDbCryoJob *   job;
    int             rowCount;
    string          loadingMessage;
    void __fastcall updateStatus(); // synchronized methods can't have args
    string          debugMessage;
    void __fastcall debugLog();
    void            combineAliquots(const vecpSampleRow & primaries, const vecpSampleRow & secondaries, vecpSampleRow & combined);
public:
    __fastcall LoadVialsJobThread();
};

class SavePlanThread : public TThread {
protected:
    void __fastcall Execute();
    void save();
    //LCDbCryoJob *   job;
    int             rowCount;
    string          loadingMessage;
    void __fastcall updateStatus();
    string          debugMessage;
    void __fastcall debugLog();
public:
    __fastcall SavePlanThread();
};

class TfrmRetrAsstPlanSamples : public TForm {
    friend class LoadVialsJobThread;
    friend class SavePlanThread;
__published:
    TSplitter *Splitter1;
    TTimer *timerLoadVials;
    TSplitter *splitterDebug;
    TPanel *panelDebug;
    TMemo *memoDebug;
    TStringGrid *sgDebug;
    TSplitter *Splitter2;
    TTimer *timerCalculate;
    TPanel *panelChunks;
    TGroupBox *groupList;
    TPanel *Panel2;
    TButton *btnCancel;
    TButton *btnSave;
    TButton *btnReject;
    TCheckBox *cbLog;
    TStringGrid *sgChunks;
    TPanel *panelVials;
    TGroupBox *groupVials;
    TStringGrid *sgVials;
    TPanel *panelLoading;
    TProgressBar *progressBottom;
    TGroupBox *groupSort;
    TButton *btnDelSort;
    TButton *btnAddSort;
    TButton *btnApplySort;
    TPanel *Panel1;
    TGroupBox *groupAutoChunk;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TEdit *editMaxSize;
    TComboBox *comboSectionSize;
    TButton *btnAddChunk;
    TEdit *editDestBoxSize;
    TButton *btnAddAllChunks;
    TUpDown *updownBoxSize;
    TUpDown *updownMaxSection;
    TButton *btnDelChunk;
    TGroupBox *GroupBox1;
    TGroupBox *GroupBox2;
    TLabel *labelPrimary;
    TLabel *labelSecondary;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall sgChunksDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall btnAddChunkClick(TObject *Sender);
    void __fastcall cbLogClick(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
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
    void __fastcall btnDelChunkClick(TObject *Sender);
    void __fastcall editDestBoxSizeChange(TObject *Sender);
    void __fastcall btnAddAllChunksClick(TObject *Sender);
    void __fastcall timerCalculateTimer(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
private:
    LoadVialsJobThread *                        loadVialsJobThread;
    void __fastcall                             loadVialsJobThreadTerminated(TObject *Sender);
    SavePlanThread *                            savePlanThread;
    void __fastcall                             savePlanThreadTerminated(TObject *Sender);
    LCDbCryoJob *                               job;
    vecpSampleRow                               primaries;
    vecpSampleRow                               secondaries;
    vecpSampleRow                               combined;
    vector< Chunk< SampleRow > *>               chunks;
    StringGridWrapper<SampleRow> *              sgwVials;
    StringGridWrapper<SampleRow> *              sgwDebug;
    StringGridWrapper< Chunk< SampleRow > > *   sgwChunks;
    void                                        addSorter();
    void                                        removeSorter();
    void                                        applySort();
    void                                        rejectList();
    bool                                        addChunk(unsigned int startrow);
    void                                        autoChunk();
    Chunk< SampleRow > *                        currentChunk();
    void                                        showChunks();
    void                                        loadRows();
//    void                                        combineAliquots(const vecpSampleRow & primaries, const vecpSampleRow & secondaries, vecpSampleRow & combined);
    void                                        showChunk(Chunk< SampleRow > * chunk=NULL);
    const char *                                loadingMessage;
    void                                        calcSizes();
    int                                         section_size;
    int                                         box_size;
public:
    __fastcall                                  TfrmRetrAsstPlanSamples(TComponent* Owner);
    void                                        debugLog(String s);
    void                                        setJob(LCDbCryoJob * ajob) { job = ajob; };
};

extern PACKAGE TfrmRetrAsstPlanSamples *frmRetrAsstPlanSamples;
#endif
