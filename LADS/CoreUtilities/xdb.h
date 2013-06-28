#ifndef xdbH
#define xdbH
//===========================================================================
#if defined(__BORLANDC__) && defined(XSQL_BDE)
#define	X_BDE	1
#define	X_ING	0
#else
#define	X_BDE	0
#define	X_ING	1
#endif
//===========================================================================
#if X_BDE
#include <Db.hpp>
#include <DBTables.hpp>
#include <DBGrids.hpp>
#elif X_ING
extern "C" {
#include <iiapi.h>	// REQUIRES PATH ii_system/files
	}
#endif
//---------------------------------------------------------------------------
#include <string>
#include <vector>
class XDB;
class XQUERY;
class XEXEC;
//---------------------------------------------------------------------------
typedef	bool (*XDB_ERROR_CALLBACK)
	( const std::string object, const int instance,
	const int ecount, const int ecode, const std::string error_txt );
//---------------------------------------------------------------------------
class XDB_ERROR
{
private:
	static	int	instances;
protected:
	XDB_ERROR( const std::string owner, XDB *db );
	~XDB_ERROR( void);
	std::string	owner_object;
	int	error_count;
	int	last_error_code;
	bool	is_valid;
	int	object_instance;
	std::string	last_error_text;
	XDB	*database;
#if X_ING
	std::string 	ingExpandError( II_PTR handle );
#endif
	bool	error( const int ecode, const std::string etxt );
	static	int	getNextInstance( void );
public:
	bool	isValid( void );
	int 	getErrorCount( void );
	std::string 	getLastError( void );
#if X_ING
	bool 	ingGetResult( IIAPI_GENPARM *genParm );
#endif
};
//---------------------------------------------------------------------------
class XDB : public XDB_ERROR
{
friend	class XSQL;
private:
	bool	is_open;
	bool	auto_commit;
	bool	local_time;
	int	connection_timeout;
	std::string	database_name;
	std::string	username, password;
#if X_BDE
	std::vector<TQuery *> query;
	TDatabase	*db;
	TQuery	*bdeSupplyQuery( void );
	void	bdeInit( const std::string target );
	void	bdeDelete( void );
	bool	bdeOpen( void );
	bool	bdeClose( void );
	int	bdeCountOpen( void );
#elif X_ING
	static	bool	ingInitialised;
	static	bool	blob_chunk_defined;
	static	int	blob_chunk_size;
	II_PTR 	connHandle;
	II_PTR 	tranHandle;
	II_PTR	envHandle;
	bool	ingInitAPI( void );
	void	ingInit( const std::string target );
	bool	ingOpen( void );
	void	ingDelete( void );
	bool	ingClose( void );
	int	ingCountOpen( void );
#endif
protected:
	XDB_ERROR_CALLBACK	errorCallBack;
public:
	XDB( const std::string target );
	~XDB( void );
	static	const	char	*os;
	void 	setErrorCallBack( XDB_ERROR_CALLBACK ec );
	bool	errorHandler( const std::string caller, const int instance,
		const int ecount, const int ecode, const std::string etxt );
					// INTEROGATORS
	std::string	getDatabaseName( void ) const;
	std::string 	getDatabaseStem( void ) const;
	bool	isOpen( void );
	int 	countOpen( void );
	bool	isAutoCommit( void ) const;
	bool	returningLocalTime( void ) const;
					// ACTORS
	bool	open( void );
	bool 	close( void );
	bool 	commit( void );
	bool	rollback( void );	// (NOT RELIABLE)
	bool	setAutoCommit( const bool ac );
	bool	useLocalTime( const bool lt );
	void	setUserName( const std::string un );
	void	setPassWord( const std::string pw );
	void	setConnectionTimeOut( int nsecs );
	XQUERY	*newQuery( const std::string query = "");
	XEXEC	*newExec( const std::string query = "" );
#if X_ING
	bool 	ingCommit( void );
	bool 	ingRollback( void );
	II_PTR	getConnHandle( void );
	II_PTR	getTranHandle( void );
	void	setTranHandle( II_PTR );
	static	void 	ingWait( IIAPI_GENPARM *genParm );
#endif
};
//===========================================================================
#endif



