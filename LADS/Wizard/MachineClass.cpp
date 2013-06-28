#include <vcl.h>
#include <dbtables.hpp>
#include <stdlib.h>
#include "MachineClass.h"
#include "TestClass.h"

// use MachineClass to provide a named container for instances of TestClass

#include "LogMod1.h"
#include "DateMod1.h"


/*@*/
MachineClass::MachineClass()
: MachineName("")
{
    MachineID = 0;
    Description = "";
    ValidFrom = DateMod->GetDefaultDate();
    ValidTo = DateMod->GetDefaultDate();
    ClusterName = "";
    ClusterID = 0;
    Status = 0;
    Location = "";
    NoteExists = 0;
    numTests = 0;
}

MachineClass::MachineClass(AnsiString aName)
: MachineName(aName)
{
    MachineID = 0;
    Description = "";
    ValidFrom = DateMod->GetDefaultDate();
    ValidTo = DateMod->GetDefaultDate();
    ClusterName = "";
    ClusterID = 0;
    Status = 0;
    Location = "";
    NoteExists = 0;
    numTests = 0;
}


__fastcall MachineClass::~MachineClass()
{
    DeleteTestClass();
}


void MachineClass::LoadFromDatabase(TQuery* query, int recno)
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
            MachineID = query -> FieldByName("machine_cid") -> AsInteger;
            MachineName = query -> FieldByName("external_name") -> AsString;
            Description = query -> FieldByName("external_full") -> AsString;
            ValidFrom = query -> FieldByName("valid_from") -> AsString;
            ValidTo = query -> FieldByName("valid_to") -> AsString;
            Status = (short)query -> FieldByName("status") -> AsInteger;
            Location = query -> FieldByName("location") -> AsString;
            ClusterID = query -> FieldByName("cluster_cid") -> AsInteger;
            ClusterName = query -> FieldByName("cluster_name") -> AsString;
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading Machine from database.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "MachineClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
}

void MachineClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " c_machine: (Part 1) ";
    LogText += "machine_cid=" + IntToStr( MachineID ) + ",";
    LogText += "external_name=" + MachineName + ",";
    LogText += "external_full=" + Description + ",";
    LogText += "valid_from=" + ValidFrom + ",";
    LogText += "valid_to=" + ValidTo + ",";
    LogText += "status=" + IntToStr( Status ) + ",";
    LogText += "location=" + Location + ",";
    LogText += "cluster_cid=" + IntToStr( ClusterID ) + ",";
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, LogText );
}

void MachineClass::InsertToDatabase(TQuery* query)
{
//  write new record with NEW status
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add(
        "Insert into c_machine(machine_cid, external_name, external_full,"
        "valid_from, valid_to, status, location, cluster_cid) "
        "values ( :cid, :name, :full, :from, :to, :status, :loc, :clid)");
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
                "Error inserting new machine.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "MachineClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void MachineClass::UpdateToDatabase(TQuery* query)
{
    // write updated record
    query -> SQL -> Clear();
    query->SQL->Add(
        "Update c_machine set "
        "external_name = :name, "
        "external_full = :full, "
        "valid_from = :from, "
        "valid_to = :to, "
        "status = :status, "
        "location = :loc, "
        "cluster_cid = :clid "
        " where machine_cid = :cid ");
    query -> ParamByName( "cid" )->AsInteger = MachineID;
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
                "Error updating machine.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "MachineClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}


void MachineClass::FormatSQL( TQuery* query )
{
    query -> ParamByName( "cid" )->AsInteger = MachineID;
    query -> ParamByName( "name" )->AsString = MachineName;
    query -> ParamByName( "full" )->AsString = Description;
    DateMod->PutDateToDatabase( query, "from", ValidFrom );
    DateMod->PutDateToDatabase( query, "to", ValidTo );
    query -> ParamByName( "status" )->AsSmallInt = Status;
    query -> ParamByName( "loc" )->AsString = Location;
    query -> ParamByName( "clid" )->AsInteger = ClusterID;
}


void MachineClass::LoadTestsFromDatabase(TQuery* query)
{
    DeleteTestClass();
    numTests = query->RecordCount;
    theTests = new TestClass*[numTests];
    query -> First();

    for (int x = 0; x < numTests; x++)
    {
        try
        {
            theTests[x] = new TestClass;
            theTests[x]->SetTestID(
                query -> FieldByName("test_cid") -> AsInteger );
            theTests[x]->SetName(
                query -> FieldByName("external_name") -> AsString );
        }
        catch(Exception& e)
        {
            Application -> MessageBox
                ("Error loading Test from database",
                "MachineClass Error",MB_OK);
            Application -> MessageBox
                (e.Message.c_str(),"MachineClass Error",MB_OK);
        }
        query -> Next();
    }
    query->Close();
}

TestClass* MachineClass::SearchForTest( AnsiString TestName )
{
    int index = 0;

    while(index < numTests && TestName != theTests[index] -> GetName())
    {
        index++;
    }

    if(index < numTests)
        return theTests[index];
    else
        return NULL;
}


void MachineClass::DeleteTestClass()
{
    for (int index=0; index < numTests; index++)
    {
        delete theTests[index];
    }
    numTests = 0;
}

