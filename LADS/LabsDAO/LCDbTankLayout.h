//---------------------------------------------------------------------------

#ifndef LCDbSectionH
#define LCDbSectionH

#include "LSDbBox.h"
#include "LDbIdClasses.h"
#include "LDbValid.h"
#include "LDbCacheBase.h"

class LCDbSectionDef : public LCDbID
{
	int tankLayoutCID;
	short fillOrder, status, confirmRack;
	std::string sectionPrefix;
	short firstRack,lastRack, rackCapacity;

public:

	LCDbSectionDef()
	 : tankLayoutCID(0), sectionPrefix("Rack"), status(LDbValid::NEW_RECORD),
	   firstRack(0), lastRack(0), fillOrder(0), rackCapacity(0), confirmRack(0)
	{}

	explicit LCDbSectionDef( const LQuery & query );

	int getTankLayoutCID() const { return tankLayoutCID; }
	short getFillOrder() const { return fillOrder; }
	const std::string & getSectionPrefix() const { return sectionPrefix; }
	short getFirstRack() const { return firstRack; }
	short getLastRack() const { return lastRack; }
	short getCapacity() const {return 1 + lastRack - firstRack;}
	short getFirstPosition() const;
	short getRackCapacity() const {return rackCapacity;}
	short getStatus() const {return status;}
	short getConfirmRack() const {return confirmRack;}
	bool isConfirmRack() const {return confirmRack != 0;}

	void setTankLayoutCID( int param ){ tankLayoutCID = param; }
	void setFillOrder( short param ){ fillOrder = param; }
	void setSectionPrefix( std::string param ){ sectionPrefix = param; }
	void setFirstRack( short param ){ firstRack = param; }
	void setLastRack( short param ){ lastRack = param; }
	void setRackCapacity( short param ){ rackCapacity = param; }
	void setConfirmRack( short param ){ confirmRack = param; }

	bool saveRecord( LQuery central );
};

//---------------------------------------------------------------------------

class LCDbSectionDefs : public LDbCache< LCDbSectionDef >, public LDbSingleton< LCDbSectionDefs >
{

public:

	bool read( LQuery central, bool readAll );
	const LCDbSectionDef * find( int tankLayout, const std::string & name ) const;
};


//---------------------------------------------------------------------------
#endif
