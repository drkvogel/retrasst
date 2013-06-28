#include <vcl.h>
#include <dbtables.hpp>
#include <stdlib.h>
#include "LogMod1.h"
#include "ProfileTestClass.h"
#include "ProfileBoxTypeClass.h"
#include "ProfileClass.h"
#include "TestClass.h"
#include "Machine1.h"
#include "ObjectName1.h"
#include "DateMod1.h"
#include "ProjectClass.h"

/*@*/
// use ProfileClass to provide a named container for instances of
// ProfileTestClass and ProfileBoxTypeClass
const int ProfileClass::SAMPLE_BIT = 1;
const int ProfileClass::WHOLE_TUBE_BIT = 2;

ProfileClass::ProfileClass()
: theName(""),numTests(0),numBoxTypes(0)
{
    ProfileID = 0;
    Description = "";
    DateLastModified = DateMod->GetDefaultDate();
    NoteExists = 0;
    DeleteStatus = 0;
    ProfileFlag = 0;
    theProject = NULL;
}

ProfileClass::ProfileClass(AnsiString aName)
: theName(aName),numTests(0),numBoxTypes(0)
{
    ProfileID = 0;
    Description = "";
    DateLastModified = DateMod->GetDefaultDate();
    NoteExists = 0;
    DeleteStatus = 0;
    ProfileFlag = 0;
    theProject = NULL;
}

__fastcall ProfileClass::~ProfileClass()
{
    DeleteTests();
    DeleteBoxTypes();
}

void ProfileClass::DeleteTests()
{
    // delete any tests
    for (int test=0; test < numTests; test++)
    {
        delete theTests[test];
    }
    delete[] theTests;
    theTests = NULL;
    numTests = -1;
}

void ProfileClass::DeleteBoxTypes()
{
    // delete any box types
    for (int type=0; type < numBoxTypes; type++)
    {
        delete theBoxTypes[type];
    }
    delete[] theBoxTypes;
    theBoxTypes = NULL;
    numBoxTypes = -1;
}


void ProfileClass::LoadFromDatabase(TQuery* query, int recno)
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
            ProfileID = query -> FieldByName("test_profile_id") -> AsInteger;
            theName = query -> FieldByName("test_profile_name") -> AsString;
            Description = query -> FieldByName("test_profile_description") -> AsString;
            DateLastModified =
                query -> FieldByName("date_last_modified") -> AsDateTime;
            NoteExists = (short)query -> FieldByName("note_exists") -> AsInteger;
            if ( !theProject->isBeforeVersion( 2, 1 ) )
            {
                ProfileFlag = query -> FieldByName("profile_flags") -> AsInteger;
            }
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading Test Profile from database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ProfileClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
}

void ProfileClass::LoadTestsFromDatabase(TQuery* query)
{
    numTests = query -> RecordCount;
    theTests = new ProfileTestClass*[numTests];
    for(int x = 0; x < numTests; x++)
    {
        theTests[x] = NULL;
    }
    query -> First();

    for (int x = 0; x < numTests; x++)
    {
        if( query->FieldByName("test_profile_id")->AsInteger == ProfileID )
        {
            theTests[x] = new ProfileTestClass(query->FieldByName("test_name") -> AsString);
            theTests[x]->SetTestID(query->FieldByName("ptest_cid") -> AsInteger);
            theTests[x]->SetProfileID(query->FieldByName("test_profile_id")->
                AsInteger);
            int machineID = query->FieldByName("machine_cid") -> AsInteger;
            theTests[x]->SetMachineID(machineID);
            AnsiString machineName = Machine->GetMachineNameFromID(machineID);
            if ( machineName == "unknown" )
                machineName = ObjectName->GetObjectNameFromID(machineID);
            theTests[x]->SetMachineName(machineName);
            theTests[x]->SetUpperLimit(query->FieldByName("upper_test_limit")->AsString);
            theTests[x]->SetLowerLimit(query->FieldByName("lower_test_limit")->AsString);
            if ( theProject->isBeforeVersion( 2, 1 ) )
            {
                theTests[x]->SetTestFlag(query->FieldByName("no_test_limits")->AsInteger);
            }
            else
            {
                theTests[x]->SetTestFlag(query->FieldByName("test_flags")->AsInteger);
            }
            int triggerID = query->FieldByName("trigger_id") -> AsInteger;
            theTests[x]->SetTriggerID(triggerID);
            theTests[x]->SetTriggerName("unknown");
            theTests[x]->SetTriggerUpperLimit(query->FieldByName("upper_trigger_limit")->AsString);
            theTests[x]->SetTriggerLowerLimit(query->FieldByName("lower_trigger_limit")->AsString);
            theTests[x]->SetProject( theProject );
        }
        query -> Next();
    }
}

void ProfileClass::LoadBoxTypesFromDatabase(TQuery* query)
{
    numBoxTypes = query -> RecordCount;
    theBoxTypes = new ProfileBoxTypeClass*[numBoxTypes];
    for(int x = 0; x < numBoxTypes; x++)
    {
        theBoxTypes[x] = NULL;
    }
    query -> First();

    for (int x = 0; x < numBoxTypes; x++)
    {
        if( query->FieldByName("profile_id")->AsInteger == ProfileID )
        {
            theBoxTypes[x] = new ProfileBoxTypeClass(
                query->FieldByName("external_name")->AsString );
            theBoxTypes[x]->SetBoxTypeID(
                query->FieldByName("box_type_cid")->AsInteger);
            theBoxTypes[x]->SetBoxOrder(
                (short)query->FieldByName("box_order")->AsInteger);
            theBoxTypes[x]->SetProfileID(
                query->FieldByName("profile_id")->AsInteger );
        }
        query -> Next();
    }
}

void ProfileClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " test_profile_description: (Part 1) ";
    LogText += "test_profile_id=" + IntToStr( ProfileID ) + ",";
    LogText += "test_profile_name=" + theName + ",";
    LogText += "test_profile_description=" + Description;
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, LogText );

    LogText = Text + " test_profile_description: (Part 2) ";
    LogText += "date_last_modified=" + DateToStr(DateLastModified) + ",";
    LogText += "note_exists=" + IntToStr(NoteExists) + ",";
    if ( !theProject->isBeforeVersion( 2, 1 ) )
    {
        LogText += "profile_flags=" + IntToStr(ProfileFlag) + ",";
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, LogText );
}

void ProfileClass::InsertToDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    if ( theProject->isBeforeVersion( 2, 1 ) )
    {
        query->SQL->Add(
            "Insert into test_profile_description("
            "test_profile_id, test_profile_name, "
            "test_profile_description, date_last_modified, note_exists )"
            "values (:tpid, :name, :descr, :date, :note) ");
    }
    else
    {
        query->SQL->Add(
            "Insert into test_profile_description("
            "test_profile_id, test_profile_name, "
            "test_profile_description, date_last_modified, note_exists, profile_flags )"
            "values (:tpid, :name, :descr, :date, :note, :flag) ");
    }

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
                "Error inserting test profile into database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ProfileClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void ProfileClass::UpdateToDatabase(TQuery* query)
{
    // write updated record
    query -> SQL -> Clear();
    query->SQL->Add(
        "Update test_profile_description set "
        "test_profile_name = :name, "
        "test_profile_description = :descr, "
        "date_last_modified = :date, ");
    if ( !theProject->isBeforeVersion( 2, 1 ) )
    {
        query->SQL->Add(
            "profile_flags = :flag, ");
    }
    query->SQL->Add(
        "note_exists = :note "
        "where test_profile_id = :tpid");
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
                "Error inserting test profile descriptor\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ProfileClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}


void ProfileClass::DeleteFromDatabase(TQuery* query)
{
    //      update old record to have DELETE status
    query -> Close();
    query -> SQL -> Clear();
    query -> SQL -> Add(
        "DELETE FROM test_profile_description "
        "WHERE test_profile_id = :tpid ");
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
                "Error deleting test profile descriptor\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ProfileClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void ProfileClass::FormatSQL( TQuery* query )
{
    // common SQL code for new and updated record
    query->ParamByName("tpid")->AsInteger = ProfileID;
    query->ParamByName("name")->AsString = theName;
    query->ParamByName("descr")->AsString = Description;
    DateMod->PutDateToDatabase( query, "date", DateToStr(DateMod->GetToday()) );
    query->ParamByName("note")->AsSmallInt = NoteExists;
    if ( !theProject->isBeforeVersion( 2, 1 ) )
    {
        query->ParamByName("flag")->AsInteger = ProfileFlag;
    }
}

// get a pointer to the instance of TPTestClass indexed by index from theTests
// array held by this ProfileClass. This is a pointer to the actual
// instance, so changes may be made.
// This method returns NULL if index is
// greater than the number of codes held by this instance of ProfileClass
ProfileTestClass* ProfileClass::GetTest( int index )
{
    if(index < numTests)
        return theTests[index];
    else
        return NULL;
}

ProfileBoxTypeClass* ProfileClass::GetBoxType( int index )
{
    if(index < numBoxTypes)
        return theBoxTypes[index];
    else
        return NULL;
}

// get a pointer to the instance of TPTestClass from theTests array held by
// this ProfileClass whose name is IDENTICAL to TestName. This is a
// pointer to the actual instance, so changes may be made.
// This method returns
// NULL if none of the TPTestClasses held by this ProfileClass have names
// which are equal ( == ) to TestName

ProfileTestClass* ProfileClass::GetTest( AnsiString TestName )
{
    int idx = -1;

    for (int index=0; index < numTests; index++ )
    {
        AnsiString name = theTests[index] -> GetName();
        if ( TestName == name )
        {
            idx = index;
        }
    }

    if(idx != -1 )
        return theTests[idx];
    else
        return NULL;
}

ProfileBoxTypeClass* ProfileClass::GetBoxType( AnsiString BoxTypeName )
{
    int index = 0;

    while(index < numBoxTypes && BoxTypeName != theBoxTypes[index] -> GetName())
    {
        index++;
    }

    if(index < numBoxTypes)
        return theBoxTypes[index];
    else
        return NULL;
}


// get a pointer to the instance of ProfileTestClass from theTests array held by
// this ProfileClass whose index is IDENTICAL to id. This is a pointer to
// the actual instance, so changes may be made.
// This method returns
// NULL if none of the TPTestClasses held by this ProfileClass have names
// which are equal ( == ) to TestName

ProfileTestClass* ProfileClass::GetTestByID( int id  )
{
    int index = 0;

    while(index < numTests && id != theTests[index] -> GetTestID())
    {
        index++;
    }

    if(index < numTests)
        return theTests[index];
    else
        return NULL;
}



