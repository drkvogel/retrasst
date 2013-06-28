//---------------------------------------------------------------------------

#ifndef LQueryH
#define LQueryH

#include "rosetta.h"
#include <string>
#include "LIMSDatabase.h"

class XQUERY;
class XMLFile;

//---------------------------------------------------------------------------

class LQuery
{
	LIMSDatabase db;
	std::string sql;
	ROSETTA parameters;
	XQUERY * cursor;
	int rows;
	bool logging;

public:

	enum LogLevel { ERRORS, UPDATES, SELECTS };

	LQuery( const LIMSDatabase & db );
	LQuery( const LQuery & other );
	LQuery & operator= ( const LQuery & other );   // not allowed
	~LQuery();

	const LIMSDatabase & getDatabase() const { return db; }
	void setSQL( const std::string & query );
	const std::string & getSQL() const { return sql; }

	void setParam( const std::string & pName, int value );
	void setParam( const std::string & pName, double value );
	void setParam( const std::string & pName, const std::string & value );
	void setParam( const std::string & pName, const XDATE & value );
	void setParam( const std::string & pName, const XTIME & value );

	bool execSQL();
	bool open();
	bool eof();
	bool next();

	bool fieldExists( const std::string & field ) const;
	int readInt( const std::string & field ) const;
	TDateTime readDateTime( const std::string & field ) const;
	double readDouble( const std::string & field ) const;
	std::string readString( const std::string & field ) const;

	const ROSETTA & getRecord() const;
	int readInt( int index ) const;
	double readDouble( int index ) const;
	TDateTime readDateTime( int index ) const;
	std::string readString( int index ) const;

private:

	static LogLevel logLevel;

	typedef bool (LQuery::*Operation)();
	bool call( Operation function, LogLevel log );
	bool doExec();
	bool doOpen();
	bool doNext();

	void dropCursor();

	XMLFile & getLog();
	void logQuery();
	void logError( const std::string & message );
	void logCount( int records );
	void logResult( const char * tag, const std::string & result );
};

//---------------------------------------------------------------------------

#endif

