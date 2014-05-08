//---------------------------------------------------------------------------

#ifndef LogFileH
#define LogFileH

#include <stack>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

//---------------------------------------------------------------------------

class LogFile
{
	std::ofstream pfOut;
	std::string type, fileName;
	bool inTag;

	static const short SHORT_STRING = 100;	   	// enough for dates etc.
	static const int CHUNK_SIZE = 500000;       // file < half megabyte

	std::ofstream & openFile();
	std::string uniqueName( std::string extension = "txt" );

	void encode( const char * text );
	void encode( unsigned char byte );
	void encode( const TDateTime & when );

	LogFile( const LogFile & );				// not implemented
	LogFile & operator=( LogFile & );		// not implemented

public:

	LogFile( const char * root ) : type( root ), inTag( false )
	{}
	~LogFile();

	void start( const char * tag );
	void endTag();

	void addText( const std::string & text );
	void addCharacter( const unsigned char byte );

	void addAttribute( const char * name, const std::string & value );
	void addAttribute( const char * name, int value );
	void addAttribute( const char * name, const TDateTime & when );

	static void logError( const char * tag, const std::string & message );
	static TDateTime isoDateTime( const std::string & when );
};

//---------------------------------------------------------------------------

#endif

