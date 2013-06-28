//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AddMachine1.h"
#include "Descriptor1.h"
#include "ObjectName1.h"
#include "LCDbObject.h"
#include "WizardLogin1.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TAddMachine *AddMachine;

//---------------------------------------------------------------------------

__fastcall TAddMachine::TAddMachine(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TAddMachine::FormActivate(TObject *Sender)
{
	FillClusterBox();
	const LCDbAnalyser * theMachine = LCDbAnalysers::records().findByID( analyserID );
	cbxCluster -> Text = "";
	if( theMachine != NULL )
	{
		Caption = "Change Machine";
		MachineName->Text = theMachine -> getName();
		Description->Text = theMachine -> getDescription();
		Location->Text = theMachine -> getLocation();

		std::pair< TDate, TDate > dates = theMachine -> getValidDates();
		dtpValidFrom->DateTime = dates.first;
		dtpValidTo->DateTime = dates.second;
		rgStatus -> ItemIndex = theMachine -> isActive() ? RGS_ACTIVE : RGS_OBSOLETE;

		int cid = theMachine -> getClusterID();
		const LCDbObject * cluster = LCDbObjects::records().findByID( cid );
		if( cluster != NULL )
			cbxCluster -> Text = cluster -> getName();
	}
	else
	{	Caption = "Add Machine";
		MachineName->Text = "";
		Description->Text = "";
		dtpValidFrom->DateTime = TDateTime::CurrentDate();
		dtpValidTo->DateTime = Now() + 3653;	// 10 years from now
		Location->Text = "S?";
		rgStatus -> ItemIndex = RGS_ACTIVE;
	}
	WizardLogin -> setCaption( Caption );
}

//---------------------------------------------------------------------------

void TAddMachine::FillClusterBox()
{
	cbxCluster -> Items -> Text = "(not clustered)";
	for( LCDbObjects::Range or = LCDbObjects::records(); or.isValid(); ++ or )
		if( or -> getObjectType() == LCDbObject::CLUSTER )
			cbxCluster->Items->Add( or -> getName() );
}

//---------------------------------------------------------------------------

void __fastcall TAddMachine::SaveClick(TObject *Sender)
{
	std::pair< TDate, TDate > dates( dtpValidFrom -> Date, dtpValidTo -> Date );
	bool validDate = (dates.second > dates.first
				 && dates.second > Now() && dates.first < Now());

	if( rgStatus -> ItemIndex == RGS_ACTIVE && !validDate )
		Application->MessageBox( "Invalid from/to dates", NULL, MB_OK);

	else if( MachineName->Text.IsEmpty() )
		Application->MessageBox( "Invalid machine name", NULL, MB_OK );

	else if( Location->Text.IsEmpty() )
		Application->MessageBox( "Location must have a value", NULL, MB_OK );
	else
	{	LCDbAnalyser theMachine( analyserID );
		theMachine.setName( MachineName->Text );
		theMachine.setDescription( Description->Text.SubString( 1, 64 ) );
		theMachine.setValidDates( dates );
		theMachine.setLocation( Location->Text );
		theMachine.setActive( rgStatus -> ItemIndex == RGS_ACTIVE );

		const LCDbObject * cluster = LCDbObjects::records().find( cbxCluster -> Text, LCDbObject::CLUSTER );
		theMachine.setClusterID( cluster == NULL ? 0 : cluster -> getID() );
		theMachine.saveRecord( QueryCentral );
		Close();
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddMachine::CancelClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

