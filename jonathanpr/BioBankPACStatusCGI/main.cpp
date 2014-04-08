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
	Model info( params,db,ru );
	try
	{
		info.parse(params);
		info.check(params);
		info.logResult(params );
	}
	catch( const std::string &problem )
	{
		report( problem + '\n' );
	}
	catch( const std::exception &error )
	{
		report( error.what() + '\n' );
	}
	View * page = info.nextStage( );
	page->sendHTML();
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
	db = ru->openDB( "biocore_pacs" );
#else
	db = ru->openDB( "bb5_extract::biocore_pacs" );
#endif
	if ( ru->isValid() )
	{
		std::vector<std::string>	sty;
		sty.push_back( "/main.css" );
		ru->htmlBeginDoc( "Biobank PACS Information", sty );
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

