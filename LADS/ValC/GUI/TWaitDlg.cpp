//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "TWaitDlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TWaitDlg *WaitDlg;
//---------------------------------------------------------------------------
__fastcall TWaitDlg::TWaitDlg(
	TComponent* Owner,
	HANDLE signal,
	unsigned long beforeWaitMillis,
	unsigned long waitMillis,
	const std::string& msg
	)
	: TForm(Owner),
	m_signal( signal ),
	m_millis( waitMillis )
{
	message->Text = msg.c_str();
	delayBeforeWait->Interval = beforeWaitMillis;
	delayBeforeWait->Enabled = true;
}
//---------------------------------------------------------------------------
void __fastcall TWaitDlg::waitForSignal(TObject *Sender)
{
	delayBeforeWait->Enabled = false;

	unsigned long waitResult = WaitForSingleObject( m_signal, m_millis );

	ModalResult = ( waitResult == WAIT_OBJECT_0 ? mrOk : mrCancel );
}
//---------------------------------------------------------------------------
