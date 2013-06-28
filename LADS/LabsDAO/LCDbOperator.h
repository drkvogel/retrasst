//---------------------------------------------------------------------------

#ifndef LCDbOperatorH
#define LCDbOperatorH

#include "LDbIdClasses.h"
#include "LDbNameBase.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LCDbOperator : public LCDbID, public LDbNames, public LDbValid
{
	std::string passCode;

	static std::string encrypt( const std::string & pass );

public:

	struct Priv
	{
		int program, project, machine;
		short page, status;

		Priv( const LQuery & central );
	};

//	enum Abilities { VALIDATION = 1, L2_VALIDATION = 2, PLOT_QC = 4, WIZARD = 8 };

	static const short NEW_PASSWORD = 2, LOCKED = 90;		// extends LDbValid::RecordStatus

	LCDbOperator( int id = 0 ) : LCDbID( id ) {}

	explicit LCDbOperator( const LQuery & central );
	bool saveRecord( LQuery central );

	bool isActive() const { return LDbValid::isActive() && !hasLockedAccount(); }

	void lockAccount() { status = LOCKED; }
	bool hasLockedAccount() const { return status == LOCKED; }

	bool needsPassword() const;
	bool hasUsedPassword( LQuery query, const std::string & password ) const;

	void setPassword( const std::string & password )
	{
		if( password.length() < 3 )
			throw Exception( "Password must be at least 3 characters" );
		passCode = encrypt( password );
	}

	bool matchPassword( const std::string & password ) const
	{
		return passCode.empty() || encrypt( password ) == passCode;
	}

	void addPermission( Priv allowed ) { permissions.push_back( allowed ); }

	bool canRun( int project, int analyser = 0, short page = 0 ) const;

private:

	std::vector< Priv > permissions;
};

//---------------------------------------------------------------------------

class LCDbOperators : public LDbCache< LCDbOperator >, public LDbSingleton< LCDbOperators >
{
//	class PasswordMatcher;

	int currentID;

public:

	LCDbOperators() : currentID( 0 ) {}

	void setCurrent( const LCDbOperator & user ) { currentID = user.getID(); }
	static int getCurrentID() { return records().currentID; }

	bool read( LQuery central, bool readAll );
	const LCDbOperator * findByName( const std::string & name ) const
	{
		return findMatch( LDbNames::LCMatcher( name ) );
	}
	const LCDbOperator * check( const std::string & name, const std::string & pass ) const;
};

//---------------------------------------------------------------------------

#endif

