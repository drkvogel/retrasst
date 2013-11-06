#ifndef xqueryH
#define xqueryH
//===========================================================================
#include "xdb.h"
#if X_BDE
#include <Db.hpp>
#include <DBTables.hpp>
#include <DBGrids.hpp>
#endif
#include <string>
#include "xsql.h"
//---------------------------------------------------------------------------
class XQUERY : public XSQL
{
friend
	class XDB;
private:
	bool	is_open;
	virtual	std::string	getClass( void );
#if X_BDE
	bool	bdeOpenAction( void );
	bool	bdeOpen( void );
	bool	bdeFetchAction( void );
	bool	bdeFetch( void );
	bool	bdeClose( void );
#elif X_ING
	IIAPI_GETCOLPARM  	getColParm;
	IIAPI_GETDESCRPARM 	getDescrParm;
	IIAPI_DATAVALUE		ing_val;
	std::vector<char *> ing_val_buf;
	bool	ingOpen( void );
	bool	ingBufInit( void );
	void	ingBufRemove( void );
	bool 	ingFetchRepackKey( const char *name, const IIAPI_DATAVALUE *v );
	bool 	ingFetchRepackInt( const char *name, const IIAPI_DATAVALUE *v );
	bool 	ingFetchRepackReal( const char *name, const IIAPI_DATAVALUE *v  );
	bool 	ingFetchRepackBlob( const IIAPI_DATAVALUE *v, int *total, char **buf );
	bool 	ingFetchRepackLVarchar( const char *name, const IIAPI_DATAVALUE *v );
	bool 	ingFetchRepackVarchar( const char *name, const IIAPI_DATAVALUE *v );
	bool 	ingFetchRepackChar( const char *name, const IIAPI_DATAVALUE *v );
	bool 	ingFetchRepackDate( const char *name, const IIAPI_DATAVALUE *v );
	bool 	ingFetchRepackByte( const char *name, const IIAPI_DATAVALUE *v );
	bool 	ingFetchRepackLByte( const char *name, const IIAPI_DATAVALUE *v );
	bool 	ingFetchRepackOther( const char *name, IIAPI_DESCRIPTOR *d,
			const IIAPI_DATAVALUE *v );
	bool 	ingFetchSetNull( const char *nam, const int typ );
	bool 	ingFetchRepack( IIAPI_DESCRIPTOR *d, const IIAPI_DATAVALUE *v );
	bool	ingFetch( void );
	bool	ingQueryClose( void );
#endif
protected:
	ROSETTA	*res_ptr;
#if X_BDE
#elif X_ING
	virtual	const char *ingPlaceholder( void );
#endif
public:
	XQUERY( XDB *db, const std::string query );
	~XQUERY( void );
	ROSETTA	result;
					// INTERROGATORS
	bool	isOpen( void );
	int	getNRows( void );
					// ACCESS FUNCTIONS
	bool	open( void );
	bool 	fetch( void );
	bool 	fetch( ROSETTA *output );
	int	fetchInt( const int default_value );
	LINT	fetchLint( const LINT default_value );
	bool	fetchBool( const bool default_value );
	std::string 	fetchString( const std::string default_value );
	XTIME 	fetchTime( const XTIME default_value );
	bool 	fetchSingle( ROSETTA *single );
	bool 	close( void );
};
//===========================================================================
#endif

