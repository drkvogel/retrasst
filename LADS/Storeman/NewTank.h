//---------------------------------------------------------------------------

#ifndef NewTankH
#define NewTankH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <memory>
#include "Inventory.h"

class LCDbObject;

//---------------------------------------------------------------------------

class TfrmNewTank : public TForm
{
__published:	// IDE-managed Components
	TLabel *LblPhys;
	TLabel *LblPos;
	TEdit *TxtSrl;
	TLabel *LblSrl;
	TButton *BtnNext;
	TButton *BtnCancel;
	TEdit *TxtPos;
	TUpDown *UpDownPos;
	TLabel *LblSect;
	TComboBox *cbLayout;
	TPanel *pnlTank;
	TPanel *pnlLayout;
	TPanel *pnlConfirm;
	TStringGrid *grdProps;
	TButton *BtnEdit;
	TButton *BtnAdd;
	TLabel *LblProps;
	TUpDown *UpDownCap;
	TEdit *TxtCount;
	TLabel *LblCount;
	TEdit *TxtLayFull;
	TLabel *LblLayFull;
	TEdit *TxtLayName;
	TLabel *LblLayName;
	TStringGrid *grdProps1;
	TEdit *TxtLayName1;
	TLabel *LblLayName1;
	TEdit *TxtPop1;
	TLabel *LblCount1;
	TEdit *TxtSrl1;
	TLabel *LblSrl1;
	TEdit *TxtName1;
	TLabel *LblFull1;
	TEdit *TxtFull1;
	TLabel *LblName1;
	TButton *BtnDelete;
	TButton *BtnBack;
	TLabel *lblStoreType;
	TLabel *Label4;
	TEdit *txtStoreType1;
	TComboBox *cbStoreType;
	TButton *btnNewType;
	TButton *btnNewLayout;
	TEdit *TxtPhysical;
	TLabel *Label3;
	TLabel *Label2;
	TEdit *TxtLocation1;
	TLabel *lblPop;
	TEdit *TxtPop;
	TUpDown *UpDownPop;
	TCheckBox *chkPop;
	TLabel *Label1;
	TEdit *TxtPos1;
	TLabel *Label5;
	TEdit *TxtPhysical1;
	TComboBox *cbLocation;

	void __fastcall NextClick(TObject *Sender);
	void __fastcall CancelClick(TObject *Sender);
	void __fastcall BackClick(TObject *Sender);
	void __fastcall OnShow(TObject *Sender);
	void __fastcall AddClick(TObject *Sender);
	void __fastcall EditClick(TObject *Sender);
	void __fastcall CountChange(TObject *Sender);
	void __fastcall DeleteClick(TObject *Sender);
	void __fastcall btnNewTypeClick(TObject *Sender);
	void __fastcall btnNewLayoutClick(TObject *Sender);
	void __fastcall chkPopClick(TObject *Sender);
	void __fastcall TxtPosChange(TObject *Sender);
	void __fastcall cbLocationChange(TObject *Sender);

private:	// User declarations

	Layouts layman;
	TTreeNode* SiteNode;
	TTreeView *tree;
	Tank* tank;
	Tank* oldtank;
	int nSections;
	int layout_cid;

	void setPopulation( short shelf );
	int getDefaultPosition();
	int getDefaultStoreTypeID();
	int getDefaultLayoutIndex();
	void fillSiteList( int selectedID );
	void fillUpLayouts();
	void fillStorageTypes( int selectedID );
	std::string getNextPopulation();
//	std::string getPopDescription();
	const LCDbObject * findStorageType();
	void clearLayoutPanel();
	void changeVessel( String site, String position );
	bool checkVessel( TEdit * textBox, TLabel * label, bool isName );
	bool checkPosition( int locationCID, short pos, short shelf );

	enum TankAvailability { ONLINE, OFFLINE };

	void ShowGrid(TStringGrid *grid, std::vector<IPart*> list);
	void ShowGridRow(TStringGrid *grid, int row, Section* data);
	void GridHeadings(TStringGrid *grid);

	void TankInit( const char* title );
	bool TankValidate();
	void LayoutInit();
	bool LayoutValidate();
	void TankConfirmInit();
	void TankConfirm();

	Tank * saveDetails();

public:		// User declarations

	enum Mode { NEW_VESSEL, EDIT_VESSEL, ADD_LAYOUT, MOD_LAYOUT } mode;

	__fastcall TfrmNewTank(TComponent* Owner);
	void initNewTank( TTreeNode* p_parent, TTreeView *p_tree );
	void initModLayout( Tank * p_tank, TTreeNode* p_parent, TTreeView *p_tree, Mode reason );

	Tank * getPart(){ return tank; };
	Tank * createTank() { return saveDetails(); }
	Tank * saveChanges() { return saveDetails(); }
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmNewTank *frmNewTank;
//---------------------------------------------------------------------------
#endif
