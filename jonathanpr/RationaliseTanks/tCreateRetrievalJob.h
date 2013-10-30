//---------------------------------------------------------------------------

#ifndef tCreateRetrievalJobH
#define tCreateRetrievalJobH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <string>
#include "rationaliseTanks.h"

using namespace RationaliseTanks;

//---------------------------------------------------------------------------
class TCreateRetrievalJobForm : public TForm
{
__published:	// IDE-managed Components
	TEdit *external_nameEdit;
	TMemo *descriptionMemo;
	TButton *ButtonCancel;
	TButton *ButtonOK;
	TLabel *label;
	TLabel *Label1;
	TBalloonHint *BalloonHint1;
	void __fastcall ButtonOKClick(TObject *Sender);
	void __fastcall ButtonCancelClick(TObject *Sender);
private:	// User declarations
	rationaliseTanks *m_pRationaliseTanks;
	std::map<std::string,std::string> m_SelectBox;
public:		// User declarations
	__fastcall TCreateRetrievalJobForm(TComponent* Owner);
	void setUp(rationaliseTanks *rationaliseTanks,const std::map<std::string,std::string> &m_map);
};
//---------------------------------------------------------------------------
extern PACKAGE TCreateRetrievalJobForm *CreateRetrievalJobForm;
//---------------------------------------------------------------------------
#endif
