//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "tFrmErrorReport.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include "Utils.h"

TFrmErrorReport *FrmErrorReport;
//---------------------------------------------------------------------------
__fastcall TFrmErrorReport::TFrmErrorReport(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFrmErrorReport::Details_ButtonClick(TObject *Sender)
{
	errorDetails_Panel->Visible = !errorDetails_Panel->Visible;

	if (errorDetails_Panel->Visible)
	{
		this->Height = ErrorTop_Panel->Height  + 25 + 200;
		Details_Button->Caption = L"Hide Details";
	}
	else
	{
		this->Height = ErrorTop_Panel->Height + 25;
		Details_Button->Caption = L"Show Details";
	}
}
//---------------------------------------------------------------------------

void TFrmErrorReport::setTitle(const std::string &title)
{
	String s = String(title.c_str());
	this->Text = s;
}
//---------------------------------------------------------------------------

void TFrmErrorReport::setReport(const std::string &report)
{
	this->report_Memo->SetTextBuf(String(report.c_str()).c_str());
	utils::resizeMemo(report_Memo, 1.0f);
}
//---------------------------------------------------------------------------

void TFrmErrorReport::setDetails(const std::string &details)
{
	this->details_Memo->SetTextBuf(String(details.c_str()).c_str());
	utils::resizeMemo(details_Memo, 4.0f);
}
//---------------------------------------------------------------------------

void __fastcall TFrmErrorReport::FormShow(TObject *Sender)
{
	CloseError_Button->SetFocus();
	utils::resizeMemo(report_Memo, 1.0f);
	utils::resizeMemo(details_Memo, 4.0f);
	Details_ButtonClick(NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFrmErrorReport::CloseError_ButtonClick(TObject *Sender)
{
	this->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFrmErrorReport::FormResize(TObject *Sender)
{
///
}
//---------------------------------------------------------------------------


