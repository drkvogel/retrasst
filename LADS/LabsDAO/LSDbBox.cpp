//---------------------------------------------------------------------------

#include <vcl.h>
#include "LCDbObject.h"
#include "LCDbOperator.h"
#include "LCDbAuditTrail.h"
#include "LSDbBox.h"
#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)

LSDbBoxEvent::LSDbBoxEvent( const LCDbObject * event )
 : 	eventCID( event -> getID() ),
	operatorCID( LCDbOperators::getCurrentID() ),
	eventDate( Now() ),
	processCID( LCDbAuditTrail::getProcessID() )
{}

//---------------------------------------------------------------------------

LSDbBoxEvent::LSDbBoxEvent( const LQuery & query )
 :	eventCID( query.readInt( "event_cid" )),
	operatorCID (query.readInt( "operator_cid") ),
	processCID (query.readInt( "process_cid") ),
	eventDate (query.readDateTime( "event_date"))
{}

//---------------------------------------------------------------------------


