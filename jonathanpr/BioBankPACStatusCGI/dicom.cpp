//---------------------------------------------------------------------------
#pragma hdrstop

#include "dicom.h"
#include "xcgi.h"
#include "xencode.h"
#include "randutil.h"
#include "xquery.h"
#include "xexec.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//std::vector<dicom> dicom::m_dicomStudies;
//---------------------------------------------------------------------------

dicom::dicom(const std::string &patiendid,const XTIME &studydate,const std::string &report,int findingsStatus,int alert,const std::string &pacsurl,const std::string &md5zip)
{
	m_patiendid = patiendid;
	m_studydate = studydate;
	m_report = report;
	m_findingsStatus = findingsStatus;
	m_alert = alert;
	m_pacsurl = pacsurl;
	m_md5zip = md5zip;
}
//---------------------------------------------------------------------------

dicom::dicom() : m_stage(UNKNOWN)
{
	m_ready = false;
}
//---------------------------------------------------------------------------

std::string dicom::getCronJobString(const XDB *db,const std::string &name) const
{
	std::string sql = "SELECT started,finished FROM cronstatus WHERE name = '" + name + "'";
	std::string returnText;

	XQUERY qp((XDB *) db, sql);
	if( qp.open( ) )
	{
		if( qp.fetch( ) )
		{
			XTIME started =  qp.result.getTime("started");
			XTIME finished =  qp.result.getTime("finished");

			if (started.getYear() < 2010)
				returnText = "Not running";
			else
			{
				returnText = "Started:" + started.dmy(0) + " " + started.hms(0);
				if (finished >= started)
					returnText += ", Finished:" + finished.dmy(0) + " " + finished.hms(0);
				else
					returnText += " and still running";
			}
		}
		qp.close( );
	}
	return returnText;
}
//---------------------------------------------------------------------------


void dicom::runGetTotalSQL(const XDB *db,const std::string &sql,const std::string &text,ROSETTA &R) const
{
	XQUERY qp((XDB *) db, sql);
	if( qp.open( ) )
	{
		if( qp.fetch( ) )
		{
			std::stringstream count;
			count << qp.result.getInt("count");
			R.setString(text,count.str());
		}
		qp.close( );
	}
}
//---------------------------------------------------------------------------

void dicom::getGeneralStats(const XDB *db,const std::string &type,ROSETTA &R) const
{
	std::string status = "";
	if (type == "Total")
	{
	}
	else
	{
		if (type == "Complete")
			status = " WHERE STATUS = 2 ";
		else if (type == "Inprogress")
			status = " WHERE STATUS = 1 ";
		else if (type == "Ignored")
			status = " WHERE STATUS = 3 ";
	}
	std::string au ="";
	for (int i=0;i<5;i++)
	{
		std::string sql = "select count(*) AS count from pacsstudy ";

		if ((status.empty()) && (std::string(enumUnknownWHEREAUs[i]).empty()))
		{
		}
		else if (std::string(enumUnknownWHEREAUs[i]).empty()) //just enumUnknownWHEREAUs is empty
			sql += status;
		else if (status.empty())
			sql += " WHERE " + std::string(enumUnknownWHEREAUs[i]);
		else //both are good
			sql += status + " AND " + std::string(enumUnknownWHEREAUs[i]);

		if (std::string(enumUnknownWHEREAUs[i]) == std::string(" au = 'BIOBANK'"))
			au = "BIOBANK";
		else if (std::string(enumUnknownWHEREAUs[i]) == std::string(" au = 'BIOBANK_QA'"))
			au = "BIOBANK_QA";
		else if (std::string(enumUnknownWHEREAUs[i]) == std::string(" au = 'BIOBANK_PSI'"))
			au = "BIOBANK_PSI";
		else if (std::string(enumUnknownWHEREAUs[i]) == std::string(" au = 'REPORTED'"))
			au = "REPORTED";
		else if (std::string(enumUnknownWHEREAUs[i]) == std::string(""))
			au = "TOTAL";
		else
			au = "UNKNOWN";
		runGetTotalSQL(db,sql,au,R);
	}
}
//---------------------------------------------------------------------------

