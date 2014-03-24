//---------------------------------------------------------------------------

#ifndef LPDbCryovialStoreH
#define LPDbCryovialStoreH

#include "LDbIdClasses.h"
#include "LDbNotes.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LPDbCryovialStore : public LPDbID, public LDbNoteCount
{
	int cryovialID, boxID, retrievalID;
	short status, position;
	float volume;

public:

	enum Status { ALLOCATED, CONFIRMED, MOVE_EXPECTED, DESTROYED, ANALYSED, TRANSFERRED, NOT_FOUND, DELETED = 99 };

	LPDbCryovialStore() : LDbNoteCount( 0 ), status( ALLOCATED ),
		 cryovialID( 0 ), retrievalID( 0 ), boxID( 0 ), position( 0 ), volume( -1 )
	{}

	LPDbCryovialStore( int id, int cryovial, int box, short pos )
	 : LPDbID( id ), LDbNoteCount( 0 ), status( ALLOCATED ), volume( -1 ),
	   cryovialID( cryovial ), retrievalID( 0 ), boxID( box ), position( pos )
	{}

	LPDbCryovialStore( const LQuery & query );

	void setStatus( Status update ) { status = update; }
	Status getStatus() const { return Status( status ); }
    int getPosition() const { return position; }
    int getBoxID() const { return boxID; }

	bool saveRecord( LQuery query );
	bool setJobRef( LQuery project, int jobRef );    
};

//---------------------------------------------------------------------------

struct LPDbCryovialStores : public LDbCache< LPDbCryovialStore >
{
	bool readMarked( LQuery ddbQuery );
	int confirmAllocation( LQuery pQuery, int boxID );
};

//---------------------------------------------------------------------------

#endif

