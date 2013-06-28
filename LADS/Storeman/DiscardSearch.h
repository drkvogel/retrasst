#ifndef DiscardSearchH
#define DiscardSearchH
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>

#include "DiscardContext.h"

class TfrmSearch : public TForm
{
__published:	// IDE-managed Components
    TPanel *pnlNavigate;
    TButton *btnBack;
    TButton *btnNext;
    TButton *btnCancel;
    TPanel *pnlStep1;
    TRadioGroup *rgType;
    TPanel *pnlStep3b;
    TComboBox *cmbMenu;
    TPanel *pnlMenu;
    TOpenDialog *openFile;
    TLabel *labMenu;
    TPanel *pnlStep2;
    TPanel *pnlStep3a;
    TPanel *pnlText;
    TLabel *labText;
    TEdit *ediText;
    TPanel *pnlStep3c;
    TPanel *pnlFile;
    TButton *btnFile;
    TRadioGroup *rgSource;
    TEdit *ediFile;
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall btnBackClick(TObject *Sender);
    void __fastcall btnNextClick(TObject *Sender);
    void __fastcall rgTypeClick(TObject *Sender);
    void __fastcall btnBrowseClick(TObject *Sender);
    void __fastcall cmbMenuChange(TObject *Sender);
    void __fastcall ediTextKeyPress(TObject *Sender, char &Key);
    void __fastcall ediTextChange(TObject *Sender);
    void __fastcall rgSourceClick(TObject *Sender);
private:	// User declarations
    void gotoStep1( );
    void gotoStep2( );
    void gotoStep3( );
    void updateUI();
    void updateUIStep1();
    void updateUIStep2();
    void updateUIStep3();
    int m_stepno;
    Discard::Context * m_context;
    IntVec m_jobnos;
public:		// User declarations
    __fastcall TfrmSearch(TComponent* Owner);
    void init( Discard::Context * context );
};

extern PACKAGE TfrmSearch *frmSearch;

#endif
