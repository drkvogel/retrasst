#ifndef RetrievalAssistantH
#define RetrievalAssistantH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>

class TfrmRetrievalAssistant : public TForm
{
__published:	// IDE-managed Components
    TComboBox *comboJob;
    TStringGrid *sgBoxes;
    TListBox *lbJobs;
private:	// User declarations
    void loadJobs();
    void loadBoxes();
public:		// User declarations
    __fastcall TfrmRetrievalAssistant(TComponent* Owner);
    void init();
};
extern PACKAGE TfrmRetrievalAssistant *frmRetrievalAssistant;
#endif
