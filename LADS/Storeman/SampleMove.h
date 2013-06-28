//---------------------------------------------------------------------------

#ifndef SampleMoveH
#define SampleMoveH

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ImgList.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <string>
#include <vector>
#include <stdio.h>

#include "inventory.h"
#include "LCDbJob.h"
#include "StoreUtil.h"

//---------------------------------------------------------------------------
class TfrmMove : public TForm
{
__published:	// IDE-managed Components

	TTreeView *AvlTree;
	TTreeView *SampleTree;
	TButton *BtnAssign;
	TButton *BtnUnassign;
	TImageList *ImageList1;
	TStringGrid *grdLeftProps;
	TLabel *LblLeftCap;
	TStringGrid *grdRightProps;
	TLabel *LblRightCap;
	TPanel *Panel2;
	TLabel *LblLegend;
	TImage *ImgOffline;
	TLabel *LblNone;
	TImage *ImgDemo;
	TLabel *LblDemo;
	TButton *BtnCreate;
	TButton *BtnDone;
	TButton *BtnRevert;
	TImage *ImgDone;
	TLabel *LblDone;
	TImage *Imgunavail;
	TLabel *LblUnavail;
	TButton *BtnSignOff;
	TProgressBar *progress;
	TImage *Image1;
	TLabel *Label1;
	TLabel *Label2;
	TImage *Image2;
	TImage *Image3;
	TLabel *Label3;

	void __fastcall AssignClicked(TObject *Sender);
	void __fastcall UnassignClicked(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CreateClick(TObject *Sender);
	void __fastcall DoneClick(TObject *Sender);
	void __fastcall SignOffClick(TObject *Sender);
	void __fastcall RevertClick(TObject *Sender);
	void __fastcall SampleTreeGetImageIndex(TObject *Sender, TTreeNode *Node);
	void __fastcall SampleTreeGetSelectedIndex(TObject *Sender, TTreeNode *Node);
	void __fastcall AvlTreeGetSelectedIndex(TObject *Sender, TTreeNode *Node);
	void __fastcall AvlTreeGetImageIndex(TObject *Sender, TTreeNode *Node);
	void __fastcall SampleTreeClick(TObject *Sender);
	void __fastcall AvlTreeClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);

private:	// User declarations

	IPart* part, *root;
	LCDbCryoJob job;
	std::vector<Box*> leftKids, rightKids;
	bool createNewJob;
	TTreeNode * current;
	LQuery jobQuery;

	void initDisplay( const std::string & title );
	bool canAssign();
	bool mapBox( Box * oldBox, Box * newSlot );
//	bool markPart( IPart* part );
	void deleteNodes( TTreeView* tree, TTreeNode* parent );
	void mapChildren();
	void mapChildrenByJob();
	void unmapChildren( IPart* left );
	void selectChildren( TTreeView* tree, TTreeNode* parent );
	void showDetails( TTreeView* tree, TStringGrid * grid, bool expand );
	void updateDisplay();
	void LoadTree( TTreeView* tree, TTreeNode* parent, IPart* data );
	int getImageIndex( IPart *data );
	int getImageIndex( TTreeNode *Node );
	bool anyBoxes( IPart* parent, Util::IconType type );
	bool anyBoxes( TTreeNode* parent, Util::IconType type );
	bool allBoxes( IPart* parent, Util::IconType type );
	bool allBoxes( TTreeNode* parent, Util::IconType type );
	void listCurrentBoxes( bool (*boxfn)(Box* b) );
	void makeBoxList( IPart* item, std::vector<Box*>& boxes, bool (*Boxfn)(Box* b) );
	std::string getFullPartName( IPart* part );
	void checkButtons();
	bool SignOff();
	void SetStatus( std::vector<Box*>& boxes, short status );
	TTreeNode * getFirstRack( TTreeView * view );

	static bool isUnmappedBox( Box* b );
	static bool isAnyBox( Box* b ){ return true; }
	static bool isAssignedBox( Box* b );
	static bool isDoneBox( Box* b );

public:		// User declarations

	__fastcall TfrmMove(TComponent* Owner);
	bool init( IPart* part );
	bool init( const LCDbCryoJob * p_job );
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMove *frmMove;
//---------------------------------------------------------------------------
#endif
