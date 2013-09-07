/*---------------------------------------------------------------------------
 *
 *	Allocate unique IDs from a central pool
 *
 *		16 Sept 2008, NG:	incorporate extra field for [laptop] central limit
 *      17 Sept 2008, NG:	refactored & split IDs required from block size
 *      4 October 08, NG:	refactored again; ask before throwing exception
 *      17 July 2013, NG:	use (2.7) sequences in place of next_id tables
 *		2 September 2013:	simplified - use sequence cache rather than local
 *
 *--------------------------------------------------------------------------*/

#include <vcl.h>
#include "LQuery.h"
#include "LDbIdClasses.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

void LDbIdBase::setID( int key )
{
	if( key == 0 )
		saved = false;
	else if( saved )
		throw Exception( "Cannot change record ID" );
	id = key;
}

//---------------------------------------------------------------------------
//	claim block of IDs using given reader; ask user about any problems
//---------------------------------------------------------------------------

int LDbIdBase::readID( LQuery & query, std::string sequence )
{
	String message;
	query.setSQL( "select next value for " + sequence );
	for( ;; ) {
		try	{
			for( int attempt = 1; attempt < 5; attempt ++ ) {
				id = query.open() ? query.readInt( 0 ) : 0;
				if( id == 0 ) {
					Sleep( 500 + random( 1000 * attempt ) );
				} else {
					return id;
				}
			}
			message = "Problem allocating next ID";
		}
		catch( Exception & nide ) {
			message = nide.Message;
		}
		if( Application -> MessageBox( (message + "\n\nTry again?\n").c_str(),
					L"Next ID Error", MB_ICONQUESTION | MB_YESNO ) == IDNO ) {
			throw Exception( "Cannot allocate unique ID" );
		}
	}
}

//---------------------------------------------------------------------------
//	Get next ID from the current project database; increasingly negative
//	(this replaces getNextID from next_id table; increasingly positive)
//---------------------------------------------------------------------------

int LPDbID::claimNextID( LQuery & query ) {
	return readID( query, "id_sequence" );
}

//---------------------------------------------------------------------------
//	Get next [buddy] ID from the central database; increasingly positive
//	(this replaces getNextID from buddy_next_id table on buddy/cluster)
//---------------------------------------------------------------------------

int LBDbID::claimNextID( LQuery & query ) {
	return readID( query, "c_id_sequence" );
}

//---------------------------------------------------------------------------
//	Get next [central] ID from the central database; increasingly positive
//	(this replaces getNextID from c_next_cid table; increasingly negative)
//---------------------------------------------------------------------------

int LCDbID::claimNextID( LQuery & query ) {
	return readID( query, "c_id_sequence" );
}

//---------------------------------------------------------------------------

