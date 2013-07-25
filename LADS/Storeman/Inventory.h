//---------------------------------------------------------------------------

#ifndef InventoryH
#define InventoryH

#include <string>
#include <vector>
#include <set>
#include <map>
#include <memory>
#include <algorithm>
#include "rosetta.h"

class LCDbCryoJob;
class Layout;

// Ingres, C, XTIME and the labs databases understand recent dates
static const TDate EPOCH_START( 1980, 1, 1 ), EPOCH_END( 2037, 12, 31 );

//---------------------------------------------------------------------------

struct IPart
{
		enum ItemType { UnknownType = -1, SitesType = 0, SiteType, TankType, LayoutType,
						SectionType, RackType, BoxType, SampleType, NUM_TYPES };

		enum Availability { UNAVAILABLE, IS_AVAILABLE, IS_EMPTY, PART_FULL, IS_FULL, UNKNOWN = -1 };

		enum ItemState { UnknownState = -1, SyncState, NewState, DirtyState, NUM_STATES };

		IPart();
		IPart(int p_id, std::string p_name);
		virtual ~IPart();
		void clearList();

		int getID() const { return id; }
		void setID( int p_id ) { id = p_id; }

		ItemType getType() const { return type; }
		virtual const char * getTypeStr() const { return "Error"; }

		virtual Availability availability() const;

		void setName( std::string p_name ) { name = p_name; }
		virtual std::string getName() const { return name; }
		virtual std::string getFullName() const { return name; }

		IPart* getMapped() const { return mapped2; }
		void setMapped( IPart* p_part ) { mapped2 = p_part; }

		IPart* getParent() const { return parent; }
		void setParent( IPart* p_parent ){ parent = p_parent; }

		bool isNew() { return state == NewState; }
		void setState( ItemState p_state ) { state = p_state; }

		short getCapacity() const { return capacity; }
		void setCapacity( int p_capacity ){ capacity = p_capacity; }
		short getPosition() const { return position; }
		void setPosition( short pos ) { position = pos; }
		short getChildCount() const { return childCount; }

		virtual void populate() = 0;
		virtual void loadParent() {};

		virtual bool save() { return true; }
		// virtual bool addToLHSJobList(int jobID) { return true; }
		// virtual bool addToRHSJobList(int jobID) { return true; }
		// virtual bool occupy() { return true; }
		virtual bool canMove() const { return false; }

		virtual bool operator<( const IPart& rhs ) const { return position < rhs.position; }
		static bool less_than( const IPart* lhs, const IPart* rhs ){ return *lhs < *rhs; }
		void sortChildren() { std::sort( partlist.begin(), partlist.end(), IPart::less_than ); }

		virtual void addToList( IPart* part );
		virtual void remove( int row );
		virtual void discardList();
		const std::vector<IPart*> & getList() const { return partlist; }

		virtual std::auto_ptr<ROSETTA> getProperties(); //to be used for showing details to user
		virtual std::auto_ptr<ROSETTA> getObjectData(); //to be used by save methods

	protected:

		int id;
		std::string name;
		IPart* parent;
		short capacity, position, childCount;
		std::vector<IPart*> partlist;
		std::map< int, std::string > history;
		IPart* mapped2;
		ItemState state;
		ItemType type;
};

class Sites : public IPart 		// not in the database
{
	public:

		Sites() : IPart() { type = SitesType; }
		Sites( int p_id, std::string p_name) : IPart(-1, "Sites") { type = SitesType; }

		virtual const char * getTypeStr() const { return "Sites"; }
		virtual std::string getName() const { return "Site List"; }

		virtual void populate();
		virtual std::auto_ptr<ROSETTA> getProperties();
};

class Site : public IPart 		// id = location_cid; name = c_object_name.external_name
{
	protected:
		std::string fullname;

	public:

		Site() : IPart() { type = SiteType; }
		Site( int p_id, std::string p_name);
		Site( const ROSETTA & data );

		virtual const char * getTypeStr() const { return "Site"; }

		void setFullName( std::string p_fullname ){ fullname = p_fullname; }
		virtual std::string getFullName() const { return fullname; }

		virtual bool operator<( const IPart& rhs ) const;
		virtual void populate();
		virtual std::auto_ptr<ROSETTA> getProperties();
		virtual std::auto_ptr<ROSETTA> getObjectData();
		virtual bool save();
};

