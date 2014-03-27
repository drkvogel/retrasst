#include "openers.h"
#include "xcgi.h"
//---------------------------------------------------------------------------
void OPENERS::outputHead( const std::string title )
{
	const	std::string	t = XCGI::makeWebSafe( title );
	printf( "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.01 Transitional//EN"
		" http://www.w3.org/TR/html4/loose.dtd\">"
		"\n<html lang=\"en\">"
		"\n<head>"
		"\n<title>%s</title>"
		"\n<META HTTP-EQUIV=\"Pragma\" CONTENT=\"no-cache\">"
		"\n<META HTTP-EQUIV=\"Expires\" CONTENT=\"-1\">"
		"\n<link rel=\"stylesheet\" href=\"andemo.css\""
		" type=\"text/css\">"
		"\n</head>", t.c_str() );
	fflush( stdout );
}
//---------------------------------------------------------------------------
static bool dbErr( const std::string caller, const int instance,
	const int ecount, const int ecode, const std::string etxt )
{
	fflush( stdout );
	const	std::string	e = XCGI::makeWebSafe( etxt );
	printf( "\n<!--\nDATABASE ERROR inst=%d, ecnt=%d, ecod=%d\n%s\n-->",
		instance, ecount, ecode, e.c_str() );
	fflush( stdout );
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define	IISYS_NAME	"II_SYSTEM"
#define	IISYS_VAL	"/dbsystem/II"
XDB *OPENERS::openDatabase( void )
{
#ifdef __BORLANDC__
//	char	*db_name = "dice_erg::erg_a2_demo";
	char	*db_name =
	"@dice.ctsu.ox.ac.uk,tcp_ip,II[erg,P314e271]::erg_a2_demo";
#else
	char	*db_name = "erg_a2_demo";
#endif
	char	*e = getenv( IISYS_NAME );      // BODGE FOR SUEXEC
	if ( NULL == e || strlen(e) < 4 )
		{putenv( IISYS_NAME "=" IISYS_VAL );
		}
	XDB *db = new XDB( db_name );
	db->setErrorCallBack( dbErr );
	if ( ! db->isValid() )
		{printf( "<!-- Invalid database -->" );
		delete db;
		return( NULL );
		}
	if ( ! db->open() )
		{printf( "\n<!-- Database failed to open -->" );
		delete db;
		return( NULL );
		}
	return( db );
}
//---------------------------------------------------------------------------

