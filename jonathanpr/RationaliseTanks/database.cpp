//---------------------------------------------------------------------------

#pragma hdrstop

#include "database.h"
#include <sstream>
#include <cstddef>
#include <string>
#include <cstdlib>
#include <sstream>
#include <System.SysUtils.hpp>
#include <algorithm>    // std::sort,std::transform
#include <xMD5.h>
#include <xexec.h>
#include "boxSelection.h"
#include "operatorSignedIn.h"
#include <System.DateUtils.hpp>
#include <System.SysUtils.hpp>
#include "assert.h"
#include <sstream>
//---------------------------------------------------------------------------
#pragma package(smart_init)

using namespace RationaliseTanks;
#define MIN_SPACE 1
//---------------------------------------------------------------------------
const std::string database::DEFINE_OBJECT_NAME_CID = "object_name_cid";
const std::string database::DEFINE_OBJECT_NAME_EXTERNAL_NAME = "object_name_ext_name";
const std::string database::DEFINE_OBJECT_NAME_EXTERNAL_FULL = "object_name_ext_full";
const std::string database::DEFINE_PROGRAM_NAME = "Rationalise tanks";
const std::string database::DEFINE_PROGRAM_VERSION = "v1.0.0";
//---------------------------------------------------------------------------
database::database()
{
	m_pCurrentProject = 0;
	m_bQuitting = false;
}
//---------------------------------------------------------------------------

database::~database()
{
    m_bQuitting = true;
	while (!m_dbProjects.empty())
	{
		std::map<std::string, XDB *>::iterator it = m_dbProjects.begin();
		delete it->second;
		m_dbProjects.erase(it);
	}
}
//---------------------------------------------------------------------------

void database::connect(String &selectDB)
{
	try
	{
		String dbName = "vnode_vlab::" + selectDB;
		m_dbCentral = std::unique_ptr<XDB>( new XDB( AnsiString(dbName.c_str()).c_str() ) );
		m_dbCentral->setErrorCallBack( dbErrorCallback );
		throwUnless ( m_dbCentral->open(), "Failed to connect!" );
	}
	catch( const std::string& msg )
	{
		onError( msg );
	}
	catch( const std::runtime_error& e )
	{
		onError( e.what() );
	}
}
//---------------------------------------------------------------------------

bool database::connectProject(std::string projectName)
{
	m_pCurrentProject = 0;

	if (m_dbProjects.find(projectName) != m_dbProjects.end())
	{
		std::map<std::string, XDB *>::iterator it = m_dbProjects.find(projectName);

		if (it != m_dbProjects.end())
		{
			m_pCurrentProject = it->second;
			return true;
		}
	}

	std::string connectionString = std::string("vnode_vlab::") + projectName;
	m_pCurrentProject = new XDB(connectionString);

	try
	{
		m_pCurrentProject->setErrorCallBack( dbErrorCallback );
		throwUnless ( m_pCurrentProject->open(), "Failed to connect!" );
		m_dbProjects[projectName] = m_pCurrentProject;
		return true;
	}
	catch( const std::string& msg )
	{
		onError( msg );
	}
	catch( const std::runtime_error& e )
	{
		onError( e.what() );
	}
	return false;
}

//---------------------------------------------------------------------------
void database::getProjectsList(std::map<std::string, std::map<std::string,std::string> > &projectList)
{
	projectList.clear();

	std::map<std::string , std::map<std::string ,AliquotInfo> >::iterator it = m_projectToAliquotsOrginal.begin();
	for (; it!=m_projectToAliquotsOrginal.end(); ++it)
	{
		std::string proj = it->first;

		std::vector<std::string> what;
		what.push_back("project_cid");
		what.push_back("external_name");
		what.push_back("external_full");
		what.push_back("activity_flags");
		what.push_back("db_name");
		std::string tablename = "C_PROJECT";
		std::string where = "project_cid  = '" + proj  + "'";

		runQuery(what,tablename,where,projectList);
	}
}

//---------------------------------------------------------------------------
void database::getAliquotList(std::string &project_cid,std::map<std::string, std::map<std::string,std::string> > &aliquotList)
{
	aliquotList.clear();

	std::map<std::string , std::map<std::string ,AliquotInfo> >::iterator it = m_projectToAliquotsOrginal.find(project_cid);
	if (it!=m_projectToAliquotsOrginal.end())
	{
		std::map<std::string ,AliquotInfo> aliquots = it->second;

		std::map<std::string ,AliquotInfo>::iterator it2 = aliquots.begin();
		for (; it2!=aliquots.end(); ++it2)
		{
			std::string aliquot_cid = it2->first;

			std::vector<std::string> what;
			what.push_back("object_cid");
			what.push_back("external_name");
			what.push_back("external_full");
			std::string tablename = "C_OBJECT_NAME";
			std::string where = "object_cid  = '" + aliquot_cid  + "' AND object_type = '6'";

			runQuery(what,tablename,where,aliquotList);
		}
	}
}
//---------------------------------------------------------------------------