class Tank : public IPart 		// Vessel: id = storage_cid, name = c_object_name.external_full
{
	protected:

		std::string srlno;

		// from c_tank_map for the currently selected population
		int store_type_cid, location_cid;
		int tank_cid, map_cid, layout_cid;
		short shelf_number;
		std::string label, description;
		short status;
//		TDateTime valid_from, valid_to;

	public:

		Tank();
		Tank( int p_id, std::string p_name);
		Tank( const ROSETTA & data );
		void setFields( const ROSETTA & data );
		void setFields( const Layout &data );

		virtual const char * getTypeStr() const { return "Vessel"; }
		virtual std::string getName() const;
		virtual std::string getFullName() const;

		const std::string& getVessel() { return name; }
		const std::string& getSrlno() { return srlno; }
		void setSrlno( const std::string& p_srlno ){ srlno = p_srlno; }
		int getLocationID() const { return location_cid; }
		void setLocationID( int p_location_cid ){ location_cid = p_location_cid; }
		int getStoreType() const { return store_type_cid; }
		void setStoreType( int obj_cid ) { store_type_cid = obj_cid; }

		int getLayoutID() const { return layout_cid; }
		int getMapCID() const { return map_cid; }
		int getTankCID() const { return tank_cid; }
		void setContent( const std::string& name, const std::string& full );
		const std::string& getContent() const { return label; }
		const std::string& getDescription() const { return description; }

		short getShelfNumber() const { return shelf_number; }
		void setShelfNumber( short pop ) { shelf_number = pop; }
		virtual Availability availability() const;
		void setAvailability( bool online );

		void takeOffline();
		bool modifySerial();
		bool modifyLayout();
		void loadTankDetails();

		virtual void populate();
		virtual std::auto_ptr<ROSETTA> getProperties();
		virtual std::auto_ptr<ROSETTA> getObjectData();
		virtual bool save();
		virtual bool operator<( const IPart& rhs ) const;
};

class Layout : public IPart 	// Population: id = tank map record id, name = external_name from tank cid
{
		std::string getDateRange() const;

	protected:
		int location_cid, tank_cid, layout_cid;
		int store_type_cid;
		std::string layout_name, layout_description;
		std::string fullname;   	// external full from tank_cid
		short status;
		TDateTime valid_from, valid_to;
		short tank_position;		// IPart::position = shelf number

	public:

		Layout();
		Layout( int p_id, std::string p_name);
		Layout( const ROSETTA & data );

		virtual const char * getTypeStr() const { return "Population"; }

		virtual std::string getName() const;
		virtual std::string getFullName() const;

		int getLocation_cid() const { return location_cid; }
		int getStore_type_cid() const { return store_type_cid; }
		int getLayout_cid() const { return layout_cid; }
		int getTank_cid() const { return tank_cid; }
		void setTank_cid( int p_tank_cid ){ tank_cid = p_tank_cid; }
		short getTankPos() const { return tank_position; }

		void setLayoutType( const std::string & p_name, const std::string & p_desc );
		const std::string & getLayoutName() const { return layout_name; }
		const std::string & getLayoutDescription() const { return layout_description; }
		const std::string & getPopulationName() const { return name; }
		const std::string & getPopulationDescription() const { return fullname; }

		TDateTime getValidFrom() const { return valid_from; }
		void setValidFrom( TDateTime p_valid_from ){ valid_from = p_valid_from; }

		TDateTime getValidUntil() const { return valid_to; }
		void setValidUntil( TDateTime p_valid_to ){ valid_to = p_valid_to; }

		virtual Availability availability() const;
		void setAvailability( bool online );
		void saveAvailability();
		virtual bool canMove() const { return true; }

		virtual bool operator<( const IPart& rhs ) const;

		virtual void populate();
		virtual std::auto_ptr<ROSETTA> getProperties();
		virtual std::auto_ptr<ROSETTA> getObjectData();
		virtual bool save();
};

class Rack;

class Section : public IPart	// id = rack_type_cid; name = section_prefix
{
	protected:
		int rack_layout_cid;
		int tank_cid, map_cid;
		short first, last;
		short rackSize;

	public:

		Section();
		Section( int p_id, std::string p_name);
		Section( const ROSETTA & data );

		virtual const char * getTypeStr() const { return "Section"; }
		virtual std::string getFullName() const;

