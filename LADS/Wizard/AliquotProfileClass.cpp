//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AliquotProfileClass.h"
#include "LogMod1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
AliquotProfileClass::AliquotProfileClass()
:theName("default construction")
{
    ProfileID = 0;
    BoxTypeID = 0;
    BoxOrder = 0;
}

/*@*/
AliquotProfileClass::AliquotProfileClass(AnsiString aName)
:theName(aName)
{
    ProfileID = 0;
    BoxTypeID = 0;
    BoxOrder = 0;
}

AliquotProfileClass::AliquotProfileClass(AliquotProfileClass& aAliquotProfile)

{
    theName = aAliquotProfile.theName;
    ProfileID = aAliquotProfile.ProfileID;
    BoxTypeID = aAliquotProfile.BoxTypeID;
    BoxOrder = aAliquotProfile.BoxOrder;
}

// default destructor - will eventually implement clean up code
 __fastcall AliquotProfileClass::~AliquotProfileClass()
{
}

void AliquotProfileClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " Aliquot_profile: ";
    LogText += "box_type_cid=" + IntToStr( BoxTypeID ) + ",";
    LogText += "box_order=" + IntToStr( BoxOrder ) + ",";
    LogText += "profile_id=" + IntToStr( ProfileID ) + ",";
    LogMod->LogProjectDb( LOGLOCAL, LogText );
}

void AliquotProfileClass::InsertToDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add(
        "Insert into aliquot_profile(box_type_cid,box_order,profile_id) "
                "values (:boxid, :boxord, :pid) ");
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
                "Error inserting aliquot profile into database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "AliquotProfileClass Error",MB_OK);
            LogMod->LogProjectDb( LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( LOGLOCAL, query );
    LogMod->Commit( query );
}

void AliquotProfileClass::UpdateToDatabase(TQuery* query)
{
    // write updated record
    query -> SQL -> Clear();
    query->SQL->Add("Update aliquot_profile set "
        "box_order = :boxord "
        "where box_type_cid = :boxid and "
        "      profile_id = :pid ");
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
                "Error inserting test profile aliquot into database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "TPTestClass Error",MB_OK);
            LogMod->LogProjectDb( LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( LOGLOCAL, query );
    LogMod->Commit( query );
}


void AliquotProfileClass::DeleteFromDatabase(TQuery* query)
{
    LogRecord( "Deleting: " );

    // delete records
    query -> Close();
    query -> SQL -> Clear();
    query -> SQL -> Add(
        "delete from aliquot_profile "
        "where box_type_cid = :boxid and "
        "      profile_id = :pid ");
    query->ParamByName("boxid")->AsInteger = BoxTypeID;
    query->ParamByName("pid")->AsInteger = ProfileID;

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
                "Error updating aliquot profile\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "AliquotProfileClass Error",MB_OK);
            LogMod->LogProjectDb( LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( LOGLOCAL, query );
    LogMod->Commit( query );
}

void AliquotProfileClass::FormatSQL( TQuery* query )
{
    //  common SQL code for new and updated record
    query->ParamByName("boxid")->AsInteger = BoxTypeID;
    query->ParamByName("boxord")->AsInteger = (int)BoxOrder;
    query->ParamByName("pid")->AsInteger = ProfileID;
}



