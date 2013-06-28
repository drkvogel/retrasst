//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddQCMaterial1.h"
#include "QCMaterialClass.h"
#include "QCMaterial1.h"
#include "WizardLogin1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAddQCMaterial *AddQCMaterial;
//---------------------------------------------------------------------------
__fastcall TAddQCMaterial::TAddQCMaterial(TComponent* Owner)
    : TForm(Owner)
{
    theMaterial = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TAddQCMaterial::FormActivate(TObject *Sender)
{
    Caption = WizardLogin -> setCaption( Function + " QC Material" );
    if ( Function == "Add" )
    {
        edtMaterial->Text = "";
        edtDescrip->Text = "";
        AddQCMaterial->edtMaterial->Enabled = true;
    }
    else
    {
        edtMaterial->Text = theMaterial->GetName();
        edtDescrip->Text = theMaterial->GetDescription();
        AddQCMaterial->edtMaterial->Enabled = false;
    }
}
//---------------------------------------------------------------------------
void __fastcall TAddQCMaterial::btnSaveClick(TObject *Sender)
{
    bool saved = false;

    theMaterial->SetName(edtMaterial->Text);
    theMaterial->SetDescription(edtDescrip->Text);
    if ( Function == "Add" )
    {
        // check that this Material Code does not already exist
        if ( QCMaterial->FindIndexOfQCMaterial( edtMaterial->Text ) != -1 )
        {
            AnsiString strMat = "This material code (" +
                edtMaterial->Text + ") already exists.";
            Application->MessageBox(
                strMat.c_str(),
                "QCMaterial Message.", MB_OK);
        }
        else
        {
            theMaterial->InsertToDatabase(QueryCentral);
            saved = true;
        }
    }
    else
    {
        theMaterial->UpdateToDatabase(QueryCentral);
        saved = true;
    }
    if ( saved )
        this->Close();
}
//---------------------------------------------------------------------------
void __fastcall TAddQCMaterial::btnCancelClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------
