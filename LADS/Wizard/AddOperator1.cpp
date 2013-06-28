//---------------------------------------------------------------------------
#include <vcl.h>

#include "AddOperator1.h"
#include "WizardLogin1.h"
#include "Descriptor1.h"
#include "Operator1.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TAddOperator *AddOperator;

//---------------------------------------------------------------------------

__fastcall TAddOperator::TAddOperator(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TAddOperator::FormActivate(TObject *Sender)
{
	const LCDbOperator * theOperator = LCDbOperators::records().findByID( userID );
	if( theOperator != NULL )
	{
		OperatorName->Text = theOperator -> getName();
		OperatorName->Enabled = false;
		Description->Text = theOperator -> getDescription();
		cbxStatus -> Checked = theOperator -> isActive();

		std::pair< TDate, TDate > dates = theOperator -> getValidDates();
		dtpFrom->DateTime = dates.first;
		dtpTo->DateTime = dates.second;

		cbVal1->Checked = theOperator -> hasAbility( LCDbOperator::VALIDATION );
		cbVal2->Checked = theOperator -> hasAbility( LCDbOperator::L2_VALIDATION );
		cbPlotqc->Checked = theOperator -> hasAbility( LCDbOperator::PLOT_QC );
		cbWizard->Checked = theOperator -> hasAbility( LCDbOperator::WIZARD );
		Caption = "Change " + WizardLogin -> setCaption( "Operator" );
	}
	else
	{	Caption = "Add " + WizardLogin -> setCaption( "Operator" );
		OperatorName->Text = "";
		OperatorName->Enabled = true;
		Description->Text = "";
		cbxStatus -> Checked = true;
		dtpFrom->DateTime = TDateTime::CurrentDate();
		dtpTo->DateTime = Now() + 30;
		cbVal1->Checked = false;
		cbVal2->Checked = false;
		cbPlotqc->Checked = false;
		cbWizard->Checked = false;
	}
	Password->Text = "";
}

//---------------------------------------------------------------------------

void __fastcall TAddOperator::CancelClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TAddOperator::SaveClick(TObject *Sender)
{
	int validform = true;
	if( OperatorName->Text.IsEmpty() )
	{
		Application->MessageBox( "Invalid operator name", NULL, MB_OK );
		validform = false;
	}
	else
	{	const LCDbOperator * other = LCDbOperators::records().findByName( OperatorName->Text );
		if( other != NULL && other -> getID() != userID )
		{
			Application->MessageBox( "Name already in use", NULL, MB_OK );
			validform = false;
		}
	}

	if( Description->Text.IsEmpty() )
	{
		Application->MessageBox("Invalid Description", NULL, MB_OK );
		validform = false;
	}

	// current users should have a start date of today or earlier
	// and a password expiry date of less than six months from today

	std::pair< TDate, TDate > dates( dtpFrom -> Date, dtpTo -> Date );
	if( cbxStatus -> Checked
	 && (dates.second < dates.first || dates.first > Now()
	  || dates.second < Now() || dates.second > Now() + 180) )
	{
		Application->MessageBox( "Invalid date range", NULL, MB_OK );
		validform = false;
	}

	if( !Password -> Text.IsEmpty()
	 || LCDbOperators::records().findByID( userID ) == NULL )
	{
		if( Password -> Text.Length() < 4 )
		{
			Application->MessageBox( "Password must be at least"
									" 4 characters", NULL, MB_OK );
			validform = false;
		}
	}

	if( validform )
	{
		LCDbOperator theOperator( userID );
		if( !Password->Text.IsEmpty() )
			theOperator.setPassword( Password->Text );

		theOperator.setName( OperatorName->Text );
		theOperator.setDescription( Description->Text );
		theOperator.setValidDates( dates );
		theOperator.setActive( cbxStatus -> Checked );

		theOperator.clearAbilities();
		if( cbVal1->Checked )
			theOperator.addAbility( LCDbOperator::VALIDATION );
		if( cbVal2->Checked )
			theOperator.addAbility( LCDbOperator::L2_VALIDATION );
		if( cbPlotqc->Checked )
			theOperator.addAbility( LCDbOperator::PLOT_QC );
		if( cbWizard->Checked )
			theOperator.addAbility( LCDbOperator::WIZARD );

		theOperator.saveRecord( QueryCentral );
		Close();
	}
}

//---------------------------------------------------------------------------

