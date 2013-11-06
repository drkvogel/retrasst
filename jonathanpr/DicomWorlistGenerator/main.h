//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include "workListGenerator.h"
//---------------------------------------------------------------------------
class TDicomWorkListGenerator : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button2;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
	workListGenerator *m_wlg;
public:		// User declarations
	__fastcall TDicomWorkListGenerator(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDicomWorkListGenerator *DicomWorkListGenerator;
//---------------------------------------------------------------------------
#endif
