/*---------------------------------------------------------------------------
 *
 *	Version history:
 *
 *	17 May 2005		Handle lower_warning if present in buddy_worklist
 *	31 May 2005		Use LDbStage for status; convert to/from character
 *  14 June, NG		Moved rerun/trigger handling into LIMSResult
 *	18 July 2005	Added clearQCEntries() to clear old QC materials
 *  20 September	Always read warning limits - error if not found
 *  3 Nov 05, NG:	Request wipe after trigger as well as rerun
 *  8 November 06	Moved checkRecord functionality into LIMSResult
 *  28 Aug 2008:	Allow for empty/null/dropped ldbc.range_set
 *  7 January 09:	Added c_buddy_worklist category ID and group ID
 *  12/03/09, NG:	Use float throughout; add test profile ID
 *  4 Jun 09, NG:	Added methods to check central (buddy) worklist
 *  11 August, NG:	Set stage for all records from a sample at once
 *  21 Sep 09, NG:	Duplicate = buddy_database ID when status = 'P'
 *  3 Feb '10, NG: 	Always use 'now' from vlab for time stamps
 *	15 Feb 10, NG:	Moved confirmation methods into LIMSAnalysis
 *  4 Jun '10, NG:	Set time stamp creating buddy_worklist record
 *  16 Nov 10, NG:	Copy category ID updating buddy_worklist
 *	1 Dec 10, NG:	Return multiple worklist entries from find
 *  25 March 2011:	Compare test IDs correctly copying qc_test_machine
 *  
 *--------------------------------------------------------------------------*/

#include <vcl.h>
#include <memory>

#include "LDbWorklist.h"
#include "LCDbQCLevel.h"
#include "LCDbAnalyser.h"
#include "LPDbProfile.h"
#include "LCDbTest.h"
#include "LIMSParams.h"
#include "LCDbProject.h"
#include "LDbSource.h"

#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------

int LBDbWorkEntry::nextRerun = LBDbWorkEntry::FIRST_RERUN_ID;

//---------------------------------------------------------------------------

TDateTime LDbWorkEntry::getTimeStamp() const
{
	static const TDate FIRST_DATE( 2000, 1, 1 );
	return timeStamp < FIRST_DATE ? Now() : timeStamp;
}

//---------------------------------------------------------------------------

LDbStage::Stage LDbWorkEntry::makeStage( char status )
{
	switch( status )
	{
		case 'S':
			return LDbStage::RE_RUN;

		case 'W':
			return LDbStage::WIPED;

		case 'P':
			return LDbStage::PROGRAMMED;

		case 'C':
			return LDbStage::ANALYSED;

		case 'F':
			return LDbStage::L1_VALIDATED;

		case 'V':
			return LDbStage::L2_VALIDATED;

		case 'N':
			return LDbStage::INVALIDATED;

		case 'T':
			return LDbStage::TRANSMITTED;

		case 'X':
			return LDbStage::DELETED;

		default:
			return LDbStage::EXPECTED;
	}
}

//---------------------------------------------------------------------------
//	Get the expected buddy_worklist status for the given work entry stage
//---------------------------------------------------------------------------

const char LDbWorkEntry::makeStatus( Stage stage )
{
	switch( stage )
	{
		case LDbStage::TRIGGERED:
		case LDbStage::RE_RUN:
			return 'S';

		case LDbStage::WIPED:
			return 'W';

		case LDbStage::PROGRAMMED:
			return 'P';

		case LDbStage::ANALYSED:
		case LDbStage::L0_HELD:
			return 'C';

		case LDbStage::L1_VALIDATED:
		case LDbStage::L1_MARKED:
			return 'F';

		case LDbStage::L2_VALIDATED:
			return 'V';

		case LDbStage::INVALIDATED:
			return 'N';

		case LDbStage::TRANSMITTED:
		case LDbStage::COLLECTED:
			return 'T';

		case LDbStage::DELETED:
			return 'X';

		default:
			return 'Q';
	}
}

//---------------------------------------------------------------------------
//	Constructor - read the current record from buddy_worklist
//---------------------------------------------------------------------------

