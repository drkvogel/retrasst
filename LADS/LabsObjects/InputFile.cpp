/*
 *	File input for LIMS comms programs; shares base class with serial I/O
 *
 *		28 June 2005, NG:	Moved logging from LIMSCommsBase to SerialComms
 *		18 November, NG:	Moved it back again, but for output only
 *		21 December 2005:	Added a few more control characters in log
 *		14 February 2006:	Added XMLInputFile to parse old i/o logs
 *		4 December 09, NG:	Use unsigned char throughout (no #fffffaa)
 *		8 Feb 2010, NG:		Encode \r and \n; decode <CR> and <LF>
 *		13 Dec 1011, NG:	Removed LIMSCommsBase into another file
 */

#include <vcl.h>
#include "InputFile.h"
#include "XMLTokens.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

InputFile::InputFile( const String & filePath )
{
	open( filePath, GENERIC_READ, FILE_SHARE_READ );
}

//---------------------------------------------------------------------------

unsigned InputFile::length()
{
	DWORD size = GetFileSize( IO_Handle, NULL );
	if( size == 0xFFFFFFFF || size == 0 )
		throw Exception( "Invalid input " + getSource() );
	else
		return size;
}

//---------------------------------------------------------------------------
//  Write data to the log file as if it had be sent to the analyser
//---------------------------------------------------------------------------

unsigned InputFile::sendBytes( const unsigned char * bytes, unsigned count )
{
	if( count != 0 )
		record( "tx", bytes, count );
	return count;
}

//---------------------------------------------------------------------------
//  Read the next few characters from the file; return number read
//---------------------------------------------------------------------------

unsigned TextInputFile::readBytes( unsigned char * buffer, unsigned max )
{
	unsigned numRead = read( buffer, max );
	if( max > 0 && numRead == 0 || numRead > remaining )
		remaining = 0;							// end of file(?)
	else
		remaining -= numRead;

	buffer[ numRead ] = '\0';
	return numRead;                 			// numRead <= count
}

//---------------------------------------------------------------------------
//  Read XML log file into passed buffer and pass back <rx> elements
//---------------------------------------------------------------------------

unsigned XMLInputFile::readBytes( unsigned char * buffer, unsigned max )
{
	if( input == NULL )
	{
		unsigned size = length();
		input = (unsigned char *) malloc( size + 1 );
		unsigned numRead = read( input, size );
		input[ numRead ] = '\0';
		next = (const char *) input;
	}

	const char * end = input;
	if( next != NULL )
	{
		next = strstr( next, "<rx" );
		if( next != NULL )
		{
			next = strchr( next, '>' );
			if( next != NULL )
			{
				end = strstr( next, "</rx" );
				if( end == NULL )
					end = next + strlen( next );

				if( end - next > max )
					throw Exception( "Insufficient buffer space" );
			}
		}
	}

	unsigned char * out = buffer;
	while( next != NULL && ++ next < end )
	{
		bool found = false;
		if( *next == '<' )
		{
			for( unsigned char ch = 0; ch < ' ' && !found; ch ++ )
			{
				const char * name = getName( ch );
				if( name != NULL && strnicmp( next + 1, name, strlen( name ) ) == 0 )
				{
					*out ++ = ch;
					next = strchr( next, '>' );
					found = true;
				}
			}
		}
		else if( *next == '&' )
		{
			const char * end = strchr( next, ';' );
			if( end != NULL )
			{
				unsigned char ch = convert( next + 1, end - next - 1 );
				if( ch != 0 )
				{
					*out ++ = ch;
					next = end;
					found = true;
				}
			}
		}

		if( !found )
			*out ++ = *next;
	}

	*out = '\0';
	return out - buffer;
}

//---------------------------------------------------------------------------

unsigned char XMLInputFile::convert( const char * entity, unsigned length )
{
	if( length == 4 && entity[ 0 ] == '#' && (entity[ 1 ] == 'x' || entity[ 1 ] == 'X') )
		return (fromHex( entity[ 2 ] ) << 4) + fromHex( entity[ 3 ] );

	static const char characters[] = "<&\'\">";
	static const char * entities[] = { "lt", "amp", "apos", "quot", "gt", NULL };
	unsigned i = 0;
	while( i < strlen( characters ) && strncmp( entity, entities[ i ], length ) != 0 )
		i ++;
	return characters[ i ];
}

//---------------------------------------------------------------------------

unsigned XMLInputFile::fromHex( unsigned char ch )
{
	if( ch >= '0' && ch <= '9' )
		return ch - '0';

	if( ch >= 'A' && ch <= 'F' )
		return ch + 10 - 'A';

	return ch + 10 - 'a';
}

//---------------------------------------------------------------------------

