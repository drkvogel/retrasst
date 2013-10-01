#ifndef RetrievalAssistantAutoChunkH
#define RetrievalAssistantAutoChunkH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ComCtrls.hpp>
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
    TEdit *editDestBoxSize;
    TLabel *Label3;
    TButton *btnAddAllChunks;
    TUpDown *updownBoxSize;
    TUpDown *updownMaxSection;
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall btnOKClick(TObject *Sender);
    void __fastcall editMaxSizeChange(TObject *Sender);
    void __fastcall timerCalculateTimer(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnAddChunkClick(TObject *Sender);
    void __fastcall btnAddAllChunksClick(TObject *Sender);
private:
    void calcSizes();
    int section_size;
public:
    __fastcall TfrmAutoChunk(TComponent* Owner);
};
extern PACKAGE TfrmAutoChunk *frmAutoChunk;
#endif
