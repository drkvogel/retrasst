
#include <vcl.h>
#include <dbtables.hpp>
#include <stdlib.h>
#include "LogMod1.h"
#include "ValueClass.h"
#include "DescriptorClass.h"
#include "DateMod1.h"
#include "DataMod1.h"
#include "ProjectClass.h"

// use DescriptorClass to provide a named container for instances of ValueClass

// DataType is obsolete in v2.2 of the database.
// It is assumed to be always String.
// Therefore, since it is defaulted to be String, I am leaving it in the class
// and allowing it to be accessed by the program, but it is not read from or
// written to the database. ACC, 17/05/04

int DescriptorClass::NextSpecimenNum = 1;
int DescriptorClass::NextMapNum = 1;
const AnsiString DescriptorClass::strDataType[] =
    {"String", "Boolean", "Date", "Integer", "Real"};
const int DescriptorClass::intString = 0;
const int DescriptorClass::intBoolean = 1;
const int DescriptorClass::intDate = 2;
const int DescriptorClass::intInteger = 3;
const int DescriptorClass::intReal = 4;
const int DescriptorClass::TUBEID_BIT = 1;

DescriptorClass::DescriptorClass()
: theName(""), numValues(-1)
{
    DescriptorID = 0;
    DescriptorSource = "";
    Description = "";
    MaximumLength = 0;
    MinimumLength = 0;
    SpecimenFieldName = "";
    MapFieldName = "";
    DataType = 0;
    ParseOrder = 0;
    Flag = 0;
}

// use DescriptorClass to provide a named container for instances of ValueClass

DescriptorClass::DescriptorClass(AnsiString aName)
: theName(aName), numValues(0)
{
    DescriptorID = 0;
    DescriptorSource = "";
    Description = "";
    MaximumLength = 0;
    MinimumLength = 0;
    SpecimenFieldName = "";
    MapFieldName = "";
    DataType = 0;
    ParseOrder = 0;
    Flag = 0;

    theValues = NULL;
}

__fastcall DescriptorClass::~DescriptorClass()
{
    DeleteValues();
}

void DescriptorClass::DeleteValues()
{
    // delete any values
    if ( theValues != NULL )
    {
        for (int val=0; val < numValues; val++)
        {
            if ( theValues[val] != NULL )
                delete theValues[val];
        }
        delete[] theValues;
        theValues = NULL;
        numValues = -1;
    }
}

void DescriptorClass::LoadValuesFromDatabase(TQuery* query)
{
    DeleteValues();
    query -> SQL -> Clear();
    query -> SQL -> Add("Select * from descriptor_value "
        " where descriptor_id = :did "
        "order by value_name, min_value ");
    query -> ParamByName( "did" )->AsInteger = DescriptorID;
    query -> Open();
    query -> First();
    numValues = query -> RecordCount;
    theValues = new ValueClass*[numValues];

    for (int x = 0; x < numValues; x++)
    {
        theValues[x] = new ValueClass;
        theValues[x]->SetName(query -> FieldByName("value_name") -> AsString);
        theValues[x]->SetValueID(query -> FieldByName("value_id") -> AsInteger);
        theValues[x]->SetDescriptorID(query -> FieldByName("descriptor_id") -> AsInteger);
        theValues[x]->SetMinValue(query -> FieldByName("min_value") -> AsString);
        theValues[x]->SetMaxValue(query -> FieldByName("max_value") -> AsString);
        query -> Next();
    }
    LogMod->Commit( query );
}

void DescriptorClass::LoadFromDatabase(TQuery* query, int recno)
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
            if ( recno == 0 )
            {
                NextSpecimenNum = 0;
                NextMapNum = 0;
            }
            DescriptorID = query -> FieldByName("descriptor_id") -> AsInteger;
            theName = query -> FieldByName("descriptor_name") -> AsString;
            DescriptorSource = query -> FieldByName("source") -> AsString;
            Description = query -> FieldByName("description") -> AsString;
            MaximumLength = (short)query -> FieldByName("max_length") -> AsInteger;
            MinimumLength = (short)query -> FieldByName("min_length") -> AsInteger;
            SpecimenFieldName = query -> FieldByName("specimen_field_name") -> AsString;
            MapFieldName = query -> FieldByName("map_field_name") -> AsString;
            ParseOrder = (short)query -> FieldByName("parse_order") -> AsInteger;
            NextSpecimenNum = max(NextSpecimenNum,GetSpecimenNum()+1);
            NextMapNum = max(NextMapNum,GetMapNum()+1);
            if ( !(DataMod->GetProject())->isBeforeVersion( 2, 1 ) )
            {
                Flag = query -> FieldByName("descriptor_flags") -> AsInteger;
            }
            if ( (DataMod->GetProject())->isBeforeVersion( 2, 2 ) )
            {
                DataType = (short)query -> FieldByName("data_type") -> AsInteger;
            }
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading descriptor.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "DescriptorClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
}

void DescriptorClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " descrip: (Part 1)";
    LogText += "descriptor_id=" + IntToStr( DescriptorID ) + ",";
    LogText += "descriptor_name=" + theName + ",";
    LogText += "source=" + DescriptorSource + ",";
    LogText += "description=" + Description + ",";
    LogText += "max_length=" + IntToStr( MaximumLength ) + ",";
    LogText += "min_length=" + IntToStr( MinimumLength );
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, LogText );

    LogText = Text + " descrip: (Part 2)";
    LogText += "specimen_field_name=" + SpecimenFieldName + ",";
    LogText += "map_field_name=" + MapFieldName + ",";
    LogText += "parse_order=" + IntToStr( ParseOrder ) + ",";
    if ( !(DataMod->GetProject())->isBeforeVersion( 2, 1 ) )
    {
        LogText += "descriptor_flags=" + IntToStr( Flag ) + ",";
    }
    if ( (DataMod->GetProject())->isBeforeVersion( 2, 2 ) )
    {
        LogText += "data_type=" + IntToStr( DataType ) + ",";
    }

    LogMod->LogProjectDb( TLogMod::LOGLOCAL, LogText );
}

void DescriptorClass::InsertToDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    if ( (DataMod->GetProject())->isBeforeVersion( 2, 1 ) )
    {
        query->SQL->Add(
            "Insert into descrip(descriptor_id, descriptor_name, source, "
            "description, max_length, min_length, "
            "specimen_field_name, map_field_name, data_type, parse_order) "
            "values ("
            ":did, :name, :source, :descrip, :max, :min, "
            ":spec, :map, :type, :order) ");
    }
    else if ( (DataMod->GetProject())->isBeforeVersion( 2, 2 ) )
    {
        query->SQL->Add(
            "Insert into descrip(descriptor_id, descriptor_name, source, "
            "description, max_length, min_length, "
            "specimen_field_name, map_field_name, data_type, parse_order, "
            "descriptor_flags) "
            "values ("
            ":did, :name, :source, :descrip, :max, :min, "
            ":spec, :map, :type, :order, :flag ) ");
    }
    else
    {
        query->SQL->Add(
            "Insert into descrip(descriptor_id, descriptor_name, source, "
            "description, max_length, min_length, "
            "specimen_field_name, map_field_name, parse_order, "
            "descriptor_flags) "
            "values ("
            ":did, :name, :source, :descrip, :max, :min, "
            ":spec, :map, :order, :flag ) ");
    }

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
                "Error inserting new descriptor.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "DescriptorClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void DescriptorClass::UpdateToDatabase(TQuery* query)
{
    //      write updated record
    query -> SQL -> Clear();
    query->SQL->Add(
        "Update descrip set "
        "descriptor_name = :name, "
        "source = :source, "
        "description = :descrip, "
        "max_length = :max, "
        "min_length = :min, "
        "specimen_field_name = :spec, "
        "map_field_name = :map, ");
    if ( !(DataMod->GetProject())->isBeforeVersion( 2, 1 ) )
    {
        query->SQL->Add(
            "descriptor_flags = :flag, ");
    }
    else if ( !(DataMod->GetProject())->isBeforeVersion( 2, 1 ) )
    {
        query->SQL->Add(
            "data_type = :type, ");
    }
    query->SQL->Add(
        "parse_order = :order "
        " where descriptor_id = :did ");
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
                "Error updating descriptor.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "DescriptorClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}


