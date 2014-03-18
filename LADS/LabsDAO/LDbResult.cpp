/*---------------------------------------------------------------------------
 *
 *	Version history:
 *
 *	   	31 May 2005		Use LDbStage for action_flag; convert to/from character
 *      14 June, NG		Split out worklist handling into LIMSResult
 *      5 July, NG		Added LPDbResult::addAuditEntry() to record sender
 *      10 November		Inherit note count from from LDbHasNote
 *      23 Oct 08, NG:	Use string for result where possible
 *		11/03/09, NG:	Use ResultValue as a base class for all results
 *      16 June 09:		Added LPDbInternal to store results in lab_result
 *      26/08/09, NG:	Added transfer summary for NightShift reporting
 *		13/10/10, NG:	Ignore orphaned entries deleting from buddy_result_float
 *      7/12/10, NG:	Added support for result_units in project databases
 *--------------------------------------------------------------------------*/

#include <vcl.h>
#include "LCDbProject.h"
#include "LDbResult.h"
#include "LCDbAnalyser.h"
#include "LDbWorkList.h"
#include "LQuery.h"
#include "LDbSource.h"
#include "LCDbAuditTrail.h"
#include "LDbNotes.h"
#include "LCDbTest.h"

#pragma hdrstop

#pragma package(smart_init)

//---------------------------------------------------------------------------

LDbResult::LDbResult( int test, const std::string & result, TDateTime when, int machine,
					int process, bool hide, short notes, int person1, int person2 )
 : ResultValue( result ), LDbStage( NEW_ENTRY ), LDbNoteCount( notes ), analysed( when ), testID( test ),
   machineID( machine ), processID( process ), valop1id( person1 ), valop2id( person2 ), internal( hide )
{}

//---------------------------------------------------------------------------
//	buddy_result_float constructors - from analyser output or database
//---------------------------------------------------------------------------

LBDbResult::LBDbResult( int testID, const std::string & result, TDateTime analysed, int analysisID )
 : LDbResult( testID, result, analysed, 0, 0, false, 0, 0, 0 ), bsid( analysisID ), modified( false )
{}

//---------------------------------------------------------------------------

LBDbResult::LBDbResult( const LQuery & cluster )
 : LBDbID( cluster.readInt( "buddy_result_id" ) ),
   LDbResult( cluster.readInt( "test_id" ),
			  cluster.readString( "res_text" ),
			  cluster.readDateTime( "date_analysed" ),
			  cluster.readInt( "machine_cid" ),
			  cluster.readInt( "process_id" ),
			  cluster.readInt( "is_private" ) != 0,
			  cluster.readInt( "note_exists" ),
			  cluster.readInt( "person1_validate" ),
			  cluster.readInt( "person2_validate" ) ),
   bsid( cluster.readInt( "buddy_sample_id" ) ),
   modified( cluster.readInt( "result_edited" ) != 0 )
{
	setStage( makeStage( cluster.readString( "action_flag" )[0] ) );
   	setUnitCode( cluster.readString( "unit_code" ) );

	// Validation still uses res_value rather than res_text
	if( modified || !isValid() )
		setValue( cluster.readDouble( "res_value" ) );
}

//---------------------------------------------------------------------------
//  Copy recent results from the cluster database into memory
//---------------------------------------------------------------------------

bool LBDbResults::read( LQuery buddyQuery, bool readAll )
{
	if( readAll )
		buddyQuery.setSQL( "select * from Buddy_Result_Float order by buddy_result_id" );
	else
	{	buddyQuery.setSQL( "select * from Buddy_Result_Float"
						  " where action_flag <> :tra and machine_cid = :aid"
						  " order by buddy_result_id" );
		buddyQuery.setParam( "aid", LCDbAnalysers::getCurrentID() );
		buddyQuery.setParam( "tra", LBDbResult::action( LDbStage::TRANSMITTED ) );
	}
	return readData( buddyQuery );
}

//---------------------------------------------------------------------------
//  Create a new buddy_result_float record for this result
//---------------------------------------------------------------------------

