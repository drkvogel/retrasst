//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BoxTypeClass.h"
#include "LogMod1.h"
#include "AliquotTypeClass.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

BoxTypeClass::BoxTypeClass()
: BoxName(""),numAliquotTypes(0)
{
    BoxTypeID = 0;
    BoxCapacity = 0;
    ProjectID = 0;
    Status = 0;
}

BoxTypeClass::BoxTypeClass(AnsiString aName)
: BoxName(aName),numAliquotTypes(0)
{
    BoxTypeID = 0;
    BoxCapacity = 0;
    ProjectID = 0;
    Status = 0;
}


__fastcall BoxTypeClass::~BoxTypeClass()
{
    DeleteAliquotTypes();
}

void BoxTypeClass::DeleteAliquotTypes()
{
    if ( theAliquotTypes != NULL )
    {
        for (int type=0; type < numAliquotTypes; type++)
        {
            delete theAliquotTypes[type];
        }
        delete[] theAliquotTypes;
    }
    theAliquotTypes = NULL;
    numAliquotTypes = -1;
}

void BoxTypeClass::LoadFromDatabase(TQuery* query, int recno)
{
    if(query -> RecordCount <= recno)
    {
        //error, index out of range
        return;
    }
    else
    {
        query -> First();
        for (int x = 0; x < recno; x++)
        {
            query -> Next();
        }

        try
        {
            BoxTypeID = query -> FieldByName("box_type_cid") -> AsInteger;
            ProjectID = query -> FieldByName("project_cid") -> AsInteger;
            BoxName = query -> FieldByName("external_name") -> AsString;
            Status = (short)query -> FieldByName("status") -> AsInteger;
            BoxCapacity = (short)query -> FieldByName("box_capacity") -> AsInteger;
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading Box from database.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "BoxTypeClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
}

void BoxTypeClass::LoadAliquotTypesFromDatabase(TQuery* query)
{
    numAliquotTypes = query -> RecordCount;
    theAliquotTypes = new AliquotTypeClass*[numAliquotTypes];
    for(int x = 0; x < numAliquotTypes; x++)
    {
        theAliquotTypes[x] = NULL;
    }
    query -> First();

    for (int x = 0; x < numAliquotTypes; x++)
    {
        if( query->FieldByName("box_type_cid")->AsInteger == BoxTypeID )
        {
            theAliquotTypes[x] = new AliquotTypeClass(
                query->FieldByName("external_name")->AsString );
            theAliquotTypes[x]->SetAliquotTypeID(
                query->FieldByName("aliquot_type_cid")->AsInteger);
            theAliquotTypes[x]->SetAliquotOrder(
                (short)query->FieldByName("aliquot_order")->AsInteger);
            theAliquotTypes[x]->SetBoxTypeID(
                query->FieldByName("box_type_cid")->AsInteger );
        }
        query -> Next();
    }
}


void BoxTypeClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " c_box_type: ";
    LogText += "box_type_cid=" + IntToStr( BoxTypeID ) + ",";
    LogText += "project_cid=" + IntToStr( ProjectID ) + ",";
    LogText += "external_name=" + BoxName + ",";
    LogText += "status=" + IntToStr( Status ) + ",";
    LogText += "box_capacity=" + IntToStr( BoxCapacity ) + ",";
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, LogText );
}

void BoxTypeClass::InsertToDatabase(TQuery* query)
{
//  write new record with NEW status
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add(
        "Insert into c_box_type(box_type_cid, project_cid, external_name, "
        "status, box_capacity) "
        "values ( :bid, :pid, :name, :status, :cap )");
    FormatSQL( query );

    try
    {
        query -> ExecSQL();
    }
    catch(Exception& e)
    {
        if(e.Message != "Error creating cursor handle")
        {
            AnsiString strError =
                "Error inserting new box.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "BoxTypeClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void BoxTypeClass::UpdateToDatabase(TQuery* query)
{
    // write updated record
    // only allow update of external_name
    query -> SQL -> Clear();
    query->SQL->Add(
        "Update c_box_type set "
        "external_name = :name "
        " where box_type_cid = :bid "
        "   and project_cid = :pid ");
    query -> ParamByName( "bid" )->AsInteger = BoxTypeID;
    query -> ParamByName( "pid" )->AsInteger = ProjectID;
    query -> ParamByName( "name" )->AsString = BoxName;
    LogRecord( "Update: New record: " );

    query -> Prepare();
    try
    {
        query -> ExecSQL();
    }
    catch(Exception& e)
    {
        if(e.Message != "Error creating cursor handle")
        {
            AnsiString strError =
                "Error updating box.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "BoxTypeClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}


void BoxTypeClass::FormatSQL( TQuery* query )
{
    query -> ParamByName( "bid" )->AsInteger = BoxTypeID;
    query -> ParamByName( "pid" )->AsInteger = ProjectID;
    query -> ParamByName( "name" )->AsString = BoxName;
    query -> ParamByName( "status" )->AsSmallInt = Status;
    query -> ParamByName( "cap" )->AsSmallInt = BoxCapacity;
}

void BoxTypeClass::DeleteFromDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query -> SQL -> Add(
            "DELETE FROM c_box_type "
            " WHERE box_type_cid = :bid "
            " AND   project_cid = :pid");
    query -> ParamByName( "bid" )->AsInteger = BoxTypeID;
    query -> ParamByName( "pid" )->AsInteger = ProjectID;

    query -> Prepare();
    try
    {
        query -> ExecSQL();
    }
    catch(Exception& e)
    {
        if(e.Message != "Error creating cursor handle")
        {
            AnsiString strError =
                "Error deleting box type.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "BoxTypeClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

AliquotTypeClass* BoxTypeClass::GetAliquotType( int index )
{
    if(index < numAliquotTypes)
        return theAliquotTypes[index];
    else
        return NULL;
}

AliquotTypeClass* BoxTypeClass::GetAliquotType( AnsiString AliquotTypeName )
{
    int index = 0;

    while(index < numAliquotTypes && AliquotTypeName !=
        theAliquotTypes[index] -> GetName())
    {
        index++;
    }

    if(index < numAliquotTypes)
        return theAliquotTypes[index];
    else
        return NULL;
}


