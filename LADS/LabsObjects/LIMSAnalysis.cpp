/*---------------------------------------------------------------------------
 *
 *	Version history:
 *
 *      14 Oct 09, NG	Split out from LIMSResult class - programme only
 *      5 Feb 2010		Added createRecord() to simplify interface
 *      24 Feb 10, NG	Accept function object to build test code list
 *      24 May 10, NG	Copy central to local worklist if necessary
 *      16 Nov 10, NG	Don't create worklist entry when buddy_read == 'Y'
 *		26 Nov, NG		Always take category ID from c_buddy_worklist
 *      16/09/11, NG	Only include local tests in getExpectedTests()
 *      18 Jan 11, NG   Ignore status of local QCs rather than adding later
 *      2 Mar 12, NG	Replace hasReruns() with more specific findParent()
 *      13/4/2012, NG	Only update one worklist entry for each test
 *--------------------------------------------------------------------------*/

#include <vcl.h>

#include "LDbResult.h"
#include "LCDbAnalyser.h"
#include "LDbWorkList.h"
#include "LDbSource.h"
#include "LCDbAuditTrail.h"
#include "LDbNotes.h"
#include "LCDbTest.h"
#include "LIMSAnalysis.h"
#include "LCDbProject.h"
#include "LPDbProfile.h"
#include "TfrmConfirm.h"
#include "LIMSParams.h"
#include "LDbWorklist.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//	create buddy_database entry; buddy_sample_id can be used as Patient ID
//---------------------------------------------------------------------------

bool LIMSAnalysis::createHeader( LQuery qCluster, LQuery qCentral )
{
	if( getExpectedTests( qCluster, qCentral ) == 0 )
		throw Exception( "No tests for " + getBarcode() + " required" );

	const LCDbTests & tests = LCDbTests::records();
	int buddyID = LCDbAnalysers::getCurrentID();
	String fluid = tests.findSampleType( buddyID, getTestIDs() );
	if( fluid.IsEmpty() )
		throw Exception( "Cannot find sample type for " + getBarcode() );
	setSampleType( fluid );

	claimNextID( qCluster );
	return saveRecord( qCluster );
}

//---------------------------------------------------------------------------
//	check central and local worklists for tests to be programmed
//---------------------------------------------------------------------------

unsigned LIMSAnalysis::getExpectedTests( LQuery qCluster, LQuery qCentral )
{
	if( !findWorkEntries( qCluster, qCentral ) && isQC() ) {
		cwl.addQCRecords( qCentral, getBarcode() );
		findWorkEntries( qCluster, qCentral );
	}
	return bwlTests.size() + cwlTests.size() + addedTests.size();
}

//---------------------------------------------------------------------------
//	remove worklist entries for given test (e.g. if not sent to analyser)
//---------------------------------------------------------------------------

void LIMSAnalysis::ignoreTest( int testCID )
{
	std::vector< LCDbWorkEntry >::iterator cwe = cwlTests.begin();
	while( cwe != cwlTests.end() ) {
		if( cwe -> getTestLimits().getID() == testCID ) {
			cwe = cwlTests.erase( cwe );
		} else {
			cwe ++;
		}
	}

	std::vector< LBDbWorkEntry >::iterator bwe = bwlTests.begin();
	while( bwe != bwlTests.end() ) {
		if( bwe -> getTestLimits().getID() == testCID ) {
			bwe = bwlTests.erase( bwe );
		} else {
			bwe ++;
		}
	}

	std::set< int >::iterator ate = addedTests.begin();
	while( ate != addedTests.end() ) {
		if( *ate == testCID ) {
			ate = addedTests.erase( ate );
		} else {
			ate ++;
		}
	}
}

//---------------------------------------------------------------------------
//	Tick off worklist entries read by getExpectedTests() except local QCs
//---------------------------------------------------------------------------

void LIMSAnalysis::confirmTests( LQuery qCluster, LQuery qCentral )
{
	LBDbSource::saveRecord( qCluster );

	std::set< int > updated;
	for( std::vector< LCDbWorkEntry >::iterator cwe = cwlTests.begin(); cwe != cwlTests.end(); ++ cwe ) {
		int test = cwe -> getTestLimits().getID();
		if( updated.count( test ) == 0 && cwe -> updateStatus( qCentral, getStage() ) ) {
			updated.insert( test );
		}
	}

	if( LIMSParams::instance().useLocalWorklist() ) {
		if( bwlTests.empty() ) {
			for( Range< LCDbWorkEntry > cwe = cwlTests; cwe.isValid(); ++ cwe ) {
				if( !cwe -> buddyRead() ) {
					LBDbWorkEntry copy( *cwe );
					if( isQC() ) {
						copy.setResultID( 0 );
						copy.setStage( LDbWorkEntry::NEW_ENTRY );
					} else {
						copy.setResultID( LBDbSource::getID() );
						copy.setStage( getStage() );
					}
					copy.createRecord( qCluster, cwe -> getProfile() );
					bwlTests.push_back( copy );
				}
			}
		}
		else if( !isQC() ) {
			updated.clear();
			std::vector< LBDbWorkEntry >::iterator bwe = bwlTests.begin();
			do {
				int test = bwe -> getTestLimits().getID();
				if( updated.count( test ) == 0 ) {
					bwe -> setResultID( LBDbSource::getID() );
					if( bwe -> updateRecord( qCluster, getStage() ) ) {
						updated.insert( test );
					}
				}
			} while( ++ bwe != bwlTests.end() );
		}
	}
}

