/*---------------------------------------------------------------------------
 *
 *	Version history:
 *
 *	   	31 May 2005		Use LDbStage for status; convert to/from integer
 *						for specimen table, character for buddy_database
 *      9 Jun '05, NG:	Incorporate start_delimiter if merging values
 *      16 June 2005	Add descriptor name as attribute for value fields
 *      12 July, NG:	Add method to get value field by descriptor name
 *      8 September     Only update status and note_exists in addStatus()
 *      1 Nov. 2005:	Added LPDbSource::readByDescriptor() to select records
 *      8 Nov 05, NG:	Store buddy ID in buddy_database, not cluster ID
 *						Added operator ID to LXDbSource (set by BoxReception)
 *		26 January 06:	Check barcode for invalid characters before saving
 *      14 February		Extend readByDescriptor() to check for any value
 *		10 March, NG:	minimised static methods to allow multiple caches
 *      27 Feb. 2007:	add but don't overwrite specimen values when parsing
 *
 *		11 Jan 08, NG:	modify LPDbSource:readCurrent() to read swiped only
 *		15 Jan 08, NG:	removed LSDbSource and LXDbSource - no need for XML
 *      1 May 08, NG:  	replace addStatus() by setStatus() [no specimen_action]
 *      24 Sep 08, NG:	pass list of descriptor value to readByDescriptors()
 *		2 October 2008	rework profile handling to reduce recalculation
 *		7 Oct 08, NG:	added NOTIFIED stage (specimen.status = 5)
 *		16-10-08, NG:	generalised matching to use descriptor/value list
 *      12/10/08, NG:	simplified date handling; moved LPDbSource::read()
 *      06.11.08, NG:	incorporate ValueFields code for values and filter
 *      11.12.08, NG:	remember if a sample's been saved; insert if not
 *      12/05/09, NG:	set specimen swipe time and operator explicitly
 *      11/06/09, NG:	added findDetails() method to merge existing data
 *		17 Jun 09, NG:	simplified handling of delete time-stamp
 *		20 Aug 09, NG:	added beckman-specific columns for analysis data
 *		13/10/10, NG:	Ignore orphaned entries deleting from buddy_database
 *      18 Jan 11, NG:	Don't assume current project in LBDbSource constructor
 *	
 *---------------------------------------------------------------------------*/

#include <vcl.h>
#include <string>
#include <sstream>

#include "LDbSource.h"
#include "LQuery.h"
#include "LDbWorklist.h"
#include "LDbResult.h"
#include "LCDbProject.h"
#include "LCDbAuditTrail.h"
#include "LCDbAnalyser.h"
#include "LPDbProfileMap.h"
#include "LDbNotes.h"
#include "LCDbOperator.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//	Read a buddy_database entry and work out which project it's from
//---------------------------------------------------------------------------

LBDbSource::LBDbSource( const LQuery & query )
 : LBDbID( query.readInt( "Buddy_Sample_id" ) ),
   LDbSource( query.readString( "barcode" ) ),
   buddyID( query.readInt( "machine_cid" ) )
{
	const LCDbProjects & projects = LCDbProjects::records();
	std::string dbName = query.readString( "Database_name" );
	const LCDbProject * proj = projects.findByName( dbName );
//	if( proj == NULL && isQC() )             FIXME
//		proj = projects.findByName( DbFamily().getQCDb() );
	specimen.first = (proj == NULL) ? 0 : proj -> getID();
	specimen.second = query.readInt( "Alpha_Sample_id" );

	if( query.fieldExists( "test_type" ) )
	   testType = query.readString( "test_type" );

	if( query.fieldExists( "sample_type" ) )
	   sampleType = query.readString( "sample_type" );

	position.first = query.fieldExists( "rack_number" ) ? query.readInt( "rack_number" ) : 0;
	position.second = query.fieldExists( "cup_number" ) ? query.readInt( "cup_number" ) : 0;

	notes = query.readInt( "Note_Exists" );
	setTimeStamp( query.readDateTime( "date_analysed" ) );
	std::string flags = query.readString( "action_flag" );
	setStage( makeStage( flags[0] ) );
}

