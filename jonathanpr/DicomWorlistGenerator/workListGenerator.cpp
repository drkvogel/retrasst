#include "workListGenerator.h"
#include <sstream>
#include <tlhelp32.h>
#include <System.DateUtils.hpp>
#include "vcl.h"

const std::string workListGenerator::CONST_DUMP2DCM = "dump2dcm.exe";
const std::string workListGenerator::CONST_WLMSCPFS = "wlmscpfs.exe";
const std::string workListGenerator::CONST_FINDSCU  = "findscu.exe";
const std::string workListGenerator::CONST_TEMP_DUMP_FILE = "wklist.dump";
const std::string workListGenerator::CONST_WORKLIST_FILE = "wklist.wl";
const std::string workListGenerator::CONST_PORT = "8673";

#define RETURN_SUCCESS std::string("")

workListGenerator::workListGenerator(const std::string &RootUID,const std::string &Directory) : XERROR( "workListGenerator" )
{
	m_Directory = Directory;
	m_UIDRoot = RootUID;

//validate inputs
	if (m_UIDRoot.empty())
	{
		setInvalid(generateError("rootUID is empty.", __LINE__, __FILE__));
		return;
	}
	if (m_Directory.empty())
	{
		setInvalid(generateError("Directory is empty.", __LINE__, __FILE__));
		return;
	}

	//MAKE SURE the root directory has a / at the end
	m_Directory = shouldHaveSlashOnEndOfDirectory(m_Directory,true);

	//check m_worklistDirectory is a valid directory
	std::string errorMSG = validateDirectory(m_Directory, false);
	if (errorMSG != RETURN_SUCCESS)
	{
		setInvalid(errorMSG);
		return;
	}
}

workListGenerator::~workListGenerator()
{
}

std::string workListGenerator::generateError(std::string ErrorString, int line, char *file)
{
	std::stringstream ssErrorReport;

	ssErrorReport << file << ":" << line << " - " << ErrorString;
	return ssErrorReport.str();
}

std::string workListGenerator::saveWorkList(const std::string &AETitle,const std::string &patientID,ROSETTA &optionalInformation)
{
	int type = optionalInformation.getType(ROSETTA_CONST_ROSSETTA_MODALITY);
	if (type == ROSETTA::typeError)
		return generateError("Modality isn't defined", __LINE__, __FILE__);

	ROSETTA *Mods = optionalInformation.getRosetta(ROSETTA_CONST_ROSSETTA_MODALITY);

	if (Mods->flagExists(MODALITY_ComputedTomography))
		m_modalitiesList.push_back(MODALITY_ComputedTomography);
	if (Mods->flagExists(MODALITY_ComputedRadiography))
		m_modalitiesList.push_back(MODALITY_ComputedRadiography);
	if (Mods->flagExists(MODALITY_MagneticResonance))
		m_modalitiesList.push_back(MODALITY_MagneticResonance);
	if (Mods->flagExists(MODALITY_Ultrasound))
		m_modalitiesList.push_back(MODALITY_Ultrasound);
	if (Mods->flagExists(MODALITY_Other))
		m_modalitiesList.push_back(MODALITY_Other);

	if (m_modalitiesList.empty())
		return generateError("Modality list is empty.", __LINE__, __FILE__);

	if ((m_UIDRoot.empty()) || (m_Directory.empty()))
		return generateError("setup has not been called or state is invalide.", __LINE__, __FILE__);

	if (AETitle.empty())
		return generateError("AETitle is emtpy.", __LINE__, __FILE__);

	if (patientID.empty())
		return generateError("PatientID is empty.", __LINE__, __FILE__);

	//check server is running
	std::string errorMSG = checkServer();
	if (errorMSG != RETURN_SUCCESS)
		return generateError(errorMSG, __LINE__, __FILE__);

//check temp dumpfile exsists.. if it does, something went wrong before.. or it's being used!
	String TempFile = String((m_Directory+ AETitle+ "\\"+CONST_TEMP_DUMP_FILE).c_str());
	if (FileExists(TempFile))
	{
		//obviously as error as this file hasn't been cleaned up.
		//if there is any logging, log it, other wise delete it and continue

		//LOG("Dump file found in temp, possible error somewhere in the system, perhaps it asserted in last run?");
		if (!DeleteFile(TempFile))
			return generateError("can't delete " + CONST_TEMP_DUMP_FILE, __LINE__, __FILE__);
	}

//check AETitle directory is good, clear out any old worklist
	std::string workListDestination = m_Directory + AETitle+ "\\";
	errorMSG = validateDirectory(workListDestination, true);
	if (errorMSG != RETURN_SUCCESS)
		return generateError("directory error[" + workListDestination + "] - " + errorMSG, __LINE__, __FILE__);

	for (unsigned int i=0;i<m_modalitiesList.size();i++)
	{
		errorMSG = generateWorkList(AETitle,patientID,m_modalitiesList.at(i),optionalInformation);
		if (errorMSG != RETURN_SUCCESS)
			return errorMSG;
	}
	return RETURN_SUCCESS;
}

