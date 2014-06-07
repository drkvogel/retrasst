//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "TSnapshotFrame.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TSnapshotFrame *SnapshotFrame;
//---------------------------------------------------------------------------
SnapshotFrameObserver::SnapshotFrameObserver()
{
}
//---------------------------------------------------------------------------
SnapshotFrameObserver::~SnapshotFrameObserver()
{
}
//---------------------------------------------------------------------------
__fastcall TSnapshotFrame::TSnapshotFrame( TComponent* Owner )
	: TFrame(Owner), m_observer(NULL)
{
}
//---------------------------------------------------------------------------
__fastcall TSnapshotFrame::~TSnapshotFrame()
{
	if (m_observer)
	{
		m_observer->notifyDestroyed( this );
	}
}
//---------------------------------------------------------------------------
void __fastcall TSnapshotFrame::ForceReloadButtonClick(TObject *Sender)
{
	if (m_observer)
	{
		m_observer->notifyForceReloadButtonClick(this);
	}
}


void __fastcall TSnapshotFrame::MainSplitterMouseUp(TObject *Sender,
													TMouseButton Button,
													TShiftState Shift,
													float X, float Y)
{
	if (m_observer)
	{
		m_observer->notifyMainSplitterMouseUp(this);
	}
}

void TSnapshotFrame::setObserver(SnapshotFrameObserver* obs)
{
	m_observer = obs;
	if (m_observer)
	{
		m_observer->notifyAssociatedWith(this);
	}
}

