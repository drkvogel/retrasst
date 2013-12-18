//---------------------------------------------------------------------------

#ifndef RetrievalListCryovialHistoryDialogH
#define RetrievalListCryovialHistoryDialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include "RetrievalListCtrl.h"
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmRetrievalListCryovialHistoryDialog : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *RichEdit;
private:	// User declarations
	retrievalList *m_pRetrievalList;

public:		// User declarations
	__fastcall TfrmRetrievalListCryovialHistoryDialog(TComponent* Owner);

	void setup(retrievalList *pList,std::map<String,String> &cryovialInfo);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRetrievalListCryovialHistoryDialog *frmRetrievalListCryovialHistoryDialog;
//---------------------------------------------------------------------------
#endif
