#ifndef RetrievalAssistantH
#define RetrievalAssistantH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.CategoryButtons.hpp>
#include <Vcl.CheckLst.hpp>
#include <sstream>
#include "LCDbJob.h"

#define RETRIEVAL_ASSISTANT_HIGHLIGHT_COLOUR clActiveCaption
#define RETRIEVAL_ASSISTANT_DONE_COLOUR   clMoneyGreen
//#define RETRIEVAL_ASSISTANT_BOX_MATCH_COLOUR clMoneyGreen
#define RETRIEVAL_ASSISTANT_ERROR_COLOUR     clRed
#define RETRIEVAL_ASSISTANT_NEW_JOB_COLOUR clSilver
#define RETRIEVAL_ASSISTANT_IN_PROGRESS_COLOUR clGray
#define RETRIEVAL_ASSISTANT_REFFERED_COLOUR  clSkyBlue

static const char * jobStatusString(short status) {
    static const char * jobStatusStrings[] = { "New job", "In progress", "Done", "Deleted" };
    return status < LCDbCryoJob::Status::DELETED ? jobStatusStrings[status] : "Invalid";
};

static const char * jobTypeString(short status) {
    static const char * jobTypeStrings[] = { "Unknown", "Box move", "Box retrieval", "Box discard", "Sample retrieval", "Sample discard", "NUM_TYPES" };
    return status < LCDbCryoJob::JobKind::NUM_TYPES ? jobTypeStrings[status] : "Invalid";
};

class TfrmRetrievalAssistant : public TForm
{
__published:	// IDE-managed Components
    TComboBox *comboJob;
    TListBox *lbJobs;
    TGroupBox *GroupBox1;
    TStringGrid *sgJobs;
    TCheckListBox *clbTest;
    TCategoryButtons *CategoryButtons1;
    void __fastcall sgJobsDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
private:	// User declarations
    void loadJobs();
    void loadBoxes();
    std::string getExerciseDescription(int exercise_cid);
    std::string getProjectDescription(int project_cid);
    std::string getAliquotDescription(int primary_aliquot);
    std::string getAuditInfo(int process_cid);
public:		// User declarations
    __fastcall TfrmRetrievalAssistant(TComponent* Owner);
    void init();
};
extern PACKAGE TfrmRetrievalAssistant *frmRetrievalAssistant;
#endif
