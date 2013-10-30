#include "xsql.h"
#include "rosetta.h"
#include "xdb.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//===========================================================================
const	std::string	XSQL::blob = "XSQL_BLOB";	// ROSETTA TAG FOR BLOB
const	std::string	XSQL::null = "XSQL_NULL";	// ROSETTA TAG FOR NULL
const	std::string	XSQL::nullable = "XSQL_NABL";	// ROSETTA TAG FOR NULL-ABLE
bool	XSQL::debug_mode = false;
bool	XSQL::enforce_singleton = false;
#ifdef __BORLANDC__
TCOMCriticalSection	XSQL::thread_lock;
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XSQL::XSQL( XDB *db, const std::string query  )
	:
	sql( query ),
	XDB_ERROR( "XSQL", db )
{
	init( db );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_BDE
bool XSQL::bdeInit( XDB *db )
{
	qry = NULL;
	return( NULL != db );
}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_ING
bool XSQL::ingInit( XDB *db )
{
	return( NULL != db );
}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XSQL::init( XDB *db )
{
	last_error_code = -1;
	last_error_text = "";
	nrows_fetched = -1;
	nrows_altered = -1;
	return_value = no_return_value;
	param_source = NULL;
#ifdef __BORLANDC__
	lock = NULL;
#endif
#if X_BDE
	bdeInit( db );
#elif X_ING
	ingInit( db );
#endif
	if ( NULL == database )
		{error( 0, "Null database" );
		}
	else if ( ! database->isValid() )
		{error( 0, "Invalid database" );
		}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_BDE
void XSQL::bdeDelete( void )
{
	if ( NULL != qry )
		{qry->Tag = 0;		// MARK FOR RE-ALLOCATION BY DATABASE OBJECT
		}
}
#elif X_ING
void XSQL::ingDelete( void )
{
	param_type.clear();
	param_name.clear();
}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XSQL::~XSQL( void )
{
	param.clear();
#if X_BDE
	bdeDelete();
#endif
	singletonEnd();
}
//---------------------------------------------------------------------------
bool XSQL::singletonInit( void )
{
	singletonEnd();
#ifdef __BORLANDC__
	if ( enforce_singleton )	// FUTURE: RETURN FALSE IF TIMEOUT?
		{lock = new TCOMCriticalSection::Lock( thread_lock );
		}
#endif
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XSQL::singletonEnd( void )
{
#ifdef __BORLANDC__
	if ( NULL != lock )
		{delete lock;
		lock = NULL;
		}
#endif
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool XSQL::enforceSingleton( const bool es )
{
	enforce_singleton = es;
	return( true );
}
//---------------------------------------------------------------------------
std::string XSQL::getSQL( void )
{
	return( sql );
}
//---------------------------------------------------------------------------
bool XSQL::setSQL( const std::string query )
{
	sql = query;
	return( isValid() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XSQL::extendSQL( const std::string query )
{
	sql += query;
	return( isValid() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XSQL::setParamSource( const ROSETTA *src )
{
	param_source = src;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_BDE
bool XSQL::bdeApplyParams( void )
{
	int	n = pars->count();
	if ( n < 1 )
		{return( true );
		}
	int	i, typ;
	std::string	nam;
	const	XTIME	*t = NULL;
	const	XDATE 	*d = NULL;
	TByteDynArray	a;
	for ( i = 0; i < n; i++ )
		{
		nam = pars->getName( i );
		if ( NULL == qry->Params->FindParam( nam.c_str() ) )
			{continue;		// SKIP PARAMETERS NOT IN SQL
			}
		typ = pars->getType( i );
		try
			{
			switch( typ )
				{
				case ROSETTA::typeInt:
					qry->ParamByName( nam.c_str() )->AsInteger
						= pars->getInt( i );
					break;
				case ROSETTA::typeBool:
					qry->ParamByName( nam.c_str() )->AsBoolean
						= pars->getBool( i );
					break;
				case ROSETTA::typeReal:
					qry->ParamByName( nam.c_str() )->AsFloat
						= pars->getReal( i );
					break;
				case ROSETTA::typeString:
					if ( pars->hasTag( i, XSQL::blob ) )
						{qry->ParamByName( nam.c_str() )->AsMemo
							= pars->getString( i ).c_str();
						}
					else
						{qry->ParamByName( nam.c_str() )->AsString
							= pars->getString( i ).c_str();
						}
					break;
				case ROSETTA::typeTime:
					t = pars->pointerTime( i );
					if ( NULL == t || ! t->isValid() )
						{error( 0, "parameter XTIME is invalid" );
						}
					qry->ParamByName( nam.c_str() )->AsDateTime
						= t->outputTDateTime();
					break;
				case ROSETTA::typeDate:
					d = pars->pointerDate( i );
					if ( NULL == d || ! d->isValid() )
						{error( 0, "parameter XDATE is invalid" );
						}
					qry->ParamByName( nam.c_str() )->AsDate
						= d->outputTDateTime();
					break;
				case ROSETTA::typeBinob:
					a.Length = pars->pointerBinob(i)->size();
					memcpy( &a[0], pars->pointerBinob(i)->data(),
						pars->pointerBinob(i)->size() );
					qry->ParamByName( nam.c_str() )->AsBlob
						= a;
/* OLD PRE-XE CODE
					qry->ParamByName( nam.c_str() )->AsBlob
						= AnsiString( (const char *)
						pars->pointerBinob(i)->data(),
						pars->pointerBinob(i)->size() );
						TByteDynArray
*/
					break;
				case ROSETTA::typeLint:
				default:		// UNHANDLED TYPE
					error( 0,
						std::string("param type not valid for SQL, name " )
						+ nam );
					break;
				}
			}
		catch ( Exception &e )
			{error( 0, "failed to attach param" );
			}
		}
	return( isValid() );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XSQL::bdeConstruct( void )
{
	if ( NULL == qry )
		{qry = database->bdeSupplyQuery();
		}
	if ( NULL == qry )
		{error( 0, "failed to obtain query object" );
		return( false );
		}
	qry->SQL->Text = sql.c_str();
	if ( ! bdeApplyParams() )
		{return( false );
		}
	return( true );
}
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#if X_ING
bool XSQL::ingGetDescr( IIAPI_GETDESCRPARM *getDescrParm, II_PTR stmtHandle )
{
	getDescrParm->gd_genParm.gp_callback = NULL;
	getDescrParm->gd_genParm.gp_closure = NULL;
	getDescrParm->gd_stmtHandle = stmtHandle;
	getDescrParm->gd_descriptorCount = 0;
	getDescrParm->gd_descriptor = NULL;
	IIapi_getDescriptor( getDescrParm );	// INVOKE API
	return( ingGetResult( &getDescrParm->gd_genParm ) ); // FAILS IF TABLE NOT PRESENT
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XSQL::ingConstructParameters( void )
{
	const	char	*c = sql.c_str();
	bool	in_quotes = false;
	int	typ;
	param_name.clear();
	param_type.clear();
	while ( *c != 0  )      	// REPLACE PARAMETERS BY MARKERS
		{
		if ( '\'' == *c )
			{in_quotes = ! in_quotes;
			query_text += *c++;
			continue;
			}
		if ( in_quotes || *c != ':' )
			{query_text += *c++;
			continue;
			}
		query_text += ingPlaceholder();		// FOUND A :PARAM
		c++;
		std::string	pname = "";
		while( isalnum( *c ) || '_' == *c )
			{pname += *c++;
			}
		typ = pars->getType( pname );
		if ( -1 == typ )		// PARAM NOT SET
			{error( 0, std::string("No value for parameter :")
				+ pname );
			return( false );
			}
		param_name.push_back( pname ); // ADD TO ORDERED LIST OF PARAMETERS
		param_type.push_back( typ );
		}
	nparam = param_name.size();
	if ( debug_mode )
		{ingConstructDisplay();
		}
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
						/* DEBUG DISPLAY */
void XSQL::ingConstructDisplay( void )
{
	printf( "\nXSQL::ingConstructDisplay\n%s\n%d parameters",
		query_text.c_str(), param_name.size() );
	std::vector<std::string>::iterator vit = param_name.begin();
	int	count = 0;
	while( vit != param_name.end() )
		{
		printf( "\n%-4d %s", count++, vit->c_str() );
		if ( pars->exists( *vit ) )
			{std::string	v = pars->getString( *vit );
			printf( "%c %s", pars->getType( *vit ), v.c_str() );
			}
		else
			{printf( "MISSING" );
			}
		vit++;
		}
	printf( "\n" );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

bool XSQL::ingConstruct( void )
{
	query_text = "";
	if ( ! ingConstructParameters() ) // UPGRADE TO CALL ONLY WHEN TEXT CHANGED
		{return( false );
		}
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XSQL::ingDescribeUserParameters( IIAPI_DESCRIPTOR *desc )
{
	int	i;
	bool	ok = true;
	IIAPI_DESCRIPTOR *pdes;
	for ( i = 0; i < nparam; i++ )          // DESCRIBE ANY USER PARAMETERS
		{
		pdes = desc + i;
		pdes->ds_nullable = FALSE;
		pdes->ds_precision = 0;
		pdes->ds_scale = 0;
		pdes->ds_columnType = IIAPI_COL_QPARM;
		pdes->ds_columnName = NULL;
		switch( param_type[i] )
			{
			case ROSETTA::typeInt:
				pdes->ds_dataType = IIAPI_INT_TYPE;
				pdes->ds_length = sizeof( II_INT4 );
				break;
			case ROSETTA::typeLint:
				pdes->ds_dataType = IIAPI_INT_TYPE;
				pdes->ds_length = sizeof( II_INT8 );
				break;
			case ROSETTA::typeString:
				if ( pars->hasTag( param_name[i], XSQL::blob ) )
					{pdes->ds_dataType = IIAPI_LVCH_TYPE;
					pdes->ds_length = (II_UINT2)
						( 2 + XDB::blob_chunk_size );
					}
				else	// N.B. INGRES FAILS IF DS_LENGTH==0
					{pdes->ds_dataType = IIAPI_CHA_TYPE;
					int len = pars->getString( param_name[i] ).size();
					pdes->ds_length = (II_UINT2)
						( ( len > 0 ) ? len : 1 );
					}
				break;
			case ROSETTA::typeReal:
				pdes->ds_dataType = IIAPI_FLT_TYPE;
				pdes->ds_length = sizeof( II_FLOAT8 );
				break;
			case ROSETTA::typeDate:
			case ROSETTA::typeTime:
				pdes->ds_dataType = IIAPI_CHA_TYPE;
				pdes->ds_length = 25;
				break;
			case ROSETTA::typeBinob:
				if ( pars->hasTag( param_name[i], XSQL::blob ) )
					{pdes->ds_dataType = IIAPI_LBYTE_TYPE;
					pdes->ds_length = (II_UINT2)
						( 2 + XDB::blob_chunk_size );
					}
				else
					{pdes->ds_dataType = IIAPI_BYTE_TYPE;
					pdes->ds_length = (II_UINT2)
						pars->getBinob( param_name[i] ).size();
					}
				break;
			default:
				error( 0, "unsupported parameter type" );
				ok = false;
				break;
			}

		}
	return( ok );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
 /* INSERT BLOB FIELD AS SEGMENTS - BLOB_CHUNK_SIZE MUST FIT INTO SHORT-TYPE */
bool XSQL::ingPutUserLong( IIAPI_PUTPARMPARM *putp, const int total,
	const unsigned char *buf )
{
	int	start = 0;
	short	bcs = (short) XDB::blob_chunk_size; // MUST BE SHORT-TYPE FOR MEMORY-MAPPING
	IIAPI_DATAVALUE *d = putp->pp_parmData;
	d->dv_length = (II_UINT2) ( bcs + 2 );
	d->dv_value = (char *) malloc( bcs + 2 );
	while ( start + bcs < total )
		{
		memcpy( d->dv_value, (char *) &bcs, 2 );
		memcpy( ((char*)d->dv_value) + 2, buf + start, bcs );
		putp->pp_moreSegments = TRUE;
		IIapi_putParms( putp );
		if ( ! ingGetResult( &(putp->pp_genParm) ) )
			{return( false );
			}
		start += bcs;
		}
	short	remainder = (short) ( total - start );
	d->dv_length = (II_UINT2) ( remainder + 2 );
	memcpy( d->dv_value, (char *) &remainder, 2 );
	memcpy( ((char*)d->dv_value) + 2, buf + start, remainder );
	return( true );		// LAST SEGMENT INSERTED BY CALLING ROUTINE
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XSQL::ingPutUserParamsDo( IIAPI_PUTPARMPARM *putp )
{
	putp->pp_moreSegments = 0;
	IIapi_putParms( putp );
	if ( ! ingGetResult( &(putp->pp_genParm) ) )
		{return( false );
		}
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XSQL::ingPutUserParam( IIAPI_PUTPARMPARM *putp, const int indx )
{
	bool	ok = true;
	IIAPI_DATAVALUE *d = putp->pp_parmData;
	int	typ = param_type[indx];
	d->dv_null = FALSE;
	if ( ROSETTA::typeInt == typ )
		{
		d->dv_length = sizeof(II_INT4);
		d->dv_value = (II_INT4 *) malloc( sizeof(II_INT4) );
		*((II_INT4*)d->dv_value) = (II_INT4) pars->getInt( param_name[indx] );
		}
	else if ( ROSETTA::typeLint == typ )
		{
		d->dv_length = sizeof(II_INT8);
		d->dv_value = (II_INT8 *) malloc( sizeof(II_INT8) );
		*((II_INT8*)d->dv_value) = (II_INT8) pars->getLint( param_name[indx] );
		}
	else if ( ROSETTA::typeString == typ )
		{
		const	std::string *s = pars->pointerString( param_name[indx] );
		if ( pars->hasTag( param_name[indx], XSQL::blob ) )
			{ok &= ingPutUserLong( putp, s->size(),
				(const unsigned char *) s->c_str() );
			}
		else
			{d->dv_length = (II_UINT2) s->size();
			d->dv_value = (char *) malloc( s->size() + 1 );
			strcpy( (char *) d->dv_value, s->c_str() );
			}
		}
	else if ( ROSETTA::typeDate == typ )
		{  // FLAG IN BYTE 0 - DMY IN 1,2,3
		const	XDATE	*a = pars->pointerDate( param_name[indx] );
		if ( NULL == a || ! a->isValid() )
			{error( 0, "parameter XDATE is invalid" );
			}
		char	abuf[50];
		sprintf( abuf, "%d/%d/%4.4d",
			a->getDay(), a->getMonth(), a->getYear() );
		d->dv_length = 25;
		char	*c = (char *) malloc( 25 );
		strncpy( c, abuf, 25 );
		d->dv_value = c;
		}
	else if ( ROSETTA::typeTime == typ )
		{  // FLAG IN BYTE 0 - DMY IN 1,2,3 - MSEC IN BYTES 4+5
		const	XTIME	*t = pars->pointerTime( param_name[indx] );
		if ( NULL == t || ! t->isValid() )
			{error( 0, "parameter XTIME is invalid" );
			}
		char	tbuf[50];
		sprintf( tbuf, "%d/%d/%4.4d %2.2d:%2.2d:%2.2d",
			t->getDay(), t->getMonth(), t->getYear(),
			t->getHour(), t->getMinute(), t->getSecond() );
		d->dv_length = 25;
		char	*c = (char *) malloc( 25 );
		strncpy( c, tbuf, 25 );
		d->dv_value = c;
		}
	else if ( ROSETTA::typeReal == typ )
		{
		d->dv_length = sizeof( II_FLOAT8 );
		d->dv_value = (II_FLOAT8 *) malloc(sizeof(II_FLOAT8));
		*((II_FLOAT8*)d->dv_value) = (II_FLOAT8) pars->getReal(
			param_name[indx] );
		}
	else if ( ROSETTA::typeBinob == typ )
		{
		XBINOB b = pars->getBinob( param_name[indx] );
		if ( pars->hasTag( param_name[indx], XSQL::blob ) )
			{ok &= ingPutUserLong( putp, b.size(), b.data() );
			}
		else
			{d->dv_length = (II_UINT2) b.size();
			d->dv_value = (char *) malloc( d->dv_length + 1 );
			memcpy( (char *) d->dv_value, b.data(), d->dv_length );
			}
		}
	else
		{error( 0, "unsupported type" );
		ok = false;
		}
	if ( ok )
		{ok &= ingPutUserParamsDo( putp );
		}
	else
		{
		printf( "BUG" );
		}
	if ( NULL != d->dv_value )
		{free( d->dv_value );
		d->dv_value = NULL;
		}
	return( ok );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XSQL::ingPutUserParameters( IIAPI_PUTPARMPARM *putp )
{
	int	i;
	bool	ok = true;
	putp->pp_parmCount = 1; 		// INSERTING PARAMETERS SINGLY	
	for ( i = 0; i < nparam; i++ )
		{ok &= ingPutUserParam( putp, i );
		}
	return( ok );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XSQL::ingGetQueryInfo( IIAPI_GETQINFOPARM *qip, II_PTR stmtHandle )
{
	qip->gq_genParm.gp_callback = NULL;
	qip->gq_genParm.gp_closure = NULL;
	qip->gq_stmtHandle = stmtHandle;
	IIapi_getQueryInfo( qip );
	XDB::ingWait( &(qip->gq_genParm) );
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XSQL::ingClose( void )
{
	IIAPI_CLOSEPARM	closeParm;
	closeParm.cl_genParm.gp_callback = NULL;
	closeParm.cl_genParm.gp_closure = NULL;
	closeParm.cl_stmtHandle = queryParm.qy_stmtHandle;
	IIapi_close( &closeParm );
	return( ingGetResult( &closeParm.cl_genParm ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XSQL::ingCancel( void )
{
	IIAPI_CANCELPARM	cancelParm;
//	cancelParm.cn_genParm = NULL;	// GUESS AT VALUE ?
	cancelParm.cn_stmtHandle = queryParm.qy_stmtHandle;
	IIapi_cancel( &cancelParm );
	return( true );
//	return( IIgetResult( &closeParm.cl_genParm ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#endif
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void XSQL::assignParameterSource( void )
{
	pars = ( NULL != param_source ) ? param_source : &param;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XSQL::construct( void )
{
	if ( sql == "" )
		{error( 0, "empty SQL" );
		return( false );
		}
	if ( ! database->isValid() )
		{error( 0, "database not Valid" );
//		return( false );
		}
	if ( ! isValid() )
		{error( 0, "previous errors" );
//		return( false );
		}
	assignParameterSource();
#if X_BDE
	if ( ! bdeConstruct() )
		{return( false );
		}
#elif X_ING
	if ( ! ingConstruct() )
		{return( false );
		}
#endif
	if ( ! database->isOpen() )
		{if ( ! database->open() )
			{error( 0, "failed to open database" );
			return( false );
			}
		}
	return( true );
}
//---------------------------------------------------------------------------
int XSQL::getNRowsFetched( void ) const
{
	return( nrows_fetched );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XSQL::getNRowsAltered( void ) const
{
	return( nrows_altered );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
int XSQL::getReturnValue( void ) const
{
	return( return_value );
}
//---------------------------------------------------------------------------
	/* MAKE A STRING SAFE TO BE SPRINTF-ED AS AN ARGUMENT IN SQL */
std::string XSQL::makeSafeString( const std::string input )
{
	if ( input.empty() )
		{return( "" );
		}
	std::string	safe = "";
	const	char	*c = input.c_str();
	while ( *c != 0 )
		{safe += *c;
		if ( '\'' == *c )
			{safe += "'";
			}
		c++;
		}
	return( safe );
}
//===========================================================================


