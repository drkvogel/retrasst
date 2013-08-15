#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistantAutoChunk.h"
#include "RetrievalAssistantSamples.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAutoChunk *frmAutoChunk;

__fastcall TfrmAutoChunk::TfrmAutoChunk(TComponent* Owner) : TForm(Owner) { }

void __fastcall TfrmAutoChunk::btnCancelClick(TObject *Sender) { Close(); }

void __fastcall TfrmAutoChunk::btnOKClick(TObject *Sender) { Close(); }

void __fastcall TfrmAutoChunk::editMaxSizeChange(TObject *Sender) {
    timerCalculate->Enabled = true;
}

void __fastcall TfrmAutoChunk::timerCalculateTimer(TObject *Sender) {
    timerCalculate->Enabled = false;
    calcSizes();
}

void TfrmAutoChunk::calcSizes() { // calculate section sizes
    comboSectionSize->Clear();
    int box_size = editDestBoxSize->Text.ToIntDef(0); // Calculate slot/box (where c_box_size.box_size_cid = box_content.box_size_cid
        // where does box_content come from?
    int section_size = box_size * 2;
    while (section_size <= editMaxSize->Text.ToIntDef(0)) {
        comboSectionSize->Items->Add(String(section_size));
        section_size += box_size;
    }
    comboSectionSize->ItemIndex = comboSectionSize->Items->Count-1;
}

void __fastcall TfrmAutoChunk::FormShow(TObject *Sender) {
    timerCalculate->Enabled = false;
    calcSizes();
}

void __fastcall TfrmAutoChunk::btnAddChunkClick(TObject *Sender) {
    frmSamples->addChunk();
}

