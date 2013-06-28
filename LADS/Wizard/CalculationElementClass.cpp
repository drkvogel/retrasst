//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "CalculationElementClass.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

#include <vcl.h>
#include <dbtables.hpp>
#include <stdlib.h>
#include "LogMod1.h"
#include "CalculationElementClass.h"
#include "MachineClass.h"
#include "DateMod1.h"


CalculationElementClass::CalculationElementClass()
{
    ResultTestID = 0;
    SourceTestID = 0;
    Calculation = (short)0;
    TestPosition = (short)0;
}


__fastcall CalculationElementClass::~CalculationElementClass()
{
}

void CalculationElementClass::LoadFromDatabase(TQuery* query, int recno)
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

            ResultTestID = query -> FieldByName("result_test_cid") -> AsInteger;
            SourceTestID = query -> FieldByName("source_test_cid") -> AsInteger;
            Calculation = (short)query -> FieldByName("calculation") -> AsInteger;
            TestPosition = (short)query -> FieldByName("test_position") -> AsInteger;
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading CalculationElement from database.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "CalculationElementClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
}


void CalculationElementClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " c_test_defnintion: ";
    LogText += "result_test_cid=" + IntToStr( ResultTestID ) + ",";
    LogText += "source_test_cid=" + IntToStr( SourceTestID ) + ",";
    LogText += "calculation=" + IntToStr( Calculation ) + ",";
    LogText += "test_position=" + IntToStr( TestPosition );
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, LogText );
}

void CalculationElementClass::InsertToDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add("Insert into c_test_definition "
        "(result_test_cid, source_test_cid, calculation, test_position) "
        "values ("
        ":rid, :sid, :calc, :pos) ");
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
                "CalculationElementClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void CalculationElementClass::UpdateToDatabase(TQuery* query)
{
    // write updated record
    query -> SQL -> Clear();
    query->SQL->Add("Update c_test_definition set "
        "source_test_cid = :sid, "
        "calculation = :calc "
        " where result_test_cid = :rid "
        "   and test_position = :pos ");
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
                "CalculationElementClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}


void CalculationElementClass::DeleteFromDatabase(TQuery* query)
{
    // write to audit log
    LogRecord( "Deleting: " );

    // delete record
    query -> Close();
    query -> SQL -> Clear();
    query -> SQL -> Add("delete from c_test_definition"
            " where result_test_cid = :rid ");
    query -> ParamByName( "tid" )->AsInteger = ResultTestID;

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
                "CalculationElementClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void CalculationElementClass::FormatSQL( TQuery* query)
{
    query -> ParamByName( "rid" )->AsInteger = ResultTestID;
    query -> ParamByName( "sid" )->AsInteger = SourceTestID;
    query -> ParamByName( "calc" )->AsSmallInt = Calculation;
    query -> ParamByName( "pos" )->AsSmallInt = TestPosition;
}