//---------------------------------------------------------------------------

LBDbSource::LBDbSource( const std::string & barcode, TDateTime programmed )
 : LDbSource( barcode ), buddyID( LCDbAnalysers::getCurrentID() ), position( 0, 0 )
{
	const LCDbProjects & projects = LCDbProjects::records();
	const LCDbProject * proj = NULL;
//	if( isQC() )	FIXME
//		proj = projects.findByName( DbFamily().getQCDb() );
	specimen.first = (proj == NULL ? 0 : proj -> getID());
	specimen.second = 0;
	setTimeStamp( programmed );
}

//---------------------------------------------------------------------------
//	Initialise the cache with (recent) buddy_database entries
//---------------------------------------------------------------------------

bool LBDbSources::read( LQuery buddyQuery, bool readAll )
{
	if( readAll )
		buddyQuery.setSQL( "select * from buddy_database order by Buddy_Sample_id" );
	else
	{	std::stringstream sql;
		sql << "select * from buddy_database where machine_cid in (";
		std::set< int > idList = LCDbAnalysers::records().getMachineIDs();
		std::set< int >::const_iterator idi = idList.begin();
		sql << *idi;
		while( ++ idi != idList.end() )
			sql << ',' << *idi;
		sql << ") and action_flag <> :tra order by Buddy_Sample_id";
		buddyQuery.setSQL( sql.str() );
		buddyQuery.setParam( "tra", LBDbSource::action( LDbStage::TRANSMITTED ) );
	}
	return readData( buddyQuery );
}

//---------------------------------------------------------------------------
//	Create or update buddy_database record and corresponding cache entry
//---------------------------------------------------------------------------

bool LBDbSource::saveRecord( LQuery bQuery )
{
	if( buddyID == 0 || LCDbAnalysers::records().findByID( buddyID ) == NULL )
		buddyID = LCDbAnalysers::getCurrentID();

	const LCDbProject * proj = NULL;
	if( specimen.first != 0 )
		proj = LCDbProjects::records().findByID( specimen.first );

	std::string pName = "Unknown";
	if( proj != NULL )
	{
		if( getStage() >= LDbStage::TRANSMITTED )
			pName = proj -> getDbName();
		else
			pName = proj -> getName();
	}
//	else if( isQC() )		FIXME
//		pName = DbFamily().getQCDb();

	short delFlag = (getStage() >= LDbStage::TRANSMITTED) ? 2 : saved ? 1 : 0;

	if( !saved )
	{
		if( getID() == 0 )
			claimNextID( bQuery );

		bQuery.setSQL( "INSERT into buddy_database"
					" (Buddy_Sample_id, Barcode, machine_cid, Date_analysed, Action_Flag,"
					"  Database_name, Alpha_Sample_id, Note_Exists,"
					"  update_when, rack_number, cup_number, test_type, sample_type)"
					" values ( :sid, :bar, :mid, :dta, :flg, :pdb, :aid, :nex,"
							 " :kud, :rno, :cno, :typ, :sty )" );
		bQuery.setParam( "bar", getBarcode() );
		bQuery.setParam( "dta", XTIME(getTimeStamp()) );
	}
	else
	{	bQuery.setSQL( "Update buddy_database"
					" set database_name = :pdb, alpha_sample_id = :aid, action_flag = :flg,"
					" sample_type = :sty, test_type = :typ, rack_number = :rno, cup_number = :cno,"
					" note_exists = note_exists + :nex, update_when = :kud"
					" where buddy_sample_id = :sid and machine_cid = :mid" );
	}

	bQuery.setParam( "sid", getID() );
	bQuery.setParam( "mid", buddyID );
	bQuery.setParam( "flg", action( getStage() ) );
	bQuery.setParam( "pdb", pName );
	bQuery.setParam( "aid", specimen.second );
	bQuery.setParam( "nex", notes );
	XTIME now;
	bQuery.setParam( "kud", now );
	bQuery.setParam( "rno", position.first );
	bQuery.setParam( "cno", position.second );
//	bQuery.setParam( "dsf", delFlag );
	bQuery.setParam( "typ", testType );
	bQuery.setParam( "sty", sampleType );
	if( !bQuery.execSQL() )
		return false;

	saved = true;
	LBDbSources::records().insert( *this );
	return true;
}

