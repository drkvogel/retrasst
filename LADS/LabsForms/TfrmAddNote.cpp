//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "TfrmAddNote.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmEditNote *frmEditNote;

//---------------------------------------------------------------------------

__fastcall TfrmEditNote::TfrmEditNote(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void TfrmEditNote::initialise( const char * options[] )
{
	for( lstExpected -> Clear(); *options != NULL; options ++ )
		lstExpected -> Items -> Add( *options );
}

//---------------------------------------------------------------------------

void __fastcall TfrmEditNote::lstExpectedClick(TObject *)
{
	String note = txtNote -> Text;
	for( int i = 0; i < lstExpected -> Items -> Count; i++ )
	{
		if( lstExpected -> Selected[ i ] )
		{
			String selected = lstExpected -> Items -> Strings[ i ];
			note = note.IsEmpty() ? selected : note + "; " + selected;
		}
	}
	txtNote -> Text = note;
}

//---------------------------------------------------------------------------          

void __fastcall TfrmEditNote::BitBtn1Click(TObject *)
{
	txtNote -> Clear();
}

//---------------------------------------------------------------------------

void __fastcall TfrmEditNote::BitBtn2Click(TObject *)
{
	txtNote -> Clear();
	Close();
}

//---------------------------------------------------------------------------

