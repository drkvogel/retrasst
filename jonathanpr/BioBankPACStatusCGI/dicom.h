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


//Deals with page 1, retreaving the required information for the page and also keeps track of that page state.
class dicom
{
	const static char* enumUnknownWHEREAUs[];
	Result m_dicompage;

//Filter status on the page
	Result m_statusFilter;
	Result m_CTSUFilter;
	Result m_reportsFilter;
	Result m_alertsFilter;
	Result m_timespanFilter;
	Result m_statusPatientIDfilter;

	void readDetails(const ROSETTA & fields );
public:
	dicom();
	void addFields( ROSETTA & fields ) const;

	Result getSelectedPage() const {return m_dicompage;}
	void parse(const XCGI *cgi,const ROSETTA &R);

	const int getStatusFilter() const {return m_statusFilter.igetText();}
	const int geCTSUFiltert() const {return m_CTSUFilter.igetText();}
	const int getReportsFilter() const {return m_reportsFilter.igetText();}
	const int getAlertsFilter() const {return m_alertsFilter.igetText();}
	const int getTimespanFilter() const {return m_timespanFilter.igetText();}
	const std::string getStatusPatientIDfilter() const {return m_statusPatientIDfilter.getText();}
	void getGeneralStats(const XDB *db,const std::string &type,ROSETTA &R) const;
	void getUnknownStats(const XDB *db,ROSETTA &R) const;
	void runGetTotalSQL(const XDB *db,const std::string &sql,const std::string &text,ROSETTA &R) const;
	std::string getCronJobString(const XDB *db,const std::string &name) const;
};

//---------------------------------------------------------------------------
#endif



