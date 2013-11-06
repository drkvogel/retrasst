#include "xdb.h"
#include "xquery.h"
#include "xexec.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#if X_BDE
#include <vcl.h>
#endif
//===========================================================================
int 	XDB_ERROR::instances = 0;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XDB_ERROR::XDB_ERROR( const std::string owner, XDB *db )
	:
	owner_object( owner ),
	database( db ),
	error_count( 0 ),
	last_error_code( -1 ),
	last_error_text( "" ),
	is_valid( true )
{
	object_instance = getNextInstance();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XDB_ERROR::~XDB_ERROR( void)
{
}
//---------------------------------------------------------------------------
			/* GENERATE UNIQUE REPORTING IDS FOR EACH OBJECT */
int XDB_ERROR::getNextInstance( void )
{	return( ++instances );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_ING
std::string XDB_ERROR::ingExpandError( II_PTR handle )
{
	int	i;
	char 	buf[305];
	std::string	txt = "Info: ";
	IIAPI_GETEINFOPARM 	gete;
	gete.ge_errorHandle = (II_PTR)handle;
	do // Call IIapi_getErrorInfo() in loop until no data or failed.
		{
		IIapi_getErrorInfo( &gete );
		if ( gete.ge_status != IIAPI_ST_SUCCESS ) // NO DATA OR FAIL
			{break;
			}
		switch( gete.ge_type )
			{
			case IIAPI_GE_ERROR:
				txt += "ERROR";
				break;
			case IIAPI_GE_WARNING:
				txt += "WARNING";
				break;
			case IIAPI_GE_MESSAGE:
				txt += "USER MESSAGE";
				break;
			default:
				sprintf( buf, "Unknown error type=%d",
					+  gete.ge_type );
				txt += buf;
				break;
			}
		sprintf( buf, "' %d: ", gete.ge_errorCode );
		txt += std::string(" '") + gete.ge_SQLSTATE + buf
			+ ( gete.ge_message
			? std::string(gete.ge_message) : std::string("NULL") );
		if ( gete.ge_serverInfoAvail )
			{
			sprintf( buf, "\n svr_id_error     = %d"
				"\n svr_local error  = %d"
				"\n svr_id_server    = %d"
				"\n svr_server_type  = %d"
				"\n svr_severity     = ",
				gete.ge_serverInfo->svr_id_error,
				gete.ge_serverInfo->svr_local_error,
				gete.ge_serverInfo->svr_id_server,
				gete.ge_serverInfo->svr_server_type );
			txt += buf;
			switch( gete.ge_serverInfo->svr_severity )
				{
				case IIAPI_SVR_DEFAULT:
					txt += "IIAPI_SVR_DEFAULT";
					break;
				case IIAPI_SVR_MESSAGE:
					txt += "IIAPI_SVR_MESSAGE";
					break;
				case IIAPI_SVR_WARNING:
					txt += "IIAPI_SVR_WARNING";
					break;
				case IIAPI_SVR_FORMATTED:
					txt += "IIAPI_SVR_FORMATTED";
					break;
				default:
					txt += "(unknown)";
					break;
				}
			sprintf( buf, " ( 0x%x )\n",
				gete.ge_serverInfo->svr_severity );
			txt += buf;
			for( i = 0; i < gete.ge_serverInfo->svr_parmCount; i++ )
				{
				txt += "\t: ";
				if ( gete.ge_serverInfo->svr_parmDescr[i].ds_columnName
					&& *gete.ge_serverInfo->svr_parmDescr[i].ds_columnName )
					{txt += gete.ge_serverInfo->svr_parmDescr[i].ds_columnName
						+ std::string(" = ");
					}
				if ( gete.ge_serverInfo->svr_parmDescr[i].ds_nullable
					&& gete.ge_serverInfo->svr_parmValue[i].dv_null )
					{txt += "NULL";
					}
				else if ( IIAPI_CHA_TYPE ==
					gete.ge_serverInfo->svr_parmDescr[i].ds_dataType )
					{
					int	len = gete.ge_serverInfo->svr_parmValue[i].dv_length;
					if ( len < 300 )
						{
						sprintf( buf, "'%*.*s'", len, len,
							gete.ge_serverInfo->svr_parmValue[i].dv_value );
						txt += buf;
						}
					else
						{txt += "svr_parmValue-too-long-to-print";
						}
					}
				else
					{sprintf( buf, "Datatype %d not displayed",
						gete.ge_serverInfo->svr_parmDescr[i].ds_dataType );
					txt += buf;
					}
				}
			}
		}
		while( true );
	return( txt );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDB_ERROR::ingGetResult( IIAPI_GENPARM *genParm )
{
	bool	ok = false;
	std::string	err;
	XDB::ingWait( genParm );
	switch( genParm->gp_status )
		{
		case IIAPI_ST_SUCCESS:
		case IIAPI_ST_MESSAGE:
		case IIAPI_ST_WARNING:
		case IIAPI_ST_NO_DATA:	// PROBABLY OK, BUT DUNNO
			ok = true;
			break;
		case IIAPI_ST_ERROR:
			err = "IIAPI_ST_ERROR";
			break;
		case IIAPI_ST_FAILURE:
			err = "IIAPI_ST_FAILURE";
			break;
		case IIAPI_ST_NOT_INITIALIZED:
			err = "IIAPI_ST_NOT_INITIALIZED";
			break;
		case IIAPI_ST_INVALID_HANDLE:
			err = "IIAPI_ST_INVALID_HANDLE";
			break;
		case IIAPI_ST_OUT_OF_MEMORY:
			err = "IIAPI_ST_OUT_OF_MEMORY";
			break;
		default:
			err = "II(unknown status)";
			break;
		}
	if ( ok )
		{return( true );
		}
	if ( genParm->gp_errorHandle )
		{err += ingExpandError( genParm->gp_errorHandle );
		}
	error( genParm->gp_status, err );
	return( false );
}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDB_ERROR::error( const int ecode, const std::string etxt )
{
	last_error_code = ecode;
	last_error_text = etxt;
	bool	result = false;
	if ( NULL == database ) 		// NO WAY TO SIGNAL NICELY
		{throw ( std::string("XDB_ERROR, NULL database") );
		}
	else
		{result = database->errorHandler( "XDB", object_instance,
			++error_count, ecode, etxt );
		}
	return( result );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XDB_ERROR::getErrorCount( void )
{
	return( error_count );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string XDB_ERROR::getLastError( void )
{
	return( last_error_text );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDB_ERROR::isValid( void )
{
	return( is_valid );
}
//===========================================================================
const	char	*XDB::os =
#if X_BDE
	"BDE";   	// COMPILED FOR BORLAND DATABASE ENGINE
#elif X_ING
	"INGRES";	// COMPILED FOR INGRES OPENAPI
bool	XDB::ingInitialised = false;
bool	XDB::blob_chunk_defined = false;
int	XDB::blob_chunk_size = 2000;
#endif
//---------------------------------------------------------------------------
XDB::XDB( const std::string target )
	:
	errorCallBack( NULL ),
	username(""),
	password(""),
	connection_timeout( -1 ),	// UNLIMITED
	XDB_ERROR( "XDB", this )
{
	object_instance = getNextInstance();
	error_count = 0;
	last_error_code = -1;
	last_error_text = "";
#if X_BDE
	bdeInit( target );
#elif X_ING
	ingInit( target );
#endif
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_BDE
void XDB::bdeInit( const std::string target )
{
	db = new TDatabase( NULL );
	is_valid = ( NULL != db );
	if ( ! is_valid )
		{return;
		}
	char	buf[20];
	sprintf( buf, "%d", getNextInstance() );
	database_name = target + "_" + buf;
	try
		{
		db->AliasName    = target.c_str();		// WHAT TO OPEN
		db->DatabaseName = database_name.c_str();	// UNIQUE IDENTIFIER
		db->SessionName  = "Default";
		db->LoginPrompt = false;	// ALL PASSWORD INFO MUST BE IN SESSION
		}
	catch ( Exception &e )
		{is_valid = false;
		}
	auto_commit = true;
	local_time = false;			// ?
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XDB::bdeDelete( void )
{
	std::vector<TQuery *>::iterator qi = query.begin();
	while ( qi != query.end() )
		{if ( (*qi)->Active )
			{(*qi)->Close();
			}
		delete (*qi);
		qi++;
		}
	db->Close();
	delete db;
}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_ING
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDB::ingInitAPI( void )
{
	IIAPI_INITPARM  initParm;
	initParm.in_timeout = -1;
	initParm.in_version = IIAPI_VERSION_5;
	IIapi_initialize( &initParm );
	envHandle = initParm.in_envHandle;
	if ( initParm.in_status != IIAPI_ST_SUCCESS )
		{char	ebuf[100];
		sprintf( ebuf, "Error in IIapi_initialize, in_status=%d",
			initParm.in_status );
		error( initParm.in_status, ebuf );
		return( false );
		}
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XDB::ingWait( IIAPI_GENPARM *genParm )
{					// WAIT TILL API FUNCTION COMPLETES
	IIAPI_WAITPARM waitParm = { -1 };
	while( FALSE == genParm->gp_completed )
		{IIapi_wait( &waitParm );
		}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XDB::ingInit( std::string target )
{
	is_open = false;
	connHandle = NULL;
	tranHandle = NULL;
	envHandle  = NULL;
	database_name = target;
	if ( ! ingInitialised )
		{is_valid = ingInitAPI();
		ingInitialised = is_valid;
		}
	else
		{is_valid = true;	// BUG IF FIRST ONE FAILED
		}
	auto_commit = false;
	local_time = false;			// ?
	if ( ! is_valid )
		{return;
		}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XDB::ingDelete( void )
{

}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XDB::~XDB( void )
{
	if ( isOpen() )
		{close();
		}
#if X_BDE
	bdeDelete();
#elif X_ING
	ingDelete();
#endif
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_ING
bool XDB::ingCommit( void )
{
	if ( NULL == tranHandle )
		{return( true );	// (GUESSING!) NOTHING TO COMMIT
		}
	IIAPI_COMMITPARM  commitParm;
	commitParm.cm_genParm.gp_callback = NULL;
	commitParm.cm_genParm.gp_closure = NULL;
	commitParm.cm_tranHandle = tranHandle;
	IIapi_commit( &commitParm );
	if ( ! ingGetResult( &commitParm.cm_genParm ) )
		{return( false );
		}
	tranHandle = NULL;	// DUNNO WHAT HAPPENS IF THIS ISN'T REACHED
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDB::ingRollback( void )
{
	if ( NULL == tranHandle )
		{return( true );	// (GUESSING!) NOTHING TO COMMIT
		}
	IIAPI_ROLLBACKPARM rbParm;
	rbParm.rb_genParm.gp_callback = NULL;
	rbParm.rb_genParm.gp_closure = NULL;

	rbParm.rb_tranHandle=tranHandle;
	rbParm.rb_savePointHandle=NULL;

	IIapi_rollback( &rbParm );
	if ( ! ingGetResult( &rbParm.rb_genParm ) )
		{return( false );
		}
	tranHandle = NULL;	// DUNNO WHAT HAPPENS IF THIS ISN'T REACHED
	return( true );
}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDB::commit( void )
{
#if X_BDE
	return( true );
#elif X_ING
	return( ingCommit() );
#endif
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDB::rollback( void )
{
#if X_BDE
	return( true );
#elif X_ING
	return( ingRollback() );
#endif
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XDB::setErrorCallBack( XDB_ERROR_CALLBACK ec )
{
	errorCallBack = ec;
}
//---------------------------------------------------------------------------
bool XDB::errorHandler( const std::string object, const int instance,
	const int ecount, const int ecode, const std::string error_txt )
{
	if ( NULL != errorCallBack )
		{return( errorCallBack( object, instance, ecount, ecode,
			error_txt ) );
		}
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDB::isOpen( void )
{
#if X_BDE
	is_open = db->Connected;
#endif
	return( is_open );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string XDB::getDatabaseName( void ) const
{
	return( database_name );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string XDB::getDatabaseStem( void ) const
{			// REMOVE ANY CONNECTION INFORMATION FROM DATABASE NAME
	const	int	colon_pos = database_name.rfind( ':' );
	const	int	dsiz = database_name.size();
	if ( colon_pos < 0 || colon_pos >= dsiz )
		{return( database_name );
		}
	return( std::string( database_name, colon_pos+1, dsiz - colon_pos -1 ));
}
//---------------------------------------------------------------------------
bool XDB::isAutoCommit( void ) const
{
	return( auto_commit );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDB::returningLocalTime( void ) const
{
	return( local_time );
}
//---------------------------------------------------------------------------
bool XDB::setAutoCommit( bool ac )
{
#if X_BDE
	/* ALWAYS AUTO-COMMIT IF BDE ? */
#elif X_ING
	auto_commit = ac;
	IIAPI_AUTOPARM	a;
	a.ac_genParm.gp_callback = NULL;
	a.ac_genParm.gp_closure  = NULL;
	if ( auto_commit )
		{			// ENABLE AUTO-COMMIT
		a.ac_connHandle = connHandle;
		a.ac_tranHandle = NULL;
		}
	else				// DISABLE AUTO-COMMIT
		{
		a.ac_connHandle = NULL;
		a.ac_tranHandle = tranHandle;
		}
	IIapi_autocommit( &a );
	if ( ! ingGetResult( &(a.ac_genParm) ) )
		{return( false );
		}
	if ( auto_commit )
		{tranHandle = a.ac_tranHandle;
		}
	else
		{tranHandle = NULL;
		}
#endif
	return( isAutoCommit() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDB::useLocalTime( bool lt )
{
	local_time = lt;
	return( returningLocalTime() );
}
//---------------------------------------------------------------------------
void XDB::setUserName( const std::string un )
{
	username = un;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XDB::setPassWord( const std::string pw )
{
	password = pw;
#if X_BDE
	Session->AddPassword( pw.c_str() );
#endif
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XDB::setConnectionTimeOut( int nsecs )
{
	if ( nsecs < 1 )
		{connection_timeout = -1;
		return;
		}
	if ( nsecs > 60 )
		{nsecs = 60;
		}
	connection_timeout = nsecs * 1000;	// CONVERT TO MILI-SECS
}
//---------------------------------------------------------------------------
#if X_BDE
bool XDB::bdeOpen( void )
{
	try
		{db->Open();
		}
	catch( Exception &e )
		{// IGNORE, OPEN STATUS CAPTURES ANY PROBLEMS
		}
	return( true );
}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_ING
bool XDB::ingOpen( void )
{
	IIAPI_CONNPARM    	connParm;
	connHandle = NULL;
	tranHandle = NULL;
	connParm.co_genParm.gp_callback = NULL;
	connParm.co_genParm.gp_closure = NULL;
	connParm.co_target     = (char *) malloc( database_name.size() + 1 );
	strcpy( connParm.co_target, database_name.c_str() );
	connParm.co_connHandle = envHandle;
	connParm.co_type       = IIAPI_CT_SQL;
	connParm.co_tranHandle = NULL;
	connParm.co_username   = username.empty()
		? NULL : (char *) username.c_str();
	connParm.co_password   = password.empty()
		? NULL : (char *) password.c_str();
	connParm.co_timeout    = connection_timeout;
	IIapi_connect( &connParm );
	if ( ! ingGetResult( &connParm.co_genParm ) )
		{return( false );
		}
	connHandle = connParm.co_connHandle;
	tranHandle = connParm.co_tranHandle;
	free( ( II_PTR )connParm.co_target );
	setAutoCommit( true );			// DEFAULT BEHAVIOUR
	if ( ! blob_chunk_defined )
		{
		IIAPI_SETENVPRMPARM	setEnvPrmParm;
		II_LONG	pv = blob_chunk_size;
		setEnvPrmParm.se_envHandle = envHandle;
		setEnvPrmParm.se_paramID = IIAPI_EP_MAX_SEGMENT_LEN;
		setEnvPrmParm.se_paramValue = (II_LONG *) &pv;
		IIapi_setEnvParam( &setEnvPrmParm );
		if ( IIAPI_ST_SUCCESS == setEnvPrmParm.se_status )
			{blob_chunk_defined = true;
			}
		else
			{error( 0, "open(), failed to set blob_chunk_size" );
			}
		}
	is_open = true;
	return( is_open );
}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDB::open( void )
{
	if ( ! is_valid )
		{error( 0, "Cannot open inValid XDB object" );
		return( false );
		}
	if ( isOpen() )			
		{return( true );
		}
#if X_BDE
	bdeOpen();
#elif X_ING
	ingOpen();
#endif
	return( isOpen() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_BDE
bool XDB::bdeClose( void )
{
	if ( ! is_valid )
		{return( false );
		}
	bool	 ok = true;
	std::vector<TQuery *>::iterator qi = query.begin();
	while ( qi != query.end() )
		{
		if ( (*qi)->Tag != 0 )
			{ok = false;	// XQUERY STILL USING OBJECT
			(*qi)->Tag = 2;
			}
		if ( (*qi)->Active )
			{(*qi)->Close();
			}
		qi++;
		}
	db->Close();
	return( ok );
}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_ING
bool XDB::ingClose( void )
{
	if ( ! is_open )
		{return( true );
		}
	if ( auto_commit )	// REQUIRED TO PREVENT ERROR ON CLOSING
		{setAutoCommit( false );
		}
	else     		// DISPOSE OF ANY UNCOMMITTED OPERATIONS
		{commit();
		}
	is_open = false;
	IIAPI_DISCONNPARM 	disconnParm;
	disconnParm.dc_genParm.gp_callback = NULL;
	disconnParm.dc_genParm.gp_closure = NULL;
	disconnParm.dc_connHandle = connHandle;
	IIapi_disconnect( &disconnParm );
	if ( ! ingGetResult( &disconnParm.dc_genParm ) )
		{return( false );
		}
	connHandle = NULL;
	return( true );
}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XDB::close( void )
{
	if ( ! is_valid )
		{return( false );
		}
#if X_BDE
	return( bdeClose() );
#elif X_ING
	return( ingClose() );
#endif
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_BDE
int XDB::bdeCountOpen( void )
{
	int	count = 0;
	std::vector<TQuery *>::iterator qi = query.begin();
	while ( qi != query.end() )
		{if ( (*qi)->Active )
			{count++;
			}
		qi++;
		}
	return( count );
}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_ING
int XDB::ingCountOpen( void )
{
	return( -1 );
}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XDB::countOpen( void )
{
#if X_BDE
	return( bdeCountOpen() );
#elif X_ING
	return( ingCountOpen() );
#endif
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_BDE
TQuery *XDB::bdeSupplyQuery( void )
{
	if ( ! is_valid )
		{return( NULL );
		}
	std::vector<TQuery *>::iterator qi = query.begin();
	while ( qi != query.end() )	// TRY TO ALLOCATE FROM EXISTING
		{
		if ( 0 == (*qi)->Tag )
			{return( *qi );
			}
		qi++;
		}
	TQuery	*qnew = new TQuery( NULL );
	qnew->Tag = 1;					// MARK AS IN-USE
	qnew->DatabaseName = db->DatabaseName;
	qnew->SessionName  = db->SessionName;
	query.push_back( qnew );
	return( qnew );
}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_ING
II_PTR XDB::getConnHandle( void )
{
	return( connHandle );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
II_PTR XDB::getTranHandle( void )
{
	return( tranHandle );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XDB::setTranHandle( II_PTR th )
{
	tranHandle = th;
}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XQUERY *XDB::newQuery( const std::string query )
{
	return( is_valid ? new XQUERY( this, query ) : NULL );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XEXEC *XDB::newExec( const std::string query )
{
	return( is_valid ? new XEXEC( this, query ) : NULL );
}
//===========================================================================





