//---------------------------------------------------------------------------

#ifndef ReferredBoxesSummaryH
#define ReferredBoxesSummaryH

//---------------------------------------------------------------------------

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "ReferredBoxes.h"

class TfrmReferredBoxesSummary : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TPanel *Panel1;
	TButton *btnConfirm;
	TStringGrid *sgBoxes;
	TButton *btnCancel;
    TSplitter *Splitter1;
    TGroupBox *GroupBox2;
    TMemo *memoLog;
	void __fastcall btnConfirmClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall sgBoxesDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
private:	// User declarations
    vector<string> info;
    vector<string> warnings;
    vector<string> errors;
    void debugLog(string s);
    void displayBoxes();
	void addEvent(LQuery & qp, BoxArrivalRecord * box);
	void copyHistory(LQuery & qp, BoxArrivalRecord * box);
	void storeTRS(LQuery & qp, BoxArrivalRecord * box);
    void updateLBA(LQuery & qp, BoxArrivalRecord * box);
    void signOffBoxes();
public:		// User declarations
	tdvecpBoxArrivalRecord summaryBoxes;
	__fastcall TfrmReferredBoxesSummary(TComponent* Owner);
};

extern PACKAGE TfrmReferredBoxesSummary *frmReferredBoxesSummary;

#endif
