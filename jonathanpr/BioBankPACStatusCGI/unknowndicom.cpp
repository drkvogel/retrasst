//---------------------------------------------------------------------------
#pragma hdrstop

#include "unknowndicom.h"
#include "xcgi.h"
#include "xencode.h"
#include "randutil.h"
#include "xquery.h"
#include "xexec.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

std::vector<unknowndicom> unknowndicom::m_unknowndicom;
std::set<std::string > unknowndicom::m_aetList;

unknowndicom::unknowndicom(long studypk,const std::string &aet,const XTIME &studydate,const std::string &patientid,const std::string &firstname)
{
	m_studypk = studypk;
	m_aet = aet;
	m_studydate = studydate;
	m_patientid = patientid;
	m_firstname = firstname;
}
//---------------------------------------------------------------------------

unknowndicom::unknowndicom() : m_stage(UNKNOWN)
{
	m_ready = false;
}
//---------------------------------------------------------------------------

void unknowndicom::parse(const XCGI *cgi,const ROSETTA &R)
{
	readDetails(R);
	m_unknownpage.setText(cgi->setParamIfExistsElseDefault("unknownpage",m_unknownpage.getText()));

	m_aetFilter.setText(cgi->setParamIfExistsElseDefault("unknownAetFilter",m_aetFilter.getText()));
	m_timespanFilter.setText(cgi->setParamIfExistsElseDefault("unknownTimeSpanFilter",m_timespanFilter.getText()));

//if user has click to change the dicom table, next or previous
	std::string dicomchange = cgi->setParamIfExistsElseDefault("unknownchange","");

	if (m_unknownpage.getText() == "")
		m_unknownpage.setText("0");

	if (dicomchange == "Next")
		m_unknownpage.isetText(m_unknownpage.igetText()+1);
	else if (dicomchange == "Prev")
		m_unknownpage.isetText(m_unknownpage.igetText()-1);

	if (cgi->setParamIfExistsElseDefault("statusRefresh","") == "Refresh")
		m_unknownpage.isetText(0);
}
//---------------------------------------------------------------------------

const unknowndicom *unknowndicom::find(const int studypk) const
{
	for( std::vector<unknowndicom >::const_iterator ci = m_unknowndicom.begin( ); ci != m_unknowndicom.end( ) ; ++ ci )
	{
		if (ci->m_studypk == studypk)
			return &(*ci);
	}
	return NULL;
}
//---------------------------------------------------------------------------
void unknowndicom::getAETs(const XDB *db )
{
	m_aetList.clear();

	std::string sqlquery = "select DISTINCT aet from pacsstudy where confirmedpatientid = '' and status = 1";

	if (m_timespanFilter.getText().empty())
		m_timespanFilter.isetText(2);
//Study date.
	sqlquery += " AND studydate > date('now') - '" + m_timespanFilter.getText() + " months'";

	XQUERY qp((XDB *) db, sqlquery);
	if( qp.open( ) )
	{
		while( qp.fetch( ) )
		{
			std::string aet = qp.result.getString(0);
			m_aetList.insert(aet);
		}
		qp.close( );
	}
	if( m_aetList.empty( ) )
		m_error = "Cannot read table";
}

//---------------------------------------------------------------------------

void unknowndicom::readList(const XDB *db )
{
	m_ready = true;
	getAETs(db);
	m_unknowndicom.clear( );

	std::string sqlquery = "select studypk,aet,studydate,patientid,firstname from pacsstudy where confirmedpatientid = '' and status = 1";

	if (m_aetFilter.getText().length() != 0)
		sqlquery += " AND aet = '" + m_aetFilter.getText() + "'";

	if (m_timespanFilter.getText().empty())
		m_timespanFilter.isetText(2);
//Study date.
	sqlquery += " AND studydate > date('now') - '" + m_timespanFilter.getText() + " months'";
	sqlquery += " order by studydate DESC";

	XQUERY qp((XDB *) db, sqlquery);
	if( qp.open( ) )
	{
		while( qp.fetch( ) )
		{
			long studypk= qp.result.getLint(0);
			std::string aet = qp.result.getString(1);
			XTIME studydate = qp.result.getTime(2);
			std::string patientid = qp.result.getString(3);
			std::string firstname = qp.result.getString(4);
			m_unknowndicom.push_back(unknowndicom(studypk,aet,studydate,patientid,firstname));
		}
		qp.close( );
	}
	if( m_unknowndicom.empty( ) )
		m_error = "Cannot read table";
}
//---------------------------------------------------------------------------

void unknowndicom::readDetails(const ROSETTA & fields )
{
	m_unknownpage.setText(fields.getStringDefault("unknownpage" ,m_unknownpage.getText()));
	m_aetFilter.setText(fields.getStringDefault("unknownAetFilter",m_aetFilter.getText()));
	m_timespanFilter.setText(fields.getStringDefault("unknownTimeSpanFilter",m_timespanFilter.getText()));
}
//---------------------------------------------------------------------------

void unknowndicom::addFields( ROSETTA & fields ) const
{
	if (!m_unknownpage.getText().empty())
		fields.setString("unknownpage",m_unknownpage.getText());
	if (!m_aetFilter.getText().empty())
		fields.setString("unknownAetFilter",m_aetFilter.getText());
	if (!m_timespanFilter.getText().empty())
		fields.setString("unknownTimeSpanFilter",m_timespanFilter.getText());
}
//---------------------------------------------------------------------------



