//---------------------------------------------------------------------------

#pragma hdrstop

#include "rationaliseTanks.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace RationaliseTanks;
//---------------------------------------------------------------------------


//---------------------------------------------------------------------------
rationaliseTanks::rationaliseTanks()
{

}
//---------------------------------------------------------------------------

rationaliseTanks::~rationaliseTanks()
{
}

//---------------------------------------------------------------------------

void rationaliseTanks::createRetrievalList(const std::string &external_name,const std::string &description, const std::string &c_project_cid, const std::string &c_aliquot_cid ,std::map<std::string,std::string> &distination_box_type)
{
	Screen->Cursor = crSQLWait;
	m_database.createRetrievalList(external_name,description,c_project_cid,c_aliquot_cid,distination_box_type);
	Screen->Cursor = crDefault;
}

//---------------------------------------------------------------------------
bool rationaliseTanks::hasSelectedDB()
{
	return !m_selectedDB.IsEmpty();
}

//---------------------------------------------------------------------------
void rationaliseTanks::initalise(String &selectedDB)
{
	m_selectedDB = selectedDB;
	m_database.connect(m_selectedDB);
}
//---------------------------------------------------------------------------

void rationaliseTanks::getSiteList(std::map<int,std::map<String,String> > &siteList)
{
	m_database.getSiteList(siteList);
}
//---------------------------------------------------------------------------

void rationaliseTanks::getProjectsList(std::map<std::string, std::map<std::string,std::string> > &projectList)
{
	m_database.getProjectsList(projectList);
}
//---------------------------------------------------------------------------

void rationaliseTanks::getAliquotList(std::string &project_cid,std::map<std::string, std::map<std::string,std::string> > &aliquotList)
{
	m_database.getAliquotList(project_cid,aliquotList);
}
//---------------------------------------------------------------------------

void rationaliseTanks::getAvalibleBoxesForReBox(const std::string &project_cid,const std::string &aliquot,std::map<std::string, std::map<std::string,std::string> > &returnInfo)
{
	m_database.getAvalibleBoxesForReBox(project_cid,aliquot,returnInfo);
}
//---------------------------------------------------------------------------

void rationaliseTanks::signIn(const std::string &userName, std::string &password,operatorSignedIn &OperatorSignedIn)
{
	m_database.signIn(userName,password,OperatorSignedIn);
}

//---------------------------------------------------------------------------
void rationaliseTanks::getVesslList(std::map<int,std::map<String,String> > &vesslList,std::map<String,String> &fromSite)
{
	m_database.getVesslList(vesslList,fromSite);
}

//---------------------------------------------------------------------------
void rationaliseTanks::analyseVessel(TMemo *Memo, std::string &project_cid,std::string &alquoit_cid)
{
	Screen->Cursor = crSQLWait;
	m_database.analyseVessel(Memo,project_cid,alquoit_cid);
	Screen->Cursor = crDefault;
}

//---------------------------------------------------------------------------
void rationaliseTanks::scanVessel(std::map<String,String> &vessel,TMemo *Memo)
{
	Screen->Cursor = crSQLWait;
	m_database.scanVessel(vessel,Memo);
	Screen->Cursor = crDefault;
}

//---------------------------------------------------------------------------
void rationaliseTanks::analiseForDatabaseUpdates(TMemo *Memo)
{
	Screen->Cursor = crSQLWait;
	m_database.analiseForDatabaseUpdates(Memo);
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

std::string rationaliseTanks::getProjectActivityStatus(const std::string &project_cid)
{
	return m_database.getProjectActivityStatus(project_cid);
}

std::string rationaliseTanks::getSelectedProject()
{
	return m_database.getSelectedProject();
}
//---------------------------------------------------------------------------

std::string rationaliseTanks::getSelectedAliquot()
{
	return m_database.getSelectedAliquot();
}
//---------------------------------------------------------------------------

bool rationaliseTanks::compileResults(const std::string &distination_box_size_cid)
{
	Screen->Cursor = crSQLWait;
	bool res = m_database.compileResults(distination_box_size_cid);
	Screen->Cursor = crDefault;
	return res;
}
//---------------------------------------------------------------------------

void rationaliseTanks::refineSearch(int threshhold)
{
	Screen->Cursor = crSQLWait;
	m_database.refineSearch(threshhold);
	Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

std::string rationaliseTanks::getDefaultBoxType()
{
	return m_database.getDefaultBoxType();
}
//---------------------------------------------------------------------------

bool rationaliseTanks::checkForCombiningBoxesInTheAliquot(bool useTheDefaultBox)
{
	return m_database.checkForCombiningBoxesInTheAliquot(useTheDefaultBox);
}
//---------------------------------------------------------------------------

std::string rationaliseTanks::getSelectedTargetBox()
{
	return m_database.getSelectedTargetBox();
}
//---------------------------------------------------------------------------

std::string rationaliseTanks::getSelectedTargetBoxSize()
{
	return m_database.getSelectedTargetBoxSize();
}
//---------------------------------------------------------------------------


