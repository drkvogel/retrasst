//---------------------------------------------------------------------------

#pragma hdrstop

#include "RatTanksCtrl.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace RationaliseTanks;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
ratTanksCtrl::ratTanksCtrl()
{

}
//---------------------------------------------------------------------------

ratTanksCtrl::~ratTanksCtrl()
{
}
//---------------------------------------------------------------------------

void ratTanksCtrl::createRetrievalList(const std::string &external_name,const std::string &description, const std::string &c_project_cid,const std::string &reason,const std::string &c_aliquot_cid ,const std::string &reboxType,TProgressBar *ProgressBar,const std::string &exercise_cid)
{
	Screen->Cursor = crSQLWait;
	m_database.createRetrievalList(external_name,description,c_project_cid,reason,c_aliquot_cid,reboxType,ProgressBar,exercise_cid);
	Screen->Cursor = crDefault;
}

//---------------------------------------------------------------------------
void ratTanksCtrl::getAllAliquotsFromCentral(std::map<std::string, std::map<std::string,std::string> >  &aliquotList)
{
	m_database.getAllAliquotsFromCentral(aliquotList);
}

//---------------------------------------------------------------------------
bool ratTanksCtrl::hasSelectedDB()
{
	return !m_selectedDB.IsEmpty();
}
//---------------------------------------------------------------------------
void ratTanksCtrl::LIMSSignIn(const std::string &userID,operatorSignedIn &OperatorSignedIn)
{
	m_pOperatorSignedIn =&OperatorSignedIn;
	m_database.LIMSSignIn(userID,OperatorSignedIn);
}
//---------------------------------------------------------------------------
void ratTanksCtrl::initalise(String &selectedDB,TMemo *debugMemo)
{
	m_selectedDB = selectedDB;
	m_database.connect(m_selectedDB,debugMemo);
}
//---------------------------------------------------------------------------

void ratTanksCtrl::getSiteList(std::map<int,std::map<String,String> > &siteList)
{
	m_database.getSiteList(siteList);
}
//---------------------------------------------------------------------------

void ratTanksCtrl::getProjectsList(std::map<std::string, std::map<std::string,std::string> > &projectList)
{
	m_database.getProjectsList(projectList);
}
//---------------------------------------------------------------------------

void ratTanksCtrl::getAliquotList(std::string &project_cid,std::map<std::string, std::map<std::string,std::string> > &aliquotList)
{
	m_database.getAliquotList(project_cid,aliquotList);
}
//---------------------------------------------------------------------------

void ratTanksCtrl::getAvalibleBoxesForReBox(const std::string &project_cid,const std::string &aliquot,std::map<std::string, std::map<std::string,std::string> > &returnInfo)
{
	m_database.getAvalibleBoxesForReBox(project_cid,aliquot,returnInfo);
}
//---------------------------------------------------------------------------

void ratTanksCtrl::signIn(const std::string &userName, std::string &password,operatorSignedIn &OperatorSignedIn)
{
	m_pOperatorSignedIn =&OperatorSignedIn;
	m_database.signIn(userName,password,OperatorSignedIn);
}

//---------------------------------------------------------------------------
void ratTanksCtrl::getVesslList(std::map<int,std::map<String,String> > &vesslList,std::map<String,String> &fromSite)
{
	m_database.getVesslList(vesslList,fromSite);
}

//---------------------------------------------------------------------------
void ratTanksCtrl::analyseVessel(TMemo *Memo, std::string &project_cid,std::string &alquoit_cid,TForm *parent)
{
	parent->Enabled = false;
	Screen->Cursor = crSQLWait;
	m_database.analyseVessel(Memo,project_cid,alquoit_cid);
	Screen->Cursor = crDefault;
	parent->Enabled = true;
}

//---------------------------------------------------------------------------
void ratTanksCtrl::getExerciseList(std::map<std::string, std::map<std::string,std::string> > &exerciseList)
{
	m_database.getExerciseList(exerciseList);
}

