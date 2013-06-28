//---------------------------------------------------------------------------
#ifndef AddProfile1H
#define AddProfile1H
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ComCtrls.hpp>
#include "LPDbProfile.h"
#include <set>

//---------------------------------------------------------------------------
class TAddProfile : public TForm
{
__published:	// IDE-managed Components

	TLabel *Label1;
	TEdit *ProfileName;
	TLabel *Label2;
	TPanel *Panel1;
	TButton *Save;
	TButton *Cancel;
	TPanel *Panel2;
	TLabel *Label3;
	TButton *Add;
	TButton *Change;
	TButton *Delete;
	TMemo *Description;
	TPageControl *pgProfile;
	TTabSheet *tabTests;
	TTabSheet *tabBoxTypes;
	TLabel *dragInstructions;
	TPanel *Panel3;
	TButton *btnClose;
	TQuery *QueryProject;
	TCheckBox *cbxActive;
	TStringGrid *grdTest;
	TStringGrid *grdBoxType;
	TCheckBox *cbxTests;
	TCheckBox *cbxSpecimen;
	TCheckBox *cbxAliquots;

	void __fastcall SaveClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall AddClick(TObject *Sender);
	void __fastcall ChangeClick(TObject *Sender);
	void __fastcall DeleteClick(TObject *Sender);
	void __fastcall grdTestMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall pgProfileChange(TObject *Sender);
	void __fastcall grdBoxTypeMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall grdBoxTypeDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall grdTestDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);

private:	// User declarations

	int profileID;
	bool NoteExists;
	bool boxRowMoved;

	std::set< int > selected;
	
	void DefineTestGrid();
	void CopyFromTestClassToGrid();
	void AddTest();
	void DeleteTest();

	void DefineBoxTypeGrid();
	void CopyFromBoxTypeClassToGrid();
	void AddBoxType();
	void DeleteBoxType();

	bool SaveProfile();
	void MoveRow( int fromRow, int toRow );

	enum { ATP_TESTID, ATP_TESTNAME, ATP_MACHINEID, ATP_MACHINENAME,
		   ATP_UPPERLIM, ATP_UPPERWARNING, ATP_LOWERWARNING, ATP_LOWERLIM,
		   ATP_TRIGGERID, ATP_TRIGGERUPPER, ATP_TRIGGERLOWER, ATP_TRIGGERNAME,
		   ATP_NUMCOLS };

	enum { BOX_BOXTYPEID, BOX_BOXNAME, BOX_NUMCOLS };

	static String display( double num );
	bool checkProfile();

public:		// User declarations

	__fastcall TAddProfile(TComponent* Owner);
	void setProfile( int profile ) { profileID = profile; }
};
//---------------------------------------------------------------------------
extern PACKAGE TAddProfile *AddProfile;
//---------------------------------------------------------------------------
#endif
