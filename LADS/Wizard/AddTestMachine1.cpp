//---------------------------------------------------------------------------
#include <vcl.h>

#include "AddTestMachine1.h"
#include "CentralTestMachine1.h"
#include "ChooseItem1.h"
#include "LCDbAnalyser.h"
#include "WizardLogin1.h"
#include "LCDbObject.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TAddTestMachine *AddTestMachine;

//---------------------------------------------------------------------------

__fastcall TAddTestMachine::TAddTestMachine(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void TAddTestMachine::setTest( int testID )
{
	theTest = LCDbTests::records().get( testID );
}

//---------------------------------------------------------------------------

void __fastcall TAddTestMachine::FormActivate(TObject *Sender)
{
	const LDbNames * item = LCDbAnalysers::records().findByID( machineCID );
	if( item == NULL )
		item = LCDbObjects::records().findByID( machineCID );
	lblMachineName -> Caption = item -> getName();

	lblTest -> Caption = theTest.getName();
	for( LCDbTest::MachineDefs mn = theTest.getCodes(); mn.isValid(); ++ mn )
	{
		if( mn -> getID() == machineCID )
		{
			Caption = "Change " + WizardLogin -> setCaption( "Test Machine" );
			edtMachineTestName -> Text = mn -> getCode();
			edtProtocol -> Text = mn -> getProtocol();
			edtTestOrder -> Text = mn -> getTestOrder();
			return;
		}
	}

	Caption = "Add " + WizardLogin -> setCaption( "Test Machine" );
	edtMachineTestName -> Text = "unknown";
	edtProtocol -> Text = 0;
	edtTestOrder -> Text = 0;
}

//---------------------------------------------------------------------------

void __fastcall TAddTestMachine::SaveClick(TObject *Sender)
{
	bool validform = true;
	if( edtMachineTestName->Text.IsEmpty() )
	{
		Application->MessageBox(
			"MachineTestName must have a value",
			"Add Central Test Machine",MB_OK);
		validform = false;
	}

	if( edtProtocol->Text.IsEmpty() )
	{
		Application->MessageBox(
			"Protocol must have a value",
			"Add Central Test Machine",MB_OK);
		validform = false;
	}

	if( edtTestOrder->Text.IsEmpty() )
	{
		Application->MessageBox(
			"Test Order must have a value",
			"Add Central Test Machine",MB_OK);
		validform = false;
	}

	if( validform )
	{
		LCDbTest::MachineDef mdef( machineCID, edtMachineTestName -> Text,
			edtProtocol -> Text.ToIntDef( 0 ), edtTestOrder -> Text.ToIntDef( 0 ) );
		theTest.saveMachine( QueryCentral, mdef );
		Close();
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddTestMachine::CancelClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

