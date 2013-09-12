//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "LCDbProject.h"
#include "AddSpecimens.h"
#include "LPDbProfile.h"
#include "LIMSDatabase.h"
#include "StoreUtil.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "dvSelector"
#pragma resource "*.dfm"

TfrmAddSpecimens *frmAddSpecimens;

//---------------------------------------------------------------------------

__fastcall TfrmAddSpecimens::TfrmAddSpecimens(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TfrmAddSpecimens::FormShow(TObject *Sender)
{
	selector -> updateDisplay();
	Timer1 -> Enabled = true;
}

//---------------------------------------------------------------------------

void __fastcall TfrmAddSpecimens::BitBtn2Click(TObject *Sender)
{
	Screen->Cursor = crSQLWait;
	samples.save( LIMSDatabase::getProjectDb(), LDbSource::NEW_ENTRY );
	Screen->Cursor = crDefault;
}

//---------------------------------------------------------------------------
//	Accept barcodes; set up screen to select descriptors and profile
//---------------------------------------------------------------------------

static const short barcode_column = 0, received_column = 1, value_columns = 2;

bool TfrmAddSpecimens::init( TStrings * barcodes )
{
	sgSpecimens -> Cells[ barcode_column ][ 0 ] = "barcode";
	sgSpecimens -> Cells[ received_column ][ 0 ] = "first received";
	sgSpecimens -> ColWidths[ received_column ] = 120;
	int column = value_columns;
	selector -> prepare( true );
	for( Range< LPDbDescriptor > dr = LPDbDescriptors::records(); dr.isValid(); ++ dr ) {
		if( !dr -> getSpecimenField().empty()
		 && dr -> getSpecimenField() != "external_name"
		 && dr -> getSpecimenField() != "barcode" ) {
			if( dr -> hasValues() ) {
				selector -> allow( *dr );
			}
			sgSpecimens -> Cells[ column ++ ][ 0 ] = dr -> getName().c_str();
		}
	}
	if( !selector -> setDescriptor( "aliquot_type" ) ) {
		Application -> MessageBox( L"No aliquot type descriptor values", NULL, MB_ICONWARNING );
		return false;
	}
	if( !selector -> setDescriptor( "analysis" ) ) {
		Application -> MessageBox( L"No analysis descriptor values", NULL, MB_ICONWARNING );
		return false;
	}
	sgSpecimens -> ColCount = column + 1;
	sgSpecimens -> Cells[ column ][ 0 ] = "test profile";
	sgSpecimens -> ColWidths[ column ] = 180;
	profile_column = column;
	sgProfiles -> Cells[ 0 ][ 0 ] = "samples";
	sgProfiles -> Cells[ 1 ][ 0 ] = "test profiles";

	samples.clear();
	if( barcodes != NULL ) {
		for( int n = 0; n < barcodes -> Count; ++ n ) {
			AnsiString barcode = barcodes -> Strings[ n ];
			samples.add( barcode.c_str() );
		}
	}
	return true;
}

//---------------------------------------------------------------------------
//	Add entries for the cryovials in the given box; return number added
//---------------------------------------------------------------------------

unsigned TfrmAddSpecimens::addFromBox( int boxID )
{
	LPDbCryovials cryovials;
	if( cryovials.readByBoxID( LIMSDatabase::getProjectDb(), boxID ) ) {
		for( Range< LPDbCryovial > ci = cryovials; ci.isValid(); ++ ci ) {
			samples.add( *ci );
		}
	}
	return cryovials.size();
}

//---------------------------------------------------------------------------
//	show result of merging profile with descriptors from the source tube
//---------------------------------------------------------------------------

void TfrmAddSpecimens::updateGrid()
{
	std::map< std::string, short > profiles;
	int row = 1;
	for( Range< SampleList::Entry > sr = samples.getEntries(); sr.isValid(); ++ sr, ++ row ) {
		sgSpecimens -> Cells[ barcode_column ][ row ] = sr -> getBarcode().c_str();
		TDate when = sr -> whenFirstSeen();
		if( when >= EPOCH_START && when < EPOCH_END ) {
			sgSpecimens -> Cells[ received_column ][ row ] = when;
		} else {
			sgSpecimens -> Cells[ received_column ][ row ] = " ";
		}
		for( int column = value_columns; column < profile_column; ++ column ) {
			AnsiString heading =  sgSpecimens -> Cells[ column ][ 0 ];
			const std::string & value = sr -> getFields().getDescriptorValue( heading.c_str() );
			sgSpecimens -> Cells[ column ][ row ] = value.c_str();
		}
		const std::string result = sr -> getProfile();
		sgSpecimens -> Cells[ profile_column ][ row ] = result.c_str();
		profiles[ result ] ++;
	}
	sgSpecimens -> RowCount = (row > 1 ? row : 2);
	row = 0;
	for( std::map< std::string, short >::const_iterator mi = profiles.begin(); mi != profiles.end(); ++ mi, ++ row ) {
		sgProfiles -> Cells[ 0 ][ row ] = mi -> second;
		sgProfiles -> Cells[ 1 ][ row ] = mi -> first.c_str();
	}
	sgProfiles -> ColWidths[ 1 ] = sgProfiles -> Width - 100;
	sgProfiles -> RowCount = (row > 0 ? row : 1);
}

//---------------------------------------------------------------------------

void __fastcall TfrmAddSpecimens::Timer1Timer(TObject *Sender)
{
	if( selector -> isUpdated() ) {
		Screen->Cursor = crSQLWait;
		samples.merge( LIMSDatabase::getProjectDb(), selector -> getSelected() );
		updateGrid();
		Screen->Cursor = crDefault;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmAddSpecimens::FormHide(TObject *Sender)
{
	Timer1 -> Enabled = false;
}

//---------------------------------------------------------------------------

