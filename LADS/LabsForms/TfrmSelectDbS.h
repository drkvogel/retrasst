//----------------------------------------------------------------------------

#ifndef TfrmSelectDbSH
#define TfrmSelectDbSH
#include <System.Classes.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>

//----------------------------------------------------------------------------

class TfrmSelectDbSystem : public TForm
{
__published:
	TBitBtn *BitBtn1;
	TRadioGroup *rgDatabase;
	TBitBtn *BitBtn2;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);

public:

	virtual __fastcall TfrmSelectDbSystem(TComponent* AOwner);
};

//----------------------------------------------------------------------------
extern PACKAGE TfrmSelectDbSystem *frmSelectDbSystem;
//----------------------------------------------------------------------------
#endif    
