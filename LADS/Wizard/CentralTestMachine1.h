//---------------------------------------------------------------------------
#ifndef CentralTestMachine1H
#define CentralTestMachine1H
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ColourStringGrid.h"
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <set>

//---------------------------------------------------------------------------
class TCentralTestMachine : public TForm
{
__published:	// IDE-managed Components

	TColourStringGrid *CentralTestMachineGrid;
	TPanel *Panel2;
	TLabel *Label1;
	TButton *Add;
	TButton *Change;
	TButton *Delete;
	TPanel *Panel1;
	TButton *btnClose;
	TQuery *QueryCentral;

	void __fastcall AddClick(TObject *Sender);
	void __fastcall ChangeClick(TObject *Sender);
	void __fastcall DeleteClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall CentralTestMachineGridMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);

private:	// User declarations

	enum { CTM_MACHINEID, CTM_MACHINE, CTM_TESTNAME,
			CTM_PROTOCOL, CTM_TESTORDER, CTM_NUMCOLS };

	int testID;

	int FillChooseMachineGrid();

public:		// User declarations

	__fastcall TCentralTestMachine(TComponent* Owner);

	void setTest( int test ) { testID = test; }
	void CopyFromClassToGrid();
};
//---------------------------------------------------------------------------
extern PACKAGE TCentralTestMachine *CentralTestMachine;
//---------------------------------------------------------------------------
#endif