LBDbWorkEntry::LBDbWorkEntry( const LQuery & cluster )
 : LDbWorkEntry( cluster.readString( "barcode" ),
				 cluster.readInt( "project_cid" ),
				 cluster.readInt( "sample_id" ),
				 cluster.readDateTime( "in_date" ),
				 cluster.readInt( "test_id" ) )
{
	limits.setAnalyserID( cluster.readInt( "machine_cid" ) );
	limits.setPrivate( cluster.readString( "private_result" ) == "Y" );
	limits.setRange( cluster.readString( "min_value" ), cluster.readString( "max_value" ) );
	limits.setWarnLevels( cluster.readString( "lower_warning" ), cluster.readString( "upper_warning" ) );

	int trigger = cluster.readInt( "trigger_id" );
	if( trigger != 0 )
	{
		limits.setTriggerLimits( trigger, cluster.readString( "trigger_lower_limit" ),
										cluster.readString( "trigger_upper_limit" ) );
	}
	int pid = 0;
	const LPDbProfiles & profiles = LPDbProfiles::records( getProjectID() );
	if( !profiles.empty() )
	{
		String reason = cluster.readString( "profile_name" );
		if( reason.Length() > 3 && reason.Pos( "re-run" ) == 0 )
		{
			const LPDbProfile * pp = profiles.findByName( reason );
			if( pp != NULL )
				pid = pp -> getID();
		}
	}
	setProfileID( pid );

	resultID = duplicate = cluster.readInt( "duplicate" );
	if( duplicate < nextRerun )
		nextRerun = duplicate - 1;		// try to keep unique

	setCategoryID( cluster.readInt( "category_id" ) );
	setTimeStamp( cluster.readDateTime( "time_stamp" ) );

	if( cluster.readChar( "buddy_read" ) == LCDbWorkEntry::FINISHED )
		setStage( LDbStage::TRANSMITTED );
	else
		setStage( makeStage( cluster.readChar( "status" ) ) );
}

//---------------------------------------------------------------------------
//  Copy worklist entry (back) into the buddy_worklist table
//---------------------------------------------------------------------------

bool LBDbWorkEntry::createRecord( LQuery cluster, String reason )
{
	int machine = limits.getAnalyserID();
	if( machine == 0 )
	{
		// QCs should stay on the same analyser; others on cluster
		machine = LCDbAnalysers::getCurrentID();
		const LCDbAnalyser * an = LCDbAnalysers::records().findByID( machine );
		if( an != NULL && !isQC() )
		{
			std::set< int > machineIDs = an -> getClusterIDs();
			if( !machineIDs.empty() )
				machine = *(machineIDs.begin());
		}
		limits.setAnalyserID( machine );
	}

	if( reason.IsEmpty() && getProfileID() != 0 )
	{
		const LPDbProfiles & profiles = LPDbProfiles::records( getProjectID() );
		const LPDbProfile * pp = profiles.findByID( getProfileID() );
		if( pp != NULL )
			reason = pp -> getName();
	}

	setTimeStamp( Now() );
	char fromCentral = LCDbWorkEntry::NEW_RECORD;
	switch( getStage() )
	{
		case NEW_ENTRY:
			break;

		case RE_RUN:
			resultID = nextRerun --;
			break;

		case TRIGGERED:
			resultID = TRIGGER_ID;
			break;

		default:
			fromCentral = LCDbWorkEntry::COPIED;
	}

	cluster.setSQL( "Insert into buddy_worklist (machine_cid, barcode, duplicate, test_name, test_id,"
					" sample_id, range_set, min_value, max_value, status, project_cid, profile_name,"
					" buddy_read, in_date, time_stamp, lower_warning, upper_warning, trigger_id,"
					" trigger_lower_limit, trigger_upper_limit, private_result, delete_status, category_id)"
				   " values ( :mid, :bar, :dup, :tnm, :tid, :aid, :rs, :min, :max, :sts, :prj,"
							" :prn, :brd, :ind, :dts, :wlo, :wup, :trd, :tlo, :tup, :prv, 0, :cat )" );
	cluster.setParam( "mid", machine );
	cluster.setParam( "bar", getBarcode() );
	cluster.setParam( "dup", resultID );

	std::pair< int, int > specimen = getSpecimen();
	cluster.setParam( "prj", specimen.first );
	cluster.setParam( "aid", specimen.second );

	const LCDbTest & test = LCDbTests::records().get( limits.getID() );
	cluster.setParam( "tid", test.getID() );
	cluster.setParam( "tnm", test.getName() );
	cluster.setParam( "prn", reason );

	std::pair< double, double > range = limits.getRange().asDoubles();
	cluster.setParam( "min", range.first );
	cluster.setParam( "max", range.second );
	cluster.setParam( "rs", (range.first == 0 && range.second == 0) ? 'N' : 'Y' );

	cluster.setParam( "sts", makeStatus( getStage() ) );
	cluster.setParam( "brd", fromCentral );
	cluster.setParam( "ind", getInDate() );
	cluster.setParam( "dts", getTimeStamp() );

	cluster.setParam( "trd", limits.getTriggerProfile() );
	if( limits.getTriggerProfile() == 0 )
	{
		cluster.setParam( "tup", "0" );
		cluster.setParam( "tlo", "0" );
	}
	else
	{	range = limits.getTriggerLimits().asDoubles();
		cluster.setParam( "tlo", range.first );
		cluster.setParam( "tup", range.second );
	}

	range = limits.getWarnLevels().asDoubles();
	cluster.setParam( "wlo", range.first );
	cluster.setParam( "wup", range.second );
	cluster.setParam( "prv", limits.isPrivate() ? 'Y' : 'N' );
	cluster.setParam( "cat", getCategoryID() );

	if( cluster.execSQL() != 1 ) {
		return false;
	}
	duplicate = resultID;
	LBDbWorkList::records().insert( *this );
	return true;
}

