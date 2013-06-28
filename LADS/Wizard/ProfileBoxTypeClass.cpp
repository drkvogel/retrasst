//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ProfileBoxTypeClass.h"
#include "LogMod1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
ProfileBoxTypeClass::ProfileBoxTypeClass()
:theName("default construction")
{
    ProfileID = 0;
    BoxTypeID = 0;
    BoxOrder = 0;
}

/*@*/
ProfileBoxTypeClass::ProfileBoxTypeClass(AnsiString aName)
:theName(aName)
{
    ProfileID = 0;
    BoxTypeID = 0;
    BoxOrder = 0;
}

ProfileBoxTypeClass::ProfileBoxTypeClass(ProfileBoxTypeClass& aBoxType)

{
    theName = aBoxType.theName;
    ProfileID = aBoxType.ProfileID;
    BoxTypeID = aBoxType.BoxTypeID;
    BoxOrder = aBoxType.BoxOrder;
}

// default destructor - will eventually implement clean up code
 __fastcall ProfileBoxTypeClass::~ProfileBoxTypeClass()
{
}

void ProfileBoxTypeClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " Aliquot_profile: ";
    LogText += "box_type_cid=" + IntToStr( BoxTypeID ) + ",";
    LogText += "box_order=" + IntToStr( BoxOrder ) + ",";
    LogText += "profile_id=" + IntToStr( ProfileID ) + ",";
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, LogText );
}

void ProfileBoxTypeClass::InsertToDatabase(TQuery* query)
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
                "ProfileBoxTypeClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void ProfileBoxTypeClass::UpdateToDatabase(TQuery* query)
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
                "Error inserting aliquot_profile into database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "TPTestClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}


void ProfileBoxTypeClass::DeleteFromDatabase(TQuery* query)
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
                "ProfileBoxTypeClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void ProfileBoxTypeClass::FormatSQL( TQuery* query )
{
    //  common SQL code for new and updated record
    query->ParamByName("boxid")->AsInteger = BoxTypeID;
    query->ParamByName("boxord")->AsInteger = (int)BoxOrder;
    query->ParamByName("pid")->AsInteger = ProfileID;
}



