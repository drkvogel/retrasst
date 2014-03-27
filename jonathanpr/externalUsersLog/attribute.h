//---------------------------------------------------------------------------

#ifndef attributeH
#define attributeH
#include <vcl.h>
#include <map>
#include <string>
#include "xexec.h"
//---------------------------------------------------------------------------
class attribute
{
public:
	attribute();
	~attribute();

	static void initalise(XDB *db);
	static std::map<std::string ,int> m_avalibleAttribs;

	void load(XDB *db,int UID);
	void save(XDB *db,int UID) const;

	void clearAttib();
	void setAttrib(std::string name,bool set);
	void setAttrib(int id,bool set);
	void toggleAttrib(std::string name);
	bool getAttrib(std::string name);
	const String getAttribString() const;
	const std::string seriliseAttrib() const;
	void deseriliseAttrib(const std::string &attribs);

private:
	std::map<std::string,bool> m_Attribs;
};
#endif
