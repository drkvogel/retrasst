#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "xbasic.h"
#include "mess.h"
#include "xtime.h"
#include "xuc.h"
//---------------------------------------------------------------------------
std::string 	MESS::person_name = "participant";
std::string 	MESS::staff_name = "staff";
std::string	MESS::logfile_name = "tmp.log";
bool 		MESS::noise_enabled = false;
//---------------------------------------------------------------------------
		/* SUBSTITUTE PERSON NAME INTO`$' SYMBOLS IN MESSAGE TEXT */
void MESS::personaliseText( std::string *txt )
{	int	i;
	int	count = 0;
	const	char	*buf = txt->c_str();
	int	len = txt->size();
	std::string	out;
	for ( i = 0; i < len; i++ )
		{if ( '$' != buf[i] )
			{out += buf[i];
			}
		else
			{out += person_name;
			count++;
			}
		}
	if ( count > 0 )
		{*txt = out;
		}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
					/* MAKE A BLEEP IF SOUND IS ENABLED */
void MESS::bleep( const bool forced )
{
	if ( forced || noise_enabled )
		{Beep();
		}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
						/* MESSAGE BOX WRAPPER */
int MESS::amsg( const std::string txt, const std::string title, const int but_code )
{
	char	*ret_txt;
	std::string	mtxt = txt;
	if ( person_name != "" )
		{personaliseText( &mtxt );
		}
	int ret_val = AMSG( mtxt, title, but_code );
	switch( ret_val )
		{case mrOk:
			ret_txt = "OK";
			break;
		case mrYes:
			ret_txt = "YES";
			break;
		case mrNo:
			ret_txt = "NO";
			break;
		case mrCancel:
			ret_txt = "CANCEL";
			break;
		case mrAbort:
			ret_txt = "ABORT";
			break;
		default:
			ret_txt = "UNKNOWN";
			break;
		}
	aside( "\nMSGBOX %s\n\t\"%s\"\n\tMSGBOX retval = %d (%s)",
		title.c_str(), txt.c_str(), ret_val, ret_txt );
	return( ret_val );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int MESS::error( const std::string txt, const int but_code )
{	return( amsg( txt, "Error", but_code ) );   //mlz
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int MESS::warning( const std::string txt, const int but_code )
{	return( amsg( txt, "Warning", but_code ) ); //mlz
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int MESS::check( const std::string txt, const int but_code )
{	return( amsg( txt, "Check", but_code ) ); //mlz
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int MESS::decide( const std::string txt, const int but_code )
{	return( amsg( txt, "User decision", but_code ) ); //mlz
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int MESS::info( const std::string txt, const int but_code )
{	return( amsg( txt, "Information", but_code ) ); //mlz
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int MESS::action( const std::string txt, const int but_code )
{	return( amsg( txt, "Action", but_code ) ); //mlz
}
//---------------------------------------------------------------------------
				/* DISPLAY A BUG MESSAGE AND LOG TO FILE */
void MESS::bug( const std::string txt )
{
	static	int	bug_count = 0;
	FILE	*fb;
	bleep( true );
	amsg( txt, "Internal Program Error!", MB_OK );  //mlz
	fb = fopen( "red.bug", "a" );
	if ( NULL == fb )
		{return;	/* FAILED TO OPEN BUG FILE */
		}
	std::string	tstr = XTIME().iso();
	fprintf( fb, "\n\n[%d]\t%s\n%s", ++bug_count, tstr.c_str(),txt.c_str());
	fclose( fb );
}
//---------------------------------------------------------------------------
						/* WRITE ONLY TO LOGFILE */
								/*VARARGS*/
void MESS::aside( const char *txt, ...  )
{
	static	int	ncalls = -1;
	va_list	args;				/* VARIABLE ARGUMENT LIST */
	char	time_stamp[30];
	FILE	*flog;
	ncalls++;
	if ( NULL == ( flog = fopen( logfile_name.c_str(), "a" ) ) )
		{if ( 0 == ncalls % 10 )
			{AMSG(	"Unable to open logfile."
				"\n\nPlease report this to CTSU",
				"Error", MB_OK );
			}
		return;			// UNABLE TO OPEN LOGFILE
		}
	XTIME	now;
	sprintf( time_stamp, "%-5d %2.2d:%2.2d:%2.2d",
		ncalls, now.getHour(), now.getMinute(), now.getSecond() );
	fprintf( flog, "\n%s ", time_stamp );
	va_start( args, txt );
	vfprintf( flog, txt, args );
	va_end( args );				// TIDY ARGUMENT LIST
	fclose( flog );
}
//---------------------------------------------------------------------------
							/* OPEN LOGFILE */
void MESS::initLogfile( const std::string directory, const std::string prefix )
{       char	emsg[200];
	FILE	*fl;
	XTIME	now;
	char	tbuf[100];
	sprintf( tbuf, "%4.4d%2.2d%2.2d_%2.2d%2.2d%2.2d",
		now.getYear(), now.getMonth(), now.getDay(),
		now.getHour(), now.getMinute(), now.getSecond() );
	logfile_name = directory + prefix + tbuf;
	fl = fopen( logfile_name.c_str(), "a" );
	if ( NULL == fl )
		{sprintf( emsg, "Unable to open logfile \"%s\""    //mlz
			"\n\nPlease report to IT support",
			logfile_name.c_str() );
		AMSG( emsg, "Error", MB_OK );
		return;
		}
	fprintf( fl, "**** INIT **** Logfile \"%s\" ****", logfile_name.c_str() );
	fclose( fl );
	aside( "logfile initialised" );
}
//---------------------------------------------------------------------------






