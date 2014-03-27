#ifndef randutilH
#define randutilH
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <time.h>
#include "xerror.h"
#include "rosetta.h"
//---------------------------------------------------------------------------
class	XCGI;
class	XDB;
class	XQUERY;
//---------------------------------------------------------------------------
class RAND_UTIL : public XERROR
{
private:
	int	id;
//	std::string	token;
	std::string	ip;
	const	std::string	sys;
	const	bool	is_live;
	time_t	tstart;
	clock_t	cstart;
	XDB	*tdb;
	XCGI	*c;
	bool	show_detail;
	bool	allowHtml( const char **s, const char *html, std::string *x,
		const bool accept_endtag );
	std::vector<std::string>	ip_priveleged;
	ROSETTA	*staff, *priv, *token;
	static	const	int	permissable_consecutive_failures;
protected:
	static	const	std::string	db_root, dir_root;
	bool 	generateToken( const int si );
	void 	logFailure( XDB *tdb, const std::string un );
public:
	RAND_UTIL( XCGI *cg );
	~RAND_UTIL( void );
	ROSETTA	webprm;
	ROSETTA	data;
	std::string getWebRoot( void ) const;
	int	getId( void );
	bool	isLive( void );
	std::string 	getComputerName( void );
	std::string 	checkAlignment( void );
	XCGI 	*getCGI( void ) const;
	bool 	showDetail( void );
	XDB 	*openDB( const std::string root );
	XDB 	*getTrialsDB( void );
	static	void 	errorMsg( const std::string etxt );
	static	bool 	dbErr( const std::string caller, const int instance,
		const int ecount, const int ecode, const std::string etxt );
	static	std::string 	prettyInt( int n );
	std::string 	determineCallerIP( void );
	bool 	privelegedIP( void );
	bool 	approvedIP( void );
	std::string 	whoAmI( void ) const;
	void 	showErrorHTML( const std::string msg, const std::string err );
	bool	hasToken( void );
	int 	createToken( const std::string username );
	std::string 	getTokenString( void );
	std::string 	getTokenUsername( void );
	bool 	invalidateToken( void );
	bool 	outputFormToken( void );
	bool	outputData( void ) const;
	int 	getTokenSessionID( void );
	std::string	setStaffUser( const std::string uap );
	std::string 	getStaffUsername( void );
	std::string 	getStaffFullname( void );
	bool	hasPriv( const std::string pr );
	bool	writeLog( XDB *db, const int severity, const std::string txt );
	void 	htmlBeginDoc( const std::string title,
		const std::vector<std::string> style_sheets );
	void 	htmlBeginDocControl( const std::string title );
	void 	htmlBeginBody( void );
	void 	htmlEndDoc( void );
	void 	htmlEndBody( void );
	static	std::string 	calcSecureMD5( const std::string unam,
		const std::string pswd );
	static	std::string 	permittedPW( const std::string pw );
	static	const	std::string	privUserManager;
	static	const	std::string	privUserViewer;
	static	const	std::string	privMenuManager;
	static	const	std::string	privMenuViewer;
	static	const	std::string	privControlManager;
	static	const	int	LogSeverityInfo    = 0;
	static	const	int	LogSeverityWarning = 1;
	static	const	int	LogSeverityError   = 2;
	static	const	int	LogSeverityBug     = 3;
	static	const	int	LogSeverityFatal   = 9;
};
//---------------------------------------------------------------------------
#endif

