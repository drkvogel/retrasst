//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "EditProjectDb1.h"
#include "Descriptor1.h"
#include "Profile1.h"
#include "ProfileMap1.h"
#include "BoxContent1.h"
#include "LCDbProject.h"
#include "Login1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditProjectDb *EditProjectDb;
//---------------------------------------------------------------------------
__fastcall TEditProjectDb::TEditProjectDb(TComponent* Owner)
	: TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TEditProjectDb::FormActivate(TObject *Sender)
{
	Caption = Login -> setProjectCaption( "Edit Project Database" );
}

//---------------------------------------------------------------------------

void __fastcall TEditProjectDb::EditDescriptorsClick(TObject *Sender)
{
	Descriptor->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TEditProjectDb::EditProfilesClick(TObject *Sender)
{
	Profile->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TEditProjectDb::EditProfileMapClick(TObject *Sender)
{
	ProfileMap->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TEditProjectDb::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TEditProjectDb::btnBoxTypesClick(TObject *Sender)
{
	BoxContent->ShowModal();
}

//---------------------------------------------------------------------------

