#include <stdlib.h>
#include "randdefs.h"
#include "xbasic.h"
#include "xcgi.h"
#include "xdb.h"
#include "xquery.h"
#include "xencode.h"
#include "randutil.h"
#include "model.h"
#include "view.h"
//---------------------------------------------------------------------------
static	RAND_UTIL	*ru = NULL;
static	XDB		*db = NULL;
//---------------------------------------------------------------------------

// REPORT TO LOG FILE.
static void report( const std::string & problem )
{
	ru->getCGI()->log( problem.c_str() );
}

static void run( void )
{
	XCGI * params = ru->getCGI( );

	Model info( params );
	try
	{
		info.parse(params);
		info.check( db, ru,params);
		info.logResult( db, params );
	}
	catch( const std::string &problem ) {
		report( problem + '\n' );
	}
	catch( const std::exception &error ) {
		report( error.what() + '\n' );
	}
	const View * page = info.nextStage( );
	page->sendHTML( );
	delete page;
}
//---------------------------------------------------------------------------
int main( int argc, char **argv )
{
	XCGI	cgi( argc, argv );
	cgi.writeHeader( XCGI::typeHtml );
	std::string	e;
	ru = new RAND_UTIL( &cgi );
#ifndef __BORLANDC__
	db = ru->openDB( "erg_attila" );
#else
	db = ru->openDB( "dice_erg::erg_attila" );
#endif
	if ( ru->isValid() )
	{
		std::vector<std::string>	sty;
		sty.push_back( "attila.css" );
		ru->htmlBeginDoc( "Attila Randomisation", sty );
		ru->htmlBeginBody( );
#ifndef _DEBUG
		if ( ru->approvedIP() )
#endif
		{
			run();
		}
		ru->htmlEndBody( );
		ru->htmlEndDoc();
	}
	else
	{
		printf(ru->getLastError().c_str());
		printf( "\nError initiating" );
	}
	XDELETE( ru );
	cgi.end();
	return( EXIT_SUCCESS );
}
//---------------------------------------------------------------------------

