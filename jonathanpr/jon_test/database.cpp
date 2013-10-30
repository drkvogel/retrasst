#include "database.h"
#include "xdb.h"
#include "xquery.h"
#include "xexec.h"
#include <sstream>

database::database()
{
	m_db = std::auto_ptr<XDB>( new XDB( "@brat.ctsu.ox.ac.uk,wintcp,II::jonathan_test" ) );
}

database::~database()
{

}

void database::connect(String &password)
{
	try
	{
		m_db->setErrorCallBack( dbErrorCallback );
		std::string username("cp");
		m_db->setUserName( username );
		m_db->setPassWord( AnsiString(password).c_str() );
		throwUnless ( m_db->open(), "Failed to connect!" );
	}
	catch( const std::string& msg )
	{
		onError( msg );
	}
	catch( const std::runtime_error& e )
	{
		onError( e.what() );
	}
}

void database::addMember(String &name)
{
	std::wstringstream sqlquery;
	sqlquery << "insert into members (name) values ('" << name.c_str() << "')";
	XEXEC query( m_db.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.exec(), "Failed to exec query" );
}

void database::addEvent(String &name)
{
	std::wstringstream sqlquery;
	sqlquery << "insert into events (name) values ('" << name.c_str() << "')";
	XEXEC query( m_db.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.exec(), "Failed to exec query" );
}

void database::removeMemberFromEvent(String &event, String &member)
{
	std::wstringstream sqlExec;
	sqlExec << "delete from attendance where event = '"  << event.c_str() << "' and member = '"  << member.c_str() << "'";
	XEXEC execquery( m_db.get(), AnsiString(sqlExec.str().c_str()).c_str() );
	throwUnless( execquery.exec(), "Failed to exec query" );
}

 void database::addMemberToEvent(String &event, String &member)
{
	// check member isn't already in event
	std::wstringstream sqlquery;
	sqlquery << "select count(*) from attendance WHERE event = '" << event.c_str() << "' AND member = '" <<  member.c_str() << "'";

	XQUERY query( m_db.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to open query" );
	bool bAlreadyAttending = false;
	if ( query.fetch() )    //any data?
	{
		bAlreadyAttending = query.result.getInt(0) > 0;
	}
	query.close();
	throwUnless(!bAlreadyAttending, "Member already attending");
	//add member to event
	std::wstringstream sqlExec;
	sqlExec << "insert into attendance (event,member) values ('" << event.c_str() << "','"  << member.c_str() << "' )";
	XEXEC execquery( m_db.get(), AnsiString(sqlExec.str().c_str()).c_str() );
	throwUnless( execquery.exec(), "Failed to exec query" );
}

void database::getEvents(std::vector<String> &eventslist)
{
	eventslist.clear();
	std::wstringstream sqlquery;
	sqlquery << "select name from events";

	XQUERY query( m_db.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve events" );
	while ( query.fetch() )    //any data?
	{
		eventslist.push_back(UnicodeString(query.result.getString(0).c_str()));
	}
	query.close();
}

void database::getMembers(std::vector<String> &memberslist)
{
	memberslist.clear();
	std::wstringstream sqlquery;
	sqlquery << "select name from members";

	XQUERY query( m_db.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve members" );
	while ( query.fetch() )    //any data?
	{
		memberslist.push_back(UnicodeString(query.result.getString(0).c_str()));
	}
	query.close();
}

void database::getMembersForEvent(String &currentEvent,std::vector<String> &memberlist)
{
	memberlist.clear();
	std::wstringstream sqlquery;
	sqlquery << "select member from attendance WHERE event = '" << currentEvent.c_str() << "'";

	XQUERY query( m_db.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve events" );
	bool bAlreadyAttending = false;
	while ( query.fetch() )    //any data?
	{
		memberlist.push_back(UnicodeString(query.result.getString(0).c_str()));
	}
	query.close();
}

void database::getMembersNoInEvent(String &currentEvent,std::vector<String> &memberlist)
{
	memberlist.clear();
	std::wstringstream sqlquery;
	sqlquery << "SELECT name FROM members WHERE name NOT IN (SELECT member FROM attendance WHERE event = '" << currentEvent.c_str() << "')";

	//sqlquery << "select member from attendance WHERE event = '" << currentEvent.c_str() << "'";

	XQUERY query( m_db.get(), AnsiString(sqlquery.str().c_str()).c_str() );
	throwUnless( query.open(), "Failed to retreve events" );
	bool bAlreadyAttending = false;
	while ( query.fetch() )    //any data?
	{
		memberlist.push_back(UnicodeString(query.result.getString(0).c_str()));
	}
	query.close();
}

void database::throwUnless( bool condition, const String& msg )
{
	if ( ! condition )
	{
		throw msg;
	}
}

void onError( const std::string& msg )
{
	throw String(msg.c_str());
//	printf( "ERROR!\n%s\n", msg.c_str() );
}
bool dbErrorCallback( const std::string object, const int instance,const int ecount, const int ecode, const std::string error_txt )
{
	onError( error_txt );
	return( true );
}
