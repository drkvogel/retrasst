//---------------------------------------------------------------------------

#include <vcl.h>

#include "AddBoxSize1.h"
#include "LDbIdClasses.h"
#include "WizardLogin1.h"
#include "LDbBoxSize.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TAddBoxSize *AddBoxSize;

//---------------------------------------------------------------------------

__fastcall TAddBoxSize::TAddBoxSize(TComponent* Owner)
 : TForm(Owner), boxSizeID( 0 )
{}

//---------------------------------------------------------------------------

void __fastcall TAddBoxSize::FormActivate(TObject *Sender)
{
	const LCDbBoxSize * theBoxSize = LCDbBoxSizes::records().findByID( boxSizeID );
	if( theBoxSize != NULL )
	{
		Caption = "Modify " + WizardLogin -> setCaption( "Box Size" );
		edtDescription->Text = theBoxSize -> getName();
		edtCapacity->Text = theBoxSize -> getSize();
		edtEmptyPos->Text = theBoxSize -> getHole();
		std::pair< TDate, TDate > dates = theBoxSize -> getValidDates();
		dtpFrom -> DateTime = dates.first;
		dtpTo -> DateTime = dates.second;
		cbVacutainer -> Checked = (theBoxSize -> getTube() == LCDbBoxSize::VACUTAINER);
		cbActive -> Checked = theBoxSize -> isActive();
	}
	else
	{	Caption = "Add " + WizardLogin -> setCaption( "Box Size" );
		edtDescription->Text = "";
		edtCapacity->Text = "100";
		edtEmptyPos->Text = "91";
		dtpFrom -> DateTime = TDateTime::CurrentDate();
		dtpTo -> DateTime = Now() + 3652;
		cbVacutainer -> Checked = false;
		cbActive -> Checked = true;
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddBoxSize::SaveClick(TObject *Sender)
{
	int validform = true;
	std::pair< TDate, TDate > dates( dtpFrom -> Date, dtpTo -> Date );
	if( cbActive -> Checked	&& (dates.second < dates.first
	  || dates.first > Now() || dates.second < Now()) )
	{
		Application->MessageBox( "Invalid date range", NULL, MB_OK );
		validform = false;
	}

	if( edtDescription->Text.IsEmpty() )
	{
		Application->MessageBox( "Invalid Description", NULL, MB_OK );
		validform = false;
	}

	short size = edtCapacity->Text.ToIntDef( 0 );
	if( size < 2 )
	{
		Application->MessageBox( "Invalid Capacity", NULL, MB_OK );
		validform = false;
	}

	short hole = edtEmptyPos->Text.ToIntDef( 0 );
	if( hole > size )
	{
		Application->MessageBox( "Invalid empty position", NULL, MB_OK );
		validform = false;
	}

	if( validform )
	{
		LCDbBoxSize theBoxSize( boxSizeID );
		theBoxSize.setName( edtDescription->Text );
		theBoxSize.setValidDates( dates );
		theBoxSize.setActive( cbActive -> Checked );
		theBoxSize.setSize( size );
		theBoxSize.setHole( hole );
		if( cbVacutainer -> Checked )
			theBoxSize.setTube( LCDbBoxSize::VACUTAINER );
		else
			theBoxSize.setTube( LCDbBoxSize::CRYOVIAL );
		theBoxSize.saveRecord( QueryCentral );
		this->Close();
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddBoxSize::CancelClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

