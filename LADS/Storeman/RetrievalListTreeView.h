#ifndef RetrievalListTreeViewH
#define RetrievalListTreeViewH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Menus.hpp>

#include "RetrievalListBase.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>

class TfrmRetrievalTreeView : public TForm {
__published:	// IDE-managed Components
    TTreeView *tree;
    TMainMenu *MainMenu1;
    TPanel *panelSide;
    TGroupBox *groupStatusCheck;
    TCheckBox *cbNewJob;
    TCheckBox *cbInProgress;
    TCheckBox *cbDone;
    TCheckBox *cbDeleted;
    TCheckBox *cbRejected;
    TGroupBox *groupRetrievalTypeCheck;
    TCheckBox *cbBoxRetrieval;
    TCheckBox *cbSampleRetrieval;
    TCheckBox *cbBoxMove;
    TCheckBox *cbBoxDiscard;
    TCheckBox *cbSampleDiscard;
    TCheckBox *cbLog;
    TBitBtn *btnClose;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall btnCloseClick(TObject *Sender);
private:	// User declarations
    void init();
    tdvecpJob           vecJobs;
    LCDbCryoJobs        jobs;

public:		// User declarations
    __fastcall TfrmRetrievalTreeView(TComponent* Owner);

};

//StringGridWrapper<LCDbCryoJob> *    sgwJobs;

extern PACKAGE TfrmRetrievalTreeView *frmRetrievalTreeView;

#endif
