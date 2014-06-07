//---------------------------------------------------------------------------

#ifndef LCDbJobH
#define LCDbJobH

#include <sstream>

#include <Extctrls.hpp>
#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LCDbCryoJob : public LCDbID, public LDbNames
{
	short jobType, status, boxSet;
	int exercise, primary, secondary;
	TDateTime time_stamp, claimed_until;
	int projectID, processID;
	TDateTime start_date, finish_date;
	std::string reason;

	bool reload( LQuery & central );

public:

	enum Status { NEW_JOB, INPROGRESS, DONE, REJECTED, DELETED = 99, NUM_STATUSES };
	enum JobKind { UNKNOWN, BOX_MOVE, BOX_RETRIEVAL, BOX_DISCARD, SAMPLE_RETRIEVAL,
					SAMPLE_DISCARD, SAMPLE_RATIONALISE, NUM_TYPES };

	LCDbCryoJob( int id = 0, JobKind type = UNKNOWN )
	: LCDbID( id ), jobType( type ), status( NEW_JOB ), boxSet( 0 ),
	  exercise( 0 ), primary( 0 ), secondary( 0 ), projectID( 0 ), processID( 0 )
	{}

	LCDbCryoJob( JobKind type, const std::string & name, const std::string & desc )
	: LCDbID( 0 ), LDbNames( name, desc ), jobType( type ), status( NEW_JOB ), boxSet( 0 ),
	  exercise( 0 ), primary( 0 ), secondary( 0 ), projectID( 0 ), processID( 0 )
	{}

	LCDbCryoJob( const LQuery & project );

	void setProjectID( int id ) { projectID = id; }
	void setJobType( JobKind type ) { jobType = type; }
	void setStatus( Status p_st ) { status = p_st; }

	bool isActive() const { return status != DELETED; }
	bool isAvailable() const;

	int getPrimaryAliquot() const { return primary; }
	void setPrimaryAliquot( int typeID ) { primary = typeID; }
	int getSecondaryAliquot() const { return secondary; }
	void setSecondaryAliquot( int typeID ) { secondary = typeID; }

	int getProjectID() const { return projectID; }
	JobKind getJobType() const { return JobKind( jobType ); }
	const char * getTypeName() const;
	Status getStatus() const { return Status( status ); }
	void setBoxSet( short job ) { boxSet = job; }
	short getBoxSet() const { return boxSet; }

	TDateTime getTimeStamp() const { return time_stamp; }
	TDateTime getStartDate() const { return start_date; }
	TDateTime getClaimedUntil() const { return claimed_until; }
	TDateTime getFinishDate() const { return finish_date; }

	int getProcessCID() const { return processID; }
	int getUserID() const;

	void setExercise( int objID ) { exercise = objID; }
	void setExercise( const std::string & exName );
	int getExercise() const { return exercise; }
	void setReason( const std::string & text ) { reason = text; }
	const std::string & getReason() const { return reason; }

	void createName( LQuery central, const std::string & nameBase = "" );
	bool saveRecord( LQuery central );
	bool claim( LQuery central, bool extend );
	bool release( LQuery central, bool finished );

    std::string str();
};

//---------------------------------------------------------------------------

class LCDbCryoJobs : public LDbCache< LCDbCryoJob >, public LCDbSingleton< LCDbCryoJobs >
{
	int currentID;
	TTimer *renew;

	void __fastcall Renewal(TObject *);

public:

	LCDbCryoJobs( bool keepAlive = true );

	static int getCurrentID() { return records().currentID; }
	void clearCurrentID() { currentID = 0; }
	void setCurrent( const LCDbCryoJob & job ) { currentID = job.getID(); }

	const LCDbCryoJob * readRecord( LQuery cQuery, int sid );
	bool read( LQuery cQuery, LCDbCryoJob::JobKind type = LCDbCryoJob::UNKNOWN, bool all = false );
	const LCDbCryoJob * findByName( const std::string & name ) const;
};

//---------------------------------------------------------------------------

#endif