std::string workListGenerator::launchTestTool(const std::string &AETitle)
{
	std::string exe = m_Directory + CONST_FINDSCU;
	std::string params = std::string("/k ") + exe.c_str() + std::string(" -v --call ") + AETitle + " localhost " + CONST_PORT.c_str() + " " + m_Directory +  "wklistqry.dcm";
	HINSTANCE hInst= ShellExecute(0, "open", "cmd.exe" ,params.c_str(), NULL, SW_NORMAL);

	if ((int)hInst <= 32)
		return "Failed to launch:" + std::string(AnsiString(CONST_FINDSCU.c_str()).c_str());
	return RETURN_SUCCESS;
}

std::string workListGenerator::checkServer()
{
 //if server not running, kick it off here.
	HANDLE hSnapshot=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	if (hSnapshot==INVALID_HANDLE_VALUE)
		return "Failed to get process list";

	bool found = false;
	PROCESSENTRY32 pe;
	pe.dwSize=sizeof(PROCESSENTRY32);

	BOOL retval=Process32First(hSnapshot,&pe);
	while(retval)
	{
		if (String(pe.szExeFile) == String("wlmscpfs.exe"))
		{
			found = true;
			break;
		}
		pe.dwSize=sizeof(PROCESSENTRY32);
		retval=Process32Next(hSnapshot,&pe);
	}
	CloseHandle(hSnapshot);

	if (!found)
	{
//Remove any \\ or / from end of directory path or it doesn't run correctly
		std::string wlDir = shouldHaveSlashOnEndOfDirectory(m_Directory,false);
		std::string params = "-v -dfp " + wlDir + " " + CONST_PORT.c_str();

//if you need to debug the server, change the flag parm from SW_HIDE to SW_NORMAL
		HINSTANCE hInst= ShellExecute(0, "open", (m_Directory + CONST_WLMSCPFS).c_str(), params.c_str(), NULL, SW_HIDE);//SW_NORMAL);
		if ((int)hInst <= 32)
			return "Failed to launch:" + std::string(AnsiString(CONST_WLMSCPFS.c_str()).c_str());
	}
	return RETURN_SUCCESS;
}

