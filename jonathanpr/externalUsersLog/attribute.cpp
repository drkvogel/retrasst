//---------------------------------------------------------------------------

#pragma hdrstop

#include "attribute.h"
#include <sstream>
#include "utils.h"
#include "xexec.h"
#include <map>
//---------------------------------------------------------------------------
#pragma package(smart_init)

attribute::attribute()
{
}
//---------------------------------------------------------------------------

attribute::~attribute()
{
}
//---------------------------------------------------------------------------

void attribute::save(XDB *db,int UID) const
{
	XEXEC exec(db);
	ROSETTA R;
	R.setInt("ru_user_id",UID);

	exec.del("user_attribs",&R);

	std::map<std::string,bool>::const_iterator it=m_Attribs.begin();
	for (;it!=m_Attribs.end();it++)
	{
		if (it->second)
		{
			std::map<std::string,int>::iterator  found = m_avalibleAttribs.find(it->first);
			if (found != m_avalibleAttribs.end())
			{
				R.setInt("attribs_id",found->second);
				if (!exec.insert("user_attribs",&R))
					throw exec.getLastError();
			}
		}
	}
}
//---------------------------------------------------------------------------

void attribute::load(XDB *db,int UID)
{
	ROSETTA output;
	std::stringstream sql;
	sql << "SELECT attribs_id FROM user_attribs WHERE ru_user_id = '" << UID << "'";
	XQUERY query(db,sql.str());

	if (!query.open())
		throw String(query.getLastError().c_str());
	while (query.fetch(&output))
	{
		long attrib_id = output.getLint("attribs_id");
		setAttrib(attrib_id,true);
	}
	query.close();
}
//---------------------------------------------------------------------------

const String attribute::getAttribString() const
{
	String result;
	std::map<std::string,bool>::const_iterator it;
	it=m_Attribs.begin();
	for (;it!=m_Attribs.end();it++)
	{
		if (it->second)
		{
			if (!result.IsEmpty())
				result += " ";
			result += "[" + String(it->first.c_str()) + "]";
		}
	}
	return result;
}
//---------------------------------------------------------------------------

const std::string attribute::seriliseAttrib() const
{
	std::stringstream result;
	std::map<std::string,bool>::const_iterator it;
	it=m_Attribs.begin();
	for (;it!=m_Attribs.end();it++)
	{
		if (it->second)
		{
			if (!result.str().empty())
				result << "#";
			result << it->first;
		}
	}
	return result.str();
}
//---------------------------------------------------------------------------

void attribute::deseriliseAttrib(const std::string &attribs)
{
	stringsplit split(attribs, "#");
	std::vector<std::string>::const_iterator it;
	it=split.begin();
	for (;it!=split.end();it++)
	{
		setAttrib((*it),true);
	}
}
//---------------------------------------------------------------------------

void attribute::clearAttib()
{
	m_Attribs.clear();
}
//---------------------------------------------------------------------------

void attribute::setAttrib(std::string name,bool set)
{
	if (m_Attribs.count(name) > 0)
		m_Attribs.erase(name);
	if (set)
		m_Attribs.insert(std::pair<std::string,bool>(name,set));
}
//---------------------------------------------------------------------------

void attribute::setAttrib(int id,bool set)
{
	std::string attrribName = "";
	std::map<std::string,int>::iterator it;
	for (it=m_avalibleAttribs.begin();it!= m_avalibleAttribs.end();it++)
	{
		if (it->second == id)
		{
			attrribName = it->first;
			break;
		}
	}

	if (attrribName != "")
	{
		if (m_Attribs.count(attrribName) > 0)
			m_Attribs.erase(attrribName);
		if (set)
			m_Attribs.insert(std::pair<std::string,bool>(attrribName,set));
	}
}
//---------------------------------------------------------------------------


void attribute::toggleAttrib(std::string name)
{
	setAttrib(name,!getAttrib(name));
}
//---------------------------------------------------------------------------

bool attribute::getAttrib(std::string name)
{
	if (m_Attribs.count(name) == 0)
		return false;
	std::map<std::string,bool>::iterator it;
	it=m_Attribs.find(name);
	return it->second;
}
//---------------------------------------------------------------------------

std::map<std::string,int> attribute::m_avalibleAttribs;
void attribute::initalise(XDB *db)
{
	std::string sql = "SELECT id,text FROM attribs";
	XQUERY query(db,sql);
	if (!query.open())
		throw String(query.getLastError().c_str());
	while (query.fetch())    //any data?
	{
		int id = query.result.getLint(0);
		std::string text = query.result.getString(1);
		m_avalibleAttribs.insert( std::pair<std::string,int>(text,id));
	}
	query.close();
}

