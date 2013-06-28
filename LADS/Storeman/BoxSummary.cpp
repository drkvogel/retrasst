//---------------------------------------------------------------------------

#include <vcl.h>
#include "LPDbBoxes.h"
#include "BoxSummary.h"
#include "LDbBoxType.h"
#include "StoreUtil.h"
#include "LPDbCryovial.h"
#include "LCDbObject.h"
#include "LDbSource.h"
#include "LPDbProfile.h"

#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------

BoxSummary::BoxSummary() {
	checker = new BoxCheckService();
}

//---------------------------------------------------------------------------

BoxSummary::~BoxSummary() {
	clear();
}

//---------------------------------------------------------------------------

void BoxSummary::restart() {
	clear();
	checker = new BoxCheckService();
}

//---------------------------------------------------------------------------

BoxSummary::BoxCheckService::BoxCheckService() : workerThread( new paulst::WorkerThread() ) {
}

//---------------------------------------------------------------------------

BoxSummary::BoxCheckService::~BoxCheckService() {
	delete workerThread;
}

//---------------------------------------------------------------------------

BoxSummary::BoxCheckTask::BoxCheckTask( Results * summary ) : results( summary ) {
}

//---------------------------------------------------------------------------

BoxSummary::BoxCheckTask::~BoxCheckTask() {
	// do nothing - results are still owned by BoxSummary
}

//---------------------------------------------------------------------------
//	stop the worker thread and and discard the accumulated results
//---------------------------------------------------------------------------

void BoxSummary::clear() {
	delete checker;
	checker = NULL;
	for( std::map< std::string, Results * >::iterator mi = checked.begin(); mi != checked.end(); mi ++ ) {
		delete mi -> second;
	}
	checked.clear();
}

//---------------------------------------------------------------------------
//	add box to list/queue if necessary; return current status/percentage
//---------------------------------------------------------------------------

int BoxSummary::check( const LPDbBoxName & box, short expected ) {
	if( expected == 0 ) {
		return NO_ANALYSES_REQUIRED;
	}
	if( box.getStatus() == LPDbBoxName::ANALYSED ) {
		return READY_FOR_STORAGE;
	}
	Results * summary;
	std::map< std::string, Results * >::iterator mi = checked.find( box.getName() );
	if( mi == checked.end() ) {
		summary = new Results( box );
		checker -> queue( summary );
		checked[ box.getName() ] = summary;
	} else {
		summary = mi -> second;
	}
	if( summary -> status == SPECIMEN_CHECKED ) {
		return (summary -> analyses * 100) / (summary -> cryovials * expected);
	} else {
		return summary -> status;
	}
}

//---------------------------------------------------------------------------
//	set initial status; queue for database check if analyses expected
//---------------------------------------------------------------------------

BoxSummary::Results::Results( const LPDbBoxName & box )
 : boxID( box.getID() ), cryovials( 0 ), analyses( 0 ), status( NOT_YET_CHECKED )
{}

//---------------------------------------------------------------------------
//	add a box to the queue; adding NULL tells the service to stop
//---------------------------------------------------------------------------

void BoxSummary::BoxCheckService::queue( Results * summary ) {
	workerThread->queueTask( new BoxCheckTask( summary ) );
}

//---------------------------------------------------------------------------

bool BoxSummary::BoxCheckTask::execute() {
	if( results == NULL ) {
		return false;
	} else {
		results -> checkContents();
		return true;
	}
}

//---------------------------------------------------------------------------
//	Count completed analyses for each of the cryovials in the box
//---------------------------------------------------------------------------

void BoxSummary::Results::checkContents()
{
	// find profiles expecting results so we don't need to check results table
	LQuery pq( Util::projectQuery() );
	LPDbProfiles & profiles = LPDbProfiles::records();
	std::set< int > testProfileIDs;
	if( !profiles.empty() || profiles.read( pq, false ) ) {
		for( Range< LPDbProfile > pr = profiles; pr.isValid(); ++ pr ) {
			if( pr -> isTested() ) {
				testProfileIDs.insert( pr -> getID() );
			}
		}
	}

	// count how many cryovials have completed specimen records for those profiles
	LPDbCryovials contents;
	if( !contents.readByBoxID( pq, boxID ) ) {
		status = CONTENTS_NOT_FOUND;
	} else {
		Range< LPDbCryovial > cr = contents;
		do {	// check a dozen or so barcodes at a time
			Cryovials bag( cr->getAliquotType() );
			for( int n = 0; cr.isValid() && n < 14; ++ cr, ++ n ) {
				cryovials ++;
				bag.addBarcode( cr->getBarcode() );
			}
			analyses += bag.checkAnalyses( pq, testProfileIDs );
		} while( cr.isValid() );
		status = SPECIMEN_CHECKED;
	}
}

//---------------------------------------------------------------------------
//	Check specimen table for cryovial-derived entries with results sent
//---------------------------------------------------------------------------

BoxSummary::Cryovials::Cryovials( int aliquotTypeID ) {
	const LCDbObject & aliquot = LCDbObjects::records().get( aliquotTypeID );
	filter.setDescriptor( "aliquot_type", aliquot.getName() );
}

//---------------------------------------------------------------------------

void BoxSummary::Cryovials::addBarcode( const std::string & cryovial ) {
	filter.getValueField( "barcode" ).value( cryovial );
}

//---------------------------------------------------------------------------

short BoxSummary::Cryovials::checkAnalyses( LQuery & pq, const std::set< int > & testProfileIDs ) {
	short analyses = 0;
	LPDbSources specimens;
	if( specimens.readByDescriptors( pq, filter ) ) {
		Range< LPDbSource > si = specimens;
		do {
			if( (si -> getStage() == LPDbSource::TRANSMITTED
			  || si -> getStage() == LPDbSource::COLLECTED)
			  && testProfileIDs.count( si->getProfileID() ) > 0 ) {
				 analyses ++;
			}
		} while( (++si).isValid() );
	}
	return analyses;
}

//---------------------------------------------------------------------------

