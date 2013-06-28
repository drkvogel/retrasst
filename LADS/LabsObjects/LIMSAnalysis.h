//---------------------------------------------------------------------------

#ifndef LIMSAnalysisH
#define LIMSAnalysisH

#include "LDbSource.h"
#include "LDbResult.h"
#include "LCDbTest.h"
#include "LDbWorkList.h"
#include "LQuery.h"
#include "LDbRange.h"

//---------------------------------------------------------------------------

class LIMSAnalysis : public LBDbSource
{
	LCDbWorkList cwl;
	int categoryID, parentID;

	std::vector< LCDbWorkEntry > cwlTests;
	std::vector< LBDbWorkEntry > bwlTests;
	std::set< int > addedTests;

	template< typename R, typename B > void scan( std::vector< R > & worklist, B& builder )
	{
		const LCDbTests & tests = LCDbTests::records();
		for( std::vector< R >::iterator wli = worklist.begin(); wli != worklist.end(); ++ wli )
		{
			int testID = wli -> getTestLimits().getID();
			const LCDbTest * test = tests.findByID( testID );
			if( test == NULL || !builder( *test, wli -> getDilution() ) )
				throw Exception( "Test " + String( testID ) + " not configured for " + getBarcode() );
		}
	}

	int findCategory( const LBDbWorkEntry & bwle );
	bool canRun( const LDbWorkEntry & wle );
	bool addBWLEntry( const LBDbWorkEntry & bwle );
	bool addCWLEntry( const LCDbWorkEntry & cwle );

	bool findWorkEntries( LQuery & qCluster, LQuery & qCentral );

public:

	LIMSAnalysis() : categoryID( 0 ), parentID( 0 )
	{}

	LIMSAnalysis( const std::string & sample, int catID, TDateTime when )
	 : LBDbSource( sample, when ), categoryID( catID ), parentID( 0 )
	{}

	LIMSAnalysis( const LBDbSource & bdbRecord )
	 : LBDbSource( bdbRecord ), categoryID( 0 ), parentID( 0 )
	{}

	static std::vector< LIMSAnalysis > checkWorklist( LQuery qCentral, int category );

	int getCategory() const { return categoryID; }
	void setCategory( int catID ) { categoryID = catID; }

	bool findParent( LQuery qCluster, LQuery qCentral );
	int getParent() const { return parentID; }
	
	bool hasAddedTests() const { return !addedTests.empty(); }
	void addTest( const LCDbTest * test );
	bool createHeader( LQuery qCluster, LQuery qCentral );

	unsigned getExpectedTests( LQuery qCluster, LQuery qCentral );
	std::set< int > getTestIDs() const;

	template< typename B > B buildTestList( B builder )
	{
		scan( cwlTests, builder );
		scan( bwlTests, builder );
		return builder;
	}

	void ignoreTest( int testCID );
	void confirmTests( LQuery cluster, LQuery central );
	void updateSample( LQuery qProject, LQuery qCluster, LQuery qCentral );
};

//---------------------------------------------------------------------------

#endif
