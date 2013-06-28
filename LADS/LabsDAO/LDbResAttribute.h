//---------------------------------------------------------------------------

#ifndef LDbResAttributeH
#define LDbResAttributeH

#include "LQuery.h"
#include "LDbRange.h"
#include <vector>

//---------------------------------------------------------------------------

class LDbResAttribute
{
	int fieldType;
	String value;
	TDateTime when;

protected:

	enum Type { DATE_TIME = 16, TEXT = 8, REAL = 4, INTEGER = 2, OTHER = 1 } dataType;

	LDbResAttribute( const String & otName, Type data, const String & val, TDateTime timeStamp );

public:

	LDbResAttribute( const LQuery & buddyData );

	int getType() const { return fieldType; }
	const String & getValue() const { return value; }
	TDateTime getTimeStamp() const { return when; }

	bool createRecord( LQuery cluster, int brid ) const;
};

//---------------------------------------------------------------------------

struct ReagentID : public LDbResAttribute
{
	ReagentID( const String & id )
	 : LDbResAttribute( "Reagent_ID", TEXT, id, Now() )
	{}
};

//---------------------------------------------------------------------------

struct ReagentLot : public LDbResAttribute
{
	ReagentLot( const String & id )
	 : LDbResAttribute( "Reagent_Lot", TEXT, id, Now() )
	{}
};

//---------------------------------------------------------------------------

struct Cuvette : public LDbResAttribute
{
	Cuvette( int id )
	 : LDbResAttribute( "Cuvette", INTEGER, id, Now() )
	{}
};

//---------------------------------------------------------------------------

class LDbAttributes
{
	std::vector< LDbResAttribute > details;

public:

	bool checkResult( LQuery cluster, int objID );
	int addReagent( LQuery qCentral );
	int checkReagent( LQuery qCentral );
	bool mark( LQuery query, int objID );
	unsigned deleteOld( LQuery cluster, TDateTime flagged, TDateTime oldest );
};

//---------------------------------------------------------------------------

#endif
