//---------------------------------------------------------------------------

#ifndef RetrievalListResultsDialogH
#define RetrievalListResultsDialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <map>
#include "RetrievalListCtrl.h"
//---------------------------------------------------------------------------
class TfrmRetrievalListResultsDialog : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *ResultStringGrid;
	void __fastcall ResultStringGridSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall ResultStringGridMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall FormShow(TObject *Sender);

private:	// User declarations
	void FillResultsTable(std::map<int,std::map<String,String> >  &results,std::map<int,String> &testnameResults);

	retrievalList *m_pRetrievalList;
	std::map<int,std::map<String,String> > m_results;
public:		// User declarations
	__fastcall TfrmRetrievalListResultsDialog(TComponent* Owner);

	void setupFromSameSource(retrievalList *pList,String &sample_id);

	void setup(retrievalList *pList,String &sample_id);
	void setup(retrievalList *pList,String &cryovialBarcode,String &aliquot_cid,String &sample_cid);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRetrievalListResultsDialog *frmRetrievalListResultsDialog;
//---------------------------------------------------------------------------
#endif
