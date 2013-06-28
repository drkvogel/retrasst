//---------------------------------------------------------------------------
#ifndef ChooseProfile1H
#define ChooseProfile1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TChooseProfile : public TForm
{
__published:	// IDE-managed Components

	TPanel *Panel1;
	TButton *Cancel;
	TButton *Save;
	TListBox *ChooseProfileBox;
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall SaveClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);

private:	// User declarations

	int profileID;

public:		// User declarations

	__fastcall TChooseProfile(TComponent* Owner);

	static const int INVALIDPROFILE;
	int getProfileID(){ return profileID; }
};

//---------------------------------------------------------------------------

extern PACKAGE TChooseProfile *ChooseProfile;

//---------------------------------------------------------------------------
#endif