//---------------------------------------------------------------------------
//	Read the (updated) buddy_worklist; group entries by barcode
//---------------------------------------------------------------------------

bool LBDbWorkList::read( LQuery bQuery, bool readAll )
{
	if( readAll )
		bQuery.setSQL( "select * from buddy_worklist order by time_stamp" );
	else
	{	bQuery.setSQL( "select * from buddy_worklist where buddy_read <> :tra"
					" and machine_cid in (" + LDbWorkEntry::listMachineIDs() + ")"
					" order by time_stamp" );
		bQuery.setParam( "tra", LBDbWorkEntry::makeStatus( LDbStage::TRANSMITTED ) );
	}

	int count = bQuery.open();
	for( cache.clear(); !bQuery.eof(); bQuery.next() )
	{
		LBDbWorkEntry record( bQuery );
		cache.insert( std::pair< String, LBDbWorkEntry >( record.getBarcode(), record ) );
	}
	return count > 0;
}

//---------------------------------------------------------------------------
//	Update an existing buddy_worklist record from the current object
//---------------------------------------------------------------------------

bool LBDbWorkEntry::updateRecord( LQuery cluster, LDbWorkEntry::Stage result )
{
	LBDbWorkEntry * cached = LBDbWorkList::records().find( *this );
	TDateTime when = Now();

	String update = "set status = :sts, time_stamp = :ts";
	if( !isQC() && getCategoryID() != 0 )
		update = update + ", category_id = " + getCategoryID();
	if( resultID != 0 && resultID != duplicate )
		update = update + ", duplicate = " + resultID;

	String condition = "test_id = :tid and duplicate = :did and barcode = :bar";
	if( duplicate == 0 ) {
		int analyser = limits.getAnalyserID();
		if( analyser == 0 )
			condition += " and machine_cid in (" + listMachineIDs() + ")";
		else
			condition += " and machine_cid = " + String( analyser );
	}

	cluster.setSQL( "update buddy_worklist " + update + " where " + condition );
	cluster.setParam( "bar", getBarcode() );
	cluster.setParam( "did", duplicate );
	cluster.setParam( "tid", limits.getID() );
	cluster.setParam( "sts", makeStatus( result ) );
	cluster.setParam( "ts", when );
	if( cluster.execSQL() != 1 ) {
		return false;
	}
	setStage( result );
	setTimeStamp( when );
	if( resultID != 0 )
		duplicate = resultID;
	if( cached != NULL )
		*cached = *this;
	else
		LBDbWorkList::records().insert( *this );
	return true;
}

//---------------------------------------------------------------------------
//	Update a c_buddy_worklist record from the current object
//---------------------------------------------------------------------------

bool LCDbWorkEntry::updateStatus( LQuery central, LDbWorkEntry::Stage result )
{
	String update = "set status = :sts, buddy_read = :brf, time_stamp = 'now'";
	if( getResultID() != 0 )
		update = update + ", buddy_result_id = " + getResultID();

	central.setSQL( "update c_buddy_worklist " + update + " where record_no = :rno" );
	switch( result )
	{
		case LDbStage::L0_HELD:
			central.setParam( "brf", LCDbWorkEntry::HELD );
			break;

		case LDbStage::TRANSMITTED:
		case LDbStage::DELETED:
			central.setParam( "brf", LCDbWorkEntry::FINISHED );
			break;

		default:
			central.setParam( "brf", LCDbWorkEntry::COPIED );
	}

	central.setParam( "rno", getID() );
	central.setParam( "sts", makeStatus( result ) );
	if( central.execSQL() != 1 )
		return false;
	else
	{	setStage( result );
		setTimeStamp( Now() );
		return true;
	}
}

