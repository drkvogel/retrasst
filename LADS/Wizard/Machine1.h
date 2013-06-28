//---------------------------------------------------------------------------
#ifndef Machine1H
#define Machine1H
#include <Classes.hpp>
#include <Controls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>

#include <set>
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>

//---------------------------------------------------------------------------

class TMachine : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TLabel *Label1;
	TButton *Add;
	TButton *Change;
	TPanel *Panel1;
	TButton *btnClose;
	TQuery *QueryCentral;
	TButton *PreviewReport;
	TStringGrid *MachineTable;

	void __fastcall AddClick(TObject *Sender);
	void __fastcall ChangeClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall MachineTableMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall PreviewReportClick(TObject *Sender);
	void __fastcall MachineTableDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);

private:	// User declarations

	std::set< int > selected;
	void CopyFromClassToGrid();

	// columns in Grid
	enum { M_MACHINE, M_DESCRIPTION, M_CLUSTER, M_VALIDFROM, M_VALIDTO,
			M_STATUS, M_LOCATION, M_MACHINEID, M_NUMCOLS };

public:		// User declarations

	__fastcall TMachine(TComponent* Owner);
};

//---------------------------------------------------------------------------
extern PACKAGE TMachine *Machine;
//---------------------------------------------------------------------------
#endif
