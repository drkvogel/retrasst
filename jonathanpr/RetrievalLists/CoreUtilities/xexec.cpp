#include "xdb.h"
#include "xexec.h"
#include "rosetta.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <set>
//===========================================================================
XEXEC::XEXEC( XDB *db, const std::string query )
	:
	XSQL( db, query )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XEXEC::XEXEC( XDB *db )
	:
	XSQL( db, "" )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XEXEC::~XEXEC( void )
{
}
//---------------------------------------------------------------------------
std::string XEXEC::getClass( void )
{	return( "XEXEC" );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XEXEC::getNRows( void )
{
	return( nrows_altered );
}
//---------------------------------------------------------------------------
#if X_BDE
bool XEXEC::bdeExecAction( void )
{
	bool	ok = true;
	try
		{qry->ExecSQL();
		}
	catch ( Exception &e )
		{ok = false;
		}
	return( ok );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XEXEC::bdeExec( void )
{
	bool 	ok;
	Screen->Cursor = crSQLWait;
	try
		{ok = bdeExecAction();
		}
	__finally
		{Screen->Cursor = crDefault;
		}
	return( ok );
}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_ING
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
const char *XEXEC::ingPlaceholder( void )
{
	return( " ? " );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XEXEC::ingGetOutcome( void )
{
	IIAPI_GETQINFOPARM	qip;
	ingGetQueryInfo( &qip, queryParm.qy_stmtHandle );
	if ( qip.gq_mask & IIAPI_GQ_ROW_COUNT )
		{nrows_altered = qip.gq_rowCount;
		}
	if ( qip.gq_mask & IIAPI_GQ_PROCEDURE_RET )
		{return_value = qip.gq_procedureReturn;
		}
	const	bool	ok = ingGetResult( &qip.gq_genParm );
	return( ok );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XEXEC::ingExecParams( void )
{
	IIAPI_SETDESCRPARM		setDescrParm;
	IIAPI_PUTPARMPARM		putParmParm;
	IIAPI_GETDESCRPARM		getDescrParm;
	bool	ok = true;
	static	int	exec_counter = 1;
	static	std::set<int>	buffers;
	char	exec_name[20], exec_text[30];
	exec_counter++;
	int	i;
	int	buffer_id = -1;
	for ( i = 0; i < exec_counter; i++ )
		{if ( buffers.find(i) == buffers.end() )
			{buffer_id = i;
			buffers.insert( buffer_id );
			break;	// FIND UNIQUE UN-USED BUFFER ID
			}
		}
	if ( buffer_id < 0 || buffer_id >= exec_counter )
		{error( 0, "out of buffer IDs" );
		return( false );
		}
	sprintf( exec_name, "e%d", buffer_id );
	sprintf( exec_text, "EXECUTE %s", exec_name );
	queryParm.qy_genParm.gp_callback = NULL;
	queryParm.qy_genParm.gp_closure = NULL;
	queryParm.qy_connHandle = database->getConnHandle();
	queryParm.qy_queryType = IIAPI_QT_QUERY;
	queryParm.qy_queryText = (char *) malloc( query_text.size() + 50 );
	sprintf( queryParm.qy_queryText, "PREPARE %s FROM %s", exec_name,
		query_text.c_str() );
	queryParm.qy_parameters = FALSE;
	queryParm.qy_tranHandle = database->getTranHandle();
	queryParm.qy_stmtHandle = ( II_PTR )NULL;
	IIapi_query( &queryParm );			// INVOKE OPENAPI
	database->setTranHandle( queryParm.qy_tranHandle );
	if ( ! ingGetResult( &queryParm.qy_genParm ) )
		{return( false );
		}
	free( queryParm.qy_queryText );
	if ( ! ingGetDescr( &getDescrParm, queryParm.qy_stmtHandle ) )
		{ingClose();
		return( false );
		}
	if ( getDescrParm.gd_descriptorCount != 0 )
		{error( 0, "number of descriptors is incorrect" );
		ingClose();
		return( false );
		}
    /*
    ** Call IIapi_getQueryInfo()
    ** and process result.
    */
//    IIapi_getQueryInfo( (IIAPI_GETQINFOPARM *) queryParm.qy_stmtHandle );

	if ( ! ingClose() )
		{return( false );
		}

	queryParm.qy_connHandle = database->getConnHandle();
	queryParm.qy_queryType = IIAPI_QT_EXEC;
	queryParm.qy_queryText = (char *) malloc( strlen(exec_text) + 1 );
	strcpy( queryParm.qy_queryText, exec_text );
	queryParm.qy_parameters = (nparam > 0 ) ? TRUE : FALSE;
	queryParm.qy_tranHandle = database->getTranHandle();
	queryParm.qy_stmtHandle = ( II_PTR )NULL;
	IIapi_query( &queryParm );
	database->setTranHandle( queryParm.qy_tranHandle );
	if ( ! ingGetResult( &queryParm.qy_genParm ) )
		{return( false );
		}
	free( queryParm.qy_queryText );
	setDescrParm.sd_genParm.gp_callback = NULL;
      	setDescrParm.sd_genParm.gp_closure = NULL;
  	setDescrParm.sd_stmtHandle = queryParm.qy_stmtHandle;
	setDescrParm.sd_descriptorCount = (II_INT2) nparam;
	setDescrParm.sd_descriptor = ( IIAPI_DESCRIPTOR * )
	    malloc( ( setDescrParm.sd_descriptorCount + 1 ) *
			  sizeof( IIAPI_DESCRIPTOR ) );

	ok = ingDescribeUserParameters( setDescrParm.sd_descriptor );
	if ( ! ok )
		{return( false );
		}
	IIapi_setDescriptor( &setDescrParm );
	if ( ! ingGetResult( &setDescrParm.sd_genParm ) )
		{
//		ingCancel();
		return( false );
		}				// * * * leaks memory
	free( ( II_PTR )setDescrParm.sd_descriptor );
				/* Provide parameters for IIapi_putParms() */
	putParmParm.pp_genParm.gp_callback = NULL;
	putParmParm.pp_genParm.gp_closure = NULL;
	putParmParm.pp_stmtHandle = queryParm.qy_stmtHandle;
	putParmParm.pp_parmCount = 1;
	IIAPI_DATAVALUE		data;
	putParmParm.pp_parmData = (IIAPI_DATAVALUE *) &data;
	ok = ingPutUserParameters( &putParmParm );
	buffers.erase( buffer_id );		// RETURN BUFFER_ID TO POOL
	if ( ! ok )
		{return( false );
		}
	ok = ingGetOutcome();
	return( ok && ingClose() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XEXEC::ingExecSimple( void )
{
	IIAPI_GETDESCRPARM		getDescrParm;
	queryParm.qy_genParm.gp_callback = NULL;
	queryParm.qy_genParm.gp_closure = NULL;
	queryParm.qy_connHandle = database->getConnHandle();
	queryParm.qy_queryType = IIAPI_QT_QUERY;
	queryParm.qy_queryText = (char *) malloc( query_text.size() + 1 );
	strcpy( queryParm.qy_queryText, query_text.c_str() );
	queryParm.qy_parameters = FALSE;
	queryParm.qy_tranHandle = database->getTranHandle();
	queryParm.qy_stmtHandle = ( II_PTR )NULL;
	IIapi_query( &queryParm );			// INVOKE OPENAPI
	database->setTranHandle( queryParm.qy_tranHandle );
	if ( ! ingGetResult( &queryParm.qy_genParm ) )
		{return( false );
		}
	free( queryParm.qy_queryText );
	if ( ! ingGetDescr( &getDescrParm, queryParm.qy_stmtHandle ) )
		{ingClose();
		return( false );
		}
	if ( getDescrParm.gd_descriptorCount != 0 )
		{error( 0, "number of descriptors is incorrect" );
		ingClose();
		return( false );
		}
	bool	ok = ingGetOutcome();
	return( ok && ingClose() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XEXEC::exec( void )
{
	nrows_altered = -1;
	return_value = no_return_value;
	if ( ! singletonInit() )
		{return( false );
		}
	if ( ! construct() )
		{singletonEnd();
		return( false );
		}
#if X_BDE
	bool	ok = bdeExec();
#elif X_ING
	bool 	ok = ( ( nparam > 0 ) ? ingExecParams() : ingExecSimple() );
#endif
	singletonEnd();
	return( ok );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XEXEC::insert( const std::string table, const ROSETTA *values )
{
	if ( table.empty() || NULL == values || ! values->isValid() )
		{error( 0, "Null/invalid input arguments" );
		return( false );
		}
	sql = std::string("INSERT INTO ") + table + " (";
	int	i;
	int	nv = values->count();
	std::string	field;
	std::string	par = "";
	for ( i = 0; i < nv; i++ )
		{if ( i > 0 )
			{sql += ",";
			par += ",";
			}
		field = values->getName(i);
		sql += field;
		par += std::string( ":") + field;
		}
	sql += std::string( ") VALUES (" ) + par + ")";
	setParamSource( values );
	return( exec() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XEXEC::update( const std::string table, const ROSETTA *update,
	const ROSETTA *where )
{
	if ( table.empty() || NULL == update || NULL == where
			|| ! update->isValid() || ! where->isValid() )
		{error( 0, "Null/invalid input arguments" );
		return( false );
		}
	std::string	field;
	int	i, j;
	int	nu = update->count();
	int	nw = where->count();
	if ( nu < 1 )
		{error( 0, "No fields in Update rosetta" );
		return( false );
		}
	std::string	stmp = std::string( "UPDATE " ) + table + " SET ";
	for ( i = 0; i < nu; i++ )
		{field = update->getName(i);
		if ( i > 0 )
			{stmp += ",";
			}
		stmp += field + "= :" + field;
		}
	if ( nw > 0 )
		{stmp += " WHERE ";
		for ( i = 0; i < nw; i++ )
			{field = where->getName(i);
			if ( i > 0 )
				{stmp += std::string( " AND " );
				}
			stmp += field + "=:" + field;
			for ( j = 0; j < nu; j++ ) // ROUTINE NOT SUITABLE FOR THIS
				{if ( update->getName(j) == field )
					{error( 0, // TODO: CASE-INSENSITIVE
		"Update/Where rosettas cannot contain the same field" );
					return( false );
					}
				}
			}
		}
	sql = stmp;	// COPY ONLY ONCE ALL ERROR CHECKS HAVE BEEN PASSED
	param.clear();
	param = *update;
	param.insert( where );
	return( exec() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XEXEC::del( const std::string table, const ROSETTA *where )
{
	if ( table.empty() || NULL == where || ! where->isValid() )
		{error( 0, "Null/invalid input arguments" );
		return( false );
		}
	int	nw = where->count();
	if ( nw < 1 )
		{error( 0, "XEXEC delete insists on at least one WHERE member");
		return( false );
		}
	std::string	field;
	int	i;
	std::string	stmp = std::string( "DELETE FROM " ) + table + " WHERE ";
	for ( i = 0; i < nw; i++ )
		{field = where->getName(i);
		if ( i > 0 )
			{stmp += std::string( " AND " );
			}
		stmp += field + "=:" + field;
		}
	sql = stmp;	// COPY ONLY ONCE ALL ERROR CHECKS HAVE BEEN PASSED
	setParamSource( where );
	return( exec() );
}
//===========================================================================
#pragma package(smart_init)

