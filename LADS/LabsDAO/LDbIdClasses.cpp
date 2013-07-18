/*---------------------------------------------------------------------------
 *
 *	Allocate unique IDs from a central pool
 *
 *		16 Sept 2008, NG:	incorporate extra field for [laptop] central limit
 *      17 Sept 2008, NG:	refactored & split IDs required from block size
 *      4 October 08, NG:	refactored again; ask before throwing exception
 *      17 July 2013, NG:	use (2.7) sequences in place of next_id tables
 *
 *--------------------------------------------------------------------------*/

#include <vcl.h>
#include "LDbIdClasses.h"
#include "LQuery.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

// int LXDbID::maxID = 0;
int LDbNextID::dbChanges = 0;

void LDbIdBase::setID( int key )
{
	if( key == 0 )
		saved = false;
	else if( saved )
		throw Exception( "Cannot change record ID" );
	id = key;
}

//---------------------------------------------------------------------------
//	prepare to claim a block of IDs; initialise the back-off algorithm once
//---------------------------------------------------------------------------

LDbNextID::LDbNextID() : nextId( 0 ), remaining( 0 ), changeStamp( -1 )
{
	static bool randomized = false;
	if( !randomized )
	{
		Randomize();
		randomized = true;
	}
}

//---------------------------------------------------------------------------

void LDbNextID::clearAll()
{
	dbChanges ++;
}

//---------------------------------------------------------------------------

void LDbNextID::setBlock( int first, int available )
{
	nextId = first;
	remaining = available;
	changeStamp = dbChanges;
}

//---------------------------------------------------------------------------
//	return the next free ID if there are the required number available
//---------------------------------------------------------------------------

int LDbNextID::getBlock( int required )
{
	if( nextId == 0 || remaining < required || changeStamp != dbChanges )
		return 0;

	int result = nextId;
	if( result > 0 )
		nextId += required;
	else
		nextId -= required;
	remaining -= required;
	return result;
}

//---------------------------------------------------------------------------
//	claim block of IDs using given reader; ask user about any problems
//---------------------------------------------------------------------------

int LDbAlloc::claimNextID( LQuery & query, int required )
{
	for( ;; )
	{
		try
		{	int next = getCache().getBlock( required );
			if( next != 0 )
			{
				setID( next );
				return next;
			}
			waitForIDs( query, required );
		}
		catch( Exception & nide )
		{
			String message = nide.Message + "\n\nTry again?\n";
			if( Application -> MessageBox( message.c_str(),
					L"Next ID Error", MB_ICONQUESTION | MB_YESNO ) == IDNO )
				throw Exception( "Cannot allocate unique ID" );
		}
	}
}

//---------------------------------------------------------------------------
//	Allocate unique IDs from database; throw Exception of unavailable
//---------------------------------------------------------------------------

void LDbAlloc::waitForIDs( LQuery & query, int required )
{
	int block = 1;
	for( int attempt = 1; attempt < 5; attempt ++ )
	{
		int claimed = readIDs( query );
		if( claimed == 0 )
			Sleep( 500 + random( 1000 * attempt ) );
		else
		{	getCache().setBlock( claimed, block );
			return;
		}
	}
	throw Exception( "problem allocating next ID" );
}

//---------------------------------------------------------------------------
//	Get next ID from the current project database; increasingly negative
//	(this replaces getNextID from next_id table; increasingly positive)
//---------------------------------------------------------------------------

int LPDbID::readIDs( LQuery & query )
{
	query.setSQL( "select next value for id_sequence" );
	return query.open() ? query.readInt( 0 ) : 0;
}

//---------------------------------------------------------------------------
//	Get next [buddy] ID from the central database; increasingly positive
//	(this replaces getNextID from buddy_next_id table on buddy/cluster)
//---------------------------------------------------------------------------

int LBDbID::readIDs( LQuery & query )
{
	query.setSQL( "select next value for c_id_sequence" );
	return query.open() ? query.readInt( 0 ) : 0;
}

//---------------------------------------------------------------------------
//	Get next [central] ID from the central database; increasingly positive
//	(this replaces getNextID from c_next_cid table; increasingly negative)
//---------------------------------------------------------------------------

int LCDbID::readIDs( LQuery & query )
{
	query.setSQL( "select next value for c_id_sequence" );
	return query.open() ? query.readInt( 0 ) : 0;
}

//---------------------------------------------------------------------------
//	Mintain supply of allocated IDs for central, project and cluster
//---------------------------------------------------------------------------

LDbNextID & LCDbID::getCache() const
{
	static LDbNextID centralIDs;
	return centralIDs;
}

//---------------------------------------------------------------------------

LDbNextID & LPDbID::getCache() const
{
	static LDbNextID projectIDs;
	return projectIDs;
}

//---------------------------------------------------------------------------

LDbNextID & LBDbID::getCache() const
{
	static LDbNextID clusterIDs;
	return clusterIDs;
}

//---------------------------------------------------------------------------

