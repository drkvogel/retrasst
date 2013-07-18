//---------------------------------------------------------------------------

#ifndef LPDbCryovialH
#define LPDbCryovialH

#include "LDbIdClasses.h"
#include "LDbNotes.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LPDbCryovial : public LPDbID, public LDbNoteCount
{
	std::string barcode;
	int boxID, sampleID, typeID, storeID, retrievalID;
	short status, position;

public:
	enum Status { EXPECTED  = 0, STORED = 1, SPLIT = 2, DESTROYED = 3, DELETED = 99 };

	LPDbCryovial() : LDbNoteCount( 0 ), status( EXPECTED ), retrievalID( 0 ),
	   sampleID( 0 ), storeID( 0 ), typeID( 0 ), boxID( 0 ), position( 0 )
	{}

	LPDbCryovial( const std::string & id, int source, int aliquot, short noteCount )
	 : LDbNoteCount( noteCount ), barcode( id ), status( EXPECTED ), retrievalID( 0 ),
	   sampleID( source ), typeID( aliquot ), storeID( 0 ), boxID( 0 ), position( 0 )
	{}

	LPDbCryovial( const LQuery & query );

	const std::string & getBarcode() const { return barcode; }
	int getAliquotType() const { return typeID; }

	void setStatus( Status update ) { status = update; }
	Status getStatus() const { return Status( status ); }

	void setPosition( int box, short pos )
	{
		boxID = box; position = pos;
	}

	int getBoxID() const { return boxID; }
	short getPosition() const { return position; }
	int getSampleID() const { return sampleID; }

	bool saveRecord( LQuery query );
//	bool setJobRef( LQuery project, int jobRef );
};

//---------------------------------------------------------------------------

class LPDbCryovials : public LDbCache< LPDbCryovial >
{
	class Matcher;
	static std::string selectFields();

public:

	const LPDbCryovial * find( const std::string & barcode, int typeCID ) const;
	bool readBySourceID( LQuery project, int specimenID );
	bool readByBoxID( LQuery project, int boxCID );
	bool readByStatus( LQuery project, LPDbCryovial::Status filter );
	const LPDbCryovial * readRecord( LQuery pQuery, const std::string & barcode, int typeID );
	const LPDbCryovial * readRecord( LQuery pQuery, int box, short pos );
	int confirmAllocation( LQuery query, int boxID );
};

//---------------------------------------------------------------------------

#endif

