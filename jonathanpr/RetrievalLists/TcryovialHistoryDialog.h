//---------------------------------------------------------------------------

#ifndef TcryovialHistoryDialogH
#define TcryovialHistoryDialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include "retrievalList.h"
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TcryovialHistoryDialog : public TForm
{
__published:	// IDE-managed Components
	TRichEdit *RichEdit;
private:	// User declarations
	retrievalList *m_pRetrievalList;

public:		// User declarations
	__fastcall TcryovialHistoryDialog(TComponent* Owner);

	void setup(retrievalList *pList,std::map<String,String> &cryovialInfo);
};
//---------------------------------------------------------------------------
extern PACKAGE TcryovialHistoryDialog *cryovialHistoryDialog;
//---------------------------------------------------------------------------
#endif