		int getTank_cid() const { return tank_cid; }
		void setTank_cid( int p_tank_cid ){ tank_cid = p_tank_cid; }

		int getMap_cid() const { return map_cid; }
		void setMap_cid( int p_map_cid ){ map_cid = p_map_cid; }

		short getRackCapacity() const { return rackSize; }
		void setRackCapacity( short p_capacity ){ rackSize = p_capacity; }

		short getFirstRack() const { return first; }
		void setFirstRack( short p_first ){ first = p_first; }

		short getLastRack() const { return last; }
		void setLastRack( short p_last ){ last = p_last; }

		virtual bool canMove() const { return childCount > 0; }
		virtual void populate();
		void populate( const std::multimap< short, Rack * > & dbRacks );

		virtual Availability availability() const;
		virtual std::auto_ptr<ROSETTA> getProperties();
		virtual std::auto_ptr<ROSETTA> getObjectData();
		virtual bool save();
};

class Rack : public IPart  	// Structure: id = rack_cid; name = c_rack_number.external_name
{
	protected:
		int tank_cid;
		int map_cid;
		int rack_type_cid;  // record_cid in c_tank_layout
		int project_cid;	// -1 if unknown; 0 if many projects
		short emptySlots;	// -1 if not yet known; 0 if full
		int box_type_cid;  	// set if all boxes the same
		short posInTank;

	public:

		Rack();
		Rack( int p_id, std::string p_name );
		Rack( const ROSETTA & data );

		virtual const char * getTypeStr() const { return "Structure"; }
		virtual std::string getFullName() const;

		int getTank_cid() const { return tank_cid; }
		void setTank_cid( int p_tank_cid ){ tank_cid = p_tank_cid; }
		int getType_cid() const { return rack_type_cid; }
		int getMap_cid() const { return map_cid; }
		void setMap_cid( int p_map_cid ){ map_cid = p_map_cid; }

		bool isSingleProject() const { return project_cid  < -1; }
		int getProject_cid() const { return project_cid; }
		void setProject_cid( int p_project_cid ){ project_cid = p_project_cid; }

		virtual void populate();
		virtual bool canMove() const { return childCount > 0; }
		short getPosInTank() const { return posInTank; }
		void setPosInTank( short pos ) { posInTank = pos; }
		short getEmptySlots() const { return emptySlots; }
		void setEmptySlots( short slots ) { emptySlots = slots; }

		virtual Availability availability() const;
		virtual std::auto_ptr<ROSETTA> getProperties();
		virtual std::auto_ptr<ROSETTA> getObjectData();
		virtual bool save();
};

class Box : public IPart	// Formation: id from box_name/store; name = box_name.external_name
{
   protected:
		int record_id;      // from latest box_store
		int rack_cid;
		int retrieval_cid;
		int project_cid;
		int box_type_id;
		short status;
		int emptySlots;		// -1 if not yet known; 0 if full

	public:

		Box();
		Box( int p_id, std::string p_name);
		Box( const ROSETTA & data );

		virtual const char * getTypeStr() const { return "Box"; }
		virtual std::string getName() const;
		virtual std::string getFullName() const;

		int getRackCID() const { return rack_cid; }
		void setRackCID( int p_rack_cid ){ rack_cid = p_rack_cid; }

		int getRetrievalCID() const { return retrieval_cid; }
		void setRetrievalCID( int p_retrieval_cid ){ retrieval_cid = p_retrieval_cid; }

		int getProjectCID() const { return project_cid; }
		void setProjectCID( int p_project_cid ){ project_cid = p_project_cid; }

		int getBoxTypeCID() const { return box_type_id; }
		void setBoxTypeCID( int p_type_id ){ box_type_id = p_type_id; }

		int getStatus() const { return status; }
		void setStatus( int p_status ){ status = p_status; }

		virtual void populate();
		virtual std::auto_ptr<ROSETTA> getProperties();
		virtual std::auto_ptr<ROSETTA> getObjectData();
		virtual Availability availability() const;

		bool canMove() const;
		bool isLHSAssigned() const;
		bool isRHSAssigned() const;
		bool isLHSDone() const;
		bool isRHSDone() const;

		virtual void loadParent();
		virtual bool save();
		virtual bool addToLHSJobList(int jobID);
		virtual bool addToRHSJobList(int jobID);
	//  virtual bool occupy();
		bool signoff();
};

