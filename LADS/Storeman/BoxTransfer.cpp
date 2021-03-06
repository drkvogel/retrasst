/* ---------------------------------------------------------------------------
 *
 *	Modified 6/6/12, NG for C++Builder XE2
 *	28/11/12   	Split out BoxSummary to use Paul's thread code
 *  12/02/13	Allow user to sort boxes and edit box types
 --------------------------------------------------------------------------- */

#include <vcl.h>
#include <set>
#include <sstream>

#include "LCDbProject.h"
#include "BoxTransfer.h"
#include "SMLogin.h"
#include "LDbBoxSize.h"
#include "StoreUtil.h"
#include "LDbSource.h"
#include "LPDbCryovial.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmBoxList *frmBoxList;

//---------------------------------------------------------------------------

struct BoxType : public LPDbBoxType {
	bool selected;
	BoxType( const LPDbBoxType &other ) : LPDbBoxType( other ), selected( false )
	{}
};

//---------------------------------------------------------------------------

__fastcall TfrmBoxList::TfrmBoxList( TComponent *Owner ) : TForm( Owner ) {
	sgBoxNames->Cells[ 0 ][ 0 ] = "box name";
	sgBoxNames->Cells[ 1 ][ 0 ] = "% analysed";
}

//---------------------------------------------------------------------------
// 	Allow user to select a project; show box types from the current one
//---------------------------------------------------------------------------

void __fastcall TfrmBoxList::FormShow( TObject *Sender ) {
	cbProject->Clear( );
	for( Range< LCDbProject > pr = LCDbProjects::records( ); pr.isValid( ); ++pr ) {
		if( pr->isInCurrentSystem() && pr->isActive() && !pr->isCentral() ) {
			cbProject->Items->Add( pr->getName( ).c_str( ) );
		}
		if( pr->getID( ) == LCDbProjects::getCurrentID( ) ) {
			init( &( *pr ) );
		}
	}
	FormResize( Sender );
}

//---------------------------------------------------------------------------

void __fastcall TfrmBoxList::FormHide( TObject *Sender ) {
	timer->Enabled = false;
	analyses.clear();
}

//---------------------------------------------------------------------------

void __fastcall TfrmBoxList::cbProjectChange( TObject *Sender ) {
	std::string proj = AnsiString( cbProject->Text ).c_str();
	init( LCDbProjects::records( ).findByName( proj ) );
}

//---------------------------------------------------------------------------
//	Switch to the selected project; list box types and box names
//---------------------------------------------------------------------------

void TfrmBoxList::init( const LCDbProject *selected ) {
	timer->Enabled = false;
	LCDbProjects &projList = LCDbProjects::records( );
	if( selected == NULL ) {
		selected = projList.findByID( LCDbProjects::getCurrentID( ) );
	} else {
		projList.setCurrent( selected );
	}
	if( selected == NULL ) {
		cbProject->Text = "";
	} else {
		cbProject->Text = selected->getName().c_str();
	}
	analyses.restart( );
	listBoxTypes( );
	listBoxNames( );
	timer->Enabled = true;
}

//---------------------------------------------------------------------------
//	List box types that could be stored after any necessary analysis
//---------------------------------------------------------------------------

void TfrmBoxList::listBoxTypes( ) {
	int row = 1;
	types.clear();
	for( const LPDbBoxType & tr : LPDbBoxTypes::records() ) {
		if( (tr.getUse() == LPDbBoxType::SHORT_TERM || tr.getUse() == LPDbBoxType::LONG_TERM)
		 && (tr.getProjectCID() == 0 || tr.getProjectCID() == LCDbProjects::getCurrentID()) ) {
			sgBoxTypes->Cells[ TYPE ][ row ] = tr.getName().c_str();
			sgBoxTypes->Cells[ COUNT ][ row ] = tr.getExpectedUses();
			sgBoxTypes->Cells[ PLUS ][ row ] = "+";
			sgBoxTypes->Cells[ MINUS ][ row ] = "-";
			row ++;
			types.push_back( tr );
		}
	}
	if( row == 1 ) {
		for( int col = 0; col < NUM_COLS; col ++ ) {
			sgBoxTypes->Cells[ col ][ row ] = "";
		}
		row ++;
	}
	sgBoxTypes->RowCount = row;
}

