//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CRTestClass.h"
#include <stdlib.h>
#include "LogMod1.h"
#include "CRTestClass.h"
#include "DateMod1.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

CRTestClass::CRTestClass()
: theName("")
{
    TestID = 0;
    CRID = 0;
}

CRTestClass::CRTestClass( AnsiString aName )
:theName(aName)
{
    TestID = 0;
    CRID = 0;
}

CRTestClass::CRTestClass(CRTestClass& aCRTestClass)
{
    theName = aCRTestClass.theName;
    CRID = aCRTestClass.CRID;
    TestID = aCRTestClass.TestID;
}

 __fastcall CRTestClass::~CRTestClass()
{
}

void CRTestClass::LoadFromDatabase(TQuery* query, int recno)
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
            theName = query -> FieldByName("external_name") -> AsString;
            TestID = query->FieldByName("test_cid") -> AsInteger;
            CRID = query->FieldByName("calibreag_cid")->AsInteger;
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading CRTest from database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "CRTestClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
}

void CRTestClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " c_calibreag_test: ";
    LogText += "test_cid=" + IntToStr( TestID ) + ",";
    LogText += "calibreag_cid=" + IntToStr( CRID ) + ",";
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, LogText );
}

void CRTestClass::InsertToDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add("Insert into c_calibreag_test(test_cid, calibreag_cid) "
                "values (:tcid, :ccid) ");
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
                "Error inserting calibreag test into database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "CRTestClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void CRTestClass::UpdateToDatabase(TQuery* query)
{
    // write updated record
    query -> SQL -> Clear();
    query->SQL->Add("Update c_calibreag_test set "
        "where test_cid = :tcid and "
        "      calibreag_cid = :ccid ");
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
                "Error inserting calibreag test into database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "CRTestClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}


void CRTestClass::DeleteFromDatabase(TQuery* query)
{
    LogRecord( "Deleting: " );

    //  delete record
    query -> Close();
    query -> SQL -> Clear();
    query -> SQL -> Add(
        "delete from c_calibreag_test "
        "where test_cid = :tcid and "
        "      calibreag_cid = :ccid ");
    query->ParamByName("ccid")->AsInteger = CRID;
    query->ParamByName("tcid")->AsInteger = TestID;

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
                "Error deleting calibreag test\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "CRTestClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void CRTestClass::FormatSQL( TQuery* query )
{
    query->ParamByName("tcid")->AsInteger = TestID;
    query->ParamByName("ccid")->AsInteger = CRID;
}



