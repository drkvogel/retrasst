//---------------------------------------------------------------------------
//
//	Version history:
//
//		17 May 2005		Handle lower_warning if present in test_profile
//      20 September	Check if warnings missing or blank; assume 0
//      9 January 07	Split out the different profile flags
//      6 Jan. 2009		validate aliquot/test status against profile flags
//
//---------------------------------------------------------------------------

#include <vcl.h>
#include <memory>

#include "LCDbProject.h"
#include "LQuery.h"
#include "LPDbProfile.h"
#include "LCDbAuditTrail.h"

#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------
//  Read test profiles (including triggers) for selected projects
//---------------------------------------------------------------------------

LPDbProfile::LPDbProfile( const LQuery & pQuery )
 : LPDbID( pQuery.readInt( "test_profile_id" ) ),
   LDbNames( pQuery.readString( "test_profile_name" ),
			 pQuery.readString( "test_profile_description" ) ),
   changed( pQuery.readDateTime( "date_last_modified" ) ),
   LDbNoteCount( pQuery.readInt( "note_exists" ) )
{
	unsigned status = 0;
	if( pQuery.fieldExists( "profile_flags" ) )
		status = pQuery.readInt( "profile_flags" );
	if( status == LDbValid::DELETED )
	{
		active = false;
		runTests = storeSource = takeAliquots = true;
	}
	else
	{	active = true;
		runTests = (status & TEST_SAMPLE) != 0;
		takeAliquots = (status & STORE_ALIQUOTS) != 0;
		storeSource = (status & STORE_SAMPLE) != 0;
	}
}

//---------------------------------------------------------------------------

LPDbProfile::BoxType::BoxType( const LQuery & pQuery )
 : LPDbID( pQuery.readInt( "box_type_cid" ) ),
   position( pQuery.readInt( "box_order" ) ),
   LDbValid( pQuery.readDateTime( "valid_from" ),
			 pQuery.readDateTime( "valid_to" ),
			 pQuery.readInt( "status" ) )
{}

//---------------------------------------------------------------------------

bool LPDbProfiles::read( LQuery pQuery, bool readAll )
{
	if( readAll )
		pQuery.setSQL( "select * from test_profile_description"
					  " order by test_profile_id" );
	else
	{	pQuery.setSQL( "select * from test_profile_description"
					  " where profile_flags <> :del"
					  " order by test_profile_id" );
		pQuery.setParam( "del", LDbValid::DELETED );
	}
	if( !readData( pQuery ) )
		return false;

	if( readAll )
	{
		pQuery.setSQL( "select * from test_profile"
					  " order by test_profile_id, test_cid, machine_cid" );
	}
	else
	{	pQuery.setSQL( "select * from test_profile"
					  " where test_flags <> :del and 'now' between valid_from and valid_to"
					  " order by test_profile_id, test_cid, machine_cid" );
		pQuery.setParam( "del", LDbValid::DELETED );
	}

	Iterator ci = begin();
	for( pQuery.open(); !pQuery.eof(); pQuery.next() )
	{
		int did = pQuery.readInt( "test_profile_id" );
		while( ci != end() && ci -> getID() < did )
			ci ++;

		if( ci != end() && ci -> getID() == did )
			ci -> addTest( pQuery );
	}

	if( readAll )
	{
		pQuery.setSQL( "select * from aliquot_profile"
					  " order by profile_id, box_type_cid, box_order" );
	}
	else
	{	pQuery.setSQL( "select * from aliquot_profile"
					  " where status <> :sts and 'now' between valid_from and valid_to"
					  " order by profile_id, box_type_cid, box_order" );
		pQuery.setParam( "sts", LDbValid::DELETED );
	}

	ci = begin();
	for( pQuery.open(); !pQuery.eof(); pQuery.next() )
	{
		int did = pQuery.readInt( "profile_id" );
		while( ci != end() && ci -> getID() < did )
			ci ++;

		if( ci != end() && ci -> getID() == did )
			ci -> addBox( pQuery );
	}
	return true;
}

//---------------------------------------------------------------------------
//  Read the test limits from the current project database
//---------------------------------------------------------------------------

