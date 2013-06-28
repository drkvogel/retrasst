//---------------------------------------------------------------------------

#ifndef SampleEntryH
#define SampleEntryH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include "inventory.h"
#include <ExtCtrls.hpp>

//---------------------------------------------------------------------------
class TfrmRetrieveMain : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *grdSamples;
	TLabel *LblCaption;
	TLabel *LblSample;
	TEdit *TxtBarcode;
	TLabel *LblAliquot1;
	TButton *BtnAdd;
	TButton *BtnRetrieve;
	TComboBox *CmbAliquot1;
	TRadioGroup *RadIDType;
	TComboBox *CmbAliquot2;
	TLabel *LblAliquot2;
	void __fastcall AddClick(TObject *Sender);
	void __fastcall Retrieve(TObject *Sender);
private:	// User declarations

//	Projects projs;
//	std::vector<AliquotType*> typelist;
//	Project* proj;
	int selectedProject;

public:		// User declarations
//	AliquotTypes ats;
	struct GridEntry
	{
		std::string sid;
		std::string cid;
		std::string aid;
		std::string adesc;
		int pid;
		GridEntry( std::string p_sid, std::string p_cid,
			std::string p_aid, std::string p_adesc, int p_pid ) :
			sid( p_sid ), cid( p_cid ), aid( p_aid ), adesc( p_adesc ), pid( p_pid ) {};
	};

	std::vector<GridEntry> rows;
	std::vector<IPart*> slist;

	__fastcall TfrmRetrieveMain(TComponent* Owner);
	void init();
	void clearGrid();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRetrieveMain *frmRetrieveMain;
//---------------------------------------------------------------------------
#endif
