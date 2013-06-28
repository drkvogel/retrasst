//---------------------------------------------------------------------------
#ifndef Project1H
#define Project1H
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ColourStringGrid.h"
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <set>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <set>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

//---------------------------------------------------------------------------
class TProject : public TForm
{
__published:	// IDE-managed Components

	TPanel *Panel2;
	TLabel *Label1;
	TButton *Add;
	TButton *Change;
	TColourStringGrid *ProjectTable;
	TPanel *Panel1;
	TButton *btnClose;
	TQuery *QueryCentral;

	void __fastcall AddClick(TObject *Sender);
	void __fastcall ChangeClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall ProjectTableMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);

private:	// User declarations

	std::set< int > selected;
	void CopyFromClassToGrid();

	// columns in Grid
	enum { P_PROJECT, P_FULLNAME, P_VALIDFROM, P_VALIDTO, P_STATUS,
			P_DATABASE, P_URL, P_DBVERSION, P_PROJECTID, NUM_PROJECT_COLS };

public:		// User declarations

	__fastcall TProject(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TProject *Project;
//---------------------------------------------------------------------------

#endif