LPDbProfile::TestLimits::TestLimits( const LQuery & pQuery )
 : LPDbID( pQuery.readInt( "record_id" ) ),
   LDbTestLimits( pQuery.readInt( "test_cid" ),
				  pQuery.readInt( "machine_cid" ) )
{
	if( pQuery.fieldExists( "valid_from" ) && pQuery.fieldExists( "valid_to" ) )
		setValidDates( std::pair< TDateTime, TDateTime >(
			pQuery.readDateTime( "valid_from" ), pQuery.readDateTime( "valid_to" ) ) );

	status = pQuery.fieldExists( "test_flags" ) ? pQuery.readInt( "test_flags" ) : 0;
	local = (status & PRIVATE_RESULT) != 0;

	setRange( pQuery.readString( "lower_test_limit" ),
			  pQuery.readString( "upper_test_limit" ) );
	setTriggerLimits( pQuery.readInt( "trigger_id" ),
					  pQuery.readString( "lower_trigger_limit" ),
					  pQuery.readString( "upper_trigger_limit" ) );
	std::string high, low;
	if( pQuery.fieldExists( "lower_warning" ) )
		low = pQuery.readString( "lower_warning" );
	if( pQuery.fieldExists( "upper_warning" ) )
		high = pQuery.readString( "upper_warning" );
	setWarnLevels( low, high );
}

//---------------------------------------------------------------------------
//	Reconstruct profile flags from interpreted values
//---------------------------------------------------------------------------

int LPDbProfile::getFlagBits() const
{
	if( !active )
		return LDbValid::DELETED;

	int flags = isTested() ? TEST_SAMPLE : 0;
	if( isStored() )
		flags |= STORE_SAMPLE;
	if( isAliquoted() )
		flags |= STORE_ALIQUOTS;
	return flags;
}

//---------------------------------------------------------------------------
//	Reconstruct test profile flags from interpreted values
//---------------------------------------------------------------------------

int LPDbProfile::TestLimits::getFlagBits() const
{
	if( !isActive() )
		return LDbValid::DELETED;
		
	int flags = local ? PRIVATE_RESULT : 0;
	std::pair< double, double > limits = vRange.asDoubles();
	if( limits.first == 0 )
		flags |= DEFAULT_LOWER;
	if( limits.second == 0 )
		flags |= DEFAULT_UPPER;
	std::pair< double, double > warns = wRange.asDoubles();
	if( warns.first == 0 )
		flags |= NO_LOWER_WARNING;
	if( warns.second == 0 )
		flags |= NO_UPPER_WARNING;
	return flags;
}

//---------------------------------------------------------------------------
//	Create/update database entry for the current profile; copy to cache
//---------------------------------------------------------------------------

void LPDbProfile::saveRecord( LQuery query )
{
	if( saved )
	{
		query.setSQL( "Update test_profile_description"
				" set test_profile_name = :name, test_profile_description = :desc,"
				" profile_flags = :fl, note_exists = :ne, date_last_modified = 'now',"
				" process_cid = :pid where test_profile_id = :tpid" );
	}
	else
	{	claimNextID( query );
		query.setSQL( "Insert into test_profile_description (test_profile_id,"
					" test_profile_name, test_profile_description, note_exists,"
					" profile_flags, date_last_modified, process_cid)"
					" values ( :tpid, :name, :desc, :ne, :fl, 'now', :pid )" );
	}

	query.setParam( "tpid", getID() );
	query.setParam( "name", getName() );
	query.setParam( "desc", getDescription() );
	query.setParam( "ne", notes );
	query.setParam( "fl", getFlagBits() );
	query.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	if( query.execSQL() )
	{
		changed = Now();
		saved = true;
		LPDbProfiles::records().insert( *this );
	}
}

//---------------------------------------------------------------------------
//	Add a box type to this profile unless it has already been included
//---------------------------------------------------------------------------

