//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "TMainForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TMainForm *MainForm;
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent* Owner)
	: TForm(Owner),
	m_alreadyShown( false )
{
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::onShow(TObject *Sender)
{
	if ( ! m_alreadyShown )
	{
		m_alreadyShown = true;

		int Ypos = 0;
		const int HEIGHT = 30;
		const int WIDTH  = 184;

		for ( int row = 0; row < 1000; ++row )
		{
			int Xpos = 0;

			for ( int col = 0; col < 20; ++col )
			{
				TPanel* p = addSubComponent<TPanel>(scrollBox);
				p->Height = HEIGHT;
				p->Width = WIDTH;
				p->StyleLookup = L"CustomPanelStyle";
				p->Position->X = Xpos;
				p->Position->Y = Ypos;
				Xpos += WIDTH;
			}

			Ypos += HEIGHT;
		}
	}
}
//---------------------------------------------------------------------------
