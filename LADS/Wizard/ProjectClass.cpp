#include <vcl.h>
#pragma hdrstop

#include "ProjectClass.h"
#include <dbtables.hpp>
#include <stdlib.h>
#include "LogMod1.h"
#include "DateMod1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
const int ProjectClass::PROJECTTEST  = 0;
const int ProjectClass::PROJECTLIVE  = 1;
const int ProjectClass::PROJECTDEAD  = 99;
//---------------------------------------------------------------------------

ProjectClass::ProjectClass()
{
    ProjectID = 0;
    theName = "";
    FullName = "";
    ValidFrom = "";
    ValidTo = "";
    Status = 0;
    DbMajorVersion = -1;
    DbMinorVersion = -1;
    DatabaseName = "";
    URL = "";
    NoteExists = 0;
}


__fastcall ProjectClass::~ProjectClass()
{
}

bool ProjectClass::isBeforeVersion( int majorVersion, int minorVersion )
{
    // check that this database is before the given version number
    // used for project databases
    // returns true if database < specified version
    bool compatibleVersion = false;
    if ( ( DbMajorVersion == majorVersion && DbMinorVersion < minorVersion )
        || DbMajorVersion < majorVersion )
        compatibleVersion = true;
    return compatibleVersion;
}

bool ProjectClass::LoadFromDatabaseByDbName(TQuery* query, AnsiString dbName)
{
    bool validRecord = false;

    try
    {
        query -> SQL -> Clear();
        query -> SQL -> Add(
            "select * from c_project where db_name = :dbnam");
        query -> ParamByName( "dbnam" )->AsString = dbName;
        query -> Open();
        query -> First();
        validRecord = LoadRecord( query );
    }
    catch(Exception& e)
    {
        AnsiString strError =
            "Error loading project from database\n" + e.Message;
        Application->MessageBox(
            strError.c_str(),
            "ProjectClass Error",MB_OK);
        LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
    }
    return validRecord;
}


bool ProjectClass::LoadFromDatabaseByName(TQuery* query, AnsiString name)
{
    bool validRecord = false;
    try
    {
        query -> SQL -> Clear();
        query -> SQL -> Add(
            "select * from c_project where external_name = :pnam");
        query -> ParamByName( "pnam" )->AsString = name;
        query -> Open();
        query -> First();

        validRecord = LoadRecord( query );
    }
    catch(Exception& e)
    {
        AnsiString strError =
            "Error loading project from database\n" + e.Message;
        Application->MessageBox(
            strError.c_str(),
            "ProjectClass Error",MB_OK);
        LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
    }
    return validRecord;
}

bool ProjectClass::LoadFromDatabase(TQuery* query, int recno)
{
    bool validRecord = false;

    if(query -> RecordCount <= recno)
    {
        //error, index out of range
        return validRecord;
    }
    else
    {
        query -> First();
        for (int x = 0; x < recno; x++)
        {
            query -> Next();
        }
        validRecord = LoadRecord( query );
    }
    return validRecord;
}

