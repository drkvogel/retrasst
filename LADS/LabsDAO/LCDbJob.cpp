//---------------------------------------------------------------------------
//  Version history:
//		07/12/10, NG:	Added method to get operator from process ID
//
//---------------------------------------------------------------------------


#include <vcl.h>
#include <sstream>

#include "LCDbProject.h"
#include "LIMSDatabase.h"
#include "LCDbJob.h"
#include "LCDbTankMap.h"
#include "LCDbAuditTrail.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

LCDbCryoJob::LCDbCryoJob( const LQuery & query )
 : LCDbID( query.readInt( "retrieval_cid" ) ),
   exercise( query.fieldExists( "exercise_cid" ) ? query.readInt( "exercise_cid" ) : 0 ),
   LDbNames( query.readString( "external_name" ), query.readString( "description" ) ),
   reason( query.fieldExists( "reason" ) ? query.readString( "reason" ) : query.readString( "description" ) ),
   jobType( query.readInt( "job_type" ) ),
   projectID( query.readInt( "project_cid" ) ),
   primary( query.fieldExists( "primary_aliquot" ) ? query.readInt( "primary_aliquot" ) : 0 ),
   secondary( query.fieldExists( "secondary_aliquot" ) ? query.readInt( "secondary_aliquot" ) : 0 ),
   processID( query.readInt( "process_cid" ) ),
   status( query.readInt( "status" ) )
{
	if( query.fieldExists( "time_stamp" ) ) {
		time_stamp = query.readDateTime( "time_stamp" );
	} else {
		time_stamp = Now();
	}
	if( query.fieldExists( "start_date" ) ) {
		time_stamp = query.readDateTime( "start_date" );
	}
	if( query.fieldExists( "finish_date" ) ) {
		time_stamp = query.readDateTime( "finish_date" );
	}
	if( query.fieldExists( "claimed_until" ) ) {
		time_stamp = query.readDateTime( "claimed_until" );
	}
}

//---------------------------------------------------------------------------
//	Allocate new ID if necessary and create corresponding external name
//---------------------------------------------------------------------------

void LCDbCryoJob::createName( LQuery central, const std::string & nameBase )
{
	std::stringstream out;
	setID( 0 );
	claimNextID( central );
	out << nameBase << getID();
	setName( out.str() );
}

//---------------------------------------------------------------------------
//	Create or update job record - timer code will claim it if necessary
//---------------------------------------------------------------------------

bool LCDbCryoJob::saveRecord( LQuery central )
{
	if( saved ) {
		central.setSQL( "update c_retrieval_job"
					   " set status = :sts, process_cid = :pid"
					   " where retrieval_cid = :myid" );
	} else {
		if( getName().empty()) {
			createName( central, "AUTOJOB" );
		}
		if( getID() == 0 ) {
			claimNextID( central );
		}
		std::string fields = "retrieval_cid, exercise_cid, external_name, description, job_type,"
							" project_cid, primary_aliquot, secondary_aliquot, process_cid, status,"
							" start_date, claimed_until";
		std::string params = ":myid, :exid, :nme, :dsc, :jt, :prj, :al1, :al2, :pid, :sts, :sdt,"
							" date('now') + date('1 minute')";
		central.setSQL( "insert into c_retrieval_job (" + fields + " ) values (" + params + " )" );
		central.setParam( "nme", getName() );
		central.setParam( "exid", exercise );
		central.setParam( "dsc", getDescription() );
		central.setParam( "prj", projectID );
		central.setParam( "jt", jobType );
		central.setParam( "al1", primary );
		central.setParam( "al2", secondary );
		central.setParam( "sdt", XDATE( start_date ) );
	}
	central.setParam( "myid", getID() );
	central.setParam( "sts", status );
	central.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	return central.execSQL() && reload( central );
}

//---------------------------------------------------------------------------
//	Attempt to claim this job or extend the lease (e.g. from timer code)
//---------------------------------------------------------------------------

bool LCDbCryoJob::claim( LQuery central, bool extend )
{
	std::string update = "update c_retrieval_job"
						" set status = :use, claimed_until = date('now') + date('10 minutes')";
	std::string filter = " where retrieval_cid = :myid";
	if( extend ) {
		filter += " and process_cid = :pid";
		central.setSQL( update + filter );
	}
	else {
		update += ", process_cid = :pid";
		filter += " and (status = :rdy or claimed_until < date('now'))";
		central.setSQL( update + filter );
		central.setParam( "rdy", NEW_JOB );
	}

	central.setParam( "use", INPROGRESS );
	central.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	central.setParam( "myid", getID() );
	return central.execSQL() && reload( central );
}

