//---------------------------------------------------------------------------

#include <vcl.h>

#include "AddQCLevel1.h"
#include "QCLotClass.h"
#include "AddQCLot1.h"
#include "QCLevelClass.h"
#include "QCMaterial1.h"
#include "WizardLogin1.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"

TAddQCLevel *AddQCLevel;
const int TAddQCLevel::QCL_LOT           = 0;
const int TAddQCLevel::QCL_LOT_DESCRIP   = 1;
const int TAddQCLevel::QCL_ARRIVAL_DATE  = 2;
const int TAddQCLevel::QCL_LOT_INDEX     = 3;
const int TAddQCLevel::QCL_NUMCOLS       = 4;
//---------------------------------------------------------------------------
__fastcall TAddQCLevel::TAddQCLevel(TComponent* Owner)
    : TForm(Owner)
{
    theQCLevel = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TAddQCLevel::FormActivate(TObject *Sender)
{
    Caption = WizardLogin -> setCaption( Function + " QC Level" );
    lblMaterial->Caption = theQCLevel->GetMaterial();
    if ( Function == "Add" )
    {
        edtLevel->Text = "";
        edtDescrip->Text = "";
        edtLevel->Enabled = true;
        btnAddLot->Enabled = false;
        btnChangeLot->Enabled = false;
        ClearGrid();
    }
    else
    {
        btnAddLot->Enabled = true;
        btnChangeLot->Enabled = true;
        edtLevel->Enabled = false;
        edtLevel->Text = theQCLevel->GetName();
        edtDescrip->Text = theQCLevel->GetDescription();
        CopyFromQCLotTableToGrid( QueryCentral );
    }

    QCLevelGrid->ColCount = QCL_NUMCOLS;
    QCLevelGrid->Cells[QCL_LOT][0] = "Lot";
    QCLevelGrid->ColWidths[QCL_LOT] = 65;
    QCLevelGrid->Cells[QCL_LOT_DESCRIP][0] = " Lot description";
    QCLevelGrid->ColWidths[QCL_LOT_DESCRIP] = 110;
    QCLevelGrid->Cells[QCL_ARRIVAL_DATE][0] = "Arrival date";
    QCLevelGrid->ColWidths[QCL_ARRIVAL_DATE] = 110;
    QCLevelGrid->Cells[QCL_LOT_INDEX][0] = "Lot index";
    QCLevelGrid->ColWidths[QCL_LOT_INDEX] = 0;
}
//---------------------------------------------------------------------------
void TAddQCLevel::CopyFromQCLotTableToGrid( TQuery* query )
{
    CopyFromQCLotTableToClass( query );
    CopyFromQCLotClassToGrid();
}
//---------------------------------------------------------------------------
void TAddQCLevel::CopyFromQCLotTableToClass( TQuery* query )
{
    DeleteQCLotClass();

    query -> SQL -> Clear();
    query -> SQL -> Add(
        "select * from qc_lot "
        "where material = :mat "
        "and qc_level = :lev "
        "order by lot desc");
    query -> ParamByName( "mat" )->AsString = theQCLevel->GetMaterial();
    query -> ParamByName( "lev" )->AsString = theQCLevel->GetName();
    query -> Open();
    numQCLots = query->RecordCount;
    theQCLots = new QCLotClass*[numQCLots]; // allow for adding one
    for (int record=0; record < query->RecordCount; record++)
    {
        theQCLots[record] = new QCLotClass;
        theQCLots[record]->LoadFromDatabase(query,record);
    }
}
//---------------------------------------------------------------------------

void TAddQCLevel::DeleteQCLotClass()
{
    for (int index=0; index < numQCLots; index++)
    {
        delete theQCLots[index];
    }
    delete[] theQCLots;
    theQCLots = NULL;
    numQCLots = -1;
}
//---------------------------------------------------------------------------
void TAddQCLevel::ClearGrid()
{
    QCLevelGrid->RowCount = 2;
    QCLevelGrid->FixedRows = 1;
    QCLevelGrid->DeleteRow(1);
    QCLevelGrid->RowCount = 2;
    QCLevelGrid->FixedRows = 1;
}
//---------------------------------------------------------------------------
void TAddQCLevel::CopyFromQCLotClassToGrid()
{
    ClearGrid();

    int row = 0;
    for (int lot=0; lot<numQCLots; lot++)
    {
        row++;
        QCLevelGrid->RowCount = row + 1;
        QCLevelGrid->Cells[QCL_LOT][row] =
            theQCLots[lot]->GetName();
        QCLevelGrid->Cells[QCL_LOT_DESCRIP][row] =
            theQCLots[lot]->GetDescription();
        QCLevelGrid->Cells[QCL_ARRIVAL_DATE][row] =
            theQCLots[lot]->GetArrivalDate();
        QCLevelGrid->Cells[QCL_LOT_INDEX][row] = lot;
    }
}
//---------------------------------------------------------------------------
void __fastcall TAddQCLevel::btnSaveClick(TObject *Sender)
{
    if ( edtLevel->Text == "" )
    {
        Application->MessageBox(
            "Level must not be blank",
            "AddQCLevel Error", MB_OK );
    }
    else
    {
        theQCLevel->SetName(edtLevel->Text);
        theQCLevel->SetDescription(edtDescrip->Text);
        theQCLevel->SetMaterial(lblMaterial->Caption);
		if( Function == "Add" )
        {
            if ( QCMaterial->FindIndexOfQCLevel( QCMaterial->FindIndexOfQCMaterial(
                lblMaterial->Caption ), edtLevel->Text ) != -1 )
            {
                AnsiString strLev = "This level code (" +
                    edtLevel->Text + ") already exists.";
                Application->MessageBox(
                    strLev.c_str(),
                    "QCMaterial Message.", MB_OK);
            }
			else theQCLevel->InsertToDatabase(QueryCentral);
		}
		else theQCLevel->UpdateToDatabase(QueryCentral);

		btnAddLot->Enabled = true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TAddQCLevel::btnCancelClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TAddQCLevel::btnAddLotClick(TObject *Sender)
{
    AddQCLot->SetFunction( "Add" );
    int maxLot = 0;
    for (int record=0; record < numQCLots; record++)
    {
        maxLot = max( maxLot, StrToInt(theQCLots[record]->GetName()) );
    }
    QCLotClass* theQCLot = new QCLotClass;
    theQCLot->SetQCMaterial( lblMaterial->Caption );
    theQCLot->SetQCLevel( edtLevel->Text );
    AddQCLot->SetQCLotClass( theQCLot );
    AddQCLot->SetQCPrevLotNumber( maxLot );
    AddQCLot->ShowModal();
    delete theQCLot;

    CopyFromQCLotTableToGrid( QueryCentral );
}
//---------------------------------------------------------------------------
int TAddQCLevel::FindIndexOfQCLot( AnsiString LotCode )
{
    int index = -1;

    for ( int lot=0; lot<numQCLots; lot++ )
    {
        if ( theQCLots[lot]->GetName() == LotCode )
        {
            index = lot;
        }
    }
    return index;
}
//---------------------------------------------------------------------------
void __fastcall TAddQCLevel::btnChangeLotClick(TObject *Sender)
{
    TColor colour;
    bool rowFound = false;

    for ( int row=1; row<QCLevelGrid->RowCount; row++ )
    {
        if ( QCLevelGrid->GetRowColour( row, colour ) )
        {
			int lot = QCLevelGrid->Cells[QCL_LOT_INDEX][row].ToIntDef( -1 );
			if( lot >= 0 )
			{
				rowFound = true;
				AddQCLot->SetFunction( "Change" );
				AddQCLot->SetQCLotClass( theQCLots[lot] );
				AddQCLot->ShowModal();
			}
			QCLevelGrid->DelRowColour( row );
        }
    }

	if( rowFound )
		CopyFromQCLotTableToGrid( QueryCentral );
	else
	{	Application->MessageBox(
            "No row changed.",
            "AddQCLevel Message",MB_OK);
    }
}
//---------------------------------------------------------------------------
void __fastcall TAddQCLevel::QCLevelGridMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
	QCLevelGrid->MouseToCell( X, Y, Col, Row );
	if( Row < 1 )
	{
        Application->MessageBox(
            "Can not access row.\n"
            "Scroll down and try again.",
            "AddQCLevel Message", MB_OK );
    }
    else
    {

    // if colour is already set, unset it and remove row from list of selected rows
    // if colour is not set, set it and add row to list of selected rows
    TColor colour;
    if ( QCLevelGrid->GetRowColour( Row, colour ) )
    {
        QCLevelGrid->DelRowColour( Row );
    }
    else
    {
        QCLevelGrid->SetRowColour( Row, clLime );
    }
    }
}
//---------------------------------------------------------------------------
void __fastcall TAddQCLevel::btnCloseClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

