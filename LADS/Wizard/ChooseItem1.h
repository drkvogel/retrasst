//---------------------------------------------------------------------------
#ifndef ChooseItem1H
#define ChooseItem1H

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ColourStringGrid.h"
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <set>

typedef std::set<AnsiString> StrSet;

//---------------------------------------------------------------------------

class TChooseItem : public TForm
{
__published:	// IDE-managed Components

	TColourStringGrid *ItemGrid;
	TPanel *Panel1;
	TButton *btnClose;
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall ItemGridMouseDown(TObject *Sender,
		  TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall FormActivate(TObject *Sender);

private:	// User declarations

	AnsiString ItemType;
	StrSet items;

	void RedrawGrid();

public:		// User declarations

	__fastcall TChooseItem(TComponent* Owner);
	
	void SetItemType( AnsiString type );
	void AddItem( AnsiString name ) { items.insert( name ); }
	int GetNumItems() { return items.size(); }
	StrSet GetSelectedItems();
};
//---------------------------------------------------------------------------
extern PACKAGE TChooseItem *ChooseItem;
//---------------------------------------------------------------------------
#endif
