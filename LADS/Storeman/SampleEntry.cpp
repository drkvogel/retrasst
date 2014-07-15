// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SampleEntry.h"
#include "RetrieveSamples.h"
#include "showsamples.h"
#include "StoreDAO.h"
#include "LCDbObject.h"
#include "LPDbCryovial.h"
#include "StoreUtil.h"
#include "LDbBoxType.h"
#include "LPDbBoxes.h"
#include "NewJob.h"
#include "NewBoxType.h"
#include "LDbBoxSize.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmRetrieveMain *frmRetrieveMain;

// ---------------------------------------------------------------------------

__fastcall TfrmRetrieveMain::TfrmRetrieveMain( TComponent* Owner ) : TForm( Owner ) {
}

// ---------------------------------------------------------------------------

void TfrmRetrieveMain::init( ) {
	resizeGrid( );
	grdSamples->Cells[ SAMPLE ][ 0 ] = "Sample";
	grdSamples->Cells[ CRYOVIAL ][ 0 ] = "Cryovial";
	grdSamples->Cells[ ALIQUOT ][ 0 ] = "Aliquot";
	grdSamples->Cells[ SITE ][ 0 ] = "Site";
	grdSamples->Cells[ LOCATION ][ 0 ] = "Location";
	grdSamples->Cells[ VESSEL ][ 0 ] = "Vessel";
	grdSamples->Cells[ SHELF ][ 0 ] = "Shelf";
	grdSamples->Cells[ STRUCTURE ][ 0 ] = "Structure";
	grdSamples->Cells[ SLOT_POS ][ 0 ] = "Slot";
	grdSamples->Cells[ OLD_POS ][ 0 ] = "Old pos";
	grdSamples->Cells[ OLD_BOX ][ 0 ] = "Old box";
	grdSamples->Cells[ NEW_BOX ][ 0 ] = "New box";
	grdSamples->Cells[ NEW_POS ][ 0 ] = "New pos";
	rows.clear( );
	sortList.clear( );
	updateDisplay( );
}

// ---------------------------------------------------------------------------
//	Expand or shrink columns to fit - allow double width for the box name
// ---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::FormResize( TObject *Sender ) {
	resizeGrid( );
	updateDisplay( );
}

// ---------------------------------------------------------------------------

void TfrmRetrieveMain::resizeGrid( ) {
	grdSamples->ColCount = COL_COUNT;
	int minColWidth = grdSamples->ClientWidth / ( COL_COUNT + 2 );
	grdSamples->DefaultColWidth = minColWidth;
	int narrowCols = ( minColWidth + 1 ) * ( COL_COUNT - 1 );
	grdSamples->ColWidths[ OLD_BOX ] = grdSamples->ClientWidth - narrowCols;
}

// ---------------------------------------------------------------------------
//	add cryovials from a text file of barcodes and (optional) destination
// ---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::AddClick( TObject *Sender ) {
	if( !OpenDialog1->Execute( ) ) {
		return;
	}
	Screen->Cursor = crSQLWait;
	progress->Position = 0;
	std::unique_ptr < TStrings > lines( new TStringList );
	lines->LoadFromFile( OpenDialog1->FileName );
	progress->Max = lines->Count;
	std::unique_ptr < TStrings > fields( new TStringList );
	bool expectPos = (rgDestOrder->ItemIndex == FROM_FILE);
	bool finished = false;
	for( int line = 0; line < lines->Count; ) {
		fields->CommaText = lines->Strings[ line ++ ];
		int pos;
		String warning;
		switch( fields->Count ) {
			case 0:
				continue;
			case 1:
				if( expectPos ) {
					warning = "No position";
				} else {
					pos = rows.size() + 1;
				}
				break;
			case 2:
				if( expectPos ) {
					pos = fields->Strings[ 1 ].ToIntDef( -1 );
					if( pos < 1 ) {
						warning = "Invalid position";
					}
					break;
				}
			default:
				warning = "Extra characters";
		}

		if( warning.IsEmpty() ) {
			warning = addCryovials( fields->Strings[ 0 ], pos );
		}
		if( !warning.IsEmpty() ) {
			warning = warning + " on line " + String( line );
			if( Application->MessageBox( warning.c_str(), L"Warning", MB_OKCANCEL | MB_ICONWARNING) != IDOK ) {
				rows.clear( );
				break;
			}
		}
		progress->StepIt( );
		fillGrid( );
		Application->ProcessMessages( );
	}
	Screen->Cursor = crDefault;
	sortList.clear( );
	updateDisplay();
}

