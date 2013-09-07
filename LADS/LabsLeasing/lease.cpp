//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
#include <math.h>
#include <winbase.h>
#include "lease.h"
#include "LQuery.h"
#include "LCDbAuditTrail.h"

//---------------------------------------------------------------------------
int TLeaseManager::instances = 0;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
TLeaseManager::TLeaseManager( const std::string & descript )
	:
	nlease( 0 ),
	retry_max( 0 )
{
	if ( instances++ > 0 )
		{Application->MessageBox( L"TLeaseManager, cannot create multiple instances",
			L"BUG!", MB_OK );
//		db = NULL;   	// ENSURE EXCEPTION IF THIS OBJECT IS USED
		return;
		}

/*	db = new TDatabase( NULL );
	db->AliasName = database_alias.c_str();
	db->DatabaseName = "TLeaseManagerDB";
	db->LoginPrompt = false;
	if( parameters != "" )
		{db->Params->Text = parameters.c_str();
		}
	q = new TQuery( NULL );
	q->DatabaseName = db->DatabaseName;
	q->UniDirectional = true;
*/
	q = new LQuery( LIMSDatabase::getCentralDb() );
	DWORD cname_length = MAX_COMPUTERNAME_LENGTH+1;
	if ( ! GetComputerName( computer_name, &cname_length ) )
		{wcscpy( computer_name, L"ANON" );
		}
	SetDescript( descript );
	lease = (TLease **) malloc( (nlease + 7) * sizeof(TLease *) );
	renew = new TTimer( NULL );
	renew->Interval = 40000 * LEASE_TIME_MINIMUM; // CONVERT TO 2/3 * MSECS
	renew->OnTimer = Renewal;
	tidy_database();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

TLeaseManager::~TLeaseManager( void )
{
	int	i;
	for ( i = 0; i < nlease; i++ )
		{delete lease[i];
		}
	free( lease );
	nlease = 0;
	tidy_database();
//	q->Close();	// CLOSE AND DESTROY ANY RESIDUAL DATABASE THINGS
	delete q;
//	db->Connected = false;
//	delete db;
    delete renew;

	instances--;	// DECREMENT COUNTER TO ALLOW A NEW INSTANCE TO BE MADE
}

//---------------------------------------------------------------------------
//  return the name of the database this manager is using
//---------------------------------------------------------------------------

std::string TLeaseManager::getDbName( void )
{
	q->setSQL( "SELECT dbmsinfo('database')" );
	query_open();
	std::string result = q->readString( 0 );
//	q->Close();
	return result;
}
//---------------------------------------------------------------------------

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
					/* TIDY UP ANY OBSELETE LEASES */
void TLeaseManager::tidy_database( void )
{
//	q->Close();
//	q->SQL->Clear();
	q->setSQL( "DELETE FROM c_lease WHERE expiry < DATE('now')" );
	query_exec();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		/* CHANGE TEXT DESCRIPTION OF TASK USING LEASE MANAGER */
void TLeaseManager::SetDescript( const std::string & descript )
{
	std::string host = AnsiString( computer_name ).c_str();
	task_descript = host + '/' + descript ;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	/* COMPARE SYSTEM AND CENTRAL TIMES, RETURN FALSE IF TOO FAR ADRIFT */
bool TLeaseManager::synch_time( TDateTime &time_system, TDateTime &time_central )
{
	double	tdif;
//	q->Close();
//	q->SQL->Clear();
	q->setSQL( "SELECT DATE('now') AS time_central FROM c_block" );
	if ( ! query_open() )
		{return( true );	// IGNORE ON ERROR
		}
/*	if ( q->RecordCount < 1 )	// CENTRAL TIME UNAVAILABLE, ASSUME OK
		{
		q->Close();
		db->Connected = false;
		return( true );	// time_central
		}
*/	time_central = q->readDateTime(0);
//	q->Close();
//	db->Connected = false;
	time_system = TDateTime::CurrentDateTime();
	tdif = 86400.0 * fabs( (double) time_central - (double) time_system );
	if ( tdif > 180 )	// SIGNAL PROBLEM IF MORE THAN 3 MINS APART
		{
		return( false );
		}
	return( true );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
		/* RETURN POINTER TO A LEASE, CREATE IF DOESN'T EXIST */
TLease *TLeaseManager::get_lease( int project_id, int task_id )
{
	int	i;
	for ( i = 0; i < nlease; i++ )
		{
		if ( project_id == lease[i]->proj_id && task_id == lease[i]->task_id )
			{return( lease[i] );
			}
		}
	TLease *lnew = lease[nlease] = new TLease( this, project_id, task_id );
	if ( 0 == ++nlease % 5 )
		{lease = (TLease **) realloc( lease, (nlease + 7) * sizeof(TLease *) );
		if ( NULL == lease )
			{Application->MessageBox( L"TLeaseManager, NULL alloc",
				L"BUG!", MB_OK );
			}
		}
	return( lnew );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
    /* RENEW ANY LEASES THAT ARE NEAR EXPIRY AND HAVE NOT BEEN DE-ACTIVATED */
void __fastcall TLeaseManager::Renewal(TObject *)
{
	int	i;
	renew->Enabled = false;
	for ( i = 0; i < nlease; i++ )
		{lease[i]->Renewal();
		}
	renew->Enabled = true;
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
					/* WHEN CAN A LEASE BE GRANTED */
int TLeaseManager::Availability( int project_id, int task_id, std::string &error_msg, int lease_type )
{
	return( get_lease( project_id, task_id )->Availability( error_msg, lease_type ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
					/* ATTEMPT TO ACTIVATE A LEASE */
int TLeaseManager::Activate( int project_id, int task_id, int period,
		std::string &error_msg, int lease_type )
{
	return( get_lease( project_id, task_id )->Activate( period, error_msg, lease_type ) );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
						/* DE-ACTIVATE A LEASE */
void TLeaseManager::Deactivate( int project_id, int task_id )
{
	get_lease( project_id, task_id )->Deactivate();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
					/* ALL LEASES FOR THIS TASK */
void TLeaseManager::DeactivateAll( void )
{
	int	i;
//	q->Close();
//	q->SQL->Clear();
	q->setSQL( "DELETE FROM c_lease WHERE task_desc=:td" );
//	q->ParamByName( "td" )->AsString = task_descript.c_str();
	q->setParam( "td", task_descript );
	query_exec();    			// IGNORE FAILURE
	for ( i = 0; i < nlease; i++ )
		{lease[i]->active = false;
		}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
					/* ATTEMPT TO OPEN A QUERY */
bool TLeaseManager::query_open( void )
{
	bool	attempt = true;
	bool	success = false;
	int	retry = retry_max;
	TCursor	initial_cursor = Screen->Cursor;
//	q->Close();
//	db->Connected = true;
	try
		{do
			{Screen->Cursor = crSQLWait;
			try
				{q->open();
				success = true;
				}
			catch ( Exception &e )
				{
				Screen->Cursor = initial_cursor;
				attempt = ( mrYes == Application->MessageBox(
					L"Unable to fetch info from database"
					"\n\nAttempt again ?",
					L"Database Error", MB_YESNO ) );
				}
			}
			while ( attempt && !success && retry-- > 0 );
		}
	__finally
		{Screen->Cursor = initial_cursor;
		}
	return( success );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
					/* ATTEMPT TO EXECUTE A QUERY */
bool TLeaseManager::query_exec( void )
{
	bool	attempt = true;
	bool	success = false;
	int	retry = retry_max;
	TCursor	initial_cursor = Screen->Cursor;
//	q->Close();
//	db->Connected = true;
	try
		{do
			{Screen->Cursor = crSQLWait;
			try
				{q->execSQL();
				success = true;
				}
			catch ( Exception &e )
				{
				Screen->Cursor = initial_cursor;
				attempt = ( mrYes == Application->MessageBox(
					L"Unable to make change in database"
					"\n\nAttempt again ?",
					L"Database Error", MB_YESNO ) );
				}
			}
			while ( attempt && !success && retry-- > 0 );
		}
	__finally
		{Screen->Cursor = initial_cursor;
//		db->Connected = false;
		}
	return( success );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
					/* RETURN LIST OF ACTIVE LEASES
void TLeaseManager::GetActive( int *nactive, TLeaseInfo **info, int project_id )
{
	int	i;
	char	sql[500];
	double	interval;
	std::string	desc;
	TDateTime	exp, now;
	*nactive = 0;
	*info = NULL;
	q->Close();
	q->SQL->Clear();
	sprintf( sql, "SELECT proj_id,task_id,task_desc,expiry,DATE('now') AS centime"
				" FROM c_lease WHERE expiry > DATE('now')" );
	if ( project_id != 0 )
		{
		char	extra[50];
		sprintf( extra, " AND proj_id=%d", project_id );
		strcat( sql, extra );
		}
	strcat( sql, " ORDER BY proj_id,task_id" );
	q->SQL->Add( sql );
	if ( ! query_open() )
		{q->Close();
		db->Connected = false;
		return;
		}
	*nactive = q->RecordCount;
	if ( *nactive > 0 )
		{*info = new TLeaseInfo[*nactive];
		}
	for ( i = 0; i < *nactive; i++ )
		{
		(*info)[i].project_id = q->Fields->Fields[0]->AsInteger;
		(*info)[i].task_id = q->Fields->Fields[1]->AsInteger;
		desc = bcsToStd( q->Fields->Fields[2]->AsString );
		strncpy( (*info)[i].task_descript, desc.c_str(), 30 );
		exp = q->Fields->Fields[3]->AsDateTime;
		now = q->Fields->Fields[4]->AsDateTime;
		interval = 1440 * (double) ( exp - now );
		(*info)[i].minutes_remaining = 1 + (int) interval;
		q->Next();
		}
	q->Close();
	db->Connected = false;
}
*/
//---------------------------------------------------------------------------

TLease::TLease( TLeaseManager *landlord, int project_ident, int task_ident )
	:
	active( false ),
	proj_id( project_ident ),
	task_id( task_ident ),
	landlord( landlord ),
	expiry( 0 ),
	renewal_due( 0 )
{
	q = landlord->q;	// LOCAL COPY FOR CONVENIENCE
//	db = landlord->db;
	task_descript = landlord->task_descript; // SAVE VALUE AT CREATE-TIME
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

TLease::~TLease( void )
{
	Deactivate();
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
						/* ATTEMPT TO RENEW LEASE */
int TLease::Renewal( void )
{
	TDateTime tnow = TDateTime::CurrentDateTime();
	if( tnow < renewal_due || !active )
		return( 0 );

	char tmp[300];
	sprintf( tmp, "UPDATE c_lease"
				" SET expiry=DATE('now')+DATE('%d minutes')"
				" WHERE proj_id=%d AND task_id=%d AND task_desc='%s'",
				length_minutes, proj_id, task_id, task_descript.c_str() );
	q->setSQL( tmp );
	if( !landlord->query_exec() )
		return( 5 );   // FAILED TO RENEW, SUGGEST RE-TRY INTERVAL

	expiry = tnow + (TDateTime)(length_minutes / 1440.0);
	renewal_due = tnow + (TDateTime)(length_minutes / 2000.0);
	return( 0 );
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
					 /* LENGTH OF TIME TO AVAILABILITY */
int TLease::Availability( std::string &error_msg, int lease_type )
{
	if ( lease_type == LEASE_TYPE_TOTAL ) {	// RESTRICT TO BLOCKING LEASES
		q->setSQL( "SELECT expiry, DATE('now') AS centime, task_desc FROM c_lease"
		" WHERE proj_id=:pi AND expiry > DATE('now')" );
	} else {
		q->setSQL( "SELECT expiry, DATE('now') AS centime, task_desc FROM c_lease"
		" WHERE proj_id=:pi AND expiry > DATE('now')"
		" AND (task_id IN"
		"  ( SELECT running FROM c_block WHERE blocked=:ti and block_type <> 99 )"
		" OR lease_type = -1)"	// -1 IS TOTAL BLOCK
		" ORDER BY expiry DESC" );
		q->setParam( "ti", task_id );
		}
		q->setParam( "pi", proj_id );
	if ( ! landlord->query_open() )
		{
		error_msg = "Database not accessible";
		return( 5 );    	// DEFAULT TO 5 SECONDS
		}
	if ( q->eof() )	{
		error_msg = "";
		return( 0 );		// ALL LEASES EXPIRED
		}
	TDateTime exp = q->readDateTime( 0 );
	TDateTime now = q->readDateTime( 1 );
	error_msg = q->readString( 2 );
	double	interval = 1440 * (double) ( exp - now );
	return( 1 + (int) interval ); 	// RETURN TIME TO LAST EXPIRY
}

//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
			 /* ACTIVATE, RETURN 0 IF OK, ELSE AVAILABLITY TIME */
int TLease::Activate( int period, std::string &error_msg, int lease_type )
{
	char	tmp[300];
	if ( lease_type != LEASE_TYPE_NORMAL && lease_type != LEASE_TYPE_TOTAL )
		{error_msg = "Invalid lease_type";
		return( 5 );
		}
	if ( TDateTime::CurrentDateTime() >= expiry )
		{active = false;	// LEASE ELAPSED (SHOULD NOT HAPPEN!)
		}
	if ( active )
		{return( Renewal() );
		}
	int	avail = Availability( error_msg, lease_type );
	if ( avail > 0 )
		{return( avail );	// UNABLE TO GET LEASE
		}
	if ( period > LEASE_TIME_MAXIMUM )
		{length_minutes = LEASE_TIME_MAXIMUM;
		}
	else if ( period < LEASE_TIME_MINIMUM )
		{length_minutes = LEASE_TIME_MINIMUM;
		}
	else
		{length_minutes = period;
		}
//	q->Close();
//	q->SQL->Clear();
	sprintf( tmp, "INSERT INTO c_lease"
				" (proj_id,task_id,"
//				" process_cid, "		/// fixme once database rebuilt
				"task_desc,lease_type,start,expiry)"
				" VALUES (:pi,:ti,"
//				" :pid, "
				":td,:lt,DATE('now'),DATE('now')+DATE('%d minutes'))",
				length_minutes );
	q->setSQL( tmp );
	q->setParam( "pi" , proj_id );
	q->setParam( "ti" , task_id );
	q->setParam( "td" , task_descript );
//	q->setParam( "pid" , LCDbAuditTrail::getCurrent().getProcessID() );
	q->setParam( "lt" , lease_type );
	TDateTime tnow = TDateTime::CurrentDateTime();
	if( !landlord->query_exec() )
	{
		expiry = tnow;
		renewal_due = tnow;
		active = false;
		error_msg = "Database not accessible";
		return( 5 );
	}
	else
	{	expiry = tnow + (TDateTime) ( length_minutes / 1440.0 );
		renewal_due = tnow + (TDateTime) ( length_minutes / 2000.0 );
		active = true;
		return( 0 );
	}
}
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
						/* DE-ACTIVATE LEASE */
void TLease::Deactivate( void )
{
//	q->Close();
//	q->SQL->Clear();
	q->setSQL( "DELETE FROM c_lease WHERE"
		" proj_id=:pi AND task_id=:ti AND task_desc=:td" );
	q->setParam( "pi" , proj_id );
	q->setParam( "ti" , task_id );
	q->setParam( "td" , task_descript );
	landlord->query_exec();    		// IGNORE FAILURE
	active = false;
}
//---------------------------------------------------------------------------

