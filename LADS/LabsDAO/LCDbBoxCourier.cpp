//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LCDbBoxCourier.h"
#include "LCDbObject.h"
#include "LCDbOperator.h"
#include "LCDbAuditTrail.h"
#include "LCDbProject.h"

const String LSDbBoxCourier::ALLOCATED_BOX="AllocatedBox";
const String LSDbBoxCourier::CONFIRMED_ALLOCATION="ConfirmedAlloctn";
const String LSDbBoxCourier::REFERRED_ALLOCATION="ReferredAlloctn";

#pragma package(smart_init)

void ColourStatus::getColours( ArrivalStatus pMs,String & pTextStatus, TColor& pC, TColor& pTc )
{
	switch(pMs )
	{
		case MATCHED:
		{
			pTextStatus="Matched";
			pC=clGreen;
			pTc=clWhite ;
		}
		break;
		case MATCH_REFERRED:
		{
			pTextStatus="Referred";
			pC=clBlue;
			pTc=clWhite;
		}
		break;
		case MATCH_DUPLICATE:
		{
			pTextStatus="Duplicate";
			pC=clRed;
			pTc=clWhite;
		}
		break;
		case UNEXPECTED:
		{
			pTextStatus="Unexpected";
			pC=clGray;
			pTc=clBlack;
		}
		break;
		case UNKNOWN:
		default:
		{
			pTextStatus="Unknown";
			pC=clWhite;
			pTc=clBlack;
		}
	}
}

LSDbBoxCourier::LSDbBoxCourier( const LQuery & query )
		: LSDbBox( query.readString( "box_name" )
				 , (short)query.readInt( "first_position")
				 , (short)query.readInt( "last_position"))
		, first_barcode( query.readString("first_barcode"))
		, last_barcode ( query.readString("last_barcode" ))
		, project_cid ( query.readInt("project_cid" ))
		, used( false )

{
}

LDbBoxExpected::LDbBoxExpected( const LQuery & query )
		: LPDbID( query.readInt( "box_expected_id" ) )
		, LSDbBoxCourier(query)
{
}

// read from c_box_expected in laptopCentral

void LDbBoxExpecteds::readAll( LQuery )
{
	LCDbProjects & pList = LCDbProjects::records();
	for( Range< LCDbProject > p = pList; p.isValid(); ++ p ) {
		if( p->isInCurrentSystem() ) {
			records( p->getID() );
		}
	}
}

void LDbBoxExpecteds::read( LQuery, bool )
{
	// l_box_expected is no longer used
}


// void LLDbBoxExpecteds::updateRecords( LQuery localDb )
// {
//         removeAllLocal( localDb);
// run through each of the records in the laptop database
//	for( Range< LLDbBoxExpected > pr = LLDbBoxExpecteds::records(); pr.isValid(); ++ pr )
//	{
// Find a corresponding record in the BoxRec database
//              const LRDbBoxExpected* bxp = LRDbBoxExpecteds::records().findRecord(pr->getExternalName());
//              if ( bxp != NULL )
//              {
// Record exists in the laptop data base
//      Check the status on the record and update as necessary.
//                        if (pr->getStatus() > 6 ) // 7 co-responds to Received or greater
//                        {
//                                bxp->setStatus(99);
//								bxp->saveRecord( localDb );
//						}
//              }else
//             {
// Record doesn't exist on the laptop database
//        Insert a fresh record in to the box rec database.
//					  LRDbBoxExpected bxexp(&(*pr));
//					  bxexp.saveRecord(localDb);
//              }
//		}
// }


void LDbBoxArrival::createId( LQuery project )
{
	if( getID() != 0 )
	{
		throw Exception( "Project specimen entry already exists" );
	}
	claimNextID( project );
}

LDbBoxArrival::LDbBoxArrival( const LQuery & query )
		:LSDbBoxCourier( query )
		, LCDbID( query.readInt( "box_arrival_id" ) )
		, laptop_cid (query.readInt("laptop_cid"))
		, process_cid ( query.readInt("process_cid" ))
		, status (query.readInt("status" ))
		, swipe_time( query.readDateTime("swipe_time"))
		, tank_cid( query.readInt("tank_cid"))
		, rack_number (query.readString("rack_number" ))
		, slotPosition( query.readInt("slot_position" ))
{
}


//---------------------------------------------------------------------------
//	Add a link for a calibrant or reagent; update the database
//---------------------------------------------------------------------------