// ---------------------------------------------------------------------------
//	add cryovials at given position from given box/sample/cryovial barcode
// ---------------------------------------------------------------------------

AnsiString TfrmRetrieveMain::addCryovials( const AnsiString & barcode, int pos ) {
	StoreDAO dao;
	short source = rgItemType->ItemIndex;
	int primary = getAliquotTypeID( CmbAliquot1 ), secondary = getAliquotTypeID( CmbAliquot2 );
	int projID = LCDbProjects::getCurrentID( );
	std::vector < ROSETTA > results;
	dao.loadCryovials( source, barcode.c_str(), primary, secondary, projID, results );
	AnsiString warning;
	switch( results.size() ) {
		case 0:
			warning = "No samples found for " + barcode;
			break;
		case 1:
			rows.push_back( GridEntry( *results.begin(), pos ) );
			break;
		case 2:
			if( primary != 0 && secondary != 0 ) {
				rows.push_back( GridEntry( results[ 0 ], pos ) );
				rows.push_back( GridEntry( results[ 1 ], pos ) );
				break;
			}
		default:
			if( source != BOXES ) {
				warning = "Multiple cryovials for " + barcode;
			}
			for( const ROSETTA & r : results ) {
				rows.push_back( GridEntry( r, pos ++ ) );
			}
	}
	return warning;
}

// ---------------------------------------------------------------------------
//	List projects in current system that may include cryovials to retrieve
// ---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::cbProjectDropDown( TObject *Sender ) {
	cbProject->Clear( );
	int selected = -1;
	for( const LCDbProject & pr : LCDbProjects::records( ) ) {
		if( pr.isInCurrentSystem( ) && pr.hasStorage( ) && !pr.isCentral( ) ) {
			if( pr.getID( ) == LCDbProjects::getCurrentID( ) ) {
				selected = cbProject->Items->Count;
			}
			cbProject->Items->Add( pr.getName( ).c_str( ) );
		}
	}
	cbProject->ItemIndex = selected;
}

// ---------------------------------------------------------------------------
//	switch project (leave existing list to allow multi-project retrievals)
// ---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::cbProjectChange( TObject *Sender ) {
	LCDbProjects &projList = LCDbProjects::records( );
	std::string proj = AnsiString( cbProject->Text ).c_str( );
	const LCDbProject *selected = projList.findByName( proj );
	if( selected != NULL ) {
		projList.setCurrent( selected );
	}
	enableButtons( );
}

// ---------------------------------------------------------------------------
//	update the display after sorting, clearing or importing cryovial data
// ---------------------------------------------------------------------------

void TfrmRetrieveMain::updateDisplay( ) {
	lbSortCols->Clear( );
	for( const Column & ci : sortList.columns ) {
		AnsiString order = ci.ascending ? " ascending" : " descending";
		lbSortCols->Items->Add( ci.label.c_str( ) + order );
	}
	if( rows.empty( ) ) {
		grdSamples->RowCount = 2;
	} else {
		fillGrid( );
	}
	for( int row = rows.size( ) + 1; row < grdSamples->RowCount; row++ ) {
		for( int col = 0; col < COL_COUNT; col++ ) {
			grdSamples->Cells[ col ][ row ] = " ";
		}
	}
	enableButtons( );
}

// ---------------------------------------------------------------------------

void TfrmRetrieveMain::fillGrid( ) {
	unsigned count = rows.size( );
	for( int n = 0; n < count; ) {
		const GridEntry & row = rows[ n++ ];
		setValue( SAMPLE, n, row.sample );
		setValue( CRYOVIAL, n, row.cryovial );
		setValue( ALIQUOT, n, row.aliquot );
		setValue( SITE, n, row.site );
		setValue( LOCATION, n, row.location );
		setValue( VESSEL, n, row.vessel );
		setValue( SHELF, n, row.shelf );
		setValue( STRUCTURE, n, row.structure );
		setValue( SLOT_POS, n, row.slot );
		setValue( OLD_BOX, n, row.old_box );
		setValue( OLD_POS, n, row.old_pos );
		setValue( NEW_POS, n, row.new_pos );
	}
	grdSamples->RowCount = count + 2;
}