bool LBDbResult::createRecord( LQuery cluster )
{
	if( getID() == 0 )
		claimNextID( cluster );

	if( getMachineID() == 0 )
		setMachineID( LCDbAnalysers::getCurrentID() );

	if( getProcessID() == 0 )
		processID = LCDbAuditTrail::getCurrent().getProcessID();

	cluster.setSQL( "INSERT into buddy_result_float (Buddy_Result_ID, Buddy_Sample_ID,"
					"  machine_cid, Test_id, Res_value, Action_Flag, person1_validate,"
					"  person2_validate, result_edited, Date_analysed, Note_exists,"
					"  delete_status, update_when, process_id, is_private, Res_text, unit_code)"
					" values ( :brid, :bsid, :mid, :tid, :rval, :flg, :op1, :op2, :red, "
					"  :dta, :nex, 0, :udw, :pid, :prv, :rtxt, :uno )" );
	cluster.setParam( "brid", getID() );
	cluster.setParam( "bsid", bsid );
	cluster.setParam( "mid", getMachineID() );
	cluster.setParam( "tid", getTestID() );
	cluster.setParam( "rval", asDouble() );
	cluster.setParam( "udw", XTIME() );
	cluster.setParam( "flg", action( getStage() ) );
	cluster.setParam( "op1", getFirstValOpID() );
	cluster.setParam( "op2", getSecondValOpID() );
	cluster.setParam( "red", modified ? 1 : 0 );
	cluster.setParam( "dta", getTimeStamp() );
	cluster.setParam( "nex", notes );
	cluster.setParam( "pid", getProcessID() );
	cluster.setParam( "rtxt", asString() );
	cluster.setParam( "prv", isPrivate() ? 1 : 0 );
	cluster.setParam( "uno", getUnitCode() );
	if( cluster.execSQL() != 1 )
		return false;
	else
	{	saved = true;
//??		LBDbResults::records().insert( *this );
		return true;
    }
}

//---------------------------------------------------------------------------
//  Update the buddy_result_float record for this result
//---------------------------------------------------------------------------

bool LBDbResult::updateRecord( LQuery cluster )
{
	cluster.setSQL( "UPDATE buddy_result_float"
				" set process_id = :pid, note_exists = note_exists + :nex,"
				" action_flag = :flg, person1_validate = :op1, person2_validate = :op2,"
				" result_edited = :red, delete_status = :dsf, update_when = :udw"
				" where buddy_result_id = :brid" );
	cluster.setParam( "pid", getProcessID() );
	cluster.setParam( "nex", notes );
	cluster.setParam( "flg", action( getStage() ) );
	cluster.setParam( "udw", XTIME() );
	cluster.setParam( "op1", getFirstValOpID() );
	cluster.setParam( "op2", getSecondValOpID() );
	cluster.setParam( "red", modified ? 1 : 0 );
	cluster.setParam( "dsf", getStage() >= TRANSMITTED ? 2 : 1 );
	cluster.setParam( "brid", getID() );
	if( cluster.execSQL() != 1 )
		return false;

	saved = true;
//??	LBDbResults::records().insert( *this );
	return true;
}

//---------------------------------------------------------------------------
//	Add result into current project's result and multi-result-float tables
//---------------------------------------------------------------------------

bool LPDbResult::createRecord( LQuery project )
{
	short flags = 0;
	if( getStage() == LDbStage::DELETED )
		flags = DELETED;
	else
	{	if( !isValid() )
			flags |= INVALID;
		if( isPrivate() )
			flags |= PRIVATE;
	}

	claimNextID( project );
	project.setSQL( "Insert into Result (result_ID, sample_id, test_cid, machine_cid,"
					" time_stamp, status, note_exists, process_cid, string_value,"
					" reagent_cid, Person1_validate, Person2_validate, result_units)"
				" values ( :rid, :sid, :tid, :mid, :tsm, :sts, :nte, :pid, :val, :rea, :op1, :op2, :ru )" );
	project.setParam( "rid", getID() );
	project.setParam( "sid", sampleID  );
	project.setParam( "tid", getTestID() );
	project.setParam( "mid", getMachineID() );
	project.setParam( "tsm", getTimeStamp() );
	project.setParam( "sts", flags );
	project.setParam( "nte", notes );
	project.setParam( "val", asString() );
	project.setParam( "pid", getProcessID() );
	project.setParam( "op1", getFirstValOpID() );
	project.setParam( "op2", getSecondValOpID() );
	project.setParam( "ru", getUnitCode() );
	project.setParam( "rea", reagent );
	if( project.execSQL() != 1 )
		return false;

	if( isValid() )
	{
		project.setSQL( "Insert into multi_result_real"
						" (result_id, seq, result_value) values ( :rid, 1, :val )" );
		project.setParam( "rid", getID() );
		project.setParam( "val", asDouble() );
		if( project.execSQL() != 1 )
			return false;
	}

	saved = true;
//??	LPDbResults::records().insert( *this );
	return true;
}

