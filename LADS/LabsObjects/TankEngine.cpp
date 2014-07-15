//---------------------------------------------------------------------------


#pragma hdrstop

#include "TankEngine.h"
#include "LIMSParams.h"
#include "LCDbProject.h"
#include "LQuery.h"
#include "LCDbObject.h"
#include "LCDbBoxCourier.h"
#include "LCDbTankLayout.h"
#include "LCDbTankMap.h"
#include "LCDbStoreDetail.h"
#include "LCDbRack.h"
#include "fAllocations.h"
#include <wia.h>

#define LAST_TANK	99

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

TankEngine*		TankEngine::instance=0;
TankInfoMap 	TankEngine::tankInfos;
TankDisplayMap 	TankEngine::tankDisplays;
TankSectionRackSlotMap TankEngine::slotsMap;
SectionCountMap TankEngine::sectionCounts;
RackCountMap 	TankEngine::rackCounts;
SlotPositionsMap TankEngine::slotPositions;

int    TankEngine::gCurrLocationCID;

TSRSExt TankEngine::storedTSRS;
//TankSectionRackSlot TankEngine::workingTSRS;
TSRSExt TankEngine::workingTSRSExt;
TankEngine::StoredLocationInfo TankEngine::sli;

bool   TankEngine::dataBuilt;

int TankEngine::minTank;
int TankEngine::maxTank;

