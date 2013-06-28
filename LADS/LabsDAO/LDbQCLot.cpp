//---------------------------------------------------------------------------
//
//	23 June, 2005:	specify machine when copying from central to buddy
//
//---------------------------------------------------------------------------

#include <vcl.h>
#include <functional>
#include <numeric.h>

#include "LDbQCLot.h"
#include "LCDbAnalyser.h"
#include "LDbWorkList.h"
#include "LCDbTest.h"
#include "LCDbProject.h"
#include "LQuery.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//	Constructor: read fields from combined tables; generate description
//---------------------------------------------------------------------------

LCDbQCLot::LCDbQCLot( const LQuery & qCentral )
 : LCDbID( qCentral.readInt( "qc_id" ) ),
   LDbQCLot( qCentral.readString( "Lot" ),
    		 qCentral.readString( "QC_Level" ),
			 qCentral.readString( "Material" ),
			 qCentral.readString( "Material_Descrip" )
			 + " " + qCentral.readString( "Level_Descrip" )
			 + " " + qCentral.readString( "Lot_Descrip" ),
			 qCentral.readShort( "status" ) == DELETED )
{}

//---------------------------------------------------------------------------

LCDbQCLot::Test::Test( const LQuery & qCentral )
 : LDbQCValues( qCentral.readInt( "test_cid" ),
				qCentral.readDouble( "packet_mean" ),
				qCentral.readDouble( "packet_stddev" ) )
{}

//---------------------------------------------------------------------------
//	Read (newly configured) QCs from the central database; return count
//---------------------------------------------------------------------------

bool LCDbQCLots::read( LQuery qCentral, bool readAll )
{
	String select = "select * from QC_Material, QC_Lot, QC_Level"
				   " where QC_Material.Material = QC_Level.Material"
				   " and QC_Lot.Material = QC_Level.Material"
				   " and QC_Lot.QC_Level = QC_Level.QC_Level";
	if( readAll )
		qCentral.setSQL( select + "order by qc_id" );
	else
	{	qCentral.setSQL( select + " and status <> :old order by qc_id" );
		qCentral.setParam( 0, LCDbQCLot::DELETED );
	}
	if( !readData( qCentral ) )
		return false;

	iterator ci = begin();
	qCentral.setSQL( "select * from QC_Test order by QC_id, test_cid" );
	for( qCentral.open(); !qCentral.eof(); qCentral.next() )
	{
		int did = qCentral.readInt( "QC_id" );
		while( ci != end() && ci -> getID() < did )
			ci ++;

		if( ci != end() && ci -> getID() == did )
			ci -> tests.insert( LCDbQCLot::Test( qCentral ) );
	}
	return true;
}

//---------------------------------------------------------------------------
//	Add an entry to the qc_gate table recording updated test values
//---------------------------------------------------------------------------

void LCDbQCLot::addQCGate( LQuery qCentral, const LDbQCValues & values ) const
{
	qCentral.setSQL( "insert into qc_gate (QC_ID, mean, stddev, test_cid, machine_cid, time_stamp)"
					" values( :qid, :mn, :sd, :tid, :mid, 'now' )" );
	qCentral.setParam( "qid", getID() );
	qCentral.setParam( "tid", values.getID() );
	qCentral.setParam( "mn", values.getMean() );
	qCentral.setParam( "sd", values.getSD() );
	qCentral.setParam( "mid", LCDbAnalysers::getCurrentID() );
	qCentral.execSQL();
}

//---------------------------------------------------------------------------
//	Find the QC lot definition that the given test has been taken from
//---------------------------------------------------------------------------

class LCDbQCLots::LotMatch : public std::unary_function< LCDbQCLot, bool >
{
	const LDbQCLot lot;

public:

	LotMatch( const LDbQCLot & qc ) : lot( qc ) {}
	operator String() const { return lot.getPrefix(); }
	bool operator() ( const LCDbQCLot & other ) const
	{
		return (lot.getName() == other.getName()
			 && lot.getLevel() == other.getLevel()
			 && lot.getMaterial() == other.getMaterial());
	}
};

//---------------------------------------------------------------------------

const LCDbQCLot * LCDbQCLots::find( const LDbQCLot & qc ) const
{
	return findMatch( LotMatch( qc ) );
}

//---------------------------------------------------------------------------
//	Read configured and calculated QCs for the selected analyser
//---------------------------------------------------------------------------

LBDbQCTest::LBDbQCTest( const LQuery & bq )
 : LBDbID( bq.readInt( "qc_machine_test_id" ) ),
   LDbQCLot( bq.readString( "lotNumber" ),
			 bq.readString( "qcLevel" ),
			 bq.readString( "qcMaterial" ),
			 bq.readString( "QCdescription" ),
			 bq.readChar( "finished" ) == 'Y' ),
   assigned( bq.readInt( "testCid" ),
			 bq.readDouble( "PreAssignedMean" ),
			 bq.readDouble( "PreAssignedSD" ) ),
   machine( bq.readInt( "machineCid" ),
			bq.readDouble( "CalculatedMean" ),
			bq.readDouble( "CalculatedSD" ) ),
   results( bq.readInt( "nResults" ) ),
   useCalc( bq.readChar( "calculateMean" ) != 'N' ),
   wgCheck( bq.readChar( "WestgardCheck" ) != 'N' )
{}

//---------------------------------------------------------------------------

void LBDbQCTests::read( LQuery buddyQuery )
{
	buddyQuery.setSQL( "select * from QCMaterial where machineCid = :mid"
					  " order by qc_machine_test_id" );
	buddyQuery.setParam( 0, LCDbAnalysers::getCurrentID() );
	readData( buddyQuery );
}

