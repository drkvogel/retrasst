//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Main.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------

void addRows( TListView* lv )
{
    UnicodeString rows[7] =
        {
            L"one",
            L"two",
            L"three",
            L"four",
            L"five",
            L"six",
            L""
        };

    UnicodeString cols[3] =
        {
            L"a",
            L"b",
            L""
        };

    TListItems* items = lv->Items;

    for ( UnicodeString* row = rows; row->Length(); ++row )
    {
        lv->AddItem( *row, 0 );

        TListItem* item = items->Item[ items->Count - 1 ];

        for ( UnicodeString* col = cols; col->Length(); ++col )
        {
            item->SubItems->Add( *col );
        }
    }
}

__fastcall TForm1::TForm1(TComponent* Owner)
    : TForm(Owner)
{
    addRows( ListView1 );
    addRows( ListView2 );
}


//---------------------------------------------------------------------------
