//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RetrievalListResultsDialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include <sstream>
#include "RetrievalListResultNotesDialog.h"
#include "RetrievalListGridUtils.h"

TfrmRetrievalListResultsDialog *ResultsDialog;
//---------------------------------------------------------------------------
__fastcall TfrmRetrievalListResultsDialog::TfrmRetrievalListResultsDialog(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TfrmRetrievalListResultsDialog::setup(retrievalList *pList,String &sample_id)
{
	m_pRetrievalList = pList;
	m_results.clear();
	std::map<int,String>  testnameResults;
	try
	{
//get the project database name
		m_pRetrievalList->getSampleResult(sample_id,m_results,testnameResults);

		//update title.
		std::wstringstream sstitle;
		sstitle << L"Analyses on the sample - found " << m_results.size();
		this->SetTextBuf(String(sstitle.str().c_str()).c_str());

		FillResultsTable(m_results,testnameResults);
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
// list all items in the project tags with that worklist id
// display barcode list and other info.

}
//---------------------------------------------------------------------------

void TfrmRetrievalListResultsDialog::setup(retrievalList *pList,String &cryovialBarcode,String &aliquot_cid,String &sample_cid)
{
	m_pRetrievalList = pList;
	m_results.clear();

	try
	{
//get the project database name
		std::map<int,String>  testnameResults;
		std::map<int,std::map<String,String> > results;
		m_pRetrievalList->getSampleResult(cryovialBarcode,aliquot_cid,sample_cid,m_results,testnameResults);

		//update title.
		std::wstringstream sstitle;
		sstitle << L"Analyses on the sample - found " << m_results.size();
		this->SetTextBuf(String(sstitle.str().c_str()).c_str());

		FillResultsTable(m_results,testnameResults);
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}

//---------------------------------------------------------------------------

void TfrmRetrievalListResultsDialog::setupFromSameSource(retrievalList *pList,String &sample_id)
{
	m_pRetrievalList = pList;
    m_results.clear();

	try
	{
//get the project database name
		std::map<int,String>  testnameResults;
		m_pRetrievalList->getCryovialListsForSameSource(sample_id,m_results,testnameResults);

		//update title.
		std::wstringstream sstitle;
		sstitle << L"Analyses on the sample - found " << m_results.size();
		this->SetTextBuf(String(sstitle.str().c_str()).c_str());

		FillResultsTable(m_results,testnameResults);
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}

//---------------------------------------------------------------------------

void TfrmRetrievalListResultsDialog::FillResultsTable(std::map<int,std::map<String,String> > &results,std::map<int,String> &testnameResults)
{
	ResultStringGrid->RowCount = results.size()+1;
	ResultStringGrid->ColCount = 5;
	ResultStringGrid->Cells[0][0] = "Date";
	ResultStringGrid->Cells[1][0] = "Result";
	ResultStringGrid->Cells[2][0] = "Test name";
	ResultStringGrid->Cells[3][0] = "Notes";
	ResultStringGrid->Cells[4][0] = "HIDDEN";

	int counter = 1;
	for (std::map<int,std::map<String,String> >::iterator it = results.begin(); it != results.end(); ++it)
	{
		std::map<String,String> temp = it->second;

		int test_cid = temp[RetrievalListDatabase::DEFINE_RESULT_TEST_CID].ToInt();
		String testName = testnameResults[test_cid];

		ResultStringGrid->Cells[0][counter] = temp[RetrievalListDatabase::DEFINE_RESULT_TIME_STAMP];
		ResultStringGrid->Cells[1][counter] = temp[RetrievalListDatabase::DEFINE_RESULT_STRING_VALUE];
		ResultStringGrid->Cells[2][counter] = testName;

		if (temp[RetrievalListDatabase::DEFINE_RESULT_NOTE_EXISTS].ToInt() > 0)
			ResultStringGrid->Cells[3][counter] = "Click to view";
		else
			ResultStringGrid->Cells[3][counter] = "No notes";
		ResultStringGrid->Cells[4][counter] = temp[RetrievalListDatabase::DEFINE_RESULT_ID];
		counter++;
	}
}

//---------------------------------------------------------------------------

/*
  select top 100 * from result where result_id = '73114' ;
select top 100 * from NOTE where object_id = '73114';
  */
void __fastcall TfrmRetrievalListResultsDialog::ResultStringGridSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect)
{
	if (ResultStringGrid->Cells[3][ARow] == "Click to view")
	{
		int iResultID = ResultStringGrid->Cells[4][ARow].ToInt();
		std::auto_ptr<TfrmRetrievalListResultNotesDialog> listDialog = std::auto_ptr<TfrmRetrievalListResultNotesDialog>( new TfrmRetrievalListResultNotesDialog(this) );
		try
		{
			listDialog->setup(m_pRetrievalList,iResultID);
			listDialog->ShowModal();
		}
		catch( const String& msg )
		{
			String displayString = String("Failed - \n\n Reason :\n") + msg;
			Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
		}
	}

}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListResultsDialog::FormResize(TObject *Sender)
{
	RetrivalListGridUtils::reSizeGrid(this,ResultStringGrid,true);

}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListResultsDialog::ResultStringGridMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y)
{
	int ACol;
	int ARow;
	ResultStringGrid->MouseToCell(X,Y,ACol,ARow);

	if (Shift.Contains(ssLeft))
	{
		if (ARow == 0)
		{
			RetrivalListGridUtils::SortStringGrid(ResultStringGrid, ACol);
		}
		return;
	}
 /*
	if (Shift.Contains(ssRight))
	{
		if (ARow > 0)
		{
		  mainStringGrid->Row = ARow;
		  PopupMenu1->Popup(Mouse->CursorPos.X,Mouse->CursorPos.Y);
		}
	}
*/
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListResultsDialog::FormShow(TObject *Sender)
{
	RetrivalListGridUtils::reSizeGrid(this,ResultStringGrid,true);
}
//---------------------------------------------------------------------------




