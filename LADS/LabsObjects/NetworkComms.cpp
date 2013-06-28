//---------------------------------------------------------------------------

#include <vcl.h>

#include "NetworkComms.h"
#include "TfrmSocketComms.h"

#pragma hdrstop

#pragma package(smart_init)    

//---------------------------------------------------------------------------

void NetworkComms::open()
{
	source = frmNetComms -> open();
}

//---------------------------------------------------------------------------

unsigned NetworkComms::available()
{
	return frmNetComms -> available();
}

//---------------------------------------------------------------------------

unsigned NetworkComms::readBytes( unsigned char * buffer, unsigned max )
{
	unsigned numRead = frmNetComms -> readBytes( buffer, max );
	record( "rx", buffer, numRead );
	return numRead;
}

//---------------------------------------------------------------------------

unsigned NetworkComms::sendBytes( const unsigned char * bytes, unsigned count )
{
	unsigned numSent = frmNetComms -> sendBytes( (void *) bytes, count );
	record( "tx", bytes, numSent );
	return numSent;
}

//---------------------------------------------------------------------------

void NetworkComms::clear()
{
	frmNetComms -> clear();
}

//---------------------------------------------------------------------------
