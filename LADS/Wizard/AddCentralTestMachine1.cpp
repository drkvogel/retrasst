//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "CentralTestMachineClass.h"
#include "AddCentralTestMachine1.h"
#include "DbShared.h"
#include "CentralTestMachine1.h"
#include "ChooseItem1.h"
#include "DataMod1.h"
#include "TestClass.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAddCentralTestMachine *AddCentralTestMachine;
//---------------------------------------------------------------------------
__fastcall TAddCentralTestMachine::TAddCentralTestMachine(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TAddCentralTestMachine::FormActivate(TObject *Sender)
{
    Caption = DataMod->SetCaption( Function + "Central Test Machine" );

    lblTest->Caption = theCentralTestMachine->GetMachineTestName();
    lblMachineName->Caption = theCentralTestMachine->GetMachineName();
    if (Function == "Add")
    {
        edtProtocol->Text = "0";
        edtTestOrder->Text = "0";
        edtMachineTestName->Text = theCentralTestMachine->GetMachineName();
    }
    else
    {
        edtMachineTestName->Text = theCentralTestMachine->GetMachineTestName();
        edtProtocol->Text =  theCentralTestMachine->GetProtocol();
        edtTestOrder->Text = theCentralTestMachine->GetTestOrder();
    }
}
//---------------------------------------------------------------------------

void __fastcall TAddCentralTestMachine::SaveClick(TObject *Sender)
{
    bool validform = true;

    if ( edtMachineTestName->Text == "" )
    {
        Application->MessageBox(
            "MachineTestName must have a value",
            "Add Central Test Machine",MB_OK);
        validform = false;
    }

    if ( edtProtocol->Text == "" )
    {
        Application->MessageBox(
            "Protocol must have a value",
            "Add Central Test Machine",MB_OK);
        validform = false;
    }

    if ( edtTestOrder->Text == "" )
    {
        Application->MessageBox(
            "Test Order must have a value",
            "Add Central Test Machine",MB_OK);
        validform = false;
    }

    if ( validform )
    {
        if (Function != "Add")
        {
            theCentralTestMachine->LogRecord( "Update: Old record: " );
        }

        theCentralTestMachine->SetMachineTestName( edtMachineTestName->Text );
        theCentralTestMachine->SetProtocol( (short) StrToInt(edtProtocol->Text) );
        theCentralTestMachine->SetTestOrder( (short) StrToInt(edtTestOrder->Text) );

        if (Function == "Add")
        {
            theCentralTestMachine->InsertToDatabase( QueryCentral );
        }
        else
        {
            theCentralTestMachine->UpdateToDatabase( QueryCentral );
        }

        AddCentralTestMachine->Close();
    }
}
//---------------------------------------------------------------------------
void __fastcall TAddCentralTestMachine::CancelClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