std::string workListGenerator::validateDirectory(const std::string &dir, bool WLDirectory)
{
	String StringDir = String(dir.c_str());
	// if direcotry not there.. create
	if (!DirectoryExists(StringDir))
	{
		if (!WLDirectory)
			return generateError("Was expecting directory: - " + dir, __LINE__, __FILE__);
		if (!CreateDir(StringDir))
			return generateError("Creating directory - " + dir, __LINE__, __FILE__);
	}

	std::string stdTempFile = dir + "temp.temp";
	UnicodeString UniCodeTempFile = UnicodeString(stdTempFile.c_str());

	//check can write, then clean up
	NativeUInt handle = FileCreate(UniCodeTempFile);
	if (handle < 0)
		return generateError("Creating file - " + stdTempFile, __LINE__, __FILE__);

	PAnsiChar  c = new AnsiChar[2];
	*c = '@';
	int writtenSize = FileWrite(handle,c, 1);
	if (writtenSize != 1)
	{
		FileClose(handle);
		return generateError("Writing to file - " + stdTempFile, __LINE__, __FILE__);
	}
	delete []c;

	FileClose(handle);
	if (!DeleteFile(UniCodeTempFile))
		return generateError("Deleting - " + stdTempFile, __LINE__, __FILE__);

	if (WLDirectory)
	{
		//if a work list directory, remove all previousl worklist files.
		//We only want the new work list
		TSearchRec F;
		UnicodeString UCSsearch = dir.c_str() + UnicodeString("\*.wl");
		try{
			if (FindFirst(UCSsearch, faNormal, F) == 0)
			{
				if (!DeleteFile(UnicodeString(dir.c_str()) + F.Name))
					return generateError("Deleting - " + std::string(AnsiString(F.Name.c_str()).c_str()), __LINE__, __FILE__);

				while (FindNext(F) == 0)
				{
					if (!DeleteFile(UnicodeString(dir.c_str()) + F.Name))
						return generateError("Deleting - " + std::string(AnsiString(F.Name.c_str()).c_str()), __LINE__, __FILE__);
				}
			}
		}
		__finally
		{
			FindClose(F);
		}

		//if we are removing wl files, it means it's a worklist directory.
		//we need to make sure we have a lockfile for the server to work.
		UCSsearch = dir.c_str() + UnicodeString("lockfile");
		try
		{
			if (FindFirst(UCSsearch, faNormal, F) == 0)
			{
				//we have a lock file.. relax
			}
			else
			{ //no lock
				//check can write, clean up
				std::string stdTempFile = dir + "lockfile";
				UnicodeString UniCodeTempFile = UnicodeString(stdTempFile.c_str());
				NativeUInt handle = FileCreate(UniCodeTempFile);
				if (handle < 0)
					return generateError("Creating file lockfile", __LINE__, __FILE__);
				FileClose(handle);
			}
		}
		__finally
		{
			FindClose(F);
		}
	}
	return RETURN_SUCCESS;
}

