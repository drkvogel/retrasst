//---------------------------------------------------------------------------
//
// v2.02 -- 06/09/00, ACC
//          Add ability to rebuild profile map (ProfileMap)
//          Rework entire form code, especially all checks.
//          Move building of new map descriptors from Descriptor to
//          ProfileMap.
// v2.03 -- 18/10/00, ACC
//          Recompile with latest next_id.
//          Don't crash out if Ingres not started.
//          Issue message; when OK is pressed, try again.
// v2.04 -- 24/10/00, ACC
//          Change table name in AddCentralTest from Edit box to Combo box.
//          Make date field in AddQCItem wide enough to display 4 digit year.
//          Change abilities in AddOperator from Edit box to Combo box.
//          Default CTSUID to 9999. Add check for duplicate name.
// v2.05 -- 27/10/00, ACC
//          Fix TPReport to work for cluster names.
// v2.06 -- 13/11/00, ACC
//          In PrepareProfileMap check for numMapColumns != 0,
//          ie, cater for empty profile map.
// v2.07 -- 15/11/00, ACC
//          Update for C++ Builder 5:
//          Change all SubString(0, to SubString(1,
//          Allocate the correct number of instances of classes,
//          instead of declaring large arrays.
// v2.08 -- 15/11/00, ACC
//          Use ACTIVE_STATUS instead of NEW_STATUS, for new and updated records.
//          When updating, do not change status, write old and new data
//          to audit log. This affects ALL classes.
//          Change handling of QC lot, QC tests, etc. to have a similar
//          "look and feel" to the rest of the Wizard.
// v2.09 -- 07/12/00, ACC
//          For all "mapping" classes, don't "update for delete",
//          instead write to audit log and delete. This is because
//          duplicate information could be re-inserted.
//          This and the changes in 2.08 will enable us to be able to use
//          Ingres' own checks for uniqueness and integrity.
//          "Mapping" classes: TPTest, MapDescriptor, LocalTestMachine,
//          CentralTestMachine, ClusterMachine, CRTest, Trigger, QCTest.
//          Not "mapping" but included as well: QCMaterial, QCLevel.
// v2.10 -- 06/02/02, ACC
//          Display only current operators with all abilities at login time.
//          Correct bug which always set project status to "Test" on Save
//          in AddProject.
//          Change grants on cust_in table: use lab_group; issue message
//          to request Core Programmers to grant access to trial users,
//          because Lab user probably doesn't know trial username. (This
//          won't be necessary when cust_in, cust_out, and sample are
//          combined.)
// v2.11 -- 25/03/02, ACC
//          Move code into DataMod, LogMod, DateMod, LeaseMod. Now most of
//          it is in a class.
//          Change ProjectDatabase to be a distributed database
//          (Ingres STAR database). This database combines tables
//          in both the central database and the project database.
//          At login display only active operators with OPER_ABIL_ALL.
//          Use latest auditing and associated classes: DbAuditTrail,
//          DbShared, LQuery, XMLFile, etc.
// V??   -- 18/04/02, ACC
//          Change default date from 01/01/1900 to 30/12/1899.
// v2.12 -- 12/07/02, ACC
//          Require entry of profile at time of selecting new profile map rows
//          (in AddProfileMap). This conforms to the constraint that all
//          profiles must be defined in test_profile_description.
// v2.13 -- 15/07/02, ACC
//          Replace all calls to UpdateToDeleteStatus with DeleteFromDatabase.
// v2.14 -- 20/09/02, ACC
//          Start working on changes for Transition database(s).
//          Get rid of use of DELETE_STATUS and DELETE_TIMESTAMP.
//          Except for DELETE_STATUS in TEST_PROFILE_DESCRIPTION.
// v2.15 -- 03/012/02, ACC
//          All other changes for Transition database(s),
//          especially add handling of aliquot profiles under Profile.
//          Also store test_profile_id instead of trigger_id in c_trigger;
//          then tests will not be stored in c_trigger.
// v2.16 -- 13/12/02, ACC
//          Machine assignments for a test for a profile are now stored in
//          test_profile instead of test_machine.
// v2.17 -- 17/12/02, ACC
//          Trigger profile selection for profile tests now works correctly.
//          AddTestToProfile form now closes immediately after Save.
//          Include latest DBShared and DBAuditTrail.
// v2.18 -- 21/01/03, ACC
//          Enable QC level to be entered when selecting "Add level"
// v2.19 -- 26/02/03, ACC
//          Add ability to specify all box types for a project.
//          Add ability to select box types used by a profile and to order them.
//          Add ability to specify aliquots in a box type.
//          Move descriptor value handling to under descriptor.
// v2.20 -- 10/04/03, ACC
//          Add code to cope with potential move of qc_tables from qc to central
//          database.
// v2.21 -- 16/04/03, ACC
//          Add ability to create project database (using REXEC).
//          Improve ability to cancel out of AddValue form.
// v2.22 -- 23/04/03, ACC
//          Add handling of md5 version of password.
// v2.23 -- 08/05/03, ACC
//          Handle operator abilities as binary sum.
// v2.24 -- 14/05/03, ACC
//          Change QueryProject to QueryCentral in all QC code.
//          Get next central id for QC (not project id).
// v2.25 -- 02/06/03, ACC
//          In ProfileMap make sure that map field numbers in descrip table
//          are consecutive and start with 1.
// v2.25.1 -- 02/06/03, ACC
//          Disable "Delete" button for descriptors.
//          Do not allow specimen_field_name to be changed for barcode and
//          byhand descriptors.
// v2.25.2 -- 02/06/03, ACC
//          Use latest version of lease code. This gets rid of the message
//          Martin and Nick were seeing about problems accessing a database.
// v2.25.3 -- 08/07/03, ACC
//          Correct select of descrip table by map_field_name to take account
//          of double digit map field numbers. (in Descriptor1.cpp)
// v3.0    -- 10/02/04, ACC
//          Upgrade to handle database v2.1
//          Change all "#define" to "static const".
//          Pass objects as arguments rather than using global variables.
// v3.1    -- 05/03/04, ACC
//          Store object_type when add new c_object_name records.
//          Improvements to handling of object names.
//          Other minor improvements to adding tests and central test machines.
// v3.2    -- 12/03/04, ACC
//          Limit descriptor value length to 12.
//          Check that min and max values are the same for byhand descriptors.
//          For non-byhand descriptors where min and max values are not the same,
//          check that they are base 36 numbers and the minbase36 < maxbase36.
//          For new projects when automatically adding the descriptor Analysis,
//          add Aliquot_type as well.
//          When adding Aliquot_type values, check that they are aliquot types
//          in c_object_name.
// v3.3    -- 16/03/04, ACC
//          Barcode descriptors must have non-zero parse order.
//          Specimen descriptors may have specimen field name of next free value
//          column or column named "barcode"
//          New flag column for descriptors: indicates patient/visit or tube
//          identification. Used by AddSpecimens program.
// v3.3.1  -- 17/03/04, ACC
//          For descriptor flag add: initial settings depending on "source";
//                                   validation.
// v3.3.2  -- 18/03/04, ACC
//          When updating in "AddCentralTestMachine", set correct machine name.
//          On close in AddTestProfile if the machine_cid for a claculated test
//          is 0, set it to the machine_cid of one of its associated tests.
//          In BoxType where checking entry in c_box_name before delete,
//          change to check box_name if v2.1 or later.
// v3.3.3  -- 22/03/04, ACC
//          Added check for v2.1 to CopyFromDatabaseToClassByMapFN which is called
//          by ProfileMap.
//          numQCLevels was local to function, should have been local to class
//          QCMaterialClass.
//          Dithering about whether to set theProject in each class or to
//          get its value from DataMod. Gradually moving toward the latter.
//          Changed LogMod to conform to this.
//          Updated all QC classes to LogMod->LogCentralDb instead of
//          LogMod->LogProjectDb, also to call LogMod->SetCaption instead of
//          LogMod->SetProjectCaption. (Because the QC_ tables had been moved
//          from the qc project database to the central database.)
//         -- 24/03/04, ACC
//          Add call to AddOperator->SetFunction before adding or changing
//          Operator.
// v3.3.4  -- 31/03/04, ACC
//          Increment NextSpecimenNum and NextMapNum after getting them in
//          DescriptorClass.
// v3.3.5  -- 02/04/04, ACC
//          Add length of descriptor value to validation check.
//          On exiting Descriptor form issue message if a descriptor does not
//          have any values.
//          Make Descriptor->numDescriptors and Descriptor->theDescriptors
//          non-global.
// v3.3.6  -- 05/04/04, ACC
//          New method of handling new profile maps with list box of
//          descriptor values, rather than a giant grid of all possible
//          combinations.
// v3.3.7  -- 19/04/04, ACC
//          Corrected a couple of ProfileMap bugs.
// v3.3.8  -- 23/04/04, ACC
//          Change to check box_name if v2.2 or later.
//          Change ProfileMap to allow single-value descriptors to have that
//          value or "any value".
// v3.4.0  -- 25/05/04, ACC
//          Prohibit update of database name for projects
//          Add database v2.2 changes -
//             new tables: c_box_size, box_content, box_name
//             obsolete tables: c_box_content, c_box_name, c_box_type
//          Move SetCaption and SetProjectCaption from LogMod to DataMod.
//
//	v3.5.0	-- 2 July 2004, NG
//			added box_set_link to box_content for v2.2 projects
//			removed data_type from descrip (always string)
//		   	check box_content in place of c_box_content for v2.2+
//
//	v3.5.1	-- SHARP release: various changes to match db v2.2
//
//	v3.5.2	12 October 1004, NG
//			incorporate DateMod, DataMod, MachineClass and ProjectClass
//
//  v3.5.3	April 2005, NG
//			Allow user to "delete" objects by setting status to 99
//
// 	v3.6.0	May - July 2005, NG
//          Use new shared classes LDbXXX for tests, projects etc.
//			Reintroduce start_delimiter in descrip definition and allow
//	 		parsing of specimen value fields as well as the bar code.
//          Release databases when switching between live and mirror system.
//			Added rack size definition.  Minor changes to user interface.
//          Added tank mapping (create only - storage management could improve)
//
//	v3.7.0, 26/11/05, NG:	Insert, don't update, profile map if requested
//          5/01/06, NG:	check descrip specimen/map field name before saving
//
//	v3.7.1, 13/1/06, NG:	store rack type in c_tank_map + first letter in name
//          27/1/06, NG:	moved add tank (mapping) button from store to tank
//			31/1/06, NG:	check aliquot descriptor name matches object type
//			30/3/06, NG:	use c_test_definition valid_from/valid_to date
//          4/04/06, NG:	replaced colour string grids with Borland's
//          25/4/06, NG:	added tank/rack/section layout to object types
//
//  v3.7.2, 05/01/07, NG:	split test profile flags from storage type
//          09/01/07, NG:	add tube type definition to c_box_size
//			26/01/07, NG:	add laptops to c_object_name (for BoxMerge)
//---------------------------------------------------------------------------
//
// Future developments:
//
//  	For most tables:
//          Add database procedure which executes "on update" to check
//			associated data and disallow the update if that data exists.
//
//          If a descriptor has not been used in the specimen table,
//          allow it to be deleted.
//          Otherwise, delete will tag the descriptor somehow.
//          If the descriptor is tagged, do not display the descriptor
//          in the descriptor grid.
//          Have profile map code handle gaps in profile map columns and to
//          ignore tagged descriptors.
//
//      Ability to update records:
//          when status moves beyond an initial value, do not allow updates
//
//      AddProfileMap complains if there is not at least one value for each
//          descriptor. AddProfileMapNew does not complain. Should they
//          AddProfileMapNew behave in the same way as AddProfileMap?
//          The reason for this was so that the profile map
//          ould not be set up with every entry having a descriptor set to
//          "any value" when it was to be given values in the future.
//          In this case, the entire profile map would have to be re-done,
//          as the value for this descriptor would be wrong for every entry.
//
//---------------------------------------------------------------------------
#include <vcl.h>