const char* dicom::enumUnknownWHEREAUs[] = {" au = 'BIOBANK'"," au = 'BIOBANK_QA'"," au = 'BIOBANK_PSI'"," au = 'REPORTED'",""};
void dicom::getUnknownStats(const XDB *db,ROSETTA &R) const
{
	for (int i=0;i<5;i++)
	{
		std::string sqlquery = "select count(*) AS count from pacsstudy where confirmedpatientid = ''";
		if (!std::string(enumUnknownWHEREAUs[i]).empty())
			sqlquery += " AND " + std::string(enumUnknownWHEREAUs[i]);

		XQUERY qp((XDB *) db, sqlquery);
		if( qp.open( ) )
		{
			while( qp.fetch( ) )
			{
				std::stringstream patientid;
				patientid << qp.result.getInt("count");
				std::string au = "";

				if (std::string(enumUnknownWHEREAUs[i]) == std::string(" au = 'BIOBANK'"))
					au = "BIOBANK";
				else if (std::string(enumUnknownWHEREAUs[i]) == std::string(" au = 'BIOBANK_QA'"))
					au = "BIOBANK_QA";
				else if (std::string(enumUnknownWHEREAUs[i]) == std::string(" au = 'BIOBANK_PSI'"))
					au = "BIOBANK_PSI";
				else if (std::string(enumUnknownWHEREAUs[i]) == std::string(" au = 'REPORTED'"))
					au = "REPORTED";
				else if (std::string(enumUnknownWHEREAUs[i]) == std::string(""))
					au = "TOTAL";
				else
					au = "UNKNOWN";

				R.setString(au, patientid.str());
			}
			qp.close( );
		}
	}
}
//---------------------------------------------------------------------------

void dicom::parse(const XCGI *cgi,const ROSETTA &R)
{
	readDetails(R);
	m_dicompage.setText(cgi->setParamIfExistsElseDefault("dicompage",m_dicompage.getText()));

	m_statusFilter.setText(cgi->setParamIfExistsElseDefault("statusFilter",m_statusFilter.getText()));
	m_CTSUFilter.setText(cgi->setParamIfExistsElseDefault("CTSUFilter",m_CTSUFilter.getText()));
	m_reportsFilter.setText(cgi->setParamIfExistsElseDefault("reportsFilter",m_reportsFilter.getText()));
	m_alertsFilter.setText(cgi->setParamIfExistsElseDefault("alertsFilter",m_alertsFilter.getText()));
	m_timespanFilter.setText(cgi->setParamIfExistsElseDefault("timespanFilter",m_timespanFilter.getText()));
	m_statusPatientIDfilter.setText(cgi->setParamIfExistsElseDefault("statusPatientIDfilter",m_statusPatientIDfilter.getText()));

//if user has click to change the dicom table, next or previous
	std::string dicomchange = cgi->setParamIfExistsElseDefault("dicomchange","");

	if (m_dicompage.getText() == "")
		m_dicompage.setText("0");

	if (dicomchange == "Next")
		m_dicompage.isetText(m_dicompage.igetText()+1);
	else if (dicomchange == "Prev")
		m_dicompage.isetText(m_dicompage.igetText()-1);

	if (cgi->setParamIfExistsElseDefault("statusRefresh","") == "Refresh")
		m_dicompage.isetText(0);
}
//---------------------------------------------------------------------------

const dicom *dicom::find(const std::string &patiendid, const XTIME &date ) const
{
/*
	for( std::vector<dicom >::const_iterator ci = m_dicomStudies.begin( ); ci != m_dicomStudies.end( ) ; ++ ci )
	{
		if ((ci->m_patiendid == patiendid ) && (ci->m_studydate == date))
			return &(*ci);
	}
*/	return NULL;
}
//---------------------------------------------------------------------------