//---------------------------------------------------------------------------
//  Read results for the given sample back into memory
//---------------------------------------------------------------------------

LPDbResult::LPDbResult( const LQuery & pQuery )
 : LPDbID( pQuery.readInt( "result_id" ) ),
   LDbResult( pQuery.readInt( "test_cid" ),
			  pQuery.readString( "string_value" ),
			  pQuery.readDateTime( "time_stamp" ),
			  pQuery.readInt( "machine_cid" ),
			  pQuery.readInt( "process_cid" ),
			  pQuery.readInt( "status" ) == 2,
			  pQuery.readInt( "note_exists" ),
			  pQuery.readInt( "person1_validate" ),
			  pQuery.readInt( "person2_validate" ) ),
   sampleID( pQuery.readInt( "sample_id" ) ),
   reagent( pQuery.readInt( "reagent_cid" ) )
{
	if( pQuery.fieldExists( "result_units" ) )
		setUnitCode( pQuery.readString( "result_units" ) );
}

//---------------------------------------------------------------------------

unsigned LPDbResults::readSample( LQuery pQuery, int sampleID )
{
	pQuery.setSQL( "select * from result where sample_id = :sid"
				  " order by result_id" );
	pQuery.setParam( "sid", sampleID );
	return readData( pQuery );
}

//---------------------------------------------------------------------------

LPDbInternal::LPDbInternal( const LQuery & pQuery )
 : LPDbID( pQuery.readInt( "invalid_result_id" ) ),
   LDbResult( pQuery.readInt( "test_cid" ),
			  pQuery.readString( "string_value" ),
			  pQuery.readDateTime( "time_stamp" ),
			  pQuery.readInt( "machine_cid" ),
			  pQuery.readInt( "process_cid" ),
			  pQuery.readInt( "status" ) == 2,
			  pQuery.readInt( "note_exists" ),
			  pQuery.readInt( "person1_validate" ),
			  pQuery.readInt( "person2_validate" ) ),
   sampleID( pQuery.readInt( "sample_id" ) ),
   reagent( pQuery.readInt( "reagent_cid" ) )
{
	if( pQuery.fieldExists( "result_units" ) )
		setUnitCode( pQuery.readString( "result_units" ) );
}

//---------------------------------------------------------------------------

unsigned LPDbInternals::readSample( LQuery pQuery, int sampleID )
{
	pQuery.setSQL( "select * from invalid_result where sample_id = :sid"
				  " order by invalid_result_id" );
	pQuery.setParam( "sid", sampleID );
	return readData( pQuery );
}

//---------------------------------------------------------------------------
//	Find the processing stage corresponding to the given action flag
//---------------------------------------------------------------------------

LDbStage::Stage LBDbResult::makeStage( char action_flag )
{
	switch( action_flag )
	{
		case '0':
			return LDbStage::ANALYSED;

		case '1':
			return LDbStage::L1_VALIDATED;

		case '2':
			return LDbStage::L2_VALIDATED;

		case '3':
			return LDbStage::TRANSMITTED;

		case '4':
			return LDbStage::INVALIDATED;

		case '5':
			return LDbStage::L1_MARKED;

		case '6':
			return LDbStage::L0_HELD;

		case '8':
			return LDbStage::PROGRAMMED;

		case '9':
			return LDbStage::WIPED;

		default:
			return LDbStage::NEW_ENTRY;
	}
}

//---------------------------------------------------------------------------
//	Get buddy_result_float/buddy_database action_flag for the given stage
//---------------------------------------------------------------------------

