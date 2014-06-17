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
	m_millis( waitMillis ),
    m_waitResult(0)
{
	message->Text = msg.c_str();
	delayBeforeWait->Interval = beforeWaitMillis;
	delayBeforeWait->Enabled = true;
}
//---------------------------------------------------------------------------
unsigned long TWaitDlg::getWaitResult() const
{
    return m_waitResult;
}
//---------------------------------------------------------------------------
void __fastcall TWaitDlg::waitForSignal(TObject *Sender)
{
	delayBeforeWait->Enabled = false;

	m_waitResult = WaitForSingleObject( m_signal, m_millis );

	ModalResult = ( m_waitResult == WAIT_OBJECT_0 ? mrOk : mrCancel );
}
//---------------------------------------------------------------------------
