//---------------------------------------------------------------------------

#ifndef centreH
#define centreH
//---------------------------------------------------------------------------
#include <vector>
#include <cstdlib>
#include "option.h"
#include "result.h"
#include "stage.h"

class XDB;
class XCGI;
class ROSETTA;

//---------------------------------------------------------------------------

class centre
{
	static std::vector< centre > centres;

	std::string m_nhstrust;
	std::string m_accocuniv;
	int m_pinvestigator_id;

	std::string m_error;

	centre( int recID, const std::string & nam,const std::string & nhstrust,const std::string & accocuniv,int pinvestigatorid);
	const centre * find( int recID ) const;
	void readDetails(const ROSETTA & fields );
	void addToRosetta(ROSETTA & fields,const std::string &name,int var) const;
public:

	Result selectedCentre;
	//int id;
  //std::string name;
	const int getID() const { return selectedCentre.id; }
	std::string getName() const { return selectedCentre.getText(); }

	void parse(const XCGI *cgi,const ROSETTA &R);
	centre(/* const XCGI *cgi */);
	void checkDetails();
	std::string getMessage() const { return m_error; }

	void addForDBFields( ROSETTA & fields ) const;
	void addFields( ROSETTA & fields ) const;
	stage getStage( ) const ;

	void readList( XDB *db );
	const std::vector<centre > & getList() const { return centres; }
};

//---------------------------------------------------------------------------



#endif





