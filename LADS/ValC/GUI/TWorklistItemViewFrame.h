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
//---------------------------------------------------------------------------
class TWorklistItemViewFrame : public TFrame
{
__published:	// IDE-managed Components
	TPanel *titlePanel;
	TLabel *Label1;
	TPanel *containerPanel;
	TGridLayout *GridLayout1;
	TLabel *Label2;
	TLabel *ID;
	TLabel *Label4;
	TLabel *barcode;
	TLabel *Label6;
	TLabel *machineID;
	TPanel *resultsPanel;
	TLabel *Label8;
	TListBox *resultListBox;
	TLabel *Label9;
	TLabel *test;
	TLabel *Label11;
	TLabel *status;
	TLabel *Label13;
	TLabel *timestamp;
	TLabel *Label15;
	TLabel *projectID;
private:	// User declarations
public:		// User declarations
	__fastcall TWorklistItemViewFrame(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TWorklistItemViewFrame *WorklistItemViewFrame;
//---------------------------------------------------------------------------
#endif
