#ifndef RetrievalAssistantProcessH
#define RetrievalAssistantProcessH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include "LCDbJob.h"
#include "RetrievalAssistant.h"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

enum {  SGRETRIEVAL_BARCODE, SGRETRIEVAL_DESTBOX, SGRETRIEVAL_DESTPOS, SGRETRIEVAL_CURRBOX, SGRETRIEVAL_CURRPOS,
        SGRETRIEVAL_SITE, SGRETRIEVAL_POSITION, SGRETRIEVAL_SHELF, SGRETRIEVAL_VESSEL, SGRETRIEVAL_STRUCTURE, SGRETRIEVAL_SLOT, // location in "Russian Doll order"
        SGRETRIEVAL_NUMCOLS};
static const char * sgRetrievalColName[SGRETRIEVAL_NUMCOLS] = {
    "Barcode", "Dest box", "Pos", "Curr box", "Pos", "Site", "Position", "Shelf", "Vessel", "Structure", "Slot"
};
static int sgRetrievalColWidth[SGRETRIEVAL_NUMCOLS] = {102, 147, 43, 275, 37, 64, 50, 43, 100, 121, 40 };


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
    TGroupBox *Chunks;
    TStringGrid *sgChunks;
    TCheckBox *cbLog;
    TMemo *memoDebug;
    TPanel *panelLoading;
    TProgressBar *progressBottom;
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall Exit1Click(TObject *Sender);
    void __fastcall cbLogClick(TObject *Sender);
private:
    LCDbCryoJob * job;
    void loadRows();
    void showRows();
    void process();
    int maxRows;
    const char *        loadingMessage;
public:
    void setJob(LCDbCryoJob * ajob) { job = ajob; }
    __fastcall TfrmProcess(TComponent* Owner);
};

extern PACKAGE TfrmProcess *frmProcess;
#endif
