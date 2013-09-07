#include <vcl.h>
#pragma hdrstop

#include "DiscardReason.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmDiscardReason *frmDiscardReason;


void
TfrmDiscardReason::init( Discard::Context * context )
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

    this->ediDesc->Text = "";
    const int dbfieldWidth = 16;
    const int jobWidth = 12; // eg " -2147483647"
    this->ediDesc->MaxLength = dbfieldWidth - jobWidth;
    
    this->cmbReason->Items->Clear();
    this->cmbReason->Enabled = false;

    // FIXME 72 begin
    StringVec reasons;
    reasons.push_back("patient has withdrawn consent");
    reasons.push_back("sample has evaporated");
    reasons.push_back("order 66");
    // FIXME 72 end

    for (StringVec::const_iterator it = reasons.begin();
        it != reasons.end(); it++)
    {
        const std::string reason = *it;
        this->cmbReason->Items->Add(reason.c_str());
        this->cmbReason->Enabled = true;
    }
    this->cmbReason->ItemIndex = -1;
    this->cmbReason->Text = "";

    updateUI();

    return;
}

void
TfrmDiscardReason::updateUI( )
{
    this->btnOK->Enabled =
        (this->ediDesc->Text != "") && (this->cmbReason->Text != "");

    return;
}


// start of callbacks

__fastcall TfrmDiscardReason::TfrmDiscardReason(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardReason::btnCancelClick(TObject *Sender)
{
	int mr = mrNone;

    do
    {
        m_context->setReason("");
        m_context->setDescription("");

	    mr = mrCancel;

    } while (false);

	this->ModalResult = mr;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardReason::cmbReasonChange(TObject *Sender)
{
    updateUI();
}
//---------------------------------------------------------------------------


void __fastcall TfrmDiscardReason::btnOKClick(TObject *Sender)
{
    int mr = mrNone;

    do
    {
		AnsiString description =  this->ediDesc->Text ;
        AnsiString reason =  this->cmbReason->Text;

        if ((description == "") || (reason == ""))
        {
            break; // FIXME
        }

		m_context->setReason(reason.c_str());
        m_context->setDescription(description.c_str());

        mr = mrOk;

    } while (false);

	this->ModalResult = mr;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardReason::ediDescChange(TObject *Sender)
{
    updateUI();
}
//---------------------------------------------------------------------------

