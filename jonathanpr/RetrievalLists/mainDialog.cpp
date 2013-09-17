//---------------------------------------------------------------------------

#include <vcl.h>
#include "mainDialog.h"
#include "TDbSelectionDialog.h"
#include "util_Holder.h"
#include "mainListDialog.h"
#include <sstream>
#include <map>
#include "LIMSDatabase.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmViewListMain *frmViewListMain;

//---------------------------------------------------------------------------
__fastcall TfrmViewListMain::TfrmViewListMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmViewListMain::FormShow(TObject *Sender)
{
//on startup
	if (!m_retrievalList.hasSelectedDB())
	{
	/** modified NG 17/9/13 - use current StoreMan settings

		std::auto_ptr<TDbSelectionDialog> dbForm = std::auto_ptr<TDbSelectionDialog>( new TDbSelectionDialog(this) );

		dbForm->ShowModal();
		if (dbForm->didUserQuit())
		{
			this->Close();
			return;
		}
		String selectedDB = dbForm->getDBSelection();
	**/
		String selectedDB = LIMSDatabase::getCentralDb().getDbName().c_str();
		OpenList2Button->Enabled = false;
		OpenList3Button->Enabled = false;
		try
		{
			m_retrievalList.initalise(selectedDB);//passwordForm->getPassword());
			//if we get to here, we have access to the database
			fillExerciseListComboBox();
			fillRetrivalListComboBox();
			fillProjectsListComboBox();
			fillAliquotListComboBox();
			return;
		}
		catch( const String& msg )
		{
			String displayString = String("Failed - \n\n Reason :\n") + msg;
			Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
		}
	}

}

//---------------------------------------------------------------------------

void TfrmViewListMain::clearComboBox(TComboBox *cleanme)
{
	for (int i = 0; i < cleanme->Items->Count; i++) {
		 delete cleanme->Items->Objects[i];
	}
	cleanme->Clear();
}
//---------------------------------------------------------------------------

