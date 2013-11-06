//---------------------------------------------------------------------------

#ifndef resultH
#define resultH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class Tfm_result : public TForm
{
__published:	// IDE-managed Components
	TPanel *panel_outcome;
	TTimer *timer_result;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall timer_resultTimer(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall Tfm_result(TComponent* Owner);
	void 	display( bool matched );
};
//---------------------------------------------------------------------------
extern PACKAGE Tfm_result *fm_result;
//---------------------------------------------------------------------------
#endif

