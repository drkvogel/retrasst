/*--------------------------------------------------------------------------
 *
 *	XMLFile - an XML-based log file for the Labs and other systems
 *
 *  22/5/2002 - converted to template to allow normal or async I/O
 *  #include with XMLFile.h as C++Builder doesn't compile templates
 *
 *  Modified 22/5/2003: flush output from endTag() if file too big
 *  Modified 7 May 2004: store xml in sections until completed
 *
 *	14 May 2004, NG:	Class (not template) moved to Shared/LabUtils
 *  18 June, NG/AM:     Checked in to CVS/GeneralClasses/CoreUtilities
 *	18 March 2005:		Allow caller to specify (single) output file
 *	19 July 05, NG:		Moved addHeader() here; checked in XMLTokens
 *	11 Jan 2008, NG:	Merged with LogFile but using std::file I/O
 *  16 July 08, NG:		Use date and time as digits in default file name
 *	11/03/2009, NG:		Always put log files into CTSU_CP (for laptops)
 *  9 Apr 2009, NG:		Bug fix: always use application name in file path
 *	8 Feb 2010, NG:		Encode \r as #xD, \n as #xA and space as #x20
 *  15 Feb 2010, NG:	Encode white space only if encodeSpace == true
 *  25 June 2012		Use std::string for Borland C++Builder XE2
 *
 *--------------------------------------------------------------------------*/

#include <vcl.h>
#include <iomanip>

#include "XMLFile.h"
#include "LCDbProject.h"
#include "LIMSParams.h"
#include "LCDbOperator.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//	Shared singleton for general event and error logging
//---------------------------------------------------------------------------

void XMLFile::logError( const char * tag, const std::string & message )
{
	static XMLFile sharedLog( "log" );
	sharedLog.start( tag );
	sharedLog.addAttribute( "time", Now() );
	sharedLog.addText( message );
	sharedLog.endTag();
}

//---------------------------------------------------------------------------
//  Close any open tags, flush and close the output file
//---------------------------------------------------------------------------

XMLFile::~XMLFile()
{
	while( !tags.empty() )
		endTag();

	if( pfOut.is_open() )
		pfOut.close();
}

//---------------------------------------------------------------------------
//  Start a new file if necessary and (re)write header to include it
//	otherwise just mark the end of the current tag; return file handle
//---------------------------------------------------------------------------

std::ofstream & XMLFile::openFile()
{
	if( pfOut.is_open() ) {
		if( inTag ) {
			pfOut << '>';
		}
	} else if( chunks ) {
		// header to include other files containing XML fragments
		parts.push_back( createChunkFile() );
		std::ofstream header( fileName.c_str() );
		if( !header ) {
			throw Exception( String( "Cannot create " ) + fileName.c_str() );
        }
		header << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
			   << "<!DOCTYPE " << type << " [\n";
		for( unsigned i = 0; i != parts.size(); i ++ ) {
			header << "<!ENTITY file" << i << " SYSTEM \'" << parts[ i ] << "\' >\n";
		}
		header << "]>\n<" << type << ">\n";
		for( unsigned i = 0; i != parts.size(); i ++ ) {
			header << "&file" << i << ";\n";
		}
		header << "</" << type << ">\n";
	}
	else
	{	// no chunking; use named output file (closed by destructor)
		if( fileName.empty() ) {
			fileName = uniqueName( "xml" );
		}
		pfOut.open( fileName.c_str() );
		pfOut << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<" << type << ">\n";
		tags.push( type );
	}
	inTag = false;
	return pfOut;
}

//---------------------------------------------------------------------------
//  Start a new section, opening the output file if necessary
//---------------------------------------------------------------------------

void XMLFile::start( const char * tag )
{
	openFile() << '<' << tag;
	inTag = true;
	tags.push( tag );
}

//---------------------------------------------------------------------------

bool XMLFile::inElement() const {
	return !tags.empty();
}

//---------------------------------------------------------------------------

const std::string & XMLFile::getElement() const {
	return tags.empty() ? type : tags.top();
}

//---------------------------------------------------------------------------
//  Close the current section; close file if it's getting too big
//---------------------------------------------------------------------------

void XMLFile::endTag()
{
	if( tags.empty() )
		throw Exception( "XML error: cannot close top-level element" );

	if( inTag )
	{
		pfOut << " />\n";
		inTag = false;
	}
	else
	{	pfOut << "</" << tags.top() << ">\n";
		pfOut.flush();
	}

	tags.pop();
	if( tags.empty() && chunks && pfOut.tellp() > CHUNK_SIZE )
		pfOut.close();
}

