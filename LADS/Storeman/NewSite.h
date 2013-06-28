//---------------------------------------------------------------------------

#ifndef NewSiteH
#define NewSiteH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <memory>
#include "Inventory.h"

//---------------------------------------------------------------------------
class TfrmNewSite : public TForm
{
__published:	// IDE-managed Components

	TButton *BtnOK;
	TEdit *TxtFull;
	TLabel *LblFull;
	TEdit *TxtName;
	TLabel *LblName;

	void __fastcall SaveClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations

	Site site;

public:		// User declarations
	__fastcall TfrmNewSite(TComponent* Owner);
	Site & getDetails() { return site; }
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmNewSite *frmNewSite;
//---------------------------------------------------------------------------
#endif
