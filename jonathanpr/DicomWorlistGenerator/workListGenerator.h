#ifndef __WORKLISTGENERATOR__HPP__
#define __WORKLISTGENERATOR__HPP__
//#include <vcl.h>
#include <string>
#include "rosetta.h"
#include "xerror.h"

//OPTIONAL WORKLIST DATA
#define ROSETTA_CONST_INT_WEIGHTKG 							std::string("WEIGHT")
#define ROSETTA_CONST_STRING_FIRSTNAME						std::string("FIRSTNAME")
#define ROSETTA_CONST_STRING_LASTNAME 						std::string("LASTNAME")

//PUT ALL THE REQUESTED MODALITIES INTO THIS SUB ROSETTA AS VALUES BELOW
#define ROSETTA_CONST_ROSSETTA_MODALITY 					std::string("MODALITY")

//LIST OF SOME COMMON MODALITIES
#define  MODALITY_ComputedTomography						std::string("CT")
#define  MODALITY_ComputedRadiography  						std::string("CR")
#define  MODALITY_MagneticResonance     					std::string("MR")
#define  MODALITY_Ultrasound 								std::string("US")
#define  MODALITY_Other 									std::string("OT")


class workListGenerator : public XERROR
{
public:
//files directory now a single directory which worklists and temp files and EXEs go
	workListGenerator(const std::string &RootUID, const std::string &directory);
	~workListGenerator();

	std::string saveWorkList(const std::string &AETitle,const std::string &patientID,ROSETTA &optionalInformation);
	std::string launchTestTool(const std::string &AETitle);
private:
	static std::string shouldHaveSlashOnEndOfDirectory(const std::string &dir,bool slash);
	std::string validateDirectory(const std::string &dir, bool removeWLFiles);
	std::string generateWorkList(const std::string &AETitle,const std::string &patientID, std::string modality,ROSETTA &r);

	std::string generateError(std::string ErrorString, int line, char *file);

	std::string checkServer(void);
	std::string getLastErrorString(void);

	std::string m_UIDRoot;
	std::string m_Directory;

	std::vector<std::string> m_modalitiesList;

	const static std::string CONST_DUMP2DCM; //dump2dcm.exe
	const static std::string CONST_WLMSCPFS; //wlmscpfs.exe
	const static std::string CONST_TEMP_DUMP_FILE; //the intermediate file form which the worklist is create from
	const static std::string CONST_WORKLIST_FILE; //the filename of the final worklist file
	const static std::string CONST_PORT;
	const static std::string CONST_FINDSCU;
};

#endif