// ---------------------------------------------------------------------------

void TfrmRetrieveMain::setValue( int col, int row, const std::string & value ) {
	grdSamples->Cells[ col ][ row ] = value.c_str( );
}

// ---------------------------------------------------------------------------

void TfrmRetrieveMain::setValue( int col, int row, short value ) {
	if( value == 0 ) {
		grdSamples->Cells[ col ][ row ] = " ";
	} else {
		grdSamples->Cells[ col ][ row ] = value;
	}
}

// ---------------------------------------------------------------------------
//	List aliquots that may be available in the current project
// ---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::CmbAliquot1DropDown( TObject *Sender ) {
	populate( CmbAliquot1, CmbAliquot2 );
}

// ---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::CmbAliquot2DropDown( TObject *Sender ) {
	populate( CmbAliquot2, CmbAliquot1 );
}

// ---------------------------------------------------------------------------

void TfrmRetrieveMain::populate( TComboBox * target, TComboBox * other ) {
	target->Clear( );
	std::set < int > types;
	for( const LPDbBoxType & bt : LPDbBoxTypes::records( ) ) {
		if( bt.getProjectCID( ) == 0 || bt.getProjectCID( ) == LCDbProjects::getCurrentID( ) ) {
			for( int at : bt.getAliquots( ) ) {
				types.insert( at );
			}
		}
	}
	types.erase( getAliquotTypeID( other ) );
	for( const LCDbObject & at : LCDbObjects::records( ) ) {
		if( types.count( at.getID( ) ) != 0 ) {
			target->Items->Add( at.getName( ).c_str( ) );
		}
	}
}

// ---------------------------------------------------------------------------

int TfrmRetrieveMain::getAliquotTypeID( TComboBox * cb ) {
	const LCDbObject * aliquot = NULL;
	std::string content = AnsiString( cb->Text ).c_str( );
	if( !content.empty( ) ) {
		aliquot = LCDbObjects::records( ).find( content, LCDbObject::ALIQUOT_TYPE );
	}
	return aliquot == NULL ? 0 : aliquot->getID( );
}

// ---------------------------------------------------------------------------
//	check location of the source boxes (if recorded); refresh display
// ---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::btnLocateClick( TObject *Sender ) {
	StoreDAO dao;
	std::map < int, const GridEntry* > boxes;
	Screen->Cursor = crSQLWait;
	progress->Position = 0;
	progress->Max = rows.size( );
	for( std::vector < GridEntry > ::iterator ge = rows.begin( ); ge != rows.end( ); ++ge ) {
		int boxID = ge->bid;
		auto found = boxes.find( boxID );
		if( found != boxes.end( ) ) {
			ge->copyLocation( *( found->second ) );
		} else {
			ROSETTA result;
			if( dao.findBox( boxID, result ) ) {
				ge->copyLocation( result );
				fillGrid( );
			}
			boxes[ boxID ] = &( *ge );
		}
		progress->StepIt( );
		Application->ProcessMessages( );
	}
	updateDisplay( );
	Screen->Cursor = crDefault;
}

// ---------------------------------------------------------------------------
//	Create retrieval job, flag cryovials for removal and set destination
// ---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::btnSaveListClick( TObject *Sender ) {
	String error;
	try {
		LPDbBoxType * boxType = getBoxType( );
		if( boxType == NULL ) {
			return;
		}
		LCDbCryoJob * job = createJob( );
		if( job == NULL ) {
			return;
		}
		Screen->Cursor = crSQLWait;
		createBoxes( *job, *boxType );

	} catch( Exception & ex ) {
		error = ex.Message;
	}
	Screen->Cursor = crDefault;
	if( error.IsEmpty( ) ) {
		ModalResult = mrOk;
	} else {
		Application->MessageBox( error.c_str( ), NULL, MB_OK );
	}
}

// ---------------------------------------------------------------------------
//	ask the user to select or create a box type to hold all the cryovials
// ---------------------------------------------------------------------------