//---------------------------------------------------------------------------
//	List selected boxes from current project (timer will start calculation)
//---------------------------------------------------------------------------

void TfrmBoxList::listBoxNames( ) {
	int row = 1;
	const LCDbProject * p = LCDbProjects::records().findByID( LCDbProjects::getCurrentID() );
	if( p != NULL && boxes.readFilled( LIMSDatabase::getProjectDb( p->getDbName() ) ) ) {
		std::set< int >typeIDs;
		for( const BoxType & ti : types ) {
			if( ti.selected ) {
				typeIDs.insert( ti.getID( ) );
			}
		}
		for( const LPDbBoxName& br : boxes ) {
			if( typeIDs.count( br.getTypeID( ) ) != 0 ) {
				sgBoxNames->Cells[ 0 ][ row ++ ] = br.getName( ).c_str( );
			}
		}
	}
	if( row == 1 ) {
		sgBoxNames->RowCount = 2;
		sgBoxNames->Cells[ 0 ][ row ] = "";
		sgBoxNames->Cells[ 1 ][ row ] = "";
	} else {
		sgBoxNames->RowCount = row;
		TGridRect selected;
		selected.Left = 0;
		selected.Right = 1;
		selected.Top = 1;
		selected.Bottom = row - 1;
		sgBoxNames->Selection = selected;
	}
}

//---------------------------------------------------------------------------
//	Export the current list of box names to a text file
//---------------------------------------------------------------------------

void __fastcall TfrmBoxList::btnExportClick( TObject *Sender ) {
	std::auto_ptr< TStrings >names( new TStringList( ) );
	TGridRect selected = sgBoxNames->Selection;
	for( int row = selected.Top; row <= selected.Bottom; ++row ) {
		String box = sgBoxNames->Cells[ 0 ][ row ];
		if( !box.IsEmpty( ) ) {
			names->Add( box );
		}
	}
	String message;
	if( names->Count == 0 ) {
		message = "No boxes selected";
	} else if( saveDialogue->Execute( ) ) {
		names->SaveToFile( saveDialogue->FileName );
		if( names->Count == 1 ) {
			message = "1 box name saved";
		} else {
			message = String( names->Count ) + " boxes saved";
		}
	}
	if( !message.IsEmpty( ) ) {
		Application->MessageBox( message.c_str( ), L"File created", MB_OK );
	}
}

//---------------------------------------------------------------------------
//	Print the highlighted boxes on the selected printer
//---------------------------------------------------------------------------

void __fastcall TfrmBoxList::btnPrintClick( TObject *Sender ) {
	if( printDialogue->Execute( ) ) {
		setPrintParams( );
		TGridRect selected = sgBoxNames->Selection;
		for( int row = selected.Top; row <= selected.Bottom; ++row ) {
			if( line == 0 ) {
				printHeader( );
			}
			AnsiString name = sgBoxNames->Cells[ 0 ][ row ];
			printText( name.c_str() );
			if( line < lastLine ) {
				line++;
			} else {
				printFooter( );
				line = 0;
			}
		}
		if( line > 0 ) {
			printFooter( );
		}
	}
}

//---------------------------------------------------------------------------
// 	Estimate line height etc. assuming 10 columns, 54 lines per page
//---------------------------------------------------------------------------

void TfrmBoxList::setPrintParams( ) {
	printer = Printer( );
	printer->Orientation = poPortrait;
	leftMargin = printer->PageWidth / 10;
	lineHeight = printer->PageHeight / 54;
	firstLine = 4;
	lastLine = 49;
	footer = 52;
	int lines = lastLine - firstLine + 1;
	pages = ( sgBoxNames->RowCount + lines - 1 ) / lines;
	page = 0;
	line = 0;
}

//---------------------------------------------------------------------------
//	Start a new page - FIXME: print something near the top of the page
//---------------------------------------------------------------------------