//---------------------------------------------------------------------------
//	List the tests required, combining local and central worklists
//---------------------------------------------------------------------------

std::set< int > LIMSAnalysis::getTestIDs() const
{
	std::set< int > tests = addedTests;
	for( Range< LCDbWorkEntry > cwe = cwlTests; cwe.isValid(); ++ cwe )
		tests.insert( cwe -> getTestLimits().getID() );
	for( Range< LBDbWorkEntry > bwe = bwlTests; bwe.isValid(); ++ bwe )
		tests.insert( bwe -> getTestLimits().getID() );
	return tests;
}

//---------------------------------------------------------------------------
//	allow the user to add a test that's not in the worklist
//---------------------------------------------------------------------------

void LIMSAnalysis::addTest( const LCDbTest * test )
{
	if( test != NULL ) {
		addedTests.insert( test -> getID() );
	}
}

//---------------------------------------------------------------------------
//  Update specimen table from buddy_database and add notes if necessary
// 	Mark local and central worklist entry finished, but leave local QCs
//---------------------------------------------------------------------------

void LIMSAnalysis::updateSample( LQuery qProject, LQuery qCluster, LQuery qCentral )
{
	// create specimen entry if necessary (mainly for QCs)
	LPDbSource source( *this );
	if( source.getID() == 0 || !source.findDetails( qProject ) )
	{
		source.createName( qProject );
		source.setArrived( getTimeStamp() );
	}

	// copy sample notes from buddy_note to project's note table
	short saved = 0;
	source.setStage( LDbStage::TRANSMITTED );
	if( hasNotes() )
	{
		LBDbNotes notes;
		if( notes.read( qCluster, LBDbSource::getID() ) ) {
			for( Range< LBDbNote > nr = notes; nr.isValid(); ++ nr ) {
				source.saveWithNote( qProject, *nr );
				saved ++;
			}
			notes.mark( qCluster, LBDbSource::getID() );
		}
	}
	if( saved == 0 ) {
		source.saveRecord( qProject );
	}
	// update buddy_database record to match specimen table entry
	specimen.first = LCDbProjects::getCurrentID();
	specimen.second = source.getID();
	setStage( LDbStage::TRANSMITTED );
	confirmTests( qCluster, qCentral );
}

//---------------------------------------------------------------------------
//	find recent entries for given sample; keep entries for local tests
//---------------------------------------------------------------------------

bool LIMSAnalysis::findWorkEntries( LQuery & qCluster, LQuery & qCentral )
{
	if( cwl.read( qCentral, getBarcode() ) ) {
		cwlTests.clear();
		for( Range< LCDbWorkEntry > wle = cwl; wle.isValid(); ++ wle ) {
			if( wle -> getStage() <= LDbWorkEntry::PROGRAMMED ) {
				if( wle -> matchCategory( categoryID ) && canRun( *wle ) ) {
					addCWLEntry( *wle );
				}
			}
		}
	}

	LBDbWorkList & bwl = LBDbWorkList::records();
	if( LIMSParams::instance().useLocalWorklist() && bwl.read( qCluster, getBarcode() ) ) {
		bwlTests.clear();
		for( MapRange< String, LBDbWorkEntry > wle = bwl.getTests( getBarcode() ); wle.isValid(); ++ wle ) {
			if( isQC() || wle -> getStage() <= LDbWorkEntry::PROGRAMMED ) {
				if( wle -> getCategoryID() == 0 ) {
					LBDbWorkEntry bwle = *wle;
					bwle.setCategoryID( findCategory( *wle ) );
					if( canRun( bwle ) ) {
						addBWLEntry( bwle );
					}
				}
				else if( wle -> matchCategory( categoryID ) && canRun( *wle ) ) {
					addBWLEntry( *wle );
				}
			}
		}
	}
	return !cwlTests.empty() || !bwlTests.empty();
}

//---------------------------------------------------------------------------
//	find original category if given entry was from the central worklist
//---------------------------------------------------------------------------

