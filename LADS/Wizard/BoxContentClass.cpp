//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "BoxContentClass.h"
#include "LogMod1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
BoxContentClass::BoxContentClass()
:theName("default construction")
{
    BoxTypeID = 0;
    AnsiString Description = "";
    Status = 0;
    BoxSizeID = 0;
    AliquotTypeID1 = 0;
    AliquotTypeID2 = 0;
    AliquotTypeID3 = 0;
}

/*@*/
BoxContentClass::BoxContentClass(AnsiString aName)
:theName(aName)
{
    BoxTypeID = 0;
    AnsiString Description = "";
    Status = 0;
    BoxSizeID = 0;
    AliquotTypeID1 = 0;
    AliquotTypeID2 = 0;
    AliquotTypeID3 = 0;
}

 __fastcall BoxContentClass::~BoxContentClass()
{
}

void BoxContentClass::LoadFromDatabase(TQuery* query, int recno)
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
            theName = query -> FieldByName("external_name") -> AsString;
            Description = query -> FieldByName("description") -> AsString;
            BoxSizeID = query -> FieldByName("box_size_cid") -> AsInteger;
            Status = (short)query -> FieldByName("status") -> AsInteger;
            AliquotTypeID1 = query -> FieldByName("aliquot_type1") -> AsInteger;
            AliquotTypeID2 = query -> FieldByName("aliquot_type2") -> AsInteger;
            AliquotTypeID3 = query -> FieldByName("aliquot_type3") -> AsInteger;
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading Box from database.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "BoxContentClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
}

void BoxContentClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " box_content: ";
    LogText += "box_type_cid=" + IntToStr( BoxTypeID ) + ",";
    LogText += "external_name=" + theName + ",";
    LogText += "description=" + Description + ",";
    LogText += "status=" + IntToStr( Status ) + ",";
    LogText += "box_size_cid=" + IntToStr( BoxSizeID ) + ",";
    LogText += "aliquot_type1=" + IntToStr( AliquotTypeID1 ) + ",";
    LogText += "aliquot_type2=" + IntToStr( AliquotTypeID2 ) + ",";
    LogText += "aliquot_type3=" + IntToStr( AliquotTypeID3 );
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, LogText );
}

void BoxContentClass::InsertToDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add(
        "Insert into box_content(box_type_cid,external_name,description, "
                "status,box_size_cid,aliquot_type1,aliquot_type2,aliquot_type3) "
                "values (:btid, :nam, :desc, :stat, :bsid, :at1, :at2, :at3) ");
    FormatSQL( query );

    try
    {
        query -> ExecSQL();
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
                "Error inserting box content into database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "BoxContentClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void BoxContentClass::UpdateToDatabase(TQuery* query)
{
// ???? need to be able to update aliquot types only ????

    // write updated record
    query -> SQL -> Clear();
    query->SQL->Add("Update box_content set "
        "external_name = :nam, "
        "description = :desc, "
        "status = :stat, "
        "box_size_cid = :bsid, "
        "aliquot_type1 = :at1, "
        "aliquot_type2 = :at2, "
        "aliquot_type3 = :at3 "
        "where box_type_cid = :btid "  );
    FormatSQL( query );
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
                "Error updating box content into database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "BoxContentClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}


void BoxContentClass::DeleteFromDatabase(TQuery* query)
{
    LogRecord( "Deleting: " );

    // delete records
    query -> Close();
    query -> SQL -> Clear();
    query -> SQL -> Add(
        "delete from box_content "
        "where box_type_cid = :btid ");
    query->ParamByName("btid")->AsInteger = BoxTypeID;

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
                "Error deleting box content\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "BoxContentClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void BoxContentClass::FormatSQL( TQuery* query )
{
    //  common SQL code for new and updated record
    query->ParamByName("btid")->AsInteger = BoxTypeID;
    query->ParamByName("nam")->AsString = theName;
    query->ParamByName("desc")->AsString = Description;
    query->ParamByName("stat")->AsInteger = Status;
    query->ParamByName("bsid")->AsInteger = BoxSizeID;
    query->ParamByName("at1")->AsInteger = AliquotTypeID1;
    query->ParamByName("at2")->AsInteger = AliquotTypeID2;
    query->ParamByName("at3")->AsInteger = AliquotTypeID3;
}

