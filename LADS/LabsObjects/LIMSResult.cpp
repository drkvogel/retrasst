/*---------------------------------------------------------------------------
 *
 *	NOTE: each "analysis" produces one or more results in buddy_result_float,
 *		linked to a single buddy_database record by machine_id + buddy_sample_id.
 *      The material can go on again for a second analysis.  Results from a
 *		"specimen" are linked by project_id + sample_id (QCs are not linked)
 *
 *	Version history:
 *
 *      14 June 2005	Created to hold business logic for saved results
 *      5 July 2005		Added method to copy result to project and audit
 *      16 August:		In copyResultData(), set project even if already transmitted
 *      25 August:		In updateWorklist(), set status in header as well as result
 *      8 September		Don't read specimen record in copySampleData(); just update
 *      4 Nov. 2005		Use fluid code as well as test code to find test ID
 *      8 November		Encourage user to create new header record for each run
 *      22 Nov 05, NG:	Added getExpectedTests() to match confirmSample()
 *      31 Jan 2006     Set buddy_database entry to ANALYSED in updateWorklist()
 *						even if there's no sample ID in worklist entry
 *      2 June 09, NG:	Added LIMSResults - all current results for a cluster
 *      30 Jun 09, NG:	Tick off central as well as local worklist entries
 *      21 Sep 09, NG:	keep track of buddy_database ID in buddy_worklist
 *      23 Dec 09, NG:	extend rather than use LBDbSource and LBDbResult
 *      8 March 2010:	only add re-runs the first time we see a result
 *      19 Mar 10, NG:	added loadAnalysis() method to restore header data
 *      31 March, NG:	copy worklist sample ID to buddy_database if necessary
 *		5 Oct 10, NG:	ignore test ID when checking if QC entries exist
 *      12 Oct 2010:	don't update QC entries in the local worklist
 *
 *--------------------------------------------------------------------------*/

#include <vcl.h>

#include "LDbResult.h"
#include "LCDbAnalyser.h"
#include "LDbWorkList.h"
#include "LDbSource.h"
#include "LCDbAuditTrail.h"
#include "LDbNotes.h"
#include "LCDbTest.h"
#include "LIMSResult.h"
#include "LCDbProject.h"
#include "LPDbProfile.h"
#include "TfrmConfirm.h"
#include "LIMSParams.h"
#include "XMLFile.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

LIMSResult::LIMSResult( const std::string & sample, int catID, TDateTime when )
: LBDbSource( sample, when )
{}

//---------------------------------------------------------------------------

LIMSResult::LIMSResult( const LBDbSource & analysis, const LBDbResult & result )
 : LBDbSource( analysis ), LBDbResult( result )
{
	if( result.getBuddySampleID() == 0 ) {
		setBuddySampleID( analysis.getID() );
	}
	else if( result.getBuddySampleID() != analysis.getID() )
		throw Exception( "Buddy result does not match analysis" );
}

//---------------------------------------------------------------------------

LIMSResult::LIMSResult( const LBDbSource & analysis )
 : LBDbSource( analysis )
{
	setBuddySampleID( analysis.getID() );
}

//---------------------------------------------------------------------------

LIMSResult::LIMSResult( const LBDbResult & result )
 : LBDbResult( result ), LBDbSource( LBDbSources::records().get( result.getBuddySampleID() ) )
{}

//---------------------------------------------------------------------------

String LIMSResult::toString() const
{
	String testName = "(unknown)";
	const LCDbTest * test = LCDbTests::records().findByID( testID );
	if( test != NULL )
		testName = test -> getName();
	return getBarcode() + ": " + testName + " = " + asString() + " " + getUnitCode();
}

//---------------------------------------------------------------------------

String LIMSResult::asSampleNote() const
{
	String summary;
	const LCDbTest * test = LCDbTests::records().findByID( testID );
	if( test != NULL && test -> keepAsNote() )
	{
		summary = test -> getName() + " = " + asString();
		if( !getUnitCode().IsEmpty() )
			summary = summary + " " + getUnitCode();
	}
	return summary;
}

//---------------------------------------------------------------------------

void LIMSResult::setStage( LDbStage::Stage stage )
{
	LBDbSource::setStage( stage );
	LBDbResult::setStage( stage );
}

//---------------------------------------------------------------------------

TDateTime LIMSResult::getTimeStamp() const
{
	TDateTime t1 = LBDbSource::getTimeStamp();
	TDateTime t2 = LBDbResult::getTimeStamp();
	return t1 > t2 ? t1 : t2;
}

//---------------------------------------------------------------------------

LDbStage::Stage LIMSResult::getStage() const
{
	LDbStage::Stage s1 = LBDbSource::getStage();
	LDbStage::Stage s2 = LBDbResult::getStage();
	return s1 > s2 ? s1 : s2;
}

//---------------------------------------------------------------------------
//	create result record, ready to be saved in buddy_result_float
//---------------------------------------------------------------------------

void LIMSResult::setTestResult( int test, TDateTime when, const std::string & value )
{
	testID = test;
	analysed = when;
	setValue( value );
}

