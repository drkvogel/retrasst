//---------------------------------------------------------------------------

#ifndef LCDbBoxCourierH
#define LCDbBoxCourierH

#include "LSDbBox.h"
#include "LDbIdClasses.h"
#include "LDbCacheBase.h"
#include "LCDbObject.h"

class RIJNDAEL;
class LQuery;

enum ArrivalStatus {MATCHED,  MATCH_DUPLICATE,MATCH_REFERRED, UNEXPECTED, UNKNOWN };

class ColourStatus
{
	public:
		void getColours( ArrivalStatus ms,String & pTextStatus,TColor& c, TColor& tc ) ;
		// c - colour of cell, tc - text color
};

class LSDbBoxCourier :  public LSDbBox
{
		public:
				LSDbBoxCourier( void )
				 : LSDbBox()
				{}

				LSDbBoxCourier( const String & name )
				 : LSDbBox(name)
				{}
				
				LSDbBoxCourier( const LQuery & query );

				const String & getFirstBarcode  ( void )const {return first_barcode ;}
				void setFirstBarcode( const String & x){ first_barcode=x; }

				const String & getLastBarcode ( void )const  {return last_barcode ;}
				void setLastBarcode( const String & x ){ last_barcode=x; }

				const int   getProjectCid( void ) const {return project_cid; }
				void setProjectCid ( const int x){project_cid=x; }

				void use( bool pUse ) { used=pUse;}
				const bool isUsed( void ) const { return used; }

				void setFirst( short x ) {first=x;}
				void setLast(  short x ) {last=x; }

				static const String ALLOCATED_BOX;
				static const String CONFIRMED_ALLOCATION;
				static const String REFERRED_ALLOCATION;

				enum BoxEvents {AllocatedBox,ConfirmedAllocation,ReferredAllocation};

	   protected:
				bool used;
				int project_cid;
				String first_barcode;
				String last_barcode;
};

// May not need

class LDbBoxExpected : public LSDbBoxCourier, public LPDbID
{
	   public:
				LDbBoxExpected( const LQuery & query );

				bool saveRecord( LQuery bQuery );
};

// class to handle records in boxreception
class LDbBoxExpecteds : public LDbCache< LDbBoxExpected >, public LPDbCacheMap< LDbBoxExpecteds >
{
		class Matcher;

	public:

		static void readAll( LQuery central );

		const LDbBoxExpected * find( const int projID
			, const String & pBoxName
			,const String & pCryo1
			,const String & pCryo2
			,const int pCryoPos1
			,const int pCryoPos2 ) const;

		void updateRecords( LQuery central );
};

class LPDbBoxEvent : public LSDbBoxEvent
{
	protected:

		int boxID;
		String text;

		LPDbBoxEvent( const LQuery & query ) : LSDbBoxEvent( query )
		{
			boxID = 0;
		}

	public:

		LPDbBoxEvent( const LPDbBoxEvent & other )
		 : boxID( other.boxID ), LSDbBoxEvent( other )
		{}

		bool saveRecord( LQuery pQuery );
};

class LDbBoxArrivalEvent : public LPDbBoxEvent
{
		int projectCid;

	public:

		LDbBoxArrivalEvent( const LQuery & query );

		LDbBoxArrivalEvent( const LDbBoxArrivalEvent & other )
		 : projectCid( other.projectCid ), LPDbBoxEvent( other )
		{}

		bool saveRecord( LQuery bQuery );
};


// ***************************************************
// Class to handle LCDbBoxArrival
// ***************************************************

class LDbBoxArrival : public LSDbBoxCourier , public LCDbID
{
	   public:

				LDbBoxArrival( const String & boxName )
				 : LSDbBoxCourier( boxName ), laptop_cid( 0 ), process_cid( 0 ), tank_cid( 0 ), slotPosition( 0 )
				{}

				LDbBoxArrival( int projectID, int boxID ) : LCDbID( boxID )
				{
					setProjectCid( projectID );
				}

				LDbBoxArrival( const LQuery & query );

				void setLaptopCid( int x ) { laptop_cid=x; }
				int getLaptopCid( void ) {return laptop_cid;}

				void setProcessCid( int x ) { process_cid=x; }
				int getProcessCid( void ) {return process_cid;}

				void setTankCid( const int x ) {tank_cid=x;}
				const int  getTankCid(void )const { return tank_cid; }

				void setRackNumber( const String &x ){ rack_number=x;}
				const String & getRackNumber( void )const {return rack_number; }

				void setSlotPosition( const int x ){slotPosition=x;}
				const int getSlotPosition(void )const{return slotPosition; }

				void setSwipeTime( const TDateTime x ) { swipe_time=x; }
				const TDateTime getSwipeTime( void ) const{ return swipe_time; }

				void setStatus( const int x ) { status =x; }
				const int getStatus(void) const { return status;}

				void setBev( const BoxEvents x ) { bev=x;}
				const BoxEvents getBev( void )const  {return bev ;}

				void setMatchStatus( const ArrivalStatus x ) { boxEntryMatch =x; }
				const ArrivalStatus getMatchStatus(void) const { return boxEntryMatch;}

				bool saveRecord( LQuery pQuery );
				void createId( LQuery project );
				void addLinkedEvent( LQuery central, const String & eventCID );

				Range< LDbBoxArrivalEvent > readBoxHistory( LQuery lQuery );

	   protected:
				int laptop_cid;
				int process_cid;
				int tank_cid;
				String rack_number;
				int slotPosition;
				TDateTime swipe_time;
				int status;
				ArrivalStatus boxEntryMatch;
				BoxEvents bev;
};

// class to handle records in laptop central
class LDbBoxArrivals : public LDbCache< LDbBoxArrival >, LPDbCacheMap< LDbBoxArrivals >
{
		class Matcher;

	public:

		static void readAll( LQuery project );

		const LDbBoxArrival * find( const int pProjectCid, const String & pBoxName
			,const String & pCryo1
			,const String & pCryo2
			,const int pCryoPos1
			,const int pCryoPos2 ) const;
};

//---------------------------------------------------------------------------
#endif
