#ifndef XCGI_H_INCLUDED
#define XCGI_H_INCLUDED       1
/*===========================================================================*/
/*		UTILITIES FOR CONSTRUCTING CGI SCRIPTS			*/
/*===========================================================================*/
#include "rosetta.h"
#include "xerror.h"
#include <stdio.h>
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
class XCGI : public XERROR
{
private:
	FILE	*flog;
	int	method;
	int 	nextParam( const char *in, const char terminator, char **out );
	int 	findMethod( void );
	void 	splitParams( const char *prm );
	int 	unhex( const char *hx );
	void 	decodeURL( char *txt );
public:
	static	int    	debug;
	static	int	max_param_num;		// CRASH PREVENTION LIMITS
	static	int	max_param_len;
	static	int	max_param_totlen;
	static	char const *logname;
	static	void 	setDebug( const int dbg );      // SET INTERNAL DEBUG VALUE
	ROSETTA	param;
	XCGI( int ac, char **av );
	~XCGI( void );
	int	getDebug( void ) const;
	int	getMethod( void ) const;
	std::string	getMethodName( void ) const;
	bool	paramExists( const std::string pname ) const;
	int 	getParamAsIntDefault( const std::string pname, const int def )const;
	int 	getParamAsInt( const std::string pname ) const;
	std::string 	getParamDefault( const std::string pname, const std::string def ) const;

	std::string setParamIfExistsElseDefault(const std::string pname, const std::string def ) const;

	std::string	getParam( const std::string pname ) const;
	void	flush( void ) const;
	bool 	verbatim( const std::string vfname,
			const bool notify_fail = true );
	static	int 	escapeLength( const char *s, const int maxlen );
	void 	end( void );			// EXIT NICELY
	void 	endFail( const std::string err_txt );	// TERMINATE DODGY REQUEST AND EXIT
	void 	writeHeaderCustom( const std::string mf );
	void 	writeHeader( const int typ );	// WRITE INITIAL MIME HEADER TO OUTPUT
	void 	paramShowall( void );
	bool 	log( const char *txt, ... );    // WRITE INTERNAL MESSAGES
	static	std::string	makeWebSafe( const std::string raw );
	static	std::string	getEnvVar( const std::string name );
						// PARAMETER INPUT METHODS
	static	const	int	methodError 	= 0;
	static	const	int	methodGet 	= 'G';
	static	const	int	methodPost 	= 'P';
	static	const	int	methodString 	= 'C';
	static	const	int	methodFile	= 'F';
	static	const	int	methodUnknown	= 'U';
						// SUPPORTED HEADER MIME-TYPES
	static	const	int	typeUnknown	= 0;
	static	const	int	typeGif		= 'G';
	static	const	int	typeHtml	= 'H';
	static	const	int	typeJpeg	= 'J';
	static	const	int	typePlain	= 'p';
	static	const	int	typePNG		= 'P';
	static	const	int	typeTiff	= 'T';
	static	const	int	typeXmlText	= 'X';
						// ERROR CODE
	static	const	int	invalidInt	= (-999666333);
};
/*===========================================================================*/
#endif                                                  	/* XCGI.H */