void database::getSiteList(std::map<int,std::map<String,String> > &siteList)
{
	siteList.clear();
	std::wstringstream sqlquery;
	sqlquery << "select object_cid,external_name,external_full from c_object_name where object_type = '8' AND status <> '99'";

	XQUERY query( m_dbCentral.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve Aliquot lists" );
	while ( query.fetch() )    //any data?
	{
		std::map<String,String> listInfo;
		std::wstringstream ss;
		ss << query.result.getInt(0);
		listInfo.insert(std::pair<String,String> (String(DEFINE_OBJECT_NAME_CID.c_str()),String(ss.str().c_str())));
		listInfo.insert(std::pair<String,String> (String(DEFINE_OBJECT_NAME_EXTERNAL_NAME.c_str()),String(query.result.getString(1).c_str())));
		listInfo.insert(std::pair<String,String> (String(DEFINE_OBJECT_NAME_EXTERNAL_FULL.c_str()),String(query.result.getString(2).c_str())));
		siteList.insert(std::pair<int,std::map<String,String> > (query.result.getInt(0),listInfo));
	}
	query.close();
}

//---------------------------------------------------------------------------

void database::getVesslList(std::map<int,std::map<String,String> > &vesslList,std::map<String,String> &fromSite)
{
	vesslList.clear();

	String SiteObjectID = fromSite[String(DEFINE_OBJECT_NAME_CID.c_str())];

	std::wstringstream sqlquery;
	sqlquery << "SELECT object_cid,external_name,external_full from c_object_name where object_cid IN (";
	sqlquery << 	"SELECT storage_cid from C_TANK_MAP WHERE location_cid = '";
	sqlquery << 	SiteObjectID.c_str();
	sqlquery << 	"' AND status <> '99' AND object_type = '16'";
	sqlquery << ")";

	XQUERY query( m_dbCentral.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve Aliquot lists" );
	while ( query.fetch() )    //any data?
	{
		std::map<String,String> listInfo;
		std::wstringstream ss;
		ss << query.result.getInt(0);
		listInfo.insert(std::pair<String,String> (String(DEFINE_OBJECT_NAME_CID.c_str()),String(ss.str().c_str())));
		listInfo.insert(std::pair<String,String> (String(DEFINE_OBJECT_NAME_EXTERNAL_NAME.c_str()),String(query.result.getString(1).c_str())));
		listInfo.insert(std::pair<String,String> (String(DEFINE_OBJECT_NAME_EXTERNAL_FULL.c_str()),String(query.result.getString(2).c_str())));
		vesslList.insert(std::pair<int,std::map<String,String> > (query.result.getInt(0),listInfo));
	}
	query.close();
}

//---------------------------------------------------------------------------

void database::getObjectNameDetails(const std::string &object_cid, std::map<std::string,std::string> &List)
{
	List.clear();
	std::wstringstream sqlquery;
	sqlquery << "select external_name,external_full from c_object_name where object_cid = '";
	sqlquery << object_cid.c_str() << "'";

	XQUERY query( m_dbCentral.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve lists" );
	if ( query.fetch() )    //any data?
	{
		List[DEFINE_OBJECT_NAME_CID] = object_cid;
		List[DEFINE_OBJECT_NAME_EXTERNAL_NAME] = query.result.getString(0).c_str();
		List[DEFINE_OBJECT_NAME_EXTERNAL_FULL] = query.result.getString(1).c_str();
	}
	query.close();
}

//----------------------------------------------------------------------------

bool database::openProject(std::string project_cid)
{
	if (m_projectInfo.find(project_cid) == m_projectInfo.end())
	{
		std::vector<std::string> what;
		what.push_back("project_cid");
		what.push_back("db_name");
		std::string tablename = "C_PROJECT";
		std::string where = "project_cid    = '" + project_cid + "'";
		runQuery(what,tablename,where,m_projectInfo);
	}
	std::string ProjectName = m_projectInfo[project_cid]["db_name"];

	return connectProject(ProjectName);
}
//----------------------------------------------------------------------------

std::string database::getProjectActivityStatus(const std::string &project_cid)
{
	std::map<std::string, std::map<std::string,std::string> >::iterator iprojects= m_projectInfo.find(project_cid);
	if (iprojects !=  m_projectInfo.end())
	{
		return iprojects->second["activity_flags"];
	}
	return "0";
}


//----------------------------------------------------------------------------

void database::openAllProjectDatabases()
{
	//ask the central db for project ids
	//open a connection to all project databases.
	{
		std::vector<std::string> what;
		what.push_back("project_cid");
		what.push_back("db_name");
		what.push_back("activity_flags");
		std::string tablename = "C_PROJECT";
		std::string where = "status <> '99' AND project_cid <> '0'";
		runQuery(what,tablename,where,m_projectInfo);
	}

	//get project list
	for (std::map<std::string, std::map<std::string,std::string> >::iterator iprojects=m_projectInfo.begin(); iprojects!=m_projectInfo.end(); ++iprojects)
	{
		m_dbProjects_cidToName.insert(std::pair<std::string,std::string>(iprojects->second["project_cid"],iprojects->second["db_name"]));

//don't care if it fails to open.. just try to open them all.
		connectProject(iprojects->second["db_name"]);
	}
}

//----------------------------------------------------------------------------
std::string database::getBoxVialCount(const std::string &box_cid)
{
	std::string returnVal = "0";
	std::string Query = std::string("SELECT count(*) from CRYOVIAL_STORE WHERE box_cid = '" + box_cid + "' and removed = '' ");
	XQUERY query( m_pCurrentProject, AnsiString(Query.c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve" );
	while ( query.fetch() )
	{
		std::string count = query.result.getString(0).c_str();
		returnVal = count;
	}
	query.close();
	return returnVal;
}

//----------------------------------------------------------------------------
int database::igetBoxVialCount(const std::string &box_cid)
{
	std::string returnVal = "0";
	std::string Query = std::string("SELECT count(*) from CRYOVIAL_STORE WHERE box_cid = '" + box_cid + "' and removed = '' ");
	XQUERY query( m_pCurrentProject, AnsiString(Query.c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve" );
	while ( query.fetch() )
	{
		std::string count = query.result.getString(0).c_str();
		returnVal = count;
	}
	query.close();
	return atoi(returnVal.c_str());
}

//----------------------------------------------------------------------------
int database::getNoVialForAliquot(const std::string &alquoit_cid,const std::string &box_cid)
{
	if (alquoit_cid == std::string("0"))
		return 0;

	int returnVal = 0;
	std::string  Query = std::string("SELECT count(*) from CRYOVIAL_STORE,cryovial WHERE CRYOVIAL_STORE.cryovial_id = cryovial.cryovial_id AND CRYOVIAL_STORE.box_cid = '" + box_cid + "' AND cryovial.aliquot_type_cid = '" + alquoit_cid + "' AND CRYOVIAL_STORE.status = '1' ");
	XQUERY query( m_pCurrentProject, AnsiString(Query.c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve" );
	while ( query.fetch() )
	{
		returnVal += atoi(query.result.getString(0).c_str());
	}
	query.close();
	return returnVal;
}

//----------------------------------------------------------------------------
void database::getVialsForAliquot(std::map<std::string, std::map<std::string,std::string> > &returnInfo,const std::string &alquoit_cid,const std::string &box_cid)
{
	std::vector<std::string> what;
	what.push_back("cryovial_store.record_id");
	what.push_back("cryovial_store.cryovial_id");
	what.push_back("cryovial_store.sample_volume");
	std::string tablename = "CRYOVIAL_STORE,cryovial";
	std::string where = " CRYOVIAL_STORE.cryovial_id = cryovial.cryovial_id AND CRYOVIAL_STORE.box_cid = '" + box_cid + "' AND cryovial.aliquot_type_cid = '" + alquoit_cid + "' AND CRYOVIAL_STORE.status = '1' ";
	runProjectQuery(what,tablename,where,returnInfo);
}

//----------------------------------------------------------------------------
void database::analiseVialsInABox(std::map<std::string,std::string> &boxstoreInfo,int BoxTotalCap)
{
	int TotalUsedSpace = 0;
	int ExtraFreeSpace = 0;
	int TotalProblemSpace = 0;

	std::string box_cid = boxstoreInfo["box_cid"];
//HAS BEEN MOVED OUT
	std::string status = " CRYOVIAL_STORE.status = '2' AND CRYOVIAL_STORE.removed <> '' ";
	std::string	Query = std::string("SELECT count(*) from CRYOVIAL_STORE WHERE CRYOVIAL_STORE.box_cid = '" + box_cid + "' AND " + status);
	{
	XQUERY query( m_pCurrentProject, AnsiString(Query.c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve" );
	while ( query.fetch() )
	{
		ExtraFreeSpace += atoi(query.result.getString(0).c_str());
	}
	query.close();
	}
//WILL BE MOVED OUT, CAN'T EMPTY THIS BOX
	status = " CRYOVIAL_STORE.status = '2' AND CRYOVIAL_STORE.removed = '' ";
	Query = std::string("SELECT count(*) from CRYOVIAL_STORE WHERE CRYOVIAL_STORE.box_cid = '" + box_cid + "' AND " + status);
	{
	XQUERY query( m_pCurrentProject, AnsiString(Query.c_str()).c_str());
	throwUnless( query.open(), "Failed to retreve" );
	while ( query.fetch() )
	{
		TotalProblemSpace += atoi(query.result.getString(0).c_str());
	}
	query.close();
	}
//VIAL, CAN BE MOVED
	status = " AND CRYOVIAL_STORE.status = '1' ";
	Query = std::string("SELECT count(*) from CRYOVIAL_STORE WHERE CRYOVIAL_STORE.box_cid = '" + box_cid + "' " + status);
	{
	XQUERY query( m_pCurrentProject, AnsiString(Query.c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve" );
	while ( query.fetch() )
	{
		TotalUsedSpace += atoi(query.result.getString(0).c_str());
	}
	query.close();
	}
//SPACE ALLOCATED, CAN'T MOVE OR EMTPY BOX
	status = " AND CRYOVIAL_STORE.status = '0' ";
	Query = std::string("SELECT count(*) from CRYOVIAL_STORE WHERE CRYOVIAL_STORE.box_cid = '" + box_cid + "' " + status);
	{
	XQUERY query( m_pCurrentProject, AnsiString(Query.c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve" );
	while ( query.fetch() )
	{
		TotalProblemSpace += atoi(query.result.getString(0).c_str());
	}
	query.close();
	}
//EMPTY SPACE
	status = " AND (CRYOVIAL_STORE.status = '3' OR  CRYOVIAL_STORE.status = '4'  OR  CRYOVIAL_STORE.status = '5') ";
	Query = std::string("SELECT count(*) from CRYOVIAL_STORE WHERE CRYOVIAL_STORE.box_cid = '" + box_cid + "' " + status);
	{
	XQUERY query( m_pCurrentProject, AnsiString(Query.c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve" );
	while ( query.fetch() )
	{
		ExtraFreeSpace += atoi(query.result.getString(0).c_str());
	}
	query.close();
	}
/*
	status = " AND CRYOVIAL_STORE.status <> '99' ";
	Query = std::string("SELECT count(*) from CRYOVIAL_STORE WHERE CRYOVIAL_STORE.box_cid = '" + box_cid + "' " + status);
	{
	XQUERY query( m_pCurrentProject, AnsiString(Query.c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve" );
	while ( query.fetch() )
	{
		String	Info = String(project_name.c_str()) + String(" total: ") + String(query.result.getString(0).c_str());
//			Info += L"Used: " + IntToString(TotalUsedSpace) + L" Free: " +IntToString(TotalFreeSpace) + L" Problems: "+ IntToString(TotalProblemSpace) + L" Total: "+ IntToString(TotalProblemSpace+TotalFreeSpace+TotalUsedSpace);
//			m_Memo->Lines->Add(Info);
	}
	query.close();
	}
*/
	//if we have values already (say from another project, we want to add to these..
	if (boxstoreInfo["TotalProblemSpace"].length() > 0)
		TotalProblemSpace += atoi(boxstoreInfo["TotalProblemSpace"].c_str());
	if (boxstoreInfo["ExtraFreeSpace"].length() > 0)
		TotalProblemSpace += atoi(boxstoreInfo["ExtraFreeSpace"].c_str());
	if (boxstoreInfo["TotalUsedSpace"].length() > 0)
		TotalProblemSpace += atoi(boxstoreInfo["TotalUsedSpace"].c_str());
	int SpaceFree = BoxTotalCap - (TotalProblemSpace+TotalUsedSpace);
	if (SpaceFree == 0)
		boxstoreInfo["PecentFreeSpace"] = std::string("0");
	else
		boxstoreInfo["PecentFreeSpace"] =  IntToStdString((int)(((float)SpaceFree/(float)BoxTotalCap)*100.0));
	boxstoreInfo["TotalProblemSpace"] = IntToStdString(TotalProblemSpace);
	boxstoreInfo["ExtraFreeSpace"] = IntToStdString(ExtraFreeSpace);
	boxstoreInfo["TotalUsedSpace"] = IntToStdString(TotalUsedSpace);
}
//----------------------------------------------------------------------------
void database::getAvalibleBoxesForReBox(const std::string &project_cid,const std::string &aliquot,std::map<std::string, std::map<std::string,std::string> > &returnInfo)
{
	openProject(project_cid);
	std::vector<std::string> what;
	what.push_back("box_type_cid");
	what.push_back("external_name");
	what.push_back("description");
	what.push_back("box_size_cid");
	what.push_back("status");
	std::string tablename = "BOX_CONTENT";
	std::string where = " aliquot_type1 = '" + aliquot + "' OR aliquot_type2 = '" + aliquot + "' OR aliquot_type3 = '" + aliquot + "' and status = '1'";
	runProjectQuery(what,tablename,where,returnInfo);
}
//----------------------------------------------------------------------------
void database::scanVessel(std::map<String,String> &vessel,TMemo *Memo)
{
	m_vessel = vessel;
	m_Memo = Memo;
	m_Memo->Lines->Clear();

	m_projectToAliquotsOrginal.clear();
//	m_AliquotsToBoxesSizeList.clear();
	m_BoxSize.clear();
	m_tankMaps.clear();
//	m_AliquotsToFreeSpace.clear();
//	m_AliquotsToUsedSpace.clear();
//	m_AliquotsToTotalBoxes.clear();

	openAllProjectDatabases();

	m_pCurrentProject = 0;

//GET THE SHELVE INFORMATION
	{
		String vessel_object_cid = vessel[String(database::DEFINE_OBJECT_NAME_CID.c_str())];
		std::vector<std::string> what;
		what.push_back("tank_cid");
		what.push_back("shelf_number");
		what.push_back("rack_layout_cid");
		what.push_back("storage_cid");
		std::string tablename = "C_TANK_MAP";
		std::string where = "storage_cid  = '" + std::string(AnsiString(vessel_object_cid.c_str()).c_str()) + "' AND status <> '99'";

		std::string myquery = getQuery(what,tablename,where);
		XQUERY query( m_dbCentral.get(), AnsiString(myquery.c_str()).c_str());
		throwUnless( query.open(), "Failed to retreve" );
		while ( query.fetch() )
		{
			VesselData info;
			std::string key = query.result.getString(0).c_str();
			for (unsigned int i=0;i<what.size();i++)
				info.tankMapInfo[what[i]] = query.result.getString(i);

			m_tankMaps.insert(std::pair<std::string, VesselData >(key,info));
		}
		query.close();
	}

	for (std::map<std::string, VesselData >::iterator itank=m_tankMaps.begin(); itank!=m_tankMaps.end(); ++itank)
	{
		VesselData tankmap_info = itank->second;
		std::string rack_layout_cid = tankmap_info.tankMapInfo["rack_layout_cid"];

		{
			std::vector<std::string> what;
			what.push_back("rack_type_cid");
			what.push_back("rack_layout_cid");
			what.push_back("rack_capacity");
			what.push_back("first_rack");
			what.push_back("last_rack");
			std::string tablename = "C_TANK_LAYOUT";
			std::string where = "rack_layout_cid  = '" + rack_layout_cid + "' AND status <> '99'";

			std::string myquery = getQuery(what,tablename,where);
			XQUERY query( m_dbCentral.get(), AnsiString(myquery.c_str()).c_str());
			throwUnless( query.open(), "Failed to retreve" );
			while ( query.fetch() )
			{
				tankLayoutData newtankLayoutData;
				std::string key = query.result.getString(0).c_str();
				for (unsigned int i=0;i<what.size();i++)
				{
					newtankLayoutData.tanklayoutInfo.insert(std::pair<std::string,std::string>(what[i],query.result.getString(i)));
				}
				m_tankMaps[itank->first].thetanklayouts.insert(std::pair<std::string,tankLayoutData>(key,newtankLayoutData));
			}
			query.close();
		}

		for (std::map<std::string, tankLayoutData >::iterator ilayoutInfo=m_tankMaps[itank->first].thetanklayouts.begin(); ilayoutInfo!=m_tankMaps[itank->first].thetanklayouts.end(); ++ilayoutInfo)
		{
			tankLayoutData thislayout_info = ilayoutInfo->second;
			std::string rack_type_cid = thislayout_info.tanklayoutInfo["rack_type_cid"];
			std::string tank_cid = tankmap_info.tankMapInfo["tank_cid"];
			{
				std::vector<std::string> what;
				what.push_back("rack_cid");
				what.push_back("position");
				what.push_back("rack_type_cid");
				what.push_back("external_name");
				what.push_back("default_box_type");
				what.push_back("project_cid");
				what.push_back("capacity");
				std::string tablename = "C_RACK_NUMBER";
				std::string where = "rack_type_cid  = '" + rack_type_cid  + "' AND tank_cid = '"+ tank_cid +"'";

				std::string myquery = getQuery(what,tablename,where);
				XQUERY query( m_dbCentral.get(), AnsiString(myquery.c_str()).c_str());
				throwUnless( query.open(), "Failed to retreve" );
				while ( query.fetch() )
				{
					rackNumberData rackData;
					for (unsigned int i=0;i<what.size();i++)
						rackData.rackNumberInfo.insert(std::pair<std::string,std::string>(what[i],query.result.getString(i)));

					std::string Rackkey = query.result.getString(0).c_str();
					std::map<std::string, rackNumberData> rackNumber = thislayout_info.theracks;
					m_tankMaps[itank->first].thetanklayouts[ilayoutInfo->first].theracks.insert(std::pair<std::string,rackNumberData >(Rackkey,rackData));
				}
				query.close();
			}
		}
	}
// now lets see if we can find some content, basically don't trust the system.
//unless we do get what is expected, we hunt through out the project databases
//Lets do an inital sweeep, then out side this loop we shall analise what we have
	for (std::map<std::string, VesselData >::iterator itank=m_tankMaps.begin(); itank!=m_tankMaps.end(); ++itank)
	{
		VesselData tankmap_info = itank->second;
		for (std::map<std::string, tankLayoutData >::iterator ilayoutInfo=tankmap_info.thetanklayouts.begin(); ilayoutInfo!=tankmap_info.thetanklayouts.end(); ++ilayoutInfo)
		{
			tankLayoutData thislayout_info = ilayoutInfo->second;
			for (std::map<std::string, rackNumberData >::iterator irackInfo=thislayout_info.theracks.begin(); irackInfo!=thislayout_info.theracks.end(); ++irackInfo)
			{
				rackNumberData thisrack_info = irackInfo->second;
				std::string project_cid = thisrack_info.rackNumberInfo["project_cid"];

				//not to worry if it's not valid or fails to open, this is just an inital pass to see what is what.
				if ((project_cid != "-1") && (project_cid != "0"))
				{
					//if could be a valid project id... lets see
					if (openProject(project_cid))
					{
						std::string rack_id = thisrack_info.rackNumberInfo["rack_cid"];
						//get all the info on the boxes are looking for.
						extractBoxInformation(project_cid,itank->first,irackInfo->first,ilayoutInfo->first);
						//now get the status on all boxes, so we have a proper count of how full this rack is.

						int MaxBoxes = atoi(thislayout_info.tanklayoutInfo["rack_capacity"].c_str());
						int capacity = getRackCapacity(rack_id, MaxBoxes);

						thisrack_info = irackInfo->second;
						thisrack_info.ourNewBoxCount = capacity;
						m_tankMaps[itank->first].thetanklayouts[ilayoutInfo->first].theracks[irackInfo->first] = thisrack_info;
					}
				}
			}
		}
	}

// Analize what we have. Do a more general db search for box content if they aren't as expected
//ALL project DBS Are now open (on start of analize)
//see if we have a full set of viles in all the boxes, and we have all boxes.
//if not, go hunting.. check against all, bar the one in project name, if there is one.
//if we find more. add it to project name..warn if did have a project name, but didn't find any. but found the lot in another.. a sign of an error
//perhaps add an error information string to rack/section or what ever it is.
//SEARCH FOR MORE BOXES

	for (std::map<std::string, VesselData >::iterator itank=m_tankMaps.begin(); itank!=m_tankMaps.end(); ++itank)
	{
		VesselData tankmap_info = itank->second;
		for (std::map<std::string, tankLayoutData >::iterator ilayoutInfo=tankmap_info.thetanklayouts.begin(); ilayoutInfo!=tankmap_info.thetanklayouts.end(); ++ilayoutInfo)
		{
			tankLayoutData thislayout_info = ilayoutInfo->second;
			for (std::map<std::string, rackNumberData >::iterator irackInfo=thislayout_info.theracks.begin(); irackInfo!=thislayout_info.theracks.end(); ++irackInfo)
			{
				rackNumberData thisrack_info = irackInfo->second;

				for (std::map<std::string,rackProjectData>::iterator irackProj=thisrack_info.rackProjectInfo.begin(); irackProj!=thisrack_info.rackProjectInfo.end(); ++irackProj)
				{
					std::string project_cid = irackProj->first;
					rackProjectData projectData = irackProj->second;

OurBoxCount ... use thisrack_info.ourNewBoxCount.. see how that works out
in out hunt through other projects.

remember, if we find some in other project update the ourNewBoxCount

not sure where totalBoxCount is being used... but ourNewBoxCount is a better search.




					int OurBoxCount = projectData.BoxStoreInfo.size();
					int MaxBoxCount = atoi(thislayout_info.tanklayoutInfo["rack_capacity"].c_str());
					int FreeSpace = atoi(thisrack_info.rackNumberInfo["capacity"].c_str());

// LOOK IN OTHER PROJECTS, we don't get the same as the
//database thinks is there.
					if (FreeSpace == -1)
						FreeSpace = 0;
					if (((OurBoxCount == MaxBoxCount - FreeSpace) || (OurBoxCount == MaxBoxCount)))
						continue;

					std::string rack_id = thisrack_info.rackNumberInfo["rack_cid"];
					//get project list
					for (std::map<std::string, std::map<std::string,std::string> >::iterator iprojects=m_projectInfo.begin(); iprojects!=m_projectInfo.end(); ++iprojects)
					{
						std::string tryproject_name = iprojects->second["db_name"];
						std::string tryproject_cid = iprojects->second["project_cid"];

						if (project_cid == tryproject_cid)
							continue; //skip the one we have done

						connectProject(tryproject_name);
						rackProjectData thisrack_info;
						extractBoxInformation(rack_id,&thisrack_info);
						if (thisrack_info.BoxStoreInfo.size() > 0)
						{
							m_tankMaps[itank->first].thetanklayouts[ilayoutInfo->first].theracks[irackInfo->first].rackProjectInfo.insert(std::pair<std::string, rackProjectData>(project_cid,thisrack_info));
						}
					}
				}
			}
		}
	}

//now get the box sizes, add this to global list
	for (std::map<std::string, VesselData >::iterator itank=m_tankMaps.begin(); itank!=m_tankMaps.end(); ++itank)
	{
		VesselData tankmap_info = itank->second;
		for (std::map<std::string, tankLayoutData >::iterator ilayoutInfo=tankmap_info.thetanklayouts.begin(); ilayoutInfo!=tankmap_info.thetanklayouts.end(); ++ilayoutInfo)
		{
			tankLayoutData thislayout_info = ilayoutInfo->second;
			for (std::map<std::string, rackNumberData >::iterator irackInfo=thislayout_info.theracks.begin(); irackInfo!=thislayout_info.theracks.end(); ++irackInfo)
			{
				rackNumberData thisrack_info = irackInfo->second;
				for (std::map<std::string,rackProjectData>::iterator irackProj=thisrack_info.rackProjectInfo.begin(); irackProj!=thisrack_info.rackProjectInfo.end(); ++irackProj)
				{
					rackProjectData projectData = irackProj->second;
					for (std::map<std::string, std::map<std::string,std::string> >::iterator iboxtypeInfo=projectData.BoxTypeInfo.begin(); iboxtypeInfo!=projectData.BoxTypeInfo.end(); ++iboxtypeInfo)
					{
						std::string box_size_cid = iboxtypeInfo->second["box_size_cid"];
						if (m_BoxSize.find(box_size_cid) == m_BoxSize.end())
						{
							std::vector<std::string> what;
							what.push_back("box_size_cid");
							what.push_back("description");
							what.push_back("box_capacity");
							what.push_back("empty_position");
							what.push_back("status");
							what.push_back("tube_type");
							std::string tablename = "C_BOX_SIZE";
							std::string where = "box_size_cid  = '" + box_size_cid  + "'";

							runQuery(what,tablename,where,m_BoxSize);
						}
					}
				}
			}
		}
	}

/************************************************************************************************************
GET VIAL COUNT, TOTAL AND FOR EACH ALIQUOT IN THE BOX. ALSO LOOK FOR BAGS
**************************************************************************************************************/
	int vialCount = 0;
	for (std::map<std::string, VesselData >::iterator itank=m_tankMaps.begin(); itank!=m_tankMaps.end(); ++itank)
	{
		VesselData tankmap_info = itank->second;
		for (std::map<std::string, tankLayoutData >::iterator ilayoutInfo=tankmap_info.thetanklayouts.begin(); ilayoutInfo!=tankmap_info.thetanklayouts.end(); ++ilayoutInfo)
		{
			tankLayoutData thislayout_info = ilayoutInfo->second;
			std::string rack_capacity = thislayout_info.tanklayoutInfo["rack_capacity"];

			for (std::map<std::string, rackNumberData >::iterator irackInfo=thislayout_info.theracks.begin(); irackInfo!=thislayout_info.theracks.end(); ++irackInfo)
			{
				rackNumberData thisrack_info = irackInfo->second;
				for (std::map<std::string,rackProjectData>::iterator irackProj=thisrack_info.rackProjectInfo.begin(); irackProj!=thisrack_info.rackProjectInfo.end(); ++irackProj)
				{
					rackProjectData projectData = irackProj->second;
					std::string project_cid = irackProj->first;

					std::string rack_id = thisrack_info.rackNumberInfo["rack_cid"];
					for (std::map<std::string, std::map<std::string,std::string> >::iterator iboxStoreInfo=projectData.BoxStoreInfo.begin(); iboxStoreInfo!=projectData.BoxStoreInfo.end(); ++iboxStoreInfo)
					{
						std::map<std::string,std::string> boxstoreInfo = iboxStoreInfo->second;

						if (projectData.BoxTypeInfo.find(boxstoreInfo["box_type_cid"]) == projectData.BoxTypeInfo.end())
							continue;    //output this as a warning
						std::map<std::string, std::map<std::string,std::string> >::iterator iTBoxTypeInfo = projectData.BoxTypeInfo.find(boxstoreInfo["box_type_cid"]);
						std::map<std::string,std::string> BoxTypeInfo = iTBoxTypeInfo->second;
						std::string box_size_cid = BoxTypeInfo["box_size_cid"];
						if (m_BoxSize.find(box_size_cid) ==  m_BoxSize.end())
							continue;    //output this as a warning
						std::map<std::string, std::map<std::string,std::string> >::iterator iboxSizeInfoinfo = m_BoxSize.find(box_size_cid);
						std::map<std::string,std::string> boxSizeInfoinfo = iboxSizeInfoinfo->second;
						{
							int boxSize =  atoi(boxSizeInfoinfo["box_capacity"].c_str());
							openProject(project_cid);
							analiseVialsInABox(boxstoreInfo,boxSize);
							std::string aliquot = BoxTypeInfo["aliquot_type1"];
							int icount = getNoVialForAliquot(aliquot, boxstoreInfo["box_cid"]);
							boxstoreInfo["TotalUsedSpace1"] = IntToStdString(icount);
							aliquot = BoxTypeInfo["aliquot_type2"];
							icount = getNoVialForAliquot(aliquot,	boxstoreInfo["box_cid"]);
							boxstoreInfo["TotalUsedSpace2"] = IntToStdString(icount);
							aliquot = BoxTypeInfo["aliquot_type3"];
							icount = getNoVialForAliquot(aliquot,	boxstoreInfo["box_cid"]);
							boxstoreInfo["TotalUsedSpace3"] = IntToStdString(icount);
							int myUsed = atoi(boxstoreInfo["TotalUsedSpace"].c_str());
							int box_capacity = boxSize - myUsed;
		//LOOK SEE IF IT IS A BAG. IF NO.. DON'T VIAL COUNT, leave it off the list.
							if ((atoi(rack_capacity.c_str()) <= 0) && (boxSize <= 0))
							{
								Memo->Lines->Add("bag fround - remove from list?- box: " + String(boxstoreInfo["box_cid"].c_str()));
							}
	 //						boxstoreInfo["box_capacity"] = IntToStdString(box_capacity);
							m_tankMaps[itank->first].thetanklayouts[ilayoutInfo->first].theracks[irackInfo->first].rackProjectInfo[project_cid].BoxStoreInfo[iboxStoreInfo->first] = boxstoreInfo;
						}
					}
				}
			}
		}
	}

/*********************************************************************************************************

********************************************************************************************************/
	for (std::map<std::string, VesselData >::iterator itank=m_tankMaps.begin(); itank!=m_tankMaps.end(); ++itank)
	{
		VesselData tankmap_info = itank->second;
		for (std::map<std::string, tankLayoutData >::iterator ilayout=tankmap_info.thetanklayouts.begin(); ilayout!=tankmap_info.thetanklayouts.end(); ++ilayout)
		{
			tankLayoutData layout_info = ilayout->second;
			for (std::map<std::string, rackNumberData>::iterator irack=layout_info.theracks.begin(); irack!=layout_info.theracks.end(); ++irack)
			{
				rackNumberData rackNumberData_info = irack->second;
				for (std::map<std::string,rackProjectData>::iterator irackProj=rackNumberData_info.rackProjectInfo.begin(); irackProj!=rackNumberData_info.rackProjectInfo.end(); ++irackProj)
				{
					rackProjectData projectData = irackProj->second;
					std::string project_cid = irackProj->first;
					for (std::map<std::string, std::map<std::string,std::string> >::iterator ibox=projectData.BoxStoreInfo.begin(); ibox!=projectData.BoxStoreInfo.end(); ++ibox)
					{
						std::map<std::string,std::string> info = ibox->second;

						if (projectData.BoxTypeInfo.find(info["box_type_cid"]) == projectData.BoxTypeInfo.end())
							continue; //ALERT USER?
						std::map<std::string, std::map<std::string,std::string> >::iterator iTBoxTypeInfo = projectData.BoxTypeInfo.find(info["box_type_cid"]);
						std::map<std::string,std::string> BoxTypeInfo = iTBoxTypeInfo->second;
						std::string box_size_cid = BoxTypeInfo["box_size_cid"];
						if (m_BoxSize.find(box_size_cid) ==  m_BoxSize.end())
							continue; //REPORT?
						std::map<std::string, std::map<std::string,std::string> >::iterator iboxSizeInfoinfo = m_BoxSize.find(box_size_cid);
						std::map<std::string,std::string> boxSizeInfoinfo = iboxSizeInfoinfo->second;
						int theirFree = atoi(info["box_capacity"].c_str());
						int boxSize =  atoi(boxSizeInfoinfo["box_capacity"].c_str());
						int theircount = boxSize - theirFree;
						int emptyPos = atoi(boxSizeInfoinfo["empty_position"].c_str());
						int myCount =  atoi(info["TotalUsedSpace"].c_str());
						if (emptyPos > 0)
							boxSize--;
						std::map<std::string ,AliquotInfo> Aliquots;
						std::map<std::string , std::map<std::string ,AliquotInfo> >::iterator  itr =  m_projectToAliquotsOrginal.find(project_cid);
						if (itr != m_projectToAliquotsOrginal.end())
							Aliquots = itr->second;

						bool valid2 = true;
						bool valid3 = true;

						std::string aliq1 = BoxTypeInfo["aliquot_type1"];
						std::string aliq2 = BoxTypeInfo["aliquot_type2"];
						std::string aliq3 = BoxTypeInfo["aliquot_type3"];

						if (aliq2 == std::string("0"))
							valid2 = false;
						if (aliq3 == std::string("0"))
							valid3 = false;

						//if we can't find any vials, then ignore this.
						if (valid2)
							valid2 = info["TotalUsedSpace2"] != "0";
						//if we can't find any vials, then ignore this.
						if (valid3)
							valid3 = info["TotalUsedSpace3"] != "0";

						bool found1 = false;
						bool found2 = !valid2; //mark as found if not valid, so it's not added to the list
						bool found3 = !valid3;

						for (unsigned int j=0;j<Aliquots.size();j++)
						{
							if (Aliquots.find(aliq1) != Aliquots.end())
								found1 = true;
							if (!found2)
							{
								if (Aliquots.find(aliq2) != Aliquots.end())
									found2 = true;
							}
							if (!found3)
							{
								if (Aliquots.find(aliq3) != Aliquots.end())
									found3 = true;
							}
						}
						if (!found1)
						{
							AliquotInfo AI;
							Aliquots.insert(std::pair<std::string,AliquotInfo >(aliq1,AI));
						}
						if (!found2)
						{
							AliquotInfo AI;
							Aliquots.insert(std::pair<std::string,AliquotInfo >(aliq2,AI));
						}
						if (!found3)
						{
							AliquotInfo AI;
							Aliquots.insert(std::pair<std::string,AliquotInfo >(aliq3,AI));
						}

//store some stats
//Free space
						std::string thisAlq = BoxTypeInfo["aliquot_type1"];
						int FreeSpace = Aliquots[thisAlq].m_FreeSpace;
						FreeSpace += boxSize - myCount;
						Aliquots[thisAlq].m_FreeSpace = FreeSpace;
						int UsedSpace = Aliquots[thisAlq].m_UsedSpace;
						UsedSpace += atoi(info["TotalUsedSpace"].c_str());
						Aliquots[thisAlq].m_UsedSpace=UsedSpace;
						int TotalBoxes = Aliquots[thisAlq].m_TotalBoxes;
						TotalBoxes++;
						Aliquots[thisAlq].m_TotalBoxes=TotalBoxes;
						if (valid2)
						{
							std::string thisAlq = BoxTypeInfo["aliquot_type2"];
							int FreeSpace = Aliquots[thisAlq].m_FreeSpace;
							FreeSpace += boxSize - myCount;
							Aliquots[thisAlq].m_FreeSpace = FreeSpace;
							int UsedSpace = Aliquots[thisAlq].m_UsedSpace;
							UsedSpace += atoi(info["TotalUsedSpace"].c_str());
							Aliquots[thisAlq].m_UsedSpace=UsedSpace;
							int TotalBoxes = Aliquots[thisAlq].m_TotalBoxes;
							TotalBoxes++;
							Aliquots[thisAlq].m_TotalBoxes=TotalBoxes;
						}
						if (valid2)
						{
							std::string thisAlq = BoxTypeInfo["aliquot_type3"];
							int FreeSpace = Aliquots[thisAlq].m_FreeSpace;
							FreeSpace += boxSize - myCount;
							Aliquots[thisAlq].m_FreeSpace = FreeSpace;
							int UsedSpace = Aliquots[thisAlq].m_UsedSpace;
							UsedSpace += atoi(info["TotalUsedSpace"].c_str());
							Aliquots[thisAlq].m_UsedSpace=UsedSpace;
							int TotalBoxes = Aliquots[thisAlq].m_TotalBoxes;
							TotalBoxes++;
							Aliquots[thisAlq].m_TotalBoxes=TotalBoxes;
						}
						m_projectToAliquotsOrginal[project_cid] = Aliquots;
					}
				}
			}
		}
	}
	for (std::map<std::string, VesselData >::iterator itank=m_tankMaps.begin(); itank!=m_tankMaps.end(); ++itank)
	{
		VesselData tankmap_info = itank->second;
		for (std::map<std::string, tankLayoutData >::iterator ilayout=tankmap_info.thetanklayouts.begin(); ilayout!=tankmap_info.thetanklayouts.end(); ++ilayout)
		{
			tankLayoutData layout_info = ilayout->second;
			for (std::map<std::string, rackNumberData>::iterator irack=layout_info.theracks.begin(); irack!=layout_info.theracks.end(); ++irack)
			{
///////////////// STORE THE TOTAL BOX COUNT IN EACH RACK FROM ALL PROJECTS
				rackNumberData rackNumberData_info = irack->second;
				std::map<std::string,int> unquieBOXIDs;
				std::map<std::string,rackProjectData>::iterator irackProj=rackNumberData_info.rackProjectInfo.begin();
				for (;irackProj!= rackNumberData_info.rackProjectInfo.end();++irackProj)
				{
					std::map<std::string, std::map<std::string,std::string> >::iterator iBoxes= irackProj->second.BoxStoreInfo.begin();
					for (;iBoxes!= irackProj->second.BoxStoreInfo.end();++iBoxes)
					{
						 unquieBOXIDs.insert(std::pair<std::string,int>(iBoxes->first,0));
					}
				}
				rackNumberData_info.totalBoxCount = unquieBOXIDs.size();
///////////////// STORE THE TOTAL VIAL COUNT IN EACH BOX, FROM ALL PROJECTS
				irackProj=rackNumberData_info.rackProjectInfo.begin();
				for (;irackProj!= rackNumberData_info.rackProjectInfo.end();++irackProj)
				{
					std::map<std::string, std::map<std::string,std::string> >::iterator iBoxes= irackProj->second.BoxStoreInfo.begin();
					for (;iBoxes!= irackProj->second.BoxStoreInfo.end();++iBoxes)
					{
						int BoxTotal = rackNumberData_info.BoxIDToTotalVialsFound[iBoxes->first];
						BoxTotal += atoi(iBoxes->second["TotalUsedSpace"].c_str());
						rackNumberData_info.BoxIDToTotalVialsFound[iBoxes->first] = BoxTotal;
						BoxTotal = rackNumberData_info.BoxIDToTotalProblemVialsFound[iBoxes->first];
						BoxTotal += atoi(iBoxes->second["TotalProblemSpace"].c_str());
						rackNumberData_info.BoxIDToTotalProblemVialsFound[iBoxes->first] = BoxTotal;
						BoxTotal = rackNumberData_info.BoxIDToTotalExtraFreeSpace[iBoxes->first];
						BoxTotal += atoi(iBoxes->second["ExtraFreeSpace"].c_str());
						rackNumberData_info.BoxIDToTotalExtraFreeSpace[iBoxes->first] = BoxTotal;
					}
					m_tankMaps[itank->first].thetanklayouts[ilayout->first].theracks[irack->first] = rackNumberData_info;
				}
			}
		}
	}
}

//----------------------------------------------------------------------------
void database::analiseForDatabaseUpdates(TMemo *Memo)
{
	Memo->Clear();
	for (std::map<std::string, VesselData >::iterator itank=m_tankMaps.begin(); itank!=m_tankMaps.end(); ++itank)
	{
		VesselData tankmap_info = itank->second;
		for (std::map<std::string, tankLayoutData >::iterator ilayout=tankmap_info.thetanklayouts.begin(); ilayout!=tankmap_info.thetanklayouts.end(); ++ilayout)
		{
			tankLayoutData layout_info = ilayout->second;
			for (std::map<std::string, rackNumberData>::iterator irack=layout_info.theracks.begin(); irack!=layout_info.theracks.end(); ++irack)
			{
				rackNumberData rackNumberData_info = irack->second;
/*******************************************************************************************************
update the c_rack_number records if the project, box type or capacity has changed
*********************************************************************************************************/
				int iprojects = rackNumberData_info.rackProjectInfo.size();
				if (iprojects == 1)
				{
					std::map<std::string,rackProjectData>::iterator project_it = rackNumberData_info.rackProjectInfo.begin();
					if (project_it->first != rackNumberData_info.rackNumberInfo["project_cid"])
					{
						String StringInfo = String(" 		db_update? rackNumber @pos:") + String(rackNumberData_info.rackNumberInfo["position"].c_str())
						 + String("  db project_cid: ")		   + String(rackNumberData_info.rackNumberInfo["project_cid"].c_str())
						 + String("  found project_cid: ")		   + String(project_it->first.c_str());
						Memo->Lines->Add(StringInfo);
					}
				}
				else
				{
					std::map<std::string,rackProjectData>::iterator project_it = rackNumberData_info.rackProjectInfo.begin();
					if (rackNumberData_info.rackNumberInfo["project_cid"] != std::string("0"))
					{
						String projectid = L" found project_cid(s):";
						project_it = rackNumberData_info.rackProjectInfo.begin();
						for (;project_it!=rackNumberData_info.rackProjectInfo.end();project_it++)
						{
							projectid += String(project_it->first.c_str());
							projectid += L",";
						}
						String StringInfo = String(" 		db_update? rackNumber @pos:") + String(rackNumberData_info.rackNumberInfo["position"].c_str())
						 + String("  db project_cid: ") + String(rackNumberData_info.rackNumberInfo["project_cid"].c_str())
						 + projectid;
						Memo->Lines->Add(StringInfo);
					}
				}
//*********************************************************************************************
				int NumberOfBoxInfoTypes = 0;
				for (std::map<std::string,rackProjectData>::iterator irackProj=rackNumberData_info.rackProjectInfo.begin(); irackProj!=rackNumberData_info.rackProjectInfo.end(); ++irackProj)
				{
					rackProjectData projectData = irackProj->second;
					std::string project_cid = irackProj->first;
					for (std::map<std::string, std::map<std::string,std::string> >::iterator ibox=projectData.BoxStoreInfo.begin(); ibox!=projectData.BoxStoreInfo.end(); ++ibox)
					{
						std::map<std::string,std::string> info = ibox->second;
						if (projectData.BoxTypeInfo.find(info["box_type_cid"]) == projectData.BoxTypeInfo.end())
							continue; //ALERT USER?
						std::map<std::string, std::map<std::string,std::string> >::iterator iTBoxTypeInfo = projectData.BoxTypeInfo.find(info["box_type_cid"]);
						std::map<std::string,std::string> BoxTypeInfo = iTBoxTypeInfo->second;
						std::string box_size_cid = BoxTypeInfo["box_size_cid"];
						if (m_BoxSize.find(box_size_cid) ==  m_BoxSize.end())
							continue; //REPORT?
						std::map<std::string, std::map<std::string,std::string> >::iterator iboxSizeInfoinfo = m_BoxSize.find(box_size_cid);
						std::map<std::string,std::string> boxSizeInfoinfo = iboxSizeInfoinfo->second;
						int theirFree = atoi(info["box_capacity"].c_str());
						int boxSize =  atoi(boxSizeInfoinfo["box_capacity"].c_str());
						int theircount = boxSize - theirFree;
						int emptyPos = atoi(boxSizeInfoinfo["empty_position"].c_str());
						int myCount = rackNumberData_info.BoxIDToTotalVialsFound[ibox->first];
						if (emptyPos > 0)
							boxSize--;
/******************************************************************************************************
//UPDATE DATABASE POSSIBLE INFO HERE
//"UPDATE the box_name record if the capacity has changed.
********************************************************************************************************/
						if ((theircount != myCount))
						{
							String StringInfo = String(" 		db_update? @pos:" + String(info["slot_position"].c_str())
							 + String("  BOXSIZE.box_cap:(Max) ")		   + (boxSizeInfoinfo["box_capacity"].c_str()))
							 + String("  BOXNAME.box_cap:(Free) ")		   + ((info["box_capacity"].c_str()))
							 + String("  problem Vials: ")		+   IntToString(rackNumberData_info.BoxIDToTotalProblemVialsFound[ibox->first])
							  + String(" moved Vials:")         + IntToString(rackNumberData_info.BoxIDToTotalExtraFreeSpace[ibox->first])
							 + String("  found Vials: ")		   +	IntToString(rackNumberData_info.BoxIDToTotalVialsFound[ibox->first])
							 + String("  found Vials (TUS): ")	    			+ String(info["TotalUsedSpace"].c_str())
							 + String(" empty_position: ") 	+ String(boxSizeInfoinfo["empty_position"].c_str());
							Memo->Lines->Add(StringInfo);
						}
/********************************************************************************************************/
					}
//count the number of boxinfotypes
					NumberOfBoxInfoTypes += irackProj->second.BoxTypeInfo.size();
				}
/*******************************************************************************************************
update the c_rack_number records if the project, box type or capacity has changed
*********************************************************************************************************/
				if (NumberOfBoxInfoTypes == 1)
				{
					std::map<std::string,rackProjectData>::iterator irackProj=rackNumberData_info.rackProjectInfo.begin();
					if (rackNumberData_info.rackNumberInfo["default_box_type"] != irackProj->second.BoxTypes_cid[0])
					{
						String StringInfo = String(" 		db_update? rackNumber @pos:" + String(rackNumberData_info.rackNumberInfo["position"].c_str()))
						 + String("  db default_box_type: ")		   + String(rackNumberData_info.rackNumberInfo["default_box_type"].c_str())
						 + String("  found box_type: ")		   		+ String(irackProj->second.BoxTypes_cid[0].c_str());
						Memo->Lines->Add(StringInfo);
					}
				}
				else
				{
					if (rackNumberData_info.rackNumberInfo["default_box_type"] != std::string("0"))
					{
						String BoxTypes_cid = L" found " + IntToString(NumberOfBoxInfoTypes) + L" box_types";
						String StringInfo = String(" 		db_update? @pos:") + String(rackNumberData_info.rackNumberInfo["position"].c_str())
						 + String("  db default_BoxType: ") + String(rackNumberData_info.rackNumberInfo["default_box_type"].c_str())
						 + BoxTypes_cid;
						Memo->Lines->Add(StringInfo);
					}
				}
//get the total number of boxes
/*
//THIS IS FOR ALL BOXES.. CHECK THIS
//go through all projects, shove all boxIDs into a map (to make sure it's unquie
// get map size.
				std::map<std::string,int> unquieBOXIDs;
				std::map<std::string,rackProjectData>::iterator irackProj=rackNumberData_info.rackProjectInfo.begin();
				for (;irackProj!= rackNumberData_info.rackProjectInfo.end();++irackProj)
				{
					std::map<std::string, std::map<std::string,std::string> >::iterator iBoxes= irackProj->second.BoxStoreInfo.begin();
					for (;iBoxes!= irackProj->second.BoxStoreInfo.end();++iBoxes)
					{
						 unquieBOXIDs.insert(std::pair<std::string,int>(iBoxes->first,0));
					}
				}
*/
				//remember capacity is FREE SPACE
				int rackCap = atoi(layout_info.tanklayoutInfo["rack_capacity"].c_str());
				int myCapacity =  rackCap - rackNumberData_info.totalBoxCount;
				int myCapacity2 =  rackCap - rackNumberData_info.ourNewBoxCount;
				int theirCapacity = atoi(rackNumberData_info.rackNumberInfo["capacity"].c_str());
				if (theirCapacity != myCapacity)
				{
					String StringInfo = String(" 		db_update? rackNumber @pos:")
					 + String(rackNumberData_info.rackNumberInfo["position"].c_str())
					 + String("  db capacity: ")
					 + IntToString(theirCapacity)
					 + String("  found capacity: ")
					 + IntToString(myCapacity);
					Memo->Lines->Add(StringInfo);
				}
/********************************************************************************************************/
			}
		}
	}
}

//----------------------------------------------------------------------------

void database::analyseVessel(TMemo *Memo, std::string &project_cid,std::string &alquoit_cid)
{
//reset the default box type combination selection.
	m_SelectedTargetBox = "";

	m_SelectedProject_cid = project_cid;
	m_SelectedAlquot_cid = alquoit_cid;

	//this is the cull list for this threshold.. clear it
	//keep the aliquot totals as they won't change
	std::map<std::string , std::map<std::string ,AliquotInfo> >::iterator it = m_projectToAliquotsOrginal.begin();
	for (; it!=m_projectToAliquotsOrginal.end(); ++it)
	{
		std::map<std::string ,AliquotInfo> proj = it->second;
		std::map<std::string ,AliquotInfo>::iterator anit = proj.begin();
		for (;anit!= proj.end();anit++)
		{
			m_projectToAliquotsOrginal[it->first][anit->first].m_AliquotsToBoxesSizeList.clear();
		}
	}

	m_Memo = Memo;
	Memo->Lines->Clear();

	String vessel_external_name = m_vessel[String(database::DEFINE_OBJECT_NAME_EXTERNAL_NAME.c_str())];
	String vessel_name_full = m_vessel[String(database::DEFINE_OBJECT_NAME_EXTERNAL_FULL.c_str())];
 //	Memo->Lines->Add("Looking at vessel:" + vessel_external_name + " [" + vessel_name_full + "]");
	int MaxBoxes = 0;
	int VessleWide_NoFreeSpace =0;
	for (std::map<std::string, VesselData >::iterator itank=m_tankMaps.begin(); itank!=m_tankMaps.end(); ++itank)
	{
		VesselData tankmap_info = itank->second;
 /*
		Memo->Lines->Add("");
		Memo->Lines->Add("Shelf Info: " + IntToString(tankmap_info.thetanklayouts.size()) + " shelf(s) found");
		Memo->Lines->Add("=======================");
		Memo->Lines->Add("");
   */
/*
		std::string tank_id = tankmap_info.tankMapInfo["tank_cid"];
		std::map<std::string,std::string> tank_cid_object_name;
		getObjectNameDetails(tank_id, tank_cid_object_name);
		std::string tank_layout_id = tankmap_info.tankMapInfo["rack_layout_cid"];
		std::map<std::string,std::string> tank_layout_cid_object_name;
		getObjectNameDetails(tank_layout_id, tank_layout_cid_object_name);
  */
 //		String StringInfo = String("shelf number - " + String(tankmap_info.tankMapInfo["shelf_number"].c_str()));
 //		Memo->Lines->Add(StringInfo);
 //		StringInfo = String("population: ") + String(tank_cid_object_name[DEFINE_OBJECT_NAME_EXTERNAL_NAME].c_str()) + L" [" + String(tank_cid_object_name[DEFINE_OBJECT_NAME_EXTERNAL_FULL].c_str()) + L"]";
 //		Memo->Lines->Add(StringInfo);
 //		StringInfo = String("Layout name: ") + String(tank_layout_cid_object_name[DEFINE_OBJECT_NAME_EXTERNAL_NAME].c_str()) + L" [" + String(tank_layout_cid_object_name[DEFINE_OBJECT_NAME_EXTERNAL_FULL].c_str()) + L"]";
 //		Memo->Lines->Add(StringInfo);
		for (std::map<std::string, tankLayoutData >::iterator ilayout=tankmap_info.thetanklayouts.begin(); ilayout!=tankmap_info.thetanklayouts.end(); ++ilayout)
		{
			tankLayoutData layout_info = ilayout->second;
/*
			StringInfo = 	  String("	section:")
					+ String(" max_racks: ") + String(layout_info.tanklayoutInfo["rack_capacity"].c_str())
					+ String(" first_rack: ") + String(layout_info.tanklayoutInfo["first_rack"].c_str())
					+ String(" last_rack: ") + String(layout_info.tanklayoutInfo["last_rack"].c_str());
 */
/*
			int UsedBoxesInSection = 0;
			int maxVialInSection = 0;
			int VialInSection = 0;
			int BoxesInSection = 0;

			int firstRack = atoi(layout_info.tanklayoutInfo["first_rack"].c_str());
			int lastRack = atoi(layout_info.tanklayoutInfo["last_rack"].c_str());
  */
  //			if (layout_info.theracks.size() > 0)
  //			{
  //				StringInfo = 	  String("	section: has ") +  IntToString(layout_info.theracks.size()) + L"/" + IntToString(lastRack - firstRack + 1) + " racks";
 //				Memo->Lines->Add(StringInfo);
  //			}

			for (std::map<std::string, rackNumberData>::iterator irack=layout_info.theracks.begin(); irack!=layout_info.theracks.end(); ++irack)
			{
				rackNumberData rackNumberData_info = irack->second;
				for (std::map<std::string,rackProjectData>::iterator irackProj=rackNumberData_info.rackProjectInfo.begin(); irackProj!=rackNumberData_info.rackProjectInfo.end(); ++irackProj)
				{
					rackProjectData projectData = irackProj->second;
					std::string project_cid = irackProj->first;

	//				int BoxesFull = 0;
	//				int VialEmtpySpace = 0;
	//				int BoxCount = 0;

	 //				int TotalBoxCount = projectData.BoxStoreInfo.size();
	/*
					StringInfo = 	  String("	racks:")
							+ String(" rack_cid: ") + String(rackNumberData_info.rackNumberInfo["rack_cid"].c_str())
							 + String(" position: ") + String(rackNumberData_info.rackNumberInfo["position"].c_str())
							+ String(" external_name: ") + String(rackNumberData_info.rackNumberInfo["external_name"].c_str())
							+ String(" default_box_type: ") + String(rackNumberData_info.rackNumberInfo["default_box_type"].c_str())
							+ String(" project_cid: ") + String(rackNumberData_info.rackNumberInfo["project_cid"].c_str())
							+ String(" capacity: ") + String(rackNumberData_info.rackNumberInfo["capacity"].c_str())
							+ String(" contains boxes: ") + IntToString(TotalBoxCount) + "/" + String(layout_info.tanklayoutInfo["rack_capacity"].c_str())
							+ projectLists;
					Memo->Lines->Add(StringInfo);
	*/
   //					MaxBoxes += atoi(layout_info.tanklayoutInfo["rack_capacity"].c_str());
					for (std::map<std::string, std::map<std::string,std::string> >::iterator ibox=projectData.BoxStoreInfo.begin(); ibox!=projectData.BoxStoreInfo.end(); ++ibox)
					{
						std::string multipleAliquots = "FALSE";
						std::map<std::string,std::string> info = ibox->second;
/*
					StringInfo = String("			[BOX_STORE/BOX_NAME]")
							 + String(" slot_position: ") 		+ String(info["slot_position"].c_str())
							 + String(" box_store.status: ") 	+ String(info["box_store.status"].c_str())
							 + String(" box_capacity: ") 		+ String(info["box_capacity"].c_str())
							 + String(" box_cid: ") 		+ String(info["box_cid"].c_str())
							 + String(" box_name.status: ") 	+ String(info["box_name.status"].c_str())
							 + String(" TotalProblemSpace: ") 			+ String(info["TotalProblemSpace"].c_str())
							 + String(" ExtraFreeSpace: ") 			+ String(info["ExtraFreeSpace"].c_str())
							 + String(" TotalUsedSpace: ") 			+ String(info["TotalUsedSpace"].c_str())
							 + String(" Free: ") 					+ String(info["PecentFreeSpace"].c_str() + String("%"));
					Memo->Lines->Add(StringInfo);
*/
						if (projectData.BoxTypeInfo.find(info["box_type_cid"]) == projectData.BoxTypeInfo.end())
							continue; //ALERT USER?
						std::map<std::string, std::map<std::string,std::string> >::iterator iTBoxTypeInfo = projectData.BoxTypeInfo.find(info["box_type_cid"]);
						std::map<std::string,std::string> BoxTypeInfo = iTBoxTypeInfo->second;
/*
					StringInfo = String("			[boxContent]")
						 + String(" external_name: ") 	+ String(BoxTypeInfo["external_name"].c_str())
							 + String(" description: ") 	+ String(BoxTypeInfo["description"].c_str())
							 + String(" status: ") 			+ String(BoxTypeInfo["status"].c_str())
							 + String(" box_size_cid: ") 	+ String(BoxTypeInfo["box_size_cid"].c_str())
						 + String(" aliquot_type1: ") 	+ String(BoxTypeInfo["aliquot_type1"].c_str())
							 + String(" aliquot_type2: ") 	+ String(BoxTypeInfo["aliquot_type2"].c_str())
							 + String(" aliquot_type3: ") 	+ String(BoxTypeInfo["aliquot_type3"].c_str());
						Memo->Lines->Add(StringInfo);
*/
						std::string box_size_cid = BoxTypeInfo["box_size_cid"];
						if (m_BoxSize.find(box_size_cid) ==  m_BoxSize.end())
							continue; //REPORT?
						std::map<std::string, std::map<std::string,std::string> >::iterator iboxSizeInfoinfo = m_BoxSize.find(box_size_cid);
						std::map<std::string,std::string> boxSizeInfoinfo = iboxSizeInfoinfo->second;
/*
					StringInfo = String("			[boxSize]")
						 + String(" description: ") 	+ String(boxSizeInfoinfo["description"].c_str())
						 + String(" box_capacity: ") 	+ String(boxSizeInfoinfo["box_capacity"].c_str())
						 + String(" empty_position: ") 	+ String(boxSizeInfoinfo["empty_position"].c_str())
						 + String(" status: ") 			+ String(boxSizeInfoinfo["status"].c_str());
	 //					 + String(" tube_type: ") 		+ String(boxSizeInfoinfo["tube_type"].c_str());
						Memo->Lines->Add(StringInfo);
  */
						int boxSize =  atoi(boxSizeInfoinfo["box_capacity"].c_str());
/*
						int theirFree = atoi(info["box_capacity"].c_str());
						int theircount = boxSize - theirFree;
						int emptyPos = atoi(boxSizeInfoinfo["empty_position"].c_str());
						int myCount =  atoi(info["TotalUsedSpace"].c_str());
						if (emptyPos > 0)
							boxSize--;
						VessleWide_NoFreeSpace += boxSize - myCount;   //do check like we don't get negitive here.
  */
						bool multiAlquots = false;
						multiAlquots = BoxTypeInfo["aliquot_type2"]!=std::string("0") && info["TotalUsedSpace2"]!=std::string("0");
						if (multiAlquots == false)
							multiAlquots = BoxTypeInfo["aliquot_type3"]!=std::string("0") && info["TotalUsedSpace3"]!=std::string("0");
						for (int i=0;i<3;i++)
						{
							std::string aliquotCount = info["TotalUsedSpace1"];
							std::string aliquot = BoxTypeInfo["aliquot_type1"];
							if (i==1)
							{
								aliquotCount = info["TotalUsedSpace2"];
								aliquot = BoxTypeInfo["aliquot_type2"];
							}
							else if (i==2)
							{
								aliquotCount = info["TotalUsedSpace3"];
								aliquot = BoxTypeInfo["aliquot_type3"];
							}
							if (aliquot != std::string("0") && aliquotCount != std::string("0"))
							{
									std::vector<std::string> Aliquots;
									std::map<std::string , std::map<std::string ,AliquotInfo> >::iterator it = m_projectToAliquotsOrginal.find(project_cid);
									std::map<std::string ,AliquotInfo> AliquotsMap = it->second;
									std::map<std::string ,AliquotInfo>::iterator itAlq =AliquotsMap.find(aliquot);
									AliquotInfo AlInfo = itAlq->second;
									std::vector<BoxSizeStruct> BoxStruct = AlInfo.m_AliquotsToBoxesSizeList;
									BoxSizeStruct bsize;
									bsize.box_cid = info["box_cid"];
									bsize.BoxStoreInfo = info;
									bsize.BoxTypeInfo = BoxTypeInfo;
									bsize.NoVials = atoi(aliquotCount.c_str());
									bsize.PecentFree = atoi(info["PecentFreeSpace"].c_str());
									bsize.NonMovableVials = atoi(info["TotalProblemSpace"].c_str());
									bsize.multipleAlquiots = multiAlquots;
									bsize.hasNonMoveableVialsOrSpace = info["TotalProblemSpace"] != std::string("0");
									bsize.EmptySpace = boxSize - (bsize.NonMovableVials + bsize.NoVials);
									BoxStruct.push_back(bsize);
									m_projectToAliquotsOrginal[it->first][itAlq->first].m_AliquotsToBoxesSizeList = BoxStruct;
								}
							}
 //
 //						if (myCount > 0)
 //							BoxesInSection++; //something in this box..!
  //						std::stringstream ss;
  //						ss << theircount;
 //						VialInSection += atoi(info["TotalUsedSpace"].c_str());
 //						maxVialInSection += boxSize;
 //					if (boxSize == atoi(info["TotalUsedSpace"].c_str()))
 //						BoxesFull++;
 //					BoxCount++;
				}
			}
			}
		}
	}


//CULL LIST DOWN TO WHAT PASSES THRESHHOLD
//Clear all zero content boxes
//not sure this is correct, as there may be non-moveable items in these boxes
//but as this list only has moveable things,

//CLEAR ALL FULL BOXES..
//CLEAR ALL BOXES WHICH ARE FULLER THEN THE % defrag, so we are not spreading
//Viles from a nearly full box into 97 other near full boxes.

 //CULL FULL BOXES, NOT SELECTED PROJECT OR ALQUOT... FOR SPECIAL CASE TEXT
	int BoxCount = 0;
	int FullCount = 0;
	int ProblemCount = 0;
	{
	std::map<std::string , std::map<std::string ,AliquotInfo> >::iterator it = m_projectToAliquotsOrginal.begin();
	for (; it!=m_projectToAliquotsOrginal.end(); ++it)
	{
		std::string project_cid = it->first;
		std::map<std::string ,AliquotInfo> AliquotList = it->second;

		std::map<std::string ,AliquotInfo>::iterator itAli = AliquotList.begin();
		for (; itAli!=AliquotList.end(); ++itAli)
		{
			std::string thisAliquot = itAli->first;
			std::vector<BoxSizeStruct> BoxSizeList = itAli->second.m_AliquotsToBoxesSizeList;

			std::vector<BoxSizeStruct>::iterator itbox = BoxSizeList.begin();
			while (itbox!= BoxSizeList.end())
			{
				if (project_cid != m_SelectedProject_cid)
					itbox = BoxSizeList.erase(itbox);
				else if (thisAliquot != m_SelectedAlquot_cid)
					itbox = BoxSizeList.erase(itbox);
				else if ((*itbox).EmptySpace == 0)   //no empty space. KILL IT
				{
					FullCount++;
					itbox = BoxSizeList.erase(itbox);
				}
				else if (((*itbox).NoVials == 0))   //no vials.. probably does, just they are not in the system yet. so ignore the box
				{
					ProblemCount++;
					itbox = BoxSizeList.erase(itbox);
				}
				else if ((*itbox).hasNonMoveableVialsOrSpace)
				{
					ProblemCount++;
					itbox = BoxSizeList.erase(itbox);
				}
				else
				{
					BoxCount++;
					itbox++;
				}
			}
			m_projectToAliquotsOrginal[it->first][itAli->first].m_AliquotsToBoxesSizeList = BoxSizeList;
		}
	}
	}

	if (BoxCount == 0)
	{
		m_Memo->Lines->Clear();

		if ((FullCount > 0) && (ProblemCount > 0))
			m_Memo->Lines->Add("All boxes within this Project/Aliquot are full, or can't be moved.");
		if (ProblemCount > 0)
			m_Memo->Lines->Add("All boxes within this Project/Aliquot can't be moved.");
		if ((FullCount > 0))
			m_Memo->Lines->Add("All boxes within this Project/Aliquot are full.");
		return;
	}

//	refineSearch(threshhold, useDefaultBox);
}


void database::cullForThreshHold(int threshhold,std::map<std::string,std::map<std::string,std::string> > &aliquotsBoxTypes)
{
	m_AliquotsBoxTypes.clear();
	{
	std::map<std::string , std::map<std::string ,AliquotInfo> >::iterator it = m_projectToAliquots.begin();
	for (; it!=m_projectToAliquots.end(); ++it)
	{
		std::string project_cid = it->first;
		std::map<std::string ,AliquotInfo> AliquotList = it->second;

		std::map<std::string ,AliquotInfo>::iterator itAli = AliquotList.begin();
		for (; itAli!=AliquotList.end(); ++itAli)
		{
			std::string thisAliquot = itAli->first;
			std::vector<BoxSizeStruct> BoxSizeList = itAli->second.m_AliquotsToBoxesSizeList;

			std::vector<BoxSizeStruct>::iterator itbox = BoxSizeList.begin();
			while (itbox!= BoxSizeList.end())
			{

	//LOOK SEE IF IT IS A BAG. IF NO.. DON'T VIAL COUNT, leave it off the list.
/*				if ((atoi(rack_capacity.c_str()) <= 0) && (boxSize <= 0))
				{
					Memo->Lines->Add("bag fround - remove from list?- box: " + String(boxstoreInfo["box_cid"].c_str()));
				}
*/
				if ((*itbox).multipleAlquiots)
					itbox++; //KEEP
				else if (((*itbox).PecentFree < threshhold))
					itbox = BoxSizeList.erase(itbox); //enough enough empty space and without multi alquots
				else
					itbox++;
			}
			m_projectToAliquots[it->first][itAli->first].m_AliquotsToBoxesSizeList = BoxSizeList;
		}
	}
	}
//now we have a final list, lets see what box types we have

	{
	std::map<std::string , std::map<std::string ,AliquotInfo> >::iterator it = m_projectToAliquots.find(m_SelectedProject_cid);
	if (it!=m_projectToAliquots.end())
	{
		std::string project_cid = it->first;
		std::map<std::string ,AliquotInfo> AliquotList = it->second;

		std::map<std::string ,AliquotInfo>::iterator itAli = AliquotList.find(m_SelectedAlquot_cid);
		if (itAli!=AliquotList.end())
		{
			std::string thisAliquot = itAli->first;

			std::vector<BoxSizeStruct> BoxSizeList = itAli->second.m_AliquotsToBoxesSizeList;
			std::vector<BoxSizeStruct>::iterator itbox = BoxSizeList.begin();

			aliquotsBoxTypes = itAli->second.m_BoxTypes;
			aliquotsBoxTypes.clear();
			while (itbox!= BoxSizeList.end())
			{
				std::string box_type = itbox->BoxStoreInfo["box_type_cid"];
				std::string box_size = itbox->BoxTypeInfo["box_size_cid"];
				std::string box_external_name = itbox->BoxStoreInfo["external_name"];

				if (aliquotsBoxTypes.find(box_type) == aliquotsBoxTypes.end())
				{
					std::map<std::string,std::string> info;
					info.insert(std::pair<std::string,std::string>("box_type_cid",itbox->BoxStoreInfo["box_type_cid"]));
					info.insert(std::pair<std::string,std::string>("box_size_cid",itbox->BoxTypeInfo["box_size_cid"]));
					info.insert(std::pair<std::string,std::string>("external_name",itbox->BoxStoreInfo["external_name"]));

					aliquotsBoxTypes.insert(std::pair<std::string,std::map<std::string,std::string> > (box_external_name,info));
//					Memo->Lines->Add(L"BoxType: " + String(box_type.c_str()));
				}
				itbox++;
			}

			m_projectToAliquots[it->first][itAli->first].m_BoxTypes = aliquotsBoxTypes;
		}
	}
	}
}


void database::refineSearch(int threshhold)
{
//	copy m_projectToAliquotsOrginal to m_projectToAliquots
	m_projectToAliquots.clear();

	std::map<std::string , std::map<std::string ,AliquotInfo> >::iterator it = m_projectToAliquotsOrginal.begin();
	for (; it!=m_projectToAliquotsOrginal.end(); ++it)
	{
		std::string project_cid = it->first;
		std::map<std::string ,AliquotInfo> AliquotList = it->second;

		std::map<std::string ,AliquotInfo>::iterator itAli = AliquotList.begin();
		for (; itAli!=AliquotList.end(); ++itAli)
		{
			std::string thisAliquot = itAli->first;

			AliquotInfo newCopy;
			newCopy.copy(itAli->second);
		   m_projectToAliquots[it->first][itAli->first] = newCopy;
		}
	}

	cullForThreshHold(threshhold,m_AliquotsBoxTypes);
}

std::string database::getDefaultBoxType()
{
	if (m_SelectedTargetBox == TtBoxSelection::DEFINE_RESULT_YES)
		return "";
	return m_SelectedTargetBox;
}

bool database::compileResults(const std::string &distination_box_size_cid)
{
	m_Memo->Clear();
//DO THIS box_size_cid needs adding
 //	m_distination_box_size_cid = distination_box_type["box_size_cid"];

	std::map<std::string, std::map<std::string,std::string> > desinationBox;

//GET INFORMATION ON THE SELECTED DESTINATION BOX TYPE
	std::vector<std::string> what;
	what.push_back("box_size_cid");
	what.push_back("description");
	what.push_back("box_capacity");
	what.push_back("empty_position");
	what.push_back("status");
	what.push_back("tube_type");
	std::string tablename = "C_BOX_SIZE";
	std::string where = "box_size_cid  = '" + distination_box_size_cid  + "'";
	runQuery(what,tablename,where,desinationBox);

	std::map<std::string , std::map<std::string ,AliquotInfo> >::iterator it = m_projectToAliquots.find(m_SelectedProject_cid);

	if (it == m_projectToAliquots.end())
		return false;

	std::string project_cid = it->first;
	std::map<std::string ,AliquotInfo> AliquotList = it->second;
	std::map<std::string ,AliquotInfo>::iterator itAli = AliquotList.find(m_SelectedAlquot_cid);

	if (itAli==AliquotList.end())
		return false;

	AliquotInfo Aliquot = itAli->second;
	std::string thisAliquot = itAli->first;
	m_TotalBoxesToBeMoved = 0;
	int TotalVialsMoved = 0;
	int lastBoxSize = -1;

	int VialFreeSpace = Aliquot.m_FreeSpace;
	int VialsUsed = Aliquot.m_UsedSpace;
	int TotalBoxes = Aliquot.m_TotalBoxes;

	std::vector<BoxSizeStruct> BoxSizeList = Aliquot.m_AliquotsToBoxesSizeList;
	std::sort(BoxSizeList.begin(), BoxSizeList.end(),mysortfunction);

	for (std::vector<BoxSizeStruct>::iterator itboxSize=BoxSizeList.begin(); itboxSize!=BoxSizeList.end(); ++itboxSize)
	{
		m_TotalBoxesToBeMoved++;
		TotalVialsMoved += (*itboxSize).NoVials;
	}

	m_Memo->Lines->Add("Total Vials in this Aliquot:" + IntToString(VialsUsed));
	m_Memo->Lines->Add("Total boxes in this Aliquot:" + IntToString(TotalBoxes));

	int SelectedBoxSize = 0;

	SelectedBoxSize = atoi(desinationBox[distination_box_size_cid]["box_capacity"].c_str());
	int empty = atoi(desinationBox[distination_box_size_cid]["empty_position"].c_str());
	if (empty > 0)
		SelectedBoxSize--;

	String InfoString = L" -- selected box has space for:" + IntToString(SelectedBoxSize);
	m_Memo->Lines->Add(InfoString);
	m_TotalNewBoxes = 0;
	if (SelectedBoxSize == 0)
	{
		m_Memo->Lines->Add("Number of new boxes: N/A ");
		m_Memo->Lines->Add("Estimated boxes saved: N/A");
		return false;
	}

	m_TotalNewBoxes = ceil((float)TotalVialsMoved/(float)SelectedBoxSize);
	if (m_TotalBoxesToBeMoved == 0)
	{
		m_Memo->Lines->Add("There are no boxes within the threshold.");
		return false;
	}

	if (m_TotalBoxesToBeMoved == m_TotalNewBoxes)
	{
		m_Memo->Lines->Add("No boxes can be rationalised.");
		return false;
	}
	m_Memo->Lines->Add("Number of boxes within the threshold:" + IntToString(m_TotalBoxesToBeMoved));
	int NoNewBoxes = ceil((float)TotalVialsMoved/(float)SelectedBoxSize);
	m_Memo->Lines->Add("Moved into new boxes:" + IntToString(NoNewBoxes));

	int totalBoxesRemoved = m_TotalBoxesToBeMoved - NoNewBoxes;
	int PercentFree = (int) (((float)(totalBoxesRemoved)/(float)TotalBoxes)*100.0f);
	m_Memo->Lines->Add("Total saving of:" + IntToString(totalBoxesRemoved) + "/" + IntToString(TotalBoxes) + " (" + IntToString(PercentFree)+ "% over all) boxes");

	PercentFree = (int) (((float)(totalBoxesRemoved)/(float)m_TotalBoxesToBeMoved)*100.0f);
	m_Memo->Lines->Add("Of boxes removed, we get a compression of :"  + IntToString(PercentFree)+ "%");
	return true;
}

//---------------------------------------------------------------------------
bool database::checkForCombiningBoxesInTheAliquot(bool useTheDefaultBox)
{
/*
 *  m_SelectedTargetBox holds what the user did last time
 *  if blank, it's the first time
 *  if holds a box_type_cid, that should be the default selection for boxtype
 *  if holds TtBoxSelection::DEFINE_RESULT_YES, then last time they selected combine
 */
	if (m_AliquotsBoxTypes.size()>1)
	{
		if (!useTheDefaultBox)
			m_SelectedTargetBox = "";

		if (m_SelectedTargetBox == TtBoxSelection::DEFINE_RESULT_YES)
		{  // user pressed yes to combine the lot
			return true;
		}

		std::string result = TtBoxSelection::DEFINE_RESULT_NO;
	// ask for box type?
		if (m_SelectedTargetBox.empty())
		{
			showMultipleBoxDialog(m_AliquotsBoxTypes);
		}

		if (m_SelectedTargetBox == TtBoxSelection::DEFINE_RESULT_YES)
		{  // user pressed yes to combine the lot
			return true;
		}
		if (m_SelectedTargetBox == TtBoxSelection::DEFINE_RESULT_CANCEL)
		{  // user cancelled
			return false;
		}
		//can it be empty at this point?
		if (!m_SelectedTargetBox.empty())
		{
			//cull all other box types.
			{
			std::map<std::string , std::map<std::string ,AliquotInfo> >::iterator it = m_projectToAliquots.begin();
			for (; it!=m_projectToAliquots.end(); ++it)
			{
				std::string project_cid = it->first;
				std::map<std::string ,AliquotInfo> AliquotList = it->second;

				std::map<std::string ,AliquotInfo>::iterator itAli = AliquotList.begin();
				for (; itAli!=AliquotList.end(); ++itAli)
				{
					std::string thisAliquot = itAli->first;
					std::vector<BoxSizeStruct> BoxSizeList = itAli->second.m_AliquotsToBoxesSizeList;

					std::vector<BoxSizeStruct>::iterator itbox = BoxSizeList.begin();

					while (itbox!= BoxSizeList.end())
					{
						std::map<std::string,std::string> BoxStoreInfo = itbox->BoxStoreInfo;
						std::string box_type = BoxStoreInfo["box_type_cid"];

						if (box_type != m_SelectedTargetBox)
							itbox = BoxSizeList.erase(itbox); //Cull the box. Wrong type
						else
							itbox++;
					}
					m_projectToAliquots[it->first][itAli->first].m_AliquotsToBoxesSizeList = BoxSizeList;
				}
			}
			}
		}
	}
	return true;
}
//---------------------------------------------------------------------------

void database::showMultipleBoxDialog(std::map<std::string,std::map<std::string,std::string> > &aliquotsBoxTypes)
{
	m_SelectedTargetBox = "";
	std::auto_ptr<TtBoxSelection> dbForm = std::auto_ptr<TtBoxSelection>( new TtBoxSelection(0) );
	dbForm->setUp(aliquotsBoxTypes,m_SelectedTargetBox);
	dbForm->ShowModal();

	if (dbForm->getResult() == TtBoxSelection::DEFINE_RESULT_NO)
	{
		m_SelectedTargetBox = dbForm->getBoxType();
		m_SelectedTargetBoxSize = dbForm->getBoxSize();
	}
	else if (dbForm->getResult() == TtBoxSelection::DEFINE_RESULT_YES)
	{
		m_SelectedTargetBox = TtBoxSelection::DEFINE_RESULT_YES;
		m_SelectedTargetBoxSize = "";
	}
}

//---------------------------------------------------------------------------

std::string database::getSelectedProject()
{
	return m_SelectedProject_cid;
}
//---------------------------------------------------------------------------

std::string database::getSelectedAliquot()
{
	return m_SelectedAlquot_cid;
}
//---------------------------------------------------------------------------
void database::extractBoxInformation(std::string project_cid,std::string itank_first,std::string irackInfo_first, std::string ilayoutInfo_first)
{
	rackNumberData thisrack_info = m_tankMaps[itank_first].thetanklayouts[ilayoutInfo_first].theracks[irackInfo_first];
	std::string rack_id = thisrack_info.rackNumberInfo["rack_cid"];

	std::map<std::string,rackProjectData>::iterator projecttre = thisrack_info.rackProjectInfo.find(project_cid);
	if (projecttre != thisrack_info.rackProjectInfo.end())
	{
		return; //already searched this one.
	}
	rackProjectData thisProject_info;
	extractBoxInformation(rack_id,&thisProject_info);
	if (thisProject_info.BoxStoreInfo.size() > 0)
	{
		m_tankMaps[itank_first].thetanklayouts[ilayoutInfo_first].theracks[irackInfo_first].rackProjectInfo.insert(std::pair<std::string, rackProjectData>(project_cid,thisProject_info));
	}

}
//----------------------------------------------------------------------------

int database::getRackCapacity(const std::string &rack_cid, int maxBoxes)
{
	bool *FoundList = new bool[maxBoxes];
	std::stringstream BoxPosList;
	for (int i=1;i<maxBoxes+1;i++)
	{
		FoundList[i-1] = false;
		if (!BoxPosList.str().empty())
			BoxPosList << ",";
		BoxPosList << "'" << i << "'";
	}
	std::string PosQuery = " ( " + BoxPosList.str() + " ) ";

	std::string Query = "SELECT TOP 100 status,removed,retrieval_cid,slot_position FROM BOX_STORE WHERE rack_cid = '" + rack_cid + "' AND slot_position IN " + PosQuery + " ORDER BY time_stamp DESC";
	XQUERY query( m_pCurrentProject, AnsiString(Query.c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve" );
	int usedCount =0;
	int emptyCount = 0;
	while ( query.fetch() )
	{
		std::string slot_position = query.result.getString(3);
		int slot_pos = atoi(slot_position.c_str());
		if(FoundList[slot_pos-1])
			continue; //this slot has been processed
		std::string status = query.result.getString(0);
		std::string removed = query.result.getString(1);
		if (removed == "ERROR")
			removed = "";
		std::string retrieval_cid = query.result.getString(2);
		if (status == "0")
			usedCount++;
		else if (status == "1")
			usedCount++;
		else if (status == "2")
		{
			if ((!removed.empty()) && (!retrieval_cid.empty()))
				usedCount++;
			else if ((removed.empty()) && (!retrieval_cid.empty()))
				return -1;
			else if ((!removed.empty()) && (retrieval_cid.empty()))
				return usedCount++;
 //			else if ((removed.empty) && (retrieval_cid.empty()))
			return -1;
		}
		else if (status == "3")
			emptyCount++;
		else if (status == "5")
			usedCount++;
		else if (status == "6")
			usedCount++;
		else if (status == "7")
			usedCount++; //referred by operator
		else
			return -1;
		FoundList[slot_pos-1] = true;
	}
	//if we get to here, we should have some idea
	//empty count is kept for the hell of it.
	//What we are interested in is the used count.
	//lets see we have some slots missing.. add these to empty
	for (int i=0;i<maxBoxes;i++)
	{
		if (!FoundList[i])
		{
        	emptyCount++;
		}
	}
//for sanity testing
	if (emptyCount + usedCount != maxBoxes)
		assert(maxBoxes != maxBoxes);
	return usedCount;
}


//---------------------------------------------------------------------------
void database::extractBoxInformation(std::string rack_id,rackProjectData *thisrack_info)
{
	{
		std::string statusCondition = " AND (BOX_STORE.status = '6') ";
		std::string Query = std::string("SELECT  BOX_STORE.box_cid, BOX_STORE.slot_position, BOX_STORE.rack_cid, BOX_STORE.status,BOX_NAME.box_capacity , BOX_NAME.box_type_cid , BOX_NAME.external_name , BOX_NAME.status AS BX_status FROM BOX_STORE,BOX_NAME WHERE BOX_STORE.rack_cid   = '") + rack_id + std::string("' AND BOX_NAME.box_cid = BOX_STORE.box_cid " + statusCondition);

//m_Memo->Lines->Add(String(Query.c_str()));

		XQUERY query( m_pCurrentProject, AnsiString(Query.c_str()).c_str() );
		throwUnless( query.open(), "Failed to retreve" );
		bool SomeData = false;
		while ( query.fetch() )
		{
			SomeData = true;
			std::string key = query.result.getString(0).c_str();
			std::map<std::string,std::string> info;
			info["box_cid"] = query.result.getString(0);
			info["slot_position"] = query.result.getString(1);
			info["rack_cid"] = query.result.getString(2);
			info["box_store.status"] = query.result.getString(3);
			info["box_capacity"] = query.result.getString(4);
			info["box_type_cid"] = query.result.getString(5);
			info["external_name"] = query.result.getString(6);
			info["box_name.status"] = query.result.getString(7);
			thisrack_info->BoxStoreInfo.insert(std::pair<std::string, std::map<std::string,std::string> >(key,info));
//save box types
			bool found = false;
			std::vector<std::string> box_cid = thisrack_info->BoxTypes_cid;
			int iTotalFoundBoxTypes = box_cid.size();
			for (int i=0;i<iTotalFoundBoxTypes;i++)
			{
				if (info["box_type_cid"] == box_cid[i])
				{
					found = true;
				}
			}
			if (!found)
				thisrack_info->BoxTypes_cid.push_back(info["box_type_cid"]);
		}
		query.close();
		std::map<std::string, std::map<std::string,std::string> > BoxTypeInfo;
		for (std::map<std::string, std::map<std::string,std::string> >::iterator iboxStoreInfo=thisrack_info->BoxStoreInfo.begin(); iboxStoreInfo!= thisrack_info->BoxStoreInfo.end(); ++iboxStoreInfo)
		{
			std::map<std::string,std::string> boxstoreInfo = iboxStoreInfo->second;
			std::string box_type_cid = boxstoreInfo["box_type_cid"];
			if (thisrack_info->BoxTypeInfo.find(box_type_cid) == thisrack_info->BoxTypeInfo.end())
			{
				if (BoxTypeInfo.find(box_type_cid) == BoxTypeInfo.end())
			//get the type of box
				{
					std::vector<std::string> what;
					what.push_back("box_type_cid");
					what.push_back("external_name");
					what.push_back("description");
					what.push_back("status");
					what.push_back("box_size_cid");
					what.push_back("aliquot_type1");
					what.push_back("aliquot_type2");
					what.push_back("aliquot_type3");
					std::string tablename = "BOX_CONTENT";
					std::string where = "box_type_cid  = '" + box_type_cid  + "'";

					String Query = runProjectQuery(what,tablename,where,BoxTypeInfo);
//m_Memo->Lines->Add(String(Query.c_str()));
				}
			}
			thisrack_info->BoxTypeInfo = BoxTypeInfo;
		}
	}
}

//---------------------------------------------------------------------------
void database::createRetrievalList(const std::string &external_name,const std::string &description, const std::string &c_project_cid, const std::string &c_aliquot_cid,std::map<std::string,std::string> &distination_box_type)
{
	openProject(c_project_cid);
	m_dbCentral->setAutoCommit(false);
	m_pCurrentProject->setAutoCommit(false);
//get rebox details
	std::string reBoxType_cid = distination_box_type["box_type_cid"];
	std::vector<std::string> what;
	what.push_back("box_type_cid");
	what.push_back("box_size_cid");
	std::string tablename = "BOX_CONTENT";
	std::string where = "box_type_cid  = '" + reBoxType_cid  + "'";
	std::map<std::string, std::map<std::string,std::string> > reBoxInfo;
	String Query = runProjectQuery(what,tablename,where,reBoxInfo);
	std::string reBoxSize_cid = reBoxInfo[reBoxType_cid]["box_size_cid"];
	what.clear();

	what.push_back("box_size_cid");
	what.push_back("description");
	what.push_back("box_capacity");
	what.push_back("empty_position");
	what.push_back("status");
	what.push_back("tube_type");
	tablename = "C_BOX_SIZE";
	where = "box_size_cid  = '" + reBoxSize_cid  + "'";
	std::map<std::string, std::map<std::string,std::string> > reBoxSize;
	runQuery(what,tablename,where,reBoxSize);

	std::map<std::string,std::string> aliquotInfo;
	getObjectNameDetails(c_aliquot_cid, aliquotInfo);
	what.clear();
	what.push_back("project_cid");
	what.push_back("external_name");
	tablename = "C_PROJECT";
	where = "project_cid  = '" + c_project_cid  + "'";

	std::map<std::string, std::map<std::string,std::string> > projectInfo;
	runQuery(what,tablename,where,projectInfo);

	std::string c_aliquotInfo_External_Name = aliquotInfo[DEFINE_OBJECT_NAME_EXTERNAL_NAME];
	std::string c_project_External_Name = projectInfo[c_project_cid]["external_name"];

	try
	{
		try
		{
			addAuditTrail("0","Generated a c_retrieval_job",m_dbCentral->getDatabaseStem());
			std::string retrieval_cid = getNextCentralIdSequence();
			TDateTime dtNow = System::Sysutils::Now();//get now
			dtNow = IncMinute(dtNow, 20); // add 20 mins
			System::UnicodeString Result;
			DateTimeToString(Result,"dd/mm/yyyy hh:nn",dtNow);
			std::string holdForDate = std::string("Date('") + std::string(AnsiString(Result.c_str()).c_str()) + std::string("')");
			std::string sqlInset = "INSERT INTO C_RETRIEVAL_JOB ";
			sqlInset += std::string("(retrieval_cid,exercise_cid ,external_name,description,job_type,project_cid ,primary_aliquot,secondary_aliquot ,process_cid ,status,claimed_until )");
			sqlInset += std::string("VALUES('"+retrieval_cid+"','0','"+external_name+"','")+description +std::string("','4','")+c_project_cid+std::string("','")+c_aliquot_cid+std::string("','0','"+m_c_id_sequence+"','0'," + holdForDate + ")");
//m_Memo->Lines->Add(sqlInset.c_str());
			XEXEC execSql(m_dbCentral.get(),sqlInset);
			throwUnless( execSql.exec(), "Failed to insert" );
		// Go through each box to be moved..
			//new crovial
			std::string cryovial_cid = "-1";
			//from box size
			int LastFreeSpace =  atoi(reBoxSize[reBoxSize_cid]["box_capacity"].c_str());
			int FirstFreeSpace = 1;
			int EmptyPosition = atoi(reBoxSize[reBoxSize_cid]["empty_position"].c_str());
			//new box
			std::string new_box_cid = "-1";   //new box
			//old box
			std::string box_cid = "-1";
			int NextFreePos = LastFreeSpace+1; //no free space. as no new box allocated.
			std::map<std::string , std::map<std::string ,AliquotInfo> >::iterator it = m_projectToAliquots.find(m_SelectedProject_cid);
			if (it != m_projectToAliquots.end())
			{
				std::string project_cid = it->first;
				std::map<std::string ,AliquotInfo> AliquotList = it->second;
				std::map<std::string ,AliquotInfo>::iterator itAli = AliquotList.find(m_SelectedAlquot_cid);
				if (itAli!=AliquotList.end())
				{
					AliquotInfo Aliquot = itAli->second;
					std::string thisAliquot = itAli->first;

					std::vector<BoxSizeStruct> BoxSizeList = Aliquot.m_AliquotsToBoxesSizeList;
					for (std::vector<BoxSizeStruct>::iterator itboxSize=BoxSizeList.begin(); itboxSize!=BoxSizeList.end(); ++itboxSize)
					{
						BoxSizeStruct Box_str = *itboxSize;

						//Now. Get a list of all vials from the db!
//SHOULD BE SORTED IN ANYWAY? BY CRYOVIAL_POSITION/TUBE_POSITION ?, try without first?

						std::map<std::string, std::map<std::string,std::string> > cryostore_Info;
						getVialsForAliquot(cryostore_Info,thisAliquot,Box_str.box_cid);

						for (std::map<std::string, std::map<std::string,std::string> >::iterator itCryoStores=cryostore_Info.begin(); itCryoStores!=cryostore_Info.end(); ++itCryoStores)
						{
							std::map<std::string,std::string> CroStoreVial = itCryoStores->second;
							std::string cryovial_store_record_id = CroStoreVial["cryovial_store.record_id"];
							std::string cryovial_store_cryovial_id = CroStoreVial["cryovial_store.cryovial_id"];
							std::string cryovial_store_sample_volume = CroStoreVial["cryovial_store.sample_volume"];
							if (NextFreePos == EmptyPosition)
								NextFreePos++;
							if (NextFreePos > LastFreeSpace)
							{   //CREATE A NEW BOX, it will do this first time
								new_box_cid = getNextProjectIdSequence();
								std::string barCode = new_box_cid;
								std::string external_name = c_project_External_Name + " " + c_aliquotInfo_External_Name + " " + new_box_cid;
								std::stringstream insertQuery;
								insertQuery << "INSERT INTO BOX_NAME ";
								insertQuery << "(box_cid,barcode,box_type_cid,box_capacity,external_name,status,note_exists,process_cid,time_stamp)";
								insertQuery << "VALUES('" << new_box_cid << "','" << barCode << "','" << reBoxType_cid;
								insertQuery << "','-1','" << external_name << "','0','0','" << m_c_id_sequence << "', date('now'))";
//m_Memo->Lines->Add(insertQuery.str().c_str());
								XEXEC execInsertSql(m_pCurrentProject,insertQuery.str());
								throwUnless( execInsertSql.exec(), "Failed to insert" );
								//set next free to the first free box.. check empty.. and docs for first 0 or 1
								NextFreePos = 1;
							}
							std::string updateQuery = "update cryovial_store set retrieval_cid = '"+ retrieval_cid+"', status  = '2' where cryovial_id = '" + cryovial_store_cryovial_id + "'";
//m_Memo->Lines->Add(updateQuery.c_str());
							XEXEC execUpdateSql(m_pCurrentProject,updateQuery);
							throwUnless( execUpdateSql.exec(), "Failed to insert" );
						//now create the NEW cryovial store enteries
							std::string record_id = getNextProjectIdSequence();
							std::stringstream insertQuery;
							insertQuery << "INSERT INTO cryovial_store ";
							insertQuery << "(record_id,cryovial_id,box_cid,tube_position,sample_volume,status,note_exists,process_cid,retrieval_cid,time_stamp )";
							insertQuery << "VALUES('" << record_id << "','" << cryovial_store_cryovial_id << "','" << new_box_cid;
							insertQuery << "','" << NextFreePos << "','" << cryovial_store_sample_volume << "','0','0','" << m_c_id_sequence << "','0','')";
//m_Memo->Lines->Add(insertQuery.str().c_str());
							XEXEC execInsertSql(m_pCurrentProject,insertQuery.str());
							throwUnless( execInsertSql.exec(), "Failed to insert" );
							NextFreePos++;
						}
					}
				}
			}
	   }
	   catch(std::exception e)
	   {
		   m_dbCentral->rollback();
		   m_pCurrentProject->rollback();
//does this save?
			addAuditTrail("5","Some error message",m_dbCentral->getDatabaseStem());
			//ERROR.. ROLL IT ALL BACK.
		   throw e;
	   }
	   catch(...)
	   {
			//ERROR.. ROLL IT ALL BACK.
		   m_dbCentral->rollback();
		   m_pCurrentProject->rollback();
//does this save?
			addAuditTrail("5","Some error message",m_dbCentral->getDatabaseStem());
			//ERROR.. ROLL IT ALL BACK.
		   throw "error";
	   }
   }
   __finally
   {
	//	submit transaction, if we have anything to submit
//	   	m_dbCentral->commit();
//		m_pCurrentProject->commit();
	//WHILE IN TEST
		m_dbCentral->rollback();
		m_pCurrentProject->rollback();
	//reset back auto commit .. check this will submit an error aduit message
		m_dbCentral->setAutoCommit(true);
		m_pCurrentProject->setAutoCommit(true);
   }
//sucess!
	addAuditTrail("7","Complete",m_dbCentral->getDatabaseStem());
}
//---------------------------------------------------------------------------
std::string database::getNextProjectIdSequence()
{
	std::string c_id_sequence = "";
	std::string sqlQuery = "SELECT NEXT VALUE FOR id_sequence";
	XQUERY query( m_pCurrentProject, sqlQuery);
	throwUnless( query.open(), "Failed" );
	if ( query.fetch() )
	{
		c_id_sequence = query.result.getString(0).c_str();
	}
	query.close();
	return c_id_sequence;
}
//---------------------------------------------------------------------------
std::string database::getNextCentralIdSequence()
{
	std::string c_id_sequence = "";
	std::string sqlQuery = "SELECT NEXT VALUE FOR c_id_sequence";
	XQUERY query( m_dbCentral.get(), sqlQuery);
	throwUnless( query.open(), "Failed" );
	if ( query.fetch() )
	{
		c_id_sequence = query.result.getString(0).c_str();
	}
	query.close();
	return c_id_sequence;
}
//---------------------------------------------------------------------------
void database::addAuditTrail(std::string messageType,std::string details,std::string databasename)
{
	std::string programname = DEFINE_PROGRAM_NAME + " " + DEFINE_PROGRAM_VERSION;
	std::string sqlInset = "INSERT INTO C_AUDIT_TRAIL (process_cid,operator_cid,message_type,machine_cid,database_name,program_name,details)";
	sqlInset += std::string("VALUES('"+m_c_id_sequence+"','")+m_pOperatorSignedIn->getOperator_cid()+std::string("','"+messageType+"','")+m_pOperatorSignedIn->getMachine_cid()+std::string("','")+databasename+std::string("','")+programname+std::string("','"+details+"')");
	XEXEC execSql(m_dbCentral.get(),sqlInset);
	throwUnless( execSql.exec(), "Failed to insert" );
}
//---------------------------------------------------------------------------
void database::signIn(const std::string &userName,std::string &password,operatorSignedIn &OperatorSignedIn)
{
	m_pOperatorSignedIn = &OperatorSignedIn;
	m_pOperatorSignedIn->setOperator_cid("");

	std::transform(password.begin(), password.end(), password.begin(), ::tolower);
	std::string password_md5 = XMD5::calcHex((const unsigned char*)password.c_str(),(const int)password.length());

	std::vector<std::string> what;
	what.push_back("operator_cid");
	std::string tablename = "C_OPERATOR";
	std::string where = "external_name = '" + userName + "' AND password_md5  = '" + password_md5 + "' AND status <> '99' AND status <> '90'";

	std::string myquery = getQuery(what,tablename,where);
	XQUERY query( m_dbCentral.get(), AnsiString(myquery.c_str()).c_str());
//	throwUnless( query.open(), "Failed" );
	if (!query.open())
		return;
	if ( query.fetch() )
	{
		m_pOperatorSignedIn->setOperator_cid(query.result.getString(0).c_str());
	}
	query.close();

	if (!m_pOperatorSignedIn->getOperator_cid().empty())
	{
	//add the user signed in audit message
		getMachine_cid();
		getOperatorPremissions();

		if (!m_pOperatorSignedIn->hasPermissionToSignIn())
		{
			m_Memo->Lines->Add("Your account doesn't have the correct permissions to use this software");
        	return;
		}

		m_c_id_sequence = getNextCentralIdSequence();
		addAuditTrail("3","** Operator logging in **",m_dbCentral->getDatabaseStem());
	}
}
//---------------------------------------------------------------------------
void database::getOperatorPremissions()
{
	std::map<std::string, std::map<std::string,std::string> > operatorPremissions;
	std::vector<std::string> what;
	what.push_back("permission_cid");
	what.push_back("program_cid");
	what.push_back("project_cid");
	what.push_back("machine_cid");
	what.push_back("page_number");
	std::string tablename = "C_PERMISSION";
	std::string where = "operator_cid  = '" + m_pOperatorSignedIn->getOperator_cid()  + "' AND status <> '99'";

	runQuery(what,tablename,where,operatorPremissions);
	m_pOperatorSignedIn->setPremissions(operatorPremissions);
}
//---------------------------------------------------------------------------
void database::getMachine_cid()
{
	std::string machine_cid = "0"; //default it to no machine
	TCHAR szBuffer[MAX_COMPUTERNAME_LENGTH +1];
	DWORD dwSize = MAX_COMPUTERNAME_LENGTH + 1;
	if (GetComputerName(szBuffer, &dwSize))
	{
		String computername = szBuffer;

		std::vector<std::string> what;
		what.push_back("machine_cid");
		std::string tablename = "C_MACHINE";
		std::string where = "external_name = '" + std::string(AnsiString(computername.c_str()).c_str()) + "'";

		std::string myquery = getQuery(what,tablename,where);
		XQUERY query( m_dbCentral.get(), AnsiString(myquery.c_str()).c_str());
		throwUnless( query.open(), "Failed to retreve" );
		if ( query.fetch() )
		{
			machine_cid = query.result.getString(0).c_str();
		}
		query.close();
	}
	m_pOperatorSignedIn->setMachine_cid(machine_cid);
}
//---------------------------------------------------------------------------
std::string database::getQuery(const std::vector<std::string> &what,const std::string &tablename,const std::string &where)
{
	std::stringstream sqlquery;
	sqlquery <<  "SELECT ";
	for (unsigned int i=0;i<what.size();i++)
	{
		if (i > 0)
			sqlquery << ",";
		sqlquery << 	" " << what[i] << " ";
	}
	sqlquery << " FROM " << tablename << " WHERE " << where;
	return sqlquery.str();
}
//---------------------------------------------------------------------------
String database::runQuery(const std::vector<std::string> &what,const std::string &tablename,const std::string &where,std::map<std::string, std::map<std::string,std::string> > &returnInfo)
{
	std::wstringstream sqlquery;
	sqlquery <<  "SELECT ";
	for (unsigned int i=0;i<what.size();i++)
	{
		if (i > 0)
			sqlquery << ",";
		sqlquery << 	" " << what[i].c_str() << " ";
	}
	sqlquery << " FROM " << tablename.c_str() << " WHERE " << where.c_str();

	XQUERY query( m_dbCentral.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed");
	while ( query.fetch() )
	{
		std::string key = query.result.getString(0).c_str();
		std::map<std::string,std::string> info;
		for (unsigned int i=0;i<what.size();i++)
		{
			info[what[i]] = query.result.getString(i);
		}
		returnInfo.insert(std::pair<std::string, std::map<std::string,std::string> >(key,info));
	}
	query.close();
	return String(sqlquery.str().c_str());
}
//---------------------------------------------------------------------------

String database::runProjectQuery(const std::vector<std::string> &what,const std::string &tablename,const std::string &where,std::map<std::string, std::map<std::string,std::string> > &returnInfo)
{
	std::wstringstream sqlquery;
	sqlquery <<  "SELECT ";
	for (unsigned int i=0;i<what.size();i++)
	{
		if (i > 0)
			sqlquery << ",";
		sqlquery << 	" " << what[i].c_str() << " ";
	}
	sqlquery << " FROM " << tablename.c_str() << " WHERE " << where.c_str();

//m_Memo->Lines->Add(String(sqlquery.str().c_str()));

	XQUERY query( m_pCurrentProject, AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve" );
	while ( query.fetch() )
	{
		std::string key = query.result.getString(0).c_str();
		std::map<std::string,std::string> info;
  //		m_Memo->Lines->Add(String("--"));
		for (unsigned int i=0;i<what.size();i++)
		{
			info[what[i]] = query.result.getString(i);
  //			m_Memo->Lines->Add(String(query.result.getString(i).c_str()));
		}
		returnInfo.insert(std::pair<std::string, std::map<std::string,std::string> >(key,info));
	}
	query.close();
	return String(sqlquery.str().c_str());
}
//---------------------------------------------------------------------------
String database::IntToString(int i)
{
	return String(IntToStdString(i).c_str());
}
//---------------------------------------------------------------------------

std::string database::IntToStdString(int i)
{
	std::stringstream ss;
	ss << i;
	return ss.str();
}
//---------------------------------------------------------------------------
void database::throwUnless( bool condition, const String& msg )
{
	if ( ! condition )
	{
		throw msg;
	}
}
//---------------------------------------------------------------------------

bool database::m_bQuitting = false;

void onError( const std::string msg )
{
	throw String(msg.c_str());
}
//---------------------------------------------------------------------------

bool dbErrorCallback( const std::string object, const int instance,const int ecount, const int ecode, const std::string error_txt )
{
	if (!database::m_bQuitting)
 		onError( error_txt );
	return( true );
}