void TfrmBoxList::printHeader( ) {
	printer->BeginDoc( );
	page++;
	line = firstLine;
}

//---------------------------------------------------------------------------
//  Add footer at the bottom of the page and send it to the printer
//---------------------------------------------------------------------------

void TfrmBoxList::printFooter( ) {
	line = footer;
	const LCDbOperator &user = LCDbOperators::records( ).get( LCDbOperators::getCurrentID( ) );
	const LCDbProject &project = LCDbProjects::records( ).get( LCDbProjects::getCurrentID( ) );
	std::stringstream text;
	text << project.getName( ) << ", " << user.getDescription( ) << ", page " << page << " of " << pages;
	printText( text.str( ).c_str( ) );
	printer->EndDoc( );
}

//---------------------------------------------------------------------------
//  Print the cryovial position or vacutainer barcode from the grid
//---------------------------------------------------------------------------

void TfrmBoxList::printText( const std::string &text ) {
	printer->Canvas->TextOut( leftMargin, line *lineHeight, text.c_str() );
}

//---------------------------------------------------------------------------
//	Allow user to add boxes to the list by entering the number on it
//---------------------------------------------------------------------------

void __fastcall TfrmBoxList::btnAddBoxClick( TObject *Sender ) {
	LQuery pq( LIMSDatabase::getProjectDb() );
	std::string box = AnsiString( ebBoxNum->Text ).c_str();
	const LPDbBoxName *found = boxes.readRecord( pq, box );
	if( found == NULL ) {
		int num = ebBoxNum->Text.ToIntDef( 0 );
		if( num != 0 ) {
			found = boxes.readRecord( pq, -num );
			if( found == NULL ) {
				found = boxes.readRecord( pq, num );
			}
		}
	}
	if( found == NULL ) {
		String message = "Cannot find box " + ebBoxNum->Text;
		Application->MessageBox( message.c_str(), NULL, MB_ICONWARNING );
	} else {
		addBox( *found );
	}
}

//---------------------------------------------------------------------------

void TfrmBoxList::addBox( const LPDbBoxName &box ) {
	int row = 1;
	for( ; ; ++row ) {
		if( row >= sgBoxNames->RowCount ) {
			sgBoxNames->RowCount = row + 1;
			break;
		}
		if( sgBoxNames->Cells[ 0 ][ row ].IsEmpty( ) ) {
			break;
		}
	}
	sgBoxNames->Cells[ 0 ][ row ] = box.getName( ).c_str( );
}

//---------------------------------------------------------------------------
//	Update number of analyses - percentage updated by background thread
//---------------------------------------------------------------------------

void __fastcall TfrmBoxList::timerTimer( TObject *Sender ) {
	for( int row = 1; row < sgBoxNames->RowCount; row ++ ) {
		AnsiString name = sgBoxNames->Cells[ 0 ][ row ];
		sgBoxNames->Cells[ 1 ][ row ] = getPercentage( name.c_str() );
	}
}

//---------------------------------------------------------------------------

String TfrmBoxList::getPercentage( const std::string &boxName ) {
	if( boxName.empty( ) ) {
		return "";
	}
	int status = checkProgress( boxName );
	switch( status ) {
		case BoxSummary::BOX_NOT_FOUND:
			return "Box name?";
		case BoxSummary::BOX_TYPE_NOT_FOUND:
			return "Box type?";
		case BoxSummary::CONTENTS_NOT_FOUND:
			return "Cryovials?";
		case BoxSummary::NOT_YET_CHECKED:
		case BoxSummary::SPECIMEN_CHECKED:
			return "tbc";
		case BoxSummary::NO_ANALYSES_REQUIRED:
			return "n/a";
		case BoxSummary::READY_FOR_STORAGE:
			return "done";
		default:
			return String( status );
	}
}

//---------------------------------------------------------------------------