void LDbBoxArrival::addLinkedEvent( LQuery central, const String & eventCID )
{
	/*
		const LCDbObject * eo = LCDbObjects::records().find( eventCID, LCDbObject::SIS_EVENT );

		central.setSQL( "insert into l_box_arrival_event_history"
					   " (box_arrival_id, event_cid, operator_cid,event_date, process_cid, project_cid)"
					   " values ( :bxarr, :evcid,:opid,:evdt,:pcid,:pjid )" );
		central.setParam( "bxarr",getID());
		central.setParam( "evcid", eo->getID() );
		central.setParam( "opid", LCDbOperators::getCurrentID() );
		central.setParam( "evdt", Now() );
		central.setParam( "pcid", LCDbAuditTrail::getProcessID());
		central.setParam( "pjid", LCDbProjects::getCurrentID());

		central.execSQL();
	*/
}

bool LDbBoxArrival::saveRecord( LQuery pQuery )
{
	setProcessCid( LCDbAuditTrail::getCurrent().getProcessID());
	if( saved )
	{
		// Indexed/find by box_arrival_id
		pQuery.setSQL( "Update l_box_arrival set"
						  " laptop_cid = :lapid"
						  " ,process_cid = :prid"
						  " ,swipe_time = :swtm"
						  " ,box_name = :bn"
						  " ,status = :sta"
						  " ,first_position = :fpos"
						  " ,first_barcode = :fbc"
						  " ,last_position = :lpos"
						  " ,last_barcode = :lbc"
						  " ,tank_cid = :tcid"
						  " ,rack_number = :rknm"
						  " ,slot_position = :slpos"
						  " ,time_stamp = 'now'"
					" where box_arrival_id = :bxid"
						" and project_cid = :pcid" );
	}
	else
	{	pQuery.setSQL( "INSERT into l_box_arrival"
						  " (laptop_cid"
						  " ,process_cid"
						  " ,box_arrival_id"
						  " ,project_cid"
						  " ,swipe_time"
						  " ,box_name"
						  " ,status"
						  " ,first_position"
						  " ,last_position"
						  " ,first_barcode"
						  " ,last_barcode"
						  " ,tank_cid"
						  " ,rack_number"
						  " ,slot_position"
						  " ,time_stamp)"
			  " values ( :lapid "
						  " ,:prid"
						  " ,:bxid"
						  " ,:pcid"
						  " ,:swtm"
						  " ,:bn"
						  " ,:sta"
						  " ,:fpos"
						  " ,:lpos"
						  " ,:fbc"
						  " ,:lbc"
						  " ,:tcid"
						  " ,:rknm"
						  " ,:slpos"
						  ", 'now' )" );
	}

	pQuery.setParam( "lapid", getLaptopCid());
	pQuery.setParam( "prid", getProcessCid());
	pQuery.setParam( "bxid", getID() );
	pQuery.setParam( "pcid", getProjectCid() );
	pQuery.setParam( "swtm", getSwipeTime() );
	pQuery.setParam( "bn", boxName );
	pQuery.setParam( "sta", getStatus() );
	pQuery.setParam( "fbc", getFirstBarcode());
	pQuery.setParam( "fpos", getFirst());
	pQuery.setParam( "lbc", getLastBarcode() );
	pQuery.setParam( "lpos", getLast() );
	pQuery.setParam( "tcid", getTankCid());
	pQuery.setParam( "rknm", getRackNumber());
	pQuery.setParam( "slpos", getSlotPosition());
	if( pQuery.execSQL() )
	{
		saved = true;
		LDbBoxArrivals::records().insert( *this );
		return true;
	}
	else return false;
}


//---------------------------------------------------------------------------
// Matcher is used to set up parameters for a predicate in a find algorithm
//---------------------------------------------------------------------------

class LDbBoxExpecteds::Matcher : public std::unary_function< LDbBoxExpected, bool >
{
	const std::string boxName,cryo1,cryo2;
	const int cryo1Pos,cryo2Pos,projCid;

public:
	operator std::string() const { return boxName; }
	Matcher( const int pProjectCid
			,const std::string & pBoxName
			,const std::string & pCryo1
			,const std::string & pCryo2
			,const int pCryoPos1
			,const int pCryoPos2 )
	 : projCid( pProjectCid ), boxName( pBoxName ), cryo1( pCryo1 ), cryo2 ( pCryo2 )
	 , cryo1Pos( pCryoPos1), cryo2Pos( pCryoPos2 )
	{}


