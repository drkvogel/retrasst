#ifndef xexecH
#define xexecH
//===========================================================================
#include "xdb.h"
#if X_BDE
#include <Db.hpp>
#include <DBTables.hpp>
#include <DBGrids.hpp>
#endif
class XDB;
#include <string>
#include "xsql.h"
//---------------------------------------------------------------------------
class XEXEC : public XSQL
{
friend
	class XDB;
private:
	virtual	std::string	getClass( void );
#if X_BDE
	bool	bdeExecAction( void );
	bool	bdeExec( void );
#elif X_ING
	bool	ingExecParams( void );
	bool	ingExecSimple( void );
	bool 	ingGetOutcome( void );	
#endif
protected:
#if X_BDE
#elif X_ING
	virtual	const char *ingPlaceholder( void );
#endif
public:
	XEXEC( XDB *db, const std::string query );
	XEXEC( XDB *db );
	~XEXEC( void );
					// INTERROGATORS
	bool	isOpen( void );
	int	getNRows( void );
					// ACCESS FUNCTIONS
	bool	exec( void );
	bool 	insert( const std::string table, const ROSETTA *values );
	bool 	update( const std::string table, const ROSETTA *update,
			const ROSETTA *where );
	bool 	del( const std::string table, const ROSETTA *where );
};
//===========================================================================
#endif

