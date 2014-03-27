//---------------------------------------------------------------------------
#pragma hdrstop

#include "projectAccount.h"
#include <sstream>
#include "utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

std::vector<projectAccount> projectAccount::m_projectAccount;

int projectAccount::DEFINE_WINDOWS = 0;
int projectAccount::DEFINE_LINUX = 1;
int projectAccount::DEFINE_GPU = 2;
int projectAccount::NEXT_USERID = -1;

int projectAccount::getNextUserID()
{
	NEXT_USERID--;
	return NEXT_USERID;
}
//---------------------------------------------------------------------------

projectAccount::projectAccount()
{
}
//---------------------------------------------------------------------------

projectAccount::projectAccount(int userID,int status,const std::string &userName,int accountType,const std::string &accountAttribs)
{
	m_userID = userID;
	m_userName = userName;
	m_accountType = accountType;
	m_status = status;
	m_attribute.deseriliseAttrib(accountAttribs);
}
//---------------------------------------------------------------------------

projectAccount::projectAccount(int userID,int status,const std::string &userName,int accountType)
{
	m_userID = userID;
	m_userName = userName;
	m_accountType = accountType;
	m_status = status;
}
//---------------------------------------------------------------------------

void projectAccount::loadAttribs(XDB *db)
{
	for( std::vector< projectAccount >::iterator ci = m_projectAccount.begin( ); ci != m_projectAccount.end( ) ; ++ ci )
		ci->m_attribute.load(db,ci->m_userID);
}
//---------------------------------------------------------------------------

projectAccount::~projectAccount()
{
}
//---------------------------------------------------------------------------

void projectAccount::initaliseAccounts()
{
	m_projectAccount.clear( );
	m_attribute.clearAttib();
}
//---------------------------------------------------------------------------

void projectAccount::load(XDB *db,int applicationID)
{
	m_projectAccount.clear();

	ROSETTA output;
	std::stringstream sql;
	sql << "SELECT * FROM ru_user WHERE app_id = '" << applicationID << "' AND status <> -1";

	XQUERY query(db,sql.str());

	if (!query.open())
		throw String(query.getLastError().c_str());
	while (query.fetch(&output))
	{
		std::string name = output.getString("name").c_str();
		int type = output.getInt("type");
		int id = output.getLint("id");
		int status = output.getInt("status");
		m_projectAccount.push_back(projectAccount(id, status,name, type));
	}
	query.close();
	loadAttribs(db);
}
//---------------------------------------------------------------------------

void projectAccount::save(XDB *db,int applicationID)
{
	for( std::vector< projectAccount >::iterator ci = m_projectAccount.begin( ); ci != m_projectAccount.end( ) ; ++ ci )
	{
		ROSETTA R;
		ci->m_userID = utils::getNextValueFor(db,"user_id_seq");

		R.setInt("id",ci->m_userID);
		R.setString("name",ci->m_userName);
		R.setInt("type",ci->m_accountType);
		R.setInt("status",ci->m_status);
		R.setInt("app_id",applicationID);
		XEXEC exec(db);
		if (!exec.insert("ru_user",&R))
			throw exec.getLastError();

		ci->m_attribute.save(db,ci->m_userID);
	}
}
//---------------------------------------------------------------------------

void projectAccount::clear(void)
{
	m_projectAccount.clear();
}
//---------------------------------------------------------------------------

