//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "TQCViewFrame.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TQCViewFrame *QCViewFrame;
//---------------------------------------------------------------------------
__fastcall TQCViewFrame::TQCViewFrame(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
