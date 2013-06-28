//---------------------------------------------------------------------------
#ifndef Profile1H
#define Profile1H

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <set>

//---------------------------------------------------------------------------
class TProfile : public TForm
{
__published:	// IDE-managed Components

	TPanel *Panel2;
	TLabel *Label1;
	TButton *Add;
	TButton *Change;
	TPanel *Panel3;
	TButton *btnClose;
	TQuery *QueryCentral;
	TQuery *QueryProject;
	TQuery *QueryProjectDdb;
	TButton *btnReport;
	TStringGrid *ProfileTable;

	void __fastcall AddClick(TObject *Sender);
	void __fastcall ChangeClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall ProfileTableMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnReportClick(TObject *Sender);
	void __fastcall ProfileTableDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);

private:	// User declarations

	std::set< int > selected;
	void CopyFromClassToGrid();

	// columns in grid
	enum { TP_PROFILEID, TP_PROFILE, TP_DESCRIPTION, TP_TEST, TP_STORE, TP_ALIQUOT,
		   TP_DATELASTMOD, TP_CURRENT, TP_NUMCOLS };

public:		// User declarations

	__fastcall TProfile(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TProfile *Profile;
//---------------------------------------------------------------------------
#endif
