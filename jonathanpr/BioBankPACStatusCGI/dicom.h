//---------------------------------------------------------------------------
#ifndef dicomH
#define dicomH

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

class dicom
{
	const static char* dicom::enumUnknownWHEREAUs[];
//	static std::vector<dicom > m_dicomStudies;
	bool m_ready;
	std::string m_patiendid;
	XTIME m_studydate;
	std::string m_report;
	int m_findingsStatus;
	int m_alert;
	std::string m_pacsurl;
	std::string m_md5zip;

	stage m_stage; //first sign on
	std::string m_error;

	Result m_dicompage;

	Result m_statusFilter;
	Result m_CTSUFilter;
	Result m_reportsFilter;
	Result m_alertsFilter;
	Result m_timespanFilter;
	Result m_statusPatientIDfilter;

	void readDetails(const ROSETTA & fields );
public:
	const bool isReady() const
	{
		return m_ready;
	}

	const dicom *find(const std::string &patiendid, const XTIME &date ) const;

	dicom();
	dicom(const std::string &patiendid,const XTIME &studydate,const std::string &report,int findingsStatus,int alert,const std::string &pacsurl,const std::string &md5zip);

	void readList(const XDB *db );
	stage getStage( ) const {return m_stage;}

	std::string getMessage() const { return m_error; }
	void addFields( ROSETTA & fields ) const;

//	const std::vector<dicom > & getList() const { return m_dicomStudies; }

	Result getSelectedPage() const {return m_dicompage;}
	void parse(const XCGI *cgi,const ROSETTA &R);

	const std::string getPatiendid() const {return m_patiendid;}
	const XTIME getStudydate() const {return  m_studydate;}
	const std::string getReport() const {return  m_report;}
	const int getFindingsStatus() const {return  m_findingsStatus;}
	const int getAlert() const {return  m_alert;}
	const std::string getPacsurl() const {return  m_pacsurl;}
	const std::string getMd5zip() const {return  m_md5zip;}

	const int getStatusFilter() const {return m_statusFilter.igetText();}
	const int geCTSUFiltert() const {return m_CTSUFilter.igetText();}
	const int getReportsFilter() const {return m_reportsFilter.igetText();}
	const int getAlertsFilter() const {return m_alertsFilter.igetText();}
	const int getTimespanFilter() const {return m_timespanFilter.igetText();}
	const std::string getStatusPatientIDfilter() const {return m_statusPatientIDfilter.getText();}
//for the stats page.
	void getGeneralStats(const XDB *db,const std::string &type,ROSETTA &R) const;
	void getUnknownStats(const XDB *db,ROSETTA &R) const;
	void runGetTotalSQL(const XDB *db,const std::string &sql,const std::string &text,ROSETTA &R) const;
	std::string getCronJobString(const XDB *db,const std::string &name) const;
};

//---------------------------------------------------------------------------
#endif