#include "WizardLogin1.h"
#include "Login1.h"
#include "TfrmAboutBox.h"
#include "LCDbAnalyser.h"
#include "LCDbObject.h"
#include "LCDbProject.h"
#include "LCDbOperator.h"
#include "LCDbTest.h"
#include "LDbBoxType.h"
#include "leaseIDs.h"
#include "LCDbBoxStore.h"
#include "LDbBoxSize.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TWizardLogin *WizardLogin;

const char * TfrmAboutBox::author = "Nicholas Goodwin";
const char * TfrmAboutBox::compiled = __DATE__;
const char * TfrmAboutBox::extension = "x3532";
const char * LCDbAuditTrail::default_email = "Nicholas.Goodwin@ctsu.ox.ac.uk";
const int LCDbProjects::leaseID = LEASEE_WIZARD;

//---------------------------------------------------------------------------

__fastcall TWizardLogin::TWizardLogin(TComponent* Owner) : TForm(Owner), trail( cQuery )
{
	SystemChoice -> ItemIndex = MIRROR_SYSTEM;
	Application -> OnException = AppException;
}

//---------------------------------------------------------------------------
//  Display exceptions and add them to the audit trail; re-initialise
//---------------------------------------------------------------------------

void __fastcall TWizardLogin::AppException(TObject *Sender, Exception *E)
{
	const String error = E -> Message;
	try
	{   trail.sendEMail( error );
		trail.addRecord( error, LCDbAuditTrail::ERROR_BASE );
	}
	catch( ... )
	{}

	static const String title = Application -> Title + " Error";
	Application -> MessageBox( error.c_str(), title.c_str(), MB_ICONWARNING );
}

