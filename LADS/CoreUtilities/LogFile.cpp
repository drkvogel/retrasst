/*--------------------------------------------------------------------------
 *
 *	LogFile - replaces XMLFile for the Labs and other systems
 *
 *--------------------------------------------------------------------------*/

#include <vcl.h>
#include <iomanip>

#include "LogFile.h"
#include "LCDbProject.h"
#include "LIMSParams.h"
#include "LCDbOperator.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//	Shared singleton for general event and error logging
//---------------------------------------------------------------------------

void LogFile::logError( const char * tag, const std::string & value )
{
	static LogFile sharedLog( "log" );
	sharedLog.start( tag );
	sharedLog.addAttribute( "time", Now() );
	sharedLog.addAttribute( "value", value );
	sharedLog.endTag();
}

//---------------------------------------------------------------------------
//  Close any open tags, flush and close the output file
//---------------------------------------------------------------------------

LogFile::~LogFile()
{
	if( pfOut.is_open() ) {
		endTag();
		pfOut.close();
	}
}

//---------------------------------------------------------------------------
//  Start a new file if necessary and (re)write header to include it
//	otherwise just mark the end of the current tag; return file handle
//---------------------------------------------------------------------------

std::ofstream & LogFile::openFile()
{
	if( pfOut.is_open() ) {
		endTag();
	} else {
		if( fileName.empty() ) {
			fileName = uniqueName();
		}
		pfOut.open( fileName.c_str() );
		inTag = false;
	}
	return pfOut;
}

//---------------------------------------------------------------------------
//  Start a new section, opening the output file if necessary
//---------------------------------------------------------------------------

void LogFile::start( const char * tag )
{
	openFile() << tag;
	inTag = true;
}

//---------------------------------------------------------------------------
//  Close the current section; close file if it's getting too big
//---------------------------------------------------------------------------

void LogFile::endTag()
{
	if( inTag )	{
		pfOut << std::endl;
		inTag = false;
		if( pfOut.tellp() > CHUNK_SIZE ) {
			pfOut.close();
			fileName = "";
		} else {
			pfOut.flush();
		}
	}
}

//---------------------------------------------------------------------------
//	Lower level routines to write out a single value or attribute
//---------------------------------------------------------------------------

void LogFile::addAttribute( const char * name, const std::string & value )
{
	pfOut << ' ' << name << "=\"" << value << '\"';
}

//---------------------------------------------------------------------------

void LogFile::addAttribute( const char * name, int value )
{
	pfOut << ' ' << name << "=" << value;
}

//---------------------------------------------------------------------------

void LogFile::addAttribute( const char * name, const TDateTime & when )
{
	pfOut << ' ' << name << "=\"";
	encode( when );
	pfOut << '\"';
}

//---------------------------------------------------------------------------

void LogFile::addText( const std::string & text ) {
	pfOut << std::endl << ' ';
	encode( text.c_str() );
	pfOut << std::endl;
}

//---------------------------------------------------------------------------

void LogFile::addCharacter( unsigned char byte ) {
	encode( byte );
}

//---------------------------------------------------------------------------
//  Add a 'C' string to the output stream, encoding control characters
//---------------------------------------------------------------------------

void LogFile::encode( const char * text )
{
	if( text != NULL ) {
		while( *text != '\0' ) {
			encode( *text ++ );
		}
	}
}

//---------------------------------------------------------------------------

void LogFile::encode( unsigned char byte )
{
	if( byte < 0x20 || byte >= 0x7F ) {
		char text[ 10 ];
		std::sprintf( text, "&#x%02X;", byte );
		pfOut << text;
	} else {
		pfOut << byte;
	}
}

//---------------------------------------------------------------------------
//  Convert a date and/or time to ISO format (CCYY-MM-DDThh:mm:ss)
//---------------------------------------------------------------------------

void LogFile::encode( const TDateTime & timeStamp )
{
	unsigned short year, month, day;
	char text[ 15 ];
	timeStamp.DecodeDate( &year, &month, &day );
	if( year + month + day != 0 )
	{
		std::sprintf( text, "%d-%02d-%02d", year, month, day );
		pfOut << text;
	}
	unsigned short hour, min, sec, msec;
	timeStamp.DecodeTime( &hour, &min, &sec, &msec );
	if( hour + min + sec != 0 || year + month + day == 0 )
	{
		std::sprintf( text, "t%02d:%02d:%02d", hour, min, sec );
		pfOut << text;
	}
}

//---------------------------------------------------------------------------
//  Parse a date field (in ISO format) back into a (Borland) date/time
//---------------------------------------------------------------------------

TDateTime LogFile::isoDateTime( const std::string & when )
{
	if( when.empty() )
		return 0;

	const char * start = when.c_str();
	int dateLen = std::strcspn( start, " tT" );
	if( dateLen == 0 )
		return TTime( start + 1 );

	unsigned short year, month, day;
	std::sscanf( start, "%hd-%hd-%hd", &year, &month, &day );
	TDate date( year, month, day );
	if( start[ dateLen ] == '\0' )
		return date;

	return date += TTime( start + dateLen + 1 );
}

//---------------------------------------------------------------------------
//  Use the current date and time to create an almost unique file name
//---------------------------------------------------------------------------

std::string LogFile::uniqueName( std::string ext )
{
	AnsiString source = ExtractFileName( Application -> ExeName );
	AnsiString base = source.SubString( 1, source.LastDelimiter( "." ) - 1 );

	unsigned short year, month, day, hour, min, sec, msec;
	TDateTime start = Now();
	start.DecodeDate( &year, &month, &day );
	start.DecodeTime( &hour, &min, &sec, &msec );

	static char letter = '@';
	++ letter;
	if( !std::isupper( letter ) )
		letter = 'A';

	if( type.empty() )
		type = "log";

	if( ext.empty() )
		ext = "txt";

	char buffer[ SHORT_STRING ];
	std::sprintf( buffer, "%s.%02d%02d%02d%02d%02d%c.%s.%s", base.c_str(),
					year % 100, month, day, hour, min, letter, type.c_str(), ext.c_str() );
	return LIMSParams::instance().getLogFolder() + buffer;
}

//---------------------------------------------------------------------------

