#include <vcl.h>
#pragma hdrstop

#include "RetrAsstAddNote.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmRetrAsstAddNote *frmRetrAsstAddNote;

__fastcall TfrmRetrAsstAddNote::TfrmRetrAsstAddNote(TComponent* Owner) : TForm(Owner) { }

void __fastcall TfrmRetrAsstAddNote::FormShow(TObject *Sender) {
    // memoNote->Lines->Clear();
    // might want to start with default text
}

void TfrmRetrAsstAddNote::setText(std::string text) {
    memoNote->Clear();
    memoNote->Text = text.c_str();
}

void __fastcall TfrmRetrAsstAddNote::btnAddNoteClick(TObject *Sender) {
    /*
    LDbNote note(noteText.c_str());
    note.saveRecord(*m_pq);
    */
    ModalResult = mrOk; //Close();
}

void __fastcall TfrmRetrAsstAddNote::btnCancelClick(TObject *Sender) {
    memoNote->Lines->Clear();
    ModalResult = mrCancel; //Close();
}


