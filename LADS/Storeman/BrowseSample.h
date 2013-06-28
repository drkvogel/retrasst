//---------------------------------------------------------------------------

#ifndef BrowseSampleH
#define BrowseSampleH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>

#include "inventory.h"
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <ImgList.hpp>
#include <Vcl.Imaging.pngimage.hpp>

//---------------------------------------------------------------------------

class TfrmBrowse : public TForm
{
__published:	// IDE-managed Components

	TTreeView *SampleTree;
	TStringGrid *grdProps;
	TLabel *LblProps;
	TButton *BtnMove;
	TPanel *Panel2;
	TLabel *LblLegend;
	TImage *ImgOffline;
	TLabel *LblNone;
	TImage *ImgOnline;
	TLabel *LblOnline;
	TButton *BtnFind;
	TImageList *ImageList1;
	TLabel *Label1;
	TLabel *Label3;
	TImage *Image3;
	TImage *Image1;
	TLabel *Label2;
	TImage *Image2;
	void __fastcall TreeClick(TObject *Sender);
	void __fastcall MoveClick(TObject *Sender);
	void __fastcall OnGetImage(TObject *Sender, TTreeNode *Node);
	void __fastcall OnSelectedImage(TObject *Sender, TTreeNode *Node);
	void __fastcall BtnFindClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);


private:	// User declarations

	void ShowMoveButton(IPart* data);
	void expandSubTree( TTreeNode* parent, const std::string & selection );

	IPart* sites;
	bool browse_only;

public:		// User declarations

	__fastcall TfrmBrowse(TComponent* Owner);
	void init( IPart * data, bool browse_only );
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmBrowse *frmBrowse;
//---------------------------------------------------------------------------
#endif
