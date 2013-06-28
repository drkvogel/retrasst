//---------------------------------------------------------------------------
#ifndef ProfileMap1H
#define ProfileMap1H
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

class TProfileMap : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel3;
	TButton *btnClose;
	TPanel *Panel2;
	TLabel *Label1;
	TButton *ChangeProfile;
	TButton *DeleteRow;
	TListBox *NoProfilesBox;
	TLabel *Label2;
	TButton *btnInsertValues;
	TButton *btnAddRowNew;
	TQuery *QueryProject;
	TButton *btnClearValues;
	TStringGrid *ProfileMapTable;

	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall ChangeProfileClick(TObject *Sender);
	void __fastcall DeleteRowClick(TObject *Sender);
	void __fastcall ProfileMapTableMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnInsertValuesClick(TObject *Sender);
	void __fastcall btnAddRowNewClick(TObject *Sender);
	void __fastcall btnClearValuesClick(TObject *Sender);
	void __fastcall ProfileMapTableDrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);

private:	// User declarations

	std::set< int > selected;
	enum { PM_PROFILE_MAP_ID, PM_TEST_PROFILE_ID, PM_VALUE_COLS };

	void SetupNoProfilesBox();
	void SetupGrid();
	void CopyFromClassToGrid();
	void DeleteProfileMapRow( int row );
	bool noDuplicateMapEntries();
	bool profileAssigned();

public:		// User declarations

	__fastcall TProfileMap(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TProfileMap *ProfileMap;
//---------------------------------------------------------------------------
#endif
