//---------------------------------------------------------------------------

#ifndef LCDbRackH
#define LCDbRackH

#include "LDbIdClasses.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

//---------------------------------------------------------------------------

class LCDbRack : public LCDbID
{
	int populationCID, mapCID, sectionCID;
	std::string section, fullName, site;
	short posInTank, posInSection, capacity;
	int projectID, boxTypeID;

	void copyFields( const LQuery & central );
	void calculatePosition( LQuery & central );
	
public:

	enum Status { EXPECTED, CONFIRMED, REMOVED, DESTROYED, DELETED = 99 };

	LCDbRack( const std::string & place, int tank, const std::string & rak );
	LCDbRack( const LQuery & central ) { copyFields( central ); }

	int getPopulationID() const { return populationCID; }
	void setSectionType( int id ) { sectionCID = id; }
	int getSectionType() const { return sectionCID; }
	void setEmptySlots( short n ) { capacity = n; }
	short getEmptySlots() const { return capacity; }
	void setProjectCID( int id ) { projectID = id; }
	int getProjectCID() const { return projectID; }
	void setBoxTypeID( int id ) { boxTypeID = id; }
	int getBoxTypeID() const { return boxTypeID; }

	void setTankMapID( int id ) { mapCID = id; }
//	int getTankMapID() const { return mapCID; }
	int getPosInTank() const { return posInTank; }
	void setPosInSection( short pos ) { posInSection = pos; }
	void setPosInTank( short pos ) { posInTank = pos; }
	int getPosInSection() const { return posInSection; }
	const std::string & getName() const { return fullName; }

	bool findRack( LQuery central );
//	bool findRackByTankMap( LQuery central );
	bool createRack( LQuery central );
	bool updateRecord( LQuery central );
};

//---------------------------------------------------------------------------

class LCDbRacks : public LDbCache< LCDbRack >
{

public:

	bool read( LQuery qCentral, int tankCID, int rackTypeCID = 0 );
};

//---------------------------------------------------------------------------

#endif