const char * TankEngine::TrimZeroC( const char * leadZeroStr )
{
		if( leadZeroStr == NULL || *leadZeroStr == '\0' ) {
			return "";
		}

		static char retVal[30];
		std::size_t len0 = strlen(leadZeroStr);
		unsigned int i = 0, last = len0 - 1;
		while( i<last && leadZeroStr[i] == '0' ) {
				i++;
		}
		if( i > 0 ) {
				strncpy(retVal,&leadZeroStr[i],len0+1-i );
		} else {
				strcpy(retVal,leadZeroStr );
		}
		return retVal;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------
int TankEngine::toPosIntDef( const std::string & sVal,int def )
{
	static const std::string Idigits="0123456789";
	if( !sVal.empty() && sVal.length()<10
	  && sVal.find_first_not_of( Idigits ) == std::string::npos ) {

		const char * in = TrimZeroC( sVal.c_str() );
		return charToIntDef(in,def,false);

		/*		int iVal=atoi( in );
		char chk[100];
		sprintf(chk,"%d",iVal);
		if( strcmp(chk,in)==0 )	{
			return iVal;*/

	}
	return def;
}
int TankEngine::charToIntDef( const char * in , int def, bool sign)
{
		int iVal=atoi( in );
		char chk[100];
		sprintf(chk,"%d",iVal);
		if( strcmp(chk,in)==0 )	{
			if( sign )
			{
				return -iVal;
			}else
			{
				return iVal;
            }
		}
		return def;

}
//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------
int TankEngine::toIntDef( const std::string & sVal,int def )
{
	static const std::string Idigits="-0123456789";

	if( !sVal.empty() && sVal.length()<10
	  && sVal.find_first_not_of( Idigits ) == std::string::npos ) {
		const char * in = TrimZeroC( sVal.c_str() );
		bool sign; sign =false;
		if( *in == '-' )
		{
			in++;
			sign =true;
			return charToIntDef(in,def,sign);

		}else
		{
			return charToIntDef(in,def,sign);
		}
	}
	return def;
}

//---------------------------------------------------------------------------
//
//
//
//---------------------------------------------------------------------------
double TankEngine::toPosDoubleDef( const std::string & sVal,double def)
{
	double dVal;
	char chk[100];
	char fd[10]="%8.3f";

	std::string Fdigits="0123456789.";
	if( sVal.empty() ) {return def;}
	if( sVal.find_first_not_of( Fdigits )==std::string::npos )
	{
		std::size_t pt = sVal.find_first_of(".");
		if( pt == std::string::npos )
		{
			return def;
		}else
		{
			int wd=pt; // whole digits
			int dd=sVal.length()-(pt+1);
			dVal=atof(sVal.c_str());
			if( wd == 0 )
			{
				// case of string like ".nnnn"
				sprintf(fd,"%%%d.%df",(int)sVal.length(),dd);
				sprintf(chk,fd,dVal);

				char *p;
				p=chk;p++;
				if( strcmp(p,sVal.c_str())==0 )
				{
					return dVal;
				}else
				{
					return def;
				}
			}   else
			{
				sprintf(fd,"%%0%d.%df",(int)sVal.length(),dd);
				sprintf(chk,fd,dVal);
				if( strcmp(chk,sVal.c_str())==0 )
				{
					return dVal;
				}else
				{
					return def;
				}
			}
		}

	}else
	{
		dVal=def;
	}
	return dVal;

}


//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

TankEngine *TankEngine::Initialize()
{
	if( instance == 0 )
	{
		dataBuilt=false;
		gCurrLocationCID= BRUNKNOWN_INT;
		instance=new TankEngine();
		instance->resetCurrents();

	}
	return instance;
}

//---------------------------------------------------------------------------
// Build data for the allocation form
//---------------------------------------------------------------------------


bool TankEngine::buildData( LQuery  dbase )
{
	int tankExt,tankCID;
	bool foundTank;

// Pull out all the tanks
	tankInfos.clear();
	tankDisplays.clear();

	for( Range <LCDbObject > o= LCDbObjects::records(); o.isValid(); ++o )
	{
		foundTank=false;

		int oType=o->getObjectType();

		if (  oType== LCDbObject::STORAGE_POPULATION   )
		{
			tankCID = o->getID();


			itrTankInfoMap ti= tankInfos.find(tankCID);
			if( ti!= tankInfos.end() )
			{
				foundTank=true;
				tankExt=ti->second.tankExt;
			}

//-----------------------------------------
// Tank details have not found
// find related details starting with:
// c_object_name ( object_type = 9  ) PK object_cid, to be used as a FK for c_tank_map.tank_cid.
// PK c_tank_map.tank_cid  uses FK c_tank_map.storage_cid to reference PK in c_storage_details.storage_cid
// PK c_storage_details.storage_cid use FK rack_layout_cid to reference K in  c_tank_layout
//-----------------------------------------
			if( !foundTank )
			{

// Get the related record using the tank_id
				const std::string name = o->getName();
				const std::string descript = o->getDescription();
// 		toPosIntDef( const std::string & sVal,int def )
//				int s=std::isdigit( name[ 1 ] );

// The tank number A49 minus the single char prefix ie 49.
				tankExt = (std::isdigit( name[ 0 ] )? toPosIntDef(name,0) : toPosIntDef(name.substr( 1, 9 ),0));

// Look for details in c_tank_map
				const LCDbTankMap* tm= LCDbTankMaps::records().findCurrent(tankCID);

				if(  tm != NULL )
				{
				int scid=	tm->getStorageCID() ;
// Look for Store details
				const LCDbObject* ot= LCDbObjects::records().findByID( scid );

				if( ot != NULL )
				{
// Pull out the location, and filter on this current location.
				int locationCID=tm->getLocationCID();
				if( locationCID== gCurrLocationCID )
				{
// retrieve the racklayout CID
					int rackLayoutCID=tm->getLayoutCID();
					int sectionCount,rackCount,slotCapacity;
// Count the number of racks in this tank.
					countRack(dbase,tankCID,rackLayoutCID,sectionCount, rackCount, slotCapacity);
// Get the object Id should be the same as tank or r->getTankCid.

					TankSumInfo tsi;
					tsi.tankExt 	= tankExt;
					tsi.descript 	= ot->getDescription();
					tsi.position 	= tm->getPosition();
					tsi.shelf 		= tm->getPopulation();
					tsi.storageCID 	= tm->getStorageCID();
					tsi.rackLayoutCID = rackLayoutCID;
					tsi.sectionCount= sectionCount;  // number of sections in the tank
					tsi.rackCount 	= rackCount;	// total number of racks in the tank regardless of population
					tsi.slotCapacity= slotCapacity; // total number of slot in the tank.
					tsi.rackCapacity= -1;

					tankInfos.insert(TankInfoMap::value_type(tankCID,tsi));
				}
				}
			}
			}
		}
	}

// Copy across to a new display map to give an order by vessel and shelf

	for(CitrTankInfoMap j= tankInfos.begin(); j!=tankInfos.end(); j++ )
	{
		PositionShelf ps;
		TankSumInfo tsi;
		ps.first=j->second.position;
		ps.second=j->second.shelf;
		tsi=j->second;
		tankDisplays.insert(TankDisplayMap::value_type(ps,tsi));
	}


	TankSectionRackSlot tsrs;
	tankExt=0;
	int rackLayoutCID;
	rackLayoutCID=BRUNKNOWN_INT;
	slotsMap.clear();
// For each project
	for( Range< LCDbProject > p = LCDbProjects::records(); p.isValid(); ++ p )
	{
		if( p->isInCurrentSystem() && !p->isCentral() ) {

// pull out boxes which have arrived
		for( Range< LDbBoxArrival > r= LDbBoxArrivals::records(p->getID()); r.isValid(); ++ r )
		{

		int rackNo ;
		std::string rackSection, tmpRS;

//-----------------------------------------
// Retrieve the tank_cid from c_box_alloc to be used later to retrieve tank details
//-----------------------------------------

		tankExt=  BRUNKNOWN_INT;
		rackLayoutCID= BRUNKNOWN_INT;
		foundTank=false;
// get information relaing to the tank
		itrTankInfoMap ti= tankInfos.find(r->getTankCid());
		if( ti!= tankInfos.end() )
		{
			foundTank=true;
			tankExt=ti->second.tankExt;
			rackLayoutCID=ti->second.rackLayoutCID;
		}

		if( tankExt!=BRUNKNOWN_INT)
		{

// Pull out Sector and Rack number
			tmpRS=r->getRackNumber();
// Separate out into sector and rack number
			int i;
			int tl;
			tl=tmpRS.length();
			for(  i = 0; i< tl ; i++ )
			{
				if( tmpRS[i]== ' ' )
				{
					rackSection=tmpRS.substr(0,i-1);
					rackNo=toPosIntDef(tmpRS.substr(i+1,(tl-i)),-1);
					break;
				}else if( isdigit(tmpRS[i]) )
				{
					rackSection=tmpRS.substr(0,i); // first character
					rackNo=toPosIntDef(tmpRS.substr(i,(tl-i)+1),-1);
					break;
				}
			}

// Build up list of slots
// populate the tank number
			tsrs.tank=tankExt ;
			tsrs.fillOrder = getFillOrder(  rackLayoutCID, rackSection, rackNo );
//			tsrs.section=rackSection;
			tsrs.rack=rackNo;
			tsrs.slot=r->getSlotPosition();
			slotsMap.insert(TankSectionRackSlotMap::value_type(tsrs,r->getBoxName()));
		}
		}
		}
	}


// Build up a list of how each tank is populated.
//	tanksMap.clear();

	minTank=9999;
	maxTank=-9999;

	PositionShelf ps;
	for( int i = 1; i <= LAST_TANK; i++ )
	{
		int	count=0;

		CitrTankInfoMap ti=getTankInfoExt(i);
		if( ti != tankInfos.end() )
		{
			int tankCID=ti->first;
			if( i > maxTank ) { maxTank=i; }
			if( i < minTank ) { minTank=i; }
		// only loop around if there is a tank at this location
			for(CitrTankSectionRackSlotMap sl=slotsMap.begin();sl!=slotsMap.end();sl++)
			{
				if( sl->first.tank== i )
				{
					if (!sl->second.empty() )
					{
						count++;
					}
				}
			}
			tankInfos[tankCID].slotsUsed=count;
			ps.first=ti->second.position;
			ps.second=ti->second.shelf;
			tankDisplays[ps].slotsUsed=count;
		}
	}

	dataBuilt=true;
	return true;
}
bool TankEngine::rebuildTankData( void )
{
// Build up a list of how each tank is populated.


	int minTank=9999;
	int maxTank=-9999;


	PositionShelf ps;

	for( int i = 1; i <= LAST_TANK; i++ )
	{
		int	count=0;

		CitrTankInfoMap ti=getTankInfoExt(i);
		if( ti != tankInfos.end() )
		{
			int tankCID=ti->first;
			if( i > maxTank ) { maxTank=i; }
			if( i < minTank ) { minTank=i; }
		// only loop around if there is a tank at this location
		for(CitrTankSectionRackSlotMap sl=slotsMap.begin();sl!=slotsMap.end();sl++)
		{
			if( sl->first.tank== i )
			{
				if (!sl->second.empty() )
				{
					count++;
				}
			}
		}
		tankInfos[tankCID].slotsUsed=count;
			ps.first=ti->second.position;
			ps.second=ti->second.shelf;
			tankDisplays[ps].slotsUsed=count;
		}
	}
	return true;
}
//---------------------------------------------------------------------------
// Add a box entry to the list of allocations in the tanks
// normally used when recieving a box entry.
//---------------------------------------------------------------------------
bool TankEngine::addBoxEntry(const TankSectionRackSlot & pTsrs, const std::string & pBoxName )
{

	slotsMap.insert(TankSectionRackSlotMap::value_type(pTsrs,pBoxName));

	CitrTankSectionRackSlotMap ti=slotsMap.find(pTsrs);
	if(ti != slotsMap.end() && !(ti->second.empty()))
	{
		std::string a;
		a=ti->second;
		return true;
	}else
	{
		return false;
	}

}
//---------------------------------------------------------------------------
// Returns boolean to indicate if tank allocation information has been set up
// Used to expose value to code outside this module.
//---------------------------------------------------------------------------

bool TankEngine::isDataBuilt(void) const
{
	return dataBuilt;
}

//---------------------------------------------------------------------------
// Returns the minium tank number, calculated when initially building data
// or re-building data.
// Used to expose value to code outside this module.
//---------------------------------------------------------------------------

int TankEngine::getMinTank(void)
{
	return minTank;
}

//---------------------------------------------------------------------------
// Returns the maximum tank number, calculated when initially building data
// or re-building data.
// Used to expose value to code outside this module.
//---------------------------------------------------------------------------

int TankEngine::getMaxTank(void)
{
	return maxTank;
}


//---------------------------------------------------------------------------
// Receiving a value of the tank stoage location ID and storing it to indentify
// tanks of interest.
//---------------------------------------------------------------------------

bool TankEngine::setLocation( int pLocation )
{

	gCurrLocationCID=pLocation ;
	return true;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

void TankEngine::resetCurrents(void)
{
	workingTSRSExt.initialize();
}

void TankEngine::StoredLocationInfo::initialize(void)
{	tankCID		 = 0;
	tankSections = 0;
	tankRacks	 = 0;
	sectionRacks = 0;
	rackSlots    = 0;
	rackLayoutID = 0;
//	fillOrder	 = 0;
}

//---------------------------------------------------------------------------
// Check that the tank selected actually exists.
//---------------------------------------------------------------------------

bool TankEngine::testTankCID( const TankSectionRackSlot& tsrs )
{
	if(! dataBuilt ){return false; }
	if( gCurrLocationCID == BRUNKNOWN_INT ) { return false; }

	CitrTankInfoMap cti=getTankInfoExt(tsrs.tank);

	if ( cti  != tankInfos.end())
	{
		return true;
	}
	return false;
}


//---------------------------------------------------------------------------
// Set tankCID so long as the tank exists. Retrieve the external tank name also.
//---------------------------------------------------------------------------

bool TankEngine::setTankExt ( const TSRSExt & pTSRSExt  )
{
	if(! dataBuilt ){return false; }

	if( gCurrLocationCID == BRUNKNOWN_INT ) { return false; }
	int pTankExt;
	pTankExt=pTSRSExt.tank;
	sli.initialize();


	CitrTankInfoMap ti=getTankInfoExt( pTankExt);
	if( ti != tankInfos.end() )
	{
		sli.tankCID		 = ti->first;
		sli.tankRacks	 = ti->second.rackCount;
		sli.tankSections = ti->second.sectionCount;
		sli.rackLayoutID = ti->second.rackLayoutCID;
		workingTSRSExt.tank = ti->second.tankExt;
		workingTSRSExt.vessel = pTSRSExt.vessel;

		return true;
	}
	return false;
}

//---------------------------------------------------------------------------
//  setting  Testing to see if a section name exists given tank and section
//---------------------------------------------------------------------------


bool TankEngine::testFillOrderAndSection ( const TankSectionRackSlot&  pTsrs )
{
	if(! dataBuilt ){return false; }
 //	if( pTsrs.tank == BRUNKNOWN_INT || pTsrs.section== UNKNOWN_STRING ) { return false; }

	int rackLayoutID=	getRackLayoutID( pTsrs.tank );
	if( rackLayoutID == BRUNKNOWN_INT) {return false; }

//	String pSection;
//	pSection=pTsrs.section;

// Search through the tank layout to find a section
	for( Range< LCDbSectionDef > i = LCDbSectionDefs::records(); i.isValid(); ++ i )
	{
		if( i->getTankLayoutCID()== rackLayoutID )
		{
//			if(	pSection.Pos( i->getSectionPrefix())!=0 )
			if ( pTsrs.fillOrder == i->getFillOrder() )
			{
				return true;
			}
		}
	}
	return false;

}

//---------------------------------------------------------------------------
//  setting  a current section, given a tank
//---------------------------------------------------------------------------

bool TankEngine::setFillOrderAndSection ( const TSRSExt & pTSRSExt )
{

	if(! dataBuilt ){return false; }
	if( sli.tankCID == BRUNKNOWN_INT ) { return false; }

//	workingTSRS.section = UNKNOWN_STRING;
	workingTSRSExt.fillOrder = BRUNKNOWN_INT;
	workingTSRSExt.rack = BRUNKNOWN_INT;
	workingTSRSExt.slot = BRUNKNOWN_INT;

	sli.sectionRacks = BRUNKNOWN_INT;
	sli.rackLayoutID= BRUNKNOWN_INT;

// Find the tank details
	itrTankInfoMap ti= tankInfos.find(sli.tankCID);
	int rackLayoutID;

	if( ti!= tankInfos.end() )
	{
// retreive the rack layout
		rackLayoutID=ti->second.rackLayoutCID;
	}else
	{
		return false;
	}

// Search through the tank layout to find a section
	for( Range< LCDbSectionDef > i = LCDbSectionDefs::records(); i.isValid(); ++ i )
	{
		if( i->getTankLayoutCID()== rackLayoutID )
		{
// Drawing all the rack details for the tank
			if( pTSRSExt.fillOrder==i->getFillOrder() )
//			if(	pSection.Pos( i->getSectionPrefix())!=0 )
			{
				sli.rackLayoutID	  = rackLayoutID;
				sli.sectionRacks      = i->getLastRack() - i->getFirstRack() + 1 ;
				workingTSRSExt.fillOrder = pTSRSExt.fillOrder;
				workingTSRSExt.section   = pTSRSExt.section;
				return true;
			}
		}
	}
	return false;

}

//---------------------------------------------------------------------------
// test the selected rack given a tank, section and rack
//---------------------------------------------------------------------------

bool TankEngine::testRack( const TankSectionRackSlot& pTsrs )
{
	if(! dataBuilt ){return false; }
	if( pTsrs.fillOrder == BRUNKNOWN_INT ) { return false; }

	int rackLayoutID=	getRackLayoutID( pTsrs.tank );
	if( rackLayoutID == BRUNKNOWN_INT) {return false; }

	for( Range< LCDbSectionDef > i = LCDbSectionDefs::records(); i.isValid(); ++ i )
	{
//		if( i->getTankLayoutCID()== rackLayoutID && i->getSectionPrefix()==pTsrs.section)
		if( i->getTankLayoutCID()== rackLayoutID && i->getFillOrder()==pTsrs.fillOrder)
		{
			if( pTsrs.rack >= i->getFirstRack() && pTsrs.rack <= i->getLastRack() )
			{
				return true;
			}
		}
	}
	return false;
}

//---------------------------------------------------------------------------
// set the current rack give a tank and section
//---------------------------------------------------------------------------

bool TankEngine::setRack( int pRack )
{
	if(! dataBuilt ){return false; }
	if( workingTSRSExt.fillOrder == BRUNKNOWN_INT ) {return false; }

	workingTSRSExt.rack = BRUNKNOWN_INT;
	workingTSRSExt.slot = BRUNKNOWN_INT;

	sli.rackSlots= BRUNKNOWN_INT;

	for( Range< LCDbSectionDef > i = LCDbSectionDefs::records(); i.isValid(); ++ i )
	{
		if( i->getTankLayoutCID()== sli.rackLayoutID )
		{
			if( pRack >= i->getFirstRack() && pRack <= i->getLastRack() )
			{
				workingTSRSExt.rack = pRack;
				sli.rackSlots = i->getRackCapacity();
				return true;
			}
		}
	}
	return false;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
bool TankEngine::testSlot( const TankSectionRackSlot&  pTsrs )
{
	if(! dataBuilt ){return false; }
	if( pTsrs.rack == BRUNKNOWN_INT ) { return false; }

	int rackLayoutID=	getRackLayoutID( pTsrs.tank );

	if( rackLayoutID == BRUNKNOWN_INT) {return false; }

	for( Range< LCDbSectionDef > i = LCDbSectionDefs::records(); i.isValid(); ++ i )
	{
		if( i->getTankLayoutCID()== rackLayoutID )
		{
			if( pTsrs.slot >= 1 && pTsrs.slot <= i->getRackCapacity() )
			{
				return true;
			}
		}
	}
	return false;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

bool TankEngine::setSlot( int pSlot )
{
	if(! dataBuilt ){return false; }
	if( workingTSRSExt.rack == BRUNKNOWN_INT ) { return false; }

	workingTSRSExt.slot	 = BRUNKNOWN_INT;

	for( Range< LCDbSectionDef > i = LCDbSectionDefs::records(); i.isValid(); ++ i )
	{
		if( i->getTankLayoutCID()== sli.rackLayoutID )
		{
			if( pSlot >= 1 && pSlot <= i->getRackCapacity() )
			{
				workingTSRSExt.slot = pSlot;
				return true;
			}
		}
	}
	return false;
}

//---------------------------------------------------------------------------
//  Checking to see if the current last is the last in the tank.
// Return true if a forced re-selection is required.
// return false otherwise
//---------------------------------------------------------------------------


bool TankEngine::CheckConfirmEndOfTank(const TankSectionRackSlot&  pTsrs)
{
	if(! dataBuilt ){return false; }
	if( pTsrs.tank == BRUNKNOWN_INT || pTsrs.fillOrder== BRUNKNOWN_INT ) { return false; }

	int NoSections;
	int rackLayoutID;

// Check to see if it is the last slot in the tank
// find out how many sections are in the tank first of all
	for( itrTankInfoMap ti= tankInfos.begin(); ti != tankInfos.end() ; ti++ )
	{
		if( ti->second.tankExt== pTsrs.tank)
		{
			NoSections= ti->second.sectionCount;
			rackLayoutID = ti->second.rackLayoutCID;
		}
	}

// Is this the last section in the tank
	for( Range< LCDbSectionDef > i = LCDbSectionDefs::records(); i.isValid(); ++ i )
	{
//		if( i->getTankLayoutCID()== rackLayoutID  && pTsrs.section.find( i->getSectionPrefix())!=std::string::npos )
		if( i->getTankLayoutCID()== rackLayoutID  && pTsrs.fillOrder == i->getFillOrder())
		{
			if( i->getFillOrder() == NoSections)
			{
				// Found the section and it is the last.
				// is it the lasst slot in the rack.
				if( pTsrs.rack == i->getLastRack() && pTsrs.slot == i->getRackCapacity() )
				{
					return true;
				}
			}
		}
	}

	return false;
}
//---------------------------------------------------------------------------
//  Checking to see if a forced re-selection of a slot is required.
// Return true if a forced re-selection is required.
// return false otherwise
//---------------------------------------------------------------------------

bool TankEngine::checkConfirmSlot ( const TankSectionRackSlot&  pTsrs )
{
	if(! dataBuilt ){return false; }
	if( pTsrs.tank == BRUNKNOWN_INT || pTsrs.fillOrder== BRUNKNOWN_INT ) { return false; }

	int NoSections;
	int rackLayoutID;

// Check to see if it is the last slot in the tank
// find out how many sections are in the tank first of all
	for( itrTankInfoMap ti= tankInfos.begin(); ti != tankInfos.end() ; ti++ )
	{
		if( ti->second.tankExt== pTsrs.tank)
		{
			NoSections	 = ti->second.sectionCount;
			rackLayoutID = ti->second.rackLayoutCID;
		}
	}

// Is this the last section in the tank
	if( pTsrs.fillOrder==NoSections )
	{
		for( Range< LCDbSectionDef > i = LCDbSectionDefs::records(); i.isValid(); ++ i )
		{
			if( i->getTankLayoutCID()== rackLayoutID && pTsrs.fillOrder== i->getFillOrder() )
//		if( i->getRackLayoutCID()== rackLayoutID && pTsrs.section.Pos( i->getSectionPrefix())!=0 )
			{
				// Found the section and it is the last.
				// is it the lasst slot in the rack.
				if( pTsrs.rack == i->getLastRack() && pTsrs.slot == i->getRackCapacity() )
				{
					return true;
				}
			}
		}
	}

	if( rackLayoutID == BRUNKNOWN_INT) {return false; }

//	std::string pSection;
//	pSection=pTsrs.section;

// Search through the tank layout to find a section
	for( Range< LCDbSectionDef > i = LCDbSectionDefs::records(); i.isValid(); ++ i )
	{
		if( i->getTankLayoutCID()== rackLayoutID && pTsrs.fillOrder== i->getFillOrder() )
//		if( i->getRackLayoutCID()== rackLayoutID && pSection.Pos( i->getSectionPrefix())!=0 )
		{
			if(	i->getConfirmRack() == 1 )
			{
// The location has to be re_selected after the last slot of each rack.
				if(  i->getRackCapacity()== pTsrs.slot )
				{
					return true;
				}else
				{
					return false;
				}
			}else
			{
// The location has to be re_selected on the last slot of the last rack in a section.
				if (i->getLastRack() == pTsrs.rack )
				{
					if(i->getRackCapacity()== pTsrs.slot )
					return true;
				}
			}
		}
	}
	return false;

}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

/*const TSRSExt& TankEngine::getWorkingTSRS(void)
{
	return workingTSRSExt;
} */
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

const TSRSExt& TankEngine::getWorkingTSRSExt(void)
{
	return workingTSRSExt;
}
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------
const TSRSExt& TankEngine::getStoredTSRS(void)
{
	return storedTSRS;
}

//---------------------------------------------------------------------------
// Storing
//---------------------------------------------------------------------------

bool TankEngine::rawStoreTSRS(const TSRSExt & tsrs )
{
	storedTSRS=tsrs;
	return true;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

bool TankEngine::storeTSRS ( const TSRSExt&  pTsrs  )
{
	if(testTankCID (  pTsrs )&& testFillOrderAndSection ( pTsrs )&& testRack(pTsrs ) && testSlot(pTsrs))
	{
	  sli.initialize();

	  if( setTankExt(pTsrs ) )
	  {
//		if( setFillOrderAndSection( pTsrs.fillOrder, pTsrs.section ))
		if( setFillOrderAndSection( pTsrs ))
		{
		  if( setRack(pTsrs.rack ))
		  {
			if( setSlot(pTsrs.slot))
			{
			  selectSlot(pTsrs );
			  return true;
			}else
			{
			  sli.initialize();
			  return false;
			}

		  }else
		  {
			sli.initialize();
			return false;
		  }
		}else
		{
		  sli.initialize();
		  return false;
		}
	  }else
	  {
		sli.initialize();
		return false;
	  }
	}
	return false;
}
//---------------------------------------------------------------------------
// Calculating tank level information using the rack layout CID
// Given a rack layout and calculate the number of sections, the number of racks
// and findout the number of slots in all the racks of that layout.
//---------------------------------------------------------------------------

void TankEngine::countRack(LQuery  dbase, int tankCID, int rackLayout
							,int& sectionCount, int & rackCount,int& slotCapacity)
{
// Counting the number of Racks in the tank
	rackCount=0;
//	availableRacks=0;
	slotCapacity=0;
	sectionCount=0;

	LCDbRacks r1;


	for( Range< LCDbSectionDef > r = LCDbSectionDefs::records(); r.isValid(); ++ r )
	{
		if( r->getTankLayoutCID() == rackLayout)
		{

			int noOfRacks;
			sectionCount++;
			noOfRacks     = r->getLastRack() - r->getFirstRack() + 1 ;
			rackCount    += noOfRacks;
			slotCapacity += noOfRacks * r->getRackCapacity();

	// c_rack_number
			r1.read(dbase,tankCID,rackLayout);
			for ( Range <LCDbRack > r= r1; r.isValid(); ++r )
			{
			}
		}
	}


}

//---------------------------------------------------------------------------
// Check to find out if  a slot is already in use
//---------------------------------------------------------------------------

bool TankEngine::isInUse(const TankSectionRackSlot & tsrs )
{
	CitrTankSectionRackSlotMap ti=slotsMap.find(tsrs);
	return (ti != slotsMap.end() && !(ti->second.empty()));
}

//---------------------------------------------------------------------------
// Save the current slot to permanent memory. This is used so the slot can be
// remembered from one session to the next.
//---------------------------------------------------------------------------

bool TankEngine::selectSlot(const TSRSExt & tsrs )
{
	rawStoreTSRS(tsrs);
	return true;

}

//---------------------------------------------------------------------------
// Retrieve the last slot used. This enables the slot to be remembered from
// session to session.
//---------------------------------------------------------------------------

bool  TankEngine::lastSlot( const TSRSExt & tsrs )
{
	rawStoreTSRS(tsrs);
	return true;
}

//---------------------------------------------------------------------------
// Calculate the next Slot given a Tank/Section/Rack/Slot
// Using the Current selected Tank.
// Returning false forces a selection of a new location.
// false return occcurs when the next section goes out of range.
// false returns occurs when the next slot is occupied.
//
//---------------------------------------------------------------------------

bool  TankEngine::findNextSlot(  TankSectionRackSlot & tsrs )
{

// Is there a currently selected tank to use

	if( sli.rackSlots == BRUNKNOWN_INT ){return false; }

	TankSectionRackSlot t1=tsrs;
//
	if( t1.slot >= sli.rackSlots )
	{
// Move on to the first slot in the next Rack
		t1.slot = 1;
		int lastRack,firstRack;
		lastRack= BRUNKNOWN_INT;
		for( Range< LCDbSectionDef > r = LCDbSectionDefs::records(); r.isValid(); ++ r )
		{
			if( r->getTankLayoutCID() == sli.rackLayoutID  && tsrs.fillOrder == r->getFillOrder() )
			{
				lastRack= r->getLastRack();
				firstRack= r->getFirstRack();
			}
		}

		if( t1.rack >= lastRack )
		{
// All the racks in the section are full move on to the next section.
			t1.rack = firstRack;
			bool isNextSection;		isNextSection = false;
			std::string nextSection;		nextSection = "";
			int nextFillOrder;		nextFillOrder= BRUNKNOWN_INT;

//			isNextSection=incSection(t1.section,nextSection,sli.tankCID);
			isNextSection=incSection(t1.fillOrder,sli.tankCID, nextFillOrder,nextSection,t1.rack);

			if( ! isNextSection )
			{
// All the sections in the tank have been used up. Ask for a new tank.
				return false;
			}
			t1.fillOrder= nextFillOrder;
//			t1.section=nextSection;
		}
		else t1.rack++;		// moved: NG, 19/5/09

	}else
	{
		t1.slot ++;
	}

// The next slot in the storage system has already been taken up.
	if(isInUse(t1))
	{
		return false;
	}else
	{
		tsrs.tank    = t1.tank;
		tsrs.fillOrder = t1.fillOrder;
//		tsrs.section = t1.section;
		tsrs.rack    = t1.rack;
		tsrs.slot	 = t1.slot;
		return true;
	}
}
//---------------------------------------------------------------------------
// Find the next section in the tank. The tank layout records are ordered by
// tank layout and then by fill ordered. The function finds the section
// and then goes to the next record if it is valid it retrieves the record
// and returns the section prefix.
//---------------------------------------------------------------------------
//bool TankEngine::incSection(const String & pSection,String & pNextSection, int pTankCID )
bool TankEngine::incSection(	int pFillOrder,int pTankCID
									,int & pNextFillOrder,std::string & pNextSection
									,int & pFirstRack )
{
// Find the tank details
	itrTankInfoMap ti= tankInfos.find(pTankCID);
	int rackLayoutID;

	if( ti!= tankInfos.end() )
	{
// retreive the rack layout
		rackLayoutID=ti->second.rackLayoutCID;
	}else
	{
		return false;
	}

// Search through the tank layout to find a section

	short int currFill;
	currFill=BRUNKNOWN_INT;

	for( Range< LCDbSectionDef > i = LCDbSectionDefs::records(); i.isValid(); ++ i )
	{
//		if( i->getRackLayoutCID()== rackLayoutID  && pSection.Pos( i->getSectionPrefix())!=0)
		if( i->getTankLayoutCID()== rackLayoutID  && pFillOrder == i->getFillOrder() )
		{
// Drawing all the rack details for the tank
			currFill =i->getFillOrder();
			currFill++;
			for( Range< LCDbSectionDef > j = LCDbSectionDefs::records(); j.isValid(); ++ j )
		//	for( Range< LCDbTankLayout > j = LCDbTankLayouts::records(); j.isValid(); ++ j )
			{
				if( j->getTankLayoutCID()== rackLayoutID && j->getFillOrder() == currFill )
				{
					pNextSection   = j->getSectionPrefix();
					pNextFillOrder = currFill;
					pFirstRack	   = j->getFirstRack();
					return true;
				}
			}
			return false;
		}
	}
	return false;
}


//---------------------------------------------------------------------------
//  Given a tankName return an iterator to that tankn info record.
//---------------------------------------------------------------------------

CitrTankInfoMap TankEngine::getTankInfoExt(int pTankNo)
{


		for ( CitrTankInfoMap ti=tankInfos.begin(); ti != tankInfos.end();ti++)
		{
			if( ti->second.tankExt==pTankNo)
			{
				return ti;
			}
		}
		return tankInfos.end();
}

//---------------------------------------------------------------------------
// Uses external name to retrieve rack layout CID.
//---------------------------------------------------------------------------

int TankEngine::getRackLayoutID(const int& pTankName)
{

	for ( CitrTankInfoMap ti=tankInfos.begin(); ti != tankInfos.end();ti++)
	{
		if( pTankName== ti->second.tankExt)
		{
			return ti->second.rackLayoutCID;
		}
	}
	return BRUNKNOWN_INT;
}

//---------------------------------------------------------------------------
// Uses the tank CID get the tank's external name eg 11,12 ,13...
//---------------------------------------------------------------------------

int TankEngine::getTankExt(int pTankCID)
{
	int tankExt;

	itrTankInfoMap ti= tankInfos.find(pTankCID);
	tankExt=BRUNKNOWN_INT;
	if( ti!= tankInfos.end() )
	{
		tankExt=ti->second.tankExt;
	}
	return tankExt;
}
int TankEngine::getFillOrder(  int pRackLayoutCID, const std::string & pSection, int pRack )
{

	for( Range< LCDbSectionDef > i = LCDbSectionDefs::records(); i.isValid(); ++ i )
	{
		if ( i->getTankLayoutCID() == pRackLayoutCID )
		{
			if( pRack  >= i->getFirstRack() && pRack <= i->getLastRack() )
			{
				if (  pSection.compare(i->getSectionPrefix() ) == 0 )
				{
					return i->getFillOrder();
				}
			}
		}

	}
	return BRUNKNOWN_INT;
}


//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

bool TankEngine::isTankInfoEnd(const CitrTankInfoMap ti )
{
	return ti ==tankInfos.end();
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

CitrTankInfoMap TankEngine::getTankInfoBegin(void)
{
	return tankInfos.begin();
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

CitrTankInfoMap TankEngine::getTankInfoEnd(void)
{
	return tankInfos.end();
}

bool TankEngine::isTankDisplayEnd(const CitrTankDisplayMap ti )
{
	return ti ==tankDisplays.end();
}

CitrTankDisplayMap TankEngine::getTankDisplayMapBegin(void)
{
	return tankDisplays.begin();
}
CitrTankDisplayMap TankEngine::getTankDisplayMapEnd(void)
{
	return tankDisplays.end();
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

/*bool TankEngine::getSectionCount(const std::string& pTankName)
{
		int tankCID;
		int rackLayoutID;
		SectionCount sc;
		TSRLevelSummary ls;

		tankCID=0;
		rackLayoutID=0;

		CitrTankInfoMap ti=getTankInfoExt(pTankName);
		tankCID=ti->first;
		rackLayoutID=ti->second.rackLayoutCID;

		sectionCounts.clear();
		int count;

		char tankNo[10];

// Creating a tank filled with blank racks
	for( Range< LCDbSectionDef > i = LCDbSectionDefs::records(); i.isValid(); ++ i )
	{
		if( i->getTankLayoutCID()== rackLayoutID )
		{
// Drawing all the rack details for the tank
			count=0;

// Count all the used slots in the current selected sector (indexed by fillOrder )
			for(CitrTankSectionRackSlotMap sl=slotsMap.begin();sl!=slotsMap.end();sl++)
			{
				itoa(sl->first.tank ,tankNo,10);
				if( pTankName.compare( std::string(tankNo)) ==0 &&
				sl->first.fillOrder== i->getFillOrder() )
				{
					if (!sl->second.empty() )
					{
						count++;
					}
				}
			}

			sc.tankCID=tankCID;
//			sc.section= i->getSectionPrefix();
			sc.fillOrder= i->getFillOrder();

			ls.section = i->getSectionPrefix();
			ls.rackCapacity= i->getRackCapacity();
			ls.rackCount= i->getLastRack() - i->getFirstRack() +1 ;
			ls.slotCapacity= ls.rackCount * ls.rackCapacity;
			ls.slotsUsed=count;

			sectionCounts.insert(SectionCountMap::value_type(sc,ls));
		}
	}
	return true;
}*/
//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

bool TankEngine::getSectionCount(int pTankCID)
{
		int tankCID;
		int rackLayoutID;
		SectionCount sc;
		TSRLevelSummary ls;

		tankCID=0;
		rackLayoutID=0;

		CitrTankInfoMap ti=getTankInfoExt(pTankCID);
		tankCID=ti->first;
		rackLayoutID=ti->second.rackLayoutCID;

		sectionCounts.clear();
		int count;

		char tankNo[10];

// Creating a tank filled with blank racks
	for( Range< LCDbSectionDef > i = LCDbSectionDefs::records(); i.isValid(); ++ i )
	{
		if( i->getTankLayoutCID()== rackLayoutID )
		{
// Drawing all the rack details for the tank
			count=0;

// Count all the used slots in the current selected sector (indexed by fillOrder )
			for(CitrTankSectionRackSlotMap sl=slotsMap.begin();sl!=slotsMap.end();sl++)
			{

				if( pTankCID==sl->first.tank &&
				sl->first.fillOrder== i->getFillOrder() )
				{
					if (!sl->second.empty() )
					{
						count++;
					}
				}
			}

			sc.tankCID=tankCID;
//			sc.section= i->getSectionPrefix();
			sc.fillOrder= i->getFillOrder();

			ls.section = i->getSectionPrefix();
			ls.rackCapacity= i->getRackCapacity();
			ls.rackCount= i->getLastRack() - i->getFirstRack() +1 ;
			ls.slotCapacity= ls.rackCount * ls.rackCapacity;
			ls.slotsUsed=count;

			sectionCounts.insert(SectionCountMap::value_type(sc,ls));
		}
	}
	return true;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

CitrSectionCountMap TankEngine::getSectionCountBegin(void)
{
	return sectionCounts.begin();
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

CitrSectionCountMap TankEngine::getSectionCountEnd(void)
{
	return sectionCounts.end();
}

//---------------------------------------------------------------------------
// Go through the tank layouts given a external tank name and section,
// Go through all the racks in that section, and for each rack create
// an entry to hold the count of boxes in that rack. Store the rack capacity
// and the count, even if the rack is empty.
// Key: Tank_cid and section.
//---------------------------------------------------------------------------

/*bool TankEngine::getRackCount(const String& pTankName,const String& pSectionName )
{
	int rackLayoutID;
	int sectionSize;

	rackCounts.clear();
	int rc;
// Creating a tank filled with blank racks

	rackLayoutID=0;

	CitrTankInfoMap ti=getTankInfoExt(pTankName);

	rackLayoutID=ti->second.rackLayoutCID;

	for( Range< LCDbTankLayout > l = LCDbTankLayouts::records(); l.isValid(); ++ l )
	{
		if( l->getRackLayoutCID()== rackLayoutID
			&& l->getSectionPrefix().Pos(pSectionName))
		{
			sectionSize= l->getLastRack() - l->getFirstRack() +1;
			for( int i=0; i<sectionSize;i++)
			{
				int	count=0;
				for(CitrTankSectionRackSlotMap sl=slotsMap.begin();sl!=slotsMap.end();sl++)
				{

					if( sl->first.tank== pTankName &&
						sl->first.section == pSectionName &&
						sl->first.rack==String(i+1) )
					{
						if (!sl->second.IsEmpty() )
						{
							count++;
						}
					}

				}

			rc= i+1;

			TSRLevelSummary ls;
			ls.rackCapacity=l->getRackCapacity();
			ls.rackCount=1;
			ls.slotCapacity=l->getRackCapacity();
			ls.slotsUsed=count;
			rackCounts.insert(RackCountMap::value_type(rc,ls));

			}
		}
	}
	return true;
} */


bool TankEngine::getRackCount(int pTankName,int pFillOrder )
{
	int rackLayoutID;
	int sectionSize;

	rackCounts.clear();

// Creating a tank filled with blank racks

	rackLayoutID=0;
//	char tankNo[10];

	CitrTankInfoMap ti=getTankInfoExt(pTankName);

	rackLayoutID=ti->second.rackLayoutCID;

	for( Range< LCDbSectionDef > l = LCDbSectionDefs::records(); l.isValid(); ++ l )
	{
		if( l->getTankLayoutCID()== rackLayoutID
			&& l->getFillOrder() == pFillOrder )
//			&& l->getSectionPrefix().Pos(pSectionName))
		{
			sectionSize= l->getLastRack() - l->getFirstRack() +1;
			int offSet= l->getFirstRack();
			for( int i=offSet; i<(sectionSize+offSet);i++)
			{
				int	count=0;
				for(CitrTankSectionRackSlotMap sl=slotsMap.begin();sl!=slotsMap.end();sl++)
				{


					if( pTankName==sl->first.tank &&
						sl->first.fillOrder == l->getFillOrder() &&
						sl->first.rack == i )
					{
						if (!sl->second.empty() )
						{
							count++;
						}
					}

				}

//			rc= i+1;
//			rc=i;
			TSRLevelSummary ls;
			ls.rackCapacity=l->getRackCapacity();
			ls.rackCount=1;
			ls.slotCapacity=l->getRackCapacity();
			ls.slotsUsed=count;
			rackCounts.insert(RackCountMap::value_type(i,ls));

			}
		}
	}
	return true;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

CitrRackCountMap TankEngine::getRackCountBegin(void)
{
	return rackCounts.begin();
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

CitrRackCountMap TankEngine::getRackCountEnd(void)
{
	return rackCounts.end();
}

//---------------------------------------------------------------------------
// build up a list of boxes by boxname in a specific rack.
//---------------------------------------------------------------------------

bool TankEngine::getSlotContent(
	int pTankName,
	const std::string & pSectionName,
	const std::string & pRackName )
{
	int rackSize;
	slotPositions.clear();

	int lFillOrder;
	rackSize=getRackSize(pTankName,pSectionName,pRackName,lFillOrder);

	for(  int i=1;i<=rackSize;i++)
	{
		slotPositions.insert(SlotPositionsMap::value_type(i,std::string(" ")));
	}

	int slotNo;
	char rackNo[10];

	for(CitrTankSectionRackSlotMap i=slotsMap.begin();i!=slotsMap.end();i++)
	{
	itoa(i->first.rack,rackNo,10);
		if( (pTankName== i->first.tank) &&
//			 pSectionName.compare(i->first.section)==0   &&
			i->first.fillOrder == lFillOrder &&
			pRackName.compare(std::string(rackNo)) ==0 )
		{
			slotNo=i->first.slot;
			if( slotNo >0 && slotNo<=rackSize)
			{
				slotPositions[slotNo]=i->second;
			}
		}
	}
	return true;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

CitrSlotPositionsMap TankEngine::getSlotPositionsBegin(void)
{
	return slotPositions.begin();
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

CitrSlotPositionsMap TankEngine::getSlotPositionsEnd(void)
{
	return slotPositions.end();
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

int TankEngine::getRackSize(int pTankName, const std::string & pSectionName
								 ,const std::string & pRackName, int & pFillOrder)
{
	int rackLayoutID;

	rackLayoutID=0;

	CitrTankInfoMap ti=getTankInfoExt(pTankName);
	if ( !isTankInfoEnd( ti ))
	{
		rackLayoutID=ti->second.rackLayoutCID;
	}

	int rackNo=toPosIntDef(pRackName,-1);



	for( Range< LCDbSectionDef > l = LCDbSectionDefs::records(); l.isValid(); ++ l )
	{
	 bool t;
	 t=l->getSectionPrefix().find(pSectionName)==std::string::npos;
		if( l->getTankLayoutCID()== rackLayoutID
				&& l->getSectionPrefix().find(pSectionName)!=std::string::npos
				&& rackNo >= l->getFirstRack() && rackNo <=l->getLastRack())
		{
			pFillOrder= l->getFillOrder();
			return l->getRackCapacity();
		}
	}
	return 0;
}

//---------------------------------------------------------------------------
//
//---------------------------------------------------------------------------

TankEngine::TankEngine()
{
}

//---------------------------------------------------------------------------

#pragma package(smart_init)
