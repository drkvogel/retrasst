#ifndef rosettaH
#define rosettaH
//---------------------------------------------------------------------------
#include <string>
#include <vector>
#include "xtime.h"
#include "xbinob.h"
//---------------------------------------------------------------------------
typedef	long long	LINT;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ROSE_BASE;
class ROSETTA;
class ROSIG;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef	bool (*ROSETTA_FILTER)( const ROSE_BASE * );
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef bool (ROSETTA_ALERT)	// PROTOTYPE FOR WARNING AND ERROR CALLBACKS
	(
	const ROSETTA *,		// ROSETTA OBJECT GENERATING THE ALERT
	const int,			// CODE (ROSETTA::ProblemX) FOR TYPE
	const std::string		// EXPLANATORY TEXT MESSAGE
	);
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSETTA_compare( const ROSE_BASE *a, const ROSE_BASE *b );
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class ROSETTA
{
friend	class	ROSIG;
friend	class	ROSE_BASE;
private:
	mutable	bool	valid;
	bool	case_sensitive;
	bool	serializeInMember( const int typ, const char *nam, char *sval );
	bool 	serializeInSplit( char *c, const bool stop_on_error );
	static	bool	use_exceptions;
protected:
	int	nerrors;
	int	nchanges;
	bool	sorted;
	bool	allow_insert;
	bool	allow_update;
	bool	allow_mutable;
	std::vector<ROSE_BASE *> pairs;
	void	init( void );
	bool 	appendMember( ROSE_BASE *b );
	bool 	isSimpleName( const char *s ) const;
	bool 	checkInsert( void );
	bool 	checkUpdate( void );
	ROSE_BASE *find( const std::string name, const bool warnOnFailure ) const;
	ROSE_BASE *findCompound( const std::string name, bool warnOnFailure ) const;
	bool	setPRet( ROSE_BASE **pb, const std::string name, const int typ );	
	ROSE_BASE *setFlagPRet(   const std::string name );
	ROSE_BASE *setIntPRet(    const std::string name, const int value );
	ROSE_BASE *setLintPRet(    const std::string name, const LINT value );
	ROSE_BASE *setBoolPRet(   const std::string name, const bool value );
	ROSE_BASE *setRealPRet(   const std::string name, const double value );
	ROSE_BASE *setStringPRet( const std::string name, const std::string value );
	ROSE_BASE *setDatePRet(   const std::string name, const XDATE value );
	ROSE_BASE *setTimePRet(   const std::string name, const XTIME value );
	ROSE_BASE *setBinobPRet(  const std::string name, const XBINOB value );
	ROSE_BASE *setRosettaPRet(const std::string name, const ROSETTA * value );
protected:
	virtual	bool	warning( const int code, const std::string emsg ) const;
	virtual	bool	error(   const int code, const std::string emsg ) const;
public:
	ROSETTA( void );
	ROSETTA( const ROSETTA &old );		     	// COPY CONSTRUCTOR
	ROSETTA( const ROSETTA *old );
	ROSETTA( const std::string serial );
	virtual ~ROSETTA( void );
	static	ROSETTA_ALERT	*warningHandler;
	static	ROSETTA_ALERT	*errorHandler;
	static	void	setWarningHandler( ROSETTA_ALERT *handler );
	static	void	setErrorHandler(   ROSETTA_ALERT *handler );
	static	bool	hasWarningHandler( void );
	static	bool	hasErrorHandler(   void );
	void	forceValid( void );
	int 	errorCount( void ) const;
	int 	getNChanges( void ) const;
	bool 	caseSensitive( const bool cs );
	bool 	isCaseSensitive( void ) const;
	int	count( void ) const; 		// NUMBER OF MEMBERS
	bool	isEmpty( void ) const;		// TRUE IFF MEMBERS==0
	bool	isEqual( const ROSETTA *p ) const; // TRUE IFF ALL MEMBERS IDENTICAL
	bool	operator==( const ROSETTA &p ) const;
	bool	operator!=( const ROSETTA &p ) const;
	bool 	exists( const std::string name ) const;
	bool	rename( const std::string from, const std::string to );
	void	sort( void );
	bool	isSorted( void ) const;
	bool	isValid( void ) const;
	bool	canChange( void ) const;
	bool 	canInsert( void ) const;
	bool 	canUpdate( void ) const;
	bool	canMutate( void ) const;
	void	allowChanges( const bool allow );
	void 	allowInsert(  const bool allow );
	void 	allowUpdate(  const bool allow );
	void	allowMutate(  const bool allow );
						// COPY FUNCTIONS
	void	clear( void );
	ROSETTA &operator=( const ROSETTA &p );
	bool 	insert( const ROSETTA *extra );
	bool 	insertMember( const ROSETTA *src, const int indx );
	bool 	insertMember( const ROSETTA *src, const std::string name );
	bool 	insertFiltered( const ROSETTA *extra, const ROSETTA_FILTER filter,
			const bool recurse );
	bool	insertFlag(    const ROSETTA &src, const std::string name );
	bool	insertInt(     const ROSETTA &src, const std::string name );
	bool	insertLint(    const ROSETTA &src, const std::string name );
	bool	insertBool(    const ROSETTA &src, const std::string name );
	bool	insertReal(    const ROSETTA &src, const std::string name );
	bool	insertString(  const ROSETTA &src, const std::string name );
	bool	insertDate(    const ROSETTA &src, const std::string name );
	bool	insertTime(    const ROSETTA &src, const std::string name );
	bool	insertBinob(   const ROSETTA &src, const std::string name );
	bool	insertRosetta( const ROSETTA &src, const std::string name );
						// REMOVER FUNCTIONS
	bool	remove( const std::string name );
						// TAGS
	bool	attachTag( const int index, const std::string tag_name );
	bool	attachTag( const std::string name, const std::string tag_name );
	bool	removeTag( const int index, const std::string tag_name );
	bool	removeTag( const std::string name, const std::string tag_name );
	bool	hasTag( const int index, const std::string tag_name ) const;
	bool	hasTag( const std::string name, const std::string tag_name ) const;
						// SERIALIZATION
	bool	serializeIn( const std::string serial_in,
		const bool stop_on_error = true );
	std::string	serializeOut( void ) const;
	std::string 	exportXML( const std::string name ) const;
	std::string 	exportFlatXML( const std::string label ) const;
						// SETTER FUNCTIONS
	bool	setFlag(   const std::string name );
	bool	setInt(    const std::string name, const int value );
	bool	setLint(    const std::string name, const LINT value );
	bool	setBool(   const std::string name, const bool value );
	bool	setReal(   const std::string name, const double value );
	bool	setString( const std::string name, const std::string value );
	bool	setDate(   const std::string name, const XDATE value );
	bool	setTime(   const std::string name, const XTIME value );
	bool	setBinob(  const std::string name, const XBINOB value );
	bool	setRosetta(const std::string name, const ROSETTA * value );
	bool 	attachSubRosetta( const std::string name, ROSETTA *sub );
						// GET-BY-INDEX FUNCTIONS
	std::string	getName( const int index ) const;
	int	getType(   const int index ) const;
	int	getInt(    const int index ) const;
	LINT getLint( const int index ) const;
	bool	getBool(   const int index ) const;
	double	getReal(   const int index ) const;
	std::string	getString( const int index ) const;
	XDATE	getDate(   const int index ) const;
	XTIME	getTime(   const int index ) const;
	XBINOB	getBinob(  const int index ) const;
	ROSETTA *getRosetta( const int index ) const;
						// TEST-TYPE-BY-INDEX FUNCTIONS
	bool	isInt(     const int index ) const;
	bool	isLint(    const int index ) const;
	bool	isBool(    const int index ) const;
	bool	isReal(    const int index ) const;
	bool	isString(  const int index ) const;
	bool	isDate(    const int index ) const;
	bool	isTime(    const int index ) const;
	bool	isBinob(   const int index ) const;
	bool	isRosetta( const int index ) const;
						// GET-BY-INDEX (DEFAULT)
	int	getIntDefault(    const int index, int def ) const;
	LINT getLintDefault( const int index, LINT def ) const;
	bool	getBoolDefault(   const int index, bool def ) const;
	double	getRealDefault(   const int index, double def ) const;
	std::string	getStringDefault( const int index, std::string def ) const;
	XDATE	getDateDefault(   const int index, XDATE def ) const;
	XTIME	getTimeDefault(   const int index, XTIME def ) const;
	XBINOB	getBinobDefault(  const int index, XBINOB def ) const;
	ROSETTA *getRosettaDefault( const int index, ROSETTA *def ) const;
	const ROSE_BASE	*pointerRoseBase( const int index ) const;
						// GET-BY-NAME FUNCTIONS
	bool	flagExists(const std::string name ) const;
	int	getType(   const std::string name ) const;
	int	getInt(    const std::string name ) const;
	LINT getLint( const std::string name ) const;
	bool	getBool(   const std::string name ) const;
	double	getReal(   const std::string name ) const;
	std::string	getString( const std::string name ) const;
	XDATE	getDate(   const std::string name ) const;
	XTIME	getTime(   const std::string name ) const;
	XBINOB	getBinob(  const std::string name ) const;
	ROSETTA *getRosetta( const std::string name ) const;
	const ROSE_BASE	*pointerRoseBase( const std::string name ) const;
						// TEST-TYPE-BY-NAME FUNCTIONS
	bool	isInt(     const std::string name ) const;
	bool	isLint(    const std::string name ) const;
	bool	isBool(    const std::string name ) const;
	bool	isReal(    const std::string name ) const;
	bool	isString(  const std::string name ) const;
	bool	isDate(    const std::string name ) const;
	bool	isTime(    const std::string name ) const;
	bool	isBinob(   const std::string name ) const;
	bool	isRosetta( const std::string name ) const;
						// GET-BY-NAME (DEFAULT)
	int	getIntDefault(    const std::string name, int def ) const;
	LINT getLintDefault( const std::string name, LINT def ) const;
	bool	getBoolDefault(   const std::string name, bool def ) const;
	double	getRealDefault(   const std::string name, double def ) const;
	std::string	getStringDefault( const std::string name, std::string def ) const;
	XDATE	getDateDefault(   const std::string name, XDATE def ) const;
	XTIME	getTimeDefault(   const std::string name, XTIME def ) const;
	XBINOB	getBinobDefault(  const std::string name, XBINOB def ) const;
	ROSETTA *getRosettaDefault( const std::string name, ROSETTA *def ) const;
						// DIRECT (DANGEROUS) LINKS
	const std::string *pointerString( const int index ) const;
	const std::string *pointerString( const std::string name ) const;
	const XDATE       *pointerDate( const int index ) const;
	const XDATE       *pointerDate( const std::string name ) const;
	const XTIME       *pointerTime( const int index ) const;
	const XTIME       *pointerTime( const std::string name ) const;
	const XBINOB      *pointerBinob( const int index ) const;
	const XBINOB      *pointerBinob( const std::string name ) const;
	ROSETTA *pointerRosetta( const int index ) const;
	ROSETTA *pointerRosetta( const std::string name ) const;
						// VALUES RETURNED ON ERROR
	static	const	int	errorInt;
	static	const	LINT 	errorLint;
	static	const	bool	errorBool;
	static	const	double	errorReal;
	static	const	std::string	errorString;
	static	const	XDATE	errorDate;
	static	const	XTIME	errorTime;
	static	const	XBINOB	errorBinob;	
	static	const	ROSETTA* errorRosetta;
	static 	void	useExceptions( const bool ue );
	static 	bool	usingExceptions( void );
						// TYPE CODES
	static	const	int	typeError	= 0;
	static	const	int	typeFlag	= (int) 'f';
	static	const	int	typeEnum	= (int) 'e';
	static	const	int	typeInt		= (int) 'i';
	static	const	int	typeLint	= (int) 'l';	// ELL
	static	const	int	typeBool	= (int) 'b';
	static	const	int	typeReal	= (int) 'r';
	static	const	int	typeString	= (int) 's';
	static	const	int	typeWstring	= (int) 'w';
	static	const	int	typeDate	= (int) 'd';
	static	const	int	typeTime	= (int) 't';
	static	const	int	typeBinob	= (int) 'o';
	static 	const	int	typeRosetta	= (int) 'R';
						// WARNING+ERROR CODES
	static	const	int	ProblemLethalBug	= 666;
	static	const	int	ProblemForbiddenByRule	= 1;
	static	const	int	ProblemForbiddenByOwner	= 2;
	static	const	int	ProblemSuspiciousString	= 3;
	static	const	int	ProblemNotFound		= 4;
	static	const	int	ProblemUndesiredType 	= 5;
	static	const	int	ProblemUncodedChar	= 6;
	static	const	int	ProblemInvalidValue	= 7;
	static	const	int	ProblemDeserialize	= 8;
						// UTILITY FUNCTIONS
	static	std::string 	compressString( const std::string input );
	static	void	makeLowerCase( std::string *s );
	static	void	makeUpperCase( std::string *s );
	static	std::string	toLowerCase( const std::string s );
};
//---------------------------------------------------------------------------
#endif

