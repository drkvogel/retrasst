#ifndef xbinobH
#define xbinobH
#include <stdio.h>
#include <string>
#include "xerror.h"
/* OBJECT TO SIMPLIFY HANDLING BINARY DATA */
//---------------------------------------------------------------------------
class XBV;
//---------------------------------------------------------------------------
class XBV
{
private:
	int	ref_count;
	int	nbytes;			// -1 IF DATA IS NOT VALID
	unsigned char *value;
	~XBV( void );
public:
	XBV( void );
	XBV( int nb, unsigned char *dat );
	XBV( XBV *p );
	int	getRefCount( void ) const;
	int	size( void ) const;
	const 	unsigned char *data( void ) const;
	bool 	append( const int input_size, const unsigned char *data );
	static	XBV  *acquire( XBV *p );
	static	void relinquish( XBV **p );
	static	bool 	equivalent( const XBV *pa, const XBV *pb );
};
//---------------------------------------------------------------------------
class XBINOB : public XERROR
{
private:
	mutable	XBV	*payload;
protected:
public:
	XBINOB( void );
	XBINOB( const int input_size, const unsigned char *data );
	XBINOB( const XBINOB &b );			// COPY CONSTRUCTOR
	virtual	~XBINOB( void );
	virtual	void 	setInvalid( const std::string emsg ) const;
	void	clear( void );
	void	makeUnique( void );
	bool 	acquire( const int input_size, unsigned char *data );
	bool	insert( const int input_size, const unsigned char *data );
	bool	insert( const XBINOB &b );
	bool	append( const int input_size, const unsigned char *data );
	bool	append( const XBINOB &b );
	bool	extract( int *output_size, unsigned char **data ) const;
	const unsigned char *data( void ) const;
	std::string	extractHex( void ) const;
	std::string	extractBase64( void ) const;
	bool 	extractFile( FILE *f ) const; 		// FOPEN AS "wb"
	bool 	insertHex( const std::string hex );
	bool 	insertBase64( const std::string b64 );
	bool	insertFile( FILE *f );			// FOPEN AS "rb"
	int	size( void ) const;
	bool 	empty( void ) const;
	XBINOB	&operator=( const XBINOB &b );
	bool	isEqual( const XBINOB &b ) const;
	bool	operator==(const XBINOB &b ) const;
	bool	operator!=(const XBINOB &b ) const;
};
//---------------------------------------------------------------------------
#endif