//---------------------------------------------------------------------------
//  Read fields from c_buddy_worklist but check current test profile
//---------------------------------------------------------------------------

LCDbWorkEntry::LCDbWorkEntry( const LQuery & central )
 : LDbIdBase( central.readInt( "record_no" ) ),
   LDbWorkEntry( central.readString( "barcode" ),
				 central.readInt( "project_cid" ),
				 central.readInt( "sample_id" ),
				 central.readDateTime( "in_date" ),
				 central.readInt( "test_cid" ) ),
   groupID( central.readInt( "group_id" ) ),
   profileName( central.readString( "profile_name" ) )
{
	const LPDbProfile * pp = NULL;
	const LPDbProfiles & profiles = LPDbProfiles::records( getProjectID() );
	if( !profiles.empty() )
	{
		if( central.fieldExists( "profile_id" ) )
		{
			int ppid = central.readInt( "profile_id" );
			if( ppid != 0 )
				pp = profiles.findByID( ppid );
		}
		
		if( pp == NULL && profileName.Length() > 2 )
			pp = profiles.findByName( profileName );
	}
	setProfileID( pp == NULL ? 0 : pp -> getID() );

	limits.setAnalyserID( central.readInt( "machine_cid" ) );
	limits.setPrivate( central.readChar( "private_result" ) == 'Y' );

	if( central.fieldExists( "min_value" ) && central.fieldExists( "max_value" ) )
	{
		limits.setRange( central.readDouble( "min_value" ),
						 central.readDouble( "max_value" ) );
	}

	if( central.fieldExists( "lower_warning" ) && central.fieldExists( "upper_warning" ) )
	{
		limits.setWarnLevels( central.readDouble( "lower_warning" ),
							  central.readDouble( "upper_warning" ) );
	}

	int trigger = central.readInt( "trigger_id" );
	if( trigger != 0 && central.fieldExists( "lower_trigger_limit" )
					 && central.fieldExists( "upper_trigger_limit" ) )
	{
		limits.setTriggerLimits( trigger,
							central.readDouble( "lower_trigger_limit" ),
							central.readDouble( "upper_trigger_limit" ) );
	}

	setTimeStamp( central.readDateTime( "time_stamp" ) );
	setCategoryID( central.readInt( "category_id" ) );

	if( central.fieldExists( "buddy_result_id" ) )
		setDilution( central.readDouble( "buddy_result_id" ) );
	else
		setDilution( 0 );

	if( central.fieldExists( "diluent" ) )
		setDilution( central.readDouble( "diluent" ) );
	else
		setDilution( 0 );

	switch( central.readChar( "buddy_read" ) )
	{
		case LCDbWorkEntry::COPIED:
			copied = true;
			setStage( makeStage( central.readChar( "status" ) ) );
			break;

		case LCDbWorkEntry::HELD:
			copied = true;
			setStage( LDbStage::L0_HELD );
			break;

		case LCDbWorkEntry::FINISHED:
			copied = true;
			setStage( LDbStage::TRANSMITTED );
			break;

		default:
			copied = false;
			setStage( makeStage( central.readChar( "status" ) ) );
	}
}

//---------------------------------------------------------------------------
//	Read updated c_buddy_worklist; sort entries by record number
//---------------------------------------------------------------------------

bool LCDbWorkList::read( LQuery cQuery, bool readAll )
{
	if( readAll )
		cQuery.setSQL( "select * from c_buddy_worklist order by record_no" );
	else
	{	cQuery.setSQL( "select * from c_buddy_worklist where buddy_read <> :fin"
					" and machine_cid in (" + LDbWorkEntry::listMachineIDs() + ")"
					" order by record_no" );
		cQuery.setParam( "fin", LCDbWorkEntry::FINISHED );
	}
	return readData( cQuery );
}

//---------------------------------------------------------------------------
//  Read buddy_worklist and keep records matching the given barcode
//---------------------------------------------------------------------------

