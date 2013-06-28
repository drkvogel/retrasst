//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ProjectNoteClass.h"
#include <dbtables.hpp>
#include <stdlib.h>
#include "LogMod1.h"
#include "DateMod1.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

/*@*/

ProjectNoteClass::ProjectNoteClass()
{
    NoteID = 0;
    ObjectID = 0;
    Contents = "";
    Timestamp = DateMod->GetDefaultDate();
}

__fastcall ProjectNoteClass::~ProjectNoteClass()
{
}

void ProjectNoteClass::LoadFromDatabase(TQuery* query, int recno)
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
        NoteID = query -> FieldByName("note_id") -> AsInteger;
        ObjectID = query -> FieldByName("object_id") -> AsInteger;
        Contents = query -> FieldByName("contents") -> AsString;
        Timestamp = query -> FieldByName("time_stamp") -> AsDateTime;
        }
        catch(Exception& e)
        {
            AnsiString strError =
                "Error loading note from database\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ProjectNoteClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
}

void ProjectNoteClass::LogRecord( AnsiString Text )
{
    AnsiString LogText;

    LogText = Text + " note: (Part 1)";
    LogText += "note_id=" + IntToStr( NoteID ) + ",";
    LogText += "object_id=" + IntToStr( ObjectID ) + ",";
    LogText += "time_stamp=" + Timestamp;
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, LogText );

    LogText = Text + " note: (Part 2)";
    LogText += "contents=" + Contents;
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, LogText );
}

void ProjectNoteClass::InsertToDatabase(TQuery* query)
{
//  write new record with NEW status
    query -> Close();
    query -> SQL -> Clear();
    query->SQL->Add(
        "Insert into note(note_id, object_id, "
        "contents, process_cid, time_stamp) "
        "values ( :nid, :object, :cont, :pcid, :time) ");
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
                "Error inserting new note\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ProjectNoteClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void ProjectNoteClass::UpdateToDatabase(TQuery* query)
{
    // write updated record
    query -> SQL -> Clear();
    query->SQL->Add(
        "Update note set "
        "object_id = :object, "
        "contents = :cont,"
        "process_cid = :pcid, "
        "time_stamp = :time "
        " where note_id = :nid ");
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
                "ProjectNoteClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

void ProjectNoteClass::FormatSQL( TQuery* query )
{
    query -> ParamByName( "nid" )->AsInteger = NoteID;
    query -> ParamByName( "object" )->AsInteger = ObjectID;
    query -> ParamByName( "cont" )->AsString = Contents;
    query -> ParamByName( "pcid" )->AsInteger = LogMod->GetProcessID();
    DateMod->PutTimeStampToDatabase( query, "time" );
}


void ProjectNoteClass::DeleteFromDatabase(TQuery* query)
{
    query -> Close();
    query -> SQL -> Clear();
    query -> SQL -> Add(
        "DELETE FROM note "
        " WHERE note_id = :nid ");
    query -> ParamByName( "nid" )->AsInteger = NoteID;
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
                "Error deleting note\n" + e.Message;
            Application->MessageBox(
                strError.c_str(),
                "ProjectNoteClass Error",MB_OK);
            LogMod->LogProjectDb( TLogMod::LOGLOCAL, strError );
        }
    }
    LogMod->LogProjectDb( TLogMod::LOGLOCAL, query );
    LogMod->Commit( query );
}

