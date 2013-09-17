//---------------------------------------------------------------------------

#ifndef TResultNotesDialogH
#define TResultNotesDialogH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>

#include <map>
#include "retrievalList.h"
//---------------------------------------------------------------------------
class TResultNotesDialog : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *NotesStringGrid;
	void __fastcall FormResize(TObject *Sender);
private:	// User declarations
	void FillResultsTable(std::map<int,std::map<String,String> >  &results);
    void reSizeGrid(TStringGrid *NotesStringGrid);
	retrievalList *m_pRetrievalList;
public:		// User declarations
	void setup(retrievalList *pList,int sampleID);

	__fastcall TResultNotesDialog(TComponent* Owner);

	void setupNotesFromResults(retrievalList *pList,std::map<String,String> &data);
	void setupNotesFromCryovial(retrievalList *pList,std::map<String,String> &data);
	void setupNotesFromCryoStore(retrievalList *pList,std::map<String,String> &data);
	void setupNotesFromSpecimen(retrievalList *pList,std::map<String,String> &data);
	void setupNotesFromBoxName(retrievalList *pList,std::map<String,String> &data);
};
//---------------------------------------------------------------------------
extern PACKAGE TResultNotesDialog *ResultNotesDialog;
//---------------------------------------------------------------------------
#endif