class Sample : public IPart		// id from cryovial/cryovial_store; name = barcode
{
	protected:

		int sample_id;
		XTIME stamp;
		int project_cid;
		std::string source_name, source_barcode;
		int box_id;
		int aliquot_type;

	public:

		Sample();
		Sample( int p_id, std::string p_name);
		Sample( const ROSETTA & data );

		virtual const char * getTypeStr() const { return "Sample"; }
		virtual std::string getFullName() const;

		int getBoxID() { return box_id; }
		void setBoxID( int p_box_cid ){ box_id = p_box_cid; }

		int getAliquot_type() { return aliquot_type; }
		void setAliquot_type( int p_aliquot_type ){ aliquot_type = p_aliquot_type; }

		XTIME getStamp() { return stamp; }
		void setStamp( XTIME p_stamp ) { stamp = p_stamp; }

		virtual Availability availability() const;

		virtual void populate();
		virtual std::auto_ptr<ROSETTA> getProperties();
		virtual std::auto_ptr<ROSETTA> getObjectData();
};

class Layouts
{
		std::vector<Layout*> layouts;

	public:
		Layouts(){};
		~Layouts();

		void clearList();
		Layout * getLayout( int i ) const;
		const std::vector<Layout*> & getList() const { return layouts; }
		void addToList( Layout * lay ) { layouts.push_back( lay ); }
		void discardList() { layouts.clear(); }

		int find( int p_id );
		bool isNameDuplicate( bool full, std::string name );
		void loadAll();
		int getDefaultLayoutId( int store_cid );
		static int getLayoutId( int tank_cid );
};

//=============== Available part classes ===================

class AvlSites : public Sites
{
	public:
		AvlSites() : Sites() {};
		AvlSites( int p_id, std::string p_name) : Sites(-1, "Sites"){};

		virtual void populate();
};

class AvlSite : public Site
{
	public:
		AvlSite( int p_id, std::string p_name) : Site(p_id, p_name){};
		AvlSite( const ROSETTA & data ) : Site( data ) {};

		virtual void populate();
};

class AvlTank : public Tank
{
		short totalSlots, usedSlots;

	public:
		AvlTank( int p_id, std::string p_name)
		 : Tank(p_id, p_name),totalSlots(-1),usedSlots(-1)
		{}
		AvlTank( const ROSETTA & data )
		 : Tank(data),totalSlots(-1),usedSlots(-1)
		{}
		void readRackOccupancy();
		void addRackOccupancy( const AvlTank * other );
		float getFillFactor() const;

		virtual void populate();
};

class AvlLayout : public Layout
{
	public:
		AvlLayout( int p_id, std::string p_name) : Layout( p_id, p_name ){};
		AvlLayout( const ROSETTA & data ) : Layout ( data ) {};

		virtual void populate();
};

class AvlSection : public Section
{
	public:
		AvlSection( int p_id, std::string p_name) : Section(p_id, p_name){};
		AvlSection( const ROSETTA & data ) : Section (data ) {};

		virtual void populate();
		float getFillFactor() const;
};

class AvlRack : public Rack
{
		std::set<int> occupied;

	public:

		AvlRack( int p_id, std::string p_name) : Rack(p_id, p_name){};
		AvlRack( const ROSETTA & data ) : Rack ( data ) {};
		AvlRack( int p_tank_cid, const std::string & p_name, int p_type_cid,
				 int p_map_cid, const std::string & p_sec, short p_capacity, short pos );

		virtual void populate();

		void setOccupancy( std::set<int>& positions );
		const std::set<int>& getOccupancy() const { return occupied; }
		float getFillFactor() const;
};

class AvlBox : public Box
{
	public:

		AvlBox( const ROSETTA & data ) : Box (data ) {};
		AvlBox( int p_rack_cid, std::string p_name );

		virtual void populate();
};

class PartFactory
{
	bool lhs;	// true => in database; false => available
	bool sibs;	// true => full racks; false => boxes only
	std::vector< Box* > boxList;

public:

	PartFactory( bool allocated, bool fillRacks ) : lhs( allocated ), sibs( fillRacks ) {}
	void loadBoxes( const LCDbCryoJob &job );
	const std::vector< Box* > & getBoxes() const { return boxList; }
	void addBox( Box * b ) { boxList.push_back( b ); }
	IPart * createSiteList() const;
};

#endif