LPDbBoxType * TfrmRetrieveMain::getBoxType( ) {
	std::set < int > aliquots, boxes, projects, tubes;
	for( const GridEntry & ge : rows ) {
		aliquots.insert( ge.aid );
		boxes.insert( ge.bid );
		projects.insert( ge.pid );
	}
	if( aliquots.size( ) > 3 ) {
		throw Exception( "Cannot include more than three aliquot types" );
	}

	LQuery cq( LIMSDatabase::getCentralDb( ) );
	LPDbBoxNames boxList;
	for( int boxID : boxes ) {
		const LPDbBoxName * box = boxList.readRecord( cq, boxID );
		int tubeSizeID = 0;
		if( box != NULL ) {
			const LCDbBoxSize * size = box->getLayout( );
			if( size != NULL ) {
				tubeSizeID = size->getTubeType( );
			}
		}
		tubes.insert( tubeSizeID );
	}
	if( tubes.size( ) != 1 || tubes.count( 0 ) == 1 ) {
		throw Exception( "Tubes must all be the same size" );
	}

	LPDbBoxType *boxType = NULL;
	frmNewBoxType->init( aliquots, *tubes.begin( ) );
	if( frmNewBoxType->ShowModal( ) == mrOk ) {
		boxType = frmNewBoxType->getDetails( );
	}
	if( boxType != NULL ) {
		if( projects.size( ) > 1 ) {
			boxType->setProjectCID( 0 );
		} else if( boxType->getID( ) == 0 ) {
			boxType->setProjectCID( *projects.begin( ) );
		}
		for( int projID : projects ) {
			LQuery pq( LIMSDatabase::getProjectDb( projID ) );
			if( !boxType->saveRecord( pq, cq ) ) {
				throw Exception( "Cannot create selected box type" );
			}
		}
	}
	return boxType;
}

// ---------------------------------------------------------------------------
//	create a retrieval job record, including a unique box set number
// ---------------------------------------------------------------------------

LCDbCryoJob * TfrmRetrieveMain::createJob( ) {
	LCDbCryoJob *job = NULL;
	frmNewJob->init( LCDbCryoJob::SAMPLE_RETRIEVAL );
	if( frmNewJob->ShowModal( ) == mrOk ) {
		job = frmNewJob->getDetails( );
	}
	if( job != NULL ) {
		std::set < int > projects;
		for( const GridEntry & ge : rows ) {
			projects.insert( ge.pid );
		}
		job->setProjectID( projects.size( ) == 1 ? *projects.begin( ) : 0 );
		job->setPrimaryAliquot( getAliquotTypeID( CmbAliquot1 ) );
		job->setSecondaryAliquot( getAliquotTypeID( CmbAliquot2 ) );
		if( !job->saveRecord( LIMSDatabase::getCentralDb( ) ) ) {
			throw Exception( "Cannot create retrieval job record" );
		}
	}
	return job;
}

// ---------------------------------------------------------------------------
//	Create a set of boxes and allocate the cryovials to them, in order
// ---------------------------------------------------------------------------

void TfrmRetrieveMain::createBoxes( const LCDbCryoJob & job, const LPDbBoxType & boxType ) {
	struct Box {
		LPDbBoxName record;
		std::set<int> projects;
	} current;
	std::map<short,Box> boxes;
	short size = boxType.getCapacity();
	if( size < 1 ) {
		size = 100;		// assume MVE boxes
	}
	// work out which projects each box belong to
	progress->Position = 0;
	for( const GridEntry & ge : rows ) {
		short boxNumber = (ge.new_pos-1) / size;
		boxes[ boxNumber ].projects.insert( ge.pid );
	}
	for( auto & entry : boxes ) {
		const std::set<int> & projList = entry.second.projects;
		int projID = projList.size() == 1 ? *projList.begin() : 0;
		entry.second.record.setProjectCID( projID );
	}
	// create records in central and project databases
	LQuery cq( LIMSDatabase::getCentralDb( ) );
	for( auto & entry : boxes ) {
		const std::set<int> & projList = entry.second.projects;
		auto pi = projList.begin();
		LQuery pq1( LIMSDatabase::getProjectDb( *pi ) );
		LPDbBoxName & box = entry.second.record;
		if( !box.create( boxType, job.getBoxSet( ), pq1, cq ) ) {
			throw Exception( "Cannot create destination box" );
		}
		while( ++pi != projList.end() ) {
			LQuery pq( LIMSDatabase::getProjectDb( *pi ) );
			if( !box.saveRecord( pq, cq ) ) {
				throw Exception( "Cannot copy destination box" );
			}
		}
	}
	// mark the cryovials for removal into the new boxes
	progress->Max = rows.size( );
	StoreDAO dao;
	for( const GridEntry & ge : rows ) {
		short boxNumber = (ge.new_pos-1) / size;
		const LPDbBoxName & box = boxes[ boxNumber ].record;
		short position = ge.new_pos - (boxNumber * size);
		LQuery pq( LIMSDatabase::getProjectDb( ge.pid ) );
		if( dao.addToRetrieval( job.getID( ), ge.cid, ge.pid, box.getID(), position ) ) {
			progress->StepIt( );
		} else {
			throw Exception( "Cannot add cryovial(s)" );
		}
	}
}

