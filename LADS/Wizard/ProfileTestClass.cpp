#include <vcl.h>
#include <stdlib.h>
#include "LogMod1.h"
#include "ProfileTestClass.h"
#include "ProjectClass.h"
#include "DateMod1.h"
const int ProfileTestClass::IGNORE_LOWER_LIMIT_BIT = 1;
const int ProfileTestClass::PRIVATE_BIT = 2;
const int ProfileTestClass::IGNORE_UPPER_LIMIT_BIT = 4;

ProfileTestClass::ProfileTestClass()
:theName("default construction")
{
    TestID = 0;
    MachineName = "unknown";
    MachineID = 0;
    ProfileID = 0;
    UpperLimit = "0";
    LowerLimit = "0";
    TestFlag = 0;
    TriggerID = 0;
    TriggerName = "unknown";
    TriggerUpperLimit = "0";
    TriggerLowerLimit = "0";
}

/*@*/
ProfileTestClass::ProfileTestClass(AnsiString aName)
:theName(aName)
{
    TestID = 0;
    MachineName = "unknown";
    MachineID = 0;
    ProfileID = 0;
    UpperLimit = "0";
    LowerLimit = "0";
    TestFlag = 0;
    TriggerID = 0;
    TriggerName = "unknown";
    TriggerUpperLimit = "0";
    TriggerLowerLimit = "0";
}

ProfileTestClass::ProfileTestClass(ProfileTestClass& aProfileTest)
{
    theName = aProfileTest.theName;
    TestID = aProfileTest.TestID;
    MachineName = aProfileTest.MachineName;
    MachineID = aProfileTest.MachineID;
    ProfileID = aProfileTest.ProfileID;
    UpperLimit = aProfileTest.UpperLimit;
    LowerLimit = aProfileTest.LowerLimit;
    TestFlag = aProfileTest.TestFlag;
    TriggerID = aProfileTest.TriggerID;
    TriggerName = aProfileTest.TriggerName;
    TriggerUpperLimit = aProfileTest.TriggerUpperLimit;
    TriggerLowerLimit = aProfileTest.TriggerLowerLimit;
}

// default destructor - will eventually implement clean up code
 __fastcall ProfileTestClass::~ProfileTestClass()
{
}

void ProfileTestClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " test_profile: (Part 1) ";
    LogText += "test_cid=" + IntToStr( TestID ) + ",";
    LogText += "machine_cid=" + IntToStr( MachineID ) + ",";
    LogText += "test_profile_id=" + IntToStr( ProfileID ) + ",";
    LogText += "upper_test_limit=" + UpperLimit + ",";
    LogText += "lower_test_limi=" + LowerLimit + ",";
    if ( theProject->isBeforeVersion( 2, 1 ) )
    {
        LogText += "no_test_limits=" + IntToStr( TestFlag );
    }
    else
    {
        LogText += "test_flags=" + IntToStr( TestFlag );
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, LogText );

    LogText = Text + " test_profile: (Part 2) ";
    LogText += "trigger_id=" + IntToStr( TriggerID ) + ",";
    LogText += "upper_trigger_limit=" + TriggerUpperLimit + ",";
    LogText += "lower_trigger_limit=" + TriggerLowerLimit + ",";
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, LogText );
}

void ProfileTestClass::InsertToDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add(
        "Insert into test_profile(test_cid, machine_cid, "
        "test_profile_id, upper_test_limit, lower_test_limit, " );
    if ( theProject->isBeforeVersion( 2, 1 ) )
    {
    query->SQL->Add(
                "no_test_limits, " );
    }
    else
    {
    query->SQL->Add(
                " test_flags, " );
    }
    query->SQL->Add(
        "trigger_id, upper_trigger_limit, lower_trigger_limit) "
        "values (:id, :mid, :tpid, :uplim, :lowlim, :flag, "
        ":ttf, :tuplim, :tlowlim) ");

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
                "Error inserting test profile test into database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ProfileTestClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void ProfileTestClass::UpdateToDatabase(TQuery* query)
{
    // write updated record
    query -> SQL -> Clear();
    query->SQL->Add("Update test_profile set "
        "machine_cid = :mid, "
        "upper_test_limit = :uplim, "
        "lower_test_limit = :lowlim, " );
    if ( theProject->isBeforeVersion( 2, 1 ) )
    {
        query->SQL->Add( "no_test_limits = :flag, " );
    }
    else
    {
        query->SQL->Add( "test_flags = :flag, " );
    }
    query->SQL->Add(
        "trigger_id = :ttf, "
        "upper_trigger_limit = :tuplim, "
        "lower_trigger_limit = :tlowlim "
        "where test_cid = :id and "
        "      test_profile_id = :tpid ");

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
                "Error inserting test profile test into database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ProfileTestClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}


void ProfileTestClass::DeleteFromDatabase(TQuery* query)
{
    LogRecord( "Deleting: " );

    // delete records
    query -> Close();
    query -> SQL -> Clear();
    query -> SQL -> Add(
        "delete from test_profile "
        "where test_cid = :id and "
        "      test_profile_id = :tpid ");
    query->ParamByName("id")->AsInteger = TestID;
    query->ParamByName("tpid")->AsInteger = ProfileID;

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
                "Error updating test profile test\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ProfileTestClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void ProfileTestClass::FormatSQL( TQuery* query )
{
    //  common SQL code for new and updated record
    query->ParamByName("id")->AsInteger = TestID;
    query->ParamByName("mid")->AsInteger = MachineID;
    query->ParamByName("tpid")->AsInteger = ProfileID;
    query->ParamByName("uplim")->AsString = UpperLimit;
    query->ParamByName("lowlim")->AsString = LowerLimit;
    query->ParamByName("ttf")->AsInteger = TriggerID;
    query->ParamByName("tuplim")->AsString = TriggerUpperLimit;
    query->ParamByName("tlowlim")->AsString = TriggerLowerLimit;
    if ( theProject->isBeforeVersion( 2, 1 ) )
    {
        query->ParamByName("flag")->AsString = IntToStr(TestFlag);
    }
    else
    {
        query->ParamByName("flag")->AsInteger = TestFlag;
    }
}




