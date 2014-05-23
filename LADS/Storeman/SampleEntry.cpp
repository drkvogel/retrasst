//---------------------------------------------------------------------------

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

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmRetrieveMain *frmRetrieveMain;

//---------------------------------------------------------------------------

__fastcall TfrmRetrieveMain::TfrmRetrieveMain(TComponent* Owner)
	: TForm(Owner)
{}

//---------------------------------------------------------------------------

void TfrmRetrieveMain::init()
{
	resizeGrid();
	grdSamples->RowCount = 2;
	grdSamples->Cells[SAMPLE][0] = "Sample";
	grdSamples->Cells[CRYOVIAL][0] = "Cryovial";
	grdSamples->Cells[ALIQUOT][0] = "Aliquot";
	grdSamples->Cells[SITE][0] = "Site";
	grdSamples->Cells[LOCATION][0] = "Location";
	grdSamples->Cells[VESSEL][0] = "Vessel";
	grdSamples->Cells[SHELF][0] = "Shelf";
	grdSamples->Cells[STRUCTURE][0] = "Structure";
	grdSamples->Cells[SLOT_POS][0] = "Slot";
	grdSamples->Cells[OLD_POS][0] = "Old pos";
	grdSamples->Cells[OLD_BOX][0] = "Old box";
	grdSamples->Cells[NEW_BOX][0] = "New box";
	grdSamples->Cells[NEW_POS][0] = "New pos";
	grdSamples->Rows[1]->Clear();
	enableButtons();
}

//---------------------------------------------------------------------------

