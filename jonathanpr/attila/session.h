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

class session
{
	static std::vector<session > m_staffnames;
	int m_id;
	std::string m_name;
	std::string m_phone;

	Result m_user_id;
	Result m_pin;

	std::string m_ctsu_signon;
	stage m_stage; //first sign on
	stage m_vstage; //varification before submit
	std::string m_error;

	Result m_varifiy_user_id;
	Result m_varifiy_pin;

	void readDetails(const ROSETTA & fields );
public:
	const session *find( int recID ) const;
	int getID() const {return m_id;}
	std::string getName() const {return m_name;}
	std::string getTelNumber() const {return m_phone;}

	session();
	session( int recID, const std::string & name, const std::string &phone);

	void readList( XDB *db );
	void checkUser( RAND_UTIL *ru,XDB *db );
	void checkUserValidation( RAND_UTIL *ru,XDB *db );
	stage getStage( ) const {return m_stage;}
	stage getVarificationStage() const {return m_vstage;}

	std::string getMessage() const { return m_error; }
	void addFields( ROSETTA & fields ) const;
	void addForDBFields( ROSETTA & fields ) const;

	const std::vector<session > & getList() const { return m_staffnames; }

	Result getSelectUserID() const {return m_user_id;}
	Result getSelectPin() const {return m_pin;}
	Result getVarifiyUserID() const {return m_varifiy_user_id;}
	Result getVarifiyPin() const {return m_varifiy_pin;}
    void parse(const XCGI *cgi,const ROSETTA &R);
};

//---------------------------------------------------------------------------
#endif



