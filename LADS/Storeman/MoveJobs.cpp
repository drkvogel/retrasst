// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MoveJobs.h"
#include "SampleMove.h"
#include "LDbRange.h"
#include "LIMSDatabase.h"
#include "BrowseSample.h"
#include "LCDbObject.h"
#include "LCDbProject.h"
#include "NewExercise.h"
#include "NewJob.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmSelectJob *frmSelectJob;

// ---------------------------------------------------------------------------

__fastcall TfrmSelectJob::TfrmSelectJob( TComponent *Owner ) : TForm( Owner ) {
}

// ---------------------------------------------------------------------------

void __fastcall TfrmSelectJob::FormShow( TObject *Sender ) {
	grdJobs->ColCount = NUM_COLS;
	grdJobs->Cells[ JOB_NAME ][ 0 ] = "Task";
	grdJobs->Cells[ REASON ][ 0 ] = "Reason";
	grdJobs->Cells[ DESCRIPTION ][ 0 ] = "Description";
	grdJobs->Cells[ START_DATE ][ 0 ] = "Started";
	int colWidth = grdJobs->Width * 0.2;
	grdJobs->ColWidths[ JOB_NAME ] = colWidth;
	grdJobs->ColWidths[ REASON ] = colWidth;
	grdJobs->ColWidths[ START_DATE ] = (grdJobs->Width - 30) - (colWidth * 4);
	grdJobs->ColWidths[ DESCRIPTION ] = colWidth * 2;
	ActiveControl = grdJobs;
}

// ---------------------------------------------------------------------------
// List the retrieval jobs for the given project; cannot add more
// ---------------------------------------------------------------------------

void TfrmSelectJob::initRetrieval( int projectCID ) {
	retrieval = true;
	const LCDbProject * p = LCDbProjects::records( ).findByID( projectCID );
	if( p != NULL && p->isInCurrentSystem( ) ) {
		project = projectCID;
		cbProjects->Text = p->getName( ).c_str( );
	} else {
		project = 0;
		cbProjects->Text = "(any)";
	}
	exercise = 0;
	cbExercise->Text = "(none)";
	showJobs( );
	btnNewJob->Visible = false;
}

// ---------------------------------------------------------------------------
// List movement jobs that are still in progress (any project/exercise)
// ---------------------------------------------------------------------------

void TfrmSelectJob::initMoveJobs( ) {
	retrieval = false;
	cbProjects->Text = "(any)";
	project = 0;
	cbExercise->Text = "(none)";
	exercise = 0;
	showJobs( );
	btnNewJob->Visible = true;
}

// ---------------------------------------------------------------------------

void TfrmSelectJob::showJobs( ) {
	LCDbCryoJobs &jobs = LCDbCryoJobs::records( );
	jobs.read( LIMSDatabase::getCentralDb( ), LCDbCryoJob::UNKNOWN, false );
	int row = grdJobs->FixedRows;
	for( const LCDbCryoJob & jr : jobs ) {
		if( jr.isAvailable() && (jr.getExercise() == exercise || exercise == 0)
		&& (jr.getProjectID() == project || jr.getProjectID() == 0 || project == 0) ) {
			bool show;
			switch( jr.getJobType( ) ) {
			case LCDbCryoJob::BOX_RETRIEVAL:
			case LCDbCryoJob::SAMPLE_RETRIEVAL:
				show = retrieval;
				break;
			case LCDbCryoJob::BOX_MOVE:
				show = !retrieval;
				break;
			default:
				show = false;
			}
			if( show ) {
				writeJob( jr, row++ );
			}
		}
	}
	grdJobs->RowCount = row + 1;
	grdJobs->Rows[ row ]->Clear( );
}

// ---------------------------------------------------------------------------

void TfrmSelectJob::writeJob( const LCDbCryoJob &job, int row ) {
	grdJobs->Cells[ JOB_NAME ][ row ] = job.getName( ).c_str( );
	grdJobs->Cells[ DESCRIPTION ][ row ] = job.getDescription( ).c_str( );
	TDateTime start = job.getStartDate( );
	if( EPOCH_START < start && EPOCH_END > start ) {
		grdJobs->Cells[ START_DATE ][ row ] = start;
	} else {
		grdJobs->Cells[ START_DATE ][ row ] = "";
	}
	grdJobs->Cells[ REASON ][ row ] = job.getReason( ).c_str( );
}

