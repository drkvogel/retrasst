//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "TSampleRunFrame.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TSampleRunFrame *SampleRunFrame;
//---------------------------------------------------------------------------
__fastcall TSampleRunFrame::TSampleRunFrame(TComponent* Owner)
	: TFrame(Owner)
{
}
//---------------------------------------------------------------------------