	bool operator()( const LDbBoxExpected & other ) const
	{
				bool t0,t1,t2,t3,t4,t5;
		t0=( other.getProjectCid() == projCid || projCid==-1 );
		t1=( boxName.compare( other.getBoxName() ) == 0 || boxName.empty() );
// if Cryovial barcodes in l_box_expected are null or '.' ignore this as a way of matching
		t2=( cryo1.compare(other.getFirstBarcode())==0  || cryo1.empty() || other.getFirstBarcode().length() < 2 );
		t3=( cryo2.compare(other.getLastBarcode())  == 0 || cryo2.empty() || other.getLastBarcode().length()  < 2 );
		t4=( other.getFirst() == cryo1Pos ||cryo1Pos ==-1 );
		t5=( other.getLast() == cryo2Pos ||cryo2Pos ==-1 );
		return (t0 && t1 && t2 && t3 && t4 && t5);
	}
};

//---------------------------------------------------------------------------

const LDbBoxExpected * LDbBoxExpecteds::find( const int pProjectCid, const std::string & pBoxName
			,const std::string & pCryo1
			,const std::string & pCryo2
			,const int pCryoPos1
			,const int pCryoPos2  ) const
{
	return findMatch( Matcher( pProjectCid, pBoxName, pCryo1, pCryo2, pCryoPos1, pCryoPos2 )  );
}

//---------------------------------------------------------------------------

void LDbBoxArrivals::readAll( LQuery central )
{
	LCDbProjects & pList = LCDbProjects::records();
	for( Range< LCDbProject > p = pList; p.isValid(); ++ p ) {
		if( p->isInCurrentSystem() ) {
			records( p->getID() );
		}
	}
}

void LDbBoxArrivals::read( LQuery pQuery, bool all )
{
	// box arrivals are now stored centrally
	LQuery central( LIMSDatabase::getCentralDb() );
	if( all ) {
		central.setSQL( "select * from l_box_arrival"
						" where project_cid = :pid"
						" order by box_arrival_id" );
	} else {
		central.setSQL( "select * from l_box_arrival"
						" where status <> :sta and project_cid = :pid"
						" order by box_arrival_id" );
		central.setParam( "sta" , LDbValid::DELETED );
	}

	const std::string & projDb = pQuery.getDatabase().getDbName();
	const LCDbProject * proj = LCDbProjects::records().findByName( projDb );
	if( proj == NULL ) {
		central.setParam( "pid", LCDbProjects::getCurrentID() );
	} else {
		central.setParam( "pid", proj->getID() );
	}
	readData( central );
}

//---------------------------------------------------------------------------
//	Find a specimen entry from the cache with the given external_name
//---------------------------------------------------------------------------

class LDbBoxArrivals::Matcher : public std::unary_function< LDbBoxArrival, bool >
{
	const std::string boxName,cryo1,cryo2;
	const int cryo1Pos,cryo2Pos,projCid;

public:
	operator std::string() const { return boxName; }
	Matcher( const int pProjectCid
			,const std::string & pBoxName
			,const std::string & pCryo1
			,const std::string & pCryo2
			,const int pCryoPos1
			,const int pCryoPos2 )
	 : projCid( pProjectCid ), boxName( pBoxName ), cryo1( pCryo1 ), cryo2 ( pCryo2 )
	 , cryo1Pos( pCryoPos1), cryo2Pos( pCryoPos2 )
	{}


	bool operator()( const LDbBoxArrival & other ) const
	{
				bool t0,t1,t2,t3,t4,t5;
		t0=( other.getProjectCid() == projCid || projCid==-1 );
		t1=( boxName.compare(other.getBoxName()) == 0 || boxName.empty() );
// if Cryovial barcodes in l_box_expected are null or '.' ignore this as a way of matching
		t2=( cryo1.compare(other.getFirstBarcode()) == 0 || cryo1.empty() || other.getFirstBarcode().length() < 2 );
		t3=( cryo2.compare(other.getLastBarcode())  == 0 || cryo2.empty() || other.getLastBarcode().length()  < 2 );
		t4=( other.getFirst() == cryo1Pos ||cryo1Pos ==-1 );
		t5=( other.getLast() == cryo2Pos ||cryo2Pos ==-1 );
		return (t0 && t1 && t2 && t3 && t4 && t5);
	}
};

//---------------------------------------------------------------------------

