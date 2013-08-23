#ifndef RetrievalAssistantProcessH
#define RetrievalAssistantProcessH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include "LCDbJob.h"

// sgRetrieval

enum {
    SGRETRIEVAL_BARCODE,
    SGRETRIEVAL_LOCATION,
//    SGRETRIEVAL_,
//    SGRETRIEVAL_,
//    SGRETRIEVAL_,
//    SGRETRIEVAL_,
    SGRETRIEVAL_NUMCOLS
} sgRetrieval_cols;

static const char * sgRetrievalColName[SGRETRIEVAL_NUMCOLS] = {
    "Barcode", "Location",
//    "Pos",
//    "Curr box",
//    "Pos",
//    "Structure",
//    "Location",
};

static int sgRetrievalColWidth[SGRETRIEVAL_NUMCOLS] = { 100, 100
    //, 30, 100, 30, 100, 100
};

class TfrmProcess : public TForm {
__published:
    TGroupBox *List;
    TGroupBox *GroupBox2;
    TButton *btnAccept;
    TButton *btnSkip;
    TEdit *editBarcode;
    TLabel *Label1;
    TStringGrid *sgRetrieval;
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
    void loadRows();
    void showRows();
    void process();
public:
    void setJob(LCDbCryoJob * ajob) { job = ajob; }
    __fastcall TfrmProcess(TComponent* Owner);
};

extern PACKAGE TfrmProcess *frmProcess;
#endif
