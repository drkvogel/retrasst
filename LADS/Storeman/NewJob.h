//---------------------------------------------------------------------------

#ifndef NewJobH
#define NewJobH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <memory>
#include "LCDbJob.h"

class Box;

//---------------------------------------------------------------------------
class TfrmNewJob : public TForm
{
__published:	// IDE-managed Components
	TEdit *TxtFull;
	TLabel *LblFull;
	TEdit *TxtName;
	TLabel *LblName;
	TLabel *LblReason;
	TComboBox *cmbReason;
	TLabel *Label1;
	TEdit *txtBoxSet;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TLabel *Label2;
	TLabel *lblExercise;
	TButton *btnAddReason;
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall btnAddReasonClick(TObject *Sender);

private:	// User declarations

	LCDbCryoJob job;
	int reasonGroup;

public:		// User declarations

	__fastcall TfrmNewJob( TComponent* Owner );
	void init( LCDbCryoJob::JobKind kind );
	void setExercise( int exID );
	bool createJob( const std::vector<Box*> & boxes );
	LCDbCryoJob * getDetails() { return &job; }
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmNewJob *frmNewJob;
//---------------------------------------------------------------------------
#endif
