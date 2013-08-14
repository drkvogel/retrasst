#ifndef RetrievalAssistantH
#define RetrievalAssistantH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.CategoryButtons.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <sstream>
#include "LCDbJob.h"

const bool RETRASSTDEBUG =
#ifdef _DEBUG
    true;
#else
    false;
#endif

#define DEFAULT_NUMROWS 25
#define SGCHUNKS_COL_SECTION    0
#define SGCHUNKS_COL_START      1
#define SGCHUNKS_COL_END        2
#define SGCHUNKS_COL_SIZE       3

#define RETRIEVAL_ASSISTANT_HIGHLIGHT_COLOUR    clActiveCaption
#define RETRIEVAL_ASSISTANT_NEW_JOB_COLOUR      clMoneyGreen
#define RETRIEVAL_ASSISTANT_IN_PROGRESS_COLOUR  clLime
#define RETRIEVAL_ASSISTANT_DONE_COLOUR         clSkyBlue
#define RETRIEVAL_ASSISTANT_ERROR_COLOUR        clRed
#define RETRIEVAL_ASSISTANT_DELETED_COLOUR      clGray

// utilities

void clearGridSelection(TStringGrid * sg) { // put this in storeutil?
    TGridRect myRect;
    myRect.Left = 0; myRect.Top = 0; myRect.Right = 0; myRect.Bottom = 0;
    sg->Selection = myRect;
}

void clearSG(TStringGrid * sg) { // put this in storeutil?
    clearGridSelection(sg);
    sg->FixedRows = 0;
    sg->RowCount = 0;
    sg->RowCount = 2;
    sg->FixedRows = 1;
    for (int i = 0; i < sg->ColCount; i++) {
        sg->Cells[i][1] = "";
        sg->Objects[i][1] = NULL;
    }
    sg->Cells[0][1] = "No results.";
}

void setupStringGrid(TStringGrid * sg, const int cols, const char * colnames[], const int colwidths[]) {
    sg->ColCount = cols;
    for (int i=0; i<cols; i++) {
        sg->Cells[i][0]    = colnames[i];
        sg->ColWidths[i]   = colwidths[i];
    }
}

std::string printColWidths(TStringGrid * sg) {
    std::ostringstream oss; //oss << __FUNC__;
    oss << sg->Name.c_str() << ": {";
    for (int i=0; i<sg->ColCount; i++) {
        oss << sg->ColWidths[i] << ", ";
    }
    oss << "}";
    return oss.str();
}

// end utilities

// chunk stringgrid setup
enum { SGCHUNKS_SECTION, SGCHUNKS_START,  SGCHUNKS_END, SGCHUNKS_SIZE, SGCHUNKS_NUMCOLS };// sgChunks_cols;
static const char * sgChunksColName[SGCHUNKS_NUMCOLS]   = { "Section", "Start", "End", "Size" };
static const int    sgChunksColWidth[SGCHUNKS_NUMCOLS]  = { 100, 100, 30, 100 };

class Chunk { // not recorded in database
public:
    Chunk() : section(0), start(0), end(0) { }
    int         section;
    //std::string name;
    int         start;
    int         end;
};
typedef std::vector< Chunk * >  vecpChunk;

// jobs grid setup
enum { SGJOBS_DESCRIP, SGJOBS_JOBTYPE, SGJOBS_STATUS, SGJOBS_PRIMARY, SGJOBS_PROJECT, SGJOBS_REASON, SGJOBS_TIMESTAMP, SGJOBS_NUMCOLS };
static const char * sgJobsColName[SGJOBS_NUMCOLS]   = { "Description", "Job type", "Status", "Primary Aliquot", "Project", "Reason", "Timestamp" };
static const int    sgJobsColWidth[SGJOBS_NUMCOLS]  = { 200, 120, 100, 200, 100, 200, 100 };

static const char * jobStatusString(short status) {
    static const char * jobStatusStrings[] = { "New job", "In progress", "Done", "Deleted" };
    return status < LCDbCryoJob::Status::DELETED ? jobStatusStrings[status] : "Invalid";
};

static const char * jobTypeString(short status) {
    static const char * jobTypeStrings[] = { "Unknown", "Box move", "Box retrieval", "Box discard", "Sample retrieval", "Sample discard", "NUM_TYPES" };
    return status < LCDbCryoJob::JobKind::NUM_TYPES ? jobTypeStrings[status] : "Invalid";
};

typedef std::vector<LCDbCryoJob *> tdvecpJob;

class TfrmRetrievalAssistant : public TForm {
__published:
    TGroupBox *GroupBox1;
    TStringGrid *sgJobs;
    TPanel *Panel2;
    TGroupBox *groupStatusCheck;
    TCheckBox *cbNewJob;
    TCheckBox *cbInProgress;
    TCheckBox *cbDone;
    TCheckBox *cbDeleted;
    TGroupBox *groupRetrievalTypeCheck;
    TCheckBox *cbBoxRetrieval;
    TCheckBox *cbSampleRetrieval;
    TCheckBox *cbBoxMove;
    TCheckBox *cbBoxDiscard;
    TCheckBox *cbSampleDiscard;
    TButton *btnExit;
    TCheckBox *cbLog;
    TMemo *memoDebug;
    void __fastcall sgJobsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    void __fastcall cbNewJobClick(TObject *Sender);
    void __fastcall cbInProgressClick(TObject *Sender);
    void __fastcall cbDoneClick(TObject *Sender);
    void __fastcall cbDeletedClick(TObject *Sender);
    void __fastcall cbBoxRetrievalClick(TObject *Sender);
    void __fastcall cbSampleRetrievalClick(TObject *Sender);
    void __fastcall sgJobsDblClick(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall cbBoxMoveClick(TObject *Sender);
    void __fastcall cbBoxDiscardClick(TObject *Sender);
    void __fastcall cbSampleDiscardClick(TObject *Sender);
    void __fastcall cbLogClick(TObject *Sender);
private:
    void debugLog(String s);
    tdvecpJob vecJobs;
    void loadJobs();
    void showJobs();
    std::string getExerciseDescription(int exercise_cid);
    std::string getProjectDescription(int project_cid);
    std::string getAliquotDescription(int primary_aliquot);
    std::string getAuditInfo(int process_cid);
public:
    __fastcall TfrmRetrievalAssistant(TComponent* Owner);
    void init();
};
extern PACKAGE TfrmRetrievalAssistant *frmRetrievalAssistant;
#endif
