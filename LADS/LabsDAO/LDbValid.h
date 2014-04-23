#ifndef LDbValidH
#define LDbValidH

#include <System.hpp>
#include <utility>
#include "xtime.h"

//---------------------------------------------------------------------------

// Ingres, C, XTIME and the labs databases only agree on recent dates
static const XDATE EPOCH_START( 1980, 1, 1 ), EPOCH_END( 2037, 12, 31 );

//---------------------------------------------------------------------------

class LDbValid
{
	TDateTime start, end;

	static TDateTime checkDate( TDateTime ts );

protected:

	short status;

	LDbValid();
	LDbValid( TDateTime from, TDateTime to, short rs );

public:

	enum Stage { NEW_RECORD = 0, RECORD_IN_USE = 1, ARCHIVED = 2, DELETED = 99 };

	std::pair< TDateTime, TDateTime > getValidDates() const;
	void setValidDates( std::pair< TDateTime, TDateTime > range );

	bool isActive() const;
	bool isNearExpiry() const;
	bool isDormant() const;
	void setActive( bool active );
};

//---------------------------------------------------------------------------

#endif

