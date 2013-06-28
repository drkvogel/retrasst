/*---------------------------------------------------------------------------
 *
 *	Allocate unique IDs from a central pool
 *
 *		16 Sept 2008, NG:	incorporate extra field for [laptop] central limit
 *      17 Sept 2008, NG:	refactored & split IDs required from block size
 *      4 October 08, NG:	refactored again; ask before throwing exception
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
	int block = (required < 4 ? 4 : required);
	for( int attempt = 1; attempt < 5; attempt ++ )
	{
		int claimed = readIDs( query, block );
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
//	Get next ID from the current project database; increasingly positive
//---------------------------------------------------------------------------

int LPDbID::readIDs( LQuery & query, int count )
{
	query.setSQL( "SELECT next_id from next_id" );
	int id = query.open() ? query.readInt( 0 ) : 0;

	//  Increment the ID for the next user to skip allocated block
	query.setSQL( "UPDATE next_id set next_id = next_id + :num" );
	query.setParam( "num", count );
	query.execSQL();

	//	Use the ID(s) unless table has been updated by anyone else
	query.setSQL( "SELECT next_id from next_id" );
	return (query.open() && query.readInt( 0 ) == id + count) ? id : 0;
}

//---------------------------------------------------------------------------
//	Get next ID from the buddy/cluster database; increasingly positive
//---------------------------------------------------------------------------

int LBDbID::readIDs( LQuery & query, int count )
{
	query.setSQL( "SELECT next_id from buddy_next_id" );
	int id = query.open() ? query.readInt( 0 ) : 0;

	//  Increment the ID for the next user to skip allocated block
	query.setSQL( "UPDATE buddy_next_id set next_id = next_id + :num" );
	query.setParam( "num", count );
	query.execSQL();

	//	Use the ID(s) unless table has been updated by anyone else
	query.setSQL( "SELECT next_id from buddy_next_id" );
	return (query.open() && query.readInt( 0 ) == id + count) ? id : 0;
}

//---------------------------------------------------------------------------
//	Get next ID from central database if available; increasingly negative
//---------------------------------------------------------------------------

int LCDbID::readIDs( LQuery & query, int count )
{
	query.setSQL( "SELECT * from c_next_cid" );
	int id = query.open() ? query.readInt( "next_cid" ) : 0;

	std::string check = "UPDATE c_next_cid set next_cid = next_cid - :num";
	if( query.fieldExists( "available_range" ) )
	{
		int range = query.readInt( "available_range" );
		if( range >= count )
		{
			// laptop database - keep track of allocations
			check += ", available_range = available_range - :num";
		}
		else if( range == -1 )
		{
			// vLab database - can allocate IDs indefinitely
		}
		else
		{
			throw Exception( "No more central IDs available" );
		}
	}

	query.setSQL( check );
	query.setParam( "num", count );
	query.execSQL();

	query.setSQL( "SELECT next_cid from c_next_cid" );
	return (query.open() && query.readInt( 0 ) == id - count) ? id : 0;
}

//---------------------------------------------------------------------------
//	Mintain suuply of allocated IDs for central, project and cluster
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

