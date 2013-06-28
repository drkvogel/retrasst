//---------------------------------------------------------------------------

#ifndef ConfigureH
#define ConfigureH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>

#include "inventory.h"
#include <ExtCtrls.hpp>
#include <ImgList.hpp>
#include <Graphics.hpp>

//---------------------------------------------------------------------------

class TfrmConfigure : public TForm
{
__published:	// IDE-managed Components

	TStringGrid *grdProps;
	TLabel *LblProps;
	TTreeView *SampleTree;
	TButton *BtnAdd;
	TButton *BtnAddSib;
	TButton *BtnModify;
	TButton *BtnOnline;
	TImageList *ImageList1;
	TPanel *Panel1;
	TLabel *Label2;
	TImage *Image2;
	TLabel *Label4;
	TImage *Image4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TImage *Image5;
	TImage *Image6;
	TLabel *Label8;
	TImage *Image7;

	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall AddSibClick(TObject *Sender);
	void __fastcall TreeClick(TObject *Sender);
	void __fastcall AddClick(TObject *Sender);
	void __fastcall OnGetImage(TObject *Sender, TTreeNode *Node);
	void __fastcall OnSelectedImage(TObject *Sender, TTreeNode *Node);
	void __fastcall ModifyClick(TObject *Sender);
	void __fastcall OnlineClick(TObject *Sender);

private:	// User declarations

	IPart* sites;

	void enableAdd(IPart::ItemType selectedType);
	Site * createSite();
	Tank * createVessel( TTreeNode * site );
	Tank * changeVessel( Tank * current, TTreeNode *parent );
	Tank * addPopulation( Tank * current, TTreeNode *parent );
	Tank * changePopulation( Layout * current, TTreeNode *parent );
	bool confirmSave( const std::string & name );

public:		// User declarations
	__fastcall TfrmConfigure(TComponent* Owner);
	void init();
	void SaveChildren( TTreeNode* parent, bool& success );
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmConfigure *frmConfigure;
//---------------------------------------------------------------------------

#endif
