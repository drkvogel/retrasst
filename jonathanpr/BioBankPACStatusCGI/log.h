//---------------------------------------------------------------------------

#ifndef logH
#define logH

#include <string>
#include <sstream>
#include <vector>
#include "stage.h"
#include "result.h"

class RAND_UTIL;
class XCGI;
class ROSETTA;
class XDB;
class XQUERY;

//---------------------------------------------------------------------------
//PAGE 2 - Logs
//Keeps the state of page 2 and loads in all the information from the database for page 2
class PACSlog
{
//Have we loaded in the data from the database?
	bool m_ready;
//Where the loaded database information is stored
	static std::vector<PACSlog> m_logs;

//The raw database data
	int m_id;
	int m_loglevel;
	XTIME m_timestamp;
	int m_logtype;
	std::string m_message;
	std::string m_patientid;
	XTIME m_studydate;

	//has the total number of rows returns from the database
	int m_rowCount;

//PAGE State
	Result m_logpage; //which table page are we on
//FILTERS
	Result m_logLevelFilter;
	Result m_logTypeFilter;
	Result m_logTimespanFilter;
	Result m_logPatientIDfilter;

	void featchTotal( XDB *db, int fromID, int maxCount );
	void readDetails(const ROSETTA & fields );
	std::string generateQuery(bool justCount, int fromID, int maxCount );
public:
	PACSlog();
	PACSlog(int id, int loglevel,const XTIME &timestamp,int logtype,const std::string &message,const std::string &patientid,const XTIME &studydate);

	const bool isReady() const
	{
		return m_ready;
	}

	void readList(const XDB *db);
	const std::vector<PACSlog> & getList() const { return m_logs; }
	void addFields( ROSETTA & fields ) const;

	Result getSelectedPage() const {return m_logpage;}
	void parse(const XCGI *cgi,const ROSETTA &R);

	const int getLoglevel() const {return m_loglevel;}
	const XTIME getTimestamp() const {return  m_timestamp;}
	const int getLogtype() const {return  m_logtype;}
	const std::string getMessage() const {return m_message;}
	const std::string getPatientid() const {return m_patientid;}
	const XTIME getStudyDate() const {return m_studydate;}

	const int getRowCount() const {return m_rowCount;}

	const int getlogLevelFilter() const {return m_logLevelFilter.igetText();}
	const int gelogTypeFilter() const {return m_logTypeFilter.igetText();}
	const int gettimespanFilter() const {return m_logTimespanFilter.igetText();}
	const std::string getlogPatientIDfilter() const {return m_logPatientIDfilter.getText();}
};

//---------------------------------------------------------------------------
#endif



