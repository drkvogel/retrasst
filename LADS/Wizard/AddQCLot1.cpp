//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddQCLot1.h"
#include "QCTestClass.h"
#include "AddQCLevel1.h"
#include "QCLotClass.h"
#include "CentralTest1.h"
#include "AddQCTest1.h"
#include "ChooseItem1.h"
#include "WizardLogin1.h"
#include "LCDbTest.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"

TAddQCLot *AddQCLot;

const int TAddQCLot::QCLT_TEST           = 0;
const int TAddQCLot::QCLT_PACKET_MEAN    = 1;
const int TAddQCLot::QCLT_PACKET_STDDEV  = 2;
const int TAddQCLot::QCLT_TEST_INDEX     = 3;
const int TAddQCLot::QCLT_NUMCOLS        = 4;
//---------------------------------------------------------------------------
__fastcall TAddQCLot::TAddQCLot(TComponent* Owner)
    : TForm(Owner)
{
    theQCLot = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TAddQCLot::FormActivate(TObject *Sender)
{
    char lotText[5];
    Caption = WizardLogin -> setCaption( Function + " QC Lot" );
    if ( Function == "Add" )
    {
        edtLot->Enabled = true;
        btnAddTest->Enabled = false;
        btnChangeTest->Enabled = false;

		std::sprintf( lotText, "%03d", PrevLotNumber + 1 );
        edtLot->Text = lotText;
        edtDescrip->Text = "";
		edtArrivalDate->Text = TDateTime::CurrentDate();
        lblMaterial->Caption = theQCLot->GetQCMaterial();
        lblLevel->Caption = theQCLot->GetQCLevel();
        btnCopyTests->Enabled = false;
        btnCopyTests->Visible = false;
        ClearGrid();
    }
    else
	{
		edtLot->Enabled = false;
		btnAddTest->Enabled = true;
		btnChangeTest->Enabled = true;

		lblMaterial->Caption = theQCLot->GetQCMaterial();
		lblLevel->Caption = theQCLot->GetQCLevel();
		edtLot->Text = theQCLot->GetName();
		edtDescrip->Text = theQCLot->GetDescription();
		edtArrivalDate->Text = theQCLot->GetArrivalDate();
		CopyFromDatabaseToGrid( QueryCentral );

		PrevLotNumber = StrToInt( theQCLot->GetName() ) - 1;

		if ( PrevLotNumber > 0 )
		{
			std::sprintf( lotText, "%03d", PrevLotNumber );
			AnsiString strLot = lotText;
			btnCopyTests->Caption = "Copy Tests from Lot " + strLot ;
			btnCopyTests->Enabled = true;
			btnCopyTests->Visible = true;
		}
		else
		{
			btnCopyTests->Enabled = false;
			btnCopyTests->Visible = false;
		}
	}

	QCLotGrid->ColCount = QCLT_NUMCOLS;
	QCLotGrid->Cells[QCLT_TEST][0] = "Test";
	QCLotGrid->ColWidths[QCLT_TEST] = 65;
	QCLotGrid->Cells[QCLT_PACKET_MEAN][0] = " Packet Mean";
	QCLotGrid->ColWidths[QCLT_PACKET_MEAN] = 100;
	QCLotGrid->Cells[QCLT_PACKET_STDDEV][0] = "Packet Std dev";
	QCLotGrid->ColWidths[QCLT_PACKET_STDDEV] = 100;
	QCLotGrid->Cells[QCLT_TEST_INDEX][0] = "Lot index";
	QCLotGrid->ColWidths[QCLT_TEST_INDEX] = 0;
}
//---------------------------------------------------------------------------
void TAddQCLot::CopyFromDatabaseToGrid( TQuery* query )
{
	CopyFromDatabaseToClass( query, theQCLot );
	CopyFromClassToGrid();
}
//---------------------------------------------------------------------------
void TAddQCLot::CopyFromDatabaseToClass( TQuery* query, QCLotClass* theQCLot )
{
	query -> SQL -> Clear();
	query -> SQL -> Add(
		"select * from qc_test "
		"where qc_id = :qcid ");
	query -> ParamByName( "qcid" )->AsInteger = theQCLot->GetQCID();
	query -> Open();
	theQCLot->LoadTestsFromDatabase(query);
	theQCLot->SortTestsAlphabetically();
}
//---------------------------------------------------------------------------
void TAddQCLot::ClearGrid()
{
	QCLotGrid->RowCount = 2;
	QCLotGrid->FixedRows = 1;
	QCLotGrid->DeleteRow(1);
	QCLotGrid->RowCount = 2;
	QCLotGrid->FixedRows = 1;
}
//---------------------------------------------------------------------------
void TAddQCLot::CopyFromClassToGrid()
{
	ClearGrid();

	int row = 0;
	for (int test=0; test<theQCLot->GetNumQCTests(); test++)
	{
		row++;
		QCLotGrid->RowCount = row + 1;
		QCLotGrid->Cells[QCLT_TEST][row] =
			theQCLot->GetQCTest(test)->GetName();
		QCLotGrid->Cells[QCLT_PACKET_MEAN][row] =
			theQCLot->GetQCTest(test)->GetPacketMean();
		QCLotGrid->Cells[QCLT_PACKET_STDDEV][row] =
			theQCLot->GetQCTest(test)->GetPacketStdDev();
		QCLotGrid->Cells[QCLT_TEST_INDEX][row] = test;
	}
}
//---------------------------------------------------------------------------
void __fastcall TAddQCLot::btnSaveClick(TObject *Sender)
{
	bool validForm = true;
	try
	{
		StrToInt( edtLot->Text );
	}
	catch(Exception& e)
	{
		Application->MessageBox(
			"Lot must be an integer",
			"AddQCLot Message", MB_OK );
		validForm = false;
	}

	if ( validForm )
	{
		theQCLot->SetName(edtLot->Text);
		theQCLot->SetDescription(edtDescrip->Text);
		theQCLot->SetArrivalDate(edtArrivalDate->Text);
		if ( Function == "Add" )
		{
			theQCLot->SetQCID( LCDbID::allocateID( LQuery( QueryCentral ), 1 ) );
			theQCLot->InsertToDatabase(QueryCentral);
		}
		else
		{
			theQCLot->UpdateToDatabase(QueryCentral);
		}
		btnAddTest->Enabled = true;
    }
}
//---------------------------------------------------------------------------
void __fastcall TAddQCLot::btnCancelClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TAddQCLot::btnAddTestClick(TObject *Sender)
{
	ChooseItem->SetItemType( "Test" );
	for( LCDbTests::Range tr = LCDbTests::records(); tr.isValid(); ++ tr )
	{
		if( tr -> isActive() )
		{
			bool testUsed = false;
			const String & TestName = tr -> getName();
			for (int row=1; row<QCLotGrid->RowCount; row++)
				if ( TestName == QCLotGrid->Cells[QCLT_TEST][row] )
					testUsed = true;

			if( !testUsed )
				ChooseItem->AddItem(TestName);
		}
	}

	if( ChooseItem->GetNumItems() > 0 )
	{
		ChooseItem->ShowModal();
		StrSet setTests = ChooseItem->GetSelectedItems();
		bool rowFound = false;

		for( StrSet::iterator itTest = setTests.begin(); itTest != setTests.end(); ++ itTest )
		{
			const String & testName = *itTest;

			QCTestClass* theQCTest = new QCTestClass( testName );
			AddQCTest->SetFunction( "Add" );
			AddQCTest->SetQCLotClass( theQCLot );
			const LCDbTest * test = LCDbTests::records().findByName( testName );
			theQCTest->SetQCTestCID( test == NULL ? 0 : test -> getID() );
			AddQCTest->SetQCTestClass( theQCTest );
			AddQCTest->ShowModal();
			delete theQCTest;
			rowFound = true;
		}

		if ( rowFound )
			CopyFromDatabaseToGrid( QueryCentral );
		else
			Application->MessageBox( "No tests selected", NULL, MB_OK );
	}
	else
	{
		Application->MessageBox("All tests already selected for this profile.",
			"AddQCLot",MB_OK);
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddQCLot::btnChangeTestClick(TObject *Sender)
{
	TColor colour;
	bool rowFound = false;
	for ( int row=1; row<QCLotGrid->RowCount; row++ )
    {
        if ( QCLotGrid->GetRowColour( row, colour ) )
        {
			int tindex = QCLotGrid->Cells[QCLT_TEST_INDEX][row].ToIntDef( -1 );
			if( tindex >= 0 )
			{
				rowFound = true;
				AddQCTest->Caption = "Change QC Test";
				AddQCTest->SetQCLotClass( theQCLot );
				AddQCTest->SetQCTestClass( theQCLot->GetQCTest(tindex) );
				AddQCTest->ShowModal();
			}
			QCLotGrid->DelRowColour( row );
		}
	}
	if( rowFound )
		CopyFromDatabaseToGrid( QueryCentral );
	else
    {   Application->MessageBox(
            "No row changed.",
            " Message",MB_OK);
    }
}
//---------------------------------------------------------------------------

void __fastcall TAddQCLot::QCLotGridMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	QCLotGrid->MouseToCell( X, Y, Col, Row );
	if( Row < 1 )
	{
		Application->MessageBox(
			"Can not access row.\n"
			"Scroll down and try again.",
			"AddQCLot Message", MB_OK );
	}
	else
	{

	// if colour is already set, unset it and remove row from list of selected rows
	// if colour is not set, set it and add row to list of selected rows
	TColor colour;
	if ( QCLotGrid->GetRowColour( Row, colour ) )
		QCLotGrid->DelRowColour( Row );
	else
		QCLotGrid->SetRowColour( Row, clLime );
	}
}
//---------------------------------------------------------------------------

void __fastcall TAddQCLot::btnCopyTestsClick(TObject *Sender)
{
    // copy tests from previous lot to current lot
    // get lot for PrevLotNumber
    QCLotClass* thePrevQCLot = new QCLotClass();

    if ( GetPreviousLot( thePrevQCLot, QueryCentral ) )
    {
        // get all tests for PrevLotNumber
        CopyFromDatabaseToClass( QueryCentral, thePrevQCLot );

        for ( int test=0; test<thePrevQCLot->GetNumQCTests(); test++ )
        {
            // set lot number to current lot number
            thePrevQCLot->GetQCTest(test)->SetQCID( theQCLot->GetQCID() );
            // insert all tests to database
            thePrevQCLot->GetQCTest(test)->InsertToDatabase( QueryCentral );
        }
    }
    delete thePrevQCLot;

    CopyFromDatabaseToGrid( QueryCentral );
}
//---------------------------------------------------------------------------

bool TAddQCLot::GetPreviousLot( QCLotClass* thePrevQCLot, TQuery* query )
{
    bool prevLotFound = true;
    char lotText[5];

    query -> SQL -> Clear();
    query -> SQL -> Add(
        "select * from qc_lot "
        "where material = :mat "
        "and qc_level = :lev "
        "and lot = :lot "
        "order by lot desc");
    query -> ParamByName( "mat" )->AsString = lblMaterial->Caption;
    query -> ParamByName( "lev" )->AsString = lblLevel->Caption;
    std::sprintf( lotText, "%03d", PrevLotNumber );
    AnsiString strLot = lotText;
    query -> ParamByName( "lot" )->AsString = strLot;
    query -> Open();
    if ( query->RecordCount <= 0 )
    {
        Application->MessageBox(
            "No previous lot found",
            "AddQCLot Error", MB_OK );
        prevLotFound = false;
    }
    else if ( query->RecordCount > 1 )
    {
		Application->MessageBox(
			"More than one record found for previous lot",
			"AddQCLot Error", MB_OK );
		prevLotFound = false;
	}
	else
	{
		thePrevQCLot->LoadFromDatabase(query,0);
	}

	return prevLotFound;
}
//---------------------------------------------------------------------------

void __fastcall TAddQCLot::btnDeleteTestClick(TObject *Sender)
{
	TColor colour;
	if( Application->MessageBox( "Are you sure you want to delete these tests?",
								 "Confirmation", MB_YESNO | MB_ICONQUESTION ) == IDYES )
	{
		bool rowFound = false;

		for ( int row=0; row<QCLotGrid->RowCount; row++ )
		{
			if ( QCLotGrid->GetRowColour( row, colour ) )
			{
				rowFound = true;
				DeleteTest( row );
				QCLotGrid->DelRowColour( row );
			}
		}
		if ( rowFound )
		{
			CopyFromDatabaseToGrid( QueryCentral );
		}
		else
		{
			Application->MessageBox(
				"No row deleted.",
				"Add QC Lot Message",MB_OK);
		}
	}
	else
	{
		for ( int row=0; row<QCLotGrid->RowCount; row++ )
		{
			if ( QCLotGrid->GetRowColour( row, colour ) )
			{
				QCLotGrid->DelRowColour( row );
			}
		}
	}
}
//---------------------------------------------------------------------------
void TAddQCLot::DeleteTest( int row )
{
    theQCLot->GetQCTest(QCLotGrid->Cells[QCLT_TEST][row])->
        DeleteFromDatabase(QueryCentral);
}
//---------------------------------------------------------------------------

void __fastcall TAddQCLot::btnCloseClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

