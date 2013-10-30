#include "xuc.h"
//===========================================================================
XUC::XUC( void )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XUC::~XUC( void )
{
}
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
String XUC::stds2cbs( const std::string in )
{
	String	s = in.c_str();
	return( s );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string XUC::cbs2stds( const String in )
{
	AnsiString	x;
	try
		{x = in;
		}
	catch ( ... )
		{x = "?1";
		}
	std::string	s = x.c_str();
	String	chk = stds2cbs( s );
	if ( in != chk )
		{return( "?2" );
		}
	return( s );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
wchar_t *XUC::char2wchar( const char *source )
{
	if ( NULL == source )
		{return( NULL );
		}
	int	outlen = strlen( source );   // DUNNO IF OUTPUT SIZE IS GOOD
	wchar_t	*wresult = (wchar_t *) malloc( sizeof(wchar_t) * ( outlen+1 ) );
	if ( NULL == wresult )
		{return( NULL );
		}
	if ( (size_t) -1 == mbstowcs( wresult, source, outlen + 1 ) )
		{free( wresult ); 		// CONVERSION FAILURE
		return( NULL );
		}
	return( wresult );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XUC::amsg( const std::string msg, const std::string title,
		const int opts )
{
	String	m = stds2cbs( msg );
	String	t = stds2cbs( title );
#pragma warn -8111		// ACCESSING DEPRECATED ENTITY T_STR
	int	retval = Application->MessageBox( m.t_str(), t.t_str(),
		opts );
#pragma warn +8111
	return( retval );
}
#endif
//===========================================================================

