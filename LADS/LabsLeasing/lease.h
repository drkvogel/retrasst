//---------------------------------------------------------------------------
#ifndef leaseH
#define leaseH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <Classes.hpp>
#include <DBTables.hpp>
#include <string>
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define	DATE_REVERSAL	1	// IF TRUE, SWAP DAY-MONTH ORDER IN DATES
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/*
	HOLDS CURRENTLY ACTIVE LEASES
TABLE C_LEASE
	PROJ_ID		INTEGER		PROJECT (I.E. DATABASE) ID
	TASK_ID		INTEGER	    	WHAT TYPE OF TASK HOLDS LEASE (E.G. 10 = USER WRAPPERS)
	TASK_DESC	TEXT[30]	DESCRIPTION OF TASK
	LEASE_TYPE	INTEGER		0 = NORMAL, -1 = TOTAL
	START		TIME		WHEN ENTRY WAS MADE IN DATABASE
	EXPIRY		TIME		WHEN ENTRY EXPIRES (180 - 1E+7 SECS)

*/
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
/*	WHEN RUNNING TASK IS ACTIVE, BLOCKED TASK CANNOT BE STARTED
TABLE C_BLOCK
	RUNNING		INTEGER		TASK RUNNING
	BLOCKED		INTEGER		TASK BLOCKED BY RUNNING ONE
*/
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define	LEASE_TYPE_NORMAL	0		// STANDARD `I AM HERE' LEVEL
#define	LEASE_TYPE_TOTAL	-1		// PREVENT ALL ACCESSES
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define	LEASE_TIME_MAXIMUM	20200		// APPROX. 2 WEEKS
#define	LEASE_TIME_DEFAULT	15		// DEFAULT, 15 MINUTES
#define LEASE_TIME_MINIMUM	3		// MINIMUM 3 MINUTES
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
#define	TASKID_WILDCARD		0		// MATCH ANY TASK ID
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
typedef	struct
	{
	int	project_id;
	int	task_id;
	char task_descript[32];
	int	minutes_remaining;
	}
	TLeaseInfo;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class TLease;
class LQuery;
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
class TLeaseManager
{
friend	TLease;
private:
	static	int	instances;	// CAN ONLY HAVE ONE LEASE MANAGER PER PROCESS
	std::string  task_descript;
	wchar_t	computer_name[MAX_COMPUTERNAME_LENGTH+5];
	int	nlease;
	int	retry_max;
	// TDatabase	*db;
	// TQuery	*q;
	LQuery * q;
	TTimer	*renew;
	TLease	*current;
	TLease	**lease;

	TLease	*get_lease( int project_id, int task_id );
	void 	tidy_database( void );
	bool 	query_open( void );
	bool 	query_exec( void );

	void __fastcall Renewal(TObject *Sender);

public:
	TLeaseManager( const std::string & descript );
	~TLeaseManager( void );
	std::string 	getDbName( void );
	bool 	synch_time( TDateTime &time_system, TDateTime &time_central );
	void	SetDescript( const std::string & descript );
	int	Availability( int project_id, int task_id, std::string &descript,
			int lease_type = LEASE_TYPE_NORMAL  ); // WHEN CAN A LEASE BE GRANTED
	int	Activate( int project_id, int task_id, int period, std::string &descript,
			int lease_type = LEASE_TYPE_NORMAL );
	void	Deactivate( int project_id, int task_id );
	void	DeactivateAll( int project_id ); // ALL LEASES FOR TASK IN PROJECT
	void	DeactivateAll( void );		// ALL LEASES FOR TASK
	void	GetActive( int *nactive, TLeaseInfo **info, int project_id = 0 );
};
//- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

class TLease
{
friend	TLeaseManager;
private:
	int	length_minutes;		// LENGTH OF LEASE
	int	proj_id;		// PROJECT ON WHICH LEASE IS TAKEN
	int	task_id;
	bool	active;			// IS LEASE (BELIEVED TO BE) ACTIVE
	std::string	task_descript;
	LQuery	*q;
	// TDatabase	*db;
	TDateTime	start;
	TDateTime	renewal_due;
	TDateTime	expiry;
	TLeaseManager	*landlord;
	TLease( TLeaseManager *owner, int project_ident, int task_ident );
	~TLease( void );
public:
	int	Availability( std::string &error_msg, int lease_type );    // LENGTH OF TIME TO AVAILABILY
	int	Activate( int period, std::string &error_msg, int lease_type ); // ACTIVATE, 0 IF OK, ELSE AVAILABLITY TIME
	int	Renewal( void );
	void	Deactivate( void );    // DE-ACTIVATE LEASE
};
//---------------------------------------------------------------------------
#endif