// ---------------------------------------------------------------------------
//	store current cryovial data, record_number gives position in list
// ---------------------------------------------------------------------------

GridEntry::GridEntry( const ROSETTA & details, short position )
	: sid( details.getInt( "sample_id" ) ), sample( details.getString( "barcode" ) ),
	cid( details.getInt( "cryovial_id" ) ), cryovial( details.getString( "cryovial_barcode" ) ),
	aid( details.getInt( "aliquot_type_cid" ) ), pid( details.getInt( "project_cid" ) ),
	bid( details.getIntDefault( "box_cid", 0 ) ), old_box( details.getString( "box_name" ) ),
	old_pos( details.getIntDefault( "cryovial_position", -1 ) ), new_pos( position ), shelf( 0 ),
	location( 0 ), rack_pos( 0 ), slot( 0 ) {
	static unsigned nextRecord = 1;
	record_number = nextRecord++;
	aliquot = LCDbObjects::records( ).get( aid ).getName( );
}

// ---------------------------------------------------------------------------

void GridEntry::copyLocation( const ROSETTA & row ) {
	vessel = row.getString( "vessel_name" );
	shelf = row.getInt( "shelf_number" );
	rack_pos = row.getInt( "rack_pos" );
	structure = row.getString( "structure" );
	slot = row.getInt( "slot_position" );
	location = row.getInt( "tank_pos" );
	site = row.getString( "site_name" );
}

// ---------------------------------------------------------------------------

void GridEntry::copyLocation( const GridEntry & other ) {
	vessel = other.vessel;
	shelf = other.shelf;
	rack_pos = other.rack_pos;
	structure = other.structure;
	slot = other.slot;
	location = other.location;
	site = other.site;
}

// ---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::btnClrSortClick( TObject *Sender ) {
	sortList.clear();
	displaySortedList();
}

// ---------------------------------------------------------------------------
//	sort the cryovial list or restore the original order
// ---------------------------------------------------------------------------

void TfrmRetrieveMain::displaySortedList() {
	std::sort( rows.begin( ), rows.end( ), sortList );
	if( rgDestOrder->ItemIndex == SORTED ) {
		for( short n = 0; n < rows.size(); n ++ ) {
			rows[ n ].new_pos = n + 1;
		}
	}
	updateDisplay( );
}

// ---------------------------------------------------------------------------

typedef const GridEntry & GER;

static int compareSample( GER i, GER j ) {
	return i.sample.compare( j.sample );
}

static int compareCryovial( GER i, GER j ) {
	return i.cryovial.compare( j.cryovial );
}

static int compareAliquot( GER i, GER j ) {
	return i.aliquot.compare( j.aliquot );
}

static int compareOldBox( GER i, GER j ) {
	return i.old_box.compare( j.old_box );
}

static int compareOldPos( GER i, GER j ) {
	return i.old_pos - j.old_pos;
}

static int compareNewPos( GER i, GER j ) {
	return i.new_pos - j.new_pos;
}

static int compareSite( GER i, GER j ) {
	return i.site.compare( j.site );
}

static int compareLocation( GER i, GER j ) {
	return i.location - j.location;
}

static int compareVessel( GER i, GER j ) {
	return i.vessel.compare( j.vessel );
}

static int compareShelf( GER i, GER j ) {
	return i.shelf - j.shelf;
}

static int compareSlot( GER i, GER j ) {
	return i.slot - j.slot;
}

static int compareStructure( GER i, GER j ) {
	return i.rack_pos - j.rack_pos;
}

