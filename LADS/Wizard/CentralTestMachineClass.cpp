
#include <vcl.h>
#include <dbtables.hpp>
#include <stdlib.h>
#include "LogMod1.h"
#include "CentralTestMachineClass.h"
#include "MachineClass.h"
#include "DateMod1.h"


CentralTestMachineClass::CentralTestMachineClass()
{
    TestID = 0;
    MachineID = 0;
    MachineTestName = ""; /* name of test used by this machine */
    Protocol = (short)0;
    TestOrder = (short)0;

    MachineName = "";
}


__fastcall CentralTestMachineClass::~CentralTestMachineClass()
{
}

void CentralTestMachineClass::LoadFromDatabase(TQuery* query, int recno)
{
    if(query -> RecordCount <= recno)
    {
        //error, index out of range
        return;
    }
    else
    {
        try
        {
            query -> First();
            for (int x = 0; x < recno; x++)
            {
                query -> Next();
            }

            TestID = query -> FieldByName("test_cid") -> AsInteger;
            MachineID = query -> FieldByName("machine_cid") -> AsInteger;
            MachineTestName = query -> FieldByName("machine_test_name") -> AsString;
            Protocol = (short)query -> FieldByName("protocol") -> AsInteger;
            TestOrder = (short)query -> FieldByName("test_order") -> AsInteger;
            MachineName = query -> FieldByName("machine_name") -> AsString;
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading CentralTestMachine from database.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "CentralTestMachineClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
}


void CentralTestMachineClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " c_test_machine: ";
    LogText += "test_cid=" + IntToStr( TestID ) + ",";
    LogText += "machine_cid=" + IntToStr( MachineID ) + ",";
    LogText += "machine_test_name=" + MachineTestName + ",";
    LogText += "protocol=" + IntToStr( Protocol ) + ",";
    LogText += "test_order=" + IntToStr( TestOrder );
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, LogText );
}

void CentralTestMachineClass::InsertToDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add("Insert into c_test_machine "
        "(test_cid, machine_cid, machine_test_name, protocol, test_order) "
        "values ("
        ":tid, :mid, :tname, :pro, :to) ");
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
                "Error inserting new CentralTestMachine.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "CentralTestMachineClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void CentralTestMachineClass::UpdateToDatabase(TQuery* query)
{
    // write updated record
    query -> SQL -> Clear();
    query->SQL->Add("Update c_test_machine set "
        "machine_test_name = :tname, "
        "protocol = :pro, "
        "test_order = :to "
        " where test_cid = :tid and "
        "       machine_cid = :mid ");
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
                "Error updating CentralTestMachine.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "CentralTestMachineClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}


void CentralTestMachineClass::DeleteFromDatabase(TQuery* query)
{
    // write to audit log
    LogRecord( "Deleting: " );

    // delete record
    query -> Close();
    query -> SQL -> Clear();
    query -> SQL -> Add("delete from c_test_machine"
            " where test_cid = :tid and "
            "       machine_cid = :mid ");
    query -> ParamByName( "tid" )->AsInteger = TestID;
    query -> ParamByName( "mid" )->AsInteger = MachineID;

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
                "Error deleting CentralTestMachine.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "CentralTestMachineClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void CentralTestMachineClass::FormatSQL( TQuery* query)
{
    query -> ParamByName( "tid" )->AsInteger = TestID;
    query -> ParamByName( "mid" )->AsInteger = MachineID;
    query -> ParamByName( "tname" )->AsString = MachineTestName;
    query -> ParamByName( "pro" )->AsSmallInt = Protocol;
    query -> ParamByName( "to" )->AsSmallInt = TestOrder;
}