//---------------------------------------------------------------------------
//	Check if current process owns this job or can [probably] claim it
//---------------------------------------------------------------------------

bool LCDbCryoJob::isAvailable() const
{
	return status != INPROGRESS
		|| processID == LCDbAuditTrail::getCurrent().getProcessID()
		|| claimed_until < Now();
}

//---------------------------------------------------------------------------
//	Update date/time etc. from database and copy into cache
//---------------------------------------------------------------------------

bool LCDbCryoJob::reload( LQuery & central )
{
	LCDbCryoJobs & cache = LCDbCryoJobs::records();
	const LCDbCryoJob * found = cache.readRecord( central, getID() );
	if( found == NULL )
		return false;
	else
	{	*this = *found;
		return true;
	}
}

//---------------------------------------------------------------------------
//	Release this job.  Return false if it wasn't ours to release
//---------------------------------------------------------------------------

bool LCDbCryoJob::release( LQuery central, bool finished )
{
	central.setSQL( "update c_retrieval_job set status = :fre, claimed_until = 'now'"
				   " where retrieval_cid = :myid and process_cid = :pid" );
	central.setParam( "fre", finished ? DONE : INPROGRESS );
	central.setParam( "myid", getID() );
	central.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	if( central.execSQL() ) {
		LCDbCryoJobs::records().erase( *this );
		return true;
	} else {
		return reload( central );
	}
}

//---------------------------------------------------------------------------
//	Read the selected specimen entry from the current project database
//---------------------------------------------------------------------------

const LCDbCryoJob * LCDbCryoJobs::readRecord( LQuery cQuery, int sid )
{
	cQuery.setSQL( "select * from c_retrieval_job where retrieval_cid = :myid" );
	cQuery.setParam( "myid", sid );
	return cQuery.open() == 1 ? insert( LCDbCryoJob( cQuery ) ) : NULL;
}

//---------------------------------------------------------------------------
//	Look up details of the operator from audit trail for this process
//---------------------------------------------------------------------------

int LCDbCryoJob::getUserID() const
{
	return LCDbAuditTrail::getCurrent().findUserID( processID );
}

//---------------------------------------------------------------------------
//	Keep jobs alive once they're added to the list for this program
//---------------------------------------------------------------------------

LCDbCryoJobs::LCDbCryoJobs( bool keepAlive )
{
	if( keepAlive ) {
		renew = new TTimer( NULL );
		renew -> OnTimer = Renewal;
		renew -> Interval = 180000;		// update every 3 minutes or so
		renew -> Enabled = true;
	} else {
		renew = NULL;
	}
}

//---------------------------------------------------------------------------

LCDbCryoJobs::~LCDbCryoJobs()
{
	if( renew != NULL ) {
		renew -> Enabled = false;
		delete renew;
		renew = NULL;
	}
}

//---------------------------------------------------------------------------

void __fastcall LCDbCryoJobs::Renewal(TObject *)
{
	renew -> Enabled = false;
	for( iterator ci = begin(); ci != end(); ci ++ )
		if( ci -> getStatus() == LCDbCryoJob::INPROGRESS
		 && ci -> getProcessCID() == LCDbAuditTrail::getCurrent().getProcessID() )
			ci -> claim( LIMSDatabase::getCentralDb(), true );
	renew -> Enabled = true;
}

//---------------------------------------------------------------------------
//	Read jobs of the given type; include or exclude deleted records
//---------------------------------------------------------------------------

bool LCDbCryoJobs::read( LQuery cQuery, LCDbCryoJob::JobKind type, bool all )
{
	std::string q = "select * from c_retrieval_job";
	switch( type ) {
		case LCDbCryoJob::BOX_MOVE:
		case LCDbCryoJob::BOX_RETRIEVAL:
		case LCDbCryoJob::BOX_DISCARD:
		case LCDbCryoJob::SAMPLE_RETRIEVAL:
		case LCDbCryoJob::SAMPLE_DISCARD:
			q += " where job_type = :jt";
			break;
		default:
			q += " where 1=1";
	}
	if( !all ) {
		q += " and status <> :del";
	}
	cQuery.setSQL( q + " order by retrieval_cid" );
	cQuery.setParam( "jt", short( type ) );
	cQuery.setParam( "del", LDbValid::DELETED );
	return readData( cQuery );
}

//---------------------------------------------------------------------------

