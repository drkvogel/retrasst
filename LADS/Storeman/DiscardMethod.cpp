#include <vcl.h>
#pragma hdrstop

#include "DiscardMethod.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmDiscardMethod *frmDiscardMethod;


void
TfrmDiscardMethod::init( Discard::Context * context )
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

    this->cmbMethod->Items->Clear();
    this->cmbMethod->Enabled = false;

    // FIXME 72 begin
    StringVec methods;
    methods.push_back("incineration");
    methods.push_back("cutting into small pieces");
    methods.push_back("application of cosmic death ray");
    // FIXME 72 end

    for (StringVec::const_iterator it = methods.begin();
        it != methods.end(); it++)
    {
        const std::string method = *it;
        this->cmbMethod->Items->Add(method.c_str());
        this->cmbMethod->Enabled = true;
    }
    this->cmbMethod->ItemIndex = -1;
    this->cmbMethod->Text = "";

    updateUI();

    return;
}

void
TfrmDiscardMethod::updateUI( )
{
    this->btnOK->Enabled = (this->cmbMethod->Text != "");

    return;
}


// start of callbacks

__fastcall TfrmDiscardMethod::TfrmDiscardMethod(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardMethod::btnCancelClick(TObject *Sender)
{
	int mr = mrNone;

    do
    {
        m_context->setMethod("");

	    mr = mrCancel;

    } while (false);

	this->ModalResult = mr;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardMethod::cmbMethodChange(TObject *Sender)
{
    updateUI();
}
//---------------------------------------------------------------------------


void __fastcall TfrmDiscardMethod::btnOKClick(TObject *Sender)
{
    int mr = mrNone;

    do
    {
		AnsiString method =  this->cmbMethod->Text;

        if (method == "")
        {
            break; // FIXME
        }

        m_context->setMethod(method.c_str());

        mr = mrOk;

    } while (false);

	this->ModalResult = mr;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscardMethod::ediDescChange(TObject *Sender)
{
    updateUI();
}
//---------------------------------------------------------------------------

