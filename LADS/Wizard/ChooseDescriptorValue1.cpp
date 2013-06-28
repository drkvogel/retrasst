//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "ChooseDescriptorValue1.h"
#include "DescriptorClass.h"
#include "ValueClass.h"
#include "Descriptor1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TChooseDescriptorValue *ChooseDescriptorValue;
//---------------------------------------------------------------------------
__fastcall TChooseDescriptorValue::TChooseDescriptorValue(TComponent* Owner)
    : TForm(Owner),descriptorIndex(-1)
{
}
//---------------------------------------------------------------------------
void __fastcall TChooseDescriptorValue::FormActivate(TObject *Sender)
{
//    fill list box with values of desired descriptor

    lbxDescriptorValue->Clear();
    if ( descriptorIndex < 0 )
    {
        Application->MessageBox(
            "descriptorIndex has not been set\n"
            "See Core Programmer.",
            "ChooseDescriptorValue Error", MB_OK);
        return;
    }

    lblDescriptor->Caption =
        Descriptor->theDescriptors[descriptorIndex]->GetName();
    if ( strMap == "" )
    {
        memMapInfo->Text = "";
    }
    else
    {
        memMapInfo->Text = "Profile Map = " + strMap;
    }

    int numItems = 0;
    for ( int value = 0;
        value<Descriptor->theDescriptors[descriptorIndex]->GetNumValues();
        value++ )
    {
        lbxDescriptorValue->Items->Add(
            Descriptor->theDescriptors[descriptorIndex]->GetValue(value)->
            GetName() );
        numItems++;
    }
    lbxDescriptorValue->Height = (( lbxDescriptorValue->ItemHeight + 1 ) *
        numItems ) + 2;
}
//---------------------------------------------------------------------------
void __fastcall TChooseDescriptorValue::OKClick(TObject *Sender)
{
    if ( lbxDescriptorValue->ItemIndex >= 0 )
    {
        AnsiString value =
            lbxDescriptorValue->Items->Strings[lbxDescriptorValue->ItemIndex];
        ValueClass* theValue = Descriptor->theDescriptors[descriptorIndex]->
            GetValue( value );
        if ( theValue != NULL )
        {
            valueID = theValue->GetValueID();
            Close();
        }
        else
        {
            Application->MessageBox(
                "Descriptor value problem\n"
                "See Core Programmer.",
                "ChooseDescriptorValue Error", MB_OK);
        }
    }
    else
    {
        Application->MessageBox(
            "No value chosen.",
            "ChooseDescriptorValue Error", MB_OK);
    }
}
//---------------------------------------------------------------------------
void __fastcall TChooseDescriptorValue::CancelClick(TObject *Sender)
{
    valueID = -1;
    Close();
}
//---------------------------------------------------------------------------
