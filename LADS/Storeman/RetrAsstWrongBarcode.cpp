#include <vcl.h>
#pragma hdrstop
#include "RetrAsstWrongBarcode.h"
#include "RetrAsstCollectSamples.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmWrongBarcode *frmWrongBarcode;

__fastcall TfrmWrongBarcode::TfrmWrongBarcode(TComponent* Owner) : TForm(Owner) { }

void __fastcall TfrmWrongBarcode::FormShow(TObject *Sender) {
    btnReplace->Visible = false;
}

void __fastcall TfrmWrongBarcode::btnAcceptClick(TObject *Sender) {
/*
                Enter:
                    barcode2 = entered
                    entered == expected:
                        # Now correct
                        Close, Mark COLLECTED; Next row
                    barcode2 == barcode1:
                        # Wrong and same
                        Display Replace button
                        continue
                    barcode2 != expected && barcode2 != barcode1:
                        # Wrong and different
                        barcode1 = barcode2
                        continue
*/


    //frmRetrAsstCollectSamples->accept()
}

void __fastcall TfrmWrongBarcode::btnDeferClick(TObject *Sender) {
    // Mark DEFFERED, Next row
    frmRetrAsstCollectSamples->skip();
    Close();
}

void __fastcall TfrmWrongBarcode::btnAddNoteClick(TObject *Sender) {
    // fixem
}