const LDbBoxArrival * LDbBoxArrivals::find( const int pProjectCid, const std::string & pBoxName
			,const std::string & pCryo1
			,const std::string & pCryo2
			,const int pCryoPos1
			,const int pCryoPos2  ) const
{
	return findMatch( Matcher( pProjectCid, pBoxName, pCryo1, pCryo2, pCryoPos1, pCryoPos2 )  );
}

//---------------------------------------------------------------------------

/*	c_box_arrival no longer used in db 2.5, NG: 13/11/09

bool LCDbBoxAllocs::readCurrent( LQuery pQuery )
{
	pQuery.setSQL( "select * from c_box_arrival" );
	readData( pQuery );
	return !cache.empty();
}
class LCDbBoxAllocs::Matcher : public std::unary_function< LCDbBoxArrival, bool >
{
	const String boxName,cryo1,cryo2;
	const int cryo1Pos,cryo2Pos,projCid;

public:
	operator String() const { return boxName; }
	Matcher( const int pProjectCid
			,const String & pBoxName
			,const String & pCryo1
			,const String & pCryo2
			,const int pCryoPos1
			,const int pCryoPos2 )
	 : projCid( pProjectCid ), boxName( pBoxName ), cryo1( pCryo1 ), cryo2 ( pCryo2 )
	 , cryo1Pos( pCryoPos1), cryo2Pos( pCryoPos2 )
	{}


	bool operator()( const LCDbBoxArrival & other ) const
	{
				bool t0,t1,t2,t3,t4,t5;
		t0=( other.getProjectCid() == projCid || projCid==-1 );
		t1=( boxName.AnsiCompareIC(other.getBoxName()) ==0  || boxName.IsEmpty() );
// if Cryovial barcodes in l_box_expected are null or '.' ignore this as a way of matching
		t2=( cryo1.AnsiCompareIC(other.getFirstBarcode()) == 0 || cryo1.IsEmpty() || other.getFirstBarcode().Length() < 2 );
		t3=( cryo2.AnsiCompareIC(other.getLastBarcode() ) == 0 || cryo2.IsEmpty() || other.getLastBarcode().Length()  < 2 );
		t4=( other.getFirst() == cryo1Pos ||cryo1Pos ==-1 );
		t5=( other.getLast() == cryo2Pos ||cryo2Pos ==-1 );
		return (t0 && t1 && t2 && t3 && t4 && t5);
	}
};

const LCDbBoxArrival * LCDbBoxAllocs::find( const int pProjectCid,	const String & pBoxName
			,const String & pCryo1
			,const String & pCryo2
			,const int pCryoPos1
			,const int pCryoPos2 ) const
{
	return findMatch( 	Matcher( pProjectCid, pBoxName, pCryo1, pCryo2, pCryoPos1, pCryoPos2 )  );
}

//---------------------------------------------------------------------------
//	read the box arrival history for a particular box : NEEDS MORE WORK!
//---------------------------------------------------------------------------

LDbBoxArrivalEvent::LDbBoxArrivalEvent( const LQuery & query )
 : LPDbBoxEvent( query )
{
	boxID = query.readInt( "box_arrival_id" );
	projectCid = query.readInt( "project_cid" );
}

//---------------------------------------------------------------------------

Range< LDbBoxArrivalEvent > LDbBoxArrival::readBoxHistory( LQuery )
{
	static std::vector< LDbBoxArrivalEvent > recent;
	recent.clear();
	lQuery.setSQL( "select * from l_box_arrival_event_history"
			" where project_cid = :pid and box_arrival_id = :bid" );
	lQuery.setParam( "pid", LCDbProjects::getCurrentID() );
	lQuery.setParam( "bid", getID() );
	for( lQuery.open(); !lQuery.eof(); lQuery.next() )
		recent.push_back( LDbBoxArrivalEvent( lQuery ) );
	return recent;
}

//---------------------------------------------------------------------------

bool LPDbBoxEvent::saveRecord( LQuery query )
{
	query.setSQL( "insert into box_event_history"
				" (box_id, event_cid, operator_cid, event_date, process_cid, comment)"
				" values ( :bid, :eid, :oid, :edt, :pid, :txt )" );
	query.setParam( "bid", boxID );
	query.setParam( "eid", getEventCID() );
	query.setParam( "oid", getOperatorCID() );
	query.setParam( "edt", getEventDate() );
	query.setParam( "pid", getProcessCID() );
	query.setParam( "txt", text );
	return query.execSQL() == 1;
}

//---------------------------------------------------------------------------
*/

