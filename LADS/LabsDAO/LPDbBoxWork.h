//---------------------------------------------------------------------------

#ifndef LPDbBoxWorklistH
#define LPDbBoxWorklistH

#include "LDbIdClasses.h"
#include "LDbCacheBase.h"

//#include <DBTables.hpp>
#include <set>

//---------------------------------------------------------------------------

class LPDbSource;

class LPDbBoxEntry : public LDbIdBase
{
	String source, destination, profile;
	int boxTypeID, sampleID, groupID, categoryID;
	char status;
	TDateTime timeStamp;

public:

	static const char NEW_ENTRY = 'Q', ALLOCATED = 'P', CREATED = 'C',
					  REMOVED = 'N', CONFIRMED = 'T';

	LPDbBoxEntry()
	 : LDbIdBase( 0 ), boxTypeID( 0 ), sampleID( 0 ), groupID( 0 ), categoryID( 0 ), status( NEW_ENTRY )
	{}

	LPDbBoxEntry( const LPDbSource & sample, int btid );
	explicit LPDbBoxEntry( const LQuery & query );

	int getSampleID() const { return sampleID; }
	void setSampleID( int specimen ) { sampleID = specimen; }

	const String & getSource() const { return source; }
	void setSource( const String & barcode ) { source = barcode; }

	const String & getDestination() const { return destination; }
	void setDestination( const String & cryovial ) { destination = cryovial; }

	TDateTime getTimeStamp();
	void setTimeStamp( TDateTime time ) { timeStamp = time; }

	int getBoxTypeID() const { return boxTypeID; }
	void setBoxTypeID( int content ) { boxTypeID = content; }

	int getCategoryID() const { return categoryID; }
	void setCategoryID( int filter ) { categoryID = filter; }

	int getGroupID() const { return groupID; }
	void setGroupID( int group ) { groupID = group; }

	char getStatus() const { return status; }
	void setStatus( char newState ) { status = newState; }
	void saveRecord( LQuery central, const String & reason );
};

//---------------------------------------------------------------------------

class LPDbBoxList : public LDbCache< LPDbBoxEntry >, public LPDbCacheMap< LPDbBoxList >
{
	class BCMatch;
	class IDMatch;

public:

	bool readCurrent( LQuery query );
	const LPDbBoxEntry * find( const String & barcode, int boxType ) const;
	const LPDbBoxEntry * find( int sampleID, int boxType ) const;
	int findGroup( LQuery cQuery, const std::set< int > & sampleIDs ) const;
	int update( LQuery ddbQuery, int boxID, char newState );
	unsigned deleteOld( LQuery central, TDateTime flagged, TDateTime oldest );
};

//---------------------------------------------------------------------------

#endif
