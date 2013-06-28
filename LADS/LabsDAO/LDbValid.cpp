/*---------------------------------------------------------------------------
 *
 *	Base class for lab objects with a status, start and end dates
 *
 *	19 July 2010: 	Always set dates, not times; end > today if active
 *                	Moved nearExpiry() here from LCDbObject
 *	26/6/12, NG:    Allow for no end (useful if XTIME wraps round in 2037)
 *
 *--------------------------------------------------------------------------*/

#include "LDbValid.h"
#include <functional>
#include <Controls.hpp>

//---------------------------------------------------------------------------

static const TDate EPOCH_START( 1980, 1, 1 ), EPOCH_END( 2037, 12, 31 );

TDateTime checkDate( TDateTime ts ) {
	if( ts < EPOCH_START ) {
		return EPOCH_START;
	} else if( ts > EPOCH_END ) {
		return EPOCH_END;
	} else {
		return ts;
    }
}

//---------------------------------------------------------------------------

LDbValid::LDbValid( )
 : start( TDateTime::CurrentDate() ), end( EPOCH_END ), status( NEW_RECORD ) {
}

//---------------------------------------------------------------------------

LDbValid::LDbValid( TDateTime from, TDateTime to, short rs )
 : start( from ), end( to ), status( rs ) {
}

//---------------------------------------------------------------------------

void LDbValid::setValidDates( std::pair< TDateTime, TDateTime > range ) {
	start = range.first;
	end = range.second;
}

//---------------------------------------------------------------------------

std::pair< TDateTime, TDateTime >LDbValid::getValidDates( ) const {
	return std::pair< TDateTime, TDateTime >( start, end );
}

//---------------------------------------------------------------------------

bool LDbValid::isActive( ) const {
	if( status == DELETED ) {
		return false;
	}
	TDateTime now = Now();
	return (start < now || start >= EPOCH_END) && (end > now || EPOCH_START >= end);
}

//---------------------------------------------------------------------------

bool LDbValid::isNearExpiry() const {
	TDateTime today = TDateTime::CurrentDate();
	return status != DELETED && end >= today && end < today + 14;
}

//---------------------------------------------------------------------------

bool LDbValid::isDormant() const {
	return status != DELETED && start < end && end < TDateTime::CurrentDate();
}

//---------------------------------------------------------------------------

void LDbValid::setActive( bool active ) {
	TDateTime now = Now();
	if( start > now || EPOCH_START >= start ) {
		start = now;
	}
	if( active ) {
		status = RECORD_IN_USE;
		if( end < now ) {
			end = EPOCH_END;
		}
	} else {
		status = DELETED;
		if( end > now || EPOCH_START >= end ) {
			end = now;
		}
	}
}

//---------------------------------------------------------------------------

