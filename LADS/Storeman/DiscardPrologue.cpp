#include <vcl.h>
#pragma hdrstop

#include "DiscardPrologue.h"
#include "StringUtil.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmDiscardPrologue *frmDiscardPrologue;


void
TfrmDiscardPrologue::init( Discard::Context * context )
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

    if (this->cmbProject->Items->Count > 0)
    {
        ; // keep old ui settings
    }
    else
    {
        StringVec names = m_context->getDb()->getProjectNames();
        this->cmbProject->Items->Clear();
        for (StringVec::const_iterator it = names.begin();
            it != names.end(); it++)
        {
            const std::string name = *it;
            this->cmbProject->Items->Add(name.c_str());
        }
        this->cmbProject->Enabled = (this->cmbProject->Items->Count > 0);
        this->cmbProject->ItemIndex = -1;
        this->rgCryovialStatus->ItemIndex = -1;
    }
    
	updateUI();

    return;
}

void
TfrmDiscardPrologue::updateUI( )
{
	this->btnOK->Enabled = false;
	do
	{
		if (this->cmbProject->ItemIndex == -1) break;

		if (this->rgCryovialStatus->ItemIndex == -1) break;

/*		if ((this->rgCryovialStatus->ItemIndex != 0) &&
			(this->rgCryovialStatus->ItemIndex != 1))
		{
			std::string error = "";
			error += "unexpected error";
			error += " at ";
			error += HERE;
			throw Exception(error.c_str());
		}
*/
		this->btnOK->Enabled = true;

	} while (false);

	return;
}

// callbacks begin

__fastcall TfrmDiscardPrologue::TfrmDiscardPrologue(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardPrologue::btnOKClick(TObject *Sender)
{
	int mr = mrNone;

	m_context->setProjectName(bcsToStd( this->cmbProject->Text ) );
	m_context->configCrstatus(this->rgCryovialStatus->ItemIndex);

	mr = mrOk;

	this->ModalResult = mr;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardPrologue::btnCancelClick(TObject *Sender)
{
	int mr = mrNone;
    m_context->setProjectName("");
    m_context->configCrstatus(-1);
	mr = mrCancel;
	this->ModalResult = mr;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardPrologue::rgCryovialStatusClick(TObject *Sender)
{
	updateUI();
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardPrologue::cmbProjectChange(TObject *Sender)
{
	updateUI();
}
//---------------------------------------------------------------------------


