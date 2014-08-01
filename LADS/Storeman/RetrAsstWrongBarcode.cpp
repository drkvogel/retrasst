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
    previous = "";
    refresh();
}
void __fastcall TfrmWrongBarcode::editBarcodeKeyUp(TObject *Sender, WORD &Key, TShiftState Shift) {
    if (VK_RETURN == Key) { accept(); }
}

void __fastcall TfrmWrongBarcode::btnAcceptClick(TObject *Sender) {
     accept();
}

void TfrmWrongBarcode::refresh() { // poor man's MVC
    labelEntered->Caption = entered;
    labelPrevious->Caption = previous;
    labelExpected->Caption = expected;
}

void TfrmWrongBarcode::accept() {

    previous = entered;
    entered = editBarcode->Text;

    if (0 == entered.Compare(expected)) {           // Now correct
        TfrmRetrievalAssistant::msgbox("Barcode now matches expected barcode");
        frmRetrAsstCollectSamples->accept(expected); // infinite loop?: duplicate code or factor out of main form
        Close();
    } else if (0 == previous.Compare(entered)) {   // Wrong and same
        TfrmRetrievalAssistant::msgbox("Barcode does not match again - you may press Replace to accept it");
        btnReplace->Visible = true;
        // continue
    } else if (0 != previous.Compare(expected) && 0 != previous.Compare(entered)) { // Wrong and different
        TfrmRetrievalAssistant::msgbox("Barcode does not match last barcode entered or expected barcode");
        // continue
    } else {
        TfrmRetrievalAssistant::msgbox("Huh?", "Huh!");
        //using TfrmRetrievalAssistant::msgbox;
        //msgbox("Huh?", "Huh!", MB_OK);
        throw runtime_error("huh?");
    }
    refresh();
    editBarcode->Text = "";

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
}

// [LLVMbugs] [Bug 14482] New: "using declaration requires a qualified name" diag should have a fixit to insert the right qualifier if it's unique
// http://lists.cs.uiuc.edu/pipermail/llvmbugs/2012-December/026136.html

/*
class A {
 protected:
  void f() {}
};

class B : public A {
 public:
  using f;
};

int main() {
  B b;
  b.f();
}
*/

void __fastcall TfrmWrongBarcode::btnDeferClick(TObject *Sender) {
    // Mark DEFFERED, Next row
    //TfrmRetrievalAssistant::msgbox("Skip");
    frmRetrAsstCollectSamples->skip();
    Close();
}

void __fastcall TfrmWrongBarcode::btnAddNoteClick(TObject *Sender) {
    // fixme
    Application->MessageBox(L"Todo: add a note", L"Huh!", MB_OK);
}


void __fastcall TfrmWrongBarcode::btnReplaceClick(TObject *Sender) {
    // replace
    TfrmRetrievalAssistant::msgbox("Todo: replace expected sample with found one");
    TfrmRetrievalAssistant::msgbox("Todo: And go to next row - replace() should be a method in TfrmRetrAsstCollectSamples");
    frmRetrAsstCollectSamples->replace(entered); // entered rather than editbox text in case it changed

    // in lcr

    // in cryovial store
    //frmRetrAsstCollectSamples->nextRow();
    Close();
}

