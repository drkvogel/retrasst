#ifndef LCDbAuditTrailH
#define LCDbAuditTrailH

//---------------------------------------------------------------------------

#include <IdSMTP.hpp>
#include <vector>
#include "LQuery.h"
#include "LDbIdClasses.h"

//---------------------------------------------------------------------------

class LCDbAuditTrail : public LCDbID
{
	std::string userName, computer, progName;
	LQuery cQuery;
	TIdSMTP * mailer;

	static LCDbAuditTrail * current;		// only one allowed
	static const char * default_email;

	const static short MAX_MESSAGE = 1000;		// size of details field
	struct LineSplitter;

	LCDbAuditTrail( const LCDbAuditTrail & );	  		// not implemented
	LCDbAuditTrail & operator=( LCDbAuditTrail & );		// not implemented

	std::string getIngresInfo( const std::string & param );

public:

	enum MessageType
	{
		INFORMATION = 0,		   	// a normal event, for info.
		STARTUP = 1,				// first entry in audit trail
		SHUTDOWN = 2,				// last (if normal shut-down)
		USER_LOGIN = 3,				// creates new process ID
		DB_CHANGE = 4,				// switch to another project
		MINOR_ERROR = 5,			// logged but continues
		MAJOR_ERROR = 6,			// e-mail message also sent
		COMPLETED = 7				// program ran to completion
	};

	LCDbAuditTrail();
	~LCDbAuditTrail();

	static LCDbAuditTrail & getCurrent();
	static bool hasStarted();

	void start( const std::string & message = "** Program started **" );
	void login() { start( "** Operator logging in **" ); }
	int getProcessID();
	void stop( const std::string & message = "** Program closing **" );

	bool sendEMail( const std::string & body, std::string address = "", std::string subject = "" );
	bool addRecord( const std::string & message, MessageType type = INFORMATION );

	struct Process
	{
		int processID, operatorID, buddyID;
		std::string program;
		TDateTime start, finish;

		Process( const LQuery & data );
	};

	int findUserID( int processID );
	std::vector< LCDbAuditTrail::Process > read( const XTIME & first, const XTIME & last, int proj );
	TDateTime findRecent( MessageType status );
};

//---------------------------------------------------------------------------

#endif

