#ifndef RetrievalAssistantH
#define RetrievalAssistantH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
//#include <ostream>
#include <sstream>

class TfrmRetrievalAssistant : public TForm
{
__published:	// IDE-managed Components
    TComboBox *comboJob;
    TListBox *lbJobs;
    TGroupBox *GroupBox1;
    TStringGrid *sgJobs;
private:	// User declarations
    void loadJobs();
    void loadBoxes();
    std::string TfrmRetrievalAssistant::getExerciseDescription(int exercise_cid);
    std::string TfrmRetrievalAssistant::getProjectDescription(int project_cid);
    std::string getAliquotDescription(int primary_aliquot);
    std::string getAuditInfo(int process_cid);

public:		// User declarations
    __fastcall TfrmRetrievalAssistant(TComponent* Owner);
    void init();
};
extern PACKAGE TfrmRetrievalAssistant *frmRetrievalAssistant;
#endif
