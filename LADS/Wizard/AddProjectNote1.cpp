//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AddProjectNote1.h"
#include "LDbNotes.h"
#include "WizardLogin1.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAddProjectNote *AddProjectNote;
//---------------------------------------------------------------------------
__fastcall TAddProjectNote::TAddProjectNote(TComponent* Owner)
    : TForm(Owner)
{
	NoteAdded = false;
	object_id = -1;
}
//---------------------------------------------------------------------------

void __fastcall TAddProjectNote::FormActivate(TObject *Sender)
{
	NoteAdded = false;
	FindNoteForObject();

	Caption = WizardLogin -> setCaption( Caption );

		lblTimestamp->Caption = "";
		NoteMemo->Text = "";
		DeleteNote->Enabled = false;
}

//---------------------------------------------------------------------------

void __fastcall TAddProjectNote::CancelClick(TObject *Sender)
{
	   Close();
}

//---------------------------------------------------------------------------

void __fastcall TAddProjectNote::DeleteNoteClick(TObject *Sender)
{
	Application -> MessageBox( "You cannot delete a note once it has been stored", NULL, MB_OK );
}

//---------------------------------------------------------------------------

void __fastcall TAddProjectNote::SaveClick(TObject *Sender)
{
	LPDbNote projectNote( NoteMemo->Text, object_id );
	projectNote.saveRecord( QueryProject );
	NoteAdded = true;
	Close();
}

//---------------------------------------------------------------------------

bool TAddProjectNote::CheckAbsence()
{
    bool checkOK = true;

    FindNoteForObject();

    if ( QueryProject->RecordCount == 1 )
    {
        Application->MessageBox(
            "Note found in note table for this object",
            "Add Project Note Message",MB_OK);
        checkOK = false;
    }
    else if ( QueryProject->RecordCount > 1 )
    {
        Application->MessageBox(
            "More than 1 note found in note table for this object",
            "Add Project Note Error",MB_OK);
            checkOK = false;
    }

    return checkOK;
}
//---------------------------------------------------------------------------

bool TAddProjectNote::CheckPresence()
{
    bool checkOK = true;

    FindNoteForObject();

    if ( QueryProject->RecordCount > 1 )
    {
        Application->MessageBox(
            "More than 1 note found in note table for this object.",
            "Add Project Note",MB_OK);
        checkOK = false;
    }
    else if ( QueryProject->RecordCount < 1 )
    {
        Application->MessageBox(
            "No notes found in note table for this object.",
            "Add Project Note",MB_OK);
        checkOK = false;
    }

    return checkOK;
}
//---------------------------------------------------------------------------

void TAddProjectNote::FindNoteForObject()
{
    QueryProject -> SQL -> Clear();

    QueryProject->SQL->Text =
        "select * from note "
        "where object_id = :objid ";
    QueryProject -> ParamByName( "objid" )->AsInteger = object_id;

    try
    {
        QueryProject -> Open();
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
                "Project Note Error",MB_OK);
		}
	}
}

//---------------------------------------------------------------------------

