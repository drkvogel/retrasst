//---------------------------------------------------------------------------

#ifndef NewSectionH
#define NewSectionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>

#include "Inventory.h"
#include <string>
//---------------------------------------------------------------------------
class TfrmNewSection : public TForm
{
__published:	// IDE-managed Components
	TButton *BtnCancel;
	TButton *BtnOK;
	TUpDown *UpDownFill;
	TEdit *TxtFill;
	TLabel *LblFill;
	TEdit *TxtPrefix;
	TLabel *LblPrefix;
	TUpDown *UpDownFirst;
	TEdit *TxtFirst;
	TLabel *LblFirst;
	TUpDown *UpDownLast;
	TEdit *TxtLast;
	TLabel *LblLast;
	TUpDown *UpDownCap;
	TEdit *TxtCap;
	TLabel *LblCap;
	void __fastcall OKClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
private:	// User declarations

	std::vector<IPart*> newSectionList;
	Section* section;

public:		// User declarations
	__fastcall TfrmNewSection(TComponent* Owner);
	void init( IPart* lay, Section* p_section );
	int getDefaultFillOrder();
	bool isFillOrderDuplicate( int order );
	bool isPrefixDuplicate( std::string prefix );
	Section* getSection(){ return section; }
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmNewSection *frmNewSection;
//---------------------------------------------------------------------------
#endif
