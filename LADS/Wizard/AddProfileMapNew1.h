//---------------------------------------------------------------------------

#ifndef AddProfileMapNew1H
#define AddProfileMapNew1H
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "LPDbProfileMap.h"
#include "LPDbDescriptor.h"
#include <Db.hpp>
#include <DBTables.hpp>

//---------------------------------------------------------------------------

class TAddProfileMapNew : public TForm
{
__published:	// IDE-managed Components

	TPanel *Panel1;
	TButton *btnCancel;
	TButton *btnSave;
	TListBox *lbxValues;
	TLabel *lblDescrip0;
	TListBox *lbxValuesChosen;
	TEdit *edtDescriptor;
	TLabel *Label1;
	TLabel *Label2;
	TButton *btnClear;
	TEdit *edtProfile;
	TLabel *Label3;
	TQuery *QueryProject;

	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall btnSaveClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall lbxValuesClick(TObject *Sender);

private:	// User declarations

	LPDbProfileMap theProfileMap;
	int descripID;
	int numValuesChosen;

	bool nextDescriptor( LPDbDescriptors::Range dr );

public:		// User declarations

	__fastcall TAddProfileMapNew(TComponent* Owner);
};

//---------------------------------------------------------------------------

extern PACKAGE TAddProfileMapNew *AddProfileMapNew;

//---------------------------------------------------------------------------
#endif
