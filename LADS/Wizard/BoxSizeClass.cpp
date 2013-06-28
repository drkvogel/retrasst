//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BoxSizeClass.h"
#include "LogMod1.h"
#include "AliquotTypeClass.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

BoxSizeClass::BoxSizeClass()
{
    BoxSizeID = 0;
    Description = "";
    BoxCapacity = 0;
    EmptyPosition = 0;
}


__fastcall BoxSizeClass::~BoxSizeClass()
{
}

void BoxSizeClass::LoadFromDatabase(TQuery* query, int recno)
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
            BoxSizeID = query -> FieldByName("box_size_cid") -> AsInteger;
            Description = query -> FieldByName("description") -> AsString;
            BoxCapacity = (short)query -> FieldByName("box_capacity") -> AsInteger;
            EmptyPosition = (short)query -> FieldByName("empty_position") -> AsInteger;
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading Box Size from database.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "BoxSizeClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
}

void BoxSizeClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " c_box_type: ";
    LogText += "box_size_cid=" + IntToStr( BoxSizeID ) + ",";
    LogText += "description=" + Description + ",";
    LogText += "box_capacity=" + IntToStr( BoxCapacity ) + ",";
    LogText += "empty_position=" + IntToStr( EmptyPosition );
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, LogText );
}

void BoxSizeClass::InsertToDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add(
        "Insert into c_box_size(box_size_cid, description, "
        "box_capacity, empty_position) "
        "values ( :bsid, :desc, :cap, :epos )");
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
                "BoxSizeClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void BoxSizeClass::UpdateToDatabase(TQuery* query)
{
    // write updated record
    // only allow update of external_name
    query -> SQL -> Clear();
    query->SQL->Add(
        "Update c_box_size set "
        "description = :desc "
        " where box_size_cid = :bsid " );
    query -> ParamByName( "bsid" )->AsInteger = BoxSizeID;
    query -> ParamByName( "desc" )->AsString = Description;
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
                "BoxSizeClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}


void BoxSizeClass::FormatSQL( TQuery* query )
{
    query -> ParamByName( "bsid" )->AsInteger = BoxSizeID;
    query -> ParamByName( "desc" )->AsString = Description;
    query -> ParamByName( "cap" )->AsSmallInt = BoxCapacity;
    query -> ParamByName( "epos" )->AsSmallInt = EmptyPosition;
}

void BoxSizeClass::DeleteFromDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query -> SQL -> Add(
            "DELETE FROM c_box_size "
            " WHERE box_size_cid = :bsid ");
    query -> ParamByName( "bsid" )->AsInteger = BoxSizeID;

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
                "BoxSizeClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}