int TfrmBoxList::checkProgress( const std::string &boxName ) {
	const LPDbBoxName *box = boxes.find( boxName );
	if( box == NULL ) {
		return BoxSummary::BOX_NOT_FOUND;
	}
	for( std::vector< BoxType >::const_iterator ti = types.begin( ); ti != types.end( ); ++ti ) {
		if( ti->getID( ) == box->getTypeID( ) ) {
			return analyses.check( *box, ti->getExpectedUses( ) );
		}
	}
	return BoxSummary::BOX_TYPE_NOT_FOUND;
}

//---------------------------------------------------------------------------
//	Colour percentage to indicate how near each box is to being ready
//---------------------------------------------------------------------------

void __fastcall TfrmBoxList::sgBoxNamesDrawCell( TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State ) {
	TColor background = clWindow;
	if( ARow == 0 ) {
		background = clBtnFace;
	} else if( ACol == 0 && State.Contains( gdSelected ) ) {
		background = clHighlight;
	} else if( ACol == 1 && ARow < sgBoxNames->RowCount ) {
		AnsiString name = sgBoxNames->Cells[ 0 ][ ARow ];
		background = getColour( name.c_str() );
	}
	TCanvas *out = sgBoxNames->Canvas;
	out->Brush->Color = background;
	out->FillRect( Rect );
	out->TextOut( Rect.Left + 5, Rect.Top + 5, sgBoxNames->Cells[ ACol ][ ARow ] );
}

//---------------------------------------------------------------------------

TColor TfrmBoxList::getColour( const std::string &boxName ) {
	static const TColor green = TColor(0x00C000), amber = TColor(0x00C0FF), red = TColor(0x0000D0);
	if( boxName.empty( ) ) {
		return clWindow;
	}
	int percentage = checkProgress( boxName );
	switch( percentage ) {
		case BoxSummary::BOX_NOT_FOUND:
		case BoxSummary::BOX_TYPE_NOT_FOUND:
		case BoxSummary::CONTENTS_NOT_FOUND:
			return red;
		case BoxSummary::NOT_YET_CHECKED:
		case BoxSummary::SPECIMEN_CHECKED:
			return amber;
		case BoxSummary::NO_ANALYSES_REQUIRED:
		case BoxSummary::READY_FOR_STORAGE:
		case 100:
			return green;
		default:
			return percentage < 90 ? red : amber;
	}
}

//---------------------------------------------------------------------------
//	Update box list when user toggles box type or modifies expected analyses
//---------------------------------------------------------------------------

void __fastcall TfrmBoxList::sgBoxTypesSelectCell( TObject *Sender, int ACol, int ARow, bool &CanSelect ) {
	timer->Enabled = false;
	if( ARow > 0 && types.size( ) >= ARow ) {
		BoxType &clicked = types[ ARow - 1 ];
		int num = sgBoxTypes->Cells[ COUNT ][ ARow ].ToIntDef( 0 );
		switch( ACol ) {
			case TYPE:
				types[ ARow - 1 ].selected = !types[ ARow - 1 ].selected;
				break;
			case PLUS:
				setExpected( ARow, num + 1 );
				break;
			case MINUS:
				if( num > 0 ) {
					setExpected( ARow, num - 1 );
				}
		}
	}
	listBoxNames( );
	timer->Enabled = true;
}

//---------------------------------------------------------------------------
//	Allow user to modify the expected number of analyses before storage
//---------------------------------------------------------------------------

void TfrmBoxList::setExpected( int row, int expected )
{
	BoxType &clicked = types[ row - 1 ];
	clicked.setExpectedUses( expected );
	clicked.saveRecord( LIMSDatabase::getProjectDb(), LIMSDatabase::getCentralDb() );
	sgBoxTypes->Cells[ COUNT ][ row ] = expected;
}

//---------------------------------------------------------------------------
//	highlight selected box types (heading spans three columns)
//---------------------------------------------------------------------------

