//---------------------------------------------------------------------------
#ifndef AddProfileMap1H
#define AddProfileMap1H
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "ColourStringGrid.h"

//---------------------------------------------------------------------------

class TAddProfileMap : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel3;
	TButton *btnAdd;
	TColourStringGrid *AddProfileMapTable;
	TButton *Cancel;
	TPanel *Panel1;
	TLabel *Label1;
	TQuery *QueryProject;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall btnAddClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);

	void __fastcall AddProfileMapTableMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations

	void CreateClassInstance( int instance );
	bool CheckProfileMapExists( int profile );
	bool CreateProfileMapClass();
	void SortProfileMapsByDescriptor();
	void DeleteProfileMapClass();
	int numProfileMaps;

public:		// User declarations
	__fastcall TAddProfileMap(TComponent* Owner);
	bool PutProfileMapToGrid();
    bool CopyFromClassToGrid();
    void CopyFromClassItemToGrid( int row, int profile );
};
//---------------------------------------------------------------------------
extern PACKAGE TAddProfileMap *AddProfileMap;
//---------------------------------------------------------------------------
#endif
