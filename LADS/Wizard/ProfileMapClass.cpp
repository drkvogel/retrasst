
#include <vcl.h>
#include <dbtables.hpp>
#include <stdlib.h>
#include "ProfileMapClass.h"

/*@*/

ProfileMapClass::ProfileMapClass()
: maxValues(30),fieldNamePrefix("vid")
{
    ProfileMapID = 0;
    ProfileID = 0;
    for ( int index=0; index<maxValues; index++ )
    {
        ValueID[index] = 0;
    }
}

 __fastcall ProfileMapClass::~ProfileMapClass()
{
}

void ProfileMapClass::LoadFromDatabase(TQuery* query, int recno)
{
    if(query -> RecordCount <= recno)
    {
        //error, index out of range
        return;
    }
    else if ( recno == 0 )
    {
        query -> First();
    }
    else
    {
        query -> Next();
    }

    try
    {
        ProfileMapID = query -> FieldByName("profile_map_id") -> AsInteger;
        ProfileID = query -> FieldByName("test_profile_id") -> AsInteger;
        AnsiString fieldNamePrefix = "value_id";
        AnsiString fieldName;
        for ( int index=0; index<maxValues; index++ )
        {
            fieldName = fieldNamePrefix + IntToStr( index + 1 );
            ValueID[index] = query->FieldByName(fieldName.c_str())->AsInteger;
        }
    }
    catch(Exception& e)
    {
        AnsiString strError =
            "Error loading profile map.\n" + e.Message;
        Application->MessageBox(
            strError.c_str(),
            "ProfileMapClass Error",MB_OK);
	}
}

void ProfileMapClass::InsertToDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add("Insert into profile_map("
        "profile_map_id, test_profile_id ");
    for ( int value=1; value<=30; value++ )
    {
        query->SQL->Add(",value_id" + IntToStr(value));
    }
    query->SQL->Add(") values (:pid, :tpid ");
    for ( int value=1; value<=30; value++ )
    {
        query->SQL->Add(",:vid" + IntToStr(value));
    }
    query->SQL->Add(")");
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
                "Error inserting new profile map.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ProfileMapClass Error",MB_OK);
		}
    }
}

void ProfileMapClass::UpdateToDatabase(TQuery* query)
{
    AnsiString fieldName;

    // write updated record
    query -> SQL -> Clear();
    query->SQL->Add("Update profile_map set test_profile_id = :tpid ");
    for ( int value=1; value<=30; value++ )
    {
        query->SQL->Add(",value_id" + IntToStr(value) +
            " = :vid" + IntToStr(value));
    }
    query->SQL->Add(" where profile_map_id = :pid ");
    FormatSQL( query );

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
                "Error updating profile map.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ProfileMapClass Error",MB_OK);
		 }
	}
}


void ProfileMapClass::DeleteFromDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query -> SQL -> Add(
        "DELETE FROM profile_map "
        " WHERE profile_map_id = :pid ");
    query -> ParamByName( "pid" )->AsInteger = ProfileMapID;
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
                "Error deleting profile map.\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ProfileMapClass Error",MB_OK);
		}
	}
}

void ProfileMapClass::FormatSQL( TQuery* query )
{
    AnsiString fieldName;

    query->ParamByName("pid")->AsInteger = ProfileMapID;
    query->ParamByName("tpid")->AsInteger = ProfileID;
    for ( int index=0; index<maxValues; index++ )
    {
        fieldName = fieldNamePrefix + IntToStr( index + 1 );
        query->ParamByName(fieldName.c_str())->AsInteger = ValueID[index];
    }
}



