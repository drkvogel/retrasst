//---------------------------------------------------------------------------

#ifndef LIMSResultH
#define LIMSResultH

#include "LDbSource.h"
#include "LDbResult.h"
#include "LCDbTest.h"
#include "LDbWorkList.h"
#include "LDbResAttribute.h"
#include "LQuery.h"
#include "LDbRange.h"

//---------------------------------------------------------------------------

class LIMSResult : public LBDbResult, public LBDbSource
{
	std::vector< LDbResAttribute > attributes;

//	void linkWorkEntry( LQuery cluster, LBDbWorkEntry update );
	void updateLocalWorklist( LQuery & qCluster, LBDbWorkEntry bwle, bool exists );
	void createReRun( LBDbWorkEntry wle, LQuery cluster );
	void createTriggers( LBDbWorkEntry wle, LQuery cluster );

	template< typename T > const T::value_type * check( T & list, bool pending );
	template< typename T > int makeResult( LQuery & qProject, LQuery & qCluster, LQuery & qCentral );

	struct Key
	{
		int project, specimen, test;
		Key( const LIMSResult & r );
		bool operator< ( const Key & other ) const;
	};

public:

	LIMSResult( const LBDbSource & header );
	LIMSResult( const LBDbResult & result );
	LIMSResult( const LBDbSource & analysis, const LBDbResult & result );
	LIMSResult( const std::string & sample, TDateTime when );

	const LBDbSource * findAnalysis( LQuery qCluster, int analysisID );
	String toString() const;

	TDateTime getTimeStamp() const;
	LDbStage::Stage getStage() const;

	void setTestResult( int test, TDateTime when, const std::string & value );
	String asSampleNote() const;
	void setStage( LDbStage::Stage stage );

	void addAttribute( const LDbResAttribute & pAttribute ) {
		attributes.push_back( pAttribute );
	}

	bool saveAnalysis( LQuery qCluster, const String & note );
	bool saveTestResult( LQuery qCluster, const String & note );
//	bool saveWithNote( LQuery qCluster, const String & note );

	bool updateWorklist( LQuery qCluster, LQuery qCentral );
	void updateSample( LQuery qProject, LQuery qCluster, LQuery qCentral );
	bool transmit( LQuery qProject, LQuery qCluster, LQuery qCentral );
};

//---------------------------------------------------------------------------

#endif

