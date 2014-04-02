#include <vcl.h>
#pragma hdrstop

#include "RetrAsstCollectEmpties.h"
#include "RetrAsstCollectSamples.h"
#include "RetrAsstMain.h"
#include "LPDbBoxes.h"

#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmCollectEmpties *frmCollectEmpties;

__fastcall TfrmCollectEmpties::TfrmCollectEmpties(TComponent* Owner)
    : TForm(Owner)
{
}