//---------------------------------------------------------------------------
//	restore details from original sample program from buddy_database
//---------------------------------------------------------------------------

const LBDbSource * LIMSResult::findAnalysis( LQuery qCluster, int analysisID )
{
	LBDbSources & bdbCache = LBDbSources::records();
	if( bdbCache.empty() )
		bdbCache.read( qCluster, false );

	const LBDbSource * header = NULL;
	if( analysisID != 0 )
	{
		header = bdbCache.findByID( analysisID );
		if( header == NULL )
			header = bdbCache.readRecord( qCluster, analysisID );
	}
	return header;
}

//---------------------------------------------------------------------------
//  Update buddy_database record, adding a note if neccessary
//---------------------------------------------------------------------------

bool LIMSResult::saveAnalysis( LQuery qCluster, const String & note )
{
	LBDbSource::notes = note.IsEmpty() ? 0 : 1;
	if( !LBDbSource::saveRecord( qCluster ) )
		return false;

	setBuddySampleID( LBDbSource::getID() );
	if( note.IsEmpty() )
		return true;

	LBDbNote buddyNote( note, LBDbSource::getID() );
	buddyNote.setTimeStamp( LBDbSource::getTimeStamp() );
	return buddyNote.saveRecord( qCluster );
}

//---------------------------------------------------------------------------
//  create buddy_result_float record, with note and attributes if necessary
//---------------------------------------------------------------------------

bool LIMSResult::saveTestResult( LQuery qCluster, const String & note )
{
	LBDbResult::notes = note.IsEmpty() ? 0 : 1;
	if( !LBDbResult::createRecord( qCluster ) )
		return false;

	int brid = LBDbResult::getID();
	for( Range< LDbResAttribute > ri = attributes; ri.isValid(); ++ ri )
		if( !ri -> createRecord( qCluster, brid ) )
			return false;

	if( note.IsEmpty() )
		return true;

	LBDbNote buddyNote( note, brid );
	buddyNote.setTimeStamp( LBDbResult::getTimeStamp() );
	return buddyNote.saveRecord( qCluster );
}

//---------------------------------------------------------------------------
//	Check worklist for entry matching this test/sample; set status if found
//---------------------------------------------------------------------------

template < typename T > const T::value_type * LIMSResult::check( T & list, bool pending )
{
	typedef std::set< const T::value_type * > Matched;
	Matched tests = list.find( getBarcode(), testID );
	for( Matched::const_iterator wli = tests.begin(); wli != tests.end(); ++ wli )
	{
		const T::value_type * wle = (*wli);
		if( pending && wle -> getStage() > LDbStage::PROGRAMMED )
			continue;

		if( specimen.second == 0 ) 	
			specimen = wle -> getSpecimen();

		else if( specimen != wle -> getSpecimen() )
			continue;

		setPrivate( wle -> getTestLimits().isPrivate() );
		return wle;
	}
	return NULL;
}

//---------------------------------------------------------------------------
//  Update or create worklist entries; keep sample ID if available
//---------------------------------------------------------------------------

bool LIMSResult::updateWorklist( LQuery qCluster, LQuery qCentral )
{
	// check both worklists, from database if necessary
	const LCDbWorkEntry * cwle = NULL;
	LCDbWorkList cwl;
	if( cwl.read( qCentral, getBarcode() ) ) {
		cwle = check( cwl, true );
	}
	const LBDbWorkEntry * bwle = NULL;
	if( LIMSParams::instance().useLocalWorklist() ) {
		LBDbWorkList & bwl = LBDbWorkList::records();
		if( bwl.read( qCluster, getBarcode() ) ) {
			bwle = check( bwl, !isQC() );
        }
	}

	// create QC records from qc_test_machine if not present
	if( cwle == NULL && bwle == NULL && isQC()
	 && cwl.addQCRecords( qCentral, getBarcode(), testID ) > 0 ) {
		cwl.read( qCentral, getBarcode() );
		cwle = check( cwl, true );
	}

	if( cwle != NULL || bwle != NULL ) {
		setStage( LDbStage::ANALYSED );
	}

	if( LBDbResult::getID() == 0 ) {
		LBDbResult::claimNextID( qCluster );
	}

	// create or update local worklist entry if necessary
	if( LIMSParams::instance().useLocalWorklist() && !isQC() ) {
		if( bwle != NULL )
			updateLocalWorklist( qCluster, *bwle, true );

		else if( cwle != NULL )
			updateLocalWorklist( qCluster, *cwle, false );
	}

	// copy status into c_buddy_worklist, except for local QCs
	if( cwle == NULL ) {
		return bwle != NULL;
	} else {
		LCDbWorkEntry record( *cwle );
		record.setResultID( LBDbResult::getID() );
		return record.updateStatus( qCentral, getStage() );
	}
}

//---------------------------------------------------------------------------
//  Find and update buddy_worklist entry; add triggered and re-run entries
//---------------------------------------------------------------------------

