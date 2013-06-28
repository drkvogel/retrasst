/*
 *	File input for LIMS comms programs; shares base class with serial I/O
 *
 *		28 June 2005, NG:	Moved logging from LIMSCommsBase to SerialComms
 *		18 November, NG:	Moved it back again, but for output only
 *		21 December 2005:	Added a few more control characters in log
 *		14 February 2006:	Added XMLInputFile to parse old i/o logs
 *		4 December 09, NG:	Use unsigned char throughout (no #fffffaa)
 *		8 Feb 2010, NG:		Encode \r and \n; decode <CR> and <LF>
 */

#include <vcl.h>
#include "LIMSCommsBase.h"
#include "XMLTokens.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

void LIMSCommsBase::open( const String & name, unsigned desiredAccess, unsigned shareMode )
{
	IO_Handle = CreateFile( name.c_str(), desiredAccess, shareMode, NULL, OPEN_EXISTING, 0, NULL );
	if( IO_Handle == INVALID_HANDLE_VALUE )
		throw Exception( "Cannot open " + name );
	else
		source = ExtractFileName( name );
}

//---------------------------------------------------------------------------

unsigned LIMSCommsBase::read( unsigned char * buffer, unsigned max )
{
	DWORD numRead = 0;
	if( max > 0 && !ReadFile( IO_Handle, buffer, max, &numRead, NULL ) )
		throw Exception( "problem reading from " + source );
	else
		return numRead;
}

//---------------------------------------------------------------------------
//  End previous section of the log and start a new one if necessary
//---------------------------------------------------------------------------

void LIMSCommsBase::record( const char * tag, const unsigned char * bytes, unsigned count )
{
	log.start( tag );
	log.addAttribute( "time", Now() );
	while( count > 0 )
	{
		const char * name = getName( *bytes );
		if( name != NULL )
			log.addEmpty( name );
		else
			log.addCharacter( *bytes );
		bytes ++, count --;
	}
	log.endTag();
}

//---------------------------------------------------------------------------
//  Find the name for given ASCII character; return NULL if alphanumeric
//---------------------------------------------------------------------------

const char * LIMSCommsBase::getName( unsigned char byte )
{
	switch( byte )
	{
		case NUL:
			return "NUL";

		case SOH:
			return "SOH";

		case STX:
			return "STX";

		case ETX:
			return "ETX";

		case EOT:
			return "EOT";

		case ENQ:
			return "ENQ";

		case ACK:
			return "ACK";

		case XON:
			return "XON";

		case XOFF:
			return "XOFF";

		case ETB:
			return "ETB";

		case NAK:
			return "NAK";

		case SYN:
			return "SYN";

		case CR:
			return "CR";

		case LF:
			return "LF";

		case ESC:
			return "ESC";

		default:
			return NULL;
	}
}

//---------------------------------------------------------------------------
//	Output buffer - grows as characters added but never shrinks
//---------------------------------------------------------------------------

Buffer::~Buffer()
{
	if( store != NULL )
		free( store );
}

//---------------------------------------------------------------------------
//  Add characters to the output buffer; allocating memory as needed
//---------------------------------------------------------------------------

unsigned char * Buffer::makeSpace( unsigned length )
{
	unsigned block = BLOCK_SIZE > length ? BLOCK_SIZE : length + 1;
	if( store == NULL )
		store = (unsigned char *) malloc( size = block );

	else if( end + length >= size )
		store = (unsigned char *) realloc( store, size += block );

	if( store == NULL )
		OutOfMemoryError();

	return store + end;
}

//---------------------------------------------------------------------------

const Buffer & Buffer::operator=( unsigned char byte )
{
	clear();
	add( byte );
	return *this;
}

//---------------------------------------------------------------------------

const Buffer & Buffer::operator=( const char * c_str )
{
	clear();
	add( (const unsigned char *) c_str, strlen( c_str ) );
	return *this;
}

//---------------------------------------------------------------------------

void Buffer::add( unsigned char byte )
{
	makeSpace();
	store[ end ++ ] = byte;
}

//---------------------------------------------------------------------------

void Buffer::add( const unsigned char * data, unsigned length )
{
	unsigned char * extra = makeSpace( length );
	std::memcpy( extra, data, length );
	end += length;
}

//---------------------------------------------------------------------------
//	Discard used portion of the buffer, ready to read the next part
//---------------------------------------------------------------------------

void Buffer::skip( unsigned length )
{
	if( store == NULL || start + length >= end )
		clear();
	else
    	start += length;
}

//---------------------------------------------------------------------------



