#ifndef TfrmAboutBoxH
#define TfrmAboutBoxH
//----------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Imaging.PngLang.hpp>

//----------------------------------------------------------------------------

class TfrmAboutBox : public TForm
{
__published:

	TPanel *Panel1;
	TImage *ProgramIcon;
	TLabel *ProductName;
	TLabel *Version;
	TLabel *Copyright;
	TLabel *Comments;
	TLabel *Memory;
	TBitBtn *okButton;
	
    void __fastcall FormShow(TObject *Sender);

private:

	static const char *author, *extension, *compiled;		// defined elsewhere


public:

	virtual __fastcall TfrmAboutBox(TComponent* AOwner);
};

//----------------------------------------------------------------------------
extern PACKAGE TfrmAboutBox *frmAboutBox;
//----------------------------------------------------------------------------
#endif
