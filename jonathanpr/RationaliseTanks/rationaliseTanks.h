//---------------------------------------------------------------------------

#ifndef rationaliseTanksH
#define rationaliseTanksH

#include <vcl.h>
#pragma hdrstop

//---------------------------------------------------------------------------
#include "database.h"


namespace RationaliseTanks
{

class rationaliseTanks
{
public:
	rationaliseTanks();
	~rationaliseTanks();

	bool hasSelectedDB(void);
	void initalise(String &selectedDB);

	void getSiteList(std::map<int,std::map<String,String> > &siteList);
	void getProjectsList(std::map<std::string, std::map<std::string,std::string> > &projectList);
 	void getAliquotList(std::string &project_cid,std::map<std::string, std::map<std::string,std::string> > &aliquotList);
	void getAvalibleBoxesForReBox(const std::string &project_cid,const std::string &aliquot,std::map<std::string, std::map<std::string,std::string> > &returnInfo);

	void signIn(const std::string &userName,std::string &password,operatorSignedIn &OperatorSignedIn);
	bool compileResults(const std::string &distination_box_size_cid);

	std::string getSelectedProject();
	std::string getSelectedAliquot();

	void createRetrievalList(const std::string &external_name,const std::string &description, const std::string &c_project_cid, const std::string &c_aliquot_cid ,std::map<std::string,std::string> &distination_box_type);

	void getVesslList(std::map<int,std::map<String,String> > &vesslList,std::map<String,String> &fromSite);
    void scanVessel(std::map<String,String> &vessel,TMemo *Memo);
	void analyseVessel(TMemo *Memo,std::string &project_cid,std::string &alquoit_cid);
	void analiseForDatabaseUpdates(TMemo *Memo);
	void refineSearch(int threshhold);

	std::string getDefaultBoxType();

	bool checkForCombiningBoxesInTheAliquot(bool useTheDefaultBox);

	std::string getSelectedTargetBox();
	std::string getSelectedTargetBoxSize();

	std::string getProjectActivityStatus(const std::string &project_cid);

private:
	database m_database;
	String m_selectedDB;
};

}

#endif