void TfrmViewListMain::fillAliquotListComboBox()
{
	clearComboBox(aliquitListComboBox);
	try
	{
	// Save this returned data as part of the class? Do we need to hold on to it?
		std::map<int,std::map<String,String> > newRetrivalLists;
		m_retrievalList.getAliquotLists(newRetrivalLists);

		std::map<int,std::map<String,String> >::iterator it;
		for (it = newRetrivalLists.begin(); it != newRetrivalLists.end(); ++it)
		{
			std::map<String,String>  &data = it->second;
			util_mapHolder *ValueHolder = new util_mapHolder(data);
			String s = ""+ data[database::DEFINE_OBJECT_NAME_EXTERNAL_NAME] + " -  " + data[database::DEFINE_OBJECT_NAME_EXTERNAL_FULL];
			aliquitListComboBox->AddItem(s,ValueHolder);
		}
		if (newRetrivalLists.size() > 0)
		{
			aliquitListComboBox->ItemIndex = 0;
		}
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
}

//---------------------------------------------------------------------------

void TfrmViewListMain::fillRetrivalListComboBox()
{
	fillRetrivalListComboBox(0);
}

//---------------------------------------------------------------------------

void TfrmViewListMain::fillRetrivalListComboBox(int exercise_id)
{
	clearComboBox(retrivalListComboBox);
	try
	{
		std::map<int,std::map<String,String> > newRetrivalLists;

		if (exercise_id == 0)
			m_retrievalList.getRetrivalLists(newRetrivalLists);
		else
			m_retrievalList.getRetrivalLists(newRetrivalLists,exercise_id);

		std::map<int,std::map<String,String> >::iterator it;
		for (it = newRetrivalLists.begin(); it != newRetrivalLists.end(); ++it)
		{
			std::map<String,String>  &data = it->second;

			String idString = data[database::DEFINE_RETRIEVAL_DISCRIPTION];

			util_mapHolder *ValueHolder = new util_mapHolder(data);
			retrivalListComboBox->AddItem(idString,ValueHolder);
		}
		if (newRetrivalLists.size() > 0)
		{
			retrivalListComboBox->ItemIndex = 0;
		}
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
}

//---------------------------------------------------------------------------

void TfrmViewListMain::fillProjectsListComboBox()
{
	clearComboBox(ProjectNamesComboBox);
	clearComboBox(ProjectNamesComboBox3);
	try
	{
		std::map<int,std::map<String,String> > newProjectLists;
		m_retrievalList.getProjectLists(newProjectLists);

		std::map<int,std::map<String,String> >::iterator it;
		for (it = newProjectLists.begin(); it != newProjectLists.end(); ++it)
		{
			std::map<String,String>  &data = it->second;

			util_mapHolder *ValueHolder = new util_mapHolder(data);

			String s = ""+ data[database::DEFINE_PROJECT_EXTERNAL_NAME] + " -  " + data[database::DEFINE_PROJECT_EXTERNAL_FULL];
			ProjectNamesComboBox->AddItem(s,ValueHolder);
			ProjectNamesComboBox3->AddItem(s,ValueHolder);
		}
		if (newProjectLists.size() > 0)
		{
			ProjectNamesComboBox->ItemIndex = 0;
			ProjectNamesComboBox3->ItemIndex = 0;
 //			ProjectNamesComboBoxChange(NULL); //fill discription
 //			ProjectNamesComboBox3Change(NULL); //fill discription
		}
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmViewListMain::OpenListByNameButtonClick(TObject *Sender)
{
	int index = retrivalListComboBox->ItemIndex;
	if (index == -1) {
		return;
	}

	util_mapHolder *mapHolder = static_cast<util_mapHolder * >(retrivalListComboBox->Items->Objects[index]);

	//new member diaglog box
	std::auto_ptr<TlistDialog> listDialog = std::auto_ptr<TlistDialog>( new TlistDialog(this) );
	try
	{
		listDialog->setup(&m_retrievalList,mapHolder->m_map);
		listDialog->ShowModal();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

bool TfrmViewListMain::loadBarCodesFile(String &filename, std::vector<String> &barcodes)
{
	int lineNo = 1;
	TStreamReader* reader = NULL;

    __try
	{
		__try
		{
			reader = new TStreamReader(filename);//, TEncoding::UTF8);
			while (!reader->EndOfStream)
			{
				String data = reader->ReadLine();
				if ((data.Length() >= DEF_BARCODE_MIN_LENGTH) && (data.Length() <= DEF_BARCODE_MAX_LENGTH))
				{
					barcodes.push_back(data);
				}
			}
		}
		catch( ... ) //catch all! Slightly naughty I know. But I don't really care about what type of error
		{
			std::wstringstream ss;
			ss << lineNo;
			String errorText = L"Error loading bar codes file at line: ";
			errorText += ss.str().c_str();
			Application->MessageBox(errorText.c_str(), L"Error", MB_OK);
			return false;
		}
	}
	__finally
	{
		if (reader)
		{
			reader->Close();
			delete reader;
		}
	}
	return true;
}


//---------------------------------------------------------------------------

void __fastcall TfrmViewListMain::OpenList2ButtonClick(TObject *Sender)
{
	//open file and read all the lines into a file
	//validate input. check max size, ascii..etc, check they all look right
	//put into array, to pass into the search function
	std::vector<String> barcodes;
	if (!loadBarCodesFile(m_filename, barcodes))
	{
		return;
	}

	util_mapHolder *mapHolder_ProjectData = static_cast<util_mapHolder * >(ProjectNamesComboBox->Items->Objects[ProjectNamesComboBox->ItemIndex]);
	util_mapHolder *mapHolder_AliquitData = static_cast<util_mapHolder * >(aliquitListComboBox->Items->Objects[aliquitListComboBox->ItemIndex]);

	String project_id = mapHolder_ProjectData->m_map[database::DEFINE_PROJECT_CID];
	String aliquit_id = mapHolder_AliquitData->m_map[database::DEFINE_OBJECT_NAME_CID];


	//new member diaglog box
	std::auto_ptr<TlistDialog> listDialog = std::auto_ptr<TlistDialog>( new TlistDialog(this) );
	try
	{
		listDialog->setup(&m_retrievalList,barcodes,project_id,aliquit_id);
		listDialog->ShowModal();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmViewListMain::findBarCodesButton3Click(TObject *Sender)
{
	OpenTextFileDialog1->Filter = "Text files (*.txt)|*.TXT|Any file (*.*)|*.*";

	if (OpenTextFileDialog1->Execute(this->Handle))
	{
		m_filename = OpenTextFileDialog1->FileName;

 		// Checking if the file exists
		if (FileExists(m_filename))
		{
			barCode3FileLabel->SetTextBuf(m_filename.c_str());
			OpenList3Button->Enabled = true;
		}
		else
		{
			m_filename = "";
			MessageDlg("File does not exist", mtError, TMsgDlgButtons() << mbOK, 0);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmViewListMain::findBarCodesButtonClick(TObject *Sender)
{
	OpenTextFileDialog1->Filter = "Text files (*.txt)|*.TXT|Any file (*.*)|*.*";

	if (OpenTextFileDialog1->Execute(this->Handle))
	{
		m_filename = OpenTextFileDialog1->FileName;

 		// Checking if the file exists
		if (FileExists(m_filename))
		{
			barCodeFileLabel->SetTextBuf(m_filename.c_str());
			OpenList2Button->Enabled = true;
		}
		else
		{
			m_filename = "";
			MessageDlg("File does not exist", mtError, TMsgDlgButtons() << mbOK, 0);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmViewListMain::OpenList3ButtonClick(TObject *Sender)
{
	std::vector<String> sample_barcodes;
	if (!loadBarCodesFile(m_filename, sample_barcodes))
	{
		return;
	}

	util_mapHolder *mapHolder_ProjectData = static_cast<util_mapHolder * >(ProjectNamesComboBox3->Items->Objects[ProjectNamesComboBox3->ItemIndex]);
	String project_id = mapHolder_ProjectData->m_map[database::DEFINE_PROJECT_CID];

	//new member diaglog box
	std::auto_ptr<TlistDialog> listDialog = std::auto_ptr<TlistDialog>( new TlistDialog(this) );
	try
	{
		listDialog->setup(&m_retrievalList,sample_barcodes,project_id);
		listDialog->ShowModal();
	}
	catch( const String& msg )
	{
		String displayString = String("Failed - \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
//---------------------------------------------------------------------------
void TfrmViewListMain::fillExerciseListComboBox()
{
	clearComboBox(exerciseListComboBox);
	try
	{
		std::map<int,std::map<String,String> > newExerciseLists;
		m_retrievalList.getExerciseLists(newExerciseLists);

//ADD DEFULT ONES
		String name = "Display all";
		exerciseListComboBox->AddItem(name,NULL);

		std::map<int,std::map<String,String> >::iterator it;
		for (it = newExerciseLists.begin(); it != newExerciseLists.end(); ++it)
		{
			std::map<String,String>  &data = it->second;
			util_mapHolder *ValueHolder = new util_mapHolder(data);
			String s = ""+ data[database::DEFINE_OBJECT_NAME_EXTERNAL_NAME] + " -  " + data[database::DEFINE_OBJECT_NAME_EXTERNAL_FULL];
			exerciseListComboBox->AddItem(s,ValueHolder);
		}
		exerciseListComboBox->ItemIndex = 0;
	}
	catch( const String& msg )
	{
		String displayString = String("Failed to acess database, please try again- \n\n Reason :\n") + msg;
		Application->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"Error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TfrmViewListMain::exerciseListComboBoxChange(TObject *Sender)
{
//exerciseListComboBox
	int index = exerciseListComboBox->ItemIndex;
	if (index == -1)
		return;

	if (index == 0) {
		fillRetrivalListComboBox();
		return;
	}

	util_mapHolder *mapHolder = static_cast<util_mapHolder * >(exerciseListComboBox->Items->Objects[index]);

	String exercise_id = mapHolder->m_map[database::DEFINE_OBJECT_NAME_CID];
	fillRetrivalListComboBox(exercise_id.ToInt());
//now fill in other combo
}
//---------------------------------------------------------------------------

void __fastcall TfrmViewListMain::FormDestroy(TObject *Sender)
{
	clearComboBox(aliquitListComboBox);
	clearComboBox(retrivalListComboBox);
	clearComboBox(ProjectNamesComboBox);
 //	clearComboBox(ProjectNamesComboBox3);// HAS THE SAME OBJECTS IN AS THE ONE ABOVE, NO NEED TO CLEAR
	clearComboBox(exerciseListComboBox);
}
//---------------------------------------------------------------------------