// ---------------------------------------------------------------------------

void __fastcall TfrmSelectJob::grdJobsDblClick( TObject *Sender ) {
	selectJob( grdJobs->Row );
}

// ---------------------------------------------------------------------------

void __fastcall TfrmSelectJob::btnOpenJobClick( TObject *Sender ) {
	selectJob( grdJobs->Row );
}

// ---------------------------------------------------------------------------

void TfrmSelectJob::selectJob( int index ) {
	record = NULL;
	if( index > 0 ) {
		AnsiString name = grdJobs->Cells[ 0 ][ grdJobs->Row ];
		if( !name.IsEmpty( ) ) {
			record = LCDbCryoJobs::records( ).findByName( name.c_str( ) );
			if( record != NULL ) {
				ModalResult = mrOk;
			}
		}
	}
}

// ---------------------------------------------------------------------------

void __fastcall TfrmSelectJob::cbProjectsDropDown( TObject *Sender ) {
	std::string current = "(any)";
	cbProjects->Clear( );
	cbProjects->Items->Add( current.c_str( ) );
	for( const LCDbProject & pr : LCDbProjects::records( ) ) {
		if( pr.isInCurrentSystem( ) && !pr.isCentral( ) ) {
			if( pr.getID( ) == project ) {
				current = pr.getName( );
			}
			cbProjects->Items->Add( pr.getName( ).c_str( ) );
		}
	}
	cbProjects->Text = current.c_str( );
}

// ---------------------------------------------------------------------------

static bool byDate( const LCDbObject & o1, const LCDbObject & o2 ) {
	return o1.getTimeStamp() > o2.getTimeStamp();
}

void __fastcall TfrmSelectJob::cbExerciseDropDown( TObject *Sender ) {
	std::vector< LCDbObject > exercises;
	for( const LCDbObject & obj : LCDbObjects::records( ) ) {
		if( obj.isActive( ) && obj.getObjectType( ) == LCDbObject::STORAGE_EXERCISE ) {
			exercises.push_back( obj );
		}
	}
	std::sort( exercises.begin(), exercises.end(), byDate );

	cbExercise->Clear( );
	cbExercise->Items->Add( "n/a" );
	int index = 0;
	for( const LCDbObject & obj : exercises ) {
		if( obj.getID() == exercise ) {
			index = cbExercise->Items->Count;
		}
		cbExercise->Items->Add( obj.getName( ).c_str( ) );
	}
	cbExercise->ItemIndex = index;
}

// ---------------------------------------------------------------------------

void __fastcall TfrmSelectJob::btnNewExClick( TObject *Sender ) {
	if( frmNewExercise->ShowModal( ) == mrOk ) {
		const LCDbObject * ex = frmNewExercise->createRecord( );
		if( ex != NULL ) {
			exercise = ex->getID();
			int pos = cbExercise->Items->Count;
			cbExercise->Items->Add( ex->getName( ).c_str( ) );
			cbExercise->ItemIndex = pos;
		}
		showJobs( );
	}
}

// ---------------------------------------------------------------------------

void __fastcall TfrmSelectJob::btnNewJobClick( TObject *Sender ) {
	PartFactory pf( true, true );
	frmBrowse->init( pf.createSiteList( ), false );
	frmNewJob->setExercise( exercise );
	record = NULL;
	ModalResult = frmBrowse->ShowModal( );
}

// ---------------------------------------------------------------------------

void __fastcall TfrmSelectJob::cbProjectsChange( TObject *Sender ) {
	AnsiString name = cbProjects->Text;
	const LCDbProject * pr = LCDbProjects::records( ).findByName( name.c_str( ) );
	project = pr != NULL ? pr->getID( ) : 0;
	showJobs( );
}

// ---------------------------------------------------------------------------

void __fastcall TfrmSelectJob::cbExerciseChange( TObject *Sender ) {
	AnsiString name = cbExercise->Text;
	const LCDbObject * ex = LCDbObjects::records( ).find( name.c_str( ), LCDbObject::STORAGE_EXERCISE );
	exercise = ex != NULL ? ex->getID( ) : 0;
	showJobs( );
}

// ---------------------------------------------------------------------------