void projectAccount::update(XDB *db,int applicationID)
{
	ROSETTA R;
	ROSETTA RWhere;
	XEXEC exec(db);

	for( std::vector< projectAccount >::iterator ci = m_projectAccount.begin( ); ci != m_projectAccount.end( ) ; ++ ci )
	{
		R.clear();
		if (ci->m_userID >= 0)
		{
 			XTIME D;
			D.setNow();
			//update
			R.setString("name",ci->m_userName);
			R.setInt("type",ci->m_accountType);
			R.setInt("status",ci->m_status);

			R.setInt("id",ci->m_userID);
			std::string sql = "UPDATE ru_user SET name= :name,type= :type,status = :status WHERE id=:id";
			exec.setSQL(sql);
			exec.setParamSource(&R);
			if (!exec.exec())
				throw exec.getLastError();
			ci->m_attribute.save(db,ci->m_userID);
		}
		else
		{
			//save a new.
			ci->m_userID = utils::getNextValueFor(db,"user_id_seq");
			R.setInt("id",ci->m_userID);
			R.setString("name",ci->m_userName);
			R.setInt("type",ci->m_accountType);
			R.setInt("app_id",applicationID);
			R.setInt("status",ci->m_status);

			if (!exec.insert("ru_user",&R))
				throw exec.getLastError();
			ci->m_attribute.save(db,ci->m_userID);
		}
	}
}

//---------------------------------------------------------------------------
projectAccount* projectAccount::findProject( int userID )
{
	for( std::vector< projectAccount >::iterator ci = m_projectAccount.begin( ); ci != m_projectAccount.end( ) ; ++ ci )
	{
		if(ci->getuserID() == userID)
			return &(*ci);
	}
	return NULL;
}
//---------------------------------------------------------------------------

int projectAccount::getAccountID(int type)
{
	if ((projectAccount::DEFINE_LINUX == type) || (projectAccount::DEFINE_GPU == type))
	{
		for( std::vector< projectAccount >::const_iterator ci = m_projectAccount.begin( ); ci != m_projectAccount.end( ) ; ++ ci )
		{
			if(ci->m_accountType == type)
				return ci->m_userID;
		}
	}
//check db, load in if nessary
	return -1;
}

//---------------------------------------------------------------------------
bool projectAccount::setCurrent(int userID )
{
	for( std::vector< projectAccount >::const_iterator ci = m_projectAccount.begin( ); ci != m_projectAccount.end( ) ; ++ ci )
	{
		if(ci->getuserID() == userID)
		{
			m_userID = ci->m_userID;
			m_userName = ci->m_userName;
			m_status = ci->m_status;
			m_accountType = ci->m_accountType;
			m_attribute.deseriliseAttrib(ci->m_attribute.seriliseAttrib());
			return true;
		}
	}
	return false;
}
//---------------------------------------------------------------------------

bool projectAccount::has(const std::string &username, int type)
{
	for( std::vector< projectAccount >::const_iterator ci = m_projectAccount.begin( ); ci != m_projectAccount.end( ) ; ++ ci )
	{
		if ((ci->m_accountType == type) && (ci->m_userName == username))
			return true;
	}
	return false;
}

//---------------------------------------------------------------------------
// any users like this? for testing if we have added a user like the user pattern
bool projectAccount::isLike(const std::string &username)
{
	for( std::vector< projectAccount >::const_iterator ci = m_projectAccount.begin( ); ci != m_projectAccount.end( ) ; ++ ci )
	{
		if (ci->m_userName.compare(0,username.length(),username) == 0)
			return true;
	}
	return false;
}
//---------------------------------------------------------------------------

bool projectAccount::hasInDB(XDB *db,const std::string &username, int type)
{
	bool res = false;
	ROSETTA output;
	std::stringstream sql;
	sql << "SELECT count(*) AS COUNT FROM ru_user WHERE name = '" << username << "' AND type = " << type;

 	XQUERY query(db,sql.str());

	if (!query.open())
	{
		std::string togo ="Error occured with trying to varify username.\r\n\r\n Error:" + query.getLastError();
		throw togo;
	}
	if (query.fetch(&output))
	{
		int type = output.getInt("COUNT");
		res = type > 0;
	}
	query.close();
	return res;
}
//---------------------------------------------------------------------------

