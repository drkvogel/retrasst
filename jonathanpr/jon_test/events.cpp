#include "events.h"

events::events()
{

}

events::~events()
{

}

void events::initalise(String &password)
{
	try
	{
		m_database.connect(password);
	}
	catch( const String& msg )
	{
		throw msg;
	}
}

void events::getEvents(std::vector<String> &eventslist)
{
	m_database.getEvents(eventslist);
}

void events::getMembers(std::vector<String> &memberslist)
{
	m_database.getMembers(memberslist);
}

void events::getMembersForEvent(String &currentEvent,std::vector<String> &memberlist)
{
	m_database.getMembersForEvent(currentEvent,memberlist);
}

void events::getMembersNoInEvent(String &currentEvent,std::vector<String> &memberlist)
{
	m_database.getMembersNoInEvent(currentEvent,memberlist);
}


void events::addMember(String &NewMember)
{
	m_database.addMember(NewMember);
}

void events::addEvent(String &NewEvent)
{
	m_database.addEvent(NewEvent);
}

void events::addMemberToEvent(String &event, String &member)
{
	m_database.addMemberToEvent(event,member);
}

void events::removeMemberFromEvent(String &event, String &member)
{
	m_database.removeMemberFromEvent(event,member);
}

void events::test(TApplication *app)
{
	try
	{
		std::vector<String> memberlist;

		String s = "computer show";
		m_database.getMembersForEvent(s,memberlist);



		String member = "tony6";
		String event = "computer show6";
		m_database.addMember(member);
 		m_database.addEvent(event);
		m_database.addMemberToEvent(event,member);

	}
	catch( const String& msg )
	{
		String displayString = String("Failed:\n") + msg;
		app->MessageBox(UnicodeString(displayString.c_str()).c_str(), L"User Error", MB_OK);
	}
}
