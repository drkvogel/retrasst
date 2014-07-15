//---------------------------------------------------------------------------

#include <vcl.h>
#include "LCDbCanned.h"
#include "LCDbAuditTrail.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

LCDbCanned::LCDbCanned( int textID  )
 : status( LDbValid::NEW_RECORD ), groupID( textID ), processID( 0 ) {
}

//---------------------------------------------------------------------------

LCDbCanned::LCDbCanned( const LQuery & query  )
 : LCDbID( query.readInt( "record_cid" ) ),
   processID( query.readInt( "process_cid" ) ),
   LDbNames( query.readString( "name" ),
			 query.readString( "value" ) ),
   groupID( query.readInt( "text_cid" ) ) {
	if( query.fieldExists( "status" ) ) {
	   status = query.readInt( "status" );
	} else {
	   status = LDbValid::NEW_RECORD;
	}
}

//---------------------------------------------------------------------------

bool LCDbCanned::saveRecord( LQuery query )
{
	if( saved ) {
		query.setSQL( "update c_canned_text"
					" set name = :nme, value = :full, process_cid = :pcid, text_cid = :tid"
					" where record_cid = :cid" );
	} else {
		claimNextID( query );
		query.setSQL( "INSERT into c_canned_text (record_cid, text_cid, name, value, process_cid)"
					" values ( :cid, :tid, :nme, :full, :pcid )" );
	}
	query.setParam( "cid", getID() );
	query.setParam( "tid", groupID );
	query.setParam( "nme", getName() );
	query.setParam( "full", getDescription() );
	query.setParam( "pcid", LCDbAuditTrail::getCurrent().getProcessID() );
	if( query.execSQL() ) {
		saved = true;
		LCDbCanneds::records().insert( *this );
		return true;
	} else {
		return false;
	}
}

//---------------------------------------------------------------------------

bool LCDbCanneds::read( LQuery cQuery, bool readAll )
{
//	if( readAll )
		cQuery.setSQL( "select * from c_canned_text order by record_cid" );
/*	else
	{	cQuery.setSQL( "select * from c_canned_text where status <> :sts"
					  " order by record_cid" );
		cQuery.setParam( "sts", LDbValid::DELETED );
	}
*/	return readData( cQuery );
}

//---------------------------------------------------------------------------