//---------------------------------------------------------------------------
//  Add a single piece of data, enclosed by matching tags
//---------------------------------------------------------------------------

void XMLFile::addLeaf( const char * tag, TDateTime timestamp )
{
	openFile() << '<' << tag << '>';
	encode( timestamp );
	pfOut << "</" << tag << '>' << std::endl;
}

//---------------------------------------------------------------------------

void XMLFile::addEmpty( const char * tag )
{
	openFile() << '<' << tag << " />";
}

//---------------------------------------------------------------------------

void XMLFile::addLeaf( const char * tag, int value )
{
	openFile() << '<' << tag << '>' << std::dec << value
			   << "</" << tag << '>' << std::endl;
}

//---------------------------------------------------------------------------

void XMLFile::addLeaf( const char * tag, const std::string & body )
{
	openFile() << '<' << tag << '>';
	encode( body.c_str() );
	pfOut << "</" << tag << '>' << std::endl;
}

//---------------------------------------------------------------------------
//	Lower level routines to write out a single value or attribute
//---------------------------------------------------------------------------

void XMLFile::addAttribute( const char * name, const std::string & value )
{
	pfOut << ' ' << name << "=\"" << value << '\"';
}

//---------------------------------------------------------------------------

void XMLFile::addAttribute( const char * name, const TDateTime & when )
{
	pfOut << ' ' << name << "=\"";
	encode( when );
	pfOut << '\"';
}

//---------------------------------------------------------------------------

void XMLFile::addText( const std::string & text ) {
	openFile();
	encode( text.c_str() );
}

//---------------------------------------------------------------------------

void XMLFile::addCharacter( unsigned char byte ) {
	openFile();
	encode( byte );
}

//---------------------------------------------------------------------------
//  Add a 'C' string to the output stream, encoding XML entities
//---------------------------------------------------------------------------

void XMLFile::encode( const char * text )
{
	if( text != NULL && *text != '\0' )
	{
		do encode( *text ++ );
		while( *text != '\0' );
	}
}

//---------------------------------------------------------------------------

void XMLFile::encode( unsigned char byte )
{
	if( isspace( byte ) ? encodeSpace : byte < 0x20 || byte >= 0x7F )
	{
		char text[ 10 ];
		std::sprintf( text, "&#x%02X;", byte );
		pfOut << text;
	}
	else
	{	switch( byte )
		{
			case '<':
				pfOut << "&lt;";
				break;

			case '&':
				pfOut << "&amp;";
				break;

			case '>':
				pfOut << "&gt;";
				break;

			case '\'':
				pfOut << "&apos;";
				break;

			case '\"':
				pfOut << "&quot;";
				break;

			default:
				pfOut << byte;
		}
	}
}

//---------------------------------------------------------------------------
//  Convert a date and/or time to ISO format (CCYY-MM-DDThh:mm:ss)
//---------------------------------------------------------------------------

void XMLFile::encode( const TDateTime & timeStamp )
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

TDateTime XMLFile::isoDateTime( const std::string & when )
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
//  Add a header to the current file with information about this process
//---------------------------------------------------------------------------

void XMLFile::addHeader()
{
	start( "history" );
	addLeaf( "project", LCDbProjects::records().get( LCDbProjects::getCurrentID() ).getName() );
	addLeaf( "program", Application -> Title + " v" + LIMSParams::instance().getProgVersion().c_str() );
	addLeaf( "operator", LCDbOperators::records().get( LCDbOperators::getCurrentID() ).getDescription() );
	addLeaf( "created", Now() );
	endTag();
}

//---------------------------------------------------------------------------
//  Work out where the file is to be stored and try to create it
//---------------------------------------------------------------------------

std::string XMLFile::createChunkFile()
{
	std::string stdPath = uniqueName();
	const char * csPath = stdPath.c_str();
	pfOut.open( csPath );
	String wsPath( csPath );
	SetFileAttributes( wsPath.c_str(), FILE_ATTRIBUTE_HIDDEN );
	if( fileName.empty() ) {
		AnsiString xml = ChangeFileExt( wsPath, ".xml" );
		fileName = xml.c_str();
	}
	return stdPath;
}

//---------------------------------------------------------------------------
//  Use the current date and time to create an almost unique file name
//---------------------------------------------------------------------------

std::string XMLFile::uniqueName( std::string ext )
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

