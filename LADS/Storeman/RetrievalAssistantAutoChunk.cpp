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
    //int box_size = editDestBoxSize->Text.ToIntDef(0); // Calculate slot/box (where c_box_size.box_size_cid = box_content.box_size_cid
    int box_size = 123;
        // now, as retrieval lists will always specify destination boxes, chunk size can be based on the number of cryovials allocated to each box
        // where does box_content come from?
    int section_size = box_size * 2;
    while (section_size <= editMaxSize->Text.ToIntDef(0)) {
        comboSectionSize->Items->Add(String(section_size));
        section_size += box_size;
    }
    comboSectionSize->ItemIndex = comboSectionSize->Items->Count-1;
}

void __fastcall TfrmAutoChunk::FormShow(TObject *Sender) {
/*
box_content.box_type_cid
18  EDTA_1(UK)  HPS2-THRIVE EDTA 1 UK samples
c_box_size.box_type_cid
Display the size of the job and ask user if they want to divide up the list.  If they do:
1.	Ask them the maximum section size (default = 500 cryovials)
2.	Calculate slot/box (where `c_box_size.box_size_cid = box_content.box_size_cid`)
3.	Ask them to select the size of first section from a list – it must be a multiple of the box size (from 2) and no more than the maximum (from 1)
4.	Allocate the appropriate number of destination boxes to the first section
5.	Repeat steps (2) and (3) until every entry has been allocated to a section
*/
    timerCalculate->Enabled = false;
    calcSizes();
}

void __fastcall TfrmAutoChunk::btnAddChunkClick(TObject *Sender) {
    frmSamples->addChunk();
}