bool LBDbWorkList::read( LQuery cluster, const String & barcode )
{
	String machine, search, pattern = rootString( barcode );
	if( pattern != barcode )
	{
		//  ignore status and run number of QC - templates stay on buddy
		search = "like \'" + pattern + "\%\'";
		machine.printf( "= %d", LCDbAnalysers::getCurrentID() );
	}
	else
	{   // otherwise look for exact barcode but all relevant cluster IDs
		search = "= \'" + barcode + "\'";
		machine = " in (" + LDbWorkEntry::listMachineIDs() + ')';
	}

	//  copy each record - should have different test ID or time stamp
	cluster.setSQL( "select * from buddy_worklist"
				  " where barcode " + search + " and machine_cid " + machine
				 + " order by time_stamp" );
	int count = cluster.open();
	for( cache.erase( pattern ); !cluster.eof(); cluster.next() )
		insert( LBDbWorkEntry( cluster ) );
	return count > 0;
}

//---------------------------------------------------------------------------
//  Strip the run number from the barcode if it looks like a QC
//---------------------------------------------------------------------------

String LBDbWorkList::rootString( const String & pattern )
{
	if( pattern.Length() > 8 && pattern[ 1 ] == 'Q' && pattern[ 2 ] == 'C' )
		return pattern.SubString( 1, 8 );
	else
		return pattern;
}

//---------------------------------------------------------------------------
//  Read c_buddy_worklist and keep records matching the given barcode
//---------------------------------------------------------------------------

bool LCDbWorkList::read( LQuery central, const String & barcode )
{
	central.setSQL( "select * from c_buddy_worklist"
			   " where barcode = :bar and machine_cid in (" + LDbWorkEntry::listMachineIDs() + ")"
			   " order by time_stamp, ts_sequence" );
	central.setParam( 0, barcode );
	if( central.open() < 1 )
		return false;

	while( !central.eof() )
	{
		insert( LCDbWorkEntry( central ) );
		central.next();
	}
	return true;
}

//---------------------------------------------------------------------------
//  Create part of SQL WHERE clause that finds buddy and cluster IDs
//---------------------------------------------------------------------------

String LDbWorkEntry::listMachineIDs()
{
	std::set< int > clusters = LCDbAnalysers::records().getMachineIDs();
	if( clusters.empty() )
		return LCDbAnalysers::getCurrentID();
	else
	{	std::set< int >::const_iterator mi = clusters.begin();
		String list = *mi;
		while( ++ mi != clusters.end() )
			list = list + ", " + *mi;
		return list;
    }
}

//---------------------------------------------------------------------------
//  Try to read c_buddy_worklist; copy records into buddy_worklist
//---------------------------------------------------------------------------

bool LBDbWorkList::copyCentral( LQuery cluster, LQuery central )
{
	try
	{	central.setSQL( "select * from c_buddy_worklist where buddy_read = :brd"
					" and machine_cid in (" + LDbWorkEntry::listMachineIDs() + ')' );
		central.setParam( 0, LCDbWorkEntry::NEW_RECORD );
		if( central.open() < 1 )
			return false;
	}
	catch( ... )
	{
		return false;
	}

	std::deque< LCDbWorkEntry > entries;
	do
	{	entries.push_back( LCDbWorkEntry( central ) );
		central.next();
	} while( !central.eof() );

	central.setSQL( "update c_buddy_worklist set buddy_read = :brd where record_id = :rid" );
	do
	{ 	const LCDbWorkEntry & original = entries.front();
		LBDbWorkEntry copy( original );
		copy.createRecord( cluster, original.getProfile() );
		central.setParam( "brd", LCDbWorkEntry::COPIED );
		central.setParam( "rid", original.getID() );
		central.execSQL();
		entries.pop_front();
	} while( !entries.empty() );

	return true;
}

//---------------------------------------------------------------------------
//  Check cache for active entry for test; return NULL if not found
//---------------------------------------------------------------------------

std::set< const LBDbWorkEntry * > LBDbWorkList::find( const String & barcode, int testCID ) const
{
	typedef std::multimap< String, LBDbWorkEntry >::const_iterator MapIter;
	std::pair< MapIter, MapIter > tests = cache.equal_range( rootString( barcode ) );
	std::set< const LBDbWorkEntry * > matched;
	for( MapIter ti = tests.first; ti != tests.second; ++ ti ) {
		const LBDbWorkEntry * wle = &(ti -> second);
		if( wle -> getTestLimits().getID() == testCID )	{
			matched.insert( wle );
		}
	}
	return matched;
}

//---------------------------------------------------------------------------
//  Copy given entry into cluster cache if it matches an existing one
//---------------------------------------------------------------------------

