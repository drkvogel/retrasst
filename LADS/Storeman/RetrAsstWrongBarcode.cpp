#include <vcl.h>
#pragma hdrstop
#include "RetrAsstWrongBarcode.h"
#include "RetrAsstCollectSamples.h"
#include "RetrAsstAddNote.h"
#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmWrongBarcode *frmWrongBarcode;

/**
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
__fastcall TfrmWrongBarcode::TfrmWrongBarcode(TComponent* Owner) : TForm(Owner) { }


void __fastcall TfrmWrongBarcode::FormShow(TObject *Sender) {
    btnReplace->Visible = false;
    labelReason->Caption = "";
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
    labelEntered->Caption  = entered.c_str();
    labelPrevious->Caption = previous.c_str();
    labelExpected->Caption = expected.c_str();
}

void TfrmWrongBarcode::accept() {

    previous = entered;
    entered = AnsiString(editBarcode->Text).c_str();

    //if (0 == entered.compare(expected)) {           // Now correct
    if (entered == expected) {           // Now correct
        //TfrmRetrievalAssistant::msgbox("Barcode now matches expected barcode");
        labelReason->Caption = "Barcode now matches expected barcode";
        //flash();
        frmRetrAsstCollectSamples->accept(expected); // infinite loop?: duplicate code or factor out of main form
        Close();
    //} else if (0 == previous.compare(entered)) {   // Wrong and same
    } else if (previous == entered) {   // Wrong and same
        //TfrmRetrievalAssistant::msgbox("Barcode does not match again - you may press Replace to accept it");
        labelReason->Caption = "Barcode does not match again - you may press Replace to accept it";
        //flash();
        btnReplace->Visible = true; // continue
    //} else if (0 != previous.compare(expected) && 0 != previous.compare(entered)) { // Wrong and different
    } else if (previous != expected && previous != entered) { // Wrong and different
        //TfrmRetrievalAssistant::msgbox("Barcode does not match last barcode entered or expected barcode"); // continue
        labelReason->Caption = "Barcode does not match last barcode entered or expected barcode";
        //flash();
    } else {
        TfrmRetrievalAssistant::msgbox("Huh?", "Huh!");
        throw runtime_error("huh?");
    }
    refresh();
    editBarcode->Text = "";
    ActiveControl = editBarcode;
}

void TfrmWrongBarcode::flash() {
    TColor other    = clBtnFace;
    TColor orig     = labelReason->Color;
    int sleepms     = RETRASSTDEBUG ? 100 : 250; // get on with it : HSE-approved

    for (int i = 0; i < 4; i++) {
        if (i % 2) {
            labelReason->Font->Color = orig;
        } else {
            labelReason->Font->Color = other;
        }
        Sleep(sleepms); // ms
        /* In the UK, the flash rate of strobe lights is restricted to a maximum of
        four flashes a second by the Health and Safety Executive. This rate is considered to be safe for most people.
        http://www.nhs.uk/ipgmedia/national/epilepsy%20action/assets/photosensitiveepilepsy.pdf */
        Repaint(); // ParentBackground propery of component must be set to false to see change in colour
    }
}

void __fastcall TfrmWrongBarcode::btnDeferClick(TObject *Sender) { // Mark DEFFERED, Next row
    frmRetrAsstCollectSamples->skip();
    Close();
}

void __fastcall TfrmWrongBarcode::btnAddNoteClick(TObject *Sender) {
    // fixme
    //Application->MessageBox(L"Todo: add a note", L"Huh!", MB_OK);
    ostringstream oss; oss<<"Wrong barcode: '"<<entered.c_str()<<"', expected: '"<<expected.c_str()<<"'";
    frmRetrAsstAddNote->setText(oss.str());
    frmRetrAsstAddNote->ShowModal();

    //LDbNote note(noteText.c_str());
    //note.saveRecord();
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

/*
//using TfrmRetrievalAssistant::msgbox;
//msgbox("Huh?", "Huh!", MB_OK);

// [LLVMbugs] [Bug 14482] New: "using declaration requires a qualified name" diag should have a fixit to insert the right qualifier if it's unique
// http://lists.cs.uiuc.edu/pipermail/llvmbugs/2012-December/026136.html

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