void LIMSResult::updateLocalWorklist( LQuery & qCluster, LBDbWorkEntry bwle, bool exists )
{
	// replace buddy_sample_id with buddy_result_id in buddy_worklist
	bwle.setResultID( LBDbResult::getID() );
	if( exists )
		bwle.updateRecord( qCluster, getStage() );
	else
	{	bwle.setStage( getStage() );
		bwle.createRecord( qCluster );
	}

	// copy worklist data for triggers and re-run, but only once
	static std::set< Key > checked;
	Key current( *this );
	if( checked.count( current ) == 0 )
	{
		const LCDbTest & tr = LCDbTests::records().get( testID );
		if( !isValid() || !tr.getLimits().includes( *this ) )
			createReRun( bwle, qCluster );

		if( isValid() && bwle.getTestLimits().triggers( *this ) )
			createTriggers( bwle, qCluster );

		checked.insert( current );
	}
}

//---------------------------------------------------------------------------

LIMSResult::Key::Key( const LIMSResult & r )
{
	test = r.getTestID();
	std::pair< int, int > sample = r.getSpecimen();
	project = sample.first;
	specimen = sample.second;
}

//---------------------------------------------------------------------------

bool LIMSResult::Key::operator< ( const Key & other ) const
{
	return (project < other.project)
		|| (project == other.project && (specimen < other.specimen
		|| (specimen == other.specimen && test < other.test)));
}

//---------------------------------------------------------------------------
//  Create a new buddy_worklist entry if the result was invalid
//---------------------------------------------------------------------------

void LIMSResult::createReRun( LBDbWorkEntry wle, LQuery qCluster )
{
	wle.setStage( LDbStage::RE_RUN );
	wle.setTimeStamp( Now() );
	wle.createRecord( qCluster, "Range error: re-run" );
}

//---------------------------------------------------------------------------
//  Add triggered tests if the measured value is out of range
//---------------------------------------------------------------------------

void LIMSResult::createTriggers( LBDbWorkEntry wle, LQuery qCluster )
{
	int profile = wle.getTestLimits().getTriggerProfile();
	const LPDbProfile * pp = LPDbProfiles::records( wle.getProjectID() ).findByID( profile );
	if( pp != NULL )
	{
		wle.setStage( LDbStage::TRIGGERED );
		wle.setTimeStamp( Now() );
		for( Range< LPDbProfile::TestLimits > tp = pp -> getTests(); tp.isValid(); ++ tp )
		{
			wle.setTestLimits( *tp );
			wle.createRecord( qCluster, pp -> getName() );
		}
	}
}

//---------------------------------------------------------------------------

template< typename T > int LIMSResult::makeResult( LQuery & qProject, LQuery & qCluster, LQuery & qCentral )
{
	LDbAttributes attributes;
	int reagentCID = 0;
	if( attributes.checkResult( qCluster, LBDbResult::getID() ) )
	{
		reagentCID = attributes.checkReagent( qCentral );
		if( reagentCID == 0 )
			reagentCID = attributes.addReagent( qCentral );
		attributes.mark( qCluster, LBDbResult::getID() );
	}

	T details( *this, getSpecimen().second, reagentCID );
	return details.createRecord( qProject ) ? details.getID() : 0;
}

//---------------------------------------------------------------------------
//  Copy result + notes to the project; update related worklist records
//---------------------------------------------------------------------------

bool LIMSResult::transmit( LQuery qProject, LQuery qCluster, LQuery qCentral )
{
	int allocatedID;
	switch( LBDbResult::getStage() )
	{
		case LDbStage::L2_VALIDATED:
			allocatedID = makeResult< LPDbResult >( qProject, qCluster, qCentral );
			break;

		case LDbStage::TRANSMITTED:
			return false;				// don't create a second copy

		default:
			allocatedID = makeResult< LPDbInternal >( qProject, qCluster, qCentral );
	}

	// copy notes from cluster database if there may be some
	if( LBDbResult::hasNotes() ) {
		LBDbNotes notes;
		if( notes.read( qCluster, LBDbResult::getID() ) )
		{
			for( Range< LBDbNote > nr = notes; nr.isValid(); ++ nr )
				LPDbNote( *nr, allocatedID ).saveRecord( qProject );
			notes.mark( qCluster, LBDbResult::getID() );
		}
	}

	// tick off in buddy_result_float and (both) worklists
	if( LIMSParams::instance().useLocalWorklist() ) {
		LBDbWorkEntry bwle( getBarcode(), testID, LBDbResult::getID() );
		bwle.updateRecord( qCluster, LDbWorkEntry::TRANSMITTED );
	}

	LCDbWorkList worklist;
	worklist.read( qCentral, getBarcode() );
	const LCDbWorkEntry * cwle = check( worklist, false );
	if( cwle != NULL ) {
		LCDbWorkEntry( *cwle ).updateStatus( qCentral, LDbWorkEntry::TRANSMITTED );
	}

	setStage( LBDbResult::TRANSMITTED );
	return updateRecord( qCluster );
}

//---------------------------------------------------------------------------