//---------------------------------------------------------------------------

void __fastcall TWizardLogin::FormActivate(TObject *Sender)
{
	cbxOperator->Text = "";
	edtPassword->Text = "";
	SystemChoiceClick( Sender );
}

//---------------------------------------------------------------------------

void __fastcall TWizardLogin::SystemChoiceClick(TObject *Sender)
{
	LCDbProjects::releaseDatabases();
	LCDbProject::changeDbName( cDatabase, isMirror() ? "t_ldbc" : "ldbc" );
}

//---------------------------------------------------------------------------

void __fastcall TWizardLogin::btnOperatorOKClick(TObject *Sender)
{
	const LCDbOperator * user = LCDbOperators::records().check( cbxOperator -> Text, edtPassword -> Text );
	if( user != NULL )
	{
		LCDbObjects::records().read( cQuery, true );
		LCDbTests::records().read( cQuery, true );
		LCDbProjects::records().read( cQuery, true );
		LCDbAnalysers::records().read( cQuery, true );
		LCDbBoxSizes::records().read( cQuery );
		LCDbRackSizes::records().read( cQuery );

		user -> makeCurrent();
		edtPassword->Text = "";
		trail.login();
		Hide();
		Login -> ShowModal();
		Show();
	}
	else Application->MessageBox( "Invalid operator name or password", NULL, MB_OK );
}

//---------------------------------------------------------------------------

void __fastcall TWizardLogin::AboutButtonClick(TObject *Sender)
{
	frmAboutBox->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TWizardLogin::btnExitClick(TObject *Sender)
{
	LCDbProjects::releaseDatabases();
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TWizardLogin::cbxOperatorDropDown(TObject *Sender)
{
	LCDbOperators::records().read( cQuery, true );
	cbxOperator -> Clear();
	for( LCDbOperators::Range ul = LCDbOperators::records(); ul.isValid(); ++ ul )
		if( ul -> isActive() && ul -> hasAbility( LCDbOperator::WIZARD ) )
			cbxOperator -> Items -> Add( ul -> getDescription() );

	if( cbxOperator -> Items -> Count < 1 )
	{
		Application->MessageBox( "No operators found to run the Wizard",
								 "Login Error", MB_OK );
	}
}

//---------------------------------------------------------------------------

