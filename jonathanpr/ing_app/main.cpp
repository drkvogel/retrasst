#include <vcl.h>
#include <windows.h>

#pragma hdrstop
#pragma argsused

#include <memory>
#include <tchar.h>
#include <stdexcept>
#include <stdio.h>
#include "xdb.h"
#include "xquery.h"

void throwUnless( bool condition, const std::string& msg )
{
	if ( ! condition )
	{
		throw msg;
	}
}

// From: http://www.cplusplus.com/forum/general/3570/
std::string getPassword( const std::string& prompt = "Enter password> " )
{
	std::string result;

	// Set the console mode to no-echo, not-line-buffered input
	DWORD mode, count;
	HANDLE ih = GetStdHandle( STD_INPUT_HANDLE  );
	HANDLE oh = GetStdHandle( STD_OUTPUT_HANDLE );
	throwUnless( GetConsoleMode( ih, &mode ),
		"You must be connected to a console to use this program." );

	SetConsoleMode( ih, mode & ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT) );

	// Get the password string
	WriteConsoleA( oh, prompt.c_str(), prompt.length(), &count, NULL );
	char c;
	while (	ReadConsoleA( ih, &c, 1, &count, NULL) &&
			(c != '\r') && (c != '\n') )
	{
		if (c == '\b')
		{
			if (result.length())
			{
				WriteConsoleA( oh, "\b \b", 3, &count, NULL );
				result.erase( result.end() -1 );
			}
		}
		else
		{
			WriteConsoleA( oh, "*", 1, &count, NULL );
			result.push_back( c );
		}
	}

	// Restore the console mode
	SetConsoleMode( ih, mode );

	return result;
}

void onError( const std::string& msg )
{
	printf( "ERROR!\n%s\n", msg.c_str() );
}

bool dbErrorCallback( const std::string object, const int instance,
	const int ecount, const int ecode, const std::string error_txt )
{
	onError( error_txt );
	return( true );
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::auto_ptr<XDB> db( new XDB( "@brat.ctsu.ox.ac.uk,wintcp,II::paulst_test" ) );
	// Note that XDB destructor will close the connection, if opened.

	try
	{
		db->setErrorCallBack( dbErrorCallback );
		std::string username("cp");
		std::string password = getPassword();
		db->setUserName( username );
		db->setPassWord( password );
		throwUnless ( db->open(), "Failed to connect!" );
		XQUERY query( db.get(), "select external_name from c_project" );
		throwUnless( query.open(), "Failed to open query" );
		while( query.fetch() )
		{
			std::printf( "%s\n", query.result.getString(0).c_str() );
		}
	}
	catch( const std::string& msg )
	{
		onError( msg );
	}
	catch( const std::runtime_error& e )
	{
		onError( e.what() );
	}

	return 0;
}