void LPDbProfile::addBoxType( LQuery pQuery, int boxTypeID )
{
	const BoxType * type = boxTypes.findByID( boxTypeID );
	if( type == NULL )
	{
		BoxType bt( boxTypeID );
		pQuery.setSQL( "Insert into aliquot_profile"
					 " (profile_id, box_type_cid, box_order, valid_from, valid_to, status)"
					 "  values ( :pid, :btid, :vf, :vt, :sts )" );
		std::pair< XTIME, XTIME > dates = bt.getValidDates();
		pQuery.setParam( "pid", getID() );
		pQuery.setParam( "btid", boxTypeID );
		pQuery.setParam( "vf", dates.first );
		pQuery.setParam( "vt", dates.second );
		pQuery.setParam( "sts", LDbValid::NEW_RECORD );
		if( pQuery.execSQL() ) {
			boxTypes.insert( boxTypeID );
			LPDbProfiles::records().insert( *this );
		}
	}
}

//---------------------------------------------------------------------------
//	Add or modify limits for a test in this profile and update the cache
//---------------------------------------------------------------------------

void LPDbProfile::addTestLimits( LQuery pQuery, const TestLimits & tpe )
{
	if( findTestLimits( tpe.getID() ) == NULL )
	{
		pQuery.setSQL( "Insert into test_profile (test_profile_id, test_cid, machine_cid,"
					  " lower_test_limit, upper_test_limit, lower_warning, upper_warning,"
					  " trigger_id, upper_trigger_limit, lower_trigger_limit,"
					  " test_flags, valid_from, valid_to) values "
					  "( :pid, :tid, :mid, :llo, :lup, :wlo, :wup, :trig, :tup, :tlo, :fl, :vf, :vt)" );
	}
	else
	{	pQuery.setSQL( "Update test_profile set machine_cid = :mid, test_flags = :flg,"
					" upper_test_limit = :lup, lower_test_limit = :llo, trigger_id = :trig,"
					" upper_trigger_limit = :tup, lower_trigger_limit = :tlo,"
					" upper_warning = :wup, lower_warning = :wlo"
					" where test_profile_id = :pid and test_cid = :tid" );
	}

	pQuery.setParam( "pid", getID() );
	pQuery.setParam( "tid", tpe.getID() );
	pQuery.setParam( "mid", tpe.getAnalyserID() );
	pQuery.setParam( "flg", tpe.getFlagBits() );

	std::pair< std::string, std::string > projectRange = tpe.getRange().asStrings();
	pQuery.setParam( "llo", projectRange.first );
	pQuery.setParam( "lup", projectRange.second );

	std::pair< std::string, std::string > triggerRange = tpe.getTriggerLimits().asStrings();
	pQuery.setParam( "trig", tpe.getTriggerProfile() );
	pQuery.setParam( "tlo", triggerRange.first );
	pQuery.setParam( "tup", triggerRange.second );

	std::pair< std::string, std::string > warnRange = tpe.getWarnLevels().asStrings();
	pQuery.setParam( "wlo", warnRange.first );
	pQuery.setParam( "wup", warnRange.second );

	if( pQuery.execSQL() ) {
		tests.insert( tpe );
		LPDbProfiles::records().insert( *this );
    }
}

//---------------------------------------------------------------------------
//	Check profile has been set up correctly; return true if consistent
//---------------------------------------------------------------------------

bool LPDbProfile::validate() const
{
	if( !active )
		return true;

	short tests = 0, aliquots = 0, stored = 0;
	for( Range< BoxType > bt = getBoxTypes(); bt.isValid(); ++ bt )
	{
		if( bt -> isActive() && bt -> isAliquot() )
			aliquots ++;
		if( bt -> isActive() && bt -> isSample() )
			stored ++;
	}

	for( Range< TestLimits > tt = getTests(); tt.isValid(); ++ tt )
		if( tt -> isActive() )
			tests ++;

	return (isTested() ? tests > 0 : tests == 0)
		&& (isAliquoted() ? aliquots > 0 : aliquots <= 0)
		&& (isStored() ? stored == 1 : stored != 1);
}

//---------------------------------------------------------------------------

const LPDbProfile * LPDbProfiles::findByName( const std::string & name ) const {
		return findMatch( LDbNames::LCMatcher( name ) );
}

//---------------------------------------------------------------------------

