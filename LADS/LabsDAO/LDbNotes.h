#ifndef LDbNotesH
#define LDbNotesH

#include "LDbIdClasses.h"
#include "LDbCacheBase.h"
#include "LCDbAnalyser.h"
#include "LCDbProject.h"

//---------------------------------------------------------------------------

class LDbNoteCount
{
protected:

	short notes;

public:

	LDbNoteCount( short count = 0 ) : notes( count )
	{}

	bool hasNotes() const { return notes > 0; }
};

//---------------------------------------------------------------------------

class LDbNote
{
	std::string content;
	TDateTime created;
	int user, process;

protected:

	LDbNote( const std::string & text, int procID, int opID, TDateTime timeStamp = 0 )
	 : content( text ), process( procID ), user( opID ), created( timeStamp )
	{}

public:

	LDbNote( const std::string & text );

	TDateTime getTimeStamp();
	void setTimeStamp( TDateTime when );
	std::string getContent() const { return content; }
	int getUserID() const { return user; }
	int getProcID() const { return process; }
};

//---------------------------------------------------------------------------

class LBDbNote : public LDbNote
{
	int objectBID, machineCID;

public:

	LBDbNote( const std::string & text, int linkID );

	LBDbNote( const LQuery & cluster );
	bool saveRecord( LQuery query );
};

//---------------------------------------------------------------------------

class LBDbNotes
{
	std::vector< LBDbNote > cache;

public:

	bool read( LQuery cluster, int objID );
	operator Range< LBDbNote >() const { return cache; }

	bool mark( LQuery cluster, int objID );
	unsigned deleteOld( LQuery cluster, TDateTime flagged, TDateTime oldest );
};

//---------------------------------------------------------------------------

class LPDbNote : public LDbNote, public LPDbID
{
	int objectPID;

public:

	LPDbNote( const std::string & text, int linkID )
	 : LDbNote( text ), objectPID( linkID )
	{}

	LPDbNote( const LDbNote & details, int linkID )
	 : LDbNote( details ), objectPID( linkID )
	{}

	LPDbNote( const LQuery & query );
	bool saveRecord( LQuery query );
};

//---------------------------------------------------------------------------

class LPDbNotes : public LDbCache< LPDbNote > // , public LPDbCacheMap< LPDbNotes >
{

public:

	bool read( LQuery project, int objID );
};

//---------------------------------------------------------------------------

#endif
