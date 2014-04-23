//---------------------------------------------------------------------------
//
//  18 July 2005:	inLocalCluster() returns true for current buddy
//  10 March 2006:	Modified cache handling to allow two copies
//	6 Aug 09, NG:	Allow cluster (object name) ID as currentID
//  24 August, NG:	Analyser can belong to more than one cluster
//
//---------------------------------------------------------------------------

#include <vcl.h>
#include "LCDbAnalyser.h"
#include "LQuery.h"
#include "LIMSParams.h"
#include "LCDbObject.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//	Make the named analyser (or cluster) current; set to 0 if not found
//---------------------------------------------------------------------------

void LCDbAnalysers::setCurrent( const std::string & name )
{
	const LCDbObject * op = LCDbObjects::records().find( name, LCDbObject::CLUSTER );
	if( op != NULL ) {
		currentID = op -> getID();
	} else {
		const LCDbAnalyser * ap = findByName( name );
		currentID = (ap == NULL ? 0 : ap -> getID());
	}
}

//---------------------------------------------------------------------------
//	Read machine name and cluster ID from the appropriate table(s)
//---------------------------------------------------------------------------

LCDbAnalyser::LCDbAnalyser( const LQuery & query  )
 : LCDbID( query.readInt( "machine_cid" ) ),
   LDbNames( query.readString( "external_name" ),
			 query.readString( "external_full" ) ),
   LDbValid( query.readDateTime( "valid_from" ),
			 query.readDateTime( "valid_to" ),
			 query.readInt( "status" ) ),
   location( query.readString( "location" ) ),
   use( UNKNOWN )
{
	int cluster = query.readInt( "cluster_cid" );
	if( cluster != 0 )
	   clusterIDs.insert( cluster );
}

//---------------------------------------------------------------------------

void LCDbAnalyser::setStatusFlags()
{
	if( !clusterIDs.empty() || status == IS_BUDDY )
		use = IS_BUDDY;
	else if( status == IS_LAPTOP )
		use = IS_LAPTOP;
}

//---------------------------------------------------------------------------
//	Read the c_machine table into the cache (or just current records)
//---------------------------------------------------------------------------

bool LCDbAnalysers::read( LQuery cQuery, bool readAll )
{
	if( readAll )
		cQuery.setSQL( "select * from c_machine order by machine_cid" );
	else
	{	cQuery.setSQL( "select * from c_machine"
					  " where status <> :sts and valid_from < 'now' "
					  " and (valid_to < valid_from or valid_to > 'now')"
					  " order by machine_cid" );
		cQuery.setParam( "sts", LCDbAnalyser::DELETED );
	}
	if( !readData( cQuery ) )
		return false;

	if( readAll ) {
		cQuery.setSQL( "select * from c_cluster_machine"
					  " order by machine_cid" );
	} else {
		cQuery.setSQL( "select * from c_cluster_machine where status <> :sts"
					  " order by machine_cid" );
		cQuery.setParam( "sts", LCDbAnalyser::DELETED );
	}

	try
	{	Iterator ci = begin();
		for( cQuery.open(); !cQuery.eof(); cQuery.next() )
		{
			int cid = cQuery.readInt( "cluster_cid" );
			int mid = cQuery.readInt( "machine_cid" );
			while( ci != end() && ci -> getID() < mid )
				ci ++;

			if( ci != end() && ci -> getID() == mid )
				ci -> addClusterID( cid );
		}
	}
	catch( ... )	// table doesn't exist yet
	{}

	for( Iterator ci = begin(); ci != end(); ci ++ )
		ci -> setStatusFlags();

	return true;
}

//---------------------------------------------------------------------------
// 	Store this analysers buddy and cluster IDs in the registry
//---------------------------------------------------------------------------

void LCDbAnalyser::makeDefault() const
{
	LIMSParams::instance().setBuddyID( getID() );
}

