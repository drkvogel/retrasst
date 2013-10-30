#include "rosig.h"
#include "xmd5.h"
#include <algorithm>
//---------------------------------------------------------------------------
bool ROSIG::filter_null( const ROSE_BASE *b )
{			// INCLUDE ALL ITEMS
	return( true );
}
//---------------------------------------------------------------------------
bool ROSIG::filter_underscore( const ROSE_BASE *b )
{               	// OMIT ANY ITEM WHO'S NAME STARTS WITH AN UNDER_SCORE
	std::string	s = b->getName();
	const	char	*c = s.c_str();
	return( NULL != c && '_' != *c );
}
//---------------------------------------------------------------------------
ROSIG::ROSIG( const ROSETTA *data, ROSETTA_FILTER filter )
	:
	ROSETTA(),
	target( data ),
	filter_mask( filter ),
	is_valid( true )
{
	if ( NULL == data )
		{is_valid = false;
		return;
		}
	identify();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
ROSIG::~ROSIG( void )
{
}
//---------------------------------------------------------------------------
bool ROSIG::isValid( void ) const
{
	return( is_valid );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSIG::identifyRos( const ROSETTA *r )
{
	std::vector<ROSE_BASE *>::const_iterator it = r->pairs.begin();
	while ( it != r->pairs.end() )
		{if ( NULL == filter_mask || filter_mask( *it ) )
			{if ( ROSETTA::typeRosetta != (*it)->type() )
				{pairs_filter.push_back( *it );
				}
			else		// PROCESS RECURSIVELY
				{identifyRos( ((ROSE_ROSETTA *)(*it))->val );
				}
			}
		it++;
		}
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	/* IDENTIFY WHICH ELEMENTS ARE TO BE CHECKED (NULL MASK => ALL) */
bool ROSIG::identify( void )
{
	pairs_filter.clear();
	if ( ! is_valid )
		{return( false );
		}
	identifyRos( target );
	std::sort( pairs_filter.begin(), pairs_filter.end(), ROSETTA_compare );
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
std::string ROSIG::getSignature( void ) const
{
	std::string	total;
	std::vector<ROSE_BASE *>::const_iterator it = pairs_filter.begin();
	while ( it != pairs_filter.end() )
		{
		total += "#";
		total += (*it)->getName();
		total += "=";
		total += (*it)->getString();
		it++;
		}
	total += "#";
	std::string	sig = std::string( "MD5:" )
		+ XMD5::calcHex( (char *) total.c_str() );
	return( sig );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
bool ROSIG::verifySignature( const std::string sig ) const
{	return( getSignature() == sig );
}
//---------------------------------------------------------------------------
#pragma package(smart_init)

