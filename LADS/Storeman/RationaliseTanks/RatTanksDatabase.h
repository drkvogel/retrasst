//---------------------------------------------------------------------------

#ifndef RatTanksDatabaseH
#define RatTanksDatabaseH
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "xdb.h"
#include "xquery.h"
#include <map>
#include <memory>

#include "RatTanksOperatorSignedIn.h"

class TCreateRetrievalJobForm;

void onError( const std::string msg );

namespace RationaliseTanks
{

	class BoxSizeStruct
	{
	public:
//Info for the box in question
		std::string box_cid;
		std::map<std::string,std::string> BoxStoreInfo;
		std::map<std::string,std::string> BoxTypeInfo;

		int NoVials; //in this box with the alquot
		int EmptySpace; //in this box in total
		int NonMovableVials; //in this box in total
		int PecentFree;
		bool multipleAlquiots;
		bool hasNonMoveableVialsOrSpace; //vials which are tagged as to be moved, or has space already allocated

		void copy(BoxSizeStruct &src)
		{
			NoVials = src.NoVials;
			EmptySpace = src.EmptySpace;
			NonMovableVials = src.NonMovableVials;
			PecentFree = src.PecentFree;
			multipleAlquiots = src.multipleAlquiots;
			hasNonMoveableVialsOrSpace = src.hasNonMoveableVialsOrSpace;
			box_cid = src.box_cid;

			BoxStoreInfo = src.BoxStoreInfo;
			BoxTypeInfo = src.BoxTypeInfo;
		}
	};

class database
{
	class AliquotInfo
	{
	public:
		AliquotInfo()
		{
			m_FreeSpace = 0;
			m_UsedSpace = 0;
			m_TotalBoxes = 0;
		}
		std::vector<BoxSizeStruct> m_AliquotsToBoxesSizeList;
		int m_FreeSpace;
		int m_UsedSpace;
		int m_TotalBoxes;
//		std::map<std::string,std::string> m_BoxTypes;
		std::map<std::string,std::map<std::string,std::string> >  m_BoxTypes;
		void copy(AliquotInfo &src)
		{
			m_FreeSpace = src.m_FreeSpace;
			m_UsedSpace = src.m_UsedSpace;
			m_TotalBoxes = src.m_TotalBoxes;

			m_BoxTypes = src.m_BoxTypes;
			for (int i=src.m_AliquotsToBoxesSizeList.size()-1;i>=0;i--)
			{
				BoxSizeStruct bs;
				bs.copy(src.m_AliquotsToBoxesSizeList[i]);
				m_AliquotsToBoxesSizeList.push_back(bs);
			}
		}
		void copy(AliquotInfo &src,int threshHold)
		{
			m_FreeSpace = src.m_FreeSpace;
			m_UsedSpace = src.m_UsedSpace;
			m_TotalBoxes = src.m_TotalBoxes;

			m_BoxTypes = src.m_BoxTypes;
			for (int i=src.m_AliquotsToBoxesSizeList.size()-1;i>=0;i--)
			{
				BoxSizeStruct bs;

				if (src.m_AliquotsToBoxesSizeList[i].PecentFree >= threshHold)
				{
					bs.copy(src.m_AliquotsToBoxesSizeList[i]);
					m_AliquotsToBoxesSizeList.push_back(bs);
				}
			}
		}
	};

	std::map<std::string , std::map<std::string ,AliquotInfo> > m_projectToAliquotsOrginal;
	std::map<std::string , std::map<std::string ,AliquotInfo> > m_projectToAliquots;

	class rackProjectData
	{
	public:
		std::map<std::string, std::map<std::string,std::string> > BoxStoreInfo;
		std::map<std::string, std::map<std::string,std::string> > BoxTypeInfo;
		std::vector<std::string> BoxTypes_cid; //keeps tack of all the box types in the rack.
	};

	class rackNumberData
	{
	public:
		rackNumberData() {totalBoxCount = -1; ourNewBoxCount = -1;}

