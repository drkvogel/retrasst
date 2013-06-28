//---------------------------------------------------------------------------
//
//	Dummy communications program, used to test shared classes
//
//      1.3.0, NG:	Refactored to move logic into LIMSResult
//      1.3.1, NG:	Create results for tests in worklist if possible
//
//---------------------------------------------------------------------------

#include <vcl.h>
#include <stdlib>
#include <stdio.h>

#include "testMain.h"
#include "TfrmLogin.h"
#include "LCDbProject.h"
#include "LIMSResult.h"
#include "LCDbTest.h"
#include "LQuery.h"
#include "LPDbProfile.h"
#include "LDbWorkList.h"
#include "LDbNotes.h"
#include "LCDbAnalyser.h"

#pragma hdrstop

#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------

TtestForm *testForm;

__fastcall TtestForm::TtestForm(TComponent* Owner) : TForm(Owner)
{
	TfrmLogin::mainForm = this;
}

//---------------------------------------------------------------------------

void __fastcall TtestForm::FormShow(TObject *Sender)
{
	timer -> Enabled = true;
}

//---------------------------------------------------------------------------

void __fastcall TtestForm::FormHide(TObject *Sender)
{
	timer -> Enabled = false;
	samples.clear();
	testIDs.clear();
	display -> Clear();
}

//---------------------------------------------------------------------------

void __fastcall TtestForm::timerTimer(TObject *Sender)
{
	timer -> Enabled = false;
	String message;
	try
	{	if( samples.empty() )
		{
			readBarcodes();
			message = String( samples.size() ) + " samples, " + testIDs.size() + " tests";
		}

		else if( wipeSample() )
			message = selected + " has been wiped";
		else
			message = String( createResults() ) + " result(s) for " + selected;
	}
	catch( Exception & ex )
	{
		message = ex.Message;
	}

	display -> Lines -> Add( message );
	timer -> Enabled = true;
}

//---------------------------------------------------------------------------

bool TtestForm::wipeSample()
{
	selected = LBDbWorkList::records().nextWipe( qCluster );
	return !selected.IsEmpty() && LBDbWorkList::records().confirmWiped( selected, qCluster );
}

//--------------------------------------------------------------------------

void TtestForm::readBarcodes()
{
	LPDbProfiles::readAll( qProject );
	LCDbProjects::releaseDatabases();

	LQuery bq( qCluster );
	bq.setSQL( "select distinct test_id from buddy_worklist" );
	for( bq.open(); !bq.eof(); bq.next() )
		testIDs.push_back( bq.readInt( 0 ) );

	bq.setSQL( "select distinct barcode from buddy_worklist" );
	for( bq.open(); !bq.eof(); bq.next() )
	{
		String barcode = bq.readString( 0 );
		if( barcode.Pos( "QC" ) )
		{
			barcode.SetLength( barcode.Length() - 1 );
			for( int run = 1; run <= 9 && !magic(); run ++ )
				samples.push_back( barcode + run );
		}
		else samples.push_back( barcode );
	}

	for( char c = 'A'; c <= 'Z'; c ++ )
		samples.push_back( AnsiString::StringOfChar( c, 8 ) );
}

//--------------------------------------------------------------------------

int TtestForm::createResults()
{
	selected = samples[ random( samples.size() ) ];
	std::set< int > expected = LIMSResult::getExpectedTests( selected, qCluster, qCentral );
	if( !magic() )
		LIMSResult::confirmSample( qCluster, selected, Now() );

	int test, count = 0;
	std::set< int >::const_iterator next = expected.begin();
	do
	{	if( next == expected.end() )
			test = testIDs[ random( testIDs.size() ) ];
		else
			test = *next ++;

		std::pair< double, double > range = LCDbTests::records().get( test ).getLimits();
		double value = random( range.first + range.second );

		LIMSResult result( selected, test, value, Now() );
		result.updateWorklist( qCluster, qCentral );
		result.saveRecord( qCluster, "Result: " + String( ++ count ) );

	} while( !magic() );

	return count;
}

//---------------------------------------------------------------------------

