#include <vcl.h>
#pragma hdrstop
#include "RetrievalAssistantAutoChunk.h"
#include "RetrievalAssistantSamples.h"
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAutoChunk *frmAutoChunk;

__fastcall TfrmAutoChunk::TfrmAutoChunk(TComponent* Owner) : TForm(Owner) {
    box_size = DEFAULT_BOX_SIZE;
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
    editDestBoxSize->Text = box_size;
    //timerCalculate->Enabled = false;
    //calcSizes();
}

void __fastcall TfrmAutoChunk::btnCancelClick(TObject *Sender) { Close(); }

void __fastcall TfrmAutoChunk::btnOKClick(TObject *Sender) { Close(); }

void __fastcall TfrmAutoChunk::editMaxSizeChange(TObject *Sender) { // or dest box size
    timerCalculate->Enabled = true;
}

void __fastcall TfrmAutoChunk::timerCalculateTimer(TObject *Sender) {
    timerCalculate->Enabled = false;
    box_size = editDestBoxSize->Text.ToIntDef(0);
    calcSizes();
}

void TfrmAutoChunk::setBoxSize(int size) {
    box_size = size;
    //editDestBoxSize->Text = box_size = size;
    //editDestBoxSize->Text = size;
    //Paint();
}

void TfrmAutoChunk::calcSizes() {
/** calculate possible chunk (section) sizes
slot/box (where c_box_size.box_size_cid = box_content.box_size_cid) (where does box_content come from?)
As retrieval lists will always specify destination boxes, chunk size can be based on the number of cryovials allocated to each box */
    comboSectionSize->Clear();
    int possibleChunkSize = box_size; // * 2; // smallest chunk
    while (possibleChunkSize <= editMaxSize->Text.ToIntDef(0)) {
        comboSectionSize->Items->Add(String(possibleChunkSize));
        possibleChunkSize += box_size;
    }
    comboSectionSize->ItemIndex = comboSectionSize->Items->Count-1;
}

void __fastcall TfrmAutoChunk::btnAddChunkClick(TObject *Sender) {
    int selectedChunkSize = comboSectionSize->Items->Strings[comboSectionSize->ItemIndex].ToIntDef(0);
    if (frmSamples->addChunk(selectedChunkSize)) {
        frmSamples->showChunks();
    } else {
        msgbox("Chosen chunk size is too big for current list");
    }
    //this->Focused = true;
}

void __fastcall TfrmAutoChunk::btnAddAllChunksClick(TObject *Sender) {
    Screen->Cursor = crSQLWait; Enabled = false;
    int selectedChunkSize = comboSectionSize->Items->Strings[comboSectionSize->ItemIndex].ToIntDef(0);
    //int numChunks = frmSamples->vials.size() % selectedChunkSize;
    float result = float(frmSamples->vials.size()) / float(selectedChunkSize);
    int numChunks = ceil(result);
    //int numChunks = ceil(frmSamples->vials.size() / selectedChunkSize);
    for (int i=0; i < numChunks; i++) {
        frmSamples->showChunks();
        if (!frmSamples->addChunk(selectedChunkSize))
            break;
    }
    frmSamples->showChunks();
    Screen->Cursor = crDefault; Enabled = true;
    Close();
}


