//---------------------------------------------------------------------------
//
//	Revision history
//
//		14 June 2005:	move code lookups from LCDbTests to LIMSResult
//  	9 September:    deleteMachine() checks records deleted from database
//  	10 March 2006:	Modified cache handling to allow two copies
//      30 March, NG:	Use c_test_definition.valid_from/valid_to if present
//      17/02/09, NG:   Use c_test_machine.valid_from/valid_to if present
//		10/03/09, NG:	Keep test limits as Strings where possible
//		30/09/09, NG:	Allow multiple entries in c_test_machine for DxH
//      22/01/01, NG:	Restored Menarini findTestID( protocol, testOrder )
//      22/02/10, NG:	Modifed findCodes to facilitate returning protocol
//      8 Feb 12, NG:	Link sample type code to machine as well as test
//      11 May 12, NG:	Include calculated tests when finding sample type
//
//---------------------------------------------------------------------------

#include <vcl.h>

#include "LCDbTest.h"
#include "LQuery.h"
#include "LCDbProject.h"
#include "LCDbAnalyser.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

LCDbTest::LCDbTest( const LQuery & query )
 : LCDbID( query.readInt( "test_cid" ) ),
   LDbNames( query.readString( "external_name" ),
			 query.readString( "external_full" ) ),
   LDbValid( query.readDateTime( "valid_from" ),
			 query.readDateTime( "valid_to" ),
			 query.readShort( "status" ) ),
   limits( query.readDouble( "default_lower_limit" ),
		   query.readDouble( "default_upper_limit" ) )
{
	if( (query.fieldExists( "min_datapoints" ) && query.readInt( "min_datapoints" ) != REQUIRED_DATA_POINTS)
	 || (query.fieldExists( "max_datapoints" ) && query.readInt( "max_datapoints" ) != REQUIRED_DATA_POINTS) )
	{
		throw Exception( "Invalid result definition for " + getName() );
	}

	if( query.fieldExists( "data_type" ) )
		dataType = query.readShort( "data_type" );
	else
		dataType = REAL_DATA;

	if( query.fieldExists( "sample_type" ) )
	   sampleType = query.readString( "sample_type" );

	if( query.fieldExists( "precision" ) )
		precision = query.readShort( "precision" );
	else
		precision = 2;
}

//---------------------------------------------------------------------------
//	Read definitions of analysed and calculated tests; assumes they
//	have been registered in _ddb if they exist in the central database
//---------------------------------------------------------------------------

bool LCDbTests::read( LQuery cQuery, bool readAll )
{
	if( readAll )
		cQuery.setSQL( "select * from c_test order by test_cid" );
	else
	{	cQuery.setSQL( "select * from c_test"
					  " where status <> :sts and 'now' between valid_from and valid_to"
					  " order by test_cid" );
		cQuery.setParam( 0, LCDbTest::DELETED );
	}
	if( !readData( cQuery ) )
		return false;

	if( readAll )
	{
		cQuery.setSQL( "select * from c_test_machine"
					  " order by test_cid, record_cid" );
	}
	else
	{	cQuery.setSQL( "select * from c_test_machine"
					  " where status <> :sts and 'now' between valid_from and valid_to"
					  " order by test_cid, record_cid" );
		cQuery.setParam( 0, LCDbTest::DELETED );
	}

	iterator ci = begin();
	for( cQuery.open(); !cQuery.eof(); cQuery.next() )
	{
		int did = cQuery.readInt( "test_cid" );
		while( ci != end() && ci -> getID() < did )
			ci ++;

		if( ci != end() && ci -> getID() == did )
			ci -> addMachineDef( cQuery );
	}

	if( readAll )
	{
		cQuery.setSQL( "select * from c_test_definition"
					  " order by result_test_cid, record_cid" );
	}
	else
	{	cQuery.setSQL( "select * from c_test_definition"
					  " where status <> :sts and 'now' between valid_from and valid_to"
					  " order by result_test_cid, record_cid" );
		cQuery.setParam( 0, LCDbTest::DELETED );
	}

	ci = begin();
	for( cQuery.open(); !cQuery.eof(); cQuery.next() )
	{
		int did = cQuery.readInt( "result_test_cid" );
		while( ci != end() && ci -> getID() < did )
			ci ++;

		if( ci != end() && ci -> getID() == did )
			ci -> addCalcDef( cQuery );
	}
	return true;
}

