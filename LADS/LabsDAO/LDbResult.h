//---------------------------------------------------------------------------

#ifndef LDbResultH
#define LDbResultH

#include "LDbIdClasses.h"
#include "LDbCacheBase.h"
#include "LDbStage.h"
#include "LDbNotes.h"
#include "TestRange.h"
#include "ValueFields.h"

//---------------------------------------------------------------------------

class LDbResult : public ResultValue, public LDbStage, public LDbNoteCount
{
	int machineID;
	int valop1id, valop2id;
	bool internal;
	std::string unitCode;

protected:

	int testID, processID;
	XTIME analysed;

	LDbResult( int test, const std::string & result, TDateTime when, int machine,
				int process, bool hide, short notes, int person1, int person2 );

	LDbResult()
	 : testID( 0 ), machineID( 0 ), processID( 0 ), valop1id( 0 ), valop2id( 0 ), internal( false )
	{}

	void setMachineID( int analyser ) { machineID = analyser; }

public:

	XTIME getTimeStamp() const { return analysed; }
	int getTestID() const { return testID; }
	int getMachineID() const { return machineID; }
	int getProcessID() const { return processID; }
	int getFirstValOpID() const { return valop1id; }
	int getSecondValOpID() const { return valop2id; }
	bool isPrivate() const { return internal; }
	void setPrivate( bool hide ) { internal = hide; }
	void setUnitCode( const std::string & s ){ unitCode = s; }
	std::string getUnitCode() const { return unitCode; }
};

//---------------------------------------------------------------------------

class LBDbResult : public LDbResult, public LBDbID
{
	int bsid;
	bool modified;

protected:

	LBDbResult() : bsid( 0 ), modified( false )
	{}

public:

	LBDbResult( int testID, const std::string & result, TDateTime analysed, int analysisID = 0 );

	explicit LBDbResult( const LQuery & cluster );
	bool createRecord( LQuery cluster );
	bool updateRecord( LQuery cluster );

	int getBuddySampleID() const { return bsid; }
	void setBuddySampleID( int bdbid ) { bsid = bdbid; }

	static char action( Stage stage );
	static Stage makeStage( char action );
};

//---------------------------------------------------------------------------

class LBDbResults : public LDbCache< LBDbResult > // , public LDbSingleton< LBDbResults >
{
public:

	bool read( LQuery buddyQuery, bool readAll );
	unsigned deleteOld( LQuery cluster, TDateTime flagged, TDateTime oldest );
};

//---------------------------------------------------------------------------

class LPDbResult : public LDbResult, public LPDbID
{
	int sampleID, reagent;

	enum Flags { INVALID = 1, PRIVATE = 2, DELETED = 99 };

public:

	LPDbResult( const LBDbResult & brfe, int psid, int reagID )
	 : LDbResult( brfe ), sampleID( psid ), reagent( reagID )
	{}

	explicit LPDbResult( const LQuery & project );
	bool createRecord( LQuery project );
	void addAuditEntry( LQuery cluster, int buddyResultID, const std::string & barcode );
};

//---------------------------------------------------------------------------

class LPDbResults : public LDbCache< LPDbResult > //, public LPDbCacheMap< LPDbResults >
{
public:

	unsigned readSample( LQuery pQuery, int sampleID );
};

//---------------------------------------------------------------------------

class LPDbInternal : public LDbResult, public LPDbID
{
	int sampleID, reagent;

	static short makeStatus( Stage stage );

public:

	LPDbInternal( const LBDbResult & brfe, int psid, int reagID )
	 : LDbResult( brfe ), sampleID( psid ), reagent( reagID )
	{}

	explicit LPDbInternal( const LQuery & project );
	bool createRecord( LQuery project );
};

//---------------------------------------------------------------------------

class LPDbInternals : public LDbCache< LPDbInternal > // , public LPDbCacheMap< LPDbInternals >
{
public:

	unsigned readSample( LQuery pQuery, int sampleID );
};

//---------------------------------------------------------------------------

#endif
