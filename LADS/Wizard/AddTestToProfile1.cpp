//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AddTestToProfile1.h"
#include "AddProfile1.h"
#include "CentralTest1.h"
#include "LCDbProject.h"
#include "Profile1.h"
#include "CentralTestMachine1.h"
#include "LCDbAnalyser.h"
#include "ObjectName1.h"
#include "LCDbObject.h"
#include "LCDbTest.h"
#include "WizardLogin1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TAddTestToProfile *AddTestToProfile;

//---------------------------------------------------------------------------

__fastcall TAddTestToProfile::TAddTestToProfile(TComponent* Owner)
	: TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TAddTestToProfile::FormShow(TObject *Sender)
{
	const LCDbTest & theTest = LCDbTests::records().get( testID );
	labelTestName -> Caption = theTest.getName();
	lblHasCalc -> Caption = theTest.getCalcParts().isValid() ? "calculated test" : "";

	const LPDbProfile & theProfile = LPDbProfiles::records().get( profileID );
	labelProfileName -> Caption = theProfile.getName();

	const LPDbTestLimits * limits = theProfile.findTestLimits( testID );
	if( limits == NULL )
	{
		theProfileTest = LPDbTestLimits( testID );
		Caption = "Add test to " + WizardLogin -> setCaption( "Test Profile" );
	}
	else
	{	theProfileTest = *limits;
		Caption = "Modify " + WizardLogin -> setCaption( "Test Profile entry" );
	}

	if( theProfileTest.isWarnSet() )
	{
		std::pair< double, double > warnLevels = theProfileTest.getWarnLevels();
		edtLowerWarning -> Text = warnLevels.first;
		edtUpperWarning -> Text = warnLevels.second;
		ckbIgnoreWarning -> Checked = false;
	}
	else
	{	edtLowerWarning -> Text = "";
		edtUpperWarning -> Text = "";
		ckbIgnoreWarning -> Checked = true;
	}

	std::pair< double, double > defaultRange = theTest.getLimits();
	std::pair< double, double > projectRange = theProfileTest.getProjectRange();
	if( projectRange.first > defaultRange.first )
	{
		editLowerLimit -> Text = projectRange.first;
		ckbIgnoreLowerLimit -> Checked = false;
	}
	else
	{	editLowerLimit -> Text = defaultRange.first;
		ckbIgnoreLowerLimit -> Checked = true;
	}

	if( projectRange.second != 0 && projectRange.second < defaultRange.second )
	{
		editUpperLimit -> Text = projectRange.second;
		ckbIgnoreUpperLimit -> Checked = false;
	}
	else
	{	editUpperLimit -> Text = defaultRange.second;
		ckbIgnoreUpperLimit -> Checked = true;
	}

	ckbPrivate -> Checked = theProfileTest.isPrivate();

	std::pair< double, double > triggerRange = theProfileTest.getTriggerLimits();
	editTriggerUpperLimit -> Text = triggerRange.second;
	editTriggerLowerLimit -> Text = triggerRange.first;

	setMachineBox( theProfileTest.getAnalyserID() );
	setTriggerBox( theProfileTest.getTriggerProfile() );
}

//---------------------------------------------------------------------------

void TAddTestToProfile::setMachineBox( int machineID )
{
	std::set< int > machines;
	const LCDbTest & theTest = LCDbTests::records().get( testID );
	if( theTest.getCalcParts().isValid() )
	{
		const LPDbProfile & theProfile = LPDbProfiles::records().get( profileID );
		for( LPDbProfile::Tests ti = theProfile.getTests(); ti.isValid(); ++ ti )
			machines.insert( ti -> getAnalyserID() );
	}
	else
	{	for( LCDbTest::MachineDefs mi = theTest.getCodes(); mi.isValid(); ++ mi )
			machines.insert( mi -> getID() );
	}

	cbxMachines -> Items -> Text = "(unknown)";
	int current = 0;
	for( std::set< int >::const_iterator mi = machines.begin(); mi != machines.end(); ++ mi )
	{
		if( *mi != 0 )
		{
			if( *mi == machineID )
				current = cbxMachines -> Items -> Count;

			const LDbNames * record = LCDbAnalysers::records().findByID( *mi );
			if( record == NULL )
				record = LCDbObjects::records().findByID( *mi );

			cbxMachines -> Items -> Add( record -> getName() );
		}
	}
	cbxMachines -> ItemIndex = current;
}

//---------------------------------------------------------------------------

void TAddTestToProfile::setTriggerBox( int triggerID )
{
	cbxTriggerProfile -> Items -> Text = "(no tests triggered)";
	int current = 0;
	for( LPDbProfiles::Range foo = LPDbProfiles::records(); foo.isValid(); ++ foo )
	{
		if( foo -> getID() == triggerID )
			current = cbxTriggerProfile -> Items -> Count;
		cbxTriggerProfile->Items->Add( foo -> getName() );
	}
	cbxTriggerProfile -> ItemIndex = current;
}

//---------------------------------------------------------------------------

void __fastcall TAddTestToProfile::btnSaveCloseClick(TObject *Sender)
{
	bool validForm = true;
	int buddyID = 0;
	if( cbxMachines -> Enabled )
	{
		const LCDbID * record = LCDbAnalysers::records().findByName( cbxMachines -> Text );
		if( record == NULL )
			record = LCDbObjects::records().find( cbxMachines -> Text, LCDbObject::CLUSTER );
		if( record != NULL )
			buddyID = record -> getID();
	}

	std::pair< double, double > projectlimits( 0, 0 ), warnings( 0, 0 );
	try
	{	if( !ckbIgnoreLowerLimit -> Checked )
			projectlimits.first = editLowerLimit -> Text.ToDouble();

		if( !ckbIgnoreUpperLimit -> Checked )
			projectlimits.second = editUpperLimit -> Text.ToDouble();

		if( !ckbIgnoreWarning -> Checked )
		{
			warnings.first = edtLowerWarning -> Text.ToDouble();
			warnings.second = edtUpperWarning -> Text.ToDouble();
		}
	}
	catch( ... )
	{
		validForm = false;
		Application->MessageBox( "Invalid test limits", NULL, MB_OK );
	}

	std::pair< double, double > triggerLimits( 0, 0 );
	int triggerID = 0;
	const LPDbProfile * profile = LPDbProfiles::records().find( cbxTriggerProfile -> Text );
	if(	profile != NULL )
	{
		try
		{	triggerLimits.first = editTriggerLowerLimit -> Text.ToDouble();
			triggerLimits.second = editTriggerUpperLimit -> Text.ToDouble();
			triggerID = profile -> getID();
		}
		catch( ... )
		{
			validForm = false;
			Application->MessageBox( "Invalid trigger limits", NULL, MB_OK );
		}
	}

	if( validForm )
	{
		theProfileTest.setAnalyserID( buddyID );
		theProfileTest.setPrivate( ckbPrivate -> Checked );
		theProfileTest.setProjectRange( projectlimits );
		theProfileTest.setWarnLevels( warnings );
		theProfileTest.setTriggerLimits( triggerID, triggerLimits );
		LPDbProfile theProfile = LPDbProfiles::records().get( profileID );
		theProfile.addTestLimits( QueryProject, theProfileTest );
		Close();
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddTestToProfile::CancelClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------



