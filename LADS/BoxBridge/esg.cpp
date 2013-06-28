//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include "esg.h"
//---------------------------------------------------------------------------
ESG_VAL::ESG_VAL( void )
	:
	bg_colour_defined( false ),
	fg_colour_defined( false )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ESG_VAL::ESG_VAL( const TColor bg, const TColor fg )
	:
	bg_colour_defined( true ),
	fg_colour_defined( true ),
	bg_colour_val( bg ),
	fg_colour_val( fg )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ESG_VAL::setBColour( const TColor c )
{
	bg_colour_defined = true;
	bg_colour_val = c;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ESG_VAL::setFColour( const TColor c )
{
	fg_colour_defined = true;
	fg_colour_val = c;
}
//---------------------------------------------------------------------------
ESG_ARRAY::ESG_ARRAY( ESG *e )
	:
	owner( e ),
	nval( 0 )
{
	val = (ESG_VAL **) malloc( sizeof(ESG_VAL *) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ESG_ARRAY::~ESG_ARRAY( void )
{
	int	i;
	for ( i = 0; i < nval; i++ )
		{delete val[i];
		}
	free( val );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ESG_ARRAY::extend( const int new_size )
{
	int	i;
	void 	*ptr = realloc( val, new_size * sizeof(ESG_VAL *) );
	if ( NULL == ptr )
		{OutOfMemoryError();
		return;
		}
	val = (ESG_VAL **) ptr;
	for ( i = nval; i < new_size; i++ )
		{val[i] = NULL;
		}
	nval = new_size;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
				/* GET VALUE OBJECT - CREATE IF NECESSARY */
ESG_VAL *ESG_ARRAY::getVal( const int pos )
{
	if ( pos >= nval )
		{extend( pos + 10 );	// ADD MARGIN TO REDUCE CPU OVERHEAD
		}
	if ( NULL == val[pos] )
		{val[pos] = new ESG_VAL();
		}
	return( val[pos] );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			/* RETURN VALUE OBJECT - NULL IF NOT EXISTS */
ESG_VAL *ESG_ARRAY::returnVal( const int pos )
{
	return( ( pos < 0 || pos >= nval ) ? NULL : val[pos] );
}
//---------------------------------------------------------------------------
ESG_P::ESG_P( ESG *e, const int ic, const int ir )
	:
	owner( e ),
	icol( ic ),
	irow( ir )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TColor ESG_P::getBColour( void ) const
{
	return( owner->getBColour( icol, irow ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ESG_P::setBColour( const TColor c )
{
	owner->setBColour( icol, irow, c );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TColor ESG_P::getFColour( void ) const
{
	return( owner->getFColour( icol, irow ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ESG_P::setFColour( const TColor c )
{
	owner->setFColour( icol, irow, c );
}
//---------------------------------------------------------------------------
ESG_C::ESG_C( ESG *e, const int ic )
	:
	icol( ic ),
	owner( e )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ESG_P ESG_C::operator[]( const int ir )
{
	return( ESG_P( owner, icol, ir ) );
}
//---------------------------------------------------------------------------
ESG_CELL::ESG_CELL( ESG *e )
	:
	owner( e )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

ESG_C ESG_CELL::operator[]( const int ic )
{
	return( owner->get_col( ic ) );
}
//---------------------------------------------------------------------------
ESG_ROW::ESG_ROW( ESG *e )
	:
	owner( e )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ESG_P ESG_ROW::operator[]( const int ir )
{
	return( ESG_P( owner, -1, ir ) );
}
//---------------------------------------------------------------------------
ESG_COL::ESG_COL( ESG *e )
	:
	owner( e )
{
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ESG_P ESG_COL::operator[]( const int ic )
{
	return( ESG_P( owner, ic, -1 ) );
}
//---------------------------------------------------------------------------

ESG::ESG( TStringGrid *sg )
	:
	ncols( 0 ),
	host( sg ),
	canv( sg->Canvas ),
	Cells( this ),
	Row( this ),
	Col( this )
{
	default_value = new ESG_VAL( sg->Color, sg->Font->Color );
	dummy_value = new ESG_VAL( clBlack, clWhite );

	row = new ESG_ARRAY( this );
	col = new ESG_ARRAY( this );
	cols = (ESG_ARRAY **) malloc( sizeof(ESG_ARRAY *) );
	cols[0] = NULL;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ESG::~ESG( void )
{
	int	i;
	delete	default_value;
	delete	dummy_value;
	delete	row;
	delete	col;
	for ( i = 0; i < ncols; i++ )
		{delete cols[i];
		}
	free( cols );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ESG::extend_cols( const int new_size )
{
	int	i;
	void 	*ptr = realloc( cols, new_size * sizeof(ESG_ARRAY **) );
	if ( NULL == ptr )
		{OutOfMemoryError();
		return;
		}
	cols = (ESG_ARRAY **) ptr;
	for ( i = ncols; i < new_size; i++ )
		{cols[i] = NULL;
		}
	ncols = new_size;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ESG_C ESG::get_col( const int ic )
{
	return( ESG_C( this, ic ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		/* RETURN POINTER TO VAL OBJECT, CREATING IF NECESSARY */
ESG_VAL *ESG::ptr_val( const int ic, const int ir )
{
	if ( ic < 0 )
		{return( dummy_value );
		}
	if ( ic >= ncols )
		{extend_cols( ic+1 );
		}
	if ( NULL == cols[ic] )
		{cols[ic] = new ESG_ARRAY( this );
		}
	return( cols[ic]->getVal( ir ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	/* SET BACKGROUND COLOUR OF CELL WITH CO-ORDINATES [NC][NR] */
void ESG::setBColour( const int ic, const int ir, const TColor c )
{
	if ( ic >= 0 )
		{if ( ir >= 0 )			// SET INDIVIDUAL CELL
			{ptr_val( ic, ir )->setBColour( c );
			}
		else				// SET WHOLE COLUMN
			{col->getVal( ic )->setBColour( c );
			}
		}
	else if ( ir >= 0 )			// SET WHOLE ROW
		{row->getVal( ir )->setBColour( c );
		}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	/* SET FOREGROUND (TEXT) COLOUR OF CELL WITH CO-ORDINATES [NC][NR] */
void ESG::setFColour( const int ic, const int ir, const TColor c )
{
	if ( ic >= 0 )
		{if ( ir >= 0 )			// SET INDIVIDUAL CELL
			{ptr_val( ic, ir )->setFColour( c );
			}
		else				// SET WHOLE COLUMN
			{col->getVal( ic )->setFColour( c );
			}
		}
	else if ( ir >= 0 )			// SET WHOLE ROW
		{row->getVal( ir )->setFColour( c );
		}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	/* GET BACKGROUND COLOUR OF CELL WITH CO-ORDINATES [NC][NR] */
TColor	ESG::getBColour( const int ic, const int ir ) const
{
	ESG_VAL	*v;
	if ( ic >= 0 && ic < ncols && NULL != cols[ic]
			&& NULL != ( v = cols[ic]->returnVal( ir ) )
			&& v->bg_colour_defined )
		{return( v->bg_colour_val );		// EXACT CELL MATCH
		}
	else if ( NULL != ( v = row->returnVal( ir ) )
			&& v->bg_colour_defined )
		{return( v->bg_colour_val );		// MATCH ON ROW
		}
	else if ( NULL != ( v = col->returnVal( ic ) )
			&& v->bg_colour_defined )
		{return( v->bg_colour_val );		// MATCH ON COLUMN
		}
	return( default_value->bg_colour_val );		// RETURN DEFAULTS
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	/* GET FOREGROUND COLOUR OF CELL WITH CO-ORDINATES [NC][NR] */
TColor	ESG::getFColour( const int ic, const int ir ) const
{
	ESG_VAL	*v;
	if ( ic >= 0 && ic < ncols && NULL != cols[ic]
			&& NULL != ( v = cols[ic]->returnVal( ir ) )
			&& v->fg_colour_defined )
		{return( v->fg_colour_val );		// EXACT CELL MATCH
		}
	else if ( NULL != ( v = row->returnVal( ir ) )
			&& v->fg_colour_defined )
		{return( v->fg_colour_val );		// MATCH ON ROW
		}
	else if ( NULL != ( v = col->returnVal( ic ) )
			&& v->fg_colour_defined )
		{return( v->fg_colour_val );		// MATCH ON COLUMN
		}
	return( default_value->fg_colour_val );		// RETURN DEFAULTS
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
void ESG::DrawCell( const int ACol, const int ARow, const TRect &Rect )
{
	canv->Brush->Color = getBColour( ACol, ARow );
	canv->FillRect( Rect );
	canv->Font->Color = getFColour( ACol, ARow );
	canv->TextOut( Rect.Left + 1,
		( Rect.Bottom + Rect.Top - abs( canv->Font->Height ) ) / 2,
		host->Cells[ACol][ARow] );
}
//---------------------------------------------------------------------------
#pragma package(smart_init)
