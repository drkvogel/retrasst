//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MoveJobs.h"
#include "SampleMove.h"
#include "LDbRange.h"
#include "LIMSDatabase.h"
#include "BrowseSample.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmSelectJob *frmSelectJob;

//---------------------------------------------------------------------------

__fastcall TfrmSelectJob::TfrmSelectJob( TComponent *Owner ) : TForm( Owner )
{}

//---------------------------------------------------------------------------

void __fastcall TfrmSelectJob::FormShow( TObject *Sender ) {
	grdJobs->Cells[ 0 ][ 0 ] = "Name";
	grdJobs->Cells[ 1 ][ 0 ] = "Description";
	grdJobs->Cells[ 2 ][ 0 ] = "Creation Date";
	grdJobs->ColWidths[ 0 ] = grdJobs->Width * 0.25;
	grdJobs->ColWidths[ 1 ] = grdJobs->Width * 0.5;
	int allocated = grdJobs->ColWidths[ 0 ] + grdJobs->ColWidths[ 1 ] + 24;
	grdJobs->ColWidths[ 2 ] = grdJobs->Width - allocated;
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
	for( Range< LCDbCryoJob > jr = jobs; jr.isValid( ); ++jr ) {
		if( jr->isAvailable() && jr -> getStatus() < LCDbCryoJob::DONE ) {
			writeJob( *jr, row++ );
		}
	}
	grdJobs->RowCount = row + 1;
	grdJobs->Rows[ row ]->Clear( );
	btnNewJob -> Visible = true;
}

//---------------------------------------------------------------------------

void TfrmSelectJob::writeJob( const LCDbCryoJob &job, int row ) {
	grdJobs->Cells[ 0 ][ row ] = job.getName( ).c_str( );
	grdJobs->Cells[ 1 ][ row ] = job.getDescription( ).c_str( );
	grdJobs->Cells[ 2 ][ row ] = job.getTimeStamp( );
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

void __fastcall TfrmSelectJob::btnNewJobClick( TObject *Sender ) {
	frmBrowse->init( NULL, false );
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

