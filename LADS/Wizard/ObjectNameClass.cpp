//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ObjectNameClass.h"
#include "DbAuditTrail.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

#include <dbtables.hpp>
#include <stdlib.h>
#include "LogMod1.h"
#include "DateMod1.h"
const int ObjectNameClass::OBJ_TYPE_CALIBRANT             = 1;
const int ObjectNameClass::OBJ_TYPE_REAGENT               = 2;
const int ObjectNameClass::OBJ_TYPE_ANALYZER_EVENT        = 3;
const int ObjectNameClass::OBJ_TYPE_SAMPLE_INVENTORY_SYSTEM_EVENT = 4;
const int ObjectNameClass::OBJ_TYPE_MACHINE_CLUSTER       = 5;
const int ObjectNameClass::OBJ_TYPE_ALIQUOT_TYPE          = 6;
const int ObjectNameClass::OBJ_TYPE_WESTGARD_QC_RULE      = 7;
const int ObjectNameClass::OBJ_TYPE_TANK_LOCATION         = 8;
const int ObjectNameClass::OBJ_TYPE_HIVE_OF_STORAGE_BOXES = 9;
const int ObjectNameClass::OBJ_NUM_OBJECTTYPES = 9;

/*@*/

ObjectNameClass::ObjectNameClass()
: theName("")
{
    ObjectID = 0;
    FullName = "";
    ObjectType = 0;
    Status = 0;
}

ObjectNameClass::ObjectNameClass(AnsiString aName)
: theName(aName)
{
    ObjectID = 0;
    FullName = "";
    ObjectType = 0;
    Status = 0;
}

__fastcall ObjectNameClass::~ObjectNameClass()
{
}

void ObjectNameClass::LoadFromDatabase(TQuery* query, int recno)
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
        ObjectID = query -> FieldByName("object_cid") -> AsInteger;
        theName = query -> FieldByName("external_name") -> AsString;
        FullName = query -> FieldByName("external_full") -> AsString;
        Status = (short) query -> FieldByName("status") -> AsInteger;
        ObjectType = (short) query -> FieldByName("object_type") -> AsInteger;
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading Object from database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ObjectName Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
}

void ObjectNameClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " c_object_name: ";
    LogText += "object_cid=" + IntToStr( ObjectID ) + ",";
    LogText += "external_name=" + theName + ",";
    LogText += "external_full=" + FullName + ",";
    LogText += "status=" + IntToStr(Status) + ",";
    LogText += "object_type=" + IntToStr(ObjectType) + ",";
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, LogText );
}

void ObjectNameClass::InsertToDatabase(TQuery* query)
{
//  write new record with NEW status
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add(
        "Insert into c_object_name(object_cid, external_name, "
        "external_full, status, object_type, process_cid, time_stamp ) "
        "values ( :cid, :name, :full, :stat, :type, :pcid, :time )");
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
                "Error inserting new Object\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ObjectName Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}


void ObjectNameClass::UpdateToDatabase(TQuery* query)
{
    // write updated record
    query -> SQL -> Clear();
    query->SQL->Add(
        "Update c_object_name set "
        "external_name = :name, "
        "external_full = :full, "
        "status = :stat, "
        "object_type = :type, "
        "process_cid = :pcid, "
        "time_stamp = :time "
        " where object_cid = :cid ");
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
                "Error updating Object\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ObjectName Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void ObjectNameClass::FormatSQL( TQuery* query )
{
    query -> ParamByName( "cid" )->AsInteger = ObjectID;
    query -> ParamByName( "name" )->AsString = theName;
    query -> ParamByName( "full" )->AsString = FullName;
    query -> ParamByName( "stat" )->AsSmallInt = Status;
    query -> ParamByName( "type" )->AsSmallInt = ObjectType;
    query -> ParamByName( "pcid" )->AsInteger = LogMod->GetProcessID();
    query -> ParamByName( "time" )->AsString = "now";
}
