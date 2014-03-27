//---------------------------------------------------------------------------

#ifndef correspondenceH
#define correspondenceH
//---------------------------------------------------------------------------
#include <string>
#include "ROSETTA.h"
#include "xexec.h"

class correspondence
{
public:
	correspondence();
	~correspondence();

	bool serialize(const std::string &data);

	bool updateCorrespondence(String &ID, String &&summery,String &&text);
	void addCorrespondence(String &&summery, String &&text);

	int getFirst();
	int getNext(int current);

	std::string getCorrespondenceText(int i);
	std::string getCorrespondenceText(std::string &id);
	bool deleteCorrespondenceID(std::string &&id);
	std::string getCorrespondenceSummary(int i);
	std::string getCorrespondenceSummary(std::string &id);
	std::string getCorrespondenceDate(int i);
	std::string getCorrespondenceID(int i);

	void save(XDB *db,int applicationID) const;
	void update(XDB *db,int applicationID) const;
	void load(XDB *db,int applicationID);
	void clear();
private:
	ROSETTA m_correspondence;
};
#endif
