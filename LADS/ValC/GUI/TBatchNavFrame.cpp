//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "TBatchNavFrame.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TBatchNavFrame *BatchNavFrame;
//---------------------------------------------------------------------------
__fastcall TBatchNavFrame::TBatchNavFrame(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
