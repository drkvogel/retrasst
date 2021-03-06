//---------------------------------------------------------------------------
#pragma hdrstop

#include "log.h"
#include "xcgi.h"
#include "xencode.h"
#include "randutil.h"
#include "xquery.h"
#include "xexec.h"
#include <sstream>
//---------------------------------------------------------------------------
#pragma package(smart_init)

std::vector<PACSlog> PACSlog::m_logs;
//---------------------------------------------------------------------------

PACSlog::PACSlog(int id, int loglevel,const XTIME &timestamp,int logtype,const std::string &message, const std::string &patientid,const XTIME &studydate)
{
	m_id = id;
	m_loglevel = loglevel;
	m_timestamp = timestamp;
	m_logtype = logtype;
	m_message = message;
	m_patientid = patientid;
	m_studydate = studydate;
}
//---------------------------------------------------------------------------

PACSlog::PACSlog()
{
	m_ready = false;
}
//---------------------------------------------------------------------------
//parse the URL string for anything usful
void PACSlog::parse(const XCGI *cgi,const ROSETTA &R)
{
//load in anything which was passed in with the decrpyted ROSETTA
	readDetails(R);
//Page state
//find which page our table on the page is currently on.
	m_logpage.setText(cgi->setParamIfExistsElseDefault("logpage",m_logpage.getText()));
//Get the Filters
	m_logLevelFilter.setText(cgi->setParamIfExistsElseDefault("logLevelFilter",m_logLevelFilter.getText()));
	m_logTypeFilter.setText(cgi->setParamIfExistsElseDefault("logTypeFilter",m_logTypeFilter.getText()));
	m_logTimespanFilter.setText(cgi->setParamIfExistsElseDefault("logtimespanFilter",m_logTimespanFilter.getText()));
	m_logPatientIDfilter.setText(cgi->setParamIfExistsElseDefault("logPatientIDfilter",m_logPatientIDfilter.getText()));

//if user has click to move to another page on the page's table, next or previous
	std::string dicomchange = cgi->setParamIfExistsElseDefault("logchange","");

	if (m_logpage.getText() == "")
		m_logpage.setText("0");
//Has user pressed Next or Prev page on the page's table
	if (dicomchange == "Next")
		m_logpage.isetText(m_logpage.igetText()+1);
	else if (dicomchange == "Prev")
		m_logpage.isetText(m_logpage.igetText()-1);
//has the user pressed the Refresh button
	if (cgi->setParamIfExistsElseDefault("logRefresh","") == "Refresh")
		m_logpage.isetText(0);
}

//---------------------------------------------------------------------------
//Generate the SQL query to pull the data from the database, it takes into account the
//filter options.
std::string PACSlog::generateQuery(bool justCount, int fromID, int maxCount )
{
	std::stringstream sqlquery;
	if (justCount)
		sqlquery << "SELECT count(*)";
	else
		sqlquery << "SELECT First " << maxCount << " id,inserttimestamp,loglevel,logtype,message,patientid,studydate";

	sqlquery << " FROM logs  WHERE ";
	std::stringstream wheresql;

	if (m_logLevelFilter.igetText() >= 0)
		wheresql << " loglevel = " << m_logLevelFilter.getText();

	if (m_logTypeFilter.getText() >= "0")
	{
		if (wheresql.str().length() > 0)
			wheresql << " AND ";
		wheresql << " logtype = " << m_logTypeFilter.getText();
	}

	if (m_logTimespanFilter.getText().empty())
		m_logTimespanFilter.isetText(1);
//Study date.
	if (wheresql.str().length() > 0)
		wheresql << " AND ";
	wheresql << " inserttimestamp > date('now') - '" << m_logTimespanFilter.getText() << " months'";

	if (!justCount)
	{
		if (wheresql.str().length() > 0)
			wheresql << " AND ";
		wheresql << " id >= " << fromID;

		if (m_logPatientIDfilter.getText().length() > 0)
		{
			wheresql << " AND ";
			wheresql << " patientid LIKE '" << m_logPatientIDfilter.getText() << "'";
		}
	}
	sqlquery << wheresql.str();
	sqlquery << " ORDER BY inserttimestamp DESC";
	return sqlquery.str();
}
//---------------------------------------------------------------------------
//get the total count in the database, using the filters, but no max count like in the SQL above, so we know
//how many pages in total there is of the data
void PACSlog::featchTotal( XDB *db, int fromID, int maxCount )
{
	m_rowCount = -1;
	std::string sqlquery = generateQuery(true, 0, 0);

	XQUERY qp( db, sqlquery);
	if( qp.open( ) )
	{
		if( qp.fetch( ) )
			m_rowCount = qp.result.getInt(0);
		qp.close( );
	}
}
//---------------------------------------------------------------------------
//Calls the SQL generated in generateQuery(), and feeds it into it's data structure
void PACSlog::readList(const XDB *db)
{
	m_ready = true;
	int fromID = 0; // page number * max per page
	int maxCount = 20; //max per page

	m_logs.clear( );
	std::string sqlquery = generateQuery(false, fromID, maxCount);

	XQUERY qp((XDB *) db, sqlquery);
	if( qp.open( ) )
	{
		while( qp.fetch( ) )
		{
			int id = qp.result.getInt(0);
			XTIME timestamp = qp.result.getTime(1);
			int loglevel = qp.result.getInt(2);
			int logtype = qp.result.getInt(3);
			std::string message = qp.result.getString(4);
			std::string patientid = qp.result.getString(5);
			XTIME studydate = qp.result.getTime(6);

			m_logs.push_back(PACSlog(id,loglevel,timestamp,logtype,message,patientid,studydate));
		}
		qp.close( );
	}
}
//---------------------------------------------------------------------------
//Extract any usful information from the ROSETTA
void PACSlog::readDetails(const ROSETTA & fields )
{
	m_logpage.setText(fields.getStringDefault("logpage" ,m_logpage.getText()));

	m_logLevelFilter.setText(fields.getStringDefault("logLevelFilter",m_logLevelFilter.getText()));
	m_logTypeFilter.setText(fields.getStringDefault("logTypeFilter",m_logTypeFilter.getText()));
	m_logTimespanFilter.setText(fields.getStringDefault("logtimespanFilter",m_logTimespanFilter.getText()));
	m_logPatientIDfilter.setText(fields.getStringDefault("logPatientIDfilter",m_logPatientIDfilter.getText()));
}
//---------------------------------------------------------------------------
//Add any information into the ROSETTA it wants to preserve between pages.
void PACSlog::addFields( ROSETTA & fields ) const
{
	if (!m_logpage.getText().empty())
		fields.setString("logpage",m_logpage.getText());

	if (!m_logLevelFilter.getText().empty())
		fields.setString("logLevelFilter",m_logLevelFilter.getText());
	if (!m_logTypeFilter.getText().empty())
		fields.setString("logTypeFilter",m_logTypeFilter.getText());
	if (!m_logTimespanFilter.getText().empty())
		fields.setString("logtimespanFilter",m_logTimespanFilter.getText());
	if (!m_logPatientIDfilter.getText().empty())
		fields.setString("logPatientIDfilter",m_logPatientIDfilter.getText());
}
//---------------------------------------------------------------------------



