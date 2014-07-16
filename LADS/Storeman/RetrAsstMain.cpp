//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RetrAsstMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RetrievalListBase"
#pragma resource "*.dfm"
TfrmRetrievalAssistant *frmRetrievalAssistant;
//---------------------------------------------------------------------------
__fastcall TfrmRetrievalAssistant::TfrmRetrievalAssistant(TComponent* Owner)
    : TfrmRetrievalJobList(Owner)
{
}
//---------------------------------------------------------------------------
