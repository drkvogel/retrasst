#include <vcl.h>
#include <dbtables.hpp>
#include <stdlib.h>
#include "LogMod1.h"
#include "TestClass.h"
#include "DateMod1.h"
#include "CalculationElementClass.h"

/*@*/
const int TestClass::T_NUM_DATATYPES = 4;
const short TestClass::T_DATATYPENUM[] = {1,2,4,8};
const AnsiString TestClass::T_DATATYPENAME[] =
    { "other (bit encoded)", "integer", "real", "text" };

TestClass::TestClass()
: theName("")
{
    TestID = 0;
    Description = "";
    ValidFrom = DateMod->GetDefaultDate();
    ValidTo = DateMod->GetDefaultDate();
    Status = 0;
    DataTypeIndex = 0;
    MinDataPoints = 0;
    MaxDataPoints = 0;
    DefaultLowerLimit = "";
    DefaultUpperLimit = "";
    SampleType = "";

    numCalcElements = -1;
}

TestClass::TestClass(AnsiString aName)
: theName(aName)
{

    TestID = 0;
    Description = "";
    ValidFrom = DateMod->GetDefaultDate();
    ValidTo = DateMod->GetDefaultDate();
    Status = 0;
    DataTypeIndex = 0;
    MinDataPoints = 0;
    MaxDataPoints = 0;
    DefaultLowerLimit = "";
    DefaultUpperLimit = "";
    SampleType = "";

    numCalcElements = -1;
}

__fastcall TestClass::~TestClass()
{
}

void TestClass::LoadFromDatabase(TQuery* query, TQuery* query1, int recno)
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
        TestID = query -> FieldByName("test_cid") -> AsInteger;
        theName = query -> FieldByName("external_name") -> AsString;
        Description = query -> FieldByName("external_full") -> AsString;
        ValidFrom = query -> FieldByName("valid_from") -> AsString;
        ValidTo = query -> FieldByName("valid_to") -> AsString;
        Status = (short)query -> FieldByName("status") -> AsInteger;
        short dt = (short)query -> FieldByName("data_type") -> AsInteger;
        SetDataTypeIndexByType( dt ); // sets DataTypeIndex associated with DataType value
        MinDataPoints = query -> FieldByName("min_datapoints") -> AsInteger;
        MaxDataPoints = query -> FieldByName("max_datapoints") -> AsInteger;
        DefaultLowerLimit = query -> FieldByName("default_lower_limit") -> AsString;
        DefaultUpperLimit = query -> FieldByName("default_upper_limit") -> AsString;
        SampleType = query -> FieldByName("sample_type") -> AsString;
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading test from database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "TestClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }

    // find if this test has calculation elements (entries in c_test_definition)
    query1->SQL->Clear();
    query1->SQL->Text =
        "select * from c_test_definition "
        "where result_test_cid = :tid ";
    query1->ParamByName( "tid" )->AsInteger = TestID;
    query1->Open();
    numCalcElements = query1->RecordCount;
    LogMod->Commit( query1 );
}

void TestClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " c_test: (Part 1) ";
    LogText += "test_cid=" + IntToStr( TestID ) + ",";
    LogText += "external_name=" + theName + ",";
    LogText += "external_full=" + Description + ",";
    LogText += "valid_from=" + ValidFrom + ",";
    LogText += "valid_to=" + ValidTo + ",";
    LogText += "status=" + IntToStr( Status ) + ",";
    LogText += "data_type=" + IntToStr( T_DATATYPENUM[DataTypeIndex] ) + ",";
    LogText += "min_datapoints=" + IntToStr( MinDataPoints ) + ",";
    LogText += "max_datapoints=" + IntToStr( MaxDataPoints ) + ",";
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, LogText );

    LogText = Text + " c_test: (Part 2) ";
    LogText += "default_lower_limit=" + DefaultLowerLimit + ",";
    LogText += "default_upper_limit=" + DefaultUpperLimit + ",";
    LogText += "sample_type=" + SampleType + ",";
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, LogText );
}

void TestClass::InsertToDatabase(TQuery* query)
{
    // write new record with NEW status
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add(
        "Insert into c_test(test_cid, external_name, external_full,"
        "valid_from, valid_to, status, data_type,"
        "min_datapoints, max_datapoints, "
        "default_lower_limit, default_upper_limit, sample_type)"
        "values ( :test, :name, :full, :from, :to, :status, :type, "
        ":min, :max, :low, :up, :stype)");
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
                "Error inserting new test\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "TestClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void TestClass::UpdateToDatabase(TQuery* query)
{
    // write updated record
    query -> SQL -> Clear();
    query->SQL->Add(
        "Update c_test set "
        "external_name = :name, "
        "external_full = :full, "
        "valid_from = :from, "
        "valid_to = :to, "
        "status = :status, "
        "data_type = :type, "
        "min_datapoints = :min, "
        "max_datapoints = :max, "
        "default_lower_limit = :low, "
        "default_upper_limit = :up, "
        "sample_type = :stype "
        " where test_cid = :test ");
    FormatSQL( query );
    LogRecord( "Update: New record: " );

        query -> Prepare();
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
                    "Error updating test\n" + e.Message;
                Application->MessageBox(
                    strError.c_str(),
                    "TestClass Error",MB_OK);
                LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
            }
        }
        LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
        LogMod->Commit( query );
}


void TestClass::FormatSQL( TQuery* query )
{
    query -> ParamByName( "test" )->AsInteger = TestID;
    query -> ParamByName( "name" )->AsString = theName;
    query -> ParamByName( "full" )->AsString = Description;
    DateMod->PutDateToDatabase( query, "from", ValidFrom );
    DateMod->PutDateToDatabase( query, "to", ValidTo );
    query -> ParamByName( "status" )->AsSmallInt = Status;
    query -> ParamByName( "type" )->AsSmallInt = T_DATATYPENUM[DataTypeIndex];
    query -> ParamByName( "min" )->AsInteger = MinDataPoints;
    query -> ParamByName( "max" )->AsInteger = MaxDataPoints;
    query -> ParamByName( "low" )->AsString = DefaultLowerLimit;
    query -> ParamByName( "up" )->AsString = DefaultUpperLimit;

    query -> ParamByName( "stype" )->AsString = SampleType;
}

void TestClass::SetDataTypeIndexByType(short type)
{
    for ( int index=0; index<T_NUM_DATATYPES; index++ )
    {
        if ( type == T_DATATYPENUM[index] )
        {
            DataTypeIndex = index;
        }
    }
}

void TestClass::SetDataTypeIndexByIndex(int index)
{
    DataTypeIndex = index;
}

bool TestClass::HasCalcElements()
{
    if ( numCalcElements > 0 )
    {
        return true;
    }
    else
    {
        return false;
    }
}


