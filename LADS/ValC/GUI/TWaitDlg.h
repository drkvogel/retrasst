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
private:	// User declarations
public:		// User declarations
	__fastcall TWaitDlg( TComponent* Owner );
	void __fastcall show( const std::string& msg );
};
//---------------------------------------------------------------------------
extern PACKAGE TWaitDlg *WaitDlg;
//---------------------------------------------------------------------------
#endif