std::string workListGenerator::generateWorkList(const std::string &AETitle,const std::string &patientID, std::string modality,ROSETTA &r)
{
	int WeightKg = 0;
	int type = r.getType( ROSETTA_CONST_INT_WEIGHTKG);
	if (type != ROSETTA::typeError)
	{
		WeightKg = r.getInt(ROSETTA_CONST_INT_WEIGHTKG);
		if ((WeightKg < 2) || (WeightKg > 600)) //560kg is the fattest man on the planet apparently, 2.1 is lightest ever person...
			return generateError("Weight is out of range", __LINE__, __FILE__);
	}

	std::stringstream ss;
	ss << WeightKg;
	std::string sWeightKg = ss.str();

	std::string PatientFirstName = "Bio";
	type = r.getType( ROSETTA_CONST_STRING_FIRSTNAME);
	if (type != ROSETTA::typeError)
		PatientFirstName = r.getString(ROSETTA_CONST_STRING_FIRSTNAME);

	std::string PatientLastName = "Bank";
	type = r.getType( ROSETTA_CONST_STRING_LASTNAME);
	if (type != ROSETTA::typeError)
		PatientLastName = r.getString(ROSETTA_CONST_STRING_LASTNAME);

	System::TDate FakeDate = System::Sysutils::EncodeDate(1999,12,31);   //NEW YEARS EVE 1999!
	TDateTime now = TDateTime::CurrentDateTime();

//file name is now  {Dir}/{AETitle}/{Modality}WorkList.wl
	std::string workListFile = m_Directory + AETitle+ "\\" +modality + CONST_WORKLIST_FILE;
	std::string DumpFile = m_Directory+ AETitle+ "\\"+CONST_TEMP_DUMP_FILE;
	UnicodeString unicodeDumpFile = DumpFile.c_str();

	TStreamWriter *SWriter;
	SWriter = new TStreamWriter(unicodeDumpFile,false);
	SWriter->WriteLine("(0010,0010) PN " + String(PatientLastName.c_str())+String("^")+String(PatientFirstName.c_str()));     //PATIENT NAME
	SWriter->WriteLine("(0010,0020) LO " + String(patientID.c_str()));       //PATIENT ID
	SWriter->WriteLine("(0010,0030) DA " + FakeDate.FormatString("yyyymmdd")); // P DOB - NEW YEARS EVE 1999
	SWriter->WriteLine(String("(0010,0040) CS O "));    //P SEX - OTHER
 //	SWriter->WriteLine("(0010,1020) DS " + data.PatientSize);  // size
	if (WeightKg> 0)
		SWriter->WriteLine("(0010,1030) DS " + String(sWeightKg.c_str()));  //weight
	SWriter->WriteLine("(0020,000d) UI " + String(m_UIDRoot.c_str()) + now.FormatString("yyyymmddhhmmss"));
	SWriter->WriteLine(String("(0032,1060) LO  BioBank Scan "));
	SWriter->WriteLine(String("(0040,0100) SQ "));
	SWriter->WriteLine(String("(fffe,e000) - "));
	SWriter->WriteLine("(0008,0060) CS " + String(modality.c_str()) );    //MODALITY
	SWriter->WriteLine(String("(0032,1070) LO "));
	SWriter->WriteLine("(0040,0001) AE " + String(AETitle.c_str()));     //ScheduledStationAETitle
	SWriter->WriteLine("(0040,0002) DA " + System::Dateutils::Today().FormatString("yyyymmdd"));//    ScheduledProcedureStepStartDate
	SWriter->WriteLine("(0040,0003) TM " + System::Dateutils::Today().FormatString("hhmmss"));//    ScheduledProcedureStepStartTime
	SWriter->WriteLine(String("(0040,0007) LO BioBank Scan"));     //     ScheduledProcedureStepDescription
	SWriter->WriteLine(String("(0040,0009) SH 0000"));  //       ScheduledProcedureStepID
	SWriter->WriteLine(String("(fffe,e00d) - "));
	SWriter->WriteLine(String("(fffe,e0dd) - "));
	SWriter->WriteLine(String("(0040,1001) SH NONE"));
	SWriter->Close();

	std::string cmd = DumpFile + std::string(" ") + workListFile;

	std::string exeLocation = m_Directory + CONST_DUMP2DCM;

	SHELLEXECUTEINFO lpExecInfo;
	lpExecInfo.cbSize  = sizeof(SHELLEXECUTEINFO);
	lpExecInfo.lpFile = exeLocation.c_str();
	lpExecInfo.fMask= SEE_MASK_DOENVSUBST|SEE_MASK_NOCLOSEPROCESS|SEE_MASK_FLAG_NO_UI ;
	lpExecInfo.hwnd = 0;
	lpExecInfo.lpVerb = "open"; // to open  program
	lpExecInfo.lpParameters = cmd.c_str();
	lpExecInfo.lpDirectory = NULL;
	lpExecInfo.nShow = SW_HIDE;
	lpExecInfo.hInstApp = 0;

	if (!ShellExecuteEx(&lpExecInfo))
	{
		return generateError(getLastErrorString(), __LINE__, __FILE__);
	}

	//wait until complete
	if(lpExecInfo.hProcess !=NULL)
	{
		::WaitForSingleObject(lpExecInfo.hProcess, INFINITE);
		::CloseHandle(lpExecInfo.hProcess);
	}
	DeleteFile(unicodeDumpFile);

	//CHECK WORLLIST.WL IS THERE AND HAS SOME SIZE
	NativeUInt handle = FileOpen(workListFile.c_str(), fmOpenRead);
	if (handle < 0)
		return generateError("no output worklist found - " + workListFile, __LINE__, __FILE__);

	int size = FileSeek(handle, 0, 2);
	FileClose(handle);
	if (size < 250)  //normally around 500-600 bytes
		return generateError("worklist file may have failed to be created sucessfully - " + workListFile, __LINE__, __FILE__);
 	return RETURN_SUCCESS;
}


std::string workListGenerator::getLastErrorString()
{
	std::string returnError = "";
	DWORD err = GetLastError();
	LPTSTR s;

	if (FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER |  FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,NULL, err, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR) &s, 	0, NULL ) == 0)
	{
	//failed
    	returnError = "Unknown error";
	}
	else
	{
		LPTSTR p = _tcschr(s, _T('\r'));
		if(p != NULL)
		{ /* lose CRLF */
			*p = _T('\0');
		} /* lose CRLF */
		String ss = s;
		returnError = AnsiString(ss.c_str()).c_str();
		LocalFree(s);
	}
	return returnError;
}

std::string workListGenerator::shouldHaveSlashOnEndOfDirectory(const std::string &dir,bool slash)
{
	std::string resultDir = dir;
	if (slash)
	{
		if ((resultDir[resultDir.length()-1] != '\\') && (resultDir[resultDir.length()-1] != '/'))
			resultDir += std::string("\\");
	}
	else
	{
		if ((resultDir[resultDir.length()-1] == '\\') || (resultDir[resultDir.length()-1] == '/'))
			resultDir = resultDir.substr(0,resultDir.length()-1);
	}
	return resultDir;
}