//---------------------------------------------------------------------------
//  Read operands of calculated results; should be applied in order
//---------------------------------------------------------------------------

LCDbTest::CalcDef::CalcDef( const LQuery & query )
 : LCDbID( query.readInt( "record_cid" ) ),
   LDbValid( query.readDateTime( "valid_from" ),
			 query.readDateTime( "valid_to" ),
			 query.readShort( "status" ) ),
   calc( query.readShort( "calculation" ) ),
   sourceTest( query.readInt( "source_test_cid" ) ),
   multiple( query.readDouble( "source_multiple" ) )
{}

//---------------------------------------------------------------------------

void LCDbTest::addCalcDef( const LQuery & defs )
{
	calculations.insert( CalcDef( defs ) );
}

//---------------------------------------------------------------------------
//	Copy test machine name definitions for each test; can search later
//---------------------------------------------------------------------------

LCDbTest::MachineDef::MachineDef( const LQuery & query )
 : LCDbID( query.readInt( "record_cid" ) ),
   LDbValid( query.readDateTime( "valid_from" ),
			 query.readDateTime( "valid_to" ),
			 query.readShort( "status" ) ),
   analyser( query.readInt( "machine_cid" ) ),
   code( query.readString( "machine_test_name" ) ),
   protocol( query.readShort( "protocol" ) ),
   testOrder( query.readShort( "test_order" ) )
{
	if( query.fieldExists( "sample_type" ) )
	   sampleType = query.readString( "sample_type" );
}

//---------------------------------------------------------------------------

void LCDbTest::addMachineDef( const LQuery & defs )
{
	MachineDef detail( defs );
	if( detail.getSampleType().IsEmpty() || detail.getSampleType() == "." )
		detail.setSampleType( sampleType );
	machineDefs.insert( detail );
}

//---------------------------------------------------------------------------
//	Create a new database entry for an analyser; copy into cache
//---------------------------------------------------------------------------

void LCDbTest::saveMachine( LQuery query, MachineDef definition )
{
	const MachineDef * current = machineDefs.findByID( definition.getID() );
	if( current == NULL )
	{
		if( definition.getID() == 0 )
			definition.claimNextID( query );

		query.setSQL( "Insert into c_test_machine"
					" (test_cid, machine_cid, machine_test_name, test_order, sample_type,"
					 " protocol, valid_from, valid_to, status, record_cid"
					" values ( :tid, :mid, :tname, :tor, :typ, :prr, :sdt, :edt, :sts, :rid )" );
		query.setParam( "tid", getID() );
		query.setParam( "mid", definition.getMachineID() );
	}
	else
	{	query.setSQL( "Update c_test_machine"
					 " set machine_test_name = :tname, test_order = :tor, protocol = :prr,"
					 " valid_from = :sdt, valid_to = :edt, status = :sts, sample_type = :typ"
					 " where record_cid = :rid" );
	}

	query.setParam( "rid", definition.getID() );
	query.setParam( "tname", definition.getCode() );
	query.setParam( "typ", sampleType );
	query.setParam( "tor", definition.getTestOrder() );
	query.setParam( "prr", definition.getProtocol() );

	std::pair< TDate, TDate > dates = definition.getValidDates();
	query.setParam( "sdt", dates.first );
	query.setParam( "edt", dates.second );
	query.setParam( "sts", definition.isActive() ? RECORD_IN_USE : DELETED );
	if( query.execSQL() > 0 )
	{
		machineDefs.insert( definition );
		LCDbTests::records().insert( *this );
	}
}