int LIMSAnalysis::findCategory( const LBDbWorkEntry & bwle )
{
	for( Range< LCDbWorkEntry > cwle = cwl; cwle.isValid(); ++ cwle ) {
		if( cwle -> getCategoryID() != 0 && cwle -> getSpecimen() == bwle.getSpecimen() ) {
			const LDbTestLimits & ctl = cwle -> getTestLimits();
			const LDbTestLimits & btl = bwle.getTestLimits();
			if( ctl.getID() == btl.getID() && ctl.getAnalyserID() == btl.getAnalyserID() )
				return cwle -> getCategoryID();
		}
	}
	return 0;		// not in central worklist???
}

//---------------------------------------------------------------------------
//	check the given entry matches the selected category and sample type
//---------------------------------------------------------------------------

bool LIMSAnalysis::canRun( const LDbWorkEntry & wle )
{
	int testID = wle.getTestLimits().getID();
	const LCDbTest * test = LCDbTests::records().findByID( testID );
	if( test == NULL )
		return false;			// worklist error or test expired

	int buddyID = LCDbAnalysers::getCurrentID();
	const LCDbTest::MachineDef * md = test -> findMachineDef( buddyID );
	if( md == NULL )
		return false;			// test doesn't run on this analyser

	const String & expected = getSampleType();
	return expected.IsEmpty() || md -> getSampleType() == expected;
}

//---------------------------------------------------------------------------
//	add given c_buddy_worklist entry if it's part of the latest group
//---------------------------------------------------------------------------

bool LIMSAnalysis::addCWLEntry( const LCDbWorkEntry & cwle )
{
	std::vector< LCDbWorkEntry >::const_iterator first = cwlTests.begin();
	if( first == cwlTests.end() )
		specimen = cwle.getSpecimen();
	else
	{	int group = first -> getGroupID();
		if( cwle.getGroupID() > group )
			return false;						// ignore old worklist entries

		else if( cwle.getGroupID() < group )
			cwlTests.clear();                   // new group; forget older one

		else if( cwle.getSpecimen() != specimen )
			throw Exception( "Multiple samples for " + getBarcode() );
	}

	cwlTests.push_back( cwle );
	return true;
}

//---------------------------------------------------------------------------
//	add given buddy_worklist entry if it's linked to current specimen
//---------------------------------------------------------------------------

bool LIMSAnalysis::addBWLEntry( const LBDbWorkEntry & bwle )
{
	if( specimen.second == 0 )
		specimen = bwle.getSpecimen();

	else if( bwle.getSpecimen().second != 0 && bwle.getSpecimen() != specimen )
		return false;

	bwlTests.push_back( bwle );
	return true;
}

//---------------------------------------------------------------------------
//	check if this sample is being re-run and identify parent analysis
//---------------------------------------------------------------------------

bool LIMSAnalysis::findParent( LQuery qCluster, LQuery qCentral )
{
	if( LIMSParams::instance().useLocalWorklist() )	{
		// check for existing buddy_database entry; use most recent
		LBDbSources & bdb = LBDbSources::records();
		const LBDbSource * previous;
		if( isQC() ) {
			previous = bdb.readRecord( qCluster, getBarcode(), "", getSampleType() );
		} else {
			previous = bdb.readRecord( qCluster, getSpecimen() );
		}
		parentID = (previous == NULL ? 0 : previous -> getID());
	} else {
		// use c_worklist_relation if Paul's Validation populated it
		std::set< int > recordIDs;
		for( Range< LCDbWorkEntry > cwle = cwlTests; cwle.isValid(); ++ cwle ) {
			recordIDs.insert( cwle -> getID() );
		}
		parentID = cwl.findParentAnalysis( qCentral, recordIDs );
	}
	return parentID != 0;
}

//---------------------------------------------------------------------------
//	Check central worklist for recently added samples; create analyses
//---------------------------------------------------------------------------

std::vector< LIMSAnalysis > LIMSAnalysis::checkWorklist( LQuery qCentral, int category )
{
	std::vector< LIMSAnalysis > pending;
	LCDbWorkList central;
	central.read( qCentral, false );
	for( Range< LCDbWorkEntry > wle = central; wle.isValid(); ++ wle ) {
		if( wle -> getStage() < LDbWorkEntry::PROGRAMMED && wle -> matchCategory( category ) ) {
			bool found = false;
			std::vector< LIMSAnalysis >::const_reverse_iterator pi = pending.rbegin();
			while( pi != pending.rend() && !found ) {
				found = (wle -> matchCategory( pi -> getCategory() ) && wle -> getBarcode() == pi -> getBarcode());
				++ pi;
			}
			if( !found ) {
				pending.push_back( LIMSAnalysis( wle -> getBarcode(), wle -> getCategoryID(), wle -> getTimeStamp() ) );
			}
		}
	}
	return pending;
}

//---------------------------------------------------------------------------