void DescriptorClass::DeleteFromDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query -> SQL -> Add(
            "DELETE FROM descrip "
            " WHERE descriptor_id = :did ");
    query -> ParamByName( "did" )->AsInteger = DescriptorID;

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
                "Error deleting descriptor.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "DescriptorClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void DescriptorClass::FormatSQL( TQuery* query )
{
    //  common SQL code for new and updated record
    query -> ParamByName( "did" )->AsInteger = DescriptorID;
    query -> ParamByName( "name" )->AsString = theName;
    query -> ParamByName( "source" )->AsString = DescriptorSource;
    query -> ParamByName( "descrip" )->AsString = Description;
    query -> ParamByName( "max" )->AsSmallInt = MaximumLength;
    query -> ParamByName( "min" )->AsSmallInt = MinimumLength;
    query -> ParamByName( "spec" )->AsString = SpecimenFieldName;
    query -> ParamByName( "map" )->AsString = MapFieldName;
    query -> ParamByName( "order" )->AsSmallInt = ParseOrder;
    if ( !(DataMod->GetProject())->isBeforeVersion( 2, 1 ) )
    {
        query -> ParamByName( "flag" )->AsSmallInt = Flag;
    }
    if ( (DataMod->GetProject())->isBeforeVersion( 2, 2 ) )
    {
        query -> ParamByName( "type" )->AsSmallInt = DataType;
    }
}

bool DescriptorClass::CheckDuplicateName(TQuery* query)
{
    bool duplicate = false;
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add(
        "select * from descrip "
        " where descriptor_id != :did "
        "   and descriptor_name = :name ");
    query -> ParamByName( "did" )->AsInteger = DescriptorID;
    query -> ParamByName( "name" )->AsString = theName;

    try
    {
        query -> Open();
    }
    catch(Exception& e)
    {
        if(e.Message != "Error creating cursor handle")
        {
            AnsiString strError =
                "Error checking for duplicate descriptor name.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "DescriptorClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    if ( query->RecordCount > 0 ) duplicate = true;
    LogMod->Commit( query );
    return duplicate;
}

bool DescriptorClass::CheckDuplicateParseOrder(TQuery* query)
{
    bool duplicate = false;

    if ( DescriptorSource == "barcode" )
    {
        query -> Close();
        query -> SQL -> Clear();
        query->SQL->Add(
            "select * from descrip "
            " where descriptor_id != :did "
            "   and source = 'barcode' "
            "   and parse_order = :order ");
        query -> ParamByName( "did" )->AsInteger = DescriptorID;
        query -> ParamByName( "order" )->AsSmallInt = ParseOrder;

        try
        {
            query -> Open();
        }
        catch(Exception& e)
        {
            if(e.Message != "Error creating cursor handle")
            {
                AnsiString strError =
                    "Error checking for duplicate parse order.\n" + e.Message;
                Application->MessageBox(
                    strError.c_str(),
                    "DescriptorClass Error",MB_OK);
                LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
            }
        }
        if ( query->RecordCount > 0 ) duplicate = true;
        LogMod->Commit( query );
    }
    return duplicate;
}

int DescriptorClass::GetSpecimenNum()
{
    int num = 0;
    if ( SpecimenFieldName.SubString(1,5) == "value" )
    {
        num = StrToInt(
            SpecimenFieldName.SubString(6,SpecimenFieldName.Length()));
    }
    return num;
}

// get a pointer to the instance of ValueClass indexed by index from theValues
// array held by this DescriptorClass. This is a pointer to the actual instance,
// so changes may be made and Profiles added. This method returns NULL if index is
// greater than the number of codes held by this instance of DescriptorClass
ValueClass* DescriptorClass::GetValue( int index )
{

    if(index < numValues)
        return theValues[index];
    else
        return NULL;
}

// get a pointer to the instance of ValueClass from theValues array held by this
// DescriptorClass whose name is IDENTICAL to ValueName. This is a pointer to the
// actual instance, so changes may be made and Profiles added. This method returns
// NULL if none of the ValueClasses held by this DescriptorClass have names which
// are equal ( == ) to ValueName

ValueClass* DescriptorClass::GetValue( AnsiString ValueName )
{
    int index = 0;

    while(index < numValues && ValueName != theValues[index] -> GetName())
    {
        index++;
    }

    if(index < numValues)
        return theValues[index];
    else
        return NULL;
}


// get a pointer to the instance of ValueClass from theValues array held by this
// DescriptorClass whose index is IDENTICAL to id. This is a pointer to the
// actual instance, so changes may be made and Profiles added. This method returns
// NULL if none of the ValueClasses held by this DescriptorClass have names which
// are equal ( == ) to ValueName

ValueClass* DescriptorClass::GetValueByID( int id  )
{
    int index = 0;

    while(index < numValues && id != theValues[index] -> GetValueID())
    {
        index++;
    }

    if(index < numValues)
        return theValues[index];
    else
        return NULL;
}

