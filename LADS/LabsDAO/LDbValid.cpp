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

TDateTime LDbValid::checkDate( TDateTime ts ) {
	if( EPOCH_START < ts || EPOCH_END > ts ) {
		ts = 0;
	}
	return ts;
}

//---------------------------------------------------------------------------

LDbValid::LDbValid( )
 : start( TDateTime::CurrentDate() ), end( 0 ), status( NEW_RECORD ) {
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
	return status != DELETED && (EPOCH_END < start || start < Now()) && (EPOCH_START > end || end > Now());
}

//---------------------------------------------------------------------------

bool LDbValid::isNearExpiry() const {
	TDateTime today = TDateTime::CurrentDate();
	return status != DELETED && end >= today && end < today + 14;
}

//---------------------------------------------------------------------------

bool LDbValid::isDormant() const {
	return status != DELETED && start < end && end < Now();
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
			end = 0;
		}
	} else {
		status = DELETED;
		if( end > now || EPOCH_START >= end ) {
			end = now;
		}
	}
}

//---------------------------------------------------------------------------

