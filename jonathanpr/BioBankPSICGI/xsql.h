#ifndef xsqlH
#define xsqlH
//===========================================================================
#include "rosetta.h"
#include "xdb.h"
#ifdef __BORLANDC__
#include <utilcls.h>
#endif
#if X_BDE
#include <Db.hpp>
#include <DBTables.hpp>
#include <DBGrids.hpp>
#endif
#include <vector>
#include <string>
//---------------------------------------------------------------------------
class XSQL : public XDB_ERROR
{
friend
	class XDB;
private:
	void 	init( XDB *db );
#if X_BDE
	bool 	bdeApplyParams( void );
	bool 	bdeInit( XDB *db );
	void	bdeDelete( void );
#elif X_ING
	bool	ingInit( XDB *db );
	void	ingDelete( void );
	bool	ingConstructParameters( void );
	void	ingConstructDisplay( void );
#endif
protected:
	XSQL( XDB *db, std::string query );
	virtual ~XSQL( void );
	int	nrows_fetched;
	int	nrows_altered;
	int	return_value;
	const	ROSETTA	*pars;
	std::string	sql;
	bool 	construct( void );
	const	ROSETTA	*param_source;
	void 	assignParameterSource( void );
	virtual	std::string	getClass( void ) = 0;
	static	bool	debug_mode;
	static	bool	enforce_singleton;
#ifdef __BORLANDC__
	TCOMCriticalSection::Lock *lock;
	static	TCOMCriticalSection	thread_lock;
#endif
#if X_BDE
	TQuery	*qry;
	bool 	bdeConstruct( void );
#elif X_ING
	IIAPI_QUERYPARM   	queryParm;
	std::string	query_text;
	std::string	object_name;
	int	nparam;
	std::vector<std::string> 	param_name;
	std::vector<int> 	param_type;
	bool	ingConstruct( void );
	bool 	ingGetDescr( IIAPI_GETDESCRPARM *getDescrParm, II_PTR stmtHandle );
	bool 	ingDescribeUserParameters( IIAPI_DESCRIPTOR *desc );
	bool 	ingPutUserLong( IIAPI_PUTPARMPARM *putp, const int total,
			const unsigned char *buf );
	bool 	ingPutUserParamsDo( IIAPI_PUTPARMPARM *putp );
	bool 	ingPutUserParam( IIAPI_PUTPARMPARM *putp, const int indx );
	bool 	ingPutUserParameters( IIAPI_PUTPARMPARM *putp );
	bool 	ingGetQueryInfo( IIAPI_GETQINFOPARM *qip, II_PTR stmtHandle );
	bool	ingClose( void );
	bool 	ingCancel( void );
	virtual	const char *ingPlaceholder( void ) = 0;
#endif
public:
	ROSETTA	param;
	void	setParamSource( const ROSETTA *src );
					// INTERROGATION FUNCTIONS
	std::string	getSQL( void );
					// ACCESS FUNCTIONS
	bool	setSQL( const std::string query );
	bool	extendSQL( const std::string query );
	int	getNRowsFetched( void ) const;
	int	getNRowsAltered( void ) const;
	int	getReturnValue( void ) const;
	static	const	std::string	blob;
	static	const	std::string	null;
	static	const	std::string	nullable;
	static	const	int		no_return_value = 666111666;
	static	std::string 	makeSafeString( const std::string input );
	bool	singletonInit( void );
	void	singletonEnd( void );
	static	bool	enforceSingleton( const bool es );
};
//===========================================================================
#endif