void TfrmRetrieveMain::resizeGrid()
{
	grdSamples->ColCount = COL_COUNT;
	int minColWidth = grdSamples->Width / (COL_COUNT + 4);
	grdSamples->DefaultColWidth = minColWidth;
	int boxCols = (grdSamples->ClientWidth - 10) - (minColWidth * (COL_COUNT - 2));
	grdSamples->ColWidths[OLD_BOX] = boxCols / 2;
	grdSamples->ColWidths[NEW_BOX] = boxCols / 2;
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::AddClick(TObject *Sender)
{
	if( !OpenDialog1->Execute() ) {
		return;
	}
	std::unique_ptr< TStrings > idList( new TStringList );
	idList->LoadFromFile(OpenDialog1->FileName);
	progress -> Position = 0;
	progress -> Max = idList->Count;

	Screen->Cursor = crSQLWait;
	StoreDAO dao;
	short source = rgItemType->ItemIndex;
	int primary = getAliquotTypeID( CmbAliquot1 ), secondary = getAliquotTypeID( CmbAliquot2 );
	const LCDbProject & proj = LCDbProjects::records().get( LCDbProjects::getCurrentID() );
	for( int i = 0; i < idList->Count; i++ ) {
		AnsiString id = idList->Strings[ i ];
		if( !id.IsEmpty() ) {
			std::vector<ROSETTA> results;
			if( !dao.loadCryovials( source, id.c_str(), primary, secondary, proj.getID(), results ) ) {
				String error = "No samples found for " + id;
				Application->MessageBox( error.c_str(), NULL, MB_OK );
			}
			for( unsigned i = 0; i < results.size(); i ++ ) {
				rows.push_back( GridEntry( results[i] ) );
			}
			drawGrid();
		}
		progress -> StepIt();
		Application -> ProcessMessages();
	}
	checkBoxTypes();
	enableButtons();
	Screen->Cursor = crDefault;
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::cbProjectChange(TObject *Sender)
{
	LCDbProjects &projList = LCDbProjects::records( );
	std::string proj = AnsiString( cbProject->Text ).c_str();
	const LCDbProject *selected = projList.findByName( proj );
	if( selected == NULL ) {
		rows.clear();
	} else {
		projList.setCurrent( *selected );
	}
	enableButtons();
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::cbProjectDropDown(TObject *Sender)
{
	cbProject->Clear();
	int selected = -1;
	for( Range< LCDbProject >pr = LCDbProjects::records( ); pr.isValid( ); ++pr ) {
		if( pr->isInCurrentSystem( ) && pr->isActive( ) && !pr->isCentral( ) ) {
			if( pr->getID( ) == LCDbProjects::getCurrentID( ) ) {
				selected = cbProject->Items->Count;
			}
			cbProject->Items->Add( pr->getName().c_str( ) );
		}
	}
	cbProject->ItemIndex = selected;
}

//---------------------------------------------------------------------------

void TfrmRetrieveMain::setValue( int col, int row, const std::string & value ) {
	grdSamples->Cells[ col ][ row ] = value.c_str();
}

//---------------------------------------------------------------------------

void TfrmRetrieveMain::setValue( int col, int row, short value ) {
	if( value == 0 ) {
		grdSamples->Cells[ col ][ row ] = " ";
	} else {
		grdSamples->Cells[ col ][ row ] = value;
	}
}

//---------------------------------------------------------------------------

void TfrmRetrieveMain::drawGrid()
{
	if( rows.empty() ) {
		grdSamples->RowCount = 2;
		grdSamples->Rows[1]->Clear();
	} else {
		int row = rows.size();
		grdSamples->RowCount = row;
		for( int i = row - 1; i >= 0; row = i -- ) {
			setValue( SAMPLE, row, rows[i].sample );
			setValue( CRYOVIAL, row, rows[i].cryovial );
			setValue( ALIQUOT, row, rows[i].aliquot );
			setValue( SITE, row, rows[i].site );
			setValue( LOCATION, row, rows[i].location );
			setValue( VESSEL, row, rows[i].vessel );
			setValue( SHELF, row, rows[i].shelf );
			setValue( STRUCTURE, row, rows[i].structure );
			setValue( SLOT_POS, row, rows[i].slot );
			setValue( OLD_BOX, row, rows[i].old_box );
			setValue( OLD_POS, row, rows[i].old_pos );
			setValue( NEW_BOX, row, rows[i].new_box );
			setValue( NEW_POS, row, rows[i].new_pos );
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::FormResize(TObject *Sender)
{
	resizeGrid();
	drawGrid();
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::btnDestinationClick(TObject *Sender)
{
	std::string content = AnsiString( cbBoxType->Text ).c_str();
	const LPDbBoxType * boxType = LPDbBoxTypes::records().find( content );
	if( boxType == NULL ) {
		throw Exception( "No matching formation type" );
	}
	short boxSet = 0;	//// FIXME: get box set from job entry

	LQuery cQuery = LIMSDatabase::getCentralDb();
	LQuery pQuery = LIMSDatabase::getProjectDb();
	LPDbBoxName box;
	for( std::vector<GridEntry>::iterator ge = rows.begin(); ge != rows.end(); ++ ge ) {
		if( !box.hasSpace() && !box.create( *boxType, boxSet, pQuery, cQuery ) ) {
			throw Exception( "Cannot create destination box" );
		}
		ge -> new_box = box.getName();
		ge -> new_pos = box.addCryovial( ge->cryovial );
		ge -> nid = box.getID();
	}
	drawGrid();
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::CmbAliquot1DropDown(TObject *Sender)
{
	populate( CmbAliquot1, CmbAliquot2 );
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::CmbAliquot2DropDown(TObject *Sender)
{
	populate( CmbAliquot2, CmbAliquot1 );
}

//---------------------------------------------------------------------------

void TfrmRetrieveMain::populate( TComboBox * target, TComboBox * other )
{
	std::set< int > types = LPDbCryovials::getAliquotTypes( LIMSDatabase::getProjectDb() );
	types.erase( getAliquotTypeID( other ) );
	target->Clear();
	for( Range< LCDbObject > at = LCDbObjects::records(); at.isValid(); ++ at ) {
		if( types.count( at->getID() ) != 0 ) {
			target->Items->Add( at -> getName().c_str() );
		}
	}
}

//---------------------------------------------------------------------------

int TfrmRetrieveMain::getAliquotTypeID( TComboBox * cb )
{
	const LCDbObject * aliquot = NULL;
	std::string content = AnsiString( cb->Text ).c_str();
	if( !content.empty() ) {
		aliquot = LCDbObjects::records().find( content, LCDbObject::ALIQUOT_TYPE );
	}
	return aliquot == NULL ? 0 : aliquot -> getID();
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::cbBoxTypeDropDown(TObject *Sender)
{
	checkBoxTypes();
}

//---------------------------------------------------------------------------

void TfrmRetrieveMain::checkBoxTypes()
{
	AnsiString current = cbBoxType->Text.Trim();
	std::set< int > required;
	for( const GridEntry & ge : rows ) {
		required.insert( ge.aid );
	}
	cbBoxType -> Clear();
	int selected = -1;
	for( const LPDbBoxType & bt : LPDbBoxTypes::records() ) {
		if( bt.getProjectCID() == 0 || bt.getProjectCID() == LCDbProjects::getCurrentID() ) {
			bool include = true;
			for( int ge : required ) {
				if( !bt.hasAliquot( ge ) ) {
					include = false;
				}
			}
			if( include ) {
				AnsiString type = bt.getName().c_str();
				if( current.AnsiCompareIC( type ) == 0 ) {
					selected = cbBoxType -> Items -> Count;
				}
				cbBoxType -> Items -> Add( type );
			}
		}
	}
	cbBoxType -> ItemIndex = selected;
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::btnLocateClick(TObject *Sender)
{
	std::map<int, const GridEntry *> boxes;
	progress -> Position = 0;
	progress -> Max = rows.size();
	for( std::vector<GridEntry>::iterator ge = rows.begin(); ge != rows.end(); ++ ge ) {
		int boxID = ge->oid;
		std::map<int, const GridEntry *>::const_iterator found = boxes.find( boxID );
		if( found != boxes.end() ) {
			ge->copyLocation( *(found->second) );
		} else {
			ROSETTA result;
			if( StoreDAO().findBox( boxID, result ) ) {
				ge->copyLocation( result );
			}
			boxes[ boxID ] = &(*ge);
		}
		progress -> StepIt();
		drawGrid();
		Application -> ProcessMessages();
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::btnSaveListClick(TObject *Sender)
{
	//// fixme - check radIDType: could be box retrieval

	frmNewJob -> init( LCDbCryoJob::SAMPLE_RETRIEVAL );

	int primary = getAliquotTypeID( CmbAliquot1 ), secondary = getAliquotTypeID( CmbAliquot2 );
	String error;
	if( frmNewJob -> ShowModal() == mrOk ) {
		Screen->Cursor = crSQLWait;
		if( !frmNewJob -> createJob( LCDbProjects::getCurrentID(), primary, secondary ) ) {
			error = "Cannot create job record";
		} else {
			const LCDbCryoJob & job = frmNewJob -> getDetails();
			StoreDAO dao;
			for( std::vector<GridEntry>::const_iterator ge = rows.begin(); ge != rows.end(); ++ ge ) {
				if( !dao.addToRetrieval( job.getID(), ge->cid, LCDbProjects::getCurrentID(), ge->nid, ge->new_pos ) ) {
					if( error.IsEmpty() ) {
						error = "Cannot add cryovial(s) ";
					} else {
						error += ", ";
					}
					error += ge->cryovial.c_str();
				}
			}
		}
		Screen->Cursor = crDefault;
	}
	if( error.IsEmpty() ) {
		this -> ModalResult = mrOk;
	} else {
		Application->MessageBox( error.c_str(), NULL, MB_OK );
	}
}

//---------------------------------------------------------------------------

GridEntry::GridEntry( const ROSETTA & row )
: sid(row.getInt("sample_id")),sample(row.getString("barcode")),
  cid(row.getInt("cryovial_id")),cryovial(row.getString("cryovial_barcode")),
  aid(row.getInt("aliquot_type_cid")),aliquot(row.getString("aliquot")),
  oid(row.getInt("box_cid")), old_box(row.getString("box_name")),
  shelf( 0 ), location( 0 ), rack_pos( 0 ), slot( 0 ), nid( 0 ), new_pos( 0 )
{
	static unsigned nextRecord = 1;
	record_number = nextRecord ++;
	old_pos = row.getIntDefault("cryovial_position", -1);
}

//---------------------------------------------------------------------------

void GridEntry::copyLocation( const ROSETTA & row ) {
	vessel = row.getString("vessel_name");
	shelf = row.getInt("shelf_number");
	rack_pos = row.getInt("rack_pos");
	structure = row.getString("structure");
	slot = row.getInt("slot_position");
	location = row.getInt("tank_pos");
	site = row.getString("site_name");
}

//---------------------------------------------------------------------------

void GridEntry::copyLocation( const GridEntry & other ) {
	vessel = other.vessel;
	shelf = other.shelf;
	rack_pos = other.rack_pos;
	structure = other.structure;
	slot = other.slot;
	location = other.location;
	site = other.site;
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::btnClrSortClick(TObject *Sender)
{
	lbSortCols->Clear();
	sortList.clear();
	std::sort( rows.begin(), rows.end(), sortList );
	drawGrid();
}

//---------------------------------------------------------------------------

typedef const GridEntry & GER;
static int compareSample( GER i, GER j ) { return i.sample.compare( j.sample ); }
static int compareCryovial( GER i, GER j ) { return i.cryovial.compare( j.cryovial ); }
static int compareAliquot( GER i, GER j ) { return i.aliquot.compare( j.aliquot ); }
static int compareOldBox( GER i, GER j ) { return i.old_box.compare( j.old_box ); }
static int compareOldPos( GER i, GER j ) { return i.old_pos - j.old_pos; }
static int compareNewBox( GER i, GER j ) { return i.new_box.compare( j.new_box ); }
static int compareNewPos( GER i, GER j ) { return i.new_pos - j.new_pos; }
static int compareSite( GER i, GER j ) { return i.site.compare( j.site ); }
static int compareLocation( GER i, GER j ) { return i.location - j.location; }
static int compareVessel( GER i, GER j ) { return i.vessel.compare( j.vessel ); }
static int compareShelf( GER i, GER j ) { return i.shelf - j.shelf; }
static int compareSlot( GER i, GER j ) { return i.slot - j.slot; }
static int compareStructure( GER i, GER j ) { return i.rack_pos - j.rack_pos; }

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::grdSamplesFixedCellClick(TObject *Sender, int ACol, int ARow)
{
	AnsiString heading = grdSamples->Cells[ACol][ARow];
	int (*compare)( GER, GER );
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
		case NEW_BOX:
			compare = compareNewBox;
			break;
		case NEW_POS:
			compare = compareNewPos;
			break;
		default:
			return;
	}

	sortList.addColumn( compare, heading.c_str() );
	lbSortCols->Clear();
	for( std::vector< Column >::const_iterator ci = sortList.columns.begin(); ci != sortList.columns.end(); ++ ci ) {
		AnsiString order = ci->ascending ? " ascending" : " descending";
		lbSortCols->Items->Add( ci->label.c_str() + order );
	}
	std::sort( rows.begin(), rows.end(), sortList );
	drawGrid();
}

//---------------------------------------------------------------------------
//	Add selected column to the sort.  Click on the same column twice to
//	reverse the sort order.  Click again later to move it to the end
//---------------------------------------------------------------------------

void TfrmRetrieveMain::Sorter::addColumn( int (*cf)( GER, GER ), const std::string & lbl ) {
	Column selected( cf, true, lbl );
	std::vector< Column >::iterator ci = columns.begin();
	while( ci != columns.end() && ci->fn != cf ) {
		ci ++;
	}
	if( &(*ci) == &(columns.back()) ) {
		selected.ascending = !ci->ascending;
	}
	if( ci != columns.end() ) {
		columns.erase( ci );
	}
	columns.push_back( selected );
}

//---------------------------------------------------------------------------
//	Compare entries using columns in the sorter.  Return true if a < b.
//	Use record_number if sorter is empty or selected values are equal.
//---------------------------------------------------------------------------

bool TfrmRetrieveMain::Sorter::operator() ( GER a, GER b ) const {
	for( std::vector< Column >::const_iterator ci = columns.begin(); ci != columns.end(); ++ ci ) {
		int diff = ci->fn( a, b );
		if( diff != 0 ) {
			return ci->ascending ? diff < 0 : diff > 0;
		}
	}
	return a.record_number < b.record_number;
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::btnNewContentClick(TObject *Sender)
{
	std::string proj = AnsiString( cbProject->Text ).c_str();
	const LCDbProject * pp = LCDbProjects::records().findByName( proj );
	if( frmNewBoxType -> ShowModal() == mrOk ) {
		LPDbBoxType created = frmNewBoxType -> getDetails();
		created.setUse( LPDbBoxType::ANALYSIS );
		created.saveRecord( LIMSDatabase::getProjectDb( pp->getID() ), LIMSDatabase::getCentralDb() );
		cbBoxType->Text = created.getName().c_str();
		checkBoxTypes();
	}
	enableButtons();
}

//---------------------------------------------------------------------------

void TfrmRetrieveMain::enableButtons() {
	std::string projName = AnsiString( cbProject->Text ).c_str();
	const LCDbProject * pp = LCDbProjects::records().findByName( projName );
	std::string btName = AnsiString( cbBoxType->Text ).c_str();
	const LPDbBoxType * bt = LPDbBoxTypes::records().find( btName );
	bool read = !rows.empty();
	bool proj = (pp != NULL);
	CmbAliquot1->Enabled = proj;
	CmbAliquot2->Enabled = proj;
	cbBoxType->Enabled = proj;
	btnAddFile->Enabled = proj;
	// fixme: btnAddRecords->Enabled = proj;
	btnLocate->Enabled = proj && read;
	btnNewContent->Enabled = proj;
	btnDestination->Enabled = read && (bt != NULL);
	btnSaveList->Enabled = read && (bt != NULL);  	/// fixme - needs destination
	btnClrSort->Enabled = !sortList.columns.empty();
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::cbBoxTypeChange(TObject *Sender)
{
	enableButtons();
}

//---------------------------------------------------------------------------

