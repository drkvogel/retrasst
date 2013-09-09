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
	grdSamples->Cells[OLD_BOX][0] = "Old box";
	grdSamples->Cells[OLD_POS][0] = "Old pos";
	grdSamples->Cells[STRUCTURE][0] = "Structure";
	grdSamples->Cells[SHELF][0] = "Shelf";
	grdSamples->Cells[VESSEL][0] = "Vessel";
	grdSamples->Cells[NEW_BOX][0] = "New box";
	grdSamples->Cells[NEW_POS][0] = "New pos";
	grdSamples->Rows[1]->Clear();
	ActiveControl = RadIDType;
}

//---------------------------------------------------------------------------

void TfrmRetrieveMain::resizeGrid()
{
	grdSamples->ColCount = COL_COUNT;
	int minColWidth = grdSamples->Width / (COL_COUNT + 3);
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
	int primary = getTypeID( CmbAliquot1 );
	int secondary = getTypeID( CmbAliquot2 );

	Screen->Cursor = crSQLWait;
	StoreDAO dao;
	progress -> Position = 0;
	progress -> Max = idList->Count;
	for( int i = 0; i < idList->Count; i++ ) {
		AnsiString id = idList->Strings[ i ];
		std::string specimen, cryovial, box;
		switch (RadIDType->ItemIndex)
		{
			case 0: //sample
				specimen = id.c_str();
				break;

			case 1: //cryovial
				cryovial = id.c_str();
				break;

			case 2: //box (query to be written )
				box = id.c_str();
				break;
		}
		std::vector< ROSETTA > results;
		if( !dao.loadCryovials( specimen, cryovial, primary, secondary, LCDbProjects::getCurrentID(), results ) ) {
			String error = "No samples found for " + id;
			Application->MessageBox(error.c_str(), NULL, MB_OK);
		}
		for( unsigned i = 0; i < results.size(); i ++ ) {
			rows.push_back( GridEntry( results[i] ) );
		}
		progress -> StepIt();
		drawGrid();
		Application -> ProcessMessages();
	}
	btnLocate->Enabled = true;
	btnDestination->Enabled = true;
	Screen->Cursor = crDefault;
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::cbProjectChange(TObject *Sender)
{
	LCDbProjects &projList = LCDbProjects::records( );
	AnsiString proj = cbProject->Text;
	const LCDbProject *selected = projList.findByName( proj.c_str() );
	bool ready = false;
	if( selected != NULL ) {
		projList.setCurrent( *selected );
		ready = true;
	}
	CmbAliquot1->Enabled = ready;
	CmbAliquot2->Enabled = ready;
	btnAddFile->Enabled = ready;
	btnAddRecords->Enabled = ready;
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::cbProjectDropDown(TObject *Sender)
{
	int selected = -1;
	for( Range< LCDbProject >pr = LCDbProjects::records( ); pr.isValid( ); ++pr ) {
		if( pr->isValid( ) && pr->isActive( ) && !pr->isCentral( ) ) {
			if( pr->getID( ) == LCDbProjects::getCurrentID( ) ) {
				selected = cbProject->Items->Count;
			}
			cbProject->Items->Add( pr->getName( ).c_str( ) );
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
			setValue( OLD_BOX, row, rows[i].old_box );
			setValue( OLD_POS, row, rows[i].old_pos );
			setValue( NEW_BOX, row, rows[i].new_box );
			setValue( NEW_POS, row, rows[i].new_pos );
			setValue( VESSEL, row, rows[i].vessel );
			setValue( SHELF, row, rows[i].shelf );
			setValue( STRUCTURE, row, rows[i].structure );
		}
	}
}

//---------------------------------------------------------------------------

TfrmRetrieveMain::GridEntry::GridEntry( const ROSETTA & row )
: sid(row.getInt("sample_id")),sample(row.getString("barcode")),
  cid(row.getInt("cryovial_id")),cryovial(row.getString("cryovial_barcode")),
  aid(row.getInt("aliquot_type_cid")),aliquot(row.getString("aliquot")),
  bid(row.getInt("box_cid")), old_box(row.getString("box_name")),
  old_pos(row.getInt("cryovial_position")), shelf( 0 ), rack_pos( 0 ), new_pos( 0 )
{}

//---------------------------------------------------------------------------

void TfrmRetrieveMain::GridEntry::copyLocation( const ROSETTA & row ) {
	vessel = row.getString("vessel");
	shelf = row.getInt("shelf_number");
	rack_pos = row.getInt("rack_pos");
	structure = row.getString("structure");
	old_pos = row.getInt("slot_position");
}

//---------------------------------------------------------------------------

void TfrmRetrieveMain::GridEntry::copyLocation( const GridEntry & other ) {
	vessel = other.vessel;
	shelf = other.shelf;
	rack_pos = other.rack_pos;
	structure = other.structure;
	old_pos = other.old_pos;
}

//---------------------------------------------------------------------------

typedef const TfrmRetrieveMain::GridEntry & GER;
static bool compareSample( GER i, GER j ) { return i.sample < j.sample; }
static bool compareCryovial( GER i, GER j ) { return i.cryovial < j.cryovial; }
static bool compareAliquot( GER i, GER j ) { return i.aliquot < j.aliquot; }
static bool compareOldBox( GER i, GER j ) { return i.old_box < j.old_box; }
static bool compareOldPos( GER i, GER j ) { return i.old_pos < j.old_pos; }
static bool compareNewBox( GER i, GER j ) { return i.new_box < j.new_box; }
static bool compareNewPos( GER i, GER j ) { return i.new_pos < j.new_pos; }
static bool compareShelf( GER i, GER j ) { return i.shelf < j.shelf; }
static bool compareVessel( GER i, GER j ) { return i.vessel < j.vessel; }
static bool compareStructure( GER i, GER j ) { return i.rack_pos < j.rack_pos; }

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::grdSamplesFixedCellClick(TObject *Sender, int ACol, int ARow)
{
	bool (*compare)( GER, GER );
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
		case SHELF:
			compare = compareShelf;
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
	std::stable_sort( rows.begin(), rows.end(), compare );
	lbSortCols->Items->Add( grdSamples->Cells[ACol][ARow] );
	drawGrid();
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
	AnsiString proj = cbProject->Text;

	// fixme - get new ID and (probably) box type
	int boxID = 12345, pos = 1;

	char box_name[ 90 ];
	for( unsigned i = 0; i < rows.size(); i ++ ) {
		std::sprintf( box_name, "%s %d", proj.c_str(), boxID );
		rows[i].new_box = box_name;
		rows[i].new_pos = pos;
		if( pos == 100 ) {	// fixme: check box size
			boxID ++; pos = 1;
		} else {
			pos ++;
		}
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
	std::set< int > types = LPDbCryovials::getAliquotTypes( Util::projectQuery() );
	types.erase( getTypeID( other ) );
	target->Clear();
	for( Range< LCDbObject > at = LCDbObjects::records(); at.isValid(); ++ at ) {
		if( types.count( at->getID() ) != 0 ) {
			target->Items->Add( at -> getName().c_str() );
		}
	}
}

//---------------------------------------------------------------------------

int TfrmRetrieveMain::getTypeID( TComboBox * cb )
{
	const LCDbObject * aliquot = NULL;
	AnsiString selected = cb->Text.Trim();
	if( !selected.IsEmpty() ) {
		aliquot = LCDbObjects::records().find( selected.c_str(), LCDbObject::ALIQUOT_TYPE );
	}
	return aliquot == NULL ? 0 : aliquot -> getID();
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::btnLocateClick(TObject *Sender)
{
	std::map<int, const GridEntry *> boxes;
	ROSETTA result;
	StoreDAO dao;
	progress -> Max = rows.size();
	progress -> Position = 0;
	for( std::vector<GridEntry>::iterator ge = rows.begin(); ge != rows.end(); ++ ge ) {
		std::map<int, const GridEntry *>::const_iterator found = boxes.find( ge->bid );
		if( found != boxes.end() ) {
			ge->copyLocation( *(found->second) );
		} else {
			if( dao.findBox( ge->bid, LCDbProjects::getCurrentID(), result ) ) {
				ge->copyLocation( result );
			}
			boxes[ ge->bid ] = &(*ge);
		}
		progress -> StepIt();
		drawGrid();
		Application -> ProcessMessages();
	}
}

//---------------------------------------------------------------------------

