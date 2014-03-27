//---------------------------------------------------------------------------

#ifndef projectAccountH
#define projectAccountH
#include <vcl.h>
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <map>
#include "xexec.h"
#include "ROSETTA.h"
#include "attribute.h"

class projectAccount
{
public:
	static int DEFINE_WINDOWS;
	static int DEFINE_LINUX;
	static int DEFINE_GPU;

	static int NEXT_USERID;

//constructors/destructors
	projectAccount();
	~projectAccount();

	void initaliseAccounts();
	void load(XDB *db,int applicationID);

	void save(XDB *db,int applicationID);
	void update(XDB *db,int applicationID);

	static bool has(const std::string &username, int type);
	static bool isLike(const std::string &username);
	static bool hasInDB(XDB *db,const std::string &username, int type);
	void clear(void);

	int getAccountID(int type);
	int loadTypeInDB(XDB *db,int applicationID, int type);
//access
	static void addUserAccount(const std::string &userName,int status, int accountType,const std::string &accountAttribs);
	static void updateUserAccount(const int uid,int status,const std::string &userName,const std::string &accountAttribs);
	static void updateUserAccount(const int uid,int status);
//	static void deleteUserAccount(int id);

//these are temp user ids, pre-saving, at which point they get a real userid
	const int getuserID() const
	{
		return m_userID;
	}

	const int getPreviousUserID() const
	{
		return m_prevUserID;
	}
	static int getNextUserID();  //unsaved user id

	const int getOSType() const {return m_accountType;}
	const String getUserName() const {return String(m_userName.c_str());}
	const String getAccountType() const;
	const std::vector< projectAccount > *getAccounts() {return &m_projectAccount;}
	const String getNoWindowsAccounts() const;
	const String getNoLinuxAccounts() const;
	const String getNoGPUAccounts() const;
	const int getNextWindowAccountID() const;
	const String getAttribString() const {return m_attribute.getAttribString();}
	attribute *getAttribute() {return &m_attribute;}
	const String getStatusType() const;
	static int getStatusValue(const String &value);
	bool setCurrent( int userID );

private:
	projectAccount(int userID,int status,const std::string &userName,int accountType,const std::string &accountAttribs);
	projectAccount(int userID,int status,const std::string &userName,int accountType);

	static projectAccount * findProject( int userID );
	void loadAttribs(XDB *db);

	int m_userID;
	int m_prevUserID;
	std::string m_userName;
	int m_accountType;
	int m_status;

	static std::vector< projectAccount > m_projectAccount;
	attribute m_attribute;
};

#endif
