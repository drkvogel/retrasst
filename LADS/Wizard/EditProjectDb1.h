//---------------------------------------------------------------------------
#ifndef EditProjectDb1H
#define EditProjectDb1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TEditProjectDb : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel2;
    TButton *EditDescriptors;
    TButton *EditProfiles;
    TButton *EditProfileMap;
    TPanel *Panel3;
    TButton *btnClose;
    TButton *btnBoxTypes;
    void __fastcall EditDescriptorsClick(TObject *Sender);
    void __fastcall EditProfilesClick(TObject *Sender);
    void __fastcall EditProfileMapClick(TObject *Sender);
    void __fastcall btnCloseClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall btnBoxTypesClick(TObject *Sender);

private:	// User declarations

public:		// User declarations
	__fastcall TEditProjectDb(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEditProjectDb *EditProjectDb;
//---------------------------------------------------------------------------
#endif
