//---------------------------------------------------------------------------
#ifndef AddCentralTestMachine1H
#define AddCentralTestMachine1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TestClass;
class TAddCentralTestMachine : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TLabel *lblMachineName;
    TLabel *Label4;
    TEdit *edtMachineTestName;
    TLabel *Label5;
    TLabel *Label6;
    TLabel *Label7;
    TLabel *Label8;
    TLabel *lblTest;
    TPanel *Panel1;
    TButton *Save;
    TButton *Cancel;
    TEdit *edtProtocol;
    TEdit *edtTestOrder;
    TLabel *Label2;
    TLabel *Label9;
    void __fastcall SaveClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall CancelClick(TObject *Sender);
private:	// User declarations
    TQuery* QueryCentral;
    AnsiString Function;
    TestClass* theTest;
    CentralTestMachineClass* theCentralTestMachine;
public:		// User declarations
    __fastcall TAddCentralTestMachine(TComponent* Owner);
    void SetQuery( TQuery* q ) { QueryCentral = q; }
    void SetFunction( AnsiString funct ) { Function = funct; }
    void SetTestClass( TestClass* tc ) { theTest = tc; }
    void SetCentralTestMachineClass( CentralTestMachineClass* tmc )
        { theCentralTestMachine = tmc; }
};
//---------------------------------------------------------------------------
extern PACKAGE TAddCentralTestMachine *AddCentralTestMachine;
//---------------------------------------------------------------------------
#endif
