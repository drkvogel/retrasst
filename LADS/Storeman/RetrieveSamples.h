//---------------------------------------------------------------------------

#ifndef RetrieveSamplesH
#define RetrieveSamplesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>

class LCDbProject;

//---------------------------------------------------------------------------
class TfrmRetrieved : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *grdSamples;
	TLabel *LblCaption;
	TButton *btnOK;
private:	// User declarations
//	Projects projs;
//	Project* proj;

public:		// User declarations
	__fastcall TfrmRetrieved(TComponent* Owner);

	void init( const LCDbProject* p_proj );
	void clearGrid();
	void Retrieve();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRetrieved *frmRetrieved;
//---------------------------------------------------------------------------





#endif
