//---------------------------------------------------------------------------

#ifndef NewReasonH
#define NewReasonH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <memory>

class LCDbCanned;

//---------------------------------------------------------------------------

class TfrmNewReason : public TForm
{
__published:	// IDE-managed Components

	TButton *BtnOK;
	TEdit *TxtFull;
	TLabel *LblFull;
	TEdit *TxtName;
	TLabel *LblName;
	TLabel *lblReasonGroup;
	TLabel *Label2;

	void __fastcall SaveClick(TObject *Sender);

private:	// User declarations

	int textID;

public:		// User declarations

	__fastcall TfrmNewReason(TComponent* Owner);
	bool init( int groupID );
	const LCDbCanned * createRecord();
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmNewReason *frmNewReason;
//---------------------------------------------------------------------------

#endif
