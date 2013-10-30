#ifndef __database__HPP_
#define __database__HPP_

#include "xdb.h"
#include "xquery.h"

#include <vector>

void onError( const std::string& msg );
bool dbErrorCallback( const std::string object, const int instance,const int ecount, const int ecode, const std::string error_txt );
class database
{
public:
	database();
	~database();

	void connect(String &password);
//add to event system
	void addMember(String &name);
	void addEvent(String &name);
	void addMemberToEvent(String &event, String &member);
//query event system
	void getEvents(std::vector<String> &eventslist);
	void getMembers(std::vector<String> &memberslist);
	void getMembersForEvent(String &currentEvent,std::vector<String> &memberlist);
 	void getMembersNoInEvent(String &currentEvent,std::vector<String> &memberlist);
 //remove
	void removeMemberFromEvent(String &event, String &member);

private:
	std::auto_ptr<XDB> m_db;

	void throwUnless( bool condition, const String& msg );
};

#endif
