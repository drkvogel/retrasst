//---------------------------------------------------------------------------

#ifndef RatTanksCtrlH
#define RatTanksCtrlH

#include <vcl.h>
#pragma hdrstop

class TCreateRetrievalJobForm;

//---------------------------------------------------------------------------
#include "RatTanksDatabase.h"


namespace RationaliseTanks
{

class ratTanksCtrl
{
public:
	ratTanksCtrl();
	~ratTanksCtrl();

	bool hasSelectedDB(void);
	void initalise(String &selectedDB,TMemo *debugMemo);

	void getProjectBoxSizeList(std::map<std::string, std::map<std::string,std::string> > &BoxSizes);
	void getSiteList(std::map<int,std::map<String,String> > &siteList);
	void getProjectsList(std::map<std::string, std::map<std::string,std::string> > &projectList);
 	void getAliquotList(std::string &project_cid,std::map<std::string, std::map<std::string,std::string> > &aliquotList);
	void getAvalibleBoxesForReBox(const std::string &project_cid,const std::string &aliquot,std::map<std::string, std::map<std::string,std::string> > &returnInfo);

	void signIn(const std::string &userName,std::string &password,operatorSignedIn &OperatorSignedIn);
	void LIMSSignIn(const std::string &userID,operatorSignedIn &OperatorSignedIn);

	bool compileResults(const std::string &distination_box_size_cid,TForm *parent);
	void scanVessel(std::map<String,String> &vessel,TMemo *Memo,TForm *parent);
	bool refineSearch(int threshhold,TForm *parent);
	void analyseVessel(TMemo *Memo,std::string &project_cid,std::string &alquoit_cid,TForm *parent);
	void analiseForDatabaseUpdates(TMemo *Memo,const std::string &project_cid,TForm *parent);

	void createRetrievalList(const std::string &external_name,const std::string &description, const std::string &c_project_cid, const std::string &c_aliquot_cid ,const std::string &reboxType,TProgressBar *ProgressBar, const std::string &Exercise_cid);
	void getAllAliquotsFromCentral(std::map<std::string, std::map<std::string,std::string> >  &aliquotList);
	void getVesslList(std::map<int,std::map<String,String> > &vesslList,std::map<String,String> &fromSite);

	void getExerciseList(std::map<std::string, std::map<std::string,std::string> > &exerciseList);
	bool checkForCombiningBoxesInTheAliquot(bool useTheDefaultBox);

	std::string getSelectedProject();
	std::string getSelectedAliquot();
	std::string getDefaultBoxType();
	std::string getSelectedTargetBox();
	std::string getSelectedTargetBoxSize();

	std::string getProjectActivityStatus(const std::string &project_cid);
	std::string  CreateRetrivalExercise(std::string &external_name,std::string &discription);
	std::string addBoxContext(const std::string &external_name,const std::string &discripton,const std::string &box_size_cid,const std::string &aliquottpye1,const std::string &aliquottpye2,const std::string &aliquottpye3);

	std::map<std::string,std::map<std::string,std::string> > getBoxTypes(std::string &project,std::string &aliquot);

	operatorSignedIn *getOperatorSignedIn() {return m_pOperatorSignedIn;}
private:
	database m_database;
	String m_selectedDB;
	operatorSignedIn *m_pOperatorSignedIn;
};

}

#endif
