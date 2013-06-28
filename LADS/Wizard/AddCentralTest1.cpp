//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AddCentralTest1.h"
#include "Descriptor1.h"
#include "CentralTest1.h"
#include "CentralTestMachine1.h"
#include "AddTestCalculation1.h"
#include "LDbIdClasses.h"
#include "WizardLogin1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TAddCentralTest *AddCentralTest;

//---------------------------------------------------------------------------

__fastcall TAddCentralTest::TAddCentralTest(TComponent* Owner)
	: TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TAddCentralTest::FormActivate(TObject *Sender)
{
	const LCDbTest * theTest = LCDbTests::records().findByID( testID );
	if( theTest != NULL )
	{
		Caption = "Modify " + WizardLogin -> setCaption( "Test" );
		TestName->Text = theTest -> getName();
		Description->Text = theTest -> getDescription();

		std::pair< TDate, TDate > dates = theTest -> getValidDates();
		dtpFrom -> DateTime = dates.first;
		dtpTo -> DateTime = dates.second;
		cbActive -> Checked = theTest -> isActive();

		std::pair< double, double > limits = theTest -> getLimits();
		DefaultLowerLimit->Text = limits.first;
		DefaultUpperLimit->Text = limits.second;
		SampleType->Text = theTest -> getSampleType();
		btnAddMachines->Enabled = !theTest -> getCalcParts().isValid();
		btnCalculation->Enabled = !theTest -> getCodes().isValid();
	}
	else
	{	Caption = "Add " + WizardLogin -> setCaption( "Test" );
		TestName->Text = "";
		Description->Text = "";

		dtpFrom -> DateTime = TDateTime::CurrentDate();
		// Add 10 years for default end date (add number of days)
		dtpTo -> DateTime = Now() + 3652;
		cbActive -> Checked = true;

		DefaultLowerLimit->Text = "";
		DefaultUpperLimit->Text = "";
		SampleType->Text = "PL";
		btnAddMachines->Enabled = false;
		btnCalculation->Enabled = false;
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddCentralTest::SaveClick(TObject *Sender)
{
	std::pair< double, double > limits;
	int validform;
	try
	{	limits.first = DefaultLowerLimit -> Text.ToDouble();
		limits.second = DefaultUpperLimit -> Text.ToDouble();
		validform = (limits.first < limits.second);
	}
	catch( ... )
	{
		validform = false;
	}

	if( !validform )
		Application->MessageBox("Invalid default limits", NULL, MB_OK);

	std::pair< TDate, TDate > dates( dtpFrom -> Date, dtpTo -> Date );
	if( cbActive -> Checked	&& (dates.second < dates.first
		|| dates.first > Now() || dates.second < Now()) )
	{
		Application->MessageBox( "Invalid date range for active test", NULL, MB_OK);
		validform = false;
	}

	if( TestName -> Text.IsEmpty() )
	{
		Application->MessageBox("Test name must not be blank", NULL, MB_OK);
		validform = false;
	}

	if( Description -> Text.IsEmpty() )
	{
		Application->MessageBox("Description must not be blank", NULL, MB_OK);
		validform = false;
	}

	if(  SampleType->Text.IsEmpty() || SampleType->Text.Length() > 3 )
	{
		Application->MessageBox("Sample type must be 1-3 characters", NULL, MB_OK);
		validform = false;
	}

	if ( validform )
	{
		LCDbTest theTest;
		if( testID != 0 )
			theTest = LCDbTests::records().get( testID );

		theTest.setName(TestName->Text);
		theTest.setDescription(Description->Text);
		theTest.setValidDates( dates );
		theTest.setActive( cbActive -> Checked );
		theTest.setSampleType( SampleType->Text );
		theTest.setLimits( limits );
		theTest.saveRecord( QueryCentral );
		testID = theTest.getID();

		btnAddMachines->Enabled = !theTest.getCalcParts().isValid();
		btnCalculation->Enabled = !theTest.getCodes().isValid();
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddCentralTest::CancelClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TAddCentralTest::btnAddMachinesClick(TObject *Sender)
{
	btnCalculation -> Enabled = false;
	btnAddMachines -> Enabled = false;
	if( LCDbTests::records().findByID( testID ) == NULL )
		Application->MessageBox( "Test must be saved before adding machines", NULL, MB_OK );
	else
	{	CentralTestMachine -> setTest( testID );
		CentralTestMachine -> ShowModal();
	}

	TestName->SetFocus();
}

//---------------------------------------------------------------------------

void __fastcall TAddCentralTest::btnCalculationClick(TObject *Sender)
{
	btnCalculation -> Enabled = false;
	btnAddMachines -> Enabled = false;
	if( LCDbTests::records().findByID( testID ) == NULL )
		Application->MessageBox( "Test must be saved before adding calculations", NULL, MB_OK );
	else
	{	AddTestCalculation -> setTest( testID );
		AddTestCalculation -> ShowModal();
	}

	TestName->SetFocus();
}

//---------------------------------------------------------------------------

void __fastcall TAddCentralTest::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

