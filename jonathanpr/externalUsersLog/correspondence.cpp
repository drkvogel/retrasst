//---------------------------------------------------------------------------

#pragma hdrstop

#include "correspondence.h"
#include <sstream>
#include "xquery.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


//---------------------------------------------------------------------------
correspondence::correspondence()
{
}
//---------------------------------------------------------------------------

correspondence::~correspondence()
{
}
//---------------------------------------------------------------------------

bool correspondence::serialize(const std::string &data)
{
	if (!data.empty())
		return m_correspondence.serializeIn(data,true);
	return false;
}
//---------------------------------------------------------------------------

void correspondence::save(XDB *db,int applicationID) const
{
	std::string serlised = m_correspondence.serializeOut();

	ROSETTA R;
	R.setInt("app_id",applicationID);
	R.setString("rosetta",serlised);
	XEXEC exec(db);
	if (!exec.insert("correspondence",&R))
		throw exec.getLastError();
}
//---------------------------------------------------------------------------

void correspondence::update(XDB *db,int applicationID) const
{
	XEXEC exec(db);
	std::string serlised = m_correspondence.serializeOut();

	ROSETTA R;
	R.setString("rosetta",serlised);
	ROSETTA RWhere;
	RWhere.setInt("app_id",applicationID);
	if (!exec.update("correspondence",&R,&RWhere))
		throw exec.getLastError();
	if (exec.getNRowsAltered() == 0)
		save(db,applicationID);
}
//---------------------------------------------------------------------------

void correspondence::load(XDB *db,int applicationID)
{
	ROSETTA output;
	std::stringstream sql;
	sql << "SELECT rosetta FROM correspondence WHERE app_id = '" << applicationID << "'";
	XQUERY query(db,sql.str());

	if (!query.open())
		throw query.getLastError();
	if (query.fetch(&output))
	{
		std::string Roset = output.getString("rosetta");
		m_correspondence.serializeIn(Roset);
	}
	query.close();
}
//---------------------------------------------------------------------------

void correspondence::clear()
{
	m_correspondence.clear();
}
//---------------------------------------------------------------------------

bool correspondence::updateCorrespondence(String &ID, String &&summary,String &&text)
{
	std::string stdid = AnsiString(ID.c_str()).c_str();
	std::string stdtext = AnsiString(text.c_str()).c_str();
	std::string stdsummary = AnsiString(summary.c_str()).c_str();

	ROSETTA *R = m_correspondence.getRosetta(stdid);

	if (!R)
		return false;

	XTIME D;
	D.setNow();
	R->setDate("timestamp",D);
	R->setString("text",stdtext);
	R->setString("summary",stdsummary);
	m_correspondence.setRosetta(stdid,R);
	return true;
}
//---------------------------------------------------------------------------

void correspondence::addCorrespondence(String &&summary, String &&text)
{
	std::string stdtext = AnsiString(text.c_str()).c_str();
	std::string stdsummary = AnsiString(summary.c_str()).c_str();
	XTIME D;
	D.setNow();
	ROSETTA R;
	R.setDate("timestamp",D);
	R.setString("text",stdtext);
	R.setInt("status",1);
	R.setString("summary",stdsummary);

	std::stringstream ss;
	ss << D.getYear() << D.getMonth() << D.getDay() << D.getHour() << D.getMinute() << D.getSecond() << D.getMilliSecond();
//save each against the timestamp.
	m_correspondence.setRosetta(ss.str(),&R);
}
//---------------------------------------------------------------------------

int correspondence::getFirst()
{
	try
	{
		for (int i=0;i<m_correspondence.count();i++)
		{
			if (m_correspondence.getRosetta(i)->getInt("status") == 1)
				return i;
		}
		return -1;
	}
	catch (...)
	{
		return -1;
	}
}
//---------------------------------------------------------------------------

int correspondence::getNext(int current)
{
	try
	{
		int next = current +1; //move to next one
		int count = m_correspondence.count();
		for (;next<m_correspondence.count();next++)
		{
			if (m_correspondence.getRosetta(next)->getInt("status") == 1)
				return next;
		}
		return -1;
	}
	catch (...)
	{
		return -1;
	}
}
//---------------------------------------------------------------------------

std::string correspondence::getCorrespondenceText(int i)
{
	try
	{
		if (i+1 > m_correspondence.count())
			return "";
		return m_correspondence.getRosetta(i)->getString("text");
	}
	catch (...)
	{
		return "";
	}
}
//---------------------------------------------------------------------------

std::string correspondence::getCorrespondenceText(std::string &id)
{
	try
	{
		for (int i=0;i<m_correspondence.count();i++)
		{
			if (getCorrespondenceID(i) == id)
				return m_correspondence.getRosetta(i)->getString("text");
		}
		return "";
	}
	catch (...)
	{
		return "";
	}
}
//---------------------------------------------------------------------------

bool correspondence::deleteCorrespondenceID(std::string &&id)
{
	try
	{
		for (int i=0;i<m_correspondence.count();i++)
		{
			if (getCorrespondenceID(i) == id)
			{
				ROSETTA *R = m_correspondence.getRosetta(i);

				if (!R)
					assert(true);

				R->setInt("status",0);
				m_correspondence.setRosetta(id,R);
				return true;
			}
		}
		return false;
	}
	catch (...)
	{
		return false;
	}
}
//---------------------------------------------------------------------------

std::string correspondence::getCorrespondenceSummary(int i)
{
	try
	{
		if (i+1 > m_correspondence.count())
			return "";
		return m_correspondence.getRosetta(i)->getString("summary");
	}
	catch (...)
	{
		return "";
	}
}
//---------------------------------------------------------------------------

std::string correspondence::getCorrespondenceSummary(std::string &id)
{
	try
	{
		for (int i=0;i<m_correspondence.count();i++)
		{
			if (getCorrespondenceID(i) == id)
				return m_correspondence.getRosetta(i)->getString("summary");
		}
		return "";
	}
	catch (...)
	{
		return "";
	}
}
//---------------------------------------------------------------------------

std::string correspondence::getCorrespondenceDate(int i)
{
	try
	{
		if (i+1 > m_correspondence.count())
			return "";
		XTIME T = m_correspondence.getRosetta(i)->getDate("timestamp");

		std::stringstream ss;
		ss << T.getDay() << " " << T.getMonthName() << " " << T.getYear();
		return ss.str();
	}
	catch (...)
	{
		return "";
	}
}
//---------------------------------------------------------------------------

std::string correspondence::getCorrespondenceID(int i)
{
	try
	{
		if (i+1 > m_correspondence.count())
			return "";
		std::string id = m_correspondence.getName(i);
		return id;
	}
	catch (...)
	{
		return "";
	}
}

//---------------------------------------------------------------------------