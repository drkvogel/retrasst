#ifndef LDbSourceH
#define LDbSourceH

#include "LDbStage.h"
#include "LDbNotes.h"
#include "ValueFields.h"
#include "LPDbProfileMap.h"
#include <set>

//---------------------------------------------------------------------------
//	Base class for all samples - buddy_database, specimen and XML
//---------------------------------------------------------------------------

class LDbSource : public LDbStage, public LDbNoteCount
{
	static const TDate FIRST_DATE;

	std::string barcode;
	TDateTime update;

protected:

	LDbSource( const std::string & sampleID = "" ) : barcode( sampleID )
	{}

public:

//	bool empty() const { return barcode.IsEmpty(); }
	const std::string & getBarcode() const { return barcode; }
	void setBarcode( const std::string & sampleID ) { barcode = sampleID; }
	bool isQC() const { return barcode.substr(0,2)== "QC"; }

	TDateTime getTimeStamp() const { return update; }
	void setTimeStamp( TDateTime when ) { update = when; }
};

//---------------------------------------------------------------------------
//	buddy_database entry - adds buddy_sample_id and link to specimen
//---------------------------------------------------------------------------

class LBDbSource : public LDbSource, public LBDbID
{
	int buddyID;
	std::string testType, sampleType;

protected:

	std::pair< int, int > specimen;
	std::pair< int, short > position;

	LBDbSource()
	 : LDbSource( "" ), buddyID( 0 ), specimen( 0, 0 ), position( 0, 0 )
	{}
	
public:

	LBDbSource( const std::string & barcode, TDateTime programmed );

	explicit LBDbSource( const LQuery & query );
	bool saveRecord( LQuery cluster );

	int getMachineID() const { return buddyID; }

	void setSpecimen( std::pair< int, int > sample ) { specimen = sample; }
	const std::pair< int, int > & getSpecimen() const { return specimen; }

	void setPosition( std::pair< int, short > cup ) { position = cup; }
	void setPosition( short cup ) { position.first = 0; position.second = cup; }
	const std::pair< int, short > & getPosition() const { return position; }

	const std::string & getSampleType() const { return sampleType; }
	void setSampleType( const std::string & type ) { sampleType = type; }

	std::string getTestType() const { return testType; }
	void setTestType( const std::string & type ) { testType = type; }

	static char action( Stage stage );
	static Stage makeStage( char action );
};

//---------------------------------------------------------------------------

class LBDbSources : public LDbCache< LBDbSource >, public LCDbSingleton< LBDbSources >
{
public:

	bool read( LQuery qCluster, bool readAll = false );
	const LBDbSource * readRecord( LQuery qCluster, int bsid );
	const LBDbSource * readRecord( LQuery qCluster, const std::string & barcode,
						 const std::string & test, const std::string & sample );
	const LBDbSource * readRecord( LQuery qCluster, std::pair< int, int > specimen );
	bool deleteOld( LQuery cluster, TDateTime flagged, TDateTime oldest );
};

//---------------------------------------------------------------------------
//	The in-memory representation of records from the specimen table
//---------------------------------------------------------------------------

class LPDbSource : public LPDbID, public LDbSource, public ValueFields
{
	int profileID, operatorID;
	TDateTime appointment, swipeTime;

	static const short NEW_RECORD = 0, SWIPED = 1, RESULTS = 2, DONE = 3, BOXED = 4, WAITING = 5;

public:

	LPDbSource( const LPDbSource & other );
	LPDbSource( const LBDbSource & other );
	LPDbSource( const LDbSource & other );
	LPDbSource( const ValueFields & values );
	LPDbSource( const std::string & barcode, TDateTime taken, TDateTime arrived );
	LPDbSource( const LQuery & query );

	void createName( LQuery project, const std::string & nameBase = "AUTO" );
	bool saveRecord( LQuery project );
	bool saveWithNote( LQuery pQuery, const LDbNote & details );
	bool findDetails( LQuery qProject );

	TDateTime getAppointment() const { return appointment; }

	void setArrived( TDateTime when, int processID = 0 );
	int getSwipeUserID() const { return operatorID; }
	TDateTime getSwipeTime() const { return swipeTime; }

	int getProfileID() const { return profileID; }
	void setProfileID( int profile ) { profileID = profile; }

	static short makeStatus( Stage stage );
	static Stage makeStage( short status );
};

//---------------------------------------------------------------------------

class LPDbSources : public LDbCache< LPDbSource >, public LPDbCacheMap< LPDbSources >
{
	class Matcher;

public:

	bool read( LQuery pQuery, LPDbSource::Stage stage = LDbStage::ARRIVED );
	bool readByScanDate( LQuery pQuery, TDateTime start, TDateTime finish );
	bool readByDescriptors( LQuery pQuery, const ValueFields & values );

	const LPDbSource * readRecord( LQuery pQuery, int sid );
	const LPDbSource * findByName( const std::string & externalName ) const;

	int setStatus( LQuery pQuery, const std::set< int > & samples, LPDbSource::Stage stage );
};

//---------------------------------------------------------------------------

#endif

