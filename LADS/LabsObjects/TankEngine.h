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
		static const bool isTankEngine( void ){return instance!= 0; }
		const bool buildData( LQuery  dbase );
		const bool rebuildTankData( void );
		const bool addBoxEntry(	const TankSectionRackSlot & tsrs,const std::string & boxName );

		const bool isDataBuilt(void) const;

		const bool testTankCID (  const TankSectionRackSlot&  tsrs ) ;
		const bool testFillOrderAndSection ( const TankSectionRackSlot&  tsrs );
		const bool testRack( const TankSectionRackSlot&  tsrs );
		const bool testSlot( const TankSectionRackSlot&  tsrs );


//==============================================================
//   Setter functions
//==============================================================
		void resetCurrents(void);
		const bool storeTSRS (  const TankSectionRackSlot& pTsrs  );

		const bool setLocation( const int pLocation );
		const bool setTankExt ( const std::string & pTankCID  );
//		const bool setFillOrder( const int pFillOrder );
		const bool setFillOrderAndSection ( const int pFillOrder,const std::string & pSection );
//		const bool setSection ( const String & pSection );
		const bool setRack    ( const int pRack );
		const bool setSlot    ( const int pSlot );

		const TankSectionRackSlot& getWorkingTSRS(void);
		const TankSectionRackSlot& getStoredTSRS(void);

//==============================================================
//   Getter functions
//==============================================================
//		int getTankSize( void ){return gCurrTankSize;}
		const int getTankExt( const int pTankCID );
		const int getFillOrder(  const int pRackLayoutCID, const std::string & pSection, int pRack );
//==============================================================
//
//==============================================================
		static int getCurrTankCID(void);


		const int getMinTank(void);
		const int getMaxTank(void);
		const bool getSectionCount( const std::string& pTankName);
//		const bool getRackCount( const String& pTankName, const String& pSectionName );
		const bool getRackCount(const std::string& pTankName,const int pFillOrder );
		const bool getSlotContent( const std::string& pTankName, const std::string& pSectionName, const std::string& pRackName );

		CitrTankInfoMap getTankInfoBegin(void);
		CitrTankInfoMap getTankInfoEnd(void);

		CitrTankDisplayMap getTankDisplayMapBegin(void);
		CitrTankDisplayMap getTankDisplayMapEnd(void);
		const bool isTankDisplayEnd(const CitrTankDisplayMap ti );

		CitrSectionCountMap getSectionCountBegin(void);
		CitrSectionCountMap getSectionCountEnd(void);

		CitrRackCountMap getRackCountBegin(void);
		CitrRackCountMap getRackCountEnd(void);

		CitrSlotPositionsMap getSlotPositionsBegin(void);
		CitrSlotPositionsMap getSlotPositionsEnd(void);

		CitrTankInfoMap getTankInfoExt(const std::string& pTankName);
		const int getRackLayoutID(const std::string &pTankName);
		const bool isTankInfoEnd(const CitrTankInfoMap ti );

		const bool isInUse(const TankSectionRackSlot & tsrs ) ;
		const bool selectSlot(const TankSectionRackSlot & tsrs );
		const bool lastSlot(  TankSectionRackSlot & tsrs );
		const bool findNextSlot(  TankSectionRackSlot & tsrs);
		const bool checkConfirmSlot ( const TankSectionRackSlot&  pTsrs );
		const bool CheckConfirmEndOfTank(const TankSectionRackSlot&  pTsrs);

//==============================================================
//		General functions which can be used by users of tank engine.
//==============================================================
		int toPosIntDef( const std::string & sVal,int def );
		int toIntDef( const std::string & sVal,int def );
		double toPosDoubleDef( const std::string & sVal,double def);

	protected:
		TankEngine();
	private:
		static TankSectionRackSlot storedTSRS;
		static TankSectionRackSlot workingTSRS;

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

		const bool rawStoreTSRS(const TankSectionRackSlot& tsrs );

		void countRack(const int rackLayout,int& sectionCount, int & rackCount,int& rackCapacity);
		const int  getRackSize(const std::string & pTankName, const std::string & pSectionName
								, const std::string & pRackName, int & pFillOrder);

//		const bool incSection(const String & pSection, String & pNextSection,const int pTankCID );
		const bool incSection(	const int pFillOrder,const int pTankCID
								, int & pNextFillOrder,std::string & pNextSection
								,int & pFirstRack );
	  	const char * TrimZeroC( const char * leadZeroStr );
		int charToIntDef( const char * in , int def, bool sign) ;


};
//---------------------------------------------------------------------------
#endif
