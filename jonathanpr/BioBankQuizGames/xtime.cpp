#include "xtime.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
/*===========================================================================*/
const 	int 	XTIME::invalid = -1;	// VALUE RETURN IF CONTENTS INVALID
const	char	*XTIME::invalid_serialization = "ERROR";
//---------------------------------------------------------------------------
	/* DOES INPUT MATCH MASK, RETURN -1 ON TRUE, FIRST FAIL IF NOT */
static int mask_match( const char *mask, const char *input )
{
	if ( NULL == input || NULL == mask )
		{return( 0 );
		}
	int	n = 0;
	while ( 0 != mask[n] )
		{
		if ( 0 == input[n] )		// INPUT SHORTER THAN MASK
			{return( n );
			}
		if ( isdigit( mask[n] ) )
			{if ( ! isdigit( input[n] ) )
				{return( n );
				}
			}
		else if ( mask[n] != input[n] )
			{return( n );
			}
		n++;
		}
	return( ( 0 != input[n] ) ? n : -1 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
static int get_int( const char *in, int length )
{
	char	ibuf[12];
	if ( length < 1 || length > 10 )
		{return( -1 );
		}
	strncpy( ibuf, in, length );
	ibuf[length] = 0;
	return( atoi( ibuf ) );
}
//---------------------------------------------------------------------------
const char *XDATE::month_name[13] =
	{
	"January",	"February",	"March",	"April",    //mlz
	"May",		"June",		"July",		"August",   //mlz
	"September",	"October",	"November",	"December", //mlz
	""
	};
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
const char *XDATE::month_abbrev[13] =
	{
	"Jan",	        "Feb",	        "Mar",	        "Apr",          //mlz
	"May",		"Jun",		"Jul",		"Aug",          //mlz
	"Sep",	        "Oct",	        "Nov",	        "Dec",          //mlz
	""
	};
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
const int XDATE::days_norm[13]
	= { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 0 };
const int XDATE::dcum_norm[13]
	= {  0, 31, 59, 90,120,151,181,212,243,273,304,334,365 };
const int XDATE::days_leap[13]
	= { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31, 0 };
const int XDATE::dcum_leap[13]
	= {  0, 31, 60, 91,121,152,182,213,244,274,305,335,366 };
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
const	int XDATE::year_min_valid = 1800;
const	int XDATE::year_max_valid = 2037;
//---------------------------------------------------------------------------
XDATE::XDATE( void )
{
	setNow();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XDATE::XDATE( const int year, const int month, const int day )
	:
	yr( year ),
	mon( month ),
	dy( day )
{
	determineValidity();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XDATE::XDATE( const XDATE &d )
	:
	yr( d.yr ),
	mon( d.mon ),
	dy( d.dy ),
	dateValid( d.dateValid )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XDATE::XDATE( const XTIME &t )
	:
	yr( t.yr ),
	mon( t.mon ),
	dy( t.dy ),
	dateValid( t.dateValid )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XDATE::XDATE( const char *s )		// LOAD SYMBOLIC OR ISO STRING
{
	set( s );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XDATE::~XDATE( void )
{
}
//---------------------------------------------------------------------------
						/* IS YEAR A LEAP YEAR ? */
bool XDATE::isLeapYear( int nyear )
{	if ( 0 != nyear % 4 )
		{return( false );
		}
	return( ( 0 == nyear % 100 && 0 != nyear % 400 ) ? false : true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
						/* IS XDATE A LEAP YEAR ? */
bool XDATE::isLeapYear( void ) const
{	return( dateValid ? isLeapYear( yr ) : false );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XDATE::daysInMonth( const int year, const int month )
{
	if ( year < year_min_valid || year > year_max_valid
			|| month < 1 || month > 12 )
		{XTIME::error( "daysInMonth, invalid input" );
		return( 30 );
		}
	return( isLeapYear( year ) ? days_leap[month-1] : days_norm[month-1] );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XDATE::daysInYear( const int year )
{
	return( isLeapYear( year ) ? 366 : 365 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDATE::determineValidity( void )
{
	dateValid = ( yr >= year_min_valid && yr <= year_max_valid
		&& mon >= 1 && mon <= 12
		&& dy >= 1 && dy <= daysInMonth( yr, mon ) );
	return( dateValid );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDATE::isValid( void ) const
{
	return( dateValid );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDATE::set( const int year, const int month, const int day )
{
	yr  = year;
	mon = month;
	dy  = day;
	return( determineValidity() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDATE::set( const XDATE &d )
{
	return( set( d.yr, d.mon, d.dy ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
				/* SET FROM STRING, ISO FORMAT OR D/M/Y */
bool XDATE::set( const char *ds )
{
	int	y = XTIME::invalid, m = XTIME::invalid, d = XTIME::invalid;
	if ( mask_match( "1234-12-12", ds ) < 0 )	// ISO FORMAT
		{
		y = get_int( ds,   4 );
		m = get_int( ds+5, 2 );
		d = get_int( ds+8, 2 );
		}
	else if ( mask_match( "12/12/1234", ds ) < 0 )
		{
		d = get_int( ds,   2 );
		m = get_int( ds+3, 2 );
		y = get_int( ds+6, 4 );
		}
	return( set( y, m, d ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDATE::set( const time_t dt )
{
	struct tm *d = localtime( &dt );		// NOT THREAD SAVE
	if ( NULL == d )
		{		// CAN HAPPEN FOR INVALID INPUT TT
		XDATE::dateValid = false;
		return( false );
		}
	return( set( d->tm_year + 1900, d->tm_mon + 1, d->tm_mday ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDATE::setNow( void )
{
	time_t	tnow;
	time( &tnow );
	return(	set( tnow ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const int XDATE::getYear( void ) const
{
	return( dateValid ? yr : XTIME::invalid );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const int XDATE::getMonth( void ) const
{
	return( dateValid ? mon : XTIME::invalid );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const std::string XDATE::getMonthName( void ) const
{
	return( ( dateValid && mon >= 1 && mon <= 12 )
		? std::string(month_name[mon-1])
		: std::string("INVALID_MONTH") );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const std::string XDATE::getMonthAbbrev( void ) const
{
	return( ( dateValid && mon >= 1 && mon <= 12 )
		? std::string(month_abbrev[mon-1])
		: std::string("INVALID_MONTH") );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const int XDATE::getDay( void ) const
{
	return( dateValid ? dy : XTIME::invalid );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XDATE &XDATE::operator=( const XDATE &d )
{
	set( d );
	return( *this );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XDATE::compare( const XDATE *a, const XDATE *b )
{
	int	y_dif = a->yr - b->yr;
	if ( y_dif != 0 )
		{return( y_dif );
		}
	int	m_dif = a->mon - b->mon;
	if ( m_dif != 0 )
		{return( m_dif );
		}
	return( a->dy - b->dy );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
struct tm XDATE::asStructTm( void ) const
{
	struct	tm	t;
	t.tm_sec  = 0;
	t.tm_min  = 0;
	t.tm_hour = 0;
	t.tm_mday = getDay();		// DAY 1-31
	t.tm_mon  = getMonth();		// MONTH 0-11
	t.tm_year = getYear() - 1900;	// YEAR SINCE 1900
	t.tm_isdst = -1;		// NOT AVAILABLE
	return( t );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
time_t XDATE::asTimeT( void ) const
{
	if ( ! isValid() )
		{return( -1 );
		}
	struct	tm	t = asStructTm();
	return( mktime( &t ) );	// RETURNS -1 IF CONVERSION FAILS
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XDATE::daysSince( const XDATE &d ) const
{
	time_t	a = asTimeT();
	time_t	b = d.asTimeT();
	if ( -1 == a || -1 == b )
		{return( 0 );
		}
	double	secs = difftime( a, b );	// ALLOW FOR DAYLIGHT SAVING
	return( (int) floor( ( secs + 43200 ) / 86400 ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XDATE::getAge( void ) const
{					// RETURN WHOLE-YEARS, ROUNDED DOWN
	XDATE   dnow;
	dnow.setNow();
	if ( ! dateValid || ! dnow.isValid() || *this > dnow )
		{return( -1 );
		}
	int	age = dnow.getYear() - yr;
	if ( dnow.getMonth() < mon || (dnow.getMonth() == mon
			&& dnow.getDay() < dy) )
		{age--;
		}
	return( age );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDATE::operator==( const XDATE &d ) const
{
	return( 0 == compare( this, &d ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDATE::operator!=( const XDATE &d ) const
{
	return( 0 != compare( this, &d ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDATE::operator<( const XDATE &d ) const
{
	return( compare( this, &d ) < 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDATE::operator>( const XDATE &d ) const
{
	return( compare( this, &d ) > 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDATE::operator<=( const XDATE &d ) const
{
	return( compare( this, &d ) <= 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDATE::operator>=( const XDATE &d ) const
{
	return( compare( this, &d ) >= 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDATE::subtract( const int nYears, const int nDays )
{
	if ( nYears < 0 || nDays < 0 || ! isValid() )
		{return( false );
		}
	bool	again = true;
	int	days_in_month;
	yr -= nYears;
	dy -= nDays;
	do
		{
		if ( dy < 1 )
			{
			mon--;
			if ( mon < 1 )
				{
				yr--;
				mon += 12;
				}
			days_in_month = daysInMonth( yr, mon );
			dy += days_in_month;
			}
		else
			{again = false;
			}
		}
		while ( again );
						/* Adjust for leap year */
	if ( dy == 29 && mon == 2 && daysInMonth( yr, mon ) == 28 )
		{dy = 28;
		}
	return( determineValidity() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDATE::add( const int nYears, const int nDays )
{
	if ( nYears < 0 || nDays < 0 || ! isValid() )
		{return( false );
		}
	bool	again = true;
	int	days_in_month;
	yr += nYears;
	dy += nDays;
	do
		{while ( mon > 12 )
			{yr++;
			mon -= 12;
			}
		days_in_month = daysInMonth( yr, mon );
		if ( dy > days_in_month )
			{dy -= days_in_month;
			mon++;
			}
		else
			{again = false;
			}
		}
		while ( again );
	return( determineValidity() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDATE::add( const int nDays )
{
	return( add( 0, nDays ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XDATE::operator+=( const int nDays )
{
	add( 0, nDays );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string XDATE::iso( void ) const
{
	if ( ! dateValid )
		{return( XTIME::invalid_serialization );
		}
	char	buf[20];
	sprintf( buf, "%4.4d-%2.2d-%2.2d", getYear(), getMonth(), getDay() );
	return( std::string( buf ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string XDATE::dmy( const int format ) const
{
	if ( ! dateValid )
		{return( XTIME::invalid_serialization );
		}
	char	buf[50];
	char const      *fmt = "%d/%d/%d";
	switch( format )
		{
		case 1:
			fmt = "%2.2d/%2.2d/%4.4d";
			break;
		default:
			break;
		}
	sprintf( buf, fmt, getDay(), getMonth(), getYear() );
	return( std::string( buf ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XTIME XDATE::asXTIME( void ) const
{
	return( XTIME( *this ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#ifdef __BORLANDC__                         
#define	SINCMP	strnicmp
#else
#define	SINCMP	strncasecmp
#endif
int XDATE::monthNameToNumber( const std::string name )
{
	int	nlen = name.size();
	if ( nlen < 3 )	// REQUIRE AT LEAST 3 CHARACTERS FOR A MATCH
		{return( -1 );
		}
	const	char	*c = name.c_str();
	int	i;
	for ( i = 0; i < 12; i++ )
		{if ( 0 == SINCMP( c, month_name[i], nlen ) )
			{return( i+1 );
			}
		}
	return( -1 );	// NO MATCH
}
#undef	SINCMP
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
				/* EXTENSIONS FOR C++BUILDER ONLY */
bool XDATE::inputTDateTime( TDateTime dt )
{
	unsigned short dt_year, dt_month, dt_day;
	dt.DecodeDate( &dt_year, &dt_month, &dt_day );
	return( set( dt_year, dt_month, dt_day ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TDateTime XDATE::outputTDateTime( void ) const
{
	if ( ! isValid() )
		{return( 0 );
		}
	return( TDateTime( (unsigned short) getYear(),
		(unsigned short) getMonth(), (unsigned short) getDay() ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XDATE::XDATE( TDateTime dt )
{
	inputTDateTime( dt );
}
#endif
/*===========================================================================*/
XCLOCK::XCLOCK( void )
{
	setNow();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XCLOCK::XCLOCK( const int hour, const int minute, const int second )
	:
	hr( hour),
	min( minute ),
	sec( second ),
	ms( 0 )
{
	determineValidity();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XCLOCK::XCLOCK( const XCLOCK &c )
	:
	hr( c.hr ),
	min( c.min ),
	sec( c.sec ),
	ms( c.ms ),
	clockValid( c.clockValid )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XCLOCK::XCLOCK( const char *s )		// LOAD SYMBOLIC OR ISO STRING
{
	set( s );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XCLOCK::~XCLOCK( void )
{
}
//---------------------------------------------------------------------------
bool XCLOCK::determineValidity( void )
{
	clockValid = ( hr >= 0 && hr < 24
		&& min >= 0 && min < 60
		&& sec >= 0 && sec < 60
		&& ms  >= 0 && ms < 1000 );
	return( clockValid );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XCLOCK::isValid( void ) const
{
	return( clockValid );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XCLOCK::set( const int hour, const int minute, const int second, const int msec )
{
	hr  = hour;
	min = minute;
	sec = second;
	ms  = msec;
	return( determineValidity() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XCLOCK::set( const int hour, const int minute, const int second )
{	return( set( hour, minute, second, 0 ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XCLOCK::set( const XCLOCK &c )
{
	return( set( c.hr, c.min, c.sec, c.ms ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/* SET FROM STRING, ISO FORMAT OR H[H]:M[M][:S[S]] */
bool XCLOCK::set( const char *cs )
{
	int	h = XTIME::invalid, m = XTIME::invalid, s = XTIME::invalid;
	if ( mask_match( "12:12:12", cs ) < 0 )
		{
		h = get_int( cs,   2 );
		m = get_int( cs+3, 2 );
		s = get_int( cs+6, 2 );
		}
	return( set( h, m, s ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XCLOCK::set( const time_t ct )
{
	struct tm *c = localtime( &ct );		// NOT THREAD SAVE
	return( set( c->tm_hour, c->tm_min, c->tm_sec ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XCLOCK::setNow( void )
{
	time_t	tnow;
	time( &tnow );
	return(	set( tnow ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const int XCLOCK::getHour( void ) const
{
	return( clockValid ? hr : XTIME::invalid );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const int XCLOCK::getMinute( void ) const
{
	return( clockValid ? min : XTIME::invalid );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const int XCLOCK::getSecond( void ) const
{
	return( clockValid ? sec : XTIME::invalid );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const int XCLOCK::getMilliSecond( void ) const
{
	return( clockValid ? ms : XTIME::invalid );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XCLOCK &XCLOCK::operator=( const XCLOCK &c )
{
	set( c );
	return( *this );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XCLOCK::compare( const XCLOCK *a, const XCLOCK *b )
{
	int	h_dif = a->hr - b->hr;
	if ( h_dif != 0 )
		{return( h_dif );
		}
	int	m_dif = a->min - b->min;
	if ( m_dif != 0 )
		{return( m_dif );
		}
	return( a->sec - b->sec );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XCLOCK::operator==( const XCLOCK &c ) const
{	return( 0 == compare( this, &c ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XCLOCK::operator!=( const XCLOCK &c ) const
{	return( 0 != compare( this, &c ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XCLOCK::operator<( const XCLOCK &c ) const
{	return( compare( this, &c ) < 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XCLOCK::operator>( const XCLOCK &c ) const
{	return( compare( this, &c ) > 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XCLOCK::operator<=( const XCLOCK &c ) const
{	return( compare( this, &c ) <= 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XCLOCK::operator>=( const XCLOCK &c ) const
{	return( compare( this, &c ) >= 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
						/* ADD RAW PERIOD TO XCLOCK */
bool XCLOCK::add( int h, int m, int s, int *day_overflow )
{
	if ( h < 0 || m < 0 || s < 0 || ! clockValid )
		{return( false );
		}
	sec += s;
	int	xs = sec / 60;
	sec -= ( 60 * xs );
	min += ( xs + m  );
	int	xm  = min / 60;
	min -= ( xm * 60 );
	hr +=  ( xm + h  );
	*day_overflow = ( hr / 24 );
	hr -= ( (*day_overflow) * 24 );
	return( determineValidity() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
					/* SUBTRACT RAW XTIME FROM XCLOCK */
bool XCLOCK::subtract( int h, int m, int s, int *day_underflow )
{
	*day_underflow = 0;
	if ( h < 0 || m < 0 || s < 0 || ! clockValid )
		{return( false );
		}
	while ( sec < s )
		{sec += 60;
		m++;
		}
	sec -= s;
	while ( min < m )
		{min += 60;
		h++;
		}
	min -= m;
	while ( hr < h )
		{hr += 24;
		(*day_underflow)++;
		}
	hr -= h;
	return( determineValidity() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string XCLOCK::iso( void ) const
{
	if ( ! clockValid )
		{return( XTIME::invalid_serialization );
		}
	char	buf[10];
	sprintf( buf, "%2.2d:%2.2d:%2.2d", getHour(), getMinute(), getSecond() );
	return( std::string( buf ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string XCLOCK::hms( const int format ) const
{
	if ( ! clockValid )
		{return( XTIME::invalid_serialization );
		}
	char	buf[50];
	sprintf( buf, "%2.2d:%2.2d", getHour(), getMinute(), getSecond() );
	if ( format > 0 )
		{
		char	sec[20];
		sprintf( sec, ":%2.2d", getSecond() );
		strcat( buf, sec );
		}
	return( std::string( buf ) );
}
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
				/* EXTENSIONS FOR C++BUILDER ONLY */
bool XCLOCK::inputTDateTime( TDateTime dt, bool include_msec )
{
	unsigned short dt_hour, dt_min, dt_sec, dt_msec;
	dt.DecodeTime( &dt_hour, &dt_min, &dt_sec, &dt_msec );
	return( set( dt_hour, dt_min, dt_sec,
		include_msec ? dt_msec : 0 ) );
}
#endif
/*===========================================================================*/
bool XTIME::use_exceptions = true;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XTIME::useExceptions( const bool ue )
{	use_exceptions = ue;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XTIME::usingExceptions( void )
{	return( use_exceptions );
}
//---------------------------------------------------------------------------
XTIME::XTIME( void )
	:
	XDATE( XTIME::invalid, XTIME::invalid, XTIME::invalid ),
	XCLOCK( XTIME::invalid, XTIME::invalid, XTIME::invalid )
{
	setNow();		// SET AS CURRENT TIME
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XTIME::XTIME( const int year, const int month, const int day,
	const int hour, const int min, const int sec )
	:
	XDATE( year, month, day ),
	XCLOCK( hour, min, sec )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XTIME::XTIME( const XTIME &t )
	:
	XDATE( t.getYear(), t.getMonth(), t.getDay() ),
	XCLOCK( t.getHour(), t.getMinute(), t.getSecond() )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XTIME::XTIME( const XDATE &d )
	:
	XDATE( d.getYear(), d.getMonth(), d.getDay() ),
	XCLOCK( 0, 0, 0 )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XTIME::XTIME( const char *s )		// LOAD SYMBOLIC OR ISO STRING
{
	set( s );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XTIME::~XTIME( void )
{
}
//---------------------------------------------------------------------------
void XTIME::error( const std::string emsg )
{
	if ( usingExceptions() )
		{throw( std::string( "XTIME exception: ") + emsg );
		}
}
//---------------------------------------------------------------------------
bool XTIME::isValid( void ) const
{	return( XDATE::isValid() && XCLOCK::isValid() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XTIME::set( const int year, const int month, const int day,
	const int hour, const int min, const int sec )
{
	XDATE::set( year, month, day );
	XCLOCK::set( hour, min, sec );
	return( isValid() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XTIME::set( const XTIME &t )
{
	XDATE::set(  (XDATE)  t );
	XCLOCK::set( (XCLOCK) t );
	return( isValid() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	/* SET FROM STRING - ISO FORMAT yyyy-mm-ddThh:mm:ss ONLY */
bool XTIME::set( const char *ts )
{
	if ( NULL == ts )
		{XDATE::dateValid = false;
		XCLOCK::clockValid = false;
		}
	else if ( strlen( ts ) != 19 || toupper( ts[10] ) != 'T' )
		{XDATE::dateValid = false;
		XCLOCK::clockValid = false;
		}
	else
		{
		char	date_buf[12];
		strncpy( date_buf, ts, 10 );
		date_buf[10] = 0;
		XDATE::set( date_buf );
		XCLOCK::set( ts+11 );
		}
	return( isValid() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XTIME::set( const time_t tt )
{
	struct tm *t = localtime( &tt );		// NOT THREAD SAVE
	if ( NULL == t )
		{		// CAN HAPPEN FOR INVALID INPUT TT
		XDATE::dateValid = false;
		XCLOCK::clockValid = false;
		return( false );
		}
	return( set( t->tm_year + 1900, t->tm_mon + 1, t->tm_mday,
		t->tm_hour, t->tm_min, t->tm_sec ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XTIME::setNow( void )
{
	time_t	tnow;
	time( &tnow );
	return(	set( tnow ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XTIME &XTIME::operator=( const XTIME &t )
{
	if ( &t != this )
		{set( t );
		}
	return( *this );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
					/* RETURN -1,0,1 ACCORDING TO VALUES */
int XTIME::compare( const XTIME *a, const XTIME *b )
{
	int	c = XDATE::compare( (const XDATE *) a, (const XDATE *) b );
	return( ( c != 0 ) ? c
		: XCLOCK::compare( (const XCLOCK *) a, (const XCLOCK *) b ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XTIME::operator==( const XTIME &t ) const
{	return( 0 == compare( this, &t ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XTIME::operator!=( const XTIME &t ) const
{	return( 0 != compare( this, &t ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XTIME::operator<( const XTIME &t ) const
{	return( compare( this, &t ) < 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XTIME::operator>( const XTIME &t ) const
{	return( compare( this, &t ) > 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XTIME::operator<=( const XTIME &t ) const
{	return( compare( this, &t ) <= 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XTIME::operator>=( const XTIME &t ) const
{	return( compare( this, &t ) >= 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XTIME::add( const int nYears, const int nDays, const int nHours,
			const int nMinutes, const int nSeconds )
{
	int	day_overflow;
	if ( XCLOCK::add( nHours, nMinutes, nSeconds, &day_overflow ) )
		{return( XDATE::add( nYears, nDays + day_overflow ) );
		}
	else
		{return( false );
		}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string XTIME::iso( void ) const
{
	if ( ! isValid() )
		{return( XTIME::invalid_serialization );
		}
	return( XDATE::iso() + "T" + XCLOCK::iso() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XDATE XTIME::asXDATE( void ) const
{
	return( XDATE( *this ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XTIME::GMT2Local( void )
{
	struct  tm parts;
	if ( ! isValid() )
		{return( false );   	// ONLY CONVERT IF DATE+CLOCK ALL OK
		}
	if ( yr < 1970 || yr > 2037 )
		{return( false );// N.B. INGRES CONVERTS 1960-2037, C WORKS ONLY AFTER 1970
		}
	parts.tm_year = getYear() - 1900;
	parts.tm_mon  = getMonth() - 1;
	parts.tm_mday = getDay();
	parts.tm_hour = getHour();
	parts.tm_min  = getMinute();
	parts.tm_sec  = getSecond();
	parts.tm_isdst = 0;             	// DAYLIGHT SAVING=NO FOR GMT
	static	char	tz[10] = "TZ=GMT";
	putenv( tz );
#ifdef __BORLANDC__
	mktime( &parts );
	if ( 0 == parts.tm_isdst )
		{return( false );		// NO TIME CONVERSION REQUIRED
		}
	add( 0, 0, 1, 0, 0 );
	return( true );
#else
	time_t	whole = mktime( &parts );
	unsetenv( "TZ" );
	if ( 0 == whole )
		{return( false );
		}
	struct tm *p = localtime( &whole );
	if ( p->tm_hour != parts.tm_hour || p->tm_min != parts.tm_min )
		{
		set( p->tm_year + 1900, p->tm_mon + 1, p->tm_mday,
			p->tm_hour, p->tm_min, p->tm_sec );
		return( true );    // INDICATE CONVERSION HAS CHANGED XTIME
		}
	return( false );
#endif
}
//---------------------------------------------------------------------------
				/* PERFORM BASIC INTERNAL TEST OF OPERATIONS */
int XTIME::selfTest( void )
{
	XTIME	jul31p( 2000, 7, 31, 23, 59, 59 );
	XTIME	aug01a( 2000, 8, 1, 0, 59, 59 );
	XTIME	dec31a( 2000, 12, 31, 0, 0, 1 );
	XTIME	later(  2004, 4, 24, 21, 19, 14 );
	jul31p.GMT2Local();		// TIME-SHIFT SHOULD BE +1 HOUR IN UK
	if ( jul31p.getMonth() != 8 || jul31p.getDay() != 1
			|| jul31p.getHour() != 0 || aug01a != jul31p )
		{
		// printf( "\n%s", jul31p.iso().c_str() );
		return( 1 );		// TIME-SHIFT ERROR
		}
	XTIME	tmp( dec31a );
	dec31a.GMT2Local();     	// TIME-SHIFT SHOULD BE ZERO
	if ( tmp != dec31a )
		{return( 2 );
		}
	dec31a.add( 2, 477, 91, 138, 73 ); // + 2YR, 477DY, 91HR, 138MI, 73SC
	if ( later != dec31a )
		{
//		printf( "\n%s", dec31a.iso().c_str() );
		return( 3 );
		}
	return( 0 );
}
//---------------------------------------------------------------------------
#ifdef __BORLANDC__
				/* EXTENSIONS FOR C++BUILDER ONLY */
XTIME::XTIME( TDateTime dt )
	:
	XDATE( XTIME::invalid, XTIME::invalid, XTIME::invalid ),
	XCLOCK( XTIME::invalid, XTIME::invalid, XTIME::invalid )
{
	inputTDateTime( dt, true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XTIME::inputTDateTime( TDateTime dt, bool include_msec )
{
	return( XDATE::inputTDateTime( dt )
		&& XCLOCK::inputTDateTime( dt, include_msec ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TDateTime XTIME::outputTDateTime( void ) const
{
	if ( ! isValid() )
		{return( 0 );
		}
	return( XDATE::outputTDateTime()
		+ TDateTime( (unsigned short) getHour(),
			(unsigned short) getMinute(),
			(unsigned short) getSecond(),
			(unsigned short) getMilliSecond() ) );
}
#endif
/*===========================================================================*/

