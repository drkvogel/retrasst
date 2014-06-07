//---------------------------------------------------------------------------

#ifndef TWorklistItemViewFrameH
#define TWorklistItemViewFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.Grid.hpp>
#include <FMX.TreeView.hpp>
//---------------------------------------------------------------------------
class TWorklistItemViewFrame : public TFrame
{
__published:	// IDE-managed Components
	TPanel *titlePanel;
	TLabel *Label1;
	TPanel *containerPanel;
	TGridLayout *GridLayout1;
	TLabel *Label2;
	TLabel *barcode;
	TLabel *Label4;
	TLabel *test;
	TLabel *Label6;
	TLabel *timestamp;
	TPanel *resultsAndRelativesPanel;
	TLabel *Label9;
	TLabel *status;
	TLabel *Label11;
	TLabel *machineID;
	TLabel *Label13;
	TLabel *projectID;
	TLabel *Label15;
	TLabel *ID;
	TPanel *resultsPanel;
	TLabel *Label3;
	TListBox *resultListBox;
	TPanel *relativesPanel;
	TLabel *Label5;
	TTreeView *familyTree;
	TFlowLayout *FlowLayout1;
private:	// User declarations
public:		// User declarations
	__fastcall TWorklistItemViewFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TWorklistItemViewFrame *WorklistItemViewFrame;
//---------------------------------------------------------------------------
#endif
