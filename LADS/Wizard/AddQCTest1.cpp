//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "AddQCTest1.h"
#include "CentralTest1.h"
#include "QCTestClass.h"
#include "QCLotClass.h"
#include "WizardLogin1.h"
#include "LCDbTest.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAddQCTest *AddQCTest;
//---------------------------------------------------------------------------
__fastcall TAddQCTest::TAddQCTest(TComponent* Owner)
    : TForm(Owner)
{
    theQCTest = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TAddQCTest::FormActivate(TObject *Sender)
{
    Caption = WizardLogin -> setCaption( Function + " QC Test" );

    if ( Function == "Add" )
    {
        edtMean->Text = "0.0";
        edtStdDev->Text = "0.0";
        lblTest->Caption = theQCTest->GetName();
    }
    else
    {
        if ( theQCTest == NULL )
        {
            Application->MessageBox(
                "No QC test identified.\n"
                "Programming error. Contact Core Programmers.",
                "AddQCTest Error", MB_OK );
            return;
        }
        lblTest->Caption = theQCTest->GetName();
        edtMean->Text = theQCTest->GetPacketMean();
        edtStdDev->Text = theQCTest->GetPacketStdDev();
    }
    lblMaterial->Caption = theQCLot->GetQCMaterial();
    lblLevel->Caption = theQCLot->GetQCLevel();
    lblLot->Caption = theQCLot->GetName();
}
//---------------------------------------------------------------------------

void __fastcall TAddQCTest::btnSaveClick(TObject *Sender)
{
	if( Function == "Add" )
	{
		theQCTest->SetName( lblTest->Caption );
		theQCTest->SetQCID( theQCLot->GetQCID() );
		const LCDbTest * test = LCDbTests::records().findByName( lblTest->Caption );
		theQCTest->SetQCTestCID( test == NULL ? 0 : test -> getID() );
		theQCTest->SetPacketMean(edtMean->Text);
		theQCTest->SetPacketStdDev(edtStdDev->Text);
		theQCTest->InsertToDatabase(QueryCentral);
	}
	else
	{   theQCTest->SetPacketMean(edtMean->Text);
        theQCTest->SetPacketStdDev(edtStdDev->Text);
        theQCTest->UpdateToDatabase(QueryCentral);
    }
    Close();
}
//---------------------------------------------------------------------------

void __fastcall TAddQCTest::btnCancelClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

