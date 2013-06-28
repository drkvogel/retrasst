#include <vcl.h>
#include <dbtables.hpp>
#include <stdlib.h>
#include "LogMod1.h"
#include "OperatorClass.h"
#include "DateMod1.h"
#include "xmd5.c"
const int OperatorClass::OPER_ABIL_VIEW      = 0;
const int OperatorClass::OPER_ABIL_VALIDATE1 = 1;
const int OperatorClass::OPER_ABIL_VALIDATE2 = 2;
const int OperatorClass::OPER_ABIL_PLOTQC    = 4;
const int OperatorClass::OPER_ABIL_WIZARD    = 8;
const int OperatorClass::NUM_ABILITIES       = 5;

const int OperatorClass::OPER_STATUS_ACTIVE    = 0;
const int OperatorClass::OPER_STATUS_INACTIVE  = 99;
const AnsiString OperatorClass::STR_OPER_STATUS_ACTIVE   = "active";
const AnsiString OperatorClass::STR_OPER_STATUS_INACTIVE = "inactive";
const int OperatorClass::NUM_OPER_STATUS          = 2;

/*@*/
OperatorClass::OperatorClass()
: theName("")
{
    OperatorID = 0;
    Description = "";
    ValidFrom = DateMod->GetDefaultDate();
    ValidTo = DateMod->GetDefaultDate();
    Status = OPER_STATUS_ACTIVE;
    Password = "";
    Password_md5 = "";
    Abilities = 0;
}

OperatorClass::OperatorClass(AnsiString aName)
: theName(aName)
{
    OperatorID = 0;
    Description = "";
    ValidFrom = DateMod->GetDefaultDate();
    ValidTo = DateMod->GetDefaultDate();
    Status = OPER_STATUS_ACTIVE;
    Password = "";
    Password_md5 = "";
    Abilities = 0;
}

__fastcall OperatorClass::~OperatorClass()
{
}

void OperatorClass::LoadFromDatabase(TQuery* query, int recno)
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
            OperatorID = query -> FieldByName("operator_cid") -> AsInteger;
            theName = query -> FieldByName("external_name") -> AsString;
            Description = query -> FieldByName("external_full") -> AsString;
            ValidFrom = query -> FieldByName("valid_from") -> AsString;
            ValidTo = query -> FieldByName("valid_to") -> AsString;
            Status = (short)query -> FieldByName("status") -> AsInteger;
            Password_md5 = query -> FieldByName("password_md5") -> AsString;
            Abilities = query -> FieldByName("abilities") -> AsInteger;
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading operator from database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "OperatorClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        }
    }
}

void OperatorClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " c_operator: (Part 1) ";
    LogText += "operator_cid=" + IntToStr( OperatorID ) + ",";
    LogText += "external_name=" + theName + ",";
    LogText += "external_full=" + Description + ",";
    LogText += "valid_from=" + ValidFrom + ",";
    LogText += "valid_to=" + ValidTo + ",";
    LogText += "status=" + IntToStr( Status ) + ",";
    LogText += "password_md5=" + Password_md5 + ",";
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, LogText );

    LogText = Text + " c_operator: (Part 2) ";
    LogText += "abilities=" + IntToStr( Abilities ) + ",";
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, LogText );
}

bool OperatorClass::InsertToDatabase(TQuery* query)
{
//  write new record with NEW status
    bool recordWritten = true;
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add(
            "Insert into c_operator(operator_cid, external_name, external_full,"
            "valid_from, valid_to, status, password_md5, abilities)"
            "values ( :operator, :name, :full, :from, :to, :status, "
            ":pwordmd5, :abil)");
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
                "Error inserting new operator\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "OperatorClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
            recordWritten = false;
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
    return recordWritten;
}


bool OperatorClass::UpdateToDatabase(TQuery* query)
{
    bool recordWritten = true;

    // write updated record
    query -> SQL -> Clear();
    query->SQL->Add(
        "Update c_operator set "
        "external_name = :name, "
        "external_full = :full,"
        "valid_from = :from, "
        "valid_to = :to, "
        "status = :status, "
        "password_md5 = :pwordmd5,"
        "abilities = :abil "
        " where operator_cid = :operator ");
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
                "Error updating operator\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "OperatorClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
            recordWritten = false;
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );

    return recordWritten;
}

void OperatorClass::FormatSQL( TQuery* query )
{
    query -> ParamByName( "operator" )->AsInteger = OperatorID;
    query -> ParamByName( "name" )->AsString = theName;
    query -> ParamByName( "full" )->AsString = Description;
    DateMod->PutDateToDatabase( query, "from", ValidFrom );
    DateMod->PutDateToDatabase( query, "to", ValidTo );
    query -> ParamByName( "status" )->AsSmallInt = Status;
    query -> ParamByName( "pwordmd5" )->AsString = Password_md5;
    query -> ParamByName( "abil" )->AsInteger = Abilities;
}

void OperatorClass::AddAbility( int abil )
{
    // apply abil to Abilities
    Abilities = Abilities | abil;
}

bool OperatorClass::HasAbility( int abil )
{
    bool hasAbility = false;
    // compare abil with Abilities
    if ( ( Abilities & abil ) == abil )
    {
        hasAbility = true;
    }
    return hasAbility;
}

void OperatorClass::SetPassword( AnsiString pass )
{
    AnsiString password = pass.LowerCase();
/*
        if ( password != "ignored: use md5" )
        {
            XMD5_DIGEST result;
            if( xmd5_calc_sz( &result, password.c_str() ) )
            {
//                Password = "ignored: use md5";
                Password = password;
                Password_md5 = result.hex;
            }
            else
            {
                Password = password;
                Password_md5 = "";
            }
        }
*/
    XMD5_DIGEST result;
    xmd5_calc_sz( &result, password.c_str() );
    Password_md5 = result.hex;
}

bool OperatorClass::ValidatePassword( AnsiString typedPassword )
{
    bool valid = false;
//    if( Password_md5.IsEmpty() )
//    {
        // validate Password
//        if ( typedPassword.LowerCase() == Password )
//            valid = true;
//    }
//    else
//    {
        // validate Password_md5
        XMD5_DIGEST result;
        if( xmd5_calc_sz( &result, typedPassword.c_str() ) )
        {
            AnsiString md5pw = result.hex;
            if ( md5pw == Password_md5 )
                valid = true;
        }
//    }
    return valid;
}

