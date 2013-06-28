//---------------------------------------------------------------------------

#ifndef frmSessionListH
#define frmSessionListH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include "LCDbAuditTrail.h"

//---------------------------------------------------------------------------

class TfrmSessionHistory : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *grid;
	TMonthCalendar *mcEndDate;
	TMonthCalendar *mcStartDate;
	TLabel *Label1;
	TLabel *Label2;
	void __fastcall mcStartDateEnter(TObject *Sender);
	void __fastcall mcEndDateEnter(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall mcDateClick(TObject *Sender);
	void __fastcall gridClick(TObject *Sender);
	void __fastcall FormHide(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);

private:	// User declarations

	enum { PROCESS, OPERATOR, MACHINE, START, FINISH, NUM_COLS };

	typedef std::vector< LCDbAuditTrail::Process > ProcessList;
	ProcessList sessions;

	void listSessions();
	void setCount( int rows );

public:		// User declarations

	static TForm * details;

	__fastcall TfrmSessionHistory(TComponent* Owner);
	std::pair< TDateTime, TDateTime > getSelected();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSessionHistory *frmSessionHistory;
//---------------------------------------------------------------------------
#endif