//---------------------------------------------------------------------------
std::string ratTanksCtrl::CreateRetrivalExercise(std::string &external_name,std::string &discription)
{
	return m_database.CreateRetrivalExercise(external_name, discription);
}

//---------------------------------------------------------------------------
void ratTanksCtrl::scanVessel(std::map<String,String> &vessel,TMemo *Memo,TForm *parent)
{
	parent->Enabled = false;
	Screen->Cursor = crSQLWait;
	m_database.scanVessel(vessel,Memo);
	Screen->Cursor = crDefault;
	parent->Enabled = true;
}

//---------------------------------------------------------------------------
void ratTanksCtrl::analiseForDatabaseUpdates(TMemo *Memo,const std::string &project_cid,TForm *parent)
{
	parent->Enabled = false;
	Screen->Cursor = crSQLWait;
	m_database.analiseForDatabaseUpdates(Memo,project_cid);
	Screen->Cursor = crDefault;
	parent->Enabled = true;
}
//---------------------------------------------------------------------------

std::string ratTanksCtrl::getProjectActivityStatus(const std::string &project_cid)
{
	return m_database.getProjectActivityStatus(project_cid);
}
//---------------------------------------------------------------------------

std::string ratTanksCtrl::getSelectedProject()
{
	return m_database.getSelectedProject();
}
//---------------------------------------------------------------------------

std::string ratTanksCtrl::getSelectedAliquot()
{
	return m_database.getSelectedAliquot();
}
//---------------------------------------------------------------------------

bool ratTanksCtrl::compileResults(const std::string &distination_box_size_cid,TForm *parent)
{
	parent->Enabled = false;
	Screen->Cursor = crSQLWait;
	bool res = m_database.compileResults(distination_box_size_cid);
	Screen->Cursor = crDefault;
	parent->Enabled = true;
	return res;
}
//---------------------------------------------------------------------------

bool ratTanksCtrl::refineSearch(int threshhold,TForm *parent)
{
	parent->Enabled = false;
	Screen->Cursor = crSQLWait;
	bool ret;
	ret = m_database.refineSearch(threshhold);
	Screen->Cursor = crDefault;
	parent->Enabled = true;
	return ret;
}

//---------------------------------------------------------------------------
void ratTanksCtrl::getProjectBoxSizeList(std::map<std::string, std::map<std::string,std::string> > &BoxSizes)
{
	return m_database.getProjectBoxSizeList(BoxSizes);
}

//---------------------------------------------------------------------------

std::string ratTanksCtrl::addBoxContext(const std::string &external_name,const std::string &discripton,const std::string &box_size_cid,const std::string &aliquottpye1,const std::string &aliquottpye2,const std::string &aliquottpye3)
{
	return m_database.addBoxContext(external_name,discripton,box_size_cid,aliquottpye1,aliquottpye2,aliquottpye3);
}

//---------------------------------------------------------------------------

std::map<std::string,std::map<std::string,std::string> > ratTanksCtrl::getBoxTypes(std::string &project,std::string &aliquot)
{
	return m_database.getBoxTypes(project,aliquot);
}
//---------------------------------------------------------------------------

std::string ratTanksCtrl::getDefaultBoxType()
{
	return m_database.getDefaultBoxType();
}
//---------------------------------------------------------------------------

bool ratTanksCtrl::checkForCombiningBoxesInTheAliquot(bool useTheDefaultBox)
{
	return m_database.checkForCombiningBoxesInTheAliquot(useTheDefaultBox);
}
//---------------------------------------------------------------------------

std::string ratTanksCtrl::getSelectedTargetBox()
{
	return m_database.getSelectedTargetBox();
}
//---------------------------------------------------------------------------

std::string ratTanksCtrl::getSelectedTargetBoxSize()
{
	return m_database.getSelectedTargetBoxSize();
}
//---------------------------------------------------------------------------


