#ifndef __events__HPP_
#define __events__HPP_

#include <Vcl.Forms.hpp>
#include "database.h"


class events
{
public:
	events();
	~events();

    bool hasPassword() {return !m_password.empty();}

	void initalise(String &password);
	void getEvents(std::vector<String> &eventslist);
	void getMembers(std::vector<String> &eventslist);
	void getMembersForEvent(String &currentEvent,std::vector<String> &memberlist);
	void getMembersNoInEvent(String &currentEvent,std::vector<String> &memberlist);

	void removeMemberFromEvent(String &event, String &member);

	void addMemberToEvent(String &event, String &member);
	void addMember(String &newMember);
	void addEvent(String &newEvent);
	void test(TApplication *app);
private:
	database m_database;
	std::string m_password;
};

#endif