//---------------------------------------------------------------------------
//	Crate or replace database entries for a calculated test result
//---------------------------------------------------------------------------

void LCDbTest::saveCalcParts( LQuery query, Range< CalcDef > parts )
{
	query.setSQL( "update c_test_definition"
				 " set status = :sts, valid_to = 'now'"
				 " where result_test_cid = :rid and status <> :sts"
				 " and (valid_to is null or valid_to > 'now')" );
	query.setParam( "rid", getID() );
	query.setParam( "sts", DELETED );
	query.execSQL();

	calculations.clear();
	while( parts.isValid() )
	{
		CalcDef def( *parts );
		def.saveRecord( query, calculations.size(), getID() );
		calculations.insert( def );
		++ parts;
	}
	LCDbTests::records().insert( *this );
}

//---------------------------------------------------------------------------
//	Create or update database entry for (part of) a calculated result
//---------------------------------------------------------------------------

bool LCDbTest::CalcDef::saveRecord( LQuery query, short part, int testID )
{
	if( getID() == 0 )
		claimNextID( query );

	query.setSQL( "Insert into c_test_definition"
				" (result_test_cid, calculation, test_position, source_test_cid,"
				" source_multiple, valid_from, valid_to, record_cid, status)"
				" values ( :tid, :cal, :pos, :sid, :mul, :sdt, :edt, :rid, :sts )" );
	query.setParam( "rid", getID() );
	query.setParam( "tid", testID );
	query.setParam( "pos", part );
	query.setParam( "cal", calc );
	query.setParam( "sid", sourceTest );
	query.setParam( "mul", multiple );
	std::pair< TDate, TDate > dates = getValidDates();
	query.setParam( "sdt", dates.first );
	query.setParam( "edt", dates.second );
	query.setParam( "sts", status );
	return (query.execSQL() == 1);
}

//---------------------------------------------------------------------------
//	Create a new database entry for the current test; copy to cache
//---------------------------------------------------------------------------

bool LCDbTest::saveRecord( LQuery query )
{
	if( saved )
	{
		query.setSQL( "update c_test"
					" set external_full = :full, valid_from = :from, valid_to = :to, status = :sts,"
					" default_lower_limit = :low, default_upper_limit = :upl, precision = :prc"
					" where test_cid = :tid" );
	}
	else
	{	claimNextID( query );
		query .setSQL( "INSERT into c_test (test_cid, external_name, external_full, data_type,"
					" valid_from, valid_to, status, default_lower_limit, default_upper_limit, precision)"
					" values ( :tid, :enm, :full, :dt, :from, :to, :sts, :low, :upl, :prc )" );
		query.setParam( "dt", dataType );
		query.setParam( "enm", getName() );
	}

	query.setParam( "sts", status );
	std::pair< TDate, TDate > dates = getValidDates();
	query.setParam( "from", dates.first );
	query.setParam( "to", dates.second );

	if( isActive() )
		status = (machineDefs.empty() ? 0 : ANALYSED) | (calculations.empty() ? 0 :	CALCULATED);

	query.setParam( "tid", getID() );
	query.setParam( "full", getDescription() );
	query.setParam( "sty", sampleType );
	query.setParam( "prc", precision );

	std::pair< String, String > range = limits.asStrings();
	query.setParam( "low", range.first );
	query.setParam( "upl", range.second );
	if( query.execSQL() != 1 )
		return false;

	saved = true;
	LCDbTests::records().insert( *this );
	return true;
}

//---------------------------------------------------------------------------
// 	Find matching definition for the test on the given analyser
//---------------------------------------------------------------------------

