//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TResultNotesDialog.h"
#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TResultNotesDialog *ResultNotesDialog;
//---------------------------------------------------------------------------
__fastcall TResultNotesDialog::TResultNotesDialog(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TResultNotesDialog::setup(retrievalList *pList,int sampleID)
{
	m_pRetrievalList = pList;
	try
	{
//get the project database name
		std::map<int,std::map<String,String> >  results;
		m_pRetrievalList->getSampleNotes(sampleID,results);
		this->SetTextBuf(L"Notes from Result");
		FillResultsTable(results);
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}

//---------------------------------------------------------------------------

void TResultNotesDialog::FillResultsTable(std::map<int,std::map<String,String> >  &results)
{
	NotesStringGrid->RowCount = results.size()+1;
	NotesStringGrid->ColCount = 2;
	NotesStringGrid->Cells[0][0] = "Date";
	NotesStringGrid->Cells[1][0] = "Note";

	int counter = 1;

	for (std::map<int,std::map<String,String> > ::iterator it = results.begin(); it != results.end(); ++it)
	{
		std::map<String,String> temp = it->second;

		NotesStringGrid->Cells[0][counter] = temp[database::DEFINE_NOTE_TIME_STAMP];
		NotesStringGrid->Cells[1][counter] = temp[database::DEFINE_NOTE_CONTENTS];
		counter++;
	}
	Utils::reSizeGrid(this,NotesStringGrid,false);
}

//---------------------------------------------------------------------------

void __fastcall TResultNotesDialog::FormResize(TObject *Sender)
{
	Utils::reSizeGrid(this,NotesStringGrid,false);
}
//---------------------------------------------------------------------------

void TResultNotesDialog::setupNotesFromResults(retrievalList *pList,std::map<String,String> &data)
{
	m_pRetrievalList = pList;
	try
	{
		std::map<int,std::map<String,String> >  results;
		m_pRetrievalList->getNotesFromResult(data,results);
		this->SetTextBuf(L"Notes from Results");
		FillResultsTable(results);
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------
void TResultNotesDialog::setupNotesFromCryovial(retrievalList *pList,std::map<String,String> &data)
{
	m_pRetrievalList = pList;
	try
	{
		std::map<int,std::map<String,String> >  results;
		m_pRetrievalList->getNotesFromCryovial(data,results);
		this->SetTextBuf(L"Notes from Cryovial");
		FillResultsTable(results);
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------
void TResultNotesDialog::setupNotesFromCryoStore(retrievalList *pList,std::map<String,String> &data)
{
	m_pRetrievalList = pList;
	try
	{
		std::map<int,std::map<String,String> >  results;
		m_pRetrievalList->getNotesFromCryoStore(data,results);
		this->SetTextBuf(L"Notes from Cryovial Store");
		FillResultsTable(results);
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------
void TResultNotesDialog::setupNotesFromSpecimen(retrievalList *pList,std::map<String,String> &data)
{
	m_pRetrievalList = pList;
	try
	{
		std::map<int,std::map<String,String> >  results;
		m_pRetrievalList->getNotesFromSpecimen(data,results);
		this->SetTextBuf(L"Notes from Specimen");
		FillResultsTable(results);
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------
void TResultNotesDialog::setupNotesFromBoxName(retrievalList *pList,std::map<String,String> &data)
{
	m_pRetrievalList = pList;
	try
	{
		std::map<int,std::map<String,String> >  results;
		m_pRetrievalList->getNotesFromBoxName(data,results);
		this->SetTextBuf(L"Notes from Box Name");
		FillResultsTable(results);
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

