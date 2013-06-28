//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AliquotTypeClass.h"
#include "LogMod1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
AliquotTypeClass::AliquotTypeClass()
:theName("default construction")
{
    BoxTypeID = 0;
    AliquotTypeID = 0;
    AliquotOrder = 0;
}

/*@*/
AliquotTypeClass::AliquotTypeClass(AnsiString aName)
:theName(aName)
{
    BoxTypeID = 0;
    AliquotTypeID = 0;
    AliquotOrder = 0;
}

AliquotTypeClass::AliquotTypeClass(AliquotTypeClass& aAliquotType)

{
    theName = aAliquotType.theName;
    BoxTypeID = aAliquotType.BoxTypeID;
    AliquotTypeID = aAliquotType.AliquotTypeID;
    AliquotOrder = aAliquotType.AliquotOrder;
}

// default destructor - will eventually implement clean up code
 __fastcall AliquotTypeClass::~AliquotTypeClass()
{
}

void AliquotTypeClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " c_box_content: ";
    LogText += "aliquot_type_cid=" + IntToStr( AliquotTypeID ) + ",";
    LogText += "aliquot_order=" + IntToStr( AliquotOrder ) + ",";
    LogText += "box_type_cid=" + IntToStr( BoxTypeID ) + ",";
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, LogText );
}

void AliquotTypeClass::InsertToDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add(
        "Insert into c_box_content(aliquot_type_cid,aliquot_order,box_type_cid) "
                "values (:aid, :ord, :bid) ");
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
                "Error inserting aliquot type into database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "AliquotTypeClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void AliquotTypeClass::UpdateToDatabase(TQuery* query)
{
    // write updated record
    query -> SQL -> Clear();
    query->SQL->Add("Update c_box_content set "
        "aliquot_order = :ord "
        "where box_type_cid = :bid and "
        "      aliquot_type_cid = :aid ");
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
                "Error updating aliquot type into database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "AliquotTypeClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}


void AliquotTypeClass::DeleteFromDatabase(TQuery* query)
{
    LogRecord( "Deleting: " );

    // delete records
    query -> Close();
    query -> SQL -> Clear();
    query -> SQL -> Add(
        "delete from c_box_content "
        "where box_type_cid = :bid and "
        "      aliquot_type_cid = :aid ");
    query->ParamByName("aid")->AsInteger = AliquotTypeID;
    query->ParamByName("bid")->AsInteger = BoxTypeID;

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
                "Error deleting aliquot type\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "AliquotTypeClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void AliquotTypeClass::FormatSQL( TQuery* query )
{
    //  common SQL code for new and updated record
    query->ParamByName("aid")->AsInteger = AliquotTypeID;
    query->ParamByName("ord")->AsInteger = (int)AliquotOrder;
    query->ParamByName("bid")->AsInteger = BoxTypeID;
}