bool ProjectClass::LoadRecord( TQuery* query )
{
    bool validProject = false;

    if ( query->RecordCount <= 0 )
    {
        AnsiString strError = "Invalid project name\n";
        Application->MessageBox(
            strError.c_str(),
            "ProjectClass Error",MB_OK);
        LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
        return validProject;
    }

    try
    {
        ProjectID = query -> FieldByName("project_cid") -> AsInteger;
        theName = query -> FieldByName("external_name") -> AsString;
        FullName = query -> FieldByName("external_full") -> AsString;
        ValidFrom = query -> FieldByName("valid_from") -> AsString;
        ValidTo = query -> FieldByName("valid_to") -> AsString;
        Status = query -> FieldByName("status") -> AsInteger;
        if ( Status == PROJECTLIVE )
        {
            StatusName = "Live";
        }
        else if ( Status == PROJECTTEST )
        {
            StatusName = "Test";
        }
        else if ( Status == PROJECTDEAD )
        {
            StatusName = "Obsolete";
        }
        else
        {
            StatusName = "Test";
            Application->MessageBox(
                "Error in Project Status\n"
                "Setting to TEST",
                "ProjectClass Error",MB_OK);
        }
        DatabaseName = query -> FieldByName("db_name") -> AsString;
        URL = query -> FieldByName("info_url") -> AsString;
        if ( query -> FindField("note_exists") != NULL )
        {
            NoteExists = (short) query -> FieldByName("note_exists") -> AsInteger;
        }
        else
        {
            NoteExists = 0;
        }
        DbMajorVersion = query -> FieldByName("database_version") -> AsInteger;
        if ( query -> FindField("database_minor_version") != NULL )
        {
            DbMinorVersion =
                query -> FieldByName("database_minor_version") -> AsInteger;
        }
        else
        {
            DbMinorVersion = 0;
        }
        validProject = true;
    }
    catch(Exception& e)
    {
        AnsiString strError =
            "Error loading project from database\n" + e.Message;
        Application->MessageBox(
            strError.c_str(),
            "ProjectClass Error",MB_OK);
        LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
    }
    return validProject;
}

void ProjectClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " c_project: (Part 1) ";
    LogText += "project_cid=" + IntToStr( ProjectID ) + ",";
    LogText += "external_name=" + theName + ",";
    LogText += "external_full=" + FullName + ",";
    LogText += "valid_from=" + ValidFrom + ",";
    LogText += "valid_to=" + ValidTo + ",";
    LogText += "status=" + IntToStr( Status ) + ",";
    LogText += "database_version=" + IntToStr( DbMajorVersion ) + ",";
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, LogText );

    LogText = Text + " c_project: (Part 2) ";
    LogText += "db_name=" + DatabaseName + ",";
    LogText += "info_url=" + URL + ",";
    LogText += "database_minor_version=" + IntToStr( DbMinorVersion ) + ",";
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, LogText );
}

bool ProjectClass::InsertToDatabase(TQuery* query)
{
    bool inserted = true;

    //  write new record with NEW status
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Text = "INSERT into c_project "
        "(project_cid, external_name,"
        "external_full, valid_from, valid_to, status, database_version, "
        "database_minor_version, db_name, info_url) "
        "values(:cid, :pnam, :fnam, :from, :to, :status, :vers, :mvers, "
        ":pdb, :url)";
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
                "Error inserting new note\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ProjectClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
            inserted = false;
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
    return inserted;
}

bool ProjectClass::UpdateToDatabase(TQuery* query)
{
    bool updated = true;

    // write updated record
    query -> SQL -> Clear();
    query -> SQL -> Add(
        "Update c_project set "
        "external_name = :pnam,"
        "external_full = :fnam, "
        "valid_from = :from, "
        "valid_to = :to, "
        "status = :status,"
        "database_version = :vers, "
        "db_name = :pdb, "
        "info_url = :url, "
        "database_minor_version = :mvers "
        " where project_cid = :cid ");

    query -> ParamByName( "cid" )->AsInteger = ProjectID;
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
                "Error updating note\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ProjectClass Error",MB_OK);
            LogMod->LogCentralDb( TLogMod::LOGLOCAL, strError );
            updated = false;
        }
    }
    LogMod->LogCentralDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
    return updated;
}

void ProjectClass::FormatSQL( TQuery* query )
{
    query -> ParamByName( "cid" )->AsInteger = ProjectID;
    query -> ParamByName( "pnam" )->AsString = theName;
    query -> ParamByName( "fnam" )->AsString = FullName;
    DateMod->PutDateToDatabase( query, "to", ValidTo );
    DateMod->PutDateToDatabase( query, "from", ValidFrom );
    query -> ParamByName( "pdb" )->AsString = DatabaseName;
    query -> ParamByName( "url" )->AsString = URL;
    query -> ParamByName( "status" )->AsInteger = Status;
    query -> ParamByName( "vers" )->AsInteger = DbMajorVersion;
    query -> ParamByName( "mvers" )->AsInteger = DbMinorVersion;
}