LBDbWorkEntry * LBDbWorkList::find( const LBDbWorkEntry & wle )
{
	typedef std::multimap< String, LBDbWorkEntry >::iterator MapIter;
	std::pair< MapIter, MapIter > tests = cache.equal_range( rootString( wle.getBarcode() ) );
	for( MapIter ti = tests.first; ti != tests.second; ti ++ ) {
		if( wle.getOldID() == ti -> second.getOldID()
		 && wle.getTestLimits().getID() == ti -> second.getTestLimits().getID() ) {
			return &(ti -> second);
		}
	}
	return NULL;
}

//---------------------------------------------------------------------------
//	Copy work list record into cache; signal re-runs in case wipe needed
//---------------------------------------------------------------------------

LBDbWorkEntry * LBDbWorkList::insert( const LBDbWorkEntry & wle )
{
	std::pair< String, LBDbWorkEntry > node( rootString( wle.getBarcode() ), wle );
	std::multimap< String, LBDbWorkEntry >::iterator added = cache.insert( node );
	return &(added -> second);
}

//---------------------------------------------------------------------------
//  Check cache for active entry for given test; return NULL if not found
//---------------------------------------------------------------------------

std::set< const LCDbWorkEntry * > LCDbWorkList::find( const String & barcode, int testCID ) const
{
	std::set< int > machineIDs = LCDbAnalysers::records().getMachineIDs();
	std::set< const LCDbWorkEntry * > matched;
	for( const_iterator wle = begin(); wle != end(); ++ wle ) {
		 if( wle -> getTestLimits().getID() == testCID
		  && machineIDs.count( wle -> getTestLimits().getAnalyserID() ) != 0
		  && wle -> getBarcode().AnsiCompareIC( barcode ) == 0 ) {
			matched.insert( &(*wle) );
		}
	}
	return matched;
}

//---------------------------------------------------------------------------
//  Look in buddy_worklist for tests to be re-run; return next barcode
//---------------------------------------------------------------------------

String LBDbWorkList::nextWipe( LQuery cluster )
{
	static TDateTime nextCheck;
	static std::set< String > wipeList;

	if( wipeList.empty() && nextCheck < Now() ) {
		cluster.setSQL( "Select barcode from Buddy_Worklist where status = :sts" );
		cluster.setParam( 0, LBDbWorkEntry::makeStatus( LDbStage::RE_RUN ) );
		for( cluster.open(); !cluster.eof(); cluster.next() )
			wipeList.insert( cluster.readString( 0 ) );
		nextCheck = Now() + 0.01;
	}

	//  otherwise just look at the queue for re-runs for this analyser
	String result;
	std::set< String >::iterator first = wipeList.begin();
	if( first != wipeList.end() ) {
		result = *first;
		wipeList.erase( first );
	}
	return result;
}

//---------------------------------------------------------------------------
//  Update buddy_worklist once old values have been wiped (ignore cache)
//---------------------------------------------------------------------------

bool LBDbWorkList::confirm( LQuery cluster, int project, int specimen )
{
	cluster.setSQL( "update Buddy_Worklist set buddy_read = :tra, time_stamp = :ts"
				   " where project_cid = :pid and sample_id = :sid" );
	cluster.setParam( "tra", LBDbWorkEntry::makeStatus( LDbStage::TRANSMITTED ) );
	cluster.setParam( "ts", Now() );
	cluster.setParam( "pid", project );
	cluster.setParam( "sid", specimen );
	return cluster.execSQL() > 0;
}          

//---------------------------------------------------------------------------
//	Delete QC entries for this machine but leave the rest of the cluster
//---------------------------------------------------------------------------

void LBDbWorkList::clearQCEntries( LQuery cluster )
{
	cluster.setSQL( "delete from buddy_worklist where barcode like 'QC%'"
				   " and machine_cid = :mid" );
	cluster.setParam( "mid", LCDbAnalysers::getCurrentID() );
	if( cluster.execSQL() > 0 )
		cache.clear();
}

//---------------------------------------------------------------------------
//	delete non-QC worklist records 20 hours after results transmitted
//---------------------------------------------------------------------------

unsigned LBDbWorkList::deleteOld( LQuery cluster, TDateTime flagged, TDateTime oldest )
{
	cluster.setSQL( "delete from buddy_worklist where (barcode not like 'QC%')"
				   " and (time_stamp < :old or (time_stamp < :fdt and status = :tra))" );
	cluster.setParam( "old", oldest );
	cluster.setParam( "fdt", flagged );
	cluster.setParam( "tra", LDbWorkEntry::makeStatus( LDbStage::TRANSMITTED ) );
	return cluster.execSQL();
}

