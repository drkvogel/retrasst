//---------------------------------------------------------------------------

#ifndef MoveJobsH
#define MoveJobsH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include "LCDbJob.h"

//---------------------------------------------------------------------------

class TfrmSelectJob : public TForm
{
__published:	// IDE-managed Components

	TStringGrid *grdJobs;
	TButton *btnNewJob;
	TButton *btnDone;
	TButton *btnOpenJob;
	
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnOpenJobClick(TObject *Sender);
	void __fastcall btnNewJobClick(TObject *Sender);
	void __fastcall grdJobsDblClick(TObject *Sender);

private:	// User declarations

	enum { JOB_NAME, EXERCISE, DESCRIPTION, START_DATE, NUM_COLS };

	const LCDbCryoJob *record;

	void writeJob( const LCDbCryoJob &job, int row );
	void selectJob( int index );

public:		// User declarations

	__fastcall TfrmSelectJob(TComponent* Owner);
	void initMoveJobs();
	void initRetrieval( int projectCID );
	const LCDbCryoJob * getSelected() const { return record; }
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSelectJob *frmSelectJob;
//---------------------------------------------------------------------------
#endif
