//---------------------------------------------------------------------------

#ifndef LCDbJobH
#define LCDbJobH

#include <Extctrls.hpp>
#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LCDbCryoJob : public LCDbID, public LDbNames
{
	short jobType, status;
	TDateTime time_stamp, claimed_until;
	int projectID, processID, aliquotType;
	std::string reason;

	bool reload( LQuery & central );

public:

	enum Status { NEW_JOB, INPROGRESS, DONE, DELETED = 99 };
	enum JobKind { UNKNOWN, BOX_MOVE, BOX_RETRIEVAL, BOX_DISCARD,
				SAMPLE_RETRIEVAL, SAMPLE_DISCARD, NUM_TYPES };

	LCDbCryoJob( int id = 0, JobKind type = UNKNOWN )
	 : LCDbID( id ), jobType( type )
	{
		processID = projectID = 0;
		status = NEW_JOB;
	}

	LCDbCryoJob( JobKind type, const std::string & name, const std::string & desc )
	 : LCDbID( 0 ), jobType( type ), LDbNames( name, desc )
	{
		processID = projectID = 0;
		status = NEW_JOB;
	}

	LCDbCryoJob( const LQuery & project );

	void setProjectID( int id ) { projectID = id; }
	void setStatus( Status p_st ) { status = p_st; };
	void setJobType( JobKind type ) { jobType = type; };

	bool isActive() const { return status != DELETED; }
	bool isAvailable() const;

	int getPrimaryAliquot() const { return aliquotType; }
	void setPrimaryAliquot( int typeID ) { aliquotType = typeID; }

	int getProjectID() const { return projectID; }
	JobKind getJobType() const { return JobKind( jobType ); }
	Status getStatus() const { return Status( status ); }
	TDateTime getTimeStamp() const { return time_stamp; }
	int getProcessCID() const { return processID; }
	int getUserID() const;

	void setReason( const std::string & nm ) { reason = nm; }
	const std::string & getReason() const { return reason; }

	void createName( LQuery central, const std::string & nameBase );
	bool saveRecord( LQuery central );
	bool claim( LQuery central, bool extend );
	bool release( LQuery central, bool finished );
};

//---------------------------------------------------------------------------

class LCDbCryoJobs : public LDbCache< LCDbCryoJob >, public LDbSingleton< LCDbCryoJobs >
{
	int currentID;
	LQuery cq;
	TTimer *renew;

	void __fastcall Renewal(TObject *);

public:

	LCDbCryoJobs( bool keepAlive = true );
	~LCDbCryoJobs();

	static int getCurrentID() { return records().currentID; }
	void clearCurrentID() { currentID = 0; }
	void setCurrent( const LCDbCryoJob & job ) { currentID = job.getID(); }

	const LCDbCryoJob * readRecord( LQuery cQuery, int sid );
	bool read( LCDbCryoJob::JobKind type, bool all );

	const LCDbCryoJob * findByName( const std::string & name ) const {
		return findMatch( LDbNames::LCMatcher( name ) );
	}  
};

//---------------------------------------------------------------------------

#endif