void __fastcall TfrmBoxList::sgBoxTypesDrawCell( TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State ) {
	TColor background = clWindow;
	String text = sgBoxTypes->Cells[ ACol ][ ARow ];
	if( ARow == 0 ) {
		background = clBtnFace;
		if( ACol == 0 ) {
			text = "box type";
		} else {
			Rect.Left = sgBoxTypes->ColWidths[ TYPE ] + 1;
			Rect.Right = Rect.Left + 3 * sgBoxTypes->DefaultColWidth;
			text = "analyses";
		}
	} else if( ACol == 0 && ARow <= types.size() && types[ ARow - 1 ].selected ) {
		background = clHighlight;
	}
	TCanvas *out = sgBoxTypes->Canvas;
	out->Brush->Color = background;
	out->FillRect( Rect );
	out->TextOut( Rect.Left + 3, Rect.Top + 5, text );
}

//---------------------------------------------------------------------------
//	sort the box types when the user clicks on a column heading
//---------------------------------------------------------------------------

static bool typeAscending( const BoxType &i, const BoxType &j ) {
	return i.getName() < j.getName();
}

static bool typeDescending( const BoxType &i, const BoxType &j ) {
	return i.getName() > j.getName();
}

static bool usesAscending( const BoxType &i, const BoxType &j ) {
	return i.getExpectedUses() < j.getExpectedUses();
}

static bool usesDescending( const BoxType &i, const BoxType &j ) {
	return i.getExpectedUses() > j.getExpectedUses();
}

//---------------------------------------------------------------------------

void __fastcall TfrmBoxList::sgBoxTypesFixedCellClick( TObject *Sender, int ACol, int ARow ) {
	if( ACol == 0 ) {
		if( typeOrder == typeAscending ) {
			typeOrder = typeDescending;
		} else {
			typeOrder = typeAscending;
		}
	} else {
		if( typeOrder == usesDescending ) {
			typeOrder = usesAscending;
		} else {
			typeOrder = usesDescending;
		}
	}
	std::stable_sort( types.begin( ), types.end( ), typeOrder );
	for( int i = 0, row = 1; row < sgBoxTypes->RowCount; i++, row++ ) {
		sgBoxTypes->Cells[ 0 ][ row ] = types[ i ].getName( ).c_str( );
		sgBoxTypes->Cells[ 1 ][ row ] = types[ i ].getExpectedUses( );
	}
}

//---------------------------------------------------------------------------

//	sort the boxes by name or colour, percentage and status
//---------------------------------------------------------------------------

struct BoxName {
	AnsiString name;
	int status;
};

//---------------------------------------------------------------------------

static bool nameAscending( const BoxName &i, const BoxName &j ) {
	return i.name < j.name;
}

static bool nameDescending( const BoxName &i, const BoxName &j ) {
	return j.name < i.name;
}

static bool stateAscending( const BoxName &i, const BoxName &j ) {
	return i.status < j.status;
}

static bool stateDescending( const BoxName &i, const BoxName &j ) {
	return j.status < i.status;
}

//---------------------------------------------------------------------------

void __fastcall TfrmBoxList::sgBoxNamesFixedCellClick( TObject *Sender, int ACol, int ARow ) {
	std::vector< BoxName > names;
	BoxName record;
	for( int row = 1; row < sgBoxNames->RowCount; row++ ) {
		record.name = sgBoxNames->Cells[ 0 ][ row ];
		record.status = checkProgress( record.name.c_str() );
		names.push_back( record );
	}
	if( ACol == 0 ) {
		if( boxOrder == nameAscending ) {
			boxOrder = nameDescending;
		} else {
			boxOrder = nameAscending;
		}
	} else {
		if( boxOrder == stateAscending ) {
			boxOrder = stateDescending;
		} else {
			boxOrder = stateAscending;
		}
	}
	std::sort( names.begin( ), names.end( ), boxOrder );
	int row = 1;
	for( std::vector< BoxName >::const_iterator ni = names.begin( ); ni != names.end( ); ++ni ) {
		sgBoxNames->Cells[ 0 ][ row++ ] = ni->name;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmBoxList::FormResize(TObject *Sender)
{
	sgBoxTypes->DefaultColWidth = 19;
	sgBoxTypes->ColWidths[ TYPE ] = 122;
	sgBoxNames->ColWidths[ 1 ] = 70;
	sgBoxNames->ColWidths[ 0 ] = sgBoxNames->Width - 92;
}

//---------------------------------------------------------------------------

