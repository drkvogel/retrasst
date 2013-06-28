//---------------------------------------------------------------------------
#ifndef AddProjectNote1H
#define AddProjectNote1H

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

//---------------------------------------------------------------------------
class TAddProjectNote : public TForm
{
__published:	// IDE-managed Components

	TMemo *NoteMemo;
	TPanel *Panel1;
	TButton *Save;
	TButton *Cancel;
	TButton *DeleteNote;
	TLabel *lblTimestamp;
	TQuery *QueryProject;
	
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall DeleteNoteClick(TObject *Sender);
	void __fastcall SaveClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);

private:	// User declarations

	int object_id;
	bool NoteAdded;

	void FindNoteForObject();

public:		// User declarations

	__fastcall TAddProjectNote(TComponent* Owner);
	void SetObjectID( int objid ) { object_id = objid; }

	bool CheckPresence();
	bool CheckAbsence();

	bool GetNoteAdded() { return NoteAdded; }
	void ResetNoteAdded() { NoteAdded = false; }
};
//---------------------------------------------------------------------------
extern PACKAGE TAddProjectNote *AddProjectNote;
//---------------------------------------------------------------------------
#endif