int projectAccount::loadTypeInDB(XDB *db,int applicationID, int type)
{
	int id = -1;
	ROSETTA output;
	std::stringstream sql;
	sql << "SELECT * FROM ru_user WHERE app_id = '" << applicationID << "' AND type = " << type;

	XQUERY query(db,sql.str());

	if (!query.open())
	{
		std::string togo ="Error occured with trying to varify username.\r\n\r\n Error:" + query.getLastError();
		throw togo;
	}
	if (query.fetch(&output))
	{
		std::string name = output.getString("name").c_str();
		int type = output.getInt("type");
		id = output.getLint("id");
		int status = output.getInt("status");
		m_projectAccount.push_back(projectAccount(id, status,name, type));
	}
	query.close();
	//don't load in the attribs.. so when it saves it's a new account.
	return id;
}

//---------------------------------------------------------------------------

const String projectAccount::getNoWindowsAccounts() const
{
	int count = 0;
	for( std::vector< projectAccount >::const_iterator ci = m_projectAccount.begin( ); ci != m_projectAccount.end( ) ; ++ ci )
	{
		if (ci->m_accountType == DEFINE_WINDOWS)
			count++;
	}
	std::stringstream ss;
	ss << count;
	return String(ss.str().c_str());
}
//---------------------------------------------------------------------------

const String projectAccount::getNoLinuxAccounts() const
{
	int count = 0;
	for( std::vector< projectAccount >::const_iterator ci = m_projectAccount.begin( ); ci != m_projectAccount.end( ) ; ++ ci )
	{
		if (ci->m_accountType == DEFINE_LINUX)
			count++;
	}
	std::stringstream ss;
	ss << count;
	return String(ss.str().c_str());
}
//---------------------------------------------------------------------------

const String projectAccount::getNoGPUAccounts() const
{
	int count = 0;
	for( std::vector< projectAccount >::const_iterator ci = m_projectAccount.begin( ); ci != m_projectAccount.end( ) ; ++ ci )
	{
		if (ci->m_accountType == DEFINE_GPU)
			count++;
	}
	std::stringstream ss;
	ss << count;
	return String(ss.str().c_str());
}
//---------------------------------------------------------------------------

void projectAccount::addUserAccount(const std::string &userName,int status,int accountType,const std::string &accountAttribs)
{
	m_projectAccount.push_back(projectAccount(getNextUserID(), status, userName, accountType,accountAttribs));
}
//---------------------------------------------------------------------------

void projectAccount::updateUserAccount(const int uid,int status, const std::string &userName,const std::string &accountAttribs)
{
	projectAccount* pa = projectAccount::findProject(uid);
	if (!pa)
		return;

	pa->m_userName = userName;
	pa->m_status = status;
	pa->m_attribute.clearAttib();
	pa->m_attribute.deseriliseAttrib(accountAttribs);
}
//---------------------------------------------------------------------------

void projectAccount::updateUserAccount(const int uid,int status)
{
	projectAccount* pa = projectAccount::findProject(uid);
	if (!pa)
		return;
	pa->m_status = status;
}
//---------------------------------------------------------------------------

const String projectAccount::getStatusType() const
{
	if (m_status == 0)
		return L"NEW";
	else if (m_status == 1)
		return L"ACTIVE";
	else if (m_status == -1)
		return L"DELETED";
	return L"UNKNOWN";
}

//---------------------------------------------------------------------------
int projectAccount::getStatusValue(const String &value)
{
	if (value.LowerCase() == L"new")
		return 0;
	else if (value.LowerCase() == L"active")
		return 1;
	else
		return -1;
}
//---------------------------------------------------------------------------

const String projectAccount::getAccountType() const
{
	if (m_accountType == DEFINE_WINDOWS)
		return L"WINDOWS";
	else if (m_accountType == DEFINE_LINUX)
		return L"LINUX";
	else if (m_accountType == DEFINE_GPU)
		return L"GPU";
	return L"UNKNOWN";
}
//---------------------------------------------------------------------------


