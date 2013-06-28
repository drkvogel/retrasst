#include <vcl.h>
#include <stdlib.h>
#include "LogMod1.h"
#include "ValueClass.h"
#include "DescriptorClass.h"
#include "DateMod1.h"

ValueClass::ValueClass()
:theName("default construction")
{

    ValueID = 0;
    DescriptorID = 0;
    MinValue = "";
    MaxValue = "";
}

/*@*/
ValueClass::ValueClass(AnsiString aName)
:theName(aName)
{

    ValueID = 0;
    DescriptorID = 0;
    MinValue = "";
    MaxValue = "";
}

ValueClass::ValueClass(ValueClass& aValueClass)
{
    theName = aValueClass.theName;
    ValueID = aValueClass.ValueID;
    DescriptorID = aValueClass.DescriptorID;
    MinValue = aValueClass.MinValue;
    MaxValue = aValueClass.MaxValue;
}

// default destructor - will eventually implement clean up code
 __fastcall ValueClass::~ValueClass()
{
}

void ValueClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " descriptor_value: ";
    LogText += "value_id=" + IntToStr( ValueID ) + ",";
    LogText += "descriptor_id=" + IntToStr( DescriptorID ) + ",";
    LogText += "value_name=" + theName + ",";
    LogText += "min_value=" + MinValue + ",";
    LogText += "max_value=" + MaxValue + ",";
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, LogText );
}

void ValueClass::InsertToDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query -> SQL -> Add(
        "Insert into descriptor_value(value_id, descriptor_id, "
        "value_name, min_value, max_value ) "
        "values ("
        ":vid, :did, :name, :min, :max)");
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
                "Error inserting new descriptor value\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ValueClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void ValueClass::UpdateToDatabase(TQuery* query)
{
    // write updated record
    query -> SQL -> Clear();
    query -> SQL -> Add(
        "Update descriptor_value set "
        "value_name = :name, "
        "min_value = :min, "
        "max_value = :max "
        " where value_id = :vid and "
        "       descriptor_id = :did ");
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
                "Error updating descriptor value\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ValueClass Error",MB_OK);
                LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}


void ValueClass::DeleteFromDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query -> SQL -> Add(
            "delete FROM descriptor_value "\
            " WHERE value_id = :vid ");
    query -> ParamByName( "vid" )->AsInteger = ValueID;
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
                "Error deleting descriptor value\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ValueClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void ValueClass::FormatSQL( TQuery* query )
{
    //  common SQL code for new and updated record
    query -> ParamByName( "vid" )->AsInteger = ValueID;
    query -> ParamByName( "did" )->AsInteger = DescriptorID;
    query -> ParamByName( "name" )->AsString = theName;
    query -> ParamByName( "min" )->AsString = MinValue;
    query -> ParamByName( "max" )->AsString = MaxValue;
}