//---------------------------------------------------------------------------

LDbStage::Stage LBDbSource::makeStage( char action_flag )
{
	return LBDbResult::makeStage( action_flag );
}

//---------------------------------------------------------------------------

char LBDbSource::action( LDbStage::Stage stage )
{
	return LBDbResult::action( stage );
}

//---------------------------------------------------------------------------
//	Find newest buddy_database entry by project+sample ID or barcode
//---------------------------------------------------------------------------
/*
const LBDbSource * LBDbSources::find( const LBDbSource & sample ) const
{
	std::pair< int, int > wanted = sample.getSpecimen();
	TDateTime minDate = sample.getTimeStamp() - 1, maxDate = TDate::CurrentDate() + 1;
	const LBDbSource * found = NULL;
	for( const_iterator next = begin(); next != end(); ++ next )
	{
		if( next -> getTimeStamp() > minDate && next -> getTimeStamp() < maxDate )
		{
			std::pair< int, int > possible = next -> getSpecimen();
			if( wanted.second != 0 && possible.second != 0
			  ? wanted == possible : sample.getBarcode() == next -> getBarcode() )
			{
				found = &(*next);
				minDate = found -> getTimeStamp();
			}
		}
	}
	return found;
}
*/

LPDbSource::LPDbSource( const LPDbSource & other )
 : LPDbID( other.getID() ), LDbSource( other ), ValueFields( other ), profileID( other.getProfileID() ),
  operatorID( other.getSwipeUserID() ), appointment( other.getAppointment() ), swipeTime( other.getSwipeTime() )
{}

LPDbSource::LPDbSource( const LBDbSource & other )
 : LPDbID( other.getSpecimen().second ), LDbSource( other ), profileID( 0 ), operatorID( 0 )
{}

LPDbSource::LPDbSource( const LDbSource & other )
 : LDbSource( other ), profileID( 0 ), operatorID( 0 )
{}

//---------------------------------------------------------------------------
//	accept one or more descriptor values - barcode is a special case
//---------------------------------------------------------------------------

LPDbSource::LPDbSource( const ValueFields & values )
 : profileID( 0 ), operatorID( 0 ) {
	for( Range< ValueField > vf = values.getValueFields(); vf.isValid(); ++ vf ) {
		if( vf->getField() == "barcode" ) {
			setBarcode( vf->getValue() );
		} else {
			insert( *vf );
		}
	}
}

//---------------------------------------------------------------------------
//	accept details for new specimen; assume new unless swiped recently
//---------------------------------------------------------------------------

LPDbSource::LPDbSource( const std::string & barcode, TDateTime taken, TDateTime arrived )
 : LDbSource( barcode ), appointment( taken ), swipeTime( arrived ), profileID( 0 ), operatorID( 0 )
{
	if( arrived > TDate( 2000, 1, 1 ) ) {
		setStage( LDbStage::ARRIVED );
	}
}

//---------------------------------------------------------------------------
//	Read a specimen entry and determine what stage it's likely to be at
//---------------------------------------------------------------------------

LPDbSource::LPDbSource( const LQuery & query )
 : LPDbID( query.readInt( "Sample_id" ) ),
   LDbSource( query.readString( "barcode" ) ),
   profileID( query.readInt( "profile_id" ) ),
   operatorID( query.readInt( "operator_cid" ) ),
   appointment( query.readDateTime( "specimen_taken" ) ),
   swipeTime( query.readDateTime( "swipe_time" ) )
{
	setField( "external_name", query.readString( "external_name" ) );
	addValueFields( query );
	notes = query.readInt( "Note_Exists" );
	TDateTime tdInsert( query.readDateTime( "insert_when" ) );
	TDateTime tdUpdate( query.readDateTime( "update_when" ) );
	setTimeStamp( tdInsert > tdUpdate ? tdInsert : tdUpdate );
	setStage( makeStage( query.readInt( "status" ) ) );
}

