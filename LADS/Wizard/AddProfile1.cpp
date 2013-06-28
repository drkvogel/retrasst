//---------------------------------------------------------------------------
#include <vcl.h>

#include "AddProfile1.h"
#include "LCDbAnalyser.h"
#include "LCDbProject.h"
#include "LCDbObject.h"
#include "LDbBoxType.h"
#include "Profile1.h"
#include "AddTestToProfile1.h"
#include "ChooseItem1.h"
#include "CentralTest1.h"
#include "AddProjectNote1.h"
#include "AddTestCalculation1.h"
#include "ObjectName1.h"
#include "Login1.h"
#include "LQuery.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TAddProfile *AddProfile;

//---------------------------------------------------------------------------

__fastcall TAddProfile::TAddProfile(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TAddProfile::FormActivate(TObject *Sender)
{
	DefineTestGrid();
	DefineBoxTypeGrid();
	boxRowMoved = false;

	Caption = Login -> setProjectCaption( "Profile" );
	LPDbProfile theProfile;
	const LPDbProfile * found = LPDbProfiles::records().findByID( profileID );
	if( found != NULL )
		theProfile = *found;
/*	{
		Change->Enabled = false;
		dragInstructions->Visible = false;
		Add->Enabled = false;
		ProfileName->Text = "";
		Description->Text = "";
		DateLastModifiedLabel->Caption = "";
		cbxTests -> Checked = true;
		cbxSpecimen -> Checked = true;
		cbxAliquots -> Checked = true;
		cbxActive -> Checked = true;
		pgProfile->ActivePage = tabTests;
	}
	else
*/	{	ProfileName->Text = theProfile.getName();
		Description->Text = theProfile.getDescription();
		cbxActive -> Checked = theProfile.isActive();
		cbxTests -> Checked = theProfile.isTested();
		cbxSpecimen -> Checked = theProfile.isStored();
		cbxAliquots -> Checked = theProfile.isAliquoted();
		cbxActive -> Checked = theProfile.isActive();
		if( theProfile.getBoxTypes().isValid() )
		{
				pgProfile->ActivePage = tabBoxTypes;
				Change -> Enabled = false;
				dragInstructions -> Visible = true;
		}
		else
		{		pgProfile->ActivePage = tabTests;
				Change -> Enabled = theProfile.getTests().isValid();
				dragInstructions -> Visible = false;
		}

		Add->Enabled = true;
	Delete->Enabled = false;

		CopyFromTestClassToGrid();
		CopyFromBoxTypeClassToGrid();
	}
}

//---------------------------------------------------------------------------

void TAddProfile::DefineTestGrid()
{
	grdTest -> RowCount = 2;
	grdTest -> FixedRows = 1;
	grdTest->ColCount = ATP_NUMCOLS;
	grdTest->Cells[ATP_TESTID][0] = "Test ID";
	grdTest->ColWidths[ATP_TESTID] = 0;
	grdTest->Cells[ATP_TESTNAME][0] = "Test name";
	grdTest->ColWidths[ATP_TESTNAME] = 60;
	grdTest->Cells[ATP_MACHINEID][0] = "Machine ID";
	grdTest->ColWidths[ATP_MACHINEID] = 0;
	grdTest->Cells[ATP_MACHINENAME][0] = "Machine name";
	grdTest->ColWidths[ATP_MACHINENAME] = 80;
	grdTest->Cells[ATP_UPPERWARNING][0] = "Upper warning";
	grdTest->ColWidths[ATP_UPPERWARNING] = 58;
	grdTest->Cells[ATP_LOWERWARNING][0] = "Lower warning";
	grdTest->ColWidths[ATP_LOWERWARNING] = 58;
	grdTest->Cells[ATP_UPPERLIM][0] = "Upper limit";
	grdTest->ColWidths[ATP_UPPERLIM] = 58;
	grdTest->Cells[ATP_LOWERLIM][0] = "Lower limit";
	grdTest->ColWidths[ATP_LOWERLIM] = 58;
	grdTest->Cells[ATP_TRIGGERUPPER][0] = "Trig. upper";
	grdTest->ColWidths[ATP_TRIGGERUPPER] = 58;
	grdTest->Cells[ATP_TRIGGERLOWER][0] = "Trig. lower";
	grdTest->ColWidths[ATP_TRIGGERLOWER] = 58;
	grdTest->Cells[ATP_TRIGGERID][0] = "Trigger ID";
	grdTest->ColWidths[ATP_TRIGGERID] = 0;
	grdTest->Cells[ATP_TRIGGERNAME][0] = "Trigger name";
	grdTest->ColWidths[ATP_TRIGGERNAME] = 110;
}

//---------------------------------------------------------------------------

void TAddProfile::DefineBoxTypeGrid()
{
	grdBoxType -> RowCount = 2;
	grdBoxType -> FixedRows = 1;
	grdBoxType->ColCount = BOX_NUMCOLS;
	grdBoxType->Cells[BOX_BOXTYPEID][0] = "Box type ID";
	grdBoxType->ColWidths[BOX_BOXTYPEID] = 0;
	grdBoxType->Cells[BOX_BOXNAME][0] = "Box name";
	grdBoxType->ColWidths[BOX_BOXNAME] = grdBoxType->Width - 10;
}

//---------------------------------------------------------------------------

void __fastcall TAddProfile::AddClick(TObject *Sender)
{
	if( pgProfile->ActivePage == tabBoxTypes )
		AddBoxType();

	else if( pgProfile->ActivePage == tabTests )
		AddTest();
}

//---------------------------------------------------------------------------

void TAddProfile::AddTest()
{
	ChooseItem->SetItemType( "Test" );
	for( LCDbTests::Range tr = LCDbTests::records(); tr.isValid(); ++ tr )
	{
		if( tr -> isActive() )
		{
			const String & name = tr -> getName();
			bool testUsed = false;
			for( int row=1; row < grdTest->RowCount; row++ )
				if( grdTest->Cells[ ATP_TESTNAME ][ row ] == name )
					testUsed = true;

			if( !testUsed )
				ChooseItem->AddItem( name );
		}
	}

	bool rowFound = false;
	if( ChooseItem->GetNumItems() > 0 )
	{
		ChooseItem->ShowModal();
		StrSet setTests = ChooseItem->GetSelectedItems();
		AddTestToProfile->setProfile( profileID );
		for( StrSet::const_iterator it = setTests.begin(); it != setTests.end(); ++ it )
		{
			const LCDbTest * test = LCDbTests::records().findByName( *it );
			if( test != NULL )
			{
				AddTestToProfile -> setTest( test -> getID() );
				AddTestToProfile->ShowModal();
				rowFound = true;
			}
		}
	}

	if ( rowFound )
		CopyFromTestClassToGrid();
	else
		Application->MessageBox( "No tests selected", NULL, MB_OK );
}

//---------------------------------------------------------------------------

void TAddProfile::AddBoxType()
{
	ChooseItem->SetItemType( "Box Type" );
	for( LPDbBoxTypes::Range type = LPDbBoxTypes::records(); type.isValid(); ++ type )
	{
		if( type -> isActive() )
		{
			const String & name = type -> getName();
			bool boxTypeUsed = false;
			for( int row=1; row < grdBoxType -> RowCount; row++ )
				if( grdBoxType->Cells[ BOX_BOXNAME ][ row ] == name )
					boxTypeUsed = true;

			if ( !boxTypeUsed )
				ChooseItem->AddItem( name );
		}
	}

	// add the selected box types
	bool rowFound = false;
	if( ChooseItem->GetNumItems() > 0 )
	{
		ChooseItem->ShowModal();
		StrSet boxTypes = ChooseItem->GetSelectedItems();
		LPDbProfile theProfile = LPDbProfiles::records().get( profileID );
		for( StrSet::const_iterator bt = boxTypes.begin(); bt != boxTypes.end(); ++ bt )
		{
			const LPDbBoxType * boxType = LPDbBoxTypes::records().find( *bt );
			if( boxType != NULL )
			{
				theProfile.addBoxType( QueryProject, boxType -> getID() );
				rowFound = true;
			}
		}
	}

	if ( rowFound )
		CopyFromBoxTypeClassToGrid();
	else
		Application->MessageBox( "No box types selected", NULL, MB_OK );
}

//---------------------------------------------------------------------------

void __fastcall TAddProfile::ChangeClick(TObject *Sender)
{
	bool rowFound = false;
	for( std::set< int >::const_iterator si = selected.begin(); si != selected.end(); ++ si )
	{
		int tid = grdTest -> Cells[ ATP_TESTID ][ *si ].ToIntDef( 0 );
		if( tid != 0 )
		{
			rowFound = true;
			AddTestToProfile->setProfile( profileID );
			AddTestToProfile -> setTest( tid );
			AddTestToProfile->ShowModal();
		}
	}

	if( rowFound )
	{
		CopyFromTestClassToGrid();
		CopyFromBoxTypeClassToGrid();
	}
	else Application->MessageBox( "No row changed", NULL, MB_OK );
}

//---------------------------------------------------------------------------

void __fastcall TAddProfile::DeleteClick(TObject *Sender)
{
	if( pgProfile->ActivePage == tabBoxTypes )
		DeleteBoxType();

	else if ( pgProfile->ActivePage == tabTests )
		DeleteTest();
}

//---------------------------------------------------------------------------

void TAddProfile::DeleteTest()
{
	Application->MessageBox( "No row deleted", NULL, MB_OK );
}

//---------------------------------------------------------------------------

void TAddProfile::DeleteBoxType()
{
	Application->MessageBox( "No row deleted", NULL, MB_OK );
}

//---------------------------------------------------------------------------

void TAddProfile::CopyFromTestClassToGrid()
{
	selected.clear();
	int row = 1;
	const LPDbProfile & theProfile = LPDbProfiles::records().get( profileID );
	for( LPDbProfile::Tests tr = theProfile.getTests(); tr.isValid(); ++ tr )
	{
		int mcid = tr -> getAnalyserID();
		const LDbNames * record = LCDbAnalysers::records().findByID( mcid );
		if( record == NULL )
			record = LCDbObjects::records().findByID( mcid );
		if( record == NULL )
			grdTest->Cells[ATP_MACHINENAME][row] = "Unknown";
		else
			grdTest->Cells[ATP_MACHINENAME][row] = record -> getName();
		grdTest->Cells[ATP_MACHINEID][row] = mcid;

		const LCDbTest & test = LCDbTests::records().get( tr -> getID() );
		grdTest->Cells[ATP_TESTID][row] = test.getID();
		grdTest->Cells[ATP_TESTNAME][row] = test.getName();

		std::pair< double, double > range = tr -> getProjectRange();
		grdTest->Cells[ATP_LOWERLIM][row] = display( range.first );
		grdTest->Cells[ATP_UPPERLIM][row] = display( range.second );

		range = tr -> getWarnLevels();
		grdTest->Cells[ATP_LOWERWARNING][row] = display( range.first );
		grdTest->Cells[ATP_UPPERWARNING][row] = display( range.second );

		int trigID = tr -> getTriggerProfile();
		grdTest->Cells[ATP_TRIGGERID][row] = trigID;
		if( trigID != 0 )
		{
			range = tr -> getTriggerLimits();
			grdTest->Cells[ATP_TRIGGERNAME][row] = LPDbProfiles::records().get( trigID ).getName();
			grdTest->Cells[ATP_TRIGGERLOWER][row] = display( range.first );
			grdTest->Cells[ATP_TRIGGERUPPER][row] = display( range.second );
		}
		else
		{	grdTest->Cells[ATP_TRIGGERLOWER][row] = "";
			grdTest->Cells[ATP_TRIGGERUPPER][row] = "";
			grdTest->Cells[ATP_TRIGGERNAME][row] = "";
		}
		row ++;
	}

	if( row > 1 )
		grdTest -> RowCount = row;
	else
	{	grdTest -> RowCount = 2;
		for( int col = 0; col < ATP_NUMCOLS; col ++ )
			grdTest -> Cells[ col ][ 1 ] = "";
	}
	grdTest -> FixedRows = 1;
}

//---------------------------------------------------------------------------

String TAddProfile::display( double num )
{
	if( num > 0 )
		return num;
	else
		return "";
}

//---------------------------------------------------------------------------

void TAddProfile::CopyFromBoxTypeClassToGrid()
{
	selected.clear();
	LPDbProfile::BoxTypes bt = LPDbProfiles::records().get( profileID ).getBoxTypes();
	int row = 1;
	if( bt.isValid() )
	{
		do
		{	const LPDbBoxType & type = LPDbBoxTypes::records().get( bt -> getID() );
			grdBoxType->Cells[BOX_BOXNAME][row] = type.getName();
			grdBoxType->Cells[BOX_BOXTYPEID][row] = type.getID();
			row ++;
		} while( (++ bt).isValid() );
	}

	if( row > 1 )
		grdBoxType -> RowCount = row;
	else
	{	grdBoxType -> RowCount = 2;
		for( int col = 0; col < BOX_NUMCOLS; col ++ )
			grdBoxType -> Cells[ col ][ 1 ] = "";
	}
	grdBoxType->FixedRows = 1;
}

//---------------------------------------------------------------------------

void __fastcall TAddProfile::grdTestMouseDown(
	  TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	grdTest->MouseToCell( X, Y, Col, Row );

	if( selected.count( Row ) == 0 )
		selected.insert( Row );
	else
		selected.erase( Row );

	grdTest -> Repaint();
}

//---------------------------------------------------------------------------

void __fastcall TAddProfile::SaveClick(TObject *Sender)
{
	if( SaveProfile() )
	{
		Add->Enabled = true;
		Change->Enabled = true;
		Delete->Enabled = true;

		if( Caption.SubString(1,3) == "Add" )
		{
			AnsiString strCaption = Caption.SubString(5,49);
			Caption = "Change " + strCaption;
		}
	}
}

//---------------------------------------------------------------------------

bool TAddProfile::SaveProfile()
{
	if( ProfileName->Text.IsEmpty() )
	{
		Application->MessageBox( "Invalid test profile name", NULL, MB_OK );
		return false;
	}
	else
	{	LPDbProfile theProfile;
		if( profileID != 0 )
			theProfile = LPDbProfiles::records().get( profileID );

		theProfile.setName(ProfileName->Text);
		theProfile.setDescription(Description->Text);
		theProfile.setActive( cbxActive->Checked );
		theProfile.setTested( cbxTests->Checked );
		theProfile.setAliquoted( cbxAliquots->Checked );
		theProfile.setStored( cbxSpecimen->Checked );
		theProfile.saveRecord( QueryProject );
		profileID = theProfile.getID();
		return true;
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddProfile::pgProfileChange(TObject *Sender)
{
	if( pgProfile->ActivePage == tabBoxTypes )
	{
		Change->Enabled = false;
		dragInstructions->Visible = true;
	}
	else if ( pgProfile->ActivePage == tabTests )
	{
		Change->Enabled = true;
		dragInstructions->Visible = false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TAddProfile::grdBoxTypeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col, Row = 0;
	grdBoxType->MouseToCell( X, Y, Col, Row );

	if( selected.count( Row ) == 0 )
		selected.insert( Row );
	else
		selected.erase( Row );

	grdBoxType -> Repaint();
}

//---------------------------------------------------------------------------

void TAddProfile::MoveRow( int fromRow, int toRow )
{
	for( int col=0; col<grdBoxType->ColCount; col++ )
	   grdBoxType->Cells[col][toRow] = grdBoxType->Cells[col][fromRow];
}

//---------------------------------------------------------------------------

void __fastcall TAddProfile::CancelClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TAddProfile::btnCloseClick(TObject *Sender)
{
	if( profileID == 0 || checkProfile() )
		Close();
}

//---------------------------------------------------------------------------

bool TAddProfile::checkProfile()
{
	const LPDbProfile & theProfile = LPDbProfiles::records().get( profileID );
	String question;
	if( theProfile.getBoxTypes().isValid() )
	{
		if( !cbxAliquots -> Checked && !cbxSpecimen -> Checked )
			question = "Set 'store source' or 'take aliquots' to use box types.";
	}
	else if( cbxAliquots -> Checked || cbxSpecimen -> Checked )
			question = "'store source' and 'take aliquots' need box types.";

	if( !question.IsEmpty() )
	{
		question += "\nClose form anyway?";
		if( Application->MessageBox( question.c_str(), NULL, MB_YESNO ) == IDNO )
		{
			return false;
		}
	}

	if( theProfile.getTests().isValid() )
	{
		question = cbxTests -> Checked ? ""
				 : "When there are tests, 'Run test profile' should be set.";
	}
	else
	{	question = !cbxTests -> Checked ? ""
				 : "'Run test profile' is set but no tests have been added.";
	}

	for( LPDbProfile::Tests tr1 = theProfile.getTests(); tr1.isValid(); ++ tr1 )
	{
		if( tr1 -> getAnalyserID() == 0 )
		{
			question = "Analyser has not been specified for all tests.";
			break;
		}

		LCDbTest::CalcDefs def = LCDbTests::records().get( tr1 -> getID() ).getCalcParts();
		while( question.IsEmpty() && def.isValid() )
		{
			for( LPDbProfile::Tests tr2 = theProfile.getTests(); tr2.isValid(); ++ tr2 )
			{
				if( tr2 -> getID() == def -> getSourceTestID()
				 && tr2 -> getAnalyserID() != tr1 -> getAnalyserID() )
				{
					question = "Analyser for calculated test does not match source.";
					break;
				}
			}
			++ def;
		}
	}

	if( question.IsEmpty() )
		return true;
	else
	{	question += "\nClose form anyway?";
		return Application->MessageBox( question.c_str(), NULL, MB_YESNO ) == IDYES;
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddProfile::grdBoxTypeDrawCell(TObject *Sender, int ACol,
	  int ARow, TRect &Rect, TGridDrawState State)
{
	if( !State.Contains( gdFixed ) )
	{
		TCanvas * canvas = grdBoxType -> Canvas;
		if( selected.count( ARow ) == 0 )
			canvas -> Brush -> Color = clWhite;
		else
			canvas -> Brush -> Color = clLime;
		canvas -> FillRect( Rect );

		int mid = (Rect.Bottom + Rect.Top - abs( canvas -> Font -> Height )) / 2 - 1;
		canvas -> Font -> Color = clBlack;
		canvas -> TextOut( Rect.Left + 2, mid, grdBoxType -> Cells[ ACol ][ ARow ] );
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddProfile::grdTestDrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
	if( !State.Contains( gdFixed ) )
	{
		TCanvas * canvas = grdTest -> Canvas;
		if( selected.count( ARow ) == 0 )
			canvas -> Brush -> Color = clWhite;
		else
			canvas -> Brush -> Color = clLime;
		canvas -> FillRect( Rect );

		int mid = (Rect.Bottom + Rect.Top - abs( canvas -> Font -> Height )) / 2 - 1;
		canvas -> Font -> Color = clBlack;
		canvas -> TextOut( Rect.Left + 2, mid, grdTest -> Cells[ ACol ][ ARow ] );
	}
}

//---------------------------------------------------------------------------