void dicom::readList(const XDB *db )
{
	m_ready = true;
/*	m_dicomStudies.clear( );

	std::string sqlquery = "SELECT dicompacs.patientid, dicompacs.studydate, dicompacs.radiographerreport, dicompacs.findingsstatus, dicompacs.alertflag, dicompacs.pacsurl, IFNULL(dicombiobank.zipmd5,'') AS zipmd5";
	sqlquery += " FROM dicompacs";
	sqlquery += " LEFT JOIN dicombiobank";
	sqlquery += " ON dicompacs.patientid=dicombiobank.patientid AND dicompacs.studydate = dicombiobank.studydate ";
	sqlquery += " WHERE dicompacs.status = 1 ";

	if (m_alertsFilter.getText() == "1")
		sqlquery += " AND dicompacs.alertflag = 1";
	else if (m_alertsFilter.getText() == "2")
		sqlquery += " AND dicompacs.alertflag = 0";

	if (m_reportsFilter.getText() == "1")
		sqlquery += " AND LENGTH(dicompacs.radiographerreport) > 0";
	else if (m_reportsFilter.getText() == "2")
		sqlquery += " AND LENGTH(dicompacs.radiographerreport) = 0";

	if (m_statusFilter.igetText() != -1)
		sqlquery += " AND dicompacs.findingsstatus = " + m_statusFilter.getText();

	if (m_CTSUFilter.getText() == "1")
		sqlquery += " AND LENGTH(IFNULL(dicombiobank.zipmd5,'')) > 0";
	else if (m_CTSUFilter.getText() == "2")
		sqlquery += " AND LENGTH(IFNULL(dicombiobank.zipmd5,'')) = 0";

	if (m_timespanFilter.getText().empty())
		m_timespanFilter.isetText(1);
//Stdu date.
	sqlquery += " AND dicompacs.studydate > date('now') - '" + m_timespanFilter.getText() + " months'";

	if (m_statusPatientIDfilter.getText().length() > 0)
	{
		sqlquery += " AND ";
		sqlquery += " dicompacs.patientid LIKE '" + m_statusPatientIDfilter.getText() + "'";
	}
	sqlquery += " ORDER BY dicompacs.studydate DESC";

	XQUERY qp((XDB *) db, sqlquery);
	if( qp.open( ) )
	{
		while( qp.fetch( ) )
		{
			std::string patiendid = qp.result.getString(0);
			XTIME studydate = qp.result.getTime(1);
			std::string report = qp.result.getString(2);
			int findingsStatus = qp.result.getInt(3);
			int alert = qp.result.getInt(4);
			std::string pacsurl = qp.result.getString(5);
			std::string md5zip = qp.result.getString(6);
			m_dicomStudies.push_back(dicom(patiendid,studydate,report,findingsStatus,alert,pacsurl,md5zip));
		}
		qp.close( );
	}
	if( m_dicomStudies.empty( ) )
		m_error = "Cannot read staff table";
*/
}
//---------------------------------------------------------------------------

void dicom::readDetails(const ROSETTA & fields )
{
	m_dicompage.setText(fields.getStringDefault("dicompage" ,m_dicompage.getText()));

	m_statusFilter.setText(fields.getStringDefault("statusFilter",m_statusFilter.getText()));
	m_CTSUFilter.setText(fields.getStringDefault("CTSUFilter",m_CTSUFilter.getText()));
	m_reportsFilter.setText(fields.getStringDefault("reportsFilter",m_reportsFilter.getText()));
	m_alertsFilter.setText(fields.getStringDefault("alertsFilter",m_alertsFilter.getText()));
	m_timespanFilter.setText(fields.getStringDefault("timespanFilter",m_timespanFilter.getText()));
	m_statusPatientIDfilter.setText(fields.getStringDefault("statusPatientIDfilter",m_statusPatientIDfilter.getText()));
}
//---------------------------------------------------------------------------

void dicom::addFields( ROSETTA & fields ) const
{
	if (!m_dicompage.getText().empty())
		fields.setString("dicompage",m_dicompage.getText());
	if (!m_statusFilter.getText().empty())
		fields.setString("statusFilter",m_statusFilter.getText());
	if (!m_CTSUFilter.getText().empty())
		fields.setString("CTSUFilter",m_CTSUFilter.getText());
	if (!m_reportsFilter.getText().empty())
		fields.setString("reportsFilter",m_reportsFilter.getText());
	if (!m_alertsFilter.getText().empty())
		fields.setString("alertsFilter",m_alertsFilter.getText());
	if (!m_timespanFilter.getText().empty())
		fields.setString("timespanFilter",m_timespanFilter.getText());
	if (!m_statusPatientIDfilter.getText().empty())
		fields.setString("statusPatientIDfilter",m_statusPatientIDfilter.getText());
}
//---------------------------------------------------------------------------



