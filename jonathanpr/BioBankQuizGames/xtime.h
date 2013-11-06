#ifndef xtimeH
#define xtimeH
/*===========================================================================*/
#include <string>
#include <time.h>
#ifdef __BORLANDC__
#include <system.hpp>			// BORLAND TDateTime
#endif
//---------------------------------------------------------------------------
class XCLOCK;
class XTIME;
//---------------------------------------------------------------------------
class XDATE
{
private:
protected:
	static	const 	int days_norm[13];
	static	const 	int dcum_norm[13];
	static	const 	int days_leap[13];
	static	const 	int dcum_leap[13];
	bool	dateValid;
	int	yr, mon, dy;
	int	*days_year;
	int	*dcum_year;
	bool	determineValidity( void );
public:
	static	const 	char	*month_name[13];
	static	const 	char	*month_abbrev[13];
	static	const	int	year_min_valid;
	static	const	int	year_max_valid;
	XDATE( const int year, const int month, const int day );
	XDATE( const XDATE &d );
	XDATE( const XTIME &t );
	XDATE( const char *s );
	XDATE( void );
	virtual ~XDATE( void );
	virtual	bool	isValid( void ) const;
	bool	set( const int year, const int month, const int day );
	bool	set( const XDATE &d );
	bool	set( const char *ds );
	bool 	set( const time_t dt );
	bool	setNow( void );
	static	bool 	isLeapYear( int nyear );
	static	int 	daysInMonth( const int year, const int month );
	static	int 	daysInYear( const int year );	
	bool	isLeapYear( void ) const;
	const	int	getYear( void )	 const;
	const	int	getMonth( void ) const;
	const	std::string	getMonthName( void ) const;
	const   std::string     getMonthAbbrev( void ) const;
	const   int     getDay( void )  const;
	bool    subtract( const int nYears, const int nDays );
	bool	add( const int nYears, const int nDays );
	bool	add( const int nDays );
	void	operator+=( const int nDays );
	virtual	std::string	iso( void ) const;
	virtual std::string 	dmy( const int format ) const;
	XTIME	asXTIME( void ) const;
	struct	tm	asStructTm( void ) const;
	time_t	asTimeT( void ) const;
	int 	daysSince( const XDATE &d ) const;
	int 	getAge( void ) const;
	XDATE	&operator=( const XDATE &d );
	static	int	compare( const XDATE *a, const XDATE *b );
	bool	operator==( const XDATE &d ) const;
	bool	operator!=( const XDATE &d ) const;
	bool	operator<(  const XDATE &d ) const;
	bool	operator>(  const XDATE &d ) const;
	bool	operator<=( const XDATE &d ) const;
	bool	operator>=( const XDATE &d ) const;
	static	int monthNameToNumber( const std::string name );
#ifdef __BORLANDC__
						/* C++ BUILDER ONLY */
	XDATE( const TDateTime dt );
	bool 	inputTDateTime( const TDateTime dt );
	TDateTime 	outputTDateTime( void ) const;
#endif
};
//---------------------------------------------------------------------------
class XCLOCK
{
private:
protected:
	bool	clockValid;
	int	hr, min, sec, ms; // MS=MILISECOND
	bool	determineValidity( void );
public:
	XCLOCK( void );
	XCLOCK( const int hour, const int minute, const int second );
	XCLOCK( const XCLOCK &c );
	XCLOCK( const char *s );
	virtual ~XCLOCK( void );
	virtual	bool	isValid( void ) const;
	bool	set( const int hour, const int minute, const int second, const int msec );
	bool	set( const int hour, const int minute, const int second );
	bool 	set( const XCLOCK &c );
	bool	set( const char *cs );
	bool 	set( const time_t ct );
	bool	setNow( void );
	const 	int	getHour( void )       const;
	const	int	getMinute( void )     const;
	const	int	getSecond( void )     const;
	const	int	getMilliSecond( void ) const;
	virtual std::string	iso( void ) const;
	virtual	std::string 	hms( const int format ) const;
	XCLOCK	&operator=( const XCLOCK &c );
	static	int	compare( const XCLOCK *a, const XCLOCK *b );
	bool	operator==( const XCLOCK &c ) const;
	bool	operator!=( const XCLOCK &c ) const;
	bool	operator<(  const XCLOCK &c ) const;
	bool	operator>(  const XCLOCK &c ) const;
	bool	operator<=( const XCLOCK &c ) const;
	bool	operator>=( const XCLOCK &c ) const;
	bool 	add( int h, int m, int s, int *day_overflow );
	bool 	subtract( int h, int m, int s, int *day_underflow );
#ifdef __BORLANDC__
						/* C++ BUILDER ONLY */
	bool 	inputTDateTime( const TDateTime dt, bool include_msec );
#endif
};
//---------------------------------------------------------------------------
class XTIME : public XDATE, public XCLOCK
{
friend class XDATE;
friend class XCLOCK;
private:
	static	bool 	use_exceptions;
protected:
	static	void 	error( const std::string emsg );
public:
	XTIME( void );
	XTIME( const int year, const int month, const int day,
			const int hour, const int min, const int sec );
	XTIME( const char *s );
	XTIME( const XTIME &t );
	XTIME( const XDATE &d );
	virtual ~XTIME( void );
	static	void	useExceptions( const bool ue );
	static	bool 	usingExceptions( void );
	virtual	bool	isValid( void ) const;
	bool	set( const int year, const int month, const int day,
			const int hour, const int min, const int sec );
	bool	set( const XTIME &t );
	bool	set( const char *ts );
	bool	set( const time_t tt );
	bool	setNow( void );
	bool	add( const int nYears, const int nDays, const int nHours,
			const int nMinutes, const int nSeconds );
	virtual	std::string	iso( void ) const;
	bool 	GMT2Local( void );
	XDATE	asXDATE( void ) const;
	XTIME	&operator=( const XTIME &t );
	static	int	compare( const XTIME *a, const XTIME *b );
	bool	operator==( const XTIME &t ) const;
	bool	operator!=( const XTIME &t ) const;
	bool	operator<(  const XTIME &t ) const;
	bool	operator>(  const XTIME &t ) const;
	bool	operator<=( const XTIME &t ) const;
	bool	operator>=( const XTIME &t ) const;
	static	const 	int 	invalid;	// VALUE RETURN IF CONTENTS INVALID
	static	const	char	*invalid_serialization;
	static	int 	selfTest( void );
#ifdef __BORLANDC__
						/* C++ BUILDER ONLY */
	XTIME( const TDateTime dt );
	bool 	inputTDateTime( const TDateTime dt, bool include_msec );
	TDateTime 	outputTDateTime( void ) const;
#endif	
};
/*===========================================================================*/
#endif

