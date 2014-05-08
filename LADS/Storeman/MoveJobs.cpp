//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MoveJobs.h"
#include "SampleMove.h"
#include "LDbRange.h"
#include "LIMSDatabase.h"
#include "BrowseSample.h"
#include "LCDbObject.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmSelectJob *frmSelectJob;

//---------------------------------------------------------------------------

__fastcall TfrmSelectJob::TfrmSelectJob( TComponent *Owner ) : TForm( Owner )
{}

//---------------------------------------------------------------------------

void __fastcall TfrmSelectJob::FormShow( TObject *Sender ) {
	grdJobs->ColCount = NUM_COLS;
	grdJobs->Cells[ JOB_NAME ][ 0 ] = "Task";
	grdJobs->Cells[ EXERCISE ][ 0 ] = "Exercise";
	grdJobs->Cells[ DESCRIPTION ][ 0 ] = "Description";
	grdJobs->Cells[ START_DATE ][ 0 ] = "Started";
	int colWidth = grdJobs->Width * 0.2;
	grdJobs->ColWidths[ JOB_NAME ] = colWidth;
	grdJobs->ColWidths[ EXERCISE ] = colWidth;
	grdJobs->ColWidths[ DESCRIPTION ] = colWidth * 2;
	int allocated = colWidth * 4 + 26;
	grdJobs->ColWidths[ START_DATE ] = grdJobs->Width - allocated;
	ActiveControl = grdJobs;
}

//---------------------------------------------------------------------------
//	List the retrieval jobs for the given project; cannot add more
//---------------------------------------------------------------------------

void TfrmSelectJob::initRetrieval( int projectCID ) {
	LCDbCryoJobs &jobs = LCDbCryoJobs::records();
	jobs.read( LIMSDatabase::getCentralDb(), LCDbCryoJob::UNKNOWN, false );
	int row = grdJobs->FixedRows;
	for( Range< LCDbCryoJob > jr = jobs; jr.isValid( ); ++jr ) {
		if( jr->isAvailable() && jr->getProjectID() == projectCID
		&& (jr->getJobType() == LCDbCryoJob::BOX_RETRIEVAL
		 || jr->getJobType() == LCDbCryoJob::SAMPLE_RETRIEVAL) ) {
			writeJob( *jr, row++ );
		}
	}
	grdJobs->RowCount = row + 1;
	grdJobs->Rows[ row ]->Clear( );
	btnNewJob -> Visible = false;
}

//---------------------------------------------------------------------------
//	List the movement jobs that are still in progress (any project)
//---------------------------------------------------------------------------

void TfrmSelectJob::initMoveJobs() {
	LCDbCryoJobs &jobs = LCDbCryoJobs::records();
	jobs.read( LIMSDatabase::getCentralDb(), LCDbCryoJob::BOX_MOVE, false );
	int row = grdJobs->FixedRows;
	for( const LCDbCryoJob & jr : jobs ) {
		if( jr.isAvailable() && jr.getStatus() < LCDbCryoJob::DONE ) {
			writeJob( jr, row++ );
		}
	}
	grdJobs->RowCount = row + 1;
	grdJobs->Rows[ row ]->Clear( );
	btnNewJob -> Visible = true;
}

//---------------------------------------------------------------------------

void TfrmSelectJob::writeJob( const LCDbCryoJob &job, int row ) {
	grdJobs->Cells[ JOB_NAME ][ row ] = job.getName( ).c_str( );
	grdJobs->Cells[ DESCRIPTION ][ row ] = job.getDescription( ).c_str( );
	TDateTime start = job.getStartDate();
	if( EPOCH_START < start && EPOCH_END > start ) {
		grdJobs->Cells[ START_DATE ][ row ] = start;
	} else {
		grdJobs->Cells[ START_DATE ][ row ] = "";
	}
	const LCDbObject * exName = NULL;
	if( job.getExercise() != 0 ) {
		exName = LCDbObjects::records().findByID( job.getExercise() );
	}
	if( exName != NULL ) {
		grdJobs->Cells[ EXERCISE ][ row ] = exName->getName().c_str();
	} else {
		grdJobs->Cells[ EXERCISE ][ row ] = "";
    }
}

//---------------------------------------------------------------------------

void __fastcall TfrmSelectJob::grdJobsDblClick(TObject *Sender) {
	selectJob( grdJobs->Row );
}

//---------------------------------------------------------------------------

void __fastcall TfrmSelectJob::btnOpenJobClick( TObject *Sender ) {
	selectJob( grdJobs->Row );
}

//---------------------------------------------------------------------------

void __fastcall TfrmSelectJob::btnNewJobClick( TObject *Sender )
{
	PartFactory pf( true, true );
	frmBrowse->init( pf.createSiteList(), false );
	record = NULL;
	ModalResult = frmBrowse->ShowModal( );
}

//---------------------------------------------------------------------------

void TfrmSelectJob::selectJob( int index )
{
	record = NULL;
	if( index > 0 ) {
		AnsiString name =  grdJobs->Cells[ 0 ][ grdJobs->Row ] ;
		if( !name.IsEmpty( ) ) {
			record = LCDbCryoJobs::records( ).findByName( name.c_str() );
			if( record != NULL ) {
				ModalResult = mrOk;
			}
		}
	}
}

//---------------------------------------------------------------------------