char LBDbResult::action( Stage stage )
{
	switch( stage )
	{
		case LDbStage::ANALYSED:
			return '0';

		case LDbStage::L1_VALIDATED:
			return '1';

		case LDbStage::L2_VALIDATED:
			return '2';

		case LDbStage::COLLECTED:
		case LDbStage::TRANSMITTED:
		case LDbStage::IN_TANK:
		case LDbStage::DELETED:
			return '3';

		case LDbStage::INVALIDATED:
			return '4';

		case LDbStage::L1_MARKED:
			return '5';

		case LDbStage::L0_HELD:
			return '6';

		case LDbStage::PROGRAMMED:
			return '8';

		case LDbStage::WIPED:
			return '9';

		default:
			return '7';
	}
}

//---------------------------------------------------------------------------

short LPDbInternal::makeStatus( Stage stage )
{
	return stage == LDbStage::DELETED ? 99 : LBDbResult::action( stage ) - '0';
}

//---------------------------------------------------------------------------
//	Copy result into the buddy_result_audit table after it has been saved
//---------------------------------------------------------------------------

void LPDbResult::addAuditEntry( LQuery cluster, int buddyResultID, const std::string & barcode )
{
	if( getID() == 0 )
		throw Exception( "Result has not yet been saved" );

	cluster.setSQL( "INSERT into buddy_result_audit	(Result_ID, Buddy_Result_ID,"
			" sample_id, test_cid, machine_id, barcode, res_value, send_time, process_cid)"
			" values( :rid, :brid, :sid, :tid, :mid, :bar, :val, :sdt, :pid )" );

	cluster.setParam( "rid", getID() );
	cluster.setParam( "brid", buddyResultID  );
	cluster.setParam( "sid", sampleID  );
	cluster.setParam( "tid", getTestID() );
	cluster.setParam( "mid", getMachineID() );
	cluster.setParam( "bar", barcode );
	cluster.setParam( "val", asDouble() );
	cluster.setParam( "sdt", XTIME() );
	cluster.setParam( "pid", LCDbAuditTrail::getCurrent().getProcessID() );
	cluster.execSQL();
}

//---------------------------------------------------------------------------
//	Add invalidated or not-yet-validated result into labs_result table
//---------------------------------------------------------------------------

bool LPDbInternal::createRecord( LQuery project )
{
	claimNextID( project );
	project.setSQL( "Insert into Invalid_Result (invalid_result_ID, sample_id, test_cid,"
					" machine_cid, time_stamp, status, note_exists, string_value,"
					" process_cid, reagent_cid, Person1_validate, Person2_validate, result_units)"
				" values ( :rid, :sid, :tid, :mid, :tsm, :sts, :nte, :val, :pid, :rae, :op1, :op2, :ru )" );
	project.setParam( "rid", getID() );
	project.setParam( "sid", sampleID  );
	project.setParam( "tid", getTestID() );
	project.setParam( "mid", getMachineID() );
	project.setParam( "tsm", getTimeStamp() );
	project.setParam( "sts", makeStatus( getStage() ) );
	project.setParam( "nte", notes );
	project.setParam( "val", asString() );
	project.setParam( "pid", getProcessID() );
	project.setParam( "rae", reagent );
	project.setParam( "op1", getFirstValOpID() );
	project.setParam( "op2", getSecondValOpID() );
	project.setParam( "ru", getUnitCode() );
	if( project.execSQL() != 1 )
		return false;

	saved = true;
//??	LPDbInternals::records().insert( *this );
	return true;
}

//---------------------------------------------------------------------------
//	delete results at least 20 hours after they all have been transmitted
//---------------------------------------------------------------------------

unsigned LBDbResults::deleteOld( LQuery cluster, TDateTime flagged, TDateTime oldest )
{
	cluster.setSQL( "delete from buddy_result_float"
					" where (delete_status = 2 and update_when < :fdt)"
					" or (update_when < :old and action_flag <> :hld)" );
	cluster.setParam( "old", XTIME(oldest) );
	cluster.setParam( "hld", LBDbSource::action( LDbStage::L0_HELD ) );
	cluster.setParam( "fdt", XTIME(flagged) );
	return cluster.execSQL();
}

//---------------------------------------------------------------------------

