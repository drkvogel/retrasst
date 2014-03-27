//---------------------------------------------------------------------------

#ifndef tFrmErrorReportH
#define tFrmErrorReportH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>

#include <string>
//---------------------------------------------------------------------------
class TFrmErrorReport : public TForm
{
__published:	// IDE-managed Components
	TPanel *ErrorTop_Panel;
	TMemo *report_Memo;
	TMemo *details_Memo;
	TButton *Details_Button;
	TPanel *errorDetails_Panel;
	TButton *CloseError_Button;
	void __fastcall Details_ButtonClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CloseError_ButtonClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);

private:	// User declarations

public:		// User declarations
	__fastcall TFrmErrorReport(TComponent* Owner);

	void setTitle(const std::string &title);
	void setReport(const std::string &report);
	void setDetails(const std::string &report);

};
//---------------------------------------------------------------------------
extern PACKAGE TFrmErrorReport *FrmErrorReport;
//---------------------------------------------------------------------------
#endif
