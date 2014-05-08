//---------------------------------------------------------------------------

#ifndef TWaitDlgH
#define TWaitDlgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
//---------------------------------------------------------------------------
#include <string>
class TWaitDlg : public TForm
{
__published:	// IDE-managed Components
	TLabel *message;
	TTimer *delayBeforeWait;
	void __fastcall waitForSignal(TObject *Sender);
private:	// User declarations
	HANDLE m_signal;
	unsigned long m_millis;
public:		// User declarations
	__fastcall TWaitDlg(
		TComponent* Owner,
		HANDLE signal,
		unsigned long beforeWaitMillis,
		unsigned long waitMillis,
		const std::string& msg );
};
//---------------------------------------------------------------------------
extern PACKAGE TWaitDlg *WaitDlg;
//---------------------------------------------------------------------------
#endif
