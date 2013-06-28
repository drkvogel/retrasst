//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ChooseDescriptorForSort1.h"
#include "Descriptor1.h"
#include "ProfileMap1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TChooseDescriptorForSort *ChooseDescriptorForSort;
//---------------------------------------------------------------------------
__fastcall TChooseDescriptorForSort::TChooseDescriptorForSort(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TChooseDescriptorForSort::SetDescriptors(
    int numDescrip, DescriptorClass** theDescrips )
{
    numDescriptors = numDescrip;
    theDescriptors = theDescrips;
}
//---------------------------------------------------------------------------
void __fastcall TChooseDescriptorForSort::num(TObject *Sender)
{
//    fill list box with names of descriptors

    selection = -1;
    numSelections = 0;
    maxSelections = 20;
    SortBox->Clear();
    ChooseDescriptorBox->Clear();
    int numDescriptorsListed = 0;
/*    for ( int descrip = 0; descrip < numDescriptors; descrip++ )
	{
		if ( theDescriptors[descrip]->GetNumValues() > 1 )
		{
			ChooseDescriptorBox->Items->Add(
				theDescriptors[descrip]->GetName());
			numDescriptorsListed++;
		}
	}
*/    ChooseDescriptorBox->Height = (( ChooseDescriptorBox->ItemHeight + 1 ) *
        numDescriptorsListed ) + 2;
}
//---------------------------------------------------------------------------

void TChooseDescriptorForSort::SetDefaultValues()
{
    selection = -1;
    maxSelections = 30;
    for ( int descrip = 0; descrip < numDescriptors; descrip++ )
    {
        selection++;
        if ( selection >= maxSelections )
        {
            Application->MessageBox(
                "Too many selections for sort.\n"
                "Ask programmer to increase arrays.",
                "Choose Descriptor For Sort Message",MB_OK);
        }
        else
        {
            numSelections = selection + 1;
/*            AnsiString descriptor = theDescriptors[descrip]->GetName();
			D_sortDescriptor[selection] =
				Descriptor->GetDescriptorIndexFromName( descriptor );
			PM_sortDescriptor[selection] = selection;
*/        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TChooseDescriptorForSort::CancelClick(TObject *Sender)
{
    SetDefaultValues();
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TChooseDescriptorForSort::btnSortClick(TObject *Sender)
{
    if ( ChooseDescriptorBox->ItemIndex <= -1 )
    {
        Application->MessageBox(
            "No descriptor selected.",
            "Choose Descriptor For Sort Message",MB_OK);
    }
    else
    {
        Close();
    }
}
//---------------------------------------------------------------------------

void __fastcall TChooseDescriptorForSort::ChooseDescriptorBoxMouseDown(
      TObject *Sender, TMouseButton Button, TShiftState Shift, int X,
      int Y)
{
    if ( ChooseDescriptorBox->ItemIndex <= -1 )
    {
        Application->MessageBox(
            "No descriptor selected.",
            "Choose Descriptor For Sort Message",MB_OK);
    }
    else if ( selection >= maxSelections )
    {
        Application->MessageBox(
            "Too many selections. Ask programmer to increase arrays.",
            "Choose Descriptor For Sort Message",MB_OK);
    }
    else
    {
        selection++;
        numSelections = selection + 1;
        AnsiString descriptor =
            ChooseDescriptorBox->Items->
                Strings[ChooseDescriptorBox->ItemIndex];
/*        D_sortDescriptor[selection] = Descriptor->GetDescriptorIndexFromName(
			descriptor );
		PM_sortDescriptor[selection] = ChooseDescriptorBox->ItemIndex;
*/        SortBox->Items->Add( descriptor );
//        SortBox->Height = ( ( SortBox->ItemHeight + 2 ) * numSelections ) + 2;
        SortBox->Height = ( ( SortBox->ItemHeight + 1 ) * numSelections ) + 2;
    }
}
//---------------------------------------------------------------------------


