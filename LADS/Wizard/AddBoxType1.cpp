//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddBoxType1.h"
#include "BoxTypeClass.h"
#include "LogMod1.h"
#include "DataMod1.h"
#include "DbShared.h"
#include "AliquotTypeClass.h"
#include "ChooseItem1.h"
#include "BoxType1.h"
#include "AllAliquotTypes1.h"
#include "AllAliquotClass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"
TAddBoxType *AddBoxType;
const int TAddBoxType::ALI_NAME       = 0;
const int TAddBoxType::ALI_ALIORDER   = 1;
const int TAddBoxType::ALI_ALITYPEID  = 2;
const int TAddBoxType::ALI_ALITYPEIDX = 3;
const int TAddBoxType::ALI_NUMCOLS    = 4;
//---------------------------------------------------------------------------
__fastcall TAddBoxType::TAddBoxType(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TAddBoxType::FormActivate(TObject *Sender)
{
    DefineAliquotGrid();
    aliquotRowMoved = false;

    Caption = DataMod->SetCaption( Function + " Box Type" );
    if (Function == "Add")
    {
        edtBoxName->Text = "";
        cbxStatus->ItemIndex = 0;
        edtCapacity->Text = "99";
        btnAddAliquot->Enabled = false;
        btnDeleteAliquot->Enabled = false;
        edtCapacity->Enabled = true;
        cbxStatus->Enabled = true;
    }
    else
    {
        edtBoxName->Text = theBoxType->GetBoxName();
        cbxStatus->ItemIndex = theBoxType->GetStatus();
        edtCapacity->Text = theBoxType->GetBoxCapacity();
        btnAddAliquot->Enabled = true;
        btnDeleteAliquot->Enabled = true;
        edtCapacity->Enabled = false;
        cbxStatus->Enabled = false;
    }
    CopyFromDatabaseToGrid();
}
//---------------------------------------------------------------------------
void TAddBoxType::DefineAliquotGrid()
{
    grdAliquot->RowCount = 2;
    grdAliquot->FixedRows = 1;
    grdAliquot->DeleteRow(1);
    grdAliquot->RowCount = 2;
    grdAliquot->FixedRows = 1;

    grdAliquot->ColCount = ALI_NUMCOLS;
    grdAliquot->Cells[ALI_ALITYPEID][0] = "Aliquot type ID";
    grdAliquot->ColWidths[ALI_ALITYPEID] = 0;
    grdAliquot->Cells[ALI_NAME][0] = "Aliquot name";
    grdAliquot->ColWidths[ALI_NAME] = 120;
    grdAliquot->Cells[ALI_ALIORDER][0] = "Order";
    grdAliquot->ColWidths[ALI_ALIORDER] = 30;
    grdAliquot->Cells[ALI_ALITYPEIDX][0] = "index of aliquot type";
    grdAliquot->ColWidths[ALI_ALITYPEIDX] = 0;
}
//---------------------------------------------------------------------------
void __fastcall TAddBoxType::SaveClick(TObject *Sender)
{
    if ( SaveBoxType() )
    {
        if ( SaveAliquotTypes() )
        {
            AddBoxType->Close();
        }
    }
}
//---------------------------------------------------------------------------
bool TAddBoxType::SaveBoxType()
{
    int validform = true;
    short tempshort;

    if ( edtBoxName->Text == "")
    {
        Application->MessageBox("Invalid box name","Add Box",MB_OK);
        validform = false;
    }

    if ( edtCapacity->Text == "")
    {
        Application->MessageBox("Invalid capacity","Add Box",MB_OK);
        validform = false;
    }

    if ( validform )
    {
        if (Function == "Add")
        {
            int boxTypeID = DbShared::getNextCID(QueryCentral);
            theBoxType->SetBoxTypeID( boxTypeID );
        }
        else
        {
            theBoxType->LogRecord( "Update: Old record: " );
        }

        theBoxType->SetBoxName( edtBoxName->Text );

        tempshort = (short)cbxStatus->ItemIndex;
        theBoxType->SetStatus(tempshort);
        tempshort = (short)StrToInt( edtCapacity->Text );
        theBoxType->SetBoxCapacity(tempshort);

        if (Caption.SubString(1,3) == "Add")
        {
            theBoxType->InsertToDatabase( QueryCentral );
        }
        else
        {
            theBoxType->UpdateToDatabase( QueryCentral );
        }
    }
    return validform;
}
//---------------------------------------------------------------------------
bool TAddBoxType::SaveAliquotTypes()
{
    // two aliquot orders are the same, do not save, return false
    if ( aliquotRowMoved )
    {
        for ( int row=1; row<grdAliquot->RowCount; row++ )
        {
            AliquotTypeClass* theAliquotType =
                theBoxType->GetAliquotType(
                StrToInt( grdAliquot->Cells[ALI_ALITYPEIDX][row] ) );
            theAliquotType->LogRecord( "Update: Old record: " );
            theAliquotType->SetAliquotOrder(
                StrToInt( grdAliquot->Cells[ALI_ALIORDER][row] ) );
            theAliquotType->UpdateToDatabase( QueryCentral );
        }
    }
    return true;
}
//---------------------------------------------------------------------------
bool TAddBoxType::CheckDuplicateAliquotOrder()
{
    bool valid_aliquot_types = true;
    // two aliquot orders are the same, do not save, return false
    for ( int row=0; row<grdAliquot->RowCount; row++ )
    {
        for ( int row1=0; row1<grdAliquot->RowCount; row1++ )
        {
            if ( ( row != row1 ) &&
                ( grdAliquot->Cells[ALI_ALIORDER][row] ==
                grdAliquot->Cells[ALI_ALIORDER][row1] ) )
            {
                if ( valid_aliquot_types )
                {
                    Application->MessageBox(
                        "Two or more aliquots have the same order number",
                        "AddBoxType Error", MB_OK );
                }
                valid_aliquot_types = false;
            }
        }
    }
    return valid_aliquot_types;
}
//---------------------------------------------------------------------------

void __fastcall TAddBoxType::CancelClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TAddBoxType::btnAddAliquotClick(TObject *Sender)
{
    // copy unused box types from box type class to ChooseItemGrid
    AnsiString AliquotTypeName;
    ChooseItem->SetItemType( "Aliquot" );
    ChooseItem->ClearGrid();
    AllAliquotClass** theAliquots = BoxType->AllAliquotTypes->GetAllAliquots();

    for (int type=0; type<BoxType->AllAliquotTypes->GetNumAllAliquots(); type++)
    {
        bool aliquotTypeUsed = false;
        AliquotTypeName = theAliquots[type]->GetAliquotName();
        for (int row=1; row<grdAliquot->RowCount; row++)
        {
            if ( AliquotTypeName == grdAliquot->Cells[ALI_NAME][row] )
            {
                aliquotTypeUsed = true;
            }
        }
        if ( !aliquotTypeUsed ) ChooseItem->AddItem(AliquotTypeName);
    }
    int order = theBoxType->GetNumAliquots();
    if ( ChooseItem->GetNumItems() > 0 )
    {
        ChooseItem->SetItemType( "Aliquot  Type" );
        ChooseItem->ShowModal();
        strSet setAliquots = ChooseItem->GetSelectedItems();
        bool rowFound = false;
        strSet::iterator itAliquot = setAliquots.begin();
        while ( itAliquot != setAliquots.end() )
        {
            AnsiString aliquotName = *itAliquot;
            int id = BoxType->AllAliquotTypes->GetAliquotTypeIDFromName( aliquotName );
            AliquotTypeClass* theAliquotType = new AliquotTypeClass;
            theAliquotType->SetAliquotTypeID(id);
            theAliquotType->SetBoxTypeID(theBoxType->GetBoxTypeID());
            theAliquotType->SetAliquotOrder( order++ );
            theAliquotType->InsertToDatabase( QueryCentral );
            delete theAliquotType;
            rowFound = true;
            itAliquot++;
        }
        if ( rowFound )
        {
            CopyFromDatabaseToGrid();
        }
        else
        {
            Application->MessageBox(
                "No aliquot types selected.",
                "AddProfile",MB_OK);
        }
    }
    else
    {
        Application->MessageBox(
            "All aliquot types already selected for this box type.",
            "AddProfile",MB_OK);
    }
}
//---------------------------------------------------------------------------

void __fastcall TAddBoxType::btnDeleteAliquotClick(TObject *Sender)
{
    TColor colour;
    int Button = Application->MessageBox(
        "Are you sure you want to delete these aliquots?",
        "Add Box Type Message",MB_YESNO);
    if ( Button == IDYES )
    {
        bool rowFound = false;

        for ( int row=0; row<grdAliquot->RowCount; row++ )
        {
            if ( grdAliquot->GetRowColour( row, colour ) )
            {
                rowFound = true;
                DeleteAliquotRow( row );
                grdAliquot->DelRowColour( row );
            }
        }
        if ( rowFound )
        {
            CopyFromDatabaseToGrid();
        }
        else
        {
            Application->MessageBox(
                "No row deleted.",
                "Add Box Type Message",MB_OK);
        }
    }
    else
    {
        for ( int row=0; row<grdAliquot->RowCount; row++ )
        {
            if ( grdAliquot->GetRowColour( row, colour ) )
            {
                grdAliquot->DelRowColour( row );
            }
        }
    }
}
//---------------------------------------------------------------------------
void TAddBoxType::DeleteAliquotRow( int row )
{
    theBoxType->GetAliquotType(grdAliquot->Cells[ALI_NAME][row])->
        DeleteFromDatabase(QueryCentral);

    return;
}
//---------------------------------------------------------------------------
void TAddBoxType::CopyFromDatabaseToGrid()
{
    CopyFromDatabaseToClass();
    CopyFromClassToGrid();
}
//---------------------------------------------------------------------------

void  TAddBoxType::CopyFromDatabaseToClass()
{
    theBoxType->DeleteAliquotTypes();

    int boxTypeID = theBoxType->GetBoxTypeID();
    QueryCentral -> SQL -> Clear();
    QueryCentral -> SQL -> Add(
        "select box_type_cid, "
        " aliquot_type_cid, aliquot_order, "
        " external_name "
        " from c_box_content c, c_object_name o "
        "where c.box_type_cid = :bid "
        "and   c.aliquot_type_cid = o.object_cid "
        "order by aliquot_order");
    QueryCentral->ParamByName("bid")->AsInteger = boxTypeID;

    try
    {
        QueryCentral -> Open();
    }
    catch(Exception& e)
    {
        if(e.Message == "Error creating cursor handle")
        {
            //do nothing
        }
        else
        {
            AnsiString strError =
                "Error reading aliquot type\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "AddBoxType Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }

    theBoxType->LoadAliquotTypesFromDatabase(QueryCentral);

    LogMod->Commit( QueryCentral );
    QueryCentral -> Active = false;
}
//---------------------------------------------------------------------------

void  TAddBoxType::CopyFromClassToGrid()
{
    grdAliquot->RowCount = 2;
    grdAliquot->FixedRows = 1;
    grdAliquot->DeleteRow(1);
    grdAliquot->RowCount = 2;
    grdAliquot->FixedRows = 1;

    int row = 0;
    int numAliquots = theBoxType->GetNumAliquots();

    for (int type=0; type<numAliquots; type++)
        {
        row++;
        grdAliquot->RowCount = row + 1;
        grdAliquot->Cells[ALI_NAME][row] =
            theBoxType->GetAliquotType(type)->GetName();
        grdAliquot->Cells[ALI_ALIORDER][row] =
            theBoxType->GetAliquotType(type)->GetAliquotOrder();
        grdAliquot->Cells[ALI_ALITYPEID][row] =
            theBoxType->GetAliquotType(type)->GetAliquotTypeID();
        grdAliquot->Cells[ALI_ALITYPEIDX][row] = IntToStr(type);
    }
}
//---------------------------------------------------------------------------

void __fastcall TAddBoxType::grdAliquotMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int Col = 0;
    mouseDownRow = 0;
    grdAliquot->MouseToCell( X, Y, Col, mouseDownRow );
    if ( mouseDownRow == -1 )
    {
        Application->MessageBox(
            "Can not access row.\n"
            "Scroll down and try again.",
            "AddBoxType Message", MB_OK );
    }
}
//---------------------------------------------------------------------------

void __fastcall TAddBoxType::grdAliquotMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int Col = 0;
    int mouseUpRow = 0;
    grdAliquot->MouseToCell( X, Y, Col, mouseUpRow );
    if ( mouseDownRow == -1 )
    {
        Application->MessageBox(
            "Can not access row.\n"
            "Scroll down and try again.",
            "AddBoxType Message", MB_OK );
    }
    else
    {
        if ( mouseDownRow == mouseUpRow )
        {
            // if colour is already set,
            //   unset it and remove row from list of selected rows
            // if colour is not set,
            //   set it and add row to list of selected rows
            TColor colour;
            if ( grdAliquot->GetRowColour( mouseDownRow, colour ) )
            {
                grdAliquot->DelRowColour( mouseDownRow );
            }
            else
            {
                grdAliquot->SetRowColour( mouseDownRow, clLime );
            }
        }
        else
        {
            MoveRow( mouseDownRow, grdAliquot->RowCount );
            if ( mouseDownRow > mouseUpRow )
            {
                // move rows down to make room for row moving up
                for ( int row=mouseDownRow; row>mouseUpRow; row-- )
                {
                    MoveRow( row-1, row );
                }
            }
            else
            {
                // move rows up to make room for row moving down
                for ( int row=mouseDownRow; row<mouseUpRow; row++ )
                {
                    MoveRow( row+1, row );
                }
            }
            MoveRow( grdAliquot->RowCount, mouseUpRow );
            aliquotRowMoved = true;
        }
    }
}
//---------------------------------------------------------------------------
void TAddBoxType::MoveRow( int fromRow, int toRow )
{
    for ( int col=0; col<grdAliquot->ColCount; col++ )
    {
        if ( col == ALI_ALIORDER )
        {
            grdAliquot->Cells[col][toRow] = IntToStr( toRow );
        }
        else
        {
            grdAliquot->Cells[col][toRow] = grdAliquot->Cells[col][fromRow];
        }
    }
}
//---------------------------------------------------------------------------

void __fastcall TAddBoxType::btnCloseClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------

