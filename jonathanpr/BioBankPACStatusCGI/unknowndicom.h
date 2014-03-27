//---------------------------------------------------------------------------
#ifndef unknowndicomH
#define unknowndicomH

#include <string>
#include <sstream>
#include <set>
#include <vector>
#include "stage.h"
#include "result.h"

class RAND_UTIL;
class XCGI;
class ROSETTA;
class XDB;
class XQUERY;

class unknowndicom
{
	static std::vector<unknowndicom > m_unknowndicom;
	static std::set<std::string> m_aetList;

	bool m_ready;

	long m_studypk;
	std::string m_aet;
	XTIME m_studydate;
	std::string m_patientid;
	std::string m_firstname;

	stage m_stage; //first sign on
	std::string m_error;

	Result m_unknownpage;
	Result m_aetFilter;
	Result m_timespanFilter;

	void readDetails(const ROSETTA & fields );
	void getAETs(const XDB *db);
public:
	const bool isReady() const
	{
		return m_ready;
	}

	const unknowndicom *find(const int studypk) const;

	unknowndicom();
	unknowndicom(long studypk,const std::string &aet,const XTIME &studydate,const std::string &patientid,const std::string &firstname);

	void readList(const XDB *db );
	stage getStage( ) const {return m_stage;}

	std::string getMessage() const { return m_error; }
	void addFields( ROSETTA & fields ) const;

	const std::vector<unknowndicom > & getList() const { return m_unknowndicom; }
	const std::set<std::string> &getAETs() const {return m_aetList; }


	Result getSelectedPage() const {return m_unknownpage;}
	void parse(const XCGI *cgi,const ROSETTA &R);

	const long getStudypk() const{return m_studypk;}
	const std::string getAet() const{return  m_aet;}
	const XTIME getStudydate() const{return  m_studydate;}
	const std::string getPatientid() const{return  m_patientid;}
	const std::string getFirstname() const{return  m_firstname;}

	const int getTimespanFilter() const {return m_timespanFilter.igetText();}
	const std::string getAetFilter() const {return m_aetFilter.getText();}
};

//---------------------------------------------------------------------------
#endif



