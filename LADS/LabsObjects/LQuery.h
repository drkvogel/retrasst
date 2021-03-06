//---------------------------------------------------------------------------

#ifndef LQueryH
#define LQueryH

#include <string>
#include "LIMSDatabase.h"
#include "rosetta.h"
#include "LogFile.h"

class XQUERY;

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

	class Iterator {
		LQuery * q;
		int record;
	public:
		Iterator( LQuery * owner, int pos ) : q( owner ), record( pos ) {}
		const ROSETTA & operator*() const;
		bool operator!=( const Iterator & other ) const;
		const Iterator& operator++();
	};

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
	void close();
	Iterator begin();
	Iterator end();

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

	LogFile &getLog() const;
	void logQuery( LogFile & file );
	void logError( const std::string & message );
	void logCount( int records );
};

//---------------------------------------------------------------------------

#endif