//---------------------------------------------------------------------------
//	Copy details of current record from cache or specimen table
//---------------------------------------------------------------------------

bool LPDbSource::findDetails( LQuery qProject )
{
	if( getID() == 0 )
		return false;

	LPDbSources & cache = LPDbSources::records();
	const LPDbSource * found = cache.findByID( getID() );
	if( found == NULL )
	{
		found = cache.readRecord( qProject, getID() );
		if( found == NULL )
			return false;
	}

	*this = *found;
	return true;
}

//---------------------------------------------------------------------------
//	Fill the current project's cache with (recent) specimen entries
//---------------------------------------------------------------------------

bool LPDbSources::read( LQuery pQuery, LPDbSource::Stage stage )
{
	pQuery.setSQL( "select * from specimen where status = :swiped order by sample_id" );
	pQuery.setParam( "swiped", LPDbSource::makeStatus( stage ) );
	return readData( pQuery );
}

//---------------------------------------------------------------------------
//	Read specimen entries with given descriptor values; false if none
//---------------------------------------------------------------------------

bool LPDbSources::readByDescriptors( LQuery pQuery, const ValueFields & values )
{
	pQuery.setSQL( "select * from specimen " + values.makeWhereClause() + " order by sample_id" );
	return readData( pQuery );
}

//---------------------------------------------------------------------------
//	Read the selected specimen entry from the current project database
//---------------------------------------------------------------------------

const LPDbSource * LPDbSources::readRecord( LQuery pQuery, int sid )
{
	pQuery.setSQL( "select * from specimen where sample_id = :sid" );
	pQuery.setParam( "sid", sid );
	return pQuery.open() == 1 ? insert( LPDbSource( pQuery ) ) : NULL;
}

//---------------------------------------------------------------------------
//	Read entry from buddy_database for a recently programmed sample
//---------------------------------------------------------------------------

const LBDbSource * LBDbSources::readRecord( LQuery qCluster, int bsid )
{
	qCluster.setSQL( "select * from buddy_database"
					" where buddy_sample_id = :sid" );
	qCluster.setParam( "sid", bsid );
	return qCluster.open() == 1 ? insert( LBDbSource( qCluster ) ) : NULL;
}

//---------------------------------------------------------------------------

const LBDbSource * LBDbSources::readRecord( LQuery qCluster, std::pair< int, int > specimen )
{
	qCluster.setSQL( "select * from buddy_database"
					" where alpha_sample_id = :sid and machine_cid = :mid"
					" order by date_analysed desc" );
	qCluster.setParam( "sid", specimen.second );
	qCluster.setParam( "mid", LCDbAnalysers::getCurrentID() );
	for( qCluster.open(); !qCluster.eof(); qCluster.next() ) {
		LBDbSource previous( qCluster );
		if( previous.getSpecimen().first == specimen.first ) {
			return insert( previous );
		}
	}
	return NULL;
}

//---------------------------------------------------------------------------

const LBDbSource * LBDbSources::readRecord( LQuery qCluster,
			const std::string & barcode, const std::string & test, const std::string & sample )
{
	qCluster.setSQL( "select * from buddy_database"
					" where barcode = :bar and machine_cid = :mid"
					" order by date_analysed desc" );
	qCluster.setParam( "bar", barcode );
	qCluster.setParam( "mid", LCDbAnalysers::getCurrentID() );
	for( qCluster.open(); !qCluster.eof(); qCluster.next() ) {
		LBDbSource bdb( qCluster );
		if( (sample.empty() || bdb.getSampleType().empty() || sample == bdb.getSampleType())
		 && (test.empty() || bdb.getTestType().empty() || test == bdb.getTestType()) ) {
			return insert( bdb );
		}
	}
	return NULL;
}

//---------------------------------------------------------------------------
//	record when a sample arrived and who swiped it in
//---------------------------------------------------------------------------

void LPDbSource::setArrived( TDateTime when, int processID )
{
	swipeTime = when;
	if( processID == 0 || processID == LCDbAuditTrail::getCurrent().getProcessID() )
		operatorID = LCDbOperators::getCurrentID();
	else
		operatorID = LCDbAuditTrail::getCurrent().findUserID( processID );
	setStage( ARRIVED );
}

