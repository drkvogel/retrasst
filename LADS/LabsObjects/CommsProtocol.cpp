//---------------------------------------------------------------------------

#include <vcl.h>
#include "CommsProtocol.h"
#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------
//	extract the given field from a record using the given delimiter(s)
//---------------------------------------------------------------------------

String CommsRecord::split( const String & input, int num, const char * delimiters )
{
	char * start = input.c_str();
	for( int part = 1; part < num && *start != '\0'; ++ start )
		for( const char * d = delimiters; *d != '\0'; ++ d )
			if( *start == *d )
				part ++;

	for( char * end = start; *end != '\0'; ++ end )
		for( const char * d = delimiters; *d != '\0'; ++ d )
			if( *end == *d )
				return String( start, end - start );

	return String( start );
}

//---------------------------------------------------------------------------
//	queue a request from the analyser to program the given sample
//---------------------------------------------------------------------------

LIMSAnalysis & CommsWorklist::add( const String & barcode )
{
	pending.push_back( LIMSAnalysis( barcode, categoryID, Now() ) );
	return pending.back();
}

//---------------------------------------------------------------------------
// 	take next barcode from the front of the queue; empty if none pending
//---------------------------------------------------------------------------

LIMSAnalysis CommsWorklist::getNextSample()
{
	LIMSAnalysis queued;
	std::deque< LIMSAnalysis >::iterator next = pending.begin();
	if( next != pending.end() ) {
		queued = *next;
		pending.erase( next );
	}
	return queued;
}

//---------------------------------------------------------------------------

