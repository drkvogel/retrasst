//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddTestCalculation1.h"
#include "CentralTest1.h"
#include "WizardLogin1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TAddTestCalculation *AddTestCalculation;

//---------------------------------------------------------------------------

__fastcall TAddTestCalculation::TAddTestCalculation(TComponent* Owner)
	: TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TAddTestCalculation::FormActivate(TObject *Sender)
{
	cbxTest1->Items->Text = "none";
	cbxTest2->Items->Text = "none";
	for( LCDbTests::Range tr = LCDbTests::records(); tr.isValid(); ++ tr )
	{
		if( tr -> isActive() && !tr -> getCalcParts().isValid() )
		{
			cbxTest1->Items->Add( tr -> getName() );
			cbxTest2->Items->Add( tr -> getName() );
		}
	}

	cbxCalc -> Items -> Text = "-";
	cbxCalc -> Items -> Add( "/" );
	cbxCalc -> Items -> Add( "+" );
	cbxCalc -> Items -> Add( "x" );

	LCDbTest::CalcDefs defs = LCDbTests::records().get( testID ).getCalcParts();
	if( defs.isValid() )
	{
		Caption = "Change " + WizardLogin -> setCaption( "Test Calculation" );
		edtTest1->Text = FormatFloat( "###0.000", defs -> getMultiple() );
		if( defs -> getSourceTestID() != 0 )
			cbxTest1 -> Text = LCDbTests::records().get( defs -> getSourceTestID() ).getName();
		else
			cbxTest1 -> Text = "";

		cbxTest2 -> Text = "";
		if( (++ defs).isValid() )
		{
			cbxCalc -> ItemIndex = defs -> getCalculation();
			edtTest2 -> Text = FormatFloat( "###0.000", defs -> getMultiple() );
			if( defs -> getSourceTestID() != 0 )
				cbxTest2->Text = LCDbTests::records().get( defs -> getSourceTestID() ).getName();
		}
		else
		{	cbxCalc -> ItemIndex = -1;
			edtTest2->Text = "";
		}
	}
	else Caption = "Add new " + WizardLogin -> setCaption( "Test Calculation" );
}

//---------------------------------------------------------------------------

void __fastcall TAddTestCalculation::btnSaveClick(TObject *Sender)
{
	bool first = (!edtTest1 -> Text.IsEmpty() || !cbxTest1 -> Text.IsEmpty());
	bool second = (!edtTest2 -> Text.IsEmpty() || !cbxTest2 -> Text.IsEmpty());
	if( cbxCalc -> Text.IsEmpty() ? (first && second) : !(first && second) )
	{
		Application -> MessageBox( "Please specify required operation", NULL, MB_OK );
		return;
	}

	double mul1 = 1, mul2 = 1;
	try
	{	if( !edtTest1 -> Text.IsEmpty() )
			mul1 = edtTest1 -> Text.ToDouble();

		if( !edtTest2 -> Text.IsEmpty() )
			mul2 = edtTest2 -> Text.ToDouble();
	}
	catch( ... )
	{
		Application -> MessageBox( "Multiples should be valid numbers", NULL, MB_OK );
		return;
	}

	std::vector< LCDbTest::CalcDef > parts;
	if( first )
	{
		const LCDbTest * test = LCDbTests::records().findByName( cbxTest1 -> Text );
		int tid = (test == NULL ? 0 : test -> getID());
		parts.push_back( LCDbTest::CalcDef( LCDbTest::CalcDef::ADDITION, mul1, tid ) );
	}

	if( second )
	{
		const LCDbTest * test = LCDbTests::records().findByName( cbxTest2 -> Text );
		int tid = (test == NULL ? 0 : test -> getID());
		parts.push_back( LCDbTest::CalcDef( cbxCalc->ItemIndex, mul2, tid ) );
	}

	LCDbTest update = LCDbTests::records().get( testID );
	update.saveCalcParts( QueryCentral, parts );
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TAddTestCalculation::btnClearClick(TObject *Sender)
{
	edtTest1 -> Text = "";
	cbxTest1 -> Text = "";
	cbxCalc -> Text = "";
	edtTest2 -> Text = "";
	cbxTest2 -> Text = "";
}

//---------------------------------------------------------------------------

void __fastcall TAddTestCalculation::btnCancelClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

