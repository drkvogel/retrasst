#include <vcl.h>
#pragma hdrstop

#include "DiscardNote.h"
#include "StringUtil.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmDiscardNote *frmDiscardNote;


void
TfrmDiscardNote::init( Discard::Context * context )
{
    if (context == 0)
    {
		std::string message = "";
		message += "null context";
		message += " at ";
        message += HERE;
		throw Exception(message.c_str());
    }
    m_context = context;

    this->cmbCanned->Items->Clear();
    this->cmbCanned->Enabled = false;

    // FIXME 72 begin
    StringVec cannedTexts;
    cannedTexts.push_back("please put the cat out");
    cannedTexts.push_back("call me");
    cannedTexts.push_back("clunk click every trip");
    // FIXME 72 end

    for (StringVec::const_iterator it = cannedTexts.begin();
        it != cannedTexts.end(); it++)
    {
        const std::string cannedText = *it;
        this->cmbCanned->Items->Add(cannedText.c_str());
        this->cmbCanned->Enabled = true;
    }
    this->cmbCanned->ItemIndex = -1;

    this->memNote->Clear();

    return;
}

void
TfrmDiscardNote::updateUI( )
{
    const bool isCannedText = (this->cmbCanned->Text != "");
    this->btnPaste->Enabled = isCannedText;

    const bool isNoteText = (this->memNote->Lines->Text != "");
//    this->btnOK->Enabled = isNoteText;
    this->btnOK->Enabled = true; // allow deletion of note

	return;
}

// start of callbacks

__fastcall TfrmDiscardNote::TfrmDiscardNote(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardNote::btnCancelClick(TObject *Sender)
{
	int mr = mrNone;

    do
    {
        const std::string note = "";
        m_context->setNote(note);

	    mr = mrCancel;

    } while (false);

	this->ModalResult = mr;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardNote::btnOKClick(TObject *Sender)
{
	int mr = mrNone;

    do
    {
		const std::string note = bcsToStd( this->memNote->Lines->Text );
        m_context->setNote(note);

	    mr = mrOk;

    } while (false);

	this->ModalResult = mr;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardNote::rgNoteClick(TObject *Sender)
{
	updateUI();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardNote::memNoteChange(TObject *Sender)
{
	updateUI();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardNote::FormShow(TObject *Sender)
{
	updateUI();
}
//---------------------------------------------------------------------------


void __fastcall TfrmDiscardNote::btnPasteClick(TObject *Sender)
{
	const std::string cannedText = bcsToStd( this->cmbCanned->Text );
	if (cannedText != "")
    {
		std::string text = bcsToStd( this->memNote->Text );
        if (text != "") text += " ";
        text += cannedText;
        this->memNote->Text = text.c_str();
    }
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardNote::cmbCannedChange(TObject *Sender)
{
    updateUI();    
}
//---------------------------------------------------------------------------