//---------------------------------------------------------------------------
//	Find if this analyser is in the cluster specified in the registry
//---------------------------------------------------------------------------
/*
bool LCDbAnalyser::inLocalCluster() const
{
	const LIMSParams & params = LIMSParams::instance();
	if( params.getBuddyID() == getID() )
		return true;

	else if( params.getClusterID() == 0 )
		return false;

	return clusterIDs.count( params.getClusterID() ) > 0;
}
*/
//---------------------------------------------------------------------------
//	Create/update database entry for current analyser; link to cluster(s)
//---------------------------------------------------------------------------

bool LCDbAnalyser::saveRecord( LQuery query )
{
	if( saved )
		query.setSQL( "Update c_machine set external_name = :name, external_full = :full,"
					 " valid_from = :from, valid_to = :to, status = :sts, location = :loc,"
					 " cluster_cid = :fcl where machine_cid = :cid" );
	else
	{	claimNextID( query );
		query.setSQL( "Insert into c_machine (machine_cid, external_name,"
					" external_full, valid_from, valid_to, status, location, cluster_cid) "
					" values ( :cid, :name, :full, :from, :to, :sts, :loc, :fcl )" );
	}

	std::pair< XTIME, XTIME > dates = getValidDates();
	status = !isActive() ? DELETED : isBuddyPC() ? IS_BUDDY : isLaptop() ? IS_LAPTOP : 0;

	query.setParam( "cid", getID() );
	query.setParam( "name", getName() );
	query.setParam( "full", getDescription() );
	query.setParam( "loc", location );
	query.setParam( "from", dates.first );
	query.setParam( "to", dates.second );
	query.setParam( "sts", status );
	std::set< int >::const_iterator ci = clusterIDs.begin();
	query.setParam( "fcl", clusterIDs.empty() ? 0 : *ci );
	if( !query.execSQL() )
		return false;

	query.setSQL( "Delete from c_cluster_machine where machine_cid = :cid" );
	query.setParam( "cid", getID() );
	query.execSQL();

	if( !clusterIDs.empty() )
	{
		LCDbID myLCDbID;
		int record = myLCDbID.claimNextID( query );
		query.setSQL( "Insert into c_cluster_machine"
					" (record_cid, machine_cid, cluster_cid, status)"
					 " values ( :rid, :mid, :cid, 0 )" );
		do
		{	query.setParam( "cid", *ci );
			query.setParam( "mid", getID() );
			query.setParam( "rid", record -- );
			if( !query.execSQL() )
				return false;
		} while( ++ ci != clusterIDs.end() );
	}

	saved = true;
	LCDbAnalysers::records().insert( *this );
	return true;
}

//---------------------------------------------------------------------------
//	list all IDs associated with the given (or current) analyser
//---------------------------------------------------------------------------

std::set< int > LCDbAnalysers::getMachineIDs( int machineID ) const
{
	if( machineID == 0 )
		machineID = getCurrentID();

	std::set< int > buddyIDs;
	const LCDbAnalyser * rp = findByID( machineID );
	if( rp != NULL )
	{
		buddyIDs = rp -> getClusterIDs();
		buddyIDs.insert( machineID );
	}
	return buddyIDs;
}

//---------------------------------------------------------------------------
//	list the analysers in the given (or current) cluster
//---------------------------------------------------------------------------

std::set< int > LCDbAnalysers::getAnalyserIDs( int machineID ) const
{
	if( machineID == 0 )
		machineID = getCurrentID();

	std::set< int > buddyIDs;
	for( ConstIter ar = begin(); ar != end(); ++ ar )
		if( ar -> getID() == machineID || ar -> getClusterIDs().count( machineID ) != 0 )
			buddyIDs.insert( ar -> getID() );
	return buddyIDs;
}

//---------------------------------------------------------------------------

const LCDbAnalyser * LCDbAnalysers::findByName( const std::string & name ) const {
		return findMatch( LDbNames::LCMatcher( name ) );
}

//---------------------------------------------------------------------------

