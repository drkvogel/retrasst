//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <Dbtables.hpp>
#include "DataMod1.h"
#include "LogMod1.h"
#include "ProjectClass.h"
#include "DbShared.h"
#include "InitParams.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TLogMod *LogMod;
const int TLogMod::LOGLOCAL    = 1;   // write to Wizard.log only
const int TLogMod::LOGINFO     = 2;   // information only ( write to all logs and audit trail )
const int TLogMod::LOGERROR    = 3;   // error message ( write to all logs and audit trail )

const int TLogMod::ACTIVE_STATUS   = 0;
const int TLogMod::NEW_STATUS      = 0;
const int TLogMod::UPDATE_STATUS   = 1;
const int TLogMod::DELETE_STATUS   = 99;
//---------------------------------------------------------------------------
__fastcall TLogMod::TLogMod(TComponent* Owner)
    : TDataModule(Owner),AuditTrail( DataMod->QueryCentral,
        "anne.charles@ctsu.ox.ac.uk" )
{
    LogFile = NULL;
    loggingStarted = false;
    auditStarted = false;
}
//---------------------------------------------------------------------------
// LogMod1.h handles all logging and calls to the audit trail routines.
//---------------------------------------------------------------------------
void TLogMod::LogProjectDb( int type, TQuery* query )
{
    // write SQL to log file
    // do not write to audit trail
        AnsiString SQLString = query->Text;
        AnsiString paramString = "";

        for ( int paramIndex=0; paramIndex<query->ParamCount; paramIndex++ )
        {
            if ( paramIndex != 0 )
            {
                paramString += ",";
            }
            paramString += query->Params->Items[paramIndex]->AsString;
        }

        LogDb_LogToFile( type, (DataMod->GetProject())->GetName(), SQLString );
        LogDb_LogToFile( type, (DataMod->GetProject())->GetName(), paramString );
}
//---------------------------------------------------------------------------
void TLogMod::LogProjectDb( int type, AnsiString text )
{
   // write text to log file and audit trail (depending on value of type)
        LogDb_LogToFile( type, (DataMod->GetProject())->GetName(), text );
}
//---------------------------------------------------------------------------
void TLogMod::LogCentralDb( int type, TQuery* query )
{
    // write SQL to log file
    // do not write to audit trail
        AnsiString SQLString = query->Text;
        AnsiString paramString = "";

        for ( int paramIndex=0; paramIndex<query->ParamCount; paramIndex++ )
        {
            if ( paramIndex != 0 )
            {
                paramString += ",";
            }
            paramString += query->Params->Items[paramIndex]->AsString;
        }

        LogDb_LogToFile( type, DataMod->GetCentralDbName(), SQLString );
        LogDb_LogToFile( type, DataMod->GetCentralDbName(), paramString );
}
//---------------------------------------------------------------------------
void TLogMod::LogCentralDb( int type, AnsiString text )
{
    // write text to log file and audit trail (depending on value of type)
        LogDb_LogToFile( type, DataMod->GetCentralDbName(), text );
}
//---------------------------------------------------------------------------
void TLogMod::LogOperator( int operatorID )
{
    AuditTrail.login(operatorID,DataMod->GetCentralDbName());
}
//---------------------------------------------------------------------------
void TLogMod::LogDb_LogToFile( int type, AnsiString dbName, AnsiString text )
{
    // write text to log file
    TDateTime Today;
    Today = Today.CurrentDateTime();
    AnsiString strTimeStamp = DateTimeToStr(Today);

    AnsiString localText = text;

//  I must have had some good reason for getting rid of the trailing "\r\n".
//  However, doing this removes most of the message I want to log when a
//  new database is created. Therefore, I have commented it out.
//  ACC, 15/04/03
//    int extraChars = localText.Pos( "\r\n" );
//    if ( extraChars != 0 )
//    {
//        localText = localText.SubString( 1, extraChars - 1 );
//    }
    AnsiString auditText = localText;

    // write to local log file
    if ( loggingStarted )
    {
        localText = dbName + "|" + localText;

        int nchar = fprintf( LogFile,"\n%s|%s",
        strTimeStamp.c_str(),
        localText.c_str() );

        if ( nchar < 0 )
        {
            Application->MessageBox
                ("Error trying to write to Wizard log file.",
                "Wizard log Error",MB_OK);
        }
    }

    bool auditWritten = false;
    // write to audit trail
    if ( ( type != LOGLOCAL ) && auditStarted )
    {
        for ( int times = 0; ( (times < 6) && !auditWritten ); times++ )
        {
            auditWritten = true;
            try
            {
                AuditTrail.write(
                    auditText, DbAuditTrail::SUCCESS, dbName  );
            }
            catch(Exception& e)
            {
                Application->MessageBox
                    ("Error writing to audit trail.",
                    "Wizard log Error",MB_OK);
                auditWritten = false;
            }
        }
    }
}
//---------------------------------------------------------------------------
void TLogMod::StartLogDb( bool LiveSystem, TDatabase* DatabaseCentral, TQuery* QueryCentral )
{
    AnsiString LogFileName = InitParams::get().logFolder;
    LogFileName = LogFileName + "\\Wizard.log";
    loggingStarted = true;

    if ( NULL == ( LogFile = fopen( LogFileName.c_str(), "a" ) ) )
    {
        LogFileName = "C:\Wizard.log";
        if ( NULL == ( LogFile = fopen( LogFileName.c_str(), "a" ) ) )
        {
            AnsiString Message;
            Message = "Error trying to open Wizard log file " + LogFileName + "\n" +
                "Logging will be cancelled.";
            Application->MessageBox(
                Message.c_str(),"Wizard log Error",MB_OK);
            loggingStarted = false;
        }
    }

    auditStarted = true;
    // start audit trail
    AnsiString startMessage;
    if ( LiveSystem )
    {
        startMessage = "Wizard logging started: Live";
    }
    else
    {
        startMessage = "Wizard logging started: Test";
    }
    try
    {
        AuditTrail.start( startMessage );
    }
    catch(Exception& e)
    {
        Application->MessageBox(
            "Problem starting audit trail.",
            "LogDb Error",MB_OK);
        auditStarted = false;
    }
    LogDb_LogToFile( LOGLOCAL, DataMod->GetCentralDbName(), startMessage );

    return;
}
//---------------------------------------------------------------------------
void TLogMod::StopLogDb( bool LiveSystem )
{
    AnsiString stopMessage = "Wizard logging ended";
    if ( loggingStarted )
    {
        LogDb_LogToFile( LOGLOCAL, DataMod->GetCentralDbName(), stopMessage );
        fclose( LogFile );
        loggingStarted = false;
    }

    if ( auditStarted )
    {
        try
        {
            AuditTrail.stop( stopMessage );
        }
        catch(Exception& e)
        {
            Application->MessageBox(
                "Problem stopping audit trail.\n",
                "LogDb Error",MB_OK);
        }
        auditStarted = false;
    }
}
//---------------------------------------------------------------------------
void TLogMod::Commit( TQuery* query )
{
    // send "Commit" to unlock table
    query->SQL->Clear();
    query->SQL->Add( "COMMIT" );
    query->ExecSQL();
    query->SQL->Clear();
}
//---------------------------------------------------------------------------
int TLogMod::GetProcessID()
{
    return AuditTrail.getProcessID();
}
//---------------------------------------------------------------------------


