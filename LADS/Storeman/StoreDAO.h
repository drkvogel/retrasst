//---------------------------------------------------------------------------

#ifndef StoreDAOH
#define StoreDAOH

#include <string>
#include <vector>
#include <set>
#include "rosetta.h"
#include <memory>
#include "LQuery.h"
#include "LCDbProject.h"

//---------------------------------------------------------------------------

class StoreDAO
{
	public:

		void loadSites(std::vector<ROSETTA>& results);
		void loadSite( int loc_id, ROSETTA& result);
		bool saveSite( ROSETTA& data );

		void loadTanks( int id, std::vector<ROSETTA>& results);
		void loadTankDetails( int storage_cid, std::vector<ROSETTA>& results );
		bool saveTankObject( ROSETTA& data );
		bool savePhysicalStore( ROSETTA& data );

		bool saveTankMap( ROSETTA& data );
		bool setLayoutAvailability( ROSETTA& data );

		void loadLayouts( int storage_cid, std::vector<ROSETTA>& results);
		bool saveLayout( ROSETTA& data );

		void loadSections( int layout_cid, std::vector<ROSETTA>& results);
		bool saveSection( ROSETTA& data );

		void loadRacks( int tank_cid, std::vector<ROSETTA>& results, int type = 0 );
		void loadRacks( const std::set< int > & rackCids, std::vector<ROSETTA>& results);
		bool occupyRack( ROSETTA& data );
		void loadRackOccupancy( int rack_cid, std::set< int > & occupied );

		bool addBoxToLHSJobList( ROSETTA& data );
		bool addBoxToRHSJobList( ROSETTA& data );
		bool updateBox( ROSETTA& data );
		bool signoffBox( ROSETTA& data );

		void loadBoxDetails( const std::string & barcode, const std::string & box_type, int proj_id, ROSETTA & result );
		void loadBoxDetails( int box_id, int proj_id, ROSETTA & result );
		void loadBoxes( int rack_id, std::vector<ROSETTA>& results);
		void loadBoxesByJobID( int jobID, int proj_id, std::vector<ROSETTA>& results);
		void loadBoxHistory( int box_id, int proj_id, std::vector<ROSETTA>& results);
		void loadSamples( int box_id, int proj_id, std::vector<ROSETTA>& results);

		void loadStorageHistory( int cryovial_id, int proj_id, std::vector<ROSETTA>& results );
		bool loadAnalysisHistory( const std::string & cryovial_barcode, int aliquot_type_id, int proj_id, std::vector<ROSETTA>& results );
		void loadAliquotTypes( std::vector<ROSETTA>& results );

		bool loadCryovials( short source, const std::string & id, int primary, int secondary, int proj_id, std::vector<ROSETTA>& results );
		bool findBox( int box_id, int proj_id, ROSETTA & result );
		bool addToRetrieval( int jobID, int cryovial_id, int proj_id, int box_cid, short pos );
};

//---------------------------------------------------------------------------

#endif
