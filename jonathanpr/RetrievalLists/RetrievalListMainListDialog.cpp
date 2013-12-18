//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RetrievalListMainListDialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#include <sstream>
#include "RetrievalListDatabase.h"
#include "RetrievalListResultsDialog.h"
#include "RetrievalListResultNotesDialog.h"
#include "RetrievalListCryovialHistoryDialog.h"
#include "RetrievalListGridUtils.h"
TfrmRetrievalListDialog *listDialog;

//---------------------------------------------------------------------------
__fastcall TfrmRetrievalListDialog::TfrmRetrievalListDialog(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TfrmRetrievalListDialog::displayGridStrings()
{
	m_BGProcessingCount = 0;
	//update title.
	std::wstringstream sstitle;
	sstitle << L"Cryovial search results - found " << m_cryoLists.size();
	this->SetTextBuf(String(sstitle.str().c_str()).c_str());

	FillCryovialList(m_cryoLists);

	std::map<int,std::map<String,String> > newMap;
//set history status to -1, so we know when we are done.
	for (std::map<int,std::map<String,String> >::iterator it = m_cryoLists.begin(); it != m_cryoLists.end(); ++it)
	{
		int cryoid = it->first;

		std::map<String,String> listInfo = it->second;
		listInfo.insert(std::pair<String,String> (RetrievalListDatabase::DEFINE_HISTORY_STATUS,String("-1")));

		newMap.insert(std::pair<int,std::map<String,String> > (cryoid,listInfo));
//		m_cryoLists.erase(it);
//		m_cryoLists.insert(std::pair<int,std::map<String,String> > (cryoid,listInfo));
	}
	m_cryoLists = newMap;
//	NOW SET OFF THE WORKER THREAD, GET MORE DETAILS
	bgWorkerThreadTask *m_pWorkerThreadTask = new bgWorkerThreadTask();
	m_pWorkerThreadTask->setUp(m_cryoLists,m_pRetrievalList,m_BGProcessingCount);
	m_workerThread.queueTask(dynamic_cast<paulst::WorkerThreadTask *>(m_pWorkerThreadTask));

    mainStringGrid->Enabled = false;

	AnimTimer->Enabled = true;
	m_AnimCount = 0;
	Timer1->Enabled = true;
}

//---------------------------------------------------------------------------

void TfrmRetrievalListDialog::setup(retrievalList *retrievalList,std::vector<String> &sample_barcodes,String &project_id)
{
	std::map<String,String> workingData;
	m_pRetrievalList = retrievalList;
	try
	{
//get the project database name
		m_pRetrievalList->getProject(project_id.ToInt(),workingData);
		String db_name = workingData[RetrievalListDatabase::DEFINE_DB_NAME];

		//connect to the project database
		m_pRetrievalList->connectToProject(db_name);
        m_cryoLists.clear();
		m_pRetrievalList->getCryovialLists(project_id,sample_barcodes,m_cryoLists);
		displayGridStrings();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}

//---------------------------------------------------------------------------

void TfrmRetrievalListDialog::setup(retrievalList *retrievalList,std::vector<String> &barcodes,String &project_id,String &aliquit_id)
{
	std::map<String,String> workingData;

	m_pRetrievalList = retrievalList;
	try
	{
//get the project database name
		m_pRetrievalList->getProject(project_id.ToInt(),workingData);
		String db_name = workingData[RetrievalListDatabase::DEFINE_DB_NAME];

		//connect to the project database
		m_pRetrievalList->connectToProject(db_name);

		m_cryoLists.clear();
		//get the cryovials
		m_pRetrievalList->getCryovialLists(project_id,aliquit_id,barcodes,m_cryoLists);
		displayGridStrings();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}

//---------------------------------------------------------------------------

void TfrmRetrievalListDialog::setup(retrievalList *pList,std::map<String,String> &data)
{
	m_pRetrievalList = pList;
	std::map<String,String> *workingData = &data;
	String project_cid = (*workingData)[RetrievalListDatabase::DEFINE_PROJECT_CID].ToInt();
	String retrieval_cid = (*workingData)[RetrievalListDatabase::DEFINE_RETRIEVAL_CID].ToInt();

	try
	{
//get the project database name
		m_pRetrievalList->getProject(project_cid.ToInt(),(*workingData));
		String db_name = (*workingData)[RetrievalListDatabase::DEFINE_DB_NAME];

		//connect to the project database
		m_pRetrievalList->connectToProject(db_name);

		m_cryoLists.clear();
		//get the cryovials
		m_pRetrievalList->getCryovialLists(retrieval_cid.ToInt(),m_cryoLists);
		displayGridStrings();
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
//select * from specimen s, result r where barcode='112017074' and s.sample_id = r.sample_id
void TfrmRetrievalListDialog::FillCryovialList(std::map<int,std::map<String,String> > &cryoLists)
{
	mainStringGrid->RowCount = cryoLists.size()+1;
	mainStringGrid->ColCount = 12;
	mainStringGrid->Cells[0][0] = "Notes";
	mainStringGrid->Cells[1][0] = "Bar code";
	mainStringGrid->Cells[2][0] = "Aliquot";
	mainStringGrid->Cells[3][0] = "Box name";
	mainStringGrid->Cells[4][0] = "Box position";
  //	mainStringGrid->Cells[5][0] = "Sample id";  //(SOURCE NAME)
	mainStringGrid->Cells[5][0] = "Site";
	mainStringGrid->Cells[6][0] = "Pos at site";
	mainStringGrid->Cells[7][0] = "Vessel";
	mainStringGrid->Cells[8][0] = "Shelf num";
	mainStringGrid->Cells[9][0] = "Rack name";
	mainStringGrid->Cells[10][0] = "Rack Pos";
	mainStringGrid->Cells[11][0] = "HIDDEN";

	int counter = 1;

	for (std::map<int,std::map<String,String> >::iterator it = cryoLists.begin(); it != cryoLists.end(); ++it)
	{
		std::map<String,String> temp = it->second;

		mainStringGrid->Cells[0][counter] = "";
		mainStringGrid->Cells[1][counter] = temp[RetrievalListDatabase::DEFINE_CRYOVIAL_BARCODE];
		mainStringGrid->Cells[2][counter] = temp[RetrievalListDatabase::DEFINE_ALIQUOT_EXTERNAL_NAME];
		mainStringGrid->Cells[3][counter] = temp[RetrievalListDatabase::DEFINE_BOXNAME_EXTERNAL_NAME];
		mainStringGrid->Cells[4][counter] = temp[RetrievalListDatabase::DEFINE_CRYOVIAL_POSITION];
		mainStringGrid->Cells[5][counter] = temp[RetrievalListDatabase::DEFINE_LOCATION_EXTERNAL_NAME];
		mainStringGrid->Cells[6][counter] = temp[RetrievalListDatabase::DEFINE_TANK_MAP_POSITION];
		mainStringGrid->Cells[7][counter] = temp[RetrievalListDatabase::DEFINE_STORAGE_EXTERNAL_NAME];
		mainStringGrid->Cells[8][counter] = temp[RetrievalListDatabase::DEFINE_TANK_MAP_SHELF_NUMBER];
		mainStringGrid->Cells[9][counter] = temp[RetrievalListDatabase::DEFINE_RACK_EXTERNAL_NAME];
		mainStringGrid->Cells[10][counter] = temp[RetrievalListDatabase::DEFINE_BOXSTORE_SLOT_POSITION];

		mainStringGrid->Cells[11][counter] = temp[RetrievalListDatabase::DEFINE_CRYOVIAL_ID];
		counter++;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::FormResize(TObject *Sender)
{
	RetrivalListGridUtils::reSizeGrid(this,mainStringGrid,true);
}
//---------------------------------------------------------------------------


void __fastcall TfrmRetrievalListDialog::Timer1Timer(TObject *Sender)
{
//the thread runs through the list backwards. so check to see if the first history status != -1
//this means the thread is complete
	Screen->Cursor = crSQLWait;
	std::map<int,std::map<String,String> >::iterator it = m_cryoLists.begin();

	if (it == m_cryoLists.end()) {
		//empty list. do nothing
		Timer1->Enabled = false;
		AnimTimer->Enabled = false;
		Screen->Cursor = crDefault;
		return;
	}

	std::map<String,String> temp = it->second;

//	if (temp[database::DEFINE_HISTORY_STATUS] == "-1")
//		return;

	if (m_pRetrievalList->isBackgroundProcessing())
		return;

	mainStringGrid->Enabled = true;

//thread is finished
	Screen->Cursor = crDefault;
	Timer1->Enabled = false;
	AnimTimer->Enabled = false;
	int counter = 1;

	//update title.
	std::wstringstream sstitle;
	sstitle << L"Cryovial search results - found " << m_cryoLists.size();
	this->SetTextBuf(String(sstitle.str().c_str()).c_str());

	for (std::map<int,std::map<String,String> >::iterator it = m_cryoLists.begin(); it != m_cryoLists.end(); ++it)
	{
		std::map<String,String> temp = it->second;

		mainStringGrid->Cells[0][counter] = temp[RetrievalListDatabase::DEFINE_HISTORY_STATUS];

		mainStringGrid->Cells[1][counter] = temp[RetrievalListDatabase::DEFINE_CRYOVIAL_BARCODE];
		mainStringGrid->Cells[2][counter] = temp[RetrievalListDatabase::DEFINE_ALIQUOT_EXTERNAL_NAME];
		mainStringGrid->Cells[3][counter] = temp[RetrievalListDatabase::DEFINE_BOXNAME_EXTERNAL_NAME];
		mainStringGrid->Cells[4][counter] = temp[RetrievalListDatabase::DEFINE_CRYOVIAL_POSITION];
		mainStringGrid->Cells[5][counter] = temp[RetrievalListDatabase::DEFINE_LOCATION_EXTERNAL_NAME];
		mainStringGrid->Cells[6][counter] = temp[RetrievalListDatabase::DEFINE_TANK_MAP_POSITION];
		mainStringGrid->Cells[7][counter] = temp[RetrievalListDatabase::DEFINE_STORAGE_EXTERNAL_NAME];
		mainStringGrid->Cells[8][counter] = temp[RetrievalListDatabase::DEFINE_TANK_MAP_SHELF_NUMBER];
		mainStringGrid->Cells[9][counter] = temp[RetrievalListDatabase::DEFINE_RACK_EXTERNAL_NAME];
		mainStringGrid->Cells[10][counter] = temp[RetrievalListDatabase::DEFINE_BOXSTORE_SLOT_POSITION];
		counter++;
	}
//resize grids, now it's completely filled all the grids
	RetrivalListGridUtils::reSizeGrid(this,mainStringGrid,true);
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::mainStringGridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
	if (ACol != 0)
		return;
	if (ARow == 0)
		return;

	String S = mainStringGrid->Cells[ACol][ARow]; // cell contents
	if (S.Length()==0)
         return;
	TColor c = mainStringGrid->Canvas->Brush->Color;
	 //CONST THESE STRINGS AND SHARE THEM WITH DATABASE.
	if (mainStringGrid->Cells[0][ARow] != "")
	{
		if (mainStringGrid->Cells[0][ARow] == "Multiple notes available")
			mainStringGrid->Canvas->Brush->Color = clWebOliveDrab ;   //http://docwiki.embarcadero.com/RADStudio/XE4/en/Colors_in_VCL
		else if (mainStringGrid->Cells[0][ARow] == "Specimen notes available")
			mainStringGrid->Canvas->Brush->Color = clWebYellowGreen;
		else if (mainStringGrid->Cells[0][ARow] == "Box notes available")
			mainStringGrid->Canvas->Brush->Color = clWebYellowGreen;
		else if (mainStringGrid->Cells[0][ARow] == "Cryovial notes available")
			mainStringGrid->Canvas->Brush->Color = clWebYellowGreen;
		else if (mainStringGrid->Cells[0][ARow] == "Cryovial store notes available")
			mainStringGrid->Canvas->Brush->Color = clWebYellowGreen;
		else if (mainStringGrid->Cells[0][ARow] == "Result notes available")
			mainStringGrid->Canvas->Brush->Color = clWebYellowGreen;

		mainStringGrid->Canvas->FillRect(Rect);
		mainStringGrid->Canvas->TextRect(Rect,  Rect.Left + 2, Rect.Top + 2, mainStringGrid->Cells[ACol][ARow]);
	}

	UINT SavedAlign = SetTextAlign( mainStringGrid->Canvas->Handle, TA_CENTER);
	mainStringGrid->Canvas->TextRect(Rect, Rect.Left + (Rect.Right - Rect.Left) / 2, Rect.Top + 2, S);
	SetTextAlign(mainStringGrid->Canvas->Handle, SavedAlign);
	mainStringGrid->Canvas->Brush->Color = c;
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::MenuSearchWithBarcodeClick(TObject *Sender)
{
	int Row = mainStringGrid->Selection.Top;
	int selectedCryovialSampleID = mainStringGrid->Cells[mainStringGrid->ColCount-1][Row].ToInt();
	String CryoBarCode = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_CRYOVIAL_BARCODE];

	try
	{
		m_cryoLists.clear();
		m_pRetrievalList->getCryovialListsWithBarCode(CryoBarCode,m_cryoLists);
		displayGridStrings();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::MenuSearchWithBarcodeAndAliquotClick(TObject *Sender)
{
	int Row = mainStringGrid->Selection.Top;
	int selectedCryovialSampleID = mainStringGrid->Cells[mainStringGrid->ColCount-1][Row].ToInt();
	String CryoBarCode = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_CRYOVIAL_BARCODE];
	String Alquot_cid = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_CRYOVIAL_ALIQUOT_CID];

	try
	{
		m_cryoLists.clear();
		m_pRetrievalList->getCryovialListsWithBarcodeAndAliquot(CryoBarCode,Alquot_cid,m_cryoLists);
		displayGridStrings();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::MenuSearchWithSampleIDClick(TObject *Sender)
{
	int Row = mainStringGrid->Selection.Top;
	int selectedCryovialSampleID = mainStringGrid->Cells[mainStringGrid->ColCount-1][Row].ToInt();
	String SrcName = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_SAMPLE_SOURCE_NAME];

	if (SrcName.Length() == 0)
      	return;

	try
	{
		m_cryoLists.clear();
		m_pRetrievalList->getCryovialListsWithSrcName(SrcName,m_cryoLists);
		displayGridStrings();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------


void __fastcall TfrmRetrievalListDialog::MenuFindAnyAnalysesClick(TObject *Sender)
{
	int Row = mainStringGrid->Selection.Top;
	int selectedCryovialSampleID = mainStringGrid->Cells[mainStringGrid->ColCount-1][Row].ToInt();
	String cryovialBarcode = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_CRYOVIAL_BARCODE];
	String aliquot_cid = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_CRYOVIAL_ALIQUOT_CID];
	String sample_id = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_CRYOVIAL_SPECIMEN_ID];

	std::auto_ptr<TfrmRetrievalListResultsDialog> listDialog = std::auto_ptr<TfrmRetrievalListResultsDialog>( new TfrmRetrievalListResultsDialog(this) );
	try
	{
		Screen->Cursor = crSQLWait;
		listDialog->setup(m_pRetrievalList,cryovialBarcode,aliquot_cid,sample_id);
		Screen->Cursor = crDefault;
		listDialog->ShowModal();
	}
	catch( const String& msg )
	{
		Screen->Cursor = crDefault;
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::MenuCryovialHistoryClick(TObject *Sender)
{
	int Row = mainStringGrid->Selection.Top;
	int selectedCryovialSampleID = mainStringGrid->Cells[mainStringGrid->ColCount-1][Row].ToInt();
	std::map<String,String> info = m_cryoLists[selectedCryovialSampleID];

	std::auto_ptr<TfrmRetrievalListCryovialHistoryDialog> listDialog = std::auto_ptr<TfrmRetrievalListCryovialHistoryDialog>( new TfrmRetrievalListCryovialHistoryDialog(this) );
	try
	{
		listDialog->setup(m_pRetrievalList,info);
		listDialog->ShowModal();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

//SAMPLEID
void __fastcall TfrmRetrievalListDialog::FindanyAnalysescryovialbarcodeClick(TObject *Sender)
{
	int Row = mainStringGrid->Selection.Top;
	int selectedCryovialSampleID = mainStringGrid->Cells[mainStringGrid->ColCount-1][Row].ToInt();
	String sample_id = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_CRYOVIAL_SPECIMEN_ID];

	std::auto_ptr<TfrmRetrievalListResultsDialog> listDialog = std::auto_ptr<TfrmRetrievalListResultsDialog>( new TfrmRetrievalListResultsDialog(this) );
	try
	{
 		Screen->Cursor = crSQLWait;
		listDialog->setup(m_pRetrievalList,sample_id);
		Screen->Cursor = crDefault;
		listDialog->ShowModal();
	}
	catch( const String& msg )
	{
		Screen->Cursor = crDefault;
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::MenuFindSiblingsClick(TObject *Sender)
{
// I GUESS THIS IS USING Cryovial_id, basically ignore the aliquot
	int Row = mainStringGrid->Selection.Top;
	int selectedCryovialSampleID = mainStringGrid->Cells[mainStringGrid->ColCount-1][Row].ToInt();
	String Cryo_id = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_CRYOVIAL_ID];

	try
	{
		m_cryoLists.clear();
		m_pRetrievalList->getCryovialListsWithCryovialid(Cryo_id,m_cryoLists);
		displayGridStrings();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::PopupMenu1Popup(TObject *Sender)
{
//enable or disable parts of the menu
	int menuCount = PopupMenu1->Items->Count;
	int Row = mainStringGrid->Selection.Top;
	int selectedCryovialSampleID = mainStringGrid->Cells[mainStringGrid->ColCount-1][Row].ToInt();

	for (int i=0;i<menuCount;i++)
	{
		if (PopupMenu1->Items->Items[i]->Name == "Rebuildlistusing")
		{
			TMenuItem *notesMenu = PopupMenu1->Items->Items[i];
			int noteCount = notesMenu->Count;

			for (int j=0;j<noteCount;j++)
			{
				if (notesMenu->Items[j]->Name == "MenuSearchWithSampleID")
				{
					String sourceName = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_SAMPLE_SOURCE_NAME];
					notesMenu->Items[j]->Enabled = sourceName.Length() > 0;
				}
				else if (notesMenu->Items[j]->Name == "MenuSearchWithBarcodeAndAliquot")
				{
					String aliquot = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_CRYOVIAL_ALIQUOT_CID];
					notesMenu->Items[j]->Enabled = aliquot.Length() > 0;
				}
			}
		}
		else if (PopupMenu1->Items->Items[i]->Name == "NotesSubMenu")
		{
			TMenuItem *notesMenu = PopupMenu1->Items->Items[i];
			int noteCount = notesMenu->Count;

			for (int j=0;j<noteCount;j++)
			{
				if (notesMenu->Items[j]->Name == "NotesFromResultsMenu")
				{
					String count = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_RESULTS_NOTES_COUNT];
					if ((count.IsEmpty()) || (count == L"0"))
						notesMenu->Items[j]->Enabled = false;
					else
						notesMenu->Items[j]->Enabled = true;
					notesMenu->Items[j]->Caption = L"Results";
				}
				else if (notesMenu->Items[j]->Name == "NotesFromCryovialMenu")
				{
					String count = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_CRYOVIAL_NOTES_EXISTS];
					if ((count.IsEmpty()) || (count == L"0"))
						notesMenu->Items[j]->Enabled = false;
					else
						notesMenu->Items[j]->Enabled = true;
					notesMenu->Items[j]->Caption = L"Cryovial";
				}
				else if (notesMenu->Items[j]->Name == "NotesFromCryoStoreMenu")
				{
					String count = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_CRYOVIAL_STORE_NOTES_EXISTS];
					if ((count.IsEmpty()) || (count == L"0"))
						notesMenu->Items[j]->Enabled = false;
					else
						notesMenu->Items[j]->Enabled = true;
						notesMenu->Items[j]->Caption = L"Cryovial Store";
				}
				else if (notesMenu->Items[j]->Name == "NotesFromSpecimenMenu")
				{
					String count = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_SPECIMEN_NOTES_EXISTS];
					if ((count.IsEmpty()) || (count == L"0"))
						notesMenu->Items[j]->Enabled = false;
					else
						notesMenu->Items[j]->Enabled = true;
					notesMenu->Items[j]->Caption = L"Specimen";
				}
				else if (notesMenu->Items[j]->Name == "NotesFromBoxNameMenu")
				{
					String count = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_BOXNAME_NOTES_EXISTS];

					if ((count.IsEmpty()) || (count == L"0"))
						notesMenu->Items[j]->Enabled = false;
					else
						notesMenu->Items[j]->Enabled = true;
					notesMenu->Items[j]->Caption = L"Box";
				}
			}
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::SearchWithCryovialSampleidClick(TObject *Sender)
{
//  try cryovial.sample_id = specimen.sample_id
//works with Feb 2005 SEARCH
	int Row = mainStringGrid->Selection.Top;
	int selectedCryovialSampleID = mainStringGrid->Cells[mainStringGrid->ColCount-1][Row].ToInt();
	String sampleid = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_CRYOVIAL_SPECIMEN_ID];

	try
	{
		m_cryoLists.clear();
		m_pRetrievalList->getCryovialListsWithSampleID(sampleid,m_cryoLists);
		displayGridStrings();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::AnimTimerTimer(TObject *Sender)
{
	//update title.
	std::wstringstream sstitle;
	sstitle << L"Cryovial search results - found " << m_cryoLists.size();
	sstitle << L"   [ SEARCHING ";
	sstitle << m_BGProcessingCount << L"/" <<  m_cryoLists.size() << " ] ";
 /*
	switch (m_AnimCount)
	{
	case 0:
		sstitle << L" -- ";
	break;
	case 1:
		sstitle << L" \\ ";
	break;
	case 2:
		sstitle << L" | ";
	break;
	case 3:
		sstitle << L" / ";
	break;
	case 4:
		sstitle << L" -- ";
	break;
	case 5:
		sstitle << L" \\ ";
	break;
	case 6:
		sstitle << L" | ";
	break;
	case 7:
		sstitle << L" / ";
	break;
	}

 //	sstitle << L" ";

	m_AnimCount++;
	if (m_AnimCount > 7)
		m_AnimCount = 0;
   */
	this->SetTextBuf(String(sstitle.str().c_str()).c_str());
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::MenuFindAnyAnalysesFromSameSourceClick(TObject *Sender)
{
	int Row = mainStringGrid->Selection.Top;
	int selectedCryovialSampleID = mainStringGrid->Cells[mainStringGrid->ColCount-1][Row].ToInt();
	String sampleid = m_cryoLists[selectedCryovialSampleID][RetrievalListDatabase::DEFINE_CRYOVIAL_SPECIMEN_ID];

	std::auto_ptr<TfrmRetrievalListResultsDialog> listDialog = std::auto_ptr<TfrmRetrievalListResultsDialog>( new TfrmRetrievalListResultsDialog(this) );
	try
	{
		Screen->Cursor = crSQLWait;
		listDialog->setupFromSameSource(m_pRetrievalList,sampleid);
		Screen->Cursor = crDefault;
		listDialog->ShowModal();
	}
	catch( const String& msg )
	{
		Screen->Cursor = crDefault;
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::mainStringGridMouseDown(TObject *Sender, TMouseButton Button,  TShiftState Shift, int X, int Y)
{
	int ACol;
	int ARow;
	mainStringGrid->MouseToCell(X,Y,ACol,ARow);

	if (Shift.Contains(ssLeft))
	{
		if (ARow == 0)
			RetrivalListGridUtils::SortStringGrid(mainStringGrid, ACol);
		return;
	}

	if (Shift.Contains(ssRight))
	{
		if (ARow > 0)
		{
			mainStringGrid->Row = ARow;
			PopupMenu1->Popup(Mouse->CursorPos.X,Mouse->CursorPos.Y);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::NotesFromResultsMenuClick(TObject *Sender)
{
	int Row = mainStringGrid->Selection.Top;
	int selectedCryovialSampleID = mainStringGrid->Cells[mainStringGrid->ColCount-1][Row].ToInt();

	std::auto_ptr<TfrmRetrievalListResultNotesDialog> listDialog = std::auto_ptr<TfrmRetrievalListResultNotesDialog>( new TfrmRetrievalListResultNotesDialog(this) );
	try
	{
		std::map<String,String> data = m_cryoLists[selectedCryovialSampleID];
		listDialog->setupNotesFromResults(m_pRetrievalList,data);
		listDialog->ShowModal();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::NotesFromCryovialMenuClick(TObject *Sender)
{
	int Row = mainStringGrid->Selection.Top;
	int selectedCryovialSampleID = mainStringGrid->Cells[mainStringGrid->ColCount-1][Row].ToInt();

	std::auto_ptr<TfrmRetrievalListResultNotesDialog> listDialog = std::auto_ptr<TfrmRetrievalListResultNotesDialog>( new TfrmRetrievalListResultNotesDialog(this) );
	try
	{
		std::map<String,String> data = m_cryoLists[selectedCryovialSampleID];
		listDialog->setupNotesFromCryovial(m_pRetrievalList,data);
		listDialog->ShowModal();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::NotesFromCryoStoreMenuClick(TObject *Sender)
{
	int Row = mainStringGrid->Selection.Top;
	int selectedCryovialSampleID = mainStringGrid->Cells[mainStringGrid->ColCount-1][Row].ToInt();

	std::auto_ptr<TfrmRetrievalListResultNotesDialog> listDialog = std::auto_ptr<TfrmRetrievalListResultNotesDialog>( new TfrmRetrievalListResultNotesDialog(this) );
	try
	{
		std::map<String,String> data = m_cryoLists[selectedCryovialSampleID];
		listDialog->setupNotesFromCryoStore(m_pRetrievalList,data);
		listDialog->ShowModal();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::NotesFromSpecimenMenuClick(TObject *Sender)
{
	int Row = mainStringGrid->Selection.Top;
	int selectedCryovialSampleID = mainStringGrid->Cells[mainStringGrid->ColCount-1][Row].ToInt();

	std::auto_ptr<TfrmRetrievalListResultNotesDialog> listDialog = std::auto_ptr<TfrmRetrievalListResultNotesDialog>( new TfrmRetrievalListResultNotesDialog(this) );
	try
	{
		std::map<String,String> data = m_cryoLists[selectedCryovialSampleID];
		listDialog->setupNotesFromSpecimen(m_pRetrievalList,data);
		listDialog->ShowModal();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::NotesFromBoxNameMenuClick(TObject *Sender)
{
	int Row = mainStringGrid->Selection.Top;
	int selectedCryovialSampleID = mainStringGrid->Cells[mainStringGrid->ColCount-1][Row].ToInt();

	std::auto_ptr<TfrmRetrievalListResultNotesDialog> listDialog = std::auto_ptr<TfrmRetrievalListResultNotesDialog>( new TfrmRetrievalListResultNotesDialog(this) );
	try
	{
		std::map<String,String> data = m_cryoLists[selectedCryovialSampleID];
		listDialog->setupNotesFromBoxName(m_pRetrievalList,data);
		listDialog->ShowModal();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmRetrievalListDialog::FormClose(TObject *Sender, TCloseAction &Action)
{
	m_workerThread.cancelCurrentlyExecuting();
	//don't quit will we know it's cancelled
	while (m_pRetrievalList->isBackgroundProcessing()) {};
}
//---------------------------------------------------------------------------