//---------------------------------------------------------------------------
//	delete central worklist records 20 hours after results transmitted
//---------------------------------------------------------------------------

unsigned LCDbWorkList::deleteOld( LQuery central, TDateTime flagged, TDateTime oldest )
{
	central.setSQL( "delete from c_buddy_worklist where time_stamp < :old"
					" or (time_stamp < :ftd and group_id not in"
				  " (select group_id from c_buddy_worklist where buddy_read <> :tra))" );
	central.setParam( "old", oldest );
	central.setParam( "ftd", flagged );
	central.setParam( "tra", LCDbWorkEntry::FINISHED );
	return central.execSQL();
}

//---------------------------------------------------------------------------
//	Find recent worklist entries for the given samples; return group ID
//---------------------------------------------------------------------------

int LCDbWorkList::findGroup( LQuery cQuery, const std::set< int > & sampleIDs ) const
{
	if( sampleIDs.empty() )
		return 0;

	std::set< int >::const_iterator si = sampleIDs.begin();
	String list = *si;
	while( ++ si != sampleIDs.end() )
		list = list + ", " + *si;

	cQuery.setSQL( "select group_id from c_buddy_worklist"
				  " where project_cid = :pid and sample_id in (" + list + ")"
				  " and time_stamp >= date_trunc('day', 'now')" );
	cQuery.setParam( 0, LCDbProjects::getCurrentID() );
	return cQuery.open() >= 1 ? cQuery.readInt( 0 ) : 0;
}


//---------------------------------------------------------------------------
//	Find parent entries for the given re-runs; return buddy_sample_id
//---------------------------------------------------------------------------

int LCDbWorkList::findParentAnalysis( LQuery cQuery, const std::set< int > & recordIDs ) const
{
	if( recordIDs.empty() )
		return 0;

	std::set< int >::const_iterator si = recordIDs.begin();
	String list = *si;
	while( ++ si != recordIDs.end() )
		list = list + ", " + *si;

	cQuery.setSQL( "select distinct br.buddy_sample_id"
			" from c_worklist_relation wr, c_buddy_worklist bw, buddy_result_float br"
			" where wr.rerun_id in (" + list + ") and wr.parent_id = bw.record_no"
			" and bw.buddy_result_id = br.buddy_result_id" );
	return cQuery.open() == 1 ? cQuery.readInt( 0 ) : 0;
}

//---------------------------------------------------------------------------
//	mark any remaining members of the given group as being finished with
//---------------------------------------------------------------------------

bool LCDbWorkList::confirm( LQuery cQuery, int groupID )
{
	cQuery.setSQL( "update c_buddy_worklist set time_stamp = 'now', buddy_read = :tra"
				  " where group_id = :gid" );
	cQuery.setParam( "gid", groupID );
	cQuery.setParam( "tra", LCDbWorkEntry::FINISHED );
	return cQuery.execSQL() > 0;
}

//---------------------------------------------------------------------------
//	return cluster/analyser IDs expecting results from the given project
//---------------------------------------------------------------------------

std::set< int > LCDbWorkList::getAnalyserIDs( LQuery cQuery, int projID ) const
{
	std::set< int > analyserIDs;
	cQuery.setSQL( "select distinct machine_cid from c_buddy_worklist"
				  " where project_cid = :pid and buddy_read <> :tra" );
	cQuery.setParam( "tra", LCDbWorkEntry::FINISHED );
	cQuery.setParam( "pid", projID == 0 ? LCDbProjects::getCurrentID() : projID );
	for( cQuery.open(); !cQuery.eof(); cQuery.next() )
		analyserIDs.insert( cQuery.readInt( 0 ) );
	return analyserIDs;
}

//---------------------------------------------------------------------------
//	return category IDs in the worklist [for the given analyser/cluster]
//---------------------------------------------------------------------------

std::set< int > LCDbWorkList::getCategoryIDs( LQuery cQuery, int buddyID ) const
{
	String query = "select distinct category_id from c_buddy_worklist"
				  " where buddy_read <> :tra";
	std::set< int > idList = LCDbAnalysers::records().getMachineIDs( buddyID );
	if( !idList.empty() )
	{
		std::set< int >::const_iterator idi = idList.begin();
		query = query + " and machine_cid in (" + *idi;
		while( ++ idi != idList.end() )
			query = query + ',' + *idi;
		query += ')';
	}

	cQuery.setSQL( query );
	cQuery.setParam( "tra", LCDbWorkEntry::FINISHED );
	std::set< int > categoryIDs;
	for( cQuery.open(); !cQuery.eof(); cQuery.next() )
		categoryIDs.insert( cQuery.readInt( 0 ) );
	return categoryIDs;
}

