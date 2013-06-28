//----------------------------------------------------------------------------
#ifndef TfrmQuestionH
#define TfrmQuestionH
//----------------------------------------------------------------------------

#include <vcl\Buttons.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>

#include <set>

//----------------------------------------------------------------------------

class TfrmQuestion : public TForm
{
__published:
	TBitBtn *okBtn;
	TBitBtn *cancelBtn;
	TLabel *lblSummary;
	TLabel *lblDetail;

public:

	virtual __fastcall TfrmQuestion(TComponent* AOwner);
	void setDetail( const String & text );
	bool confirm( const String & text, const String & okText, const String & cancel );
	bool confirmExit();
};

//----------------------------------------------------------------------------
extern PACKAGE TfrmQuestion *frmQuestion;
//----------------------------------------------------------------------------
#endif    
