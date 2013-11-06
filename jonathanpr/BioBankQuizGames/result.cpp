//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "result.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
Tfm_result *fm_result;
//---------------------------------------------------------------------------
__fastcall Tfm_result::Tfm_result(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall Tfm_result::FormShow(TObject *Sender)
{
	fm_result->timer_result->Enabled = true;	
}
//---------------------------------------------------------------------------
void __fastcall Tfm_result::timer_resultTimer(TObject *Sender)
{
	fm_result->timer_result->Enabled = false;
	Close();
}
//---------------------------------------------------------------------------
void Tfm_result::display( bool matched )
{
	if ( matched )
		{
		panel_outcome->Font->Color = clYellow;
		panel_outcome->Caption = "Pair!";
		}
	else
		{
		panel_outcome->Font->Color = clRed;
		panel_outcome->Caption = "miss";
		}
	ShowModal();
}
//---------------------------------------------------------------------------

