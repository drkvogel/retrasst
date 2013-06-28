//----------------------------------------------------------------------------
#ifndef TfrmConfirmH
#define TfrmConfirmH
#include <System.Classes.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
//----------------------------------------------------------------------------

#include <set>

//----------------------------------------------------------------------------

class TfrmConfirm : public TForm
{
__published:

	TLabel *Label1;
	TEdit *edtPassword;
	TLabel *Label2;
	TComboBox *cbUserNames;
	TLabel *Label4;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TLabel *Label5;
	TLabel *lblSummary;

	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall userDetailChange(TObject *Sender);

public:

	virtual __fastcall TfrmConfirm(TComponent* AOwner);
	void initialise( short stage, const std::string & summary );
	void initialise( short stage, const std::string & summary, const std::set< int > & projects );
	void remove( const std::set< int > & userIDs );
};

//----------------------------------------------------------------------------
extern PACKAGE TfrmConfirm *frmConfirm;
//----------------------------------------------------------------------------
#endif    
