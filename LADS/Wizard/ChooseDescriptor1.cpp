//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ChooseDescriptor1.h"
#include "Descriptor1.h"
#include "AddValue1.h"
#include "DescriptorClass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TChooseDescriptor *ChooseDescriptor;
//---------------------------------------------------------------------------
__fastcall TChooseDescriptor::TChooseDescriptor(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TChooseDescriptor::OKClick(TObject *Sender)
{
    if ( ChooseDescriptorBox->ItemIndex >= 0 )
    {
        AnsiString descriptor =
            ChooseDescriptorBox->Items->
                Strings[ChooseDescriptorBox->ItemIndex];
        descripIndex = Descriptor->GetDescriptorIndexFromName(
            descriptor);
        Close();
    }
    else
    {
        Application->MessageBox(
            "No descriptor chosen",
            "Choose Descriptor Message",MB_OK);
    }
}
//---------------------------------------------------------------------------
void __fastcall TChooseDescriptor::CancelClick(TObject *Sender)
{
    descripIndex = -1;
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TChooseDescriptor::FormActivate(TObject *Sender)
{
//    fill list box with names of descriptors
//    do not include descriptors of source `cust_out'

    int numDescriptors = Descriptor->GetNumDescriptors();
    DescriptorClass** theDescriptors = Descriptor->GetDescriptors();

    ChooseDescriptorBox->Clear();
    int numItems = 0;
    for ( int descrip = 0; descrip < numDescriptors; descrip++ )
    {
        if ( theDescriptors[descrip]->GetDescriptorSource() != "cust_out" )
        {
            ChooseDescriptorBox->Items->Add(theDescriptors[descrip]->GetName());
            numItems++;
        }
    }
    ChooseDescriptorBox->Height = (( ChooseDescriptorBox->ItemHeight + 1 ) *
        numItems ) + 2;
}
//---------------------------------------------------------------------------
