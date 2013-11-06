//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "main.h"
#include <System.DateUtils.hpp>
#include <System.SysUtils.hpp>
#include <sstream>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TDicomWorkListGenerator *DicomWorkListGenerator;

//THE ROOT UID FOR THE BIOBANK
	const std::string ROOT_UID = "1.2.826.0.1.4978912";
//WHERE THE WORKLIST SERVER WILL LOOK FOR WORK LISTS, EACH MODALITY WILL HAVE THEIR OWN SUB-DIRECTORY
	const std::string ROOT_DIR = "c:\\wklists\\";

//---------------------------------------------------------------------------
__fastcall TDicomWorkListGenerator::TDicomWorkListGenerator(TComponent* Owner)
	: TForm(Owner)
{
	m_wlg = new workListGenerator(ROOT_UID, ROOT_DIR);

}


//---------------------------------------------------------------------------
void __fastcall TDicomWorkListGenerator::Button1Click(TObject *Sender)
{
//THE FOLLOWING CODE SETS UP THREE WORK LISTS FOR THREE SCANNERS.
//SCANNER1, SCANNER2 and SCANNER3

	int successCount = 0;

 //CHECK ERRORS IN CREATING THE WORKLIST GENERATOR OBJECT
	if (!m_wlg->isValid())
	{
		int nErrors = m_wlg->getNErrors();

		for (int i=0;i<nErrors;i++)
		{
			std::stringstream ss;
			ss << "Error: " << m_wlg->getErrorsPtr()->at(i);
			String Wss = AnsiString(ss.str().c_str());
			Application->MessageBox(Wss.c_str(), L"Setup error", MB_OK);
		}
		return;
	}

//GENERATE A WORKLIST FOR SCANNER1
//TWO MODALITIES

	std::string AETitle = "Scanner1";
	std::string PatientID = "ANONPATIENT57834";
	std::string lastName = "Price";
	int WeightKG = 91;

	{
	ROSETTA R;
	R.setString(ROSETTA_CONST_STRING_LASTNAME,lastName);
	R.setInt(ROSETTA_CONST_INT_WEIGHTKG,WeightKG);

	ROSETTA MODALITY;
	MODALITY.setFlag(MODALITY_ComputedTomography);
	R.setRosetta(ROSETTA_CONST_ROSSETTA_MODALITY,&MODALITY);

	std::string result = m_wlg->saveWorkList(AETitle,PatientID,R);
	if (result != "")
	{
		std::stringstream ss;
		ss << "Error: " << result;
		String Wss = AnsiString(ss.str().c_str());
		Application->MessageBox(Wss.c_str(), L"WorkList error", MB_OK);
	}
	else
		successCount++;
	}
 /*
	{
//GENERATE A WORKLIST FOR SCANNER3
//FOUR MODALITIES
	AETitle = "BMR";
	PatientID = "ANONPATIENT40987";
 //	std::string firstName = "Jonathan";
	std::string lastName = "Price";
	WeightKG = 91;

	ROSETTA R;
 //	R.setString(ROSETTA_CONST_STRING_FIRSTNAME,firstName);
	R.setString(ROSETTA_CONST_STRING_LASTNAME,lastName);
	R.setInt(ROSETTA_CONST_INT_WEIGHTKG,WeightKG);

	ROSETTA MODALITY;
	MODALITY.setFlag(MODALITY_ComputedTomography);
	R.setRosetta(ROSETTA_CONST_ROSSETTA_MODALITY,&MODALITY);

	std::string result = m_wlg->saveWorkList(AETitle,PatientID,R);
	if (result != "")
	{
		std::stringstream ss;
		ss << "Error: " << result;
		String Wss = AnsiString(ss.str().c_str());
		Application->MessageBox(Wss.c_str(), L"WorkList error", MB_OK);
	}
	else
		successCount++;
	}
*/
	std::stringstream ss;
	ss << "Generated " << successCount;
	String Wss = AnsiString(ss.str().c_str());
	Application->MessageBox(Wss.c_str(), L"Result", MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TDicomWorkListGenerator::Button2Click(TObject *Sender)
{
	std::string AETitle = "Scanner1";

	std::string result = m_wlg->launchTestTool(AETitle);
	if (result != "")
	{
		std::stringstream ss;
		ss << "Error: " << result;
		String Wss = AnsiString(ss.str().c_str());
		Application->MessageBox(Wss.c_str(), L"WorkList error", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TDicomWorkListGenerator::FormDestroy(TObject *Sender)
{
	if (m_wlg)
		delete m_wlg;
}
//---------------------------------------------------------------------------

