//---------------------------------------------------------------------------

#ifndef clinicianH
#define clinicianH
//---------------------------------------------------------------------------

#include <vector>
#include <cstdlib>
#include "option.h"
#include "stage.h"
#include "result.h"

class XDB;
class XCGI;
class ROSETTA;
class centre;

//---------------------------------------------------------------------------

class clinician
{
	static std::vector<clinician > workers;

	std::string m_email;
	std::string m_phone;
	std::string m_error;

	const clinician * find( int recID ) const;
	clinician( int recID, const std::string & title, const std::string & given, const std::string & family );

   	void readDetails(const ROSETTA & fields );
	void addToRosetta(ROSETTA & fields,const std::string &name,int var) const;

public:
	clinician(/* const XCGI *cgi */);

	Result selectedClinician;
	std::string selectedphone;
	std::string selectedemail;
	void parse(const XCGI *cgi,const ROSETTA &R);
	std::string getName() const { return selectedClinician.getText(); }
	std::string getTelNumber() const { return selectedphone; }
	std::string getEmail() const { return selectedemail; }

	void checkDetails( );
	void checkLink( const centre &location, XDB *db );
	stage getStage( ) const ;
	std::string getMessage() const { return m_error; }
	void addFields( ROSETTA & fields ) const;
	void addForDBFields( ROSETTA & fields ) const;

	void readList( XDB *db );
	const std::vector< clinician > & getList() const { return workers; }
};

//---------------------------------------------------------------------------

#endif
