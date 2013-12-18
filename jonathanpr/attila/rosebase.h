#ifndef rosebaseH
#define rosebaseH
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include <set>
#include "xtime.h"
#include "xbinob.h"
#include "rosetta.h"
//---------------------------------------------------------------------------
class ROSETTA;
//---------------------------------------------------------------------------
class ROSE_BASE
{
friend	class 	ROSETTA;
private:
	int 	unhex( const char *hx ) const;
	void 	tohex( char c, char *hx ) const;
	std::set<std::string>	*tags;
protected:
	std::string	id;
	bool	valid;
	ROSE_BASE( const ROSETTA *owner, const std::string name );
	virtual	~ROSE_BASE( void );
	const	ROSETTA	*owner_rosetta;
	const	std::set<std::string>	*tagPtr( void ) const;
	virtual	bool	warning( const int ecode, const std::string emsg ) const;
	virtual	bool	error(   const int ecode, const std::string emsg ) const;
	virtual	void 	errorSerializeIn( const char *in ) const;
public:
	bool 	isValid( void ) const;
	std::string 	getName( void ) const;
	bool	hasTag( const std::string tag_name ) const;
	bool	attachTag( const std::string tag_name );
	bool	attachTagSet( const std::set<std::string> *tagset );
	bool	removeTag( const std::string tag_name );
	bool	rename( const std::string new_name );
	int	compareName( const ROSE_BASE *b ) const;
	bool	lessThan( const ROSE_BASE *b ) const;
	bool	greaterThan( const ROSE_BASE *b ) const;
	virtual	void	sort( void );
					// MUST BE PROVIDED BY BASE CLASS
	virtual	int	type( void ) const = 0;
	virtual	bool	equalValue( const ROSE_BASE *b ) const = 0;
	virtual	ROSE_BASE *copy( const ROSETTA *owner ) const = 0;
	virtual	std::string	serializeOut( void ) const;
	virtual	std::string	serializeOutValue( void ) const = 0;
	virtual	std::string 	exportXML( void ) const;
	virtual	std::string 	exportFlatXML( const std::string prefix ) const;
	virtual	std::string 	exportXMLValue( void ) const;
	virtual	bool	serializeInValue( const char *in ) = 0;
					// SETTERS
	virtual bool	setFlag( void );
	virtual	bool 	setInt(  const int value );
	virtual	bool 	setLint( const LINT value );
	virtual	bool 	setBool( const bool value );
	virtual	bool 	setReal( const double value );
	virtual	bool 	setString( const std::string value );
	virtual	bool	setTime( const XTIME value );
	virtual	bool 	setRosetta( const ROSETTA *value );
		bool	setRosetta( const ROSETTA &value );
	bool	operator=( const int    value );
	bool	operator=( const bool   value );
	bool	operator=( const double value );
	bool	operator=( const std::string value );
	bool	operator=( const XTIME  value );
	bool	operator=( const void * value );
	bool	operator=( const ROSETTA &value );
					// GETTERS
	virtual	int	getInt(   void ) const;
	virtual	LINT getLint(void ) const;
	virtual	bool	getBool(  void ) const;
	virtual	double	getReal(  void ) const;
	virtual	std::string	getString(void ) const = 0;
	virtual XDATE	getDate(  void ) const;
	virtual XTIME	getTime(  void ) const;
	virtual XBINOB	getBinob(  void ) const;
	virtual	ROSETTA *getRosetta(void ) const;
						// UTILITIES
	static	bool 	isAllowableName( const std::string nm );
	static	bool 	isPlainCharacter( const char c );
	bool 	decodeURL( char *txt ) const;
	void 	encodeURL( const char *in, char *out ) const;
};
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ROSE_FLAG : public ROSE_BASE
{
private:
public:
	ROSE_FLAG( const ROSETTA *owner, const std::string name );
	virtual	~ROSE_FLAG( void );
	virtual	int	type( void ) const;
	virtual	ROSE_BASE *copy( const ROSETTA *owner ) const;
	bool 	equalValue( const ROSE_BASE *b ) const;
	std::string 	getString( void ) const;		
	virtual	std::string	serializeOutValue( void ) const;
	virtual	bool	serializeInValue( const char *in );
};
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/* BASE-TYPE TO STORE PARAMETERS OF ORDERED VALUES */
template <class T> class ROSE_COMMON : public ROSE_BASE
{
friend class ROSETTA;
private:
protected:
	ROSE_COMMON( const ROSETTA *owner, const std::string name, const T value );
	virtual	~ROSE_COMMON( void );
	T	val;
	void	set_native( const T value );
	T 	get_native( void ) const;
	virtual	bool	equalValue( const ROSE_BASE *b ) const;
public:
};
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ROSE_INT : public ROSE_COMMON<int>
{
private:
public:
	ROSE_INT( const ROSETTA *owner, const std::string name, const int value );
	virtual	~ROSE_INT( void );
	virtual	int	type( void ) const;
	virtual	bool	setInt( const int value );
	virtual	int	getInt(   void ) const;
	virtual	LINT	getLint(  void ) const;
	virtual	double	getReal(  void ) const;
	virtual	std::string	getString( void ) const;
	virtual	ROSE_BASE *copy( const ROSETTA *owner ) const;
	virtual	std::string	serializeOutValue( void ) const;
	virtual	bool	serializeInValue( const char *in );
};
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ROSE_LINT : public ROSE_COMMON<LINT>
{
private:
public:
	ROSE_LINT( const ROSETTA *owner, const std::string name,
		const LINT value );
	virtual	~ROSE_LINT( void );
	virtual	int	type( void ) const;
	virtual	bool	setLint( const LINT value );
	virtual	LINT	getLint( void ) const;
	virtual	double	getReal( void ) const;
	virtual	std::string	getString( void ) const;
	virtual	ROSE_BASE *copy( const ROSETTA *owner ) const;
	virtual	std::string	serializeOutValue( void ) const;
	virtual	bool	serializeInValue( const char *in );
};
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ROSE_BOOL : public ROSE_COMMON<bool>
{
private:
public:
	ROSE_BOOL( const ROSETTA *owner, const std::string name, const bool value );
	virtual	~ROSE_BOOL( void );
	virtual	int	type( void ) const;
	virtual	bool	setBool( const bool value );
	virtual	bool	getBool( void ) const;
	virtual	int	getInt(  void ) const;
	virtual	std::string	getString( void ) const;
	virtual	ROSE_BASE *copy( const ROSETTA *owner ) const;
	virtual	std::string	serializeOutValue( void ) const;
	virtual	bool	serializeInValue( const char *in );
};
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ROSE_REAL : public ROSE_COMMON<double>
{
private:
public:
	ROSE_REAL( const ROSETTA *owner, const std::string name, const double value );
	virtual	~ROSE_REAL( void );
	virtual	int	type( void ) const;
	virtual	bool	setReal( const double value );
	virtual	double	getReal( void ) const;
	virtual	std::string	getString( void ) const;
	virtual	ROSE_BASE *copy( const ROSETTA *owner ) const;
	virtual	std::string	serializeOutValue( void ) const;
	virtual	bool	serializeInValue( const char *in );
};
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ROSE_STRING : public ROSE_COMMON<std::string>
{
private:
public:
	ROSE_STRING( const ROSETTA *owner, const std::string name,
		const std::string value );
	virtual	~ROSE_STRING( void );
	virtual	int	type( void ) const;
	virtual	bool	setString( const std::string value );
	virtual	std::string	getString( void ) const;
	virtual	const	std::string	*pointerString( void ) const;
	virtual	ROSE_BASE *copy( const ROSETTA *owner ) const;
	virtual	std::string	serializeOutValue( void ) const;
	virtual	std::string 	exportXMLValue( void ) const;
	virtual	bool	serializeInValue( const char *in );
};
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ROSE_ROSETTA : public ROSE_COMMON<ROSETTA *>
{
friend	class	ROSIG;
friend	class 	ROSETTA;
private:
	ROSE_ROSETTA( const ROSETTA *owner, const std::string name );
public:
	ROSE_ROSETTA( const ROSETTA *owner, const std::string name,
		const ROSETTA * value );
	virtual	~ROSE_ROSETTA( void );
	virtual	int	type( void ) const;
	virtual	bool	setRosetta( const ROSETTA * value );
	virtual	std::string	getString( void ) const;
	virtual	ROSETTA *getRosetta( void ) const;
	virtual	void	sort( void );
	virtual	ROSE_BASE *copy( const ROSETTA *owner ) const;
	virtual	std::string 	exportXML( void ) const;
	virtual	std::string 	exportFlatXML( const std::string prefix ) const;
	virtual	std::string	serializeOutValue( void ) const;
	virtual	bool	serializeInValue( const char *in );
	virtual	bool 	equalValue( const ROSE_BASE *b ) const;
};
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ROSE_XDATE : public ROSE_COMMON<XDATE>
{
private:
public:
	ROSE_XDATE( const ROSETTA *owner, const std::string name,
		const XDATE value );
	virtual	~ROSE_XDATE( void );
	virtual	int	type( void ) const;
	virtual	bool	setDate( const XDATE value );
	virtual	std::string	getString( void ) const;
	virtual	XDATE 	getDate( void ) const;
	virtual	XTIME 	getTime( void ) const;
	virtual	ROSE_BASE *copy( const ROSETTA *owner ) const;
	virtual	std::string	serializeOutValue( void ) const;
	virtual	bool	serializeInValue( const char *in );
};
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ROSE_XTIME : public ROSE_COMMON<XTIME>
{
private:
	static	bool allow_deserialize_invalid;
public:
	ROSE_XTIME( const ROSETTA *owner, const std::string name,
		const XTIME value );
	static	void	setAllowDeserializeInvalid( const bool adi );
	static	bool 	getAllowDeserializeInvalid( void );
	virtual	~ROSE_XTIME( void );
	virtual	int	type( void ) const;
	virtual	bool	setTime( const XTIME value );
	virtual	std::string	getString( void ) const;
	virtual	XDATE 	getDate( void ) const;
	virtual	XTIME 	getTime( void ) const;
	virtual	ROSE_BASE *copy( const ROSETTA *owner ) const;
	virtual	std::string	serializeOutValue( void ) const;
	virtual	bool	serializeInValue( const char *in );
};
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ROSE_XBINOB : public ROSE_COMMON<XBINOB>
{
private:
public:
	ROSE_XBINOB( const ROSETTA *owner, const std::string name,
		const XBINOB value );
	virtual	~ROSE_XBINOB( void );
	virtual	int	type( void ) const;
	virtual	bool	setBinob( const XBINOB value );
	virtual	std::string	getString( void ) const;
	virtual	XBINOB 	getBinob( void ) const;
	virtual	ROSE_BASE *copy( const ROSETTA *owner ) const;
	virtual	std::string	serializeOutValue( void ) const;
	virtual	bool	serializeInValue( const char *in );
};
//---------------------------------------------------------------------------
#endif

