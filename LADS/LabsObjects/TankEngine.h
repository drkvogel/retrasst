//---------------------------------------------------------------------------

#ifndef TankEngineH
#define TankEngineH

#include "BRType.h"
#include "LQuery.h"


#define     BA_KEY_NAME			"BoxReception"

#define     NEXT_SLOT_ID 		"Next Slot ID"
#define     NEXT_RACK_ID 		"Next Rack ID"
#define		NEXT_SECTION_ID 	"Next Section ID"
#define     NEXT_TANK_ID 		"Next Tank ID"
//---------------------------------------------------------------------------
class TankEngine
{
// Tank Engine is a singleton used to hand out Next slots based on tank layouts
	public:
		static TankEngine *Initialize();
		static bool isTankEngine( void ){return instance!= 0; }
		 bool buildData( LQuery  dbase );
		 bool rebuildTankData( void );
		 bool addBoxEntry(	const TankSectionRackSlot & tsrs,const std::string & boxName );

		 bool isDataBuilt(void) const;

		 bool testTankCID (  const TankSectionRackSlot&  tsrs ) ;
		 bool testFillOrderAndSection ( const TankSectionRackSlot&  tsrs );
		 bool testRack( const TankSectionRackSlot&  tsrs );
		 bool testSlot( const TankSectionRackSlot&  tsrs );


//==============================================================
//   Setter functions
//==============================================================
		void resetCurrents(void);
		 bool storeTSRS (  const TSRSExt& pTsrs  );

		 bool setLocation( const int pLocation );
		 bool setTankExt ( const TSRSExt & pTSRSExt  );
//		const bool setFillOrder( const int pFillOrder );
		 bool setFillOrderAndSection ( const TSRSExt & pTSRSExt );
//		const bool setSection ( const String & pSection );
		 bool setRack    ( const int pRack );
		 bool setSlot    ( const int pSlot );

 //		const TankSectionRackSlot& getWorkingTSRS(void);
		const TSRSExt& getWorkingTSRSExt(void);
		const TSRSExt& getStoredTSRS(void);

//==============================================================
//   Getter functions
//==============================================================
//		int getTankSize( void ){return gCurrTankSize;}
		 int getTankExt( const int pTankCID );
		 int getFillOrder(  const int pRackLayoutCID, const std::string & pSection, int pRack );
//==============================================================
//
//==============================================================
		static int getCurrTankCID(void);


		 int getMinTank(void);
		 int getMaxTank(void);
//		const bool getSectionCount( const std::string& pTankName);
		 bool getSectionCount( const int pTankCID);
//		const bool getRackCount( const String& pTankName, const String& pSectionName );
		 bool getRackCount(const int pTankName,const int pFillOrder );
		 bool getSlotContent( const int pTankName, const std::string& pSectionName, const std::string& pRackName );

		CitrTankInfoMap getTankInfoBegin(void);
		CitrTankInfoMap getTankInfoEnd(void);

		CitrTankDisplayMap getTankDisplayMapBegin(void);
		CitrTankDisplayMap getTankDisplayMapEnd(void);
		 bool isTankDisplayEnd(const CitrTankDisplayMap ti );

		CitrSectionCountMap getSectionCountBegin(void);
		CitrSectionCountMap getSectionCountEnd(void);

		CitrRackCountMap getRackCountBegin(void);
		CitrRackCountMap getRackCountEnd(void);

		CitrSlotPositionsMap getSlotPositionsBegin(void);
		CitrSlotPositionsMap getSlotPositionsEnd(void);

		CitrTankInfoMap getTankInfoExt(const int pTankNo);
		 int getRackLayoutID(const int &pTankName);
		 bool isTankInfoEnd(const CitrTankInfoMap ti );

		 bool isInUse(const TankSectionRackSlot & tsrs ) ;
		 bool selectSlot(const TSRSExt & tsrs );
		 bool lastSlot(  const TSRSExt & tsrs );
		 bool findNextSlot(  TankSectionRackSlot & tsrs);
		 bool checkConfirmSlot ( const TankSectionRackSlot&  pTsrs );
		 bool CheckConfirmEndOfTank(const TankSectionRackSlot&  pTsrs);

//==============================================================
//		General functions which can be used by users of tank engine.
//==============================================================
		int toPosIntDef( const std::string & sVal,int def );
		int toIntDef( const std::string & sVal,int def );
		double toPosDoubleDef( const std::string & sVal,double def);

	protected:
		TankEngine();
	private:
// Stores the TSRS selected
		static TSRSExt storedTSRS;
// Stores detail as you go from Tank to Section to Rack to Slot.
//		static TankSectionRackSlot workingTSRS;
// Extended version
		static TSRSExt workingTSRSExt;

// StoredLocationInfo is a structure containing summary infomation on the
// current selected tank/section/rack/slot. Used as quick reference in tank engine.
		class StoredLocationInfo
		{
			public:
				void initialize(void);
				int    tankCID;		 // The cid of the current selected Tank.
				int    tankSections; // Total number of Sections in the cerrent selceted tank.
				int    tankRacks;	 // Total number of rack in the current selected tank.
				int    sectionRacks; // Number of racks in the current selected section.
				int    rackSlots;	 // Number of slots in the current selected rack.
				int    rackLayoutID; // RackLayout ID used
//				int    fillOrder;	 // Fill order of the current section used to positively identify them
		};

//==============================================================
//   Private Members
//==============================================================
		static TankEngine* instance;
		static TankInfoMap tankInfos; // Summary of tank infomation
		static TankDisplayMap tankDisplays;

		 // sectionCounts - Summary of Slots avaliable and used
		 // with a key of tankCID and section name.
		static SectionCountMap sectionCounts;

		 // rackCounts - Summary of Slots avaliable and used
		 // with a key of tankCID and section name.
		static RackCountMap rackCounts;
		static SlotPositionsMap slotPositions;
		static TankSectionRackSlotMap slotsMap;
//		static MapIntInt tanksMap;

//==============================================================
//  Current selected location/tank/section/rack/slot.
//==============================================================
		static int    gCurrLocationCID;

		static StoredLocationInfo sli;

		static bool	  dataBuilt;
		static int minTank;
		static int maxTank;

//==============================================================
// Helper functions - private member functions
//==============================================================

		 bool rawStoreTSRS(const TSRSExt& tsrs );

		void countRack(LQuery  dbase,const int tankCID,const int rackLayout,int& sectionCount, int & rackCount,int& rackCapacity);
		 int  getRackSize(const int pTankName, const std::string & pSectionName
								, const std::string & pRackName, int & pFillOrder);

//		const bool incSection(const String & pSection, String & pNextSection,const int pTankCID );
		 bool incSection(	const int pFillOrder,const int pTankCID
								, int & pNextFillOrder,std::string & pNextSection
								,int & pFirstRack );
	  	const char * TrimZeroC( const char * leadZeroStr );
		int charToIntDef( const char * in , int def, bool sign) ;


};
//---------------------------------------------------------------------------
#endif
