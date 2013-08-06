#ifndef RetrievalAssistantProcessH
#define RetrievalAssistantProcessH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include "LCDbJob.h"

class TfrmProcess : public TForm {
__published:
    TGroupBox *GroupBox1;
    TGroupBox *GroupBox2;
    TButton *btnAccept;
    TButton *btnSkip;
    TEdit *editBarcode;
    TLabel *Label1;
    TStringGrid *StringGrid1;
    TGroupBox *GroupBox3;
    TLabel *Label3;
    TLabel *Label2;
    TMainMenu *MainMenu1;
    TMenuItem *Exit1;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall Exit1Click(TObject *Sender);
private:
    LCDbCryoJob * job;
public:
    void setJob(LCDbCryoJob * ajob) { job = ajob; }
    __fastcall TfrmProcess(TComponent* Owner);
};

extern PACKAGE TfrmProcess *frmProcess;
#endif