// ---------------------------------------------------------------------------
//	add or reverse a comparator when the user clicks on a column heading
// ---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::grdSamplesFixedCellClick( TObject *Sender, int ACol, int ARow ) {
	int( *compare )( GER, GER );
	AnsiString heading = grdSamples->Cells[ ACol ][ ARow ];
	switch( ACol ) {
	case SAMPLE:
		compare = compareSample;
		break;
	case CRYOVIAL:
		compare = compareCryovial;
		break;
	case ALIQUOT:
		compare = compareAliquot;
		break;
	case VESSEL:
		compare = compareVessel;
		break;
	case LOCATION:
		compare = compareLocation;
		break;
	case SITE:
		compare = compareSite;
		break;
	case SHELF:
		compare = compareShelf;
		break;
	case SLOT_POS:
		compare = compareSlot;
		break;
	case STRUCTURE:
		compare = compareStructure;
		break;
	case OLD_BOX:
		compare = compareOldBox;
		break;
	case OLD_POS:
		compare = compareOldPos;
		break;
	case NEW_POS:
		compare = compareNewPos;
		break;
	default:
		return;
	}
	sortList.addColumn( compare, heading.c_str( ) );
	displaySortedList( );
}

// ---------------------------------------------------------------------------
// Add selected column to the sort.  Click on the same column twice to
// reverse the sort order.  Click again later to move it to the end
// ---------------------------------------------------------------------------

void TfrmRetrieveMain::Sorter::addColumn( int( *cf )( GER, GER ), const std::string & lbl ) {
	Column selected( cf, true, lbl );
	std::vector < Column > ::iterator ci = columns.begin( );
	while( ci != columns.end( ) && ci->fn != cf ) {
		ci++;
	}
	if( &( *ci ) == &( columns.back( ) ) ) {
		selected.ascending = !ci->ascending;
	}
	if( ci != columns.end( ) ) {
		columns.erase( ci );
	}
	columns.push_back( selected );
}

// ---------------------------------------------------------------------------
// Compare entries using columns in the sorter.  Return true if a < b.
// Use record_number if sorter is empty or selected values are equal.
// ---------------------------------------------------------------------------

bool TfrmRetrieveMain::Sorter:: operator( )( GER a, GER b ) const {
	for( std::vector < Column > ::const_iterator ci = columns.begin( ); ci != columns.end( ); ++ci )
	{
		int diff = ci->fn( a, b );
		if( diff != 0 ) {
			return ci->ascending ? diff < 0 : diff > 0;
		}
	}
	return a.record_number < b.record_number;
}

// ---------------------------------------------------------------------------
//	try to prevent the user from doing anything they shouldn't
// ---------------------------------------------------------------------------

void TfrmRetrieveMain::enableButtons( ) {
	std::string projName = AnsiString( cbProject->Text ).c_str( );
	const LCDbProject * pp = LCDbProjects::records( ).findByName( projName );
	if( pp == NULL ) {
		// user must select project before importing any data
		CmbAliquot1->Enabled = false;
		CmbAliquot2->Enabled = false;
		btnAddFile->Enabled = false;
	} else if( rgItemType->ItemIndex == BOXES ) {
		// box list cannot include secondary or position
		CmbAliquot1->Enabled = true;
		CmbAliquot2->Enabled = false;
		btnAddFile->Enabled = (CmbAliquot2->ItemIndex < 0) && (rgDestOrder->ItemIndex != FROM_FILE);
	} else {
		// sorting should not affect position of secondary
		CmbAliquot1->Enabled = true;
		CmbAliquot2->Enabled = true;
		btnAddFile->Enabled = (CmbAliquot2->ItemIndex < 0) || (rgDestOrder->ItemIndex != SORTED);
	}
	btnLocate->Enabled = !rows.empty( );
	btnClrSort->Enabled = !sortList.empty( );
	btnClearList->Enabled = !rows.empty( );
	btnSaveList->Enabled = !rows.empty( );
	btnAddRecords->Enabled = false;  	/// FIXME
}

// ---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::btnClearListClick( TObject *Sender ) {
	rows.clear( );
	sortList.clear( );
	updateDisplay( );
}

// ---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::rgItemTypeClick(TObject *Sender) {
	if( rgItemType->ItemIndex == BOXES ) {
		CmbAliquot1->Clear();
		CmbAliquot2->Clear();
	}
	enableButtons();
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::CmbAliquotChange(TObject *Sender) {
	enableButtons();
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::rgDestOrderClick(TObject *Sender) {
	enableButtons();
}

//---------------------------------------------------------------------------

