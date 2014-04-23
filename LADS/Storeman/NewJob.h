//---------------------------------------------------------------------------

#ifndef NewJobH
#define NewJobH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <memory>
#include "LCDbJob.h"

class Box;

//---------------------------------------------------------------------------
class TfrmNewJob : public TForm
{
__published:	// IDE-managed Components

	TButton *BtnOK;
	TEdit *TxtFull;
	TLabel *LblFull;
	TEdit *TxtName;
	TLabel *LblName;
	TLabel *LblWhy;
	TComboBox *CbExercise;
	TButton *BtnNewEx;
	TLabel *LblReason;
	TEdit *TxtReason;

	void __fastcall SaveClick(TObject *Sender);
	void __fastcall BtnNewExClick(TObject *Sender);

private:	// User declarations

	LCDbCryoJob job;

public:		// User declarations

	__fastcall TfrmNewJob( TComponent* Owner );
	void init( LCDbCryoJob::JobKind kind );
	bool createJob( const std::vector<Box*> & boxes );
	bool createJob( int projID, int al1, int al2 );
	const LCDbCryoJob & getDetails() const { return job; }
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmNewJob *frmNewJob;
//---------------------------------------------------------------------------
#endif
