//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SampleEntry.h"
#include "RetrieveSamples.h"
#include "showsamples.h"
#include "StoreDAO.h"
#include "LCDbObject.h"

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
	grdSamples->ColCount = COL_COUNT;
	grdSamples->Cells[SAMPLE][0] = "Sample";
	grdSamples->Cells[CRYOVIAL][0] = "Cryovial";
	grdSamples->Cells[ALIQUOT][0] = "Aliquot";
	grdSamples->Cells[OLD_BOX][0] = "Old Box";
	grdSamples->Cells[OLD_POS][0] = "Position";
	grdSamples->Cells[STRUCTURE][0] = "Structure";
	grdSamples->Cells[SHELF][0] = "Shelf";
	grdSamples->Cells[VESSEL][0] = "Vessel";
	grdSamples->Cells[NEW_BOX][0] = "New Box";
	grdSamples->Cells[NEW_POS][0] = "Position";
	clearGrid();
	this->ActiveControl = RadIDType;
}

//---------------------------------------------------------------------------

void TfrmRetrieveMain::clearGrid()
{
	int minColWidth = grdSamples->Width / (COL_COUNT + 2);
	grdSamples->DefaultColWidth = minColWidth;
	int boxCols = (grdSamples->Width - 40) - (minColWidth * (COL_COUNT - 2));
	grdSamples->ColWidths[OLD_BOX] = boxCols / 2;
	grdSamples->ColWidths[NEW_BOX] = boxCols / 2;
	grdSamples->RowCount = 2;
	grdSamples->Rows[1]->Clear();
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::AddClick(TObject *Sender)
{
	/// FIXME: check primary aliquot set if sample or cryovial selected
	/// FIXME: check secondary aliquot doesn't match primary if set

	if( !OpenDialog1->Execute() ) {
		return;
	}
	std::unique_ptr< TStrings > idList( new TStringList );
	idList->LoadFromFile(OpenDialog1->FileName);

	int aid = 0;		/// FIXME - primary or secondary

	std::vector< ROSETTA > results;
	StoreDAO & dao = StoreDAO::records();
	Screen->Cursor = crSQLWait;
	progress -> Position = 0;
	progress -> Max = idList->Count;
	for( int i = 0; i < idList->Count; i++ ) {
		AnsiString id = idList->Strings[ i ];
		std::string sid, cid, box;
		switch (RadIDType->ItemIndex)
		{
			case 0: //sample
				sid = id.c_str();
				break;

			case 1: //cryovial
				cid = id.c_str();
				break;

			case 2: //box (query to be written )
				box = id.c_str();
				break;
		}
		if( !dao.loadCryovials( sid, cid, aid, results ) ) {
			String error = "No samples found for " + id;
			Application->MessageBox(error.c_str(), NULL, MB_OK);
		}
		for( int i = 0; i < results.size(); i ++ ) {
			rows.push_back( GridEntry( results[i] ) );
		}
		drawGrid();
		progress -> StepIt();
		Application -> ProcessMessages();
	}
	Screen->Cursor = crDefault;
}

//---------------------------------------------------------------------------

void TfrmRetrieveMain::drawGrid()
{
	for( int i = 0; i < rows.size(); i ++ ) {
		int row = i + 1;
		grdSamples->Cells[SAMPLE][row] = rows[i].sid.c_str();
		grdSamples->Cells[CRYOVIAL][row] = rows[i].cid.c_str();
		grdSamples->Cells[ALIQUOT][row] = rows[i].aid;
		grdSamples->Cells[OLD_BOX][row] = rows[i].old_box.c_str();
		grdSamples->Cells[OLD_POS][row] = rows[i].old_pos;
		grdSamples->Cells[NEW_BOX][row] = rows[i].new_box.c_str();
		grdSamples->Cells[NEW_POS][row] = rows[i].new_pos;
		grdSamples->Cells[STRUCTURE][row] = rows[i].structure.c_str();
		grdSamples->Cells[SHELF][row] = rows[i].shelf;
		grdSamples->Cells[VESSEL][row] = rows[i].vessel.c_str();
		grdSamples->RowCount = row + 1;
	}
}

//---------------------------------------------------------------------------

TfrmRetrieveMain::GridEntry::GridEntry( const ROSETTA & row )
: sid(row.getString("barcode")),
 cid(row.getString("cryovial_barcode")),
 aid(row.getInt("aliquot_type_cid")),
 old_pos(row.getInt("cryovial_position")),
 old_box(row.getString("box")),
 shelf(row.getInt("shelf_number")),
 vessel(row.getString("vessel")),
 structure(row.getString("structure")),
 new_pos( 0 )
{}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::Retrieve(TObject *Sender)
{
	frmRetrieved->init( NULL );
	frmRetrieved->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::cbaDropDown(TObject *Sender)
{
	TComboBox * cb = dynamic_cast< TComboBox * >( Sender );
	if( cb ) {
		cb->Clear();
		for( Range< LCDbObject > at = LCDbObjects::records(); at.isValid(); ++ at ) {
			if( at -> isActive() && at -> getObjectType() == LCDbObject::ALIQUOT_TYPE ) {
				cb->Items->Add( at -> getName().c_str() );
			}
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::cbProjectChange(TObject *Sender)
{
	LCDbProjects &projList = LCDbProjects::records( );
	AnsiString proj = cbProject->Text;
	const LCDbProject *selected = projList.findByName( proj.c_str() );
	if( selected != NULL ) {
		projList.setCurrent( *selected );
	}
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

typedef const TfrmRetrieveMain::GridEntry & GER;
static bool compareSample( GER i, GER j ) { return i.sid < j.sid; }
static bool compareCryovial( GER i, GER j ) { return i.cid < j.cid; }
static bool compareBox( GER i, GER j ) { return i.old_box < j.old_box; }
static bool compareShelf( GER i, GER j ) { return i.shelf < j.shelf; }
static bool compareVessel( GER i, GER j ) { return i.vessel < j.vessel; }
static bool compareStructure( GER i, GER j ) { return i.structure < j.structure; }

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
		case OLD_BOX:
			compare = compareBox;
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
		default:
			return;
	}
	std::sort( rows.begin(), rows.end(), compare );
    drawGrid();
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::FormResize(TObject *Sender)
{
	clearGrid();
	drawGrid();
}

//---------------------------------------------------------------------------

void __fastcall TfrmRetrieveMain::BtnDestClick(TObject *Sender)
{
	AnsiString proj = cbProject->Text;

	// fixme - get new ID and (probably) box type
	int boxID = 12345, pos = 1;

	char box_name[ 90 ];
	for( int i = 0; i < rows.size(); i ++ ) {
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

