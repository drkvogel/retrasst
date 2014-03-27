/*===========================================================================*/
/*		TREATMENT MINIMIZATION UTILITIES			*/
/*===========================================================================*/
#include <time.h>
#include "xbasic.h"
#include "xdb.h"
#include "xsql.h"
#include "xquery.h"
#include "xmz.h"
/*===========================================================================*/
XMZ_VAR::XMZ_VAR( const std::string title, const int nvalues,
	const std::string db_field )
	:
	label( title ),
	nvals( nvalues ),
	field( db_field ),
	role( XMZ_VAR::Dat )	// DEFAULT TO INPUT VARIABLE
{
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
XMZ_VAR::~XMZ_VAR( void )
{
}
/*---------------------------------------------------------------------------*/
bool XMZ_VAR::setRole( const int r )
{
	if ( XMZ_VAR::Dat == r || XMZ_VAR::Out == r || XMZ_VAR::Ign == r )
		{role = r;
		return( true );
		}
	return( false );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int XMZ_VAR::getRole( void ) const
{
	return( role );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
bool XMZ_VAR::addPast( const int past_value )
{
	if ( past_value < 0 || past_value >= nvals )
		{return( false );
		}
	past.push_back( past_value );
	return( true );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
std::string XMZ_VAR::getFieldName( void ) const
{
	return( field );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
std::string XMZ_VAR::getTitle( void ) const
{
	return( label );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int XMZ_VAR::getNVals( void ) const
{
	return( nvals );
}
/*===========================================================================*/
		/* ALLOCATE AND INITIALIZE TREATMENT STRATEGY STRUCTURE */
XMZ::XMZ( const std::string title )
	:
	label( title ),
	ncols( 0 ),
	npast( 0 ),
	trt( NULL ),
	debug( 0 )

{
//FOR SIMPLE ALLOCATION, E.G. X STRATA => NCOLS = X+1, (EXTRA IS TREATMENT)
//	PAST[A][B]	HOLDS VALUE FOR COL A, PERSON B
}
/*---------------------------------------------------------------------------*/
XMZ::~XMZ( void )
{
	std::vector<XMZ_VAR *>::iterator	ci = col.begin();
	while ( ci != col.end() )
		{delete *ci;
		ci++;
		}
}
/*---------------------------------------------------------------------------*/
void XMZ::setDebug( const int td )      // SET INTERNAL DEBUGGING PARAMETER
{
	debug = td;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
XMZ_VAR *XMZ::createVar( const std::string title, const int nvalues,
	const std::string db_field )
{
	XMZ_VAR	*v = new XMZ_VAR( title, nvalues, db_field );
	if ( NULL == v || ! v->isValid() )
		{setInvalid( "Bad addVar" );
		return( NULL );
		}
	col.push_back( v );
	ncols = col.size();
	return( v );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
XMZ_VAR *XMZ::pointerVar( const std::string title )
{
	std::vector<XMZ_VAR *>::const_iterator	ci = col.begin();
	while ( ci != col.end() )
		{if ( (*ci)->getTitle() == title )
			{return( *ci );
			}
		ci++;
		}
	return( NULL );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
XMZ_VAR *XMZ::pointerVar( const int indx )
{
	return( ( indx >= 0 && indx < (int) col.size() ) ? col[indx] : NULL );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
			/* ORDER TWO COUNTS ACCORDING TO THEIR METRICS */
int XMZ::sort_cmp_TREAT( const void *a, const void *b )
{
	double	rdif;
	XMZ_TREAT	*sca, *scb;
	sca = (XMZ_TREAT *) a;
	scb = (XMZ_TREAT *) b;
	rdif = sca->metric - scb->metric;
	if ( rdif < 0.0 )
		{return( -1 );
		}
	if ( rdif > 0.0 )
		{return( 1 );
		}
	return( 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* COUNT TREATMENTS FOR ENTRIES MATCHING CURRENT PERSON */
void XMZ::countMatch( const int nrows, std::vector<int> *past_val,
	std::vector<int> *past_trt, const int match, XMZ_TREAT *tcount )
{	int	i;
	for ( i = 0; i < nrows; i++ )
		{if ( (*past_val)[i] == match )
			{tcount[ (*past_trt)[i] ].count++;
			}
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* MARK AS NOT EVALUATED */
void XMZ::clearResults( void )
{
	if ( NULL != trt )
		{delete [] trt;
		trt = NULL;
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
						/* DUMP INTERNALS FOR DEBUG */
void XMZ::dump( XMZ_VAR *ctreat )
{
	int	i, j;
	char	buf[500], tmp[50];
	for ( i = 0; i < ctreat->nvals; i++ )
		{sprintf( buf, "rank[%d]\tindx %d\tcount=%d\tmetric=%lg",
			i, trt[i].index,
			trt[i].count, trt[i].metric );
		}
	if ( debug & 2 )		/* SHOW ALL PAST STORAGE */
		{
		for ( i = 0; i < npast; i++ )
			{sprintf( buf, "PAST %d", i );
			for ( j = 0; j < ncols; j++ )
				{sprintf( tmp, " %d", col[j]->past[i] );
				}
			}
		}
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
				/* ALLOCATE TREATMENT USING MINIMIZATION */
			/* ANALYSE ONLY FOR COLUMNS WITH +VE WEIGHT */
bool XMZ::eval( const std::vector<int> *current, const std::vector<int> *weight )
{
	int	i, cur;
	int	treat_col = -1, ndat = 0, nign = 0;
	XMZ_VAR	*cval, *ctreat;
	clearResults();
	if ( NULL == current || (int) current->size() != ncols )
		{setInvalid( "eval, current is NULL or has incorrect size" );
		return( false );
		}
	for ( i = 0; i < ncols; i++ )
		{switch( col[i]->role )
			{case XMZ_VAR::Out:
				if ( treat_col >= 0 )
					{setInvalid("eval, multiple outputs");
					return( false );
					}
				treat_col = i;
				break;
			case XMZ_VAR::Dat:
				ndat++;
				break;
			case XMZ_VAR::Ign:
				nign++;
				break;
			default:
				setInvalid( "eval, invalid role" );
				return( false );
				break;
			}
		}
//	aside( "eval treatcol=%d, %d data, %d ignored",	treat_col, ndat, nign );
	if ( treat_col < 0 || treat_col >= ncols )
		{setInvalid( "eval, no treatment/output item" );
		return( false );
		}
	ctreat = col[treat_col];
	treat_nvals = ctreat->nvals;
	if ( NULL != weight && (int) weight->size() != treat_nvals )
		{setInvalid( "eval, weight has incorrect size" );
		return( false );
		}
	trt = new XMZ_TREAT[ctreat->nvals];
	for ( i = 0; i < ctreat->nvals; i++ )
		{
		trt[i].index = i;
		trt[i].count = 0;
		}
	for ( i = 0; i < ncols; i++ )
		{
		if ( XMZ_VAR::Dat == col[i]->role )
			{
			cval = col[i];
			cur = (*current)[i];
			if ( cur < 0 || cur >= cval->nvals )
				{setInvalid( "eval: illegal current" );
				clearResults();
				return( false );
				}
			if ( (int) cval->past.size() != npast )
				{setInvalid( "eval, npast mis-match" );
				clearResults();
				return( false );
				}
			countMatch( npast, &(cval->past), &(ctreat->past),
				cur, trt );
			}
		}				/* FIND LOWEST METRIC */
	for ( i = 0; i < ctreat->nvals; i++ )
		{trt[i].metric = (double) trt[i].count;
		if ( NULL != weight )
			{const	int	wt = (*weight)[i];
			if ( wt < 1 || wt > 1000 )
				{char	buf[500];
				sprintf( buf, "eval: weight[%d]=%d is invalid",
					i, wt );
				setInvalid( buf );
				clearResults();
				return( false );
				}
			trt[i].metric /= (double) wt;
			}
		trt[i].metric += 0.001 * rand.getRandom01();  // RANDOMIZE TIES
		}
	qsort( trt, (size_t) ctreat->nvals, sizeof(XMZ_TREAT),
		sort_cmp_TREAT );
	if ( debug > 0 )
		{dump( ctreat);
		}
	return( true );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* SELECT CHOICE OF TREATMENT BY RANK OF GOODNESS, 0 IS BEST */
int XMZ::select( int rank )
{
	if ( NULL == trt )
		{setInvalid( "select: eval not called successfully" );
		return( -1 );
		}
	if ( rank < 0 || rank >= treat_nvals )
		{setInvalid( "select, rank out of range" );
		return( -1 );
		}
	return( trt[ rank ].index );
}
/*---------------------------------------------------------------------------*/
		/* COUNT NUMBER OF PEOPLE WITH A PARTICULAR VAL IN A COL */
int XMZ::countVal( const int column, const int val )
{	int	i;
	int	sum = 0;
	if ( column < 0 || column >= (int) col.size() )
		{return( -1 );
		}
	for ( i = 0; i < npast; i++ )
		{if ( val == col[column]->past[i] )
			{sum++;
			}
		}
	return( sum );
}
/*---------------------------------------------------------------------------*/
						/* ADD NEW DATA TO COUNTERS */
bool XMZ::addPast( const std::vector<int> *val )
{
	int	i;
	clearResults();				/* MARK AS NOT EVALUATED */
	if ( (int) val->size() != ncols )
		{setInvalid( "column count mis-match" );
		return( false );
		}
	for ( i = 0; i < ncols; i++ )
		{
		if ( ! col[i]->addPast( (*val)[i] ) )
			{char	buf[500];
			sprintf( buf, "invalid val[%d]=%d", i, (*val)[i] );
			setInvalid( buf );
			return( false );
			}
		}
	npast++;
	return( true );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
int XMZ::getNPast( void ) const
{
	return( npast );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
		/* ADD NEW DATA TO COUNTERS DIRECTLY FROM DATABASE */
int XMZ::addFromDB( XDB *d, const std::string table, const std::string where )
{
	int	i;
	int	ndone = 0, error_count = 0;
	std::string	sql = std::string("SELECT ") + col[0]->getFieldName();
	for ( i = 1; i < ncols; i++ )
		{
		sql += std::string( "," ) + col[i]->getFieldName();
		}
	sql += std::string( " FROM " ) + table + " WHERE rec_status=0";
	if ( ! where.empty() )
		{sql += std::string( " AND " ) + where;
		}
	XQUERY	qm( d, sql );
	bool	db_open = d->isOpen();
	if ( ! db_open )
		{if ( ! d->open() )
			{return( -1 );
			}
		}
	if ( ! qm.open() )
		{return( -1 );
		}
	std::vector<int>	vals( ncols );
	while ( qm.fetch() )
		{for ( i = 0; i < ncols; i++ )
			{vals[i] = qm.result.getInt( i );
			}
		if ( ! addPast( &vals ) )
			{error_count++;
			}
		ndone++;
		}
	qm.close();
	if ( ! db_open )
		{d->close();			// RESTORE INITIAL STATE
		}
//	aside( "addFromDB: %d read, %d errors", ndone, error_count );
	return( error_count );
}
/*===========================================================================*/

