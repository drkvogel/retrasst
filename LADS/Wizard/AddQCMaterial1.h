//---------------------------------------------------------------------------

#ifndef AddQCMaterial1H
#define AddQCMaterial1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class QCMaterialClass;
class TAddQCMaterial : public TForm
{
__published:	// IDE-managed Components
    TLabel *lblCode;
    TEdit *edtMaterial;
    TLabel *lblDescrip;
    TEdit *edtDescrip;
    TPanel *Panel1;
    TButton *btnCancel;
    TButton *btnSave;
	TQuery *QueryCentral;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);

private:	// User declarations

	QCMaterialClass* theMaterial;
	AnsiString Function;

public:		// User declarations

    __fastcall TAddQCMaterial(TComponent* Owner);
    void SetMaterialClass( QCMaterialClass* mat ) { theMaterial = mat; }
	void SetFunction( AnsiString funct ) { Function = funct; }
};
//---------------------------------------------------------------------------
extern PACKAGE TAddQCMaterial *AddQCMaterial;
//---------------------------------------------------------------------------
#endif