		std::map<std::string,std::string> rackNumberInfo;
		std::map<std::string,rackProjectData> rackProjectInfo; //one for each project

	//a total of each boxes vials from all projects.
		std::map<std::string,int> BoxIDToTotalVialsFound;
		std::map<std::string,int> BoxIDToTotalProblemVialsFound;
//		std::map<std::string,int> BoxIDToTotalExtraFreeSpace;
	//number of boxes in this rack for all projects
		int totalBoxCount;
		int ourNewBoxCount;
	};

	class tankLayoutData
	{
	public:
		std::map<std::string,std::string> tanklayoutInfo;
		std::map<std::string, rackNumberData> theracks;
	};

	class VesselData
	{
	public:
		std::map<std::string,std::string> tankMapInfo;
		std::map<std::string, tankLayoutData > thetanklayouts;
	};

public:
	static const std::string DEFINE_OBJECT_NAME_CID;
	static const std::string DEFINE_OBJECT_NAME_EXTERNAL_NAME;
	static const std::string DEFINE_OBJECT_NAME_EXTERNAL_FULL;

	static const std::string DEFINE_PROGRAM_NAME;
	static const std::string DEFINE_PROGRAM_VERSION;
	database();
	~database();

	TMemo *m_debugMemo;

	void connect(String &selectDB,TMemo *debugMem);
	bool connectProject(std::string projectName);

	void getExerciseList(std::map<std::string, std::map<std::string,std::string> > &exerciseList);
	void getAllAliquotsFromCentral(std::map<std::string, std::map<std::string,std::string> >  &aliquotList);
	void getProjectBoxSizeList(std::map<std::string, std::map<std::string,std::string> > &BoxSizes);
	void getAliquotList(std::string &project_cid,std::map<std::string, std::map<std::string,std::string> > &aliquotList);
	void getProjectsList(std::map<std::string, std::map<std::string,std::string> > &projectList);
	void scanVessel(std::map<String,String> &vessel,TMemo *Memo);
	void getSiteList(std::map<int,std::map<String,String> > &siteList);
	void getVesslList(std::map<int,std::map<String,String> > &vesslList,std::map<String,String> &fromSite);
	void analyseVessel(TMemo *Memo,std::string &project_cid,std::string &alquoit_cid);
	void analiseForDatabaseUpdates(TMemo *Memo,const std::string &project_cid);
	void getAvalibleBoxesForReBox(const std::string &project_cid,const std::string &aliquot,std::map<std::string, std::map<std::string,std::string> > &returnInfo);

	void createRetrievalList(const std::string &external_name,const std::string &description, const std::string &c_project_cid, const std::string &c_aliquot_cid,const std::string &reboxType,TProgressBar *ProgressBar,const std::string &exercise_cid);
	void signIn(const std::string &userName,std::string &password,operatorSignedIn &OperatorSignedIn);
	void LIMSSignIn(const std::string &userID,operatorSignedIn &OperatorSignedIn);

	std::string getSelectedProject();
	std::string getSelectedAliquot();
	std::string getSelectedTargetBox() {return m_SelectedTargetBox;}
	std::string getSelectedTargetBoxSize() {return m_SelectedTargetBoxSize;}

	bool refineSearch(int threshhold);
	bool compileResults(const std::string &distination_box_size_cid);
	std::string getDefaultBoxType();
	bool checkForCombiningBoxesInTheAliquot(bool useTheDefaultBox);
	std::string getProjectActivityStatus(const std::string &project_cid);

	std::map<std::string,std::string> getBoxSize(std::string &box_size_cid)
	{
		return m_BoxSize[box_size_cid];
	}

	std::map<std::string,std::map<std::string,std::string> > getBoxTypes(std::string &project,std::string &aliquot)
	{
		return m_projectToAliquots[project][aliquot].m_BoxTypes;
	}
	std::string addBoxContext(const std::string &external_name,const std::string &discripton,const std::string &box_size_cid,const std::string &aliquottpye1,const std::string &aliquottpye2,const std::string &aliquottpye3);
	std::string  CreateRetrivalExercise(std::string &external_name,std::string &discription);

