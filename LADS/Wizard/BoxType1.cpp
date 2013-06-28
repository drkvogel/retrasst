//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BoxType1.h"
#include "BoxTypeClass.h"
#include "ProjectClass.h"
#include "LogMod1.h"
#include "AddBoxType1.h"
#include "AllAliquotTypes1.h"
#include "DataMod1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"
TBoxType *BoxType;
const int TBoxType::BT_NAME             = 0;
const int TBoxType::BT_STATUS           = 1;
const int TBoxType::BT_CAPACITY         = 2;
const int TBoxType::BT_TYPE_ID          = 3;
const int TBoxType::BT_TYPE_IDX         = 4;
const int TBoxType::BT_NUM_COLS         = 5;
//---------------------------------------------------------------------------
__fastcall TBoxType::TBoxType(TComponent* Owner)
    : TForm(Owner)
{
    theProject = NULL;
    theBoxTypes = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TBoxType::FormActivate(TObject *Sender)
{
    Caption = "Box Types for " + theProject->GetName();
    Caption = DataMod->SetProjectCaption( Caption );

    BoxTypeTable->RowCount = 2;
    BoxTypeTable->ColCount = BT_NUM_COLS;
    BoxTypeTable->Cells[BT_NAME][0] = "Box";
    BoxTypeTable->ColWidths[BT_NAME] = 170;
    BoxTypeTable->Cells[BT_STATUS][0] = "Status";
    BoxTypeTable->ColWidths[BT_STATUS] = 90;
    BoxTypeTable->Cells[BT_CAPACITY][0] = "Capacity";
    BoxTypeTable->ColWidths[BT_CAPACITY] = 70;
    BoxTypeTable->Cells[BT_TYPE_ID][0] = "Box ID";
    BoxTypeTable->ColWidths[BT_TYPE_ID] = 0;
    BoxTypeTable->Cells[BT_TYPE_IDX][0] = "Box Index";
    BoxTypeTable->ColWidths[BT_TYPE_IDX] = 0;

    AllAliquotTypes = new TAllAliquotTypes;
    AllAliquotTypes->CopyFromDatabaseToClass( QueryCentral );
    CopyFromDatabaseToGrid( QueryCentral );
}
//---------------------------------------------------------------------------
void __fastcall TBoxType::AddClick(TObject *Sender)
{
    AddBoxType->SetQueryCentral( QueryCentral );
    BoxTypeClass* theBoxType;
    theBoxType = new BoxTypeClass;
    theBoxType->SetProjectID( theProject->GetProjectID() );
    AddBoxType->SetBoxType( theBoxType );
    AddBoxType->SetFunction( "Add" );
    AddBoxType->ShowModal();
    delete theBoxType;
    CopyFromDatabaseToGrid( QueryCentral );
}
//---------------------------------------------------------------------------

void __fastcall TBoxType::ChangeClick(TObject *Sender)
{
    TColor colour;
    bool rowFound = false;
    for ( int row=1; row<BoxTypeTable->RowCount; row++ )
    {
        if ( BoxTypeTable->GetRowColour( row, colour ) )
        {
            rowFound = true;
            AddBoxType->SetQueryCentral( QueryCentral );
            AddBoxType->SetBoxType(
                theBoxTypes[StrToInt( BoxTypeTable->Cells[BT_TYPE_IDX][row] )] );
            AddBoxType->SetFunction( "Change" );
            AddBoxType->ShowModal();
            BoxTypeTable->DelRowColour( row );
        }
    }
    if ( rowFound )
    {
        CopyFromDatabaseToGrid( QueryCentral );
    }
    else
    {
        Application->MessageBox(
            "No row changed.",
            "Box Message",MB_OK);
    }
}
//---------------------------------------------------------------------------

void __fastcall TBoxType::DeleteClick(TObject *Sender)
{
    TColor colour;
    int Button = Application->MessageBox(
        "Are you sure you want to delete these boxes?",
        "Box Message",MB_YESNO);
    if ( Button == IDYES )
    {
        bool boxDeleted = false;

        for ( int row=0; row<BoxTypeTable->RowCount; row++ )
        {
            if ( BoxTypeTable->GetRowColour( row, colour ) )
            {
                boxDeleted = DeleteBox( row, QueryCentral );
                BoxTypeTable->DelRowColour( row );
            }
        }
        if ( boxDeleted )
        {
            CopyFromDatabaseToGrid( QueryCentral );
        }
        else
        {
            Application->MessageBox(
                "No row deleted.",
                "Box Message",MB_OK);
        }
    }
    else
    {
        for ( int row=0; row<BoxTypeTable->RowCount; row++ )
        {
            if ( BoxTypeTable->GetRowColour( row, colour ) )
            {
                BoxTypeTable->DelRowColour( row );
            }
        }
    }
}
//---------------------------------------------------------------------------
bool TBoxType::DeleteBox( int row, TQuery* query )
{
// return true if deleted, false is not deleted
    bool deleted = false;
    bool okay_to_delete = true;

//  check for presence of box_type_cid in c_box_name and c_box_content
    int boxID = StrToInt(BoxTypeTable->Cells[BT_TYPE_ID][row]);
    int box = StrToInt(BoxTypeTable->Cells[BT_TYPE_IDX][row]);

//  The profile_map must not contain this value_id.
//  If it does, issue error message and don't delete.
    AnsiString tableName = "box_name";
    if ( theProject->isBeforeVersion( 2, 2 ) )
        tableName = "c_box_name";
    if ( IsBoxTypeInTable( boxID, tableName, DataMod->QueryProject ) )
    {
        okay_to_delete = false;
        AnsiString strMessage =
            "A " + tableName + " record uses this box_type_cid.";
        Application->MessageBox(
            strMessage.c_str(),
            "Box Message",MB_OK);
    }
    tableName = "c_box_content";
    if ( IsBoxTypeInTable( boxID, tableName, query ) )
    {
        okay_to_delete = false;
        Application->MessageBox(
            "A c_box_content record uses this box_type_cid.",
            "Box Message",MB_OK);
    }

    if ( okay_to_delete )
    {
        theBoxTypes[box]->DeleteFromDatabase(query);
        deleted = true;
    }
    return deleted;
}
//---------------------------------------------------------------------------
bool TBoxType::IsBoxTypeInTable( int boxID, AnsiString tableName, TQuery* query )
{
    bool InTable = true;

    query -> Close();
    query -> SQL -> Clear();
    AnsiString strSQL = "SELECT * FROM " + tableName +
        " WHERE box_type_cid = :boxid ";
    query -> SQL -> Add( strSQL );
    query->ParamByName("boxid")->AsInteger = boxID;
    query -> Open();

    if ( query->RecordCount > 0 ) InTable = false;
    LogMod->Commit( query );
    query -> Active = false;
    return InTable;
}
//---------------------------------------------------------------------------

void TBoxType::CopyFromClassToGrid()
{
    BoxTypeTable->RowCount = 2;
    BoxTypeTable->FixedRows = 1;
    BoxTypeTable->DeleteRow(1);
    BoxTypeTable->RowCount = 2;
    BoxTypeTable->FixedRows = 1;

    int row = 0;
    for (int box=0; box<numBoxes; box++)
    {
        row++;
        BoxTypeTable->RowCount = row + 1;
        BoxTypeTable->Cells[BT_NAME][row] = theBoxTypes[box]->GetBoxName();
        BoxTypeTable->Cells[BT_TYPE_ID][row] =
            IntToStr( theBoxTypes[box]->GetBoxTypeID() );
        BoxTypeTable->Cells[BT_TYPE_IDX][row] = IntToStr(box);
        BoxTypeTable->Cells[BT_STATUS][row] =
            AddBoxType->cbxStatus->Items->Strings[theBoxTypes[box]->GetStatus()];
        BoxTypeTable->Cells[BT_CAPACITY][row] =
            IntToStr( theBoxTypes[box]->GetBoxCapacity() );
    }
}
//---------------------------------------------------------------------------

void TBoxType::CopyFromDatabaseToGrid( TQuery* query )
{
// delete all values, create anew from database, copy into grid
    CopyFromDatabaseToClass( query );
    CopyFromClassToGrid();
}

//---------------------------------------------------------------------------

void TBoxType::CopyFromDatabaseToClass( TQuery* query )
{
    DeleteBoxTypeClass();

    query -> SQL -> Clear();
    query -> SQL -> Add("Select * from c_box_type "
        "where project_cid = :pid "
        "order by external_name ");
    query -> ParamByName( "pid" )->AsInteger = theProject->GetProjectID();
    query -> Open();
    numBoxes = query->RecordCount;
    theBoxTypes = new BoxTypeClass*[numBoxes];
    for (int box=0; box < numBoxes; box++)
    {
        theBoxTypes[box] = new BoxTypeClass;
        theBoxTypes[box]->LoadFromDatabase(query, box);
    }
    query -> SQL -> Clear();
    LogMod->Commit( query );
    query -> Active = false;
}

//---------------------------------------------------------------------------

void __fastcall TBoxType::btnCloseClick(TObject *Sender)
{
// delete all boxes
    DeleteBoxTypeClass();
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TBoxType::BoxTypeTableMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int Col = 0;
    int Row = 0;
    BoxTypeTable->MouseToCell( X, Y, Col, Row );
    if ( Row == -1 )
    {
        Application->MessageBox(
            "Can not access row.\n"
            "Scroll down and try again.",
            "Box Message", MB_OK );
    }
    else
    {

// if colour is already set, unset it and remove row from list of selected rows
// if colour is not set, set it and add row to list of selected rows

    TColor colour;
    if ( BoxTypeTable->GetRowColour( Row, colour ) )
    {
        BoxTypeTable->DelRowColour( Row );
    }
    else
    {
        BoxTypeTable->SetRowColour( Row, clLime );
    }
    }
}
//---------------------------------------------------------------------------

void TBoxType::DeleteBoxTypeClass()
{
    if ( theBoxTypes != NULL )
    {
        for (int box=0; box < numBoxes; box++)
        {
            delete theBoxTypes[box];
        }
        delete[] theBoxTypes;
        theBoxTypes = NULL;
    }
    numBoxes = -1;
}
//---------------------------------------------------------------------------
