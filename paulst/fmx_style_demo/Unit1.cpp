//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "Unit1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	enableGrabTextButton();
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	panel1->StyleLookup =
		0 == panel1->StyleLookup.CompareIC(L"alarmon") ?
		L"alarmoff" : L"alarmon";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button2Click(TObject *Sender)
{
	label1->StyleLookup =
		0 == label1->StyleLookup.CompareIC(L"labelstylenowarnings") ?
		L"labelstylewarnings" :	L"labelstylenowarnings";
}
//---------------------------------------------------------------------------

void __fastcall TForm1::Button3Click(TObject *Sender)
{
	Panel4->StyleLookup =
		0 == Panel4->StyleLookup.CompareIC(L"plainpanel") ?
		L"checkmemo" : L"plainpanel";
	enableGrabTextButton();
}
//---------------------------------------------------------------------------
void TForm1::enableGrabTextButton()
{
	grabTextButton->Enabled = 0 == Panel4->StyleLookup.CompareIC(L"checkmemo");
}
//---------------------------------------------------------------------------
void __fastcall TForm1::grabTextButtonClick(TObject *Sender)
{
	if ( 0 == Panel4->StyleLookup.CompareIC( L"checkmemo" ) )
	{
		TMemo* memo = (TMemo*) (Panel4->FindStyleResource("Memo1"));
		if ( memo )
		{
			ShowMessage(
				UnicodeString(L"The text in the panel is: ") + memo->Text );
		}
	}
}
//---------------------------------------------------------------------------