//---------------------------------------------------------------------------
//	Use values from QC result table to calculate mean and standard deviation
//---------------------------------------------------------------------------

class LBDbQCTest::AddDiffSq : public std::binary_function< double, double, double >
{
	const double mean;

public:

	AddDiffSq( double val ) : mean( val ) {}
	double operator() ( double sum, double next ) const
	{
		double diff = next - mean;
		return sum + (diff * diff);
	}
};

//---------------------------------------------------------------------------

bool LBDbQCTest::checkResults( LQuery buddyQuery )
{
	buddyQuery.setSQL( "select res_value from buddy_qcresult_float"
					  " where action_flag = '2' and machine_cid = :mid and test_id = :tid"
					  " and qcMaterial = :qcm and qcLevel = :qcl and lotNumber = :qcn" );
	buddyQuery.setParam( "mid", machine.getID() );
	buddyQuery.setParam( "tid", assigned.getID() );
	buddyQuery.setParam( "qcm", getMaterial() );
	buddyQuery.setParam( "qcl", getLevel() );
	buddyQuery.setParam( "qcn", getName() );
	if( buddyQuery.open() < 2 )
		return false;

	std::vector< double > validated;
	do
	{ 	validated.push_back( buddyQuery.readDouble( 0 ) );
		buddyQuery.next();
	} while( !buddyQuery.eof() );

	results = validated.size();
	double mean = std::accumulate( validated.begin(), validated.end(), 0.0 ) / results;
	machine.setMean( mean );
	double diffs = std::accumulate( validated.begin(), validated.end(), 0.0, AddDiffSq( mean ) );
	machine.setSD( std::sqrt( diffs / (results - 1) ) );
	return true;
}

//---------------------------------------------------------------------------
//	Update record ID (if necessary); store mean and standard deviation
//---------------------------------------------------------------------------

bool LBDbQCTest::saveRecord( LQuery buddyQuery )
{
	if( getID() == 0 )
		claimNextID( buddyQuery );

	if( saved )
	{
		buddyQuery.setSQL( "update QCMaterial set qc_machine_test_id = :qid,"
					" nResults = :num, CalculatedMean = :cmn, CalculatedSD = :csd,"
					" PreAssignedMean = :pmn, PreAssignedSD = :psd,"
					" CalculateMean = :cmf, WestgardCheck = :wgf, Finished = :fin"
					" where qcMaterial = :qcm and qcLevel = :qcl and lotNumber = :qcn"
					" and machineCid = :mid and testCid = :tid" );
	}
	else
	{	buddyQuery.setSQL( "Insert into QCMaterial"
					"(qc_machine_test_id, QCMaterial, QCLevel, LotNumber, MachineCID,"
					" TestCID, QCDescription, PreAssignedMean, PreAssignedSD, CalculateMean,"
					" WestgardCheck, Finished, nResults, CalculatedMean, CalculatedSD)"
					" values ( :qid, :qcm, :qcl, :qcn, :mid, :tid, :des, :pmn, :psd, :cmf,"
					" :wgf, :fin, :num, :cmn, :csd)" );
		buddyQuery.setParam( "des", getDescription() );
	}

	buddyQuery.setParam( "qid", getID() );
	buddyQuery.setParam( "mid", machine.getID() );
	buddyQuery.setParam( "cmn", machine.getMean() );
	buddyQuery.setParam( "csd", machine.getSD() );
	buddyQuery.setParam( "num", results );
	buddyQuery.setParam( "tid", assigned.getID() );
	buddyQuery.setParam( "pmn", assigned.getMean() );
	buddyQuery.setParam( "psd", assigned.getSD() );
	buddyQuery.setParam( "qcm", getMaterial() );
	buddyQuery.setParam( "qcl", getLevel() );
	buddyQuery.setParam( "qcn", getName() );
	buddyQuery.setParam( "cmf", useCalc ? 'Y' : 'N' );
	buddyQuery.setParam( "wgf", wgCheck ? 'Y' : 'N' );
	buddyQuery.setParam( "fin", isFinished() ? 'Y' : 'N' );
	return buddyQuery.execSQL() > 0;
}

//---------------------------------------------------------------------------
//	Find the QC test that the given result should be compared against
//---------------------------------------------------------------------------

class /* LBDbQCTests:: */ Matcher : public std::unary_function< LBDbQCTest, bool >
{
	const int test;
	String material, level, lot;

public:

	Matcher( const LDbQCLot & qc, int testID )
	 : test( testID ), material( qc.getMaterial() ), level( qc.getLevel() ), lot( qc.getName() )
	{}

	Matcher( const String & barcode, int testID )
	 : test( testID )
	{
		if( barcode.Length() < 8 || barcode[ 1 ] != 'Q' || barcode[ 2 ] != 'C' )
			throw Exception( barcode + " is not a QC" );

		material = barcode.SubString( 3, 2 );
		level = barcode.SubString( 5, 1 );
		lot = barcode.SubString( 6, 3 );
	}

	operator String() const { return "QC" + material + level + lot + test; }

	bool operator() ( const LBDbQCTest & other ) const
	{
		return (test == other.getAssigned().getID()
			 && lot == other.getName() && level == other.getLevel()
			 && material == other.getMaterial());
	}
};

//---------------------------------------------------------------------------

const LBDbQCTest * LBDbQCTests::find( const LDbQCLot & qc, int test ) const
{
	return findMatch( Matcher( qc, test ) );
}

//---------------------------------------------------------------------------

const LBDbQCTest * LBDbQCTests::find( const String & barcode, int test ) const
{
	return findMatch( Matcher( barcode, test ) );
}

//---------------------------------------------------------------------------

