//---------------------------------------------------------------------------
#ifndef EditCentralDb1H
#define EditCentralDb1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------

class TEditCentralDb : public TForm
{
__published:	// IDE-managed Components

	TPanel *Panel1;
	TButton *ProjectInfo;
	TButton *Tests;
	TButton *Machines;
	TPanel *Panel2;
	TButton *btnClose;
	TButton *Operators;
	TButton *btnBoxSize;
	TButton *btnQC;
	TButton *btnRackSize;
	TListBox *lbObjects;

	void __fastcall ProjectInfoClick(TObject *Sender);
	void __fastcall TestsClick(TObject *Sender);
	void __fastcall MachinesClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall OperatorsClick(TObject *Sender);
	void __fastcall btnQCClick(TObject *Sender);
	void __fastcall btnBoxSizeClick(TObject *Sender);
	void __fastcall btnRackSizeClick(TObject *Sender);
	void __fastcall lbObjectsClick(TObject *Sender);

private:	// User declarations

public:		// User declarations
	__fastcall TEditCentralDb(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TEditCentralDb *EditCentralDb;
//---------------------------------------------------------------------------
#endif