const LCDbTest::MachineDef * LCDbTest::findMachineDef( int buddyID ) const
{
	const LCDbAnalysers & machines = LCDbAnalysers::records();
	if( buddyID == 0 )
		buddyID = machines.getCurrentID();

	const MachineDef * found = NULL;
	const std::set< int > allIDs = machines.getMachineIDs( buddyID );
	for( Range< MachineDef > mi = getCodes(); mi.isValid(); ++ mi )
	{
		if( allIDs.count( mi -> getMachineID() ) != 0 )
		{
			// give preference to local definition against clusters'
			if( found == NULL || (mi -> getMachineID() == buddyID
							&& found -> getMachineID() != buddyID) )
				found = &(*mi);

			// but don't return anything if the definition's ambiguous
			else if( found -> getProtocol() != mi -> getProtocol()
				  || found -> getTestOrder() != mi -> getTestOrder()
				  || found -> getCode() != mi -> getCode() )
			{
				return NULL;
			}
		}
	}
	return found;
}

//---------------------------------------------------------------------------
// 	Find if this test (or its components) can run on a given analyser
//---------------------------------------------------------------------------

bool LCDbTest::isConfigured( int buddyID ) const
{
	if( calculations.empty() )
		return findMachineDef( buddyID ) != NULL;

	const LCDbTests & tests = LCDbTests::records();
	for( Range< CalcDef > defs = calculations; defs.isValid(); ++ defs )
	{
		int sourceID = defs -> getSourceTestID();
		if( sourceID != 0 ) {
			const LCDbTest * part = tests.findByID( buddyID );
			if( part == NULL || part -> findMachineDef( buddyID ) == NULL )
				return false;
		}
	}
	return true;
}

//---------------------------------------------------------------------------
// 	Find common test name for given protocol on the given analysers
//---------------------------------------------------------------------------

String LCDbTest::findTestCode( const std::set< int > & machines, short protocol ) const
{
	std::set< String > codes;
	for( Range< LCDbTest::MachineDef > mi = getCodes(); mi.isValid(); ++ mi )
		if( mi -> getProtocol() == protocol && machines.count( mi -> getMachineID() ) != 0 )
			codes.insert( mi -> getCode() );

	if( codes.size() == 1 )
		return *(codes.begin());
	else
		return "";
}

//---------------------------------------------------------------------------
//	Find test from the given test machine name and (optional) fluid code
//---------------------------------------------------------------------------

int LCDbTests::findTestID( int buddyID, const String & testCode, const String & fluidCode ) const
{
	const LCDbAnalysers & machines = LCDbAnalysers::records();
	const std::set< int > allIDs = machines.getMachineIDs( buddyID );
	std::set< int > tests;
	for( const_iterator ti = begin(); ti != end(); ++ ti ) {
		for( Range< LCDbTest::MachineDef > mi = ti -> getCodes(); mi.isValid(); ++ mi )	{
			if( mi -> getCode() == testCode && allIDs.count( mi -> getMachineID() ) != 0 ) {
				if( fluidCode.IsEmpty() || mi -> getSampleType().IsEmpty()
				 || fluidCode == mi -> getSampleType() ) {
					tests.insert( ti -> getID() );
				}
			}
		}
	}
	return tests.size() == 1 ? *(tests.begin()) : 0;
}

//---------------------------------------------------------------------------
// 	Check the given tests all use the same sample type; return it
//---------------------------------------------------------------------------

String LCDbTests::findSampleType( int buddyID, std::set< int > testIDs ) const
{
	std::set< String > fluids;
	while( !testIDs.empty() ) {
		std::set< int >::iterator next = testIDs.begin();
		const LCDbTest * test = findByID( *next );
		testIDs.erase( next );
		if( test == NULL ) {
			return "";				// invalid/obsolete test?
		}
		const LCDbTest::MachineDef * md = test -> findMachineDef( buddyID );
		if( md != NULL ) {
			fluids.insert( md -> getSampleType() );
		} else {
			Range< LCDbTest::CalcDef > rcd = test -> getCalcParts();
			if( !rcd.isValid() ) {
				return "";			// test not configured?
			}
			do { // track back to source test if calculated
				testIDs.insert( rcd -> getSourceTestID() );
			} while( (++ rcd).isValid() );
		}
	}
	if( fluids.size() == 1 )
		return *(fluids.begin());
	else
		return "";					// category not selected?
}

//---------------------------------------------------------------------------

