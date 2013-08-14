#ifndef RetrievalAssistantAutoChunkH
#define RetrievalAssistantAutoChunkH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
class TfrmAutoChunk : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *GroupBox1;
    TEdit *editMaxSize;
    TButton *btnOK;
    TLabel *Label1;
    TComboBox *comboSectionSize;
    TLabel *Label2;
    TTimer *timerCalculate;
    TButton *btnAddChunk;
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall editMaxSizeChange(TObject *Sender);
    void __fastcall timerCalculateTimer(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnAddChunkClick(TObject *Sender);
private:
    void calcSizes();
public:
    __fastcall TfrmAutoChunk(TComponent* Owner);
};
extern PACKAGE TfrmAutoChunk *frmAutoChunk;
#endif
