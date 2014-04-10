//---------------------------------------------------------------------------

#ifndef sessionH
#define sessionH

#include <string>
#include "option.h"
#include "stage.h"
#include "result.h"
#include <sstream>
#include <vector>

class RAND_UTIL;
class XCGI;
class ROSETTA;
class XDB;
class XQUERY;


//---------------------------------------------------------------------------

/*
 * Session varifies user sign on and
 */
class session
{
	static std::vector<session > m_staffnames;
	std::string m_name;

	Result m_username;
	Result m_password;
 	std::string m_viewpage;

	std::string m_ctsu_signon;
	stage m_stage; //first sign on
	std::string m_error;

	void readDetails(const ROSETTA & fields );
public:
	const session *find( std::string name ) const;
	std::string getName() const {return m_name;}

	session();
	session( const std::string & name);

	void readList(const XDB *db );
	void checkUser(RAND_UTIL *ru,const XDB *db );
	stage getStage( ) const {return m_stage;}

	std::string getMessage() const { return m_error; }
	void addFields( ROSETTA & fields ) const;

	const std::vector<session > & getList() const { return m_staffnames; }

	Result getSelectUserID() const {return m_username;}
	Result getSelectPin() const {return m_password;}
	std::string getViewPage() const {return m_viewpage;}
	void parse(const XCGI *cgi,const ROSETTA &R);
};

//---------------------------------------------------------------------------
#endif



