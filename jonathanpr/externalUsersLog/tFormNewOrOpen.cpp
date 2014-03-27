//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "tFormNewOrOpen.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TNewOrOpenForm *NewOrOpenForm;
//---------------------------------------------------------------------------
__fastcall TNewOrOpenForm::TNewOrOpenForm(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
