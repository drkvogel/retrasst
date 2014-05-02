#include <vcl.h>
#pragma hdrstop
#include "RetrAsstCollectBoxes.h"


#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmRetrAsstCollectBoxes *frmRetrAsstCollectBoxes;

__fastcall TfrmRetrAsstCollectBoxes::TfrmRetrAsstCollectBoxes(TComponent* Owner)
    : TForm(Owner)
{
}

// box retrieval plan : Select * from c_box_retrieval b order by b.section, b.rj_box_cid