//---------------------------------------------------------------------------
//	Determine the expected specimen status flag for the given stage
//---------------------------------------------------------------------------

short LPDbSource::makeStatus( LDbStage::Stage stage )
{
	switch( stage )
	{
		case LDbStage::NEW_ENTRY:
		case LDbStage::EXPECTED:
			return LPDbSource::NEW_RECORD;

		case LDbStage::L2_VALIDATED:
		case LDbStage::INVALIDATED:
		case LDbStage::TRANSMITTED:
			return LPDbSource::RESULTS;

		case LDbStage::COLLECTED:
			return LPDbSource::DONE;

		case LDbStage::ALLOCATED:
		case LDbStage::ALIQUOTED:
			return LPDbSource::BOXED;

		case LDbStage::NOTIFIED:
			return LPDbSource::WAITING;

		case LDbStage::DELETED:        
			return LDbValid::DELETED;

		default:
			return LPDbSource::SWIPED;
	}
}

//---------------------------------------------------------------------------
//	Determine the most likely stage for the given specimen status value
//---------------------------------------------------------------------------

LDbStage::Stage LPDbSource::makeStage( short status )
{
	switch( status )
	{
		default:
			return LDbStage::EXPECTED;

		case LPDbSource::SWIPED:
			return LDbStage::ARRIVED;

		case LPDbSource::RESULTS:
			return LDbStage::TRANSMITTED;

		case LPDbSource::DONE:
			return LDbStage::COLLECTED;

		case LPDbSource::BOXED:
			return LDbStage::ALIQUOTED;

		case LPDbSource::WAITING:
			return LDbStage::NOTIFIED;

		case LDbValid::DELETED:
			return LDbStage::DELETED;
	}
}

//---------------------------------------------------------------------------
//	Allocate a new ID and set up a corresponding external name
//---------------------------------------------------------------------------

void LPDbSource::createName( LQuery project, const std::string & nameBase )
{
	setID( 0 );
	claimNextID( project );
	std::stringstream out;
	out << nameBase << "__" << getID();
	setField( "external_name", out.str() );
	setStage( LDbStage::EXPECTED );
	saved = false;
}

//---------------------------------------------------------------------------
//	Create or update specimen record, including any values in the map
//---------------------------------------------------------------------------

bool LPDbSource::saveRecord( LQuery pQuery )
{
	if( saved ) {
		std::string fList = "swipe_time = :swt, operator_cid = :oid, note_exists = note_exists + :nex,"
					  " update_when = :upd, status = :sts, profile_id = :pro, process_cid = :pid";
		for( Range< ValueField > ff = getValueFields(); ff.isValid(); ++ ff )
			fList = fList + ", " + ff -> getField() + " = '" + ff -> getValue() + "'";
		pQuery.setSQL( "Update Specimen set " + fList + " where sample_id = :sid" );
	}
	else
	{ 	if( getID() == 0 )
			claimNextID( pQuery );       // assume external_name exists

		std::string fList = "sample_id, barcode, swipe_time, operator_cid, note_exists,"
					  " insert_when, status, profile_id, process_cid";
		std::string vList = ":sid, :bar, :swt, :oid, :nex, :upd, :sts, :pro, :pid";
		for( Range< ValueField > ff = getValueFields(); ff.isValid(); ++ ff )
		{
			fList = fList + ", " + ff -> getField();
			vList = vList + ", '" + ff -> getValue() + "'";
		}
		pQuery.setSQL( "Insert into specimen ( " + fList + " ) values( " + vList + " )" );
		pQuery.setParam( "bar", getBarcode() );
	}

	short status =  makeStatus( getStage() );
	if( status == LPDbSource::SWIPED )
	{
		const static TDate FIRST_SAMPLE( 2000, 1, 1 );
		if( swipeTime < FIRST_SAMPLE )
			swipeTime = Now();

		if( operatorID == 0 )
			operatorID = LCDbOperators::getCurrentID();
	}

	if( profileID == 0 )
	{
		const LPDbProfileMap * pm = findProfileMap();
		if( pm != NULL )
			profileID = pm -> getProfileID();
	}

	XTIME now;
	pQuery.setParam( "sid", getID() );
	pQuery.setParam( "oid", operatorID );
	pQuery.setParam( "swt", XTIME(swipeTime) );
	pQuery.setParam( "nex", notes );
	pQuery.setParam( "pro", profileID );
	pQuery.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	pQuery.setParam( "sts", status );
	pQuery.setParam( "upd", now );
	if( !pQuery.execSQL() )
		return false;

	saved = true;
	setTimeStamp( now.outputTDateTime() );
	LPDbSources::records().insert( *this );
	return true;
}