	static bool m_bQuitting;
private:
	bool m_bCompletedDatabaseUpdate;
	void searchForBoxTypes(bool singleType);
	bool isUnqueBoxTypeCID(const std::string &ID);
	bool isUnqueBoxCID(const std::string &ID);
	int getNoBoxesInRack(const std::string &rack_cid);
	void getOperatorPremissions();

 //	std::map<std::string,std::string> m_AliquotsBoxTypes;
	std::map<std::string,std::map<std::string,std::string> >   m_AliquotsBoxTypes;

	void cullForThreshHold(int threshhold/*,std::map<std::string,std::map<std::string,std::string> > &aliquotsBoxTypes*/);
	void getVialsForAliquot(std::map<int, std::map<std::string,std::string> > &returnInfo,const std::string &alquoit_cid,const std::string &box_cid);
//	void createRetrievalList(const std::string &external_name /*defrag_yyyymmddhhmmss*/,const std::string &description );
	std::string getNextCentralIdSequence();
	std::string getNextProjectIdSequence();
	void showMultipleBoxDialog(std::map<std::string,std::map<std::string,std::string> > &aliquotsBoxTypes);

	void getMachine_cid();
	void addAuditTrail(std::string messageType,std::string details,std::string databasename);

	void extractBoxInformation(std::string rack_id,rackProjectData *thisrack_info);
	int getNoVialForAliquot(const std::string &alquoit_cid,const std::string &box_cid);
	void analiseVialsInABox(std::map<std::string,std::string> &boxstoreInfo, int BoxTotalCap);
	void throwUnless( bool condition, const String& msg );

	String runQuery(const std::vector<std::string> &what,const std::string &tablename,const std::string &where,std::map<std::string, std::map<std::string,std::string> > &returnInfo);
	String runProjectQuery(const std::vector<std::string> &what,const std::string &tablename,const std::string &where,std::map<std::string, std::map<std::string,std::string> > &returnInfo);
	std::string getQuery(const std::vector<std::string> &what,const std::string &tablename,const std::string &where);
	bool openProject(std::string project_cid);
	std::string getBoxVialCount(const std::string &box_cid);
	int igetBoxVialCount(const std::string &box_cid);
	void getObjectNameDetails(const std::string &object_cid, std::map<std::string,std::string> &List);
//Combined database, when you have the project
	std::map<std::string, XDB *> m_dbProjects;

	std::map<std::string, std::string> m_dbProjects_cidToName;
	XDB *m_pCurrentProject;
    void openAllProjectDatabases();

	void extractBoxInformation(std::string project_cid,std::string itank_first,std::string irackInfo_first, std::string ilayoutInfo_first);
	std::map<std::string, VesselData > m_tankMaps;
//Central database
	std::unique_ptr<XDB> m_dbCentral;

	std::string IntToStdString(int i);
	String IntToString(int i);
	std::string FloatToStdString(float f);
	String FloatToString(float f);
	std::map<std::string, std::map<std::string,std::string> > m_projectInfo;

	std::map<std::string, std::map<std::string,std::string> > m_BoxSize;

	TMemo *m_Memo;
	std::string m_c_id_sequence;

	std::map<String,String> m_vessel;

	std::string m_SelectedProject_cid;
	std::string m_SelectedAlquot_cid;
	std::string m_SelectedTargetBox; //store the user box selection, which we will set as default.
	std::string m_SelectedTargetBoxSize; //store the user box selection, which we will set as default.

//infomation for the retreavil list generating
	int m_TotalBoxesToBeMoved;
	int m_TotalNewBoxes;

	operatorSignedIn *m_pOperatorSignedIn;
	void setAutoCommit(bool On);

public:
	static bool m_bIsAutoCommit;
};
 
}

#endif