//---------------------------------------------------------------------------
//  Create master copy of worklist entry in c_buddy_worklist
//---------------------------------------------------------------------------

bool LCDbWorkEntry::createRecord( LQuery cQuery, const String & reason )
{
	cQuery.setSQL( "Insert into c_buddy_worklist"
			" (machine_cid, barcode, test_cid, group_id, category_id, sample_id, range_set,"
			"  min_value, max_value, project_cid, profile_id, profile_name, buddy_read,"
			"  time_stamp, in_date, trigger_id, lower_trigger_limit, upper_trigger_limit,"
			"  buddy_result_id, private_result, status, diluent)"
			" values ( :mid, :bar, :tid, :grp, :cat, :sid, :rs, :mnv, :mxv, :proj, :pfid,"
					" :pnm, 'N', 'now', :ind, :trid, :ltl, :utl, :rid, :prv, 'Q', :dil)" );

	cQuery.setParam( "bar", getBarcode() );
	cQuery.setParam( "grp", getGroupID() );
	cQuery.setParam( "cat", getCategoryID() );
	cQuery.setParam( "pfid", getProfileID() );
	cQuery.setParam( "pnm", reason );
	cQuery.setParam( "dil", diluent );
	cQuery.setParam( "ind", getInDate() );
	cQuery.setParam( "rid", getResultID() );

	std::pair< int, int > specimen = getSpecimen();
	cQuery.setParam( "proj", specimen.first );
	cQuery.setParam( "sid", specimen.second );
	cQuery.setParam( "tid", limits.getID() );
	cQuery.setParam( "mid", limits.getAnalyserID() );

	std::pair< double, double > range = limits.getRange().asDoubles();
	if( range.first == 0 && range.second == 0 )
		cQuery.setParam( "rs", 'N' );
	else
		cQuery.setParam( "rs", 'Y' );
	cQuery.setParam( "mnv", range.first );
	cQuery.setParam( "mxv", range.second );

	range = limits.getTriggerLimits().asDoubles();
	cQuery.setParam( "trid", limits.getTriggerProfile() );
	cQuery.setParam( "ltl", range.first );
	cQuery.setParam( "utl", range.second );
	cQuery.setParam( "prv", limits.isPrivate() ? 'Y' : 'N' );
	return cQuery.execSQL() == 1;

	// don't add to cache - Marty's RFPs add record_no and ts_sequence
}

//---------------------------------------------------------------------------
// 	create c_buddy_worklist record(s) for QCs with the given barcode
//---------------------------------------------------------------------------

unsigned LCDbWorkList::addQCRecords( LQuery qCentral, const String & barcode, int testID )
{
	String material = barcode.SubString( 3, 2 );
	String qcLevel = barcode.SubString( 5, 1 );
	const LCDbQCLevel * qc = LCDbQCLevels::records().find( material, qcLevel );
	if( qc == NULL )
		throw Exception( "No QC defined for " + barcode );

	const LCDbProjects & pdbc = LCDbProjects::records();
	const LCDbProject * qcProj = pdbc.findByName( DbFamily().getQCDb() );
	LCDbWorkEntry qcEntry( barcode, qcProj -> getID(), 0, 0, 0 );

	static LCDbID myLCDbID;
	myLCDbID.claimNextID( qCentral );
	qcEntry.setGroupID( myLCDbID.getID() );

	unsigned added = 0;
	for( Range< LCDbQCLevel::Test > tr = qc -> getTests(); tr.isValid(); ++ tr )
	{
		if( tr -> isActive() && (testID == 0 || tr -> getTestID() == testID)
		 && tr -> getMachineID() == LCDbAnalysers::getCurrentID() )
		{
			LDbTestLimits noLimits( tr -> getTestID(), LCDbAnalysers::getCurrentID() );
			qcEntry.setTestLimits( noLimits );
			qcEntry.setDilution( tr -> getDilution() );

			const LCDbTest * test = LCDbTests::records().findByID( tr -> getTestID() );
			String name = (test == NULL ? String( tr -> getTestID() ) : test -> getName());
			qcEntry.createRecord( qCentral, qc -> getDescription().Trim() + " " + name );
			added ++;
		}
	}
	return added;
}

//---------------------------------------------------------------------------

