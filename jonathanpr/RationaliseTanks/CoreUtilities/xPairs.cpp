#include <vcl.h>
#pragma hdrstop
#include "xpairs.h"
//---------------------------------------------------------------------------
#define	XPAIR_VAL_TYPE_INT	1
#define	XPAIR_VAL_TYPE_STRING	2
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define	XPAIR_EMPTY_INT		(-9991999)
#define	XPAIR_EMPTY_STRING	"[NULL]"
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XPAIR_VAL::XPAIR_VAL( String id, int ival )
	:
	name( id ),
	type( 'i' ),
	ivalue( ival ),
	svalue( XPAIR_EMPTY_STRING )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XPAIR_VAL::XPAIR_VAL( String id, String sval )
	:
	name( id ),
	type( 's' ),
	ivalue( XPAIR_EMPTY_INT ),
	svalue( sval )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XPAIR_VAL::~XPAIR_VAL( void )
{
}
//---------------------------------------------------------------------------
XPAIRS::XPAIRS( void )
	:
	npairs( 0 )
{
	pair = (XPAIR_VAL **) malloc( 12 * sizeof(XPAIR_VAL) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XPAIRS::~XPAIRS( void )
{	free( pair );
}
//---------------------------------------------------------------------------
void XPAIRS::extend( void )
{	if ( 0 == ++npairs % 10 )
		{pair = (XPAIR_VAL **) realloc( pair, ( npairs + 12 )
			* sizeof(XPAIR_VAL *) );
		}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XPAIRS::set_int( String id, int ival )
{
	XPAIR_VAL *v = get_value( id );
	if ( NULL == v )
		{pair[npairs] = new XPAIR_VAL( id, ival );
		extend();
		}
	else
		{v->ivalue = ival;
		}
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool XPAIRS::set_string( String id, String sval )
{
	XPAIR_VAL *v = get_value( id );
	if ( NULL == v )
		{pair[npairs] = new XPAIR_VAL( id, sval );
		extend();
		}
	else
		{v->svalue = sval;
		}
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
XPAIR_VAL *XPAIRS::get_value( String id )
{
	int	i;
	for ( i = 0; i < npairs; i++ )
		{if ( pair[i]->name == id )
			{return( pair[i] );
			}
		}
	return( NULL );
}
//---------------------------------------------------------------------------
#pragma package(smart_init)

