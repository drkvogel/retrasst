//---------------------------------------------------------------------------
#ifndef AddQCTest1H
#define AddQCTest1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class QCLotClass;
class QCTestClass;

class TAddQCTest : public TForm
{
__published:	// IDE-managed Components
    TPanel *pnlMaterial;
    TLabel *Label1;
    TLabel *lblMaterial;
    TPanel *pnlLevel;
    TLabel *lblLevel;
    TLabel *Label6;
    TPanel *Panel1;
    TButton *btnCancel;
    TButton *btnSave;
    TPanel *pnlLot;
    TLabel *lblLot;
    TLabel *Label3;
    TLabel *Label2;
    TLabel *Label4;
    TEdit *edtMean;
    TEdit *edtStdDev;
    TLabel *Label5;
    TLabel *Label7;
    TLabel *lblTest;
	TQuery *QueryCentral;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
private:	// User declarations

	 QCLotClass* theQCLot;
    QCTestClass* theQCTest;
    AnsiString Test;
    AnsiString Function;

public:		// User declarations
    __fastcall TAddQCTest(TComponent* Owner);
    void SetFunction( AnsiString funct ) { Function = funct; }
    void SetQCLotClass( QCLotClass* lotClass ) { theQCLot = lotClass; }
    void SetQCTestClass( QCTestClass* testClass ) { theQCTest = testClass; }
};
//---------------------------------------------------------------------------
extern PACKAGE TAddQCTest *AddQCTest;
//---------------------------------------------------------------------------
#endif