//---------------------------------------------------------------------------
//	Add a note to this entry (create note; create or update specimen)
//---------------------------------------------------------------------------

bool LPDbSource::saveWithNote( LQuery pQuery, const LDbNote & details )
{
	notes ++;
	return saveRecord( pQuery ) && LPDbNote( details, getID() ).saveRecord( pQuery );
}

//---------------------------------------------------------------------------
//	Find a specimen entry from the cache with the given external_name
//---------------------------------------------------------------------------

class LPDbSources::Matcher : public std::unary_function< LPDbSource, bool >
{
	const std::string field, value;

public:

	Matcher( const std::string & fName, const std::string & fValue )
	 : field( fName ), value( fValue )
	{}

	operator std::string() const { return value; }

	bool operator()( const LPDbSource & other ) const
	{
		return other.getValue( field ).compare( value ) == 0;
	}
};

//---------------------------------------------------------------------------

const LPDbSource * LPDbSources::findByName( const std::string & externalName ) const
{
	return findMatch( Matcher( "external_name", externalName ) );
}

//---------------------------------------------------------------------------
//	Update specimen record (and cache) to show new status; count changes
//---------------------------------------------------------------------------

int LPDbSources::setStatus( LQuery pQuery, const std::set< int > & samples, LPDbSource::Stage stage )
{
	if( samples.empty() )
		return 0;

	std::auto_ptr< TStrings > idList( new TStringList );
	for( std::set< int >::const_iterator ci = samples.begin(); ci != samples.end(); ci ++ ) {
		idList -> Add( *ci );
    }
	std::string ids = AnsiString( idList -> CommaText ).c_str();
	pQuery.setSQL( "Update Specimen set status = :sts where sample_id in (" + ids + ")" );
	pQuery.setParam( "sts", LPDbSource::makeStatus( stage ) );
	int count = 0;
	if( pQuery.execSQL() ) {
		for( std::set< int >::const_iterator ci = samples.begin(); ci != samples.end(); ci ++ ) {
			LPDbSource * found = find( *ci );
			if( found != NULL ) {
				found -> setStage( stage );
				count ++;
			}
		}
	}
	return count;
}

//---------------------------------------------------------------------------
//	read specimen records scanned into the project between the given dates
//---------------------------------------------------------------------------

bool LPDbSources::readByScanDate( LQuery pQuery, TDateTime start, TDateTime finish )
{
	pQuery.setSQL( "select * from specimen where swipe_time between :st and :ft"
				  " order by sample_id" );
	pQuery.setParam( "st", XTIME(start) );
	pQuery.setParam( "ft", XTIME(finish) );
	return readData( pQuery );
}

//---------------------------------------------------------------------------
//	delete analyses after all the results have been transmitted
//---------------------------------------------------------------------------

bool LBDbSources::deleteOld( LQuery cluster, TDateTime flagged, TDateTime oldest )
{
	cluster.setSQL( "delete from buddy_database"
					" where (update_when < :fdt)"
					" or (update_when < :old and action_flag <> :hld)" );
	cluster.setParam( "old", XTIME(oldest) );
	cluster.setParam( "hld", LBDbSource::action( LDbStage::L0_HELD ) );
	cluster.setParam( "fdt", XTIME(flagged) );
	return cluster.execSQL();
}

//---------------------------------------------------------------------------

