//---------------------------------------------------------------------------

#include <vcl.h>

#include "LCDbProject.h"
#include "LDbBoxStore.h"
#include "AnalyseSamples.h"
#include "SelectSamples.h"
#include "AddSpecimens.h"
#include "SMLogin.h"
#include "StoreUtil.h"
#include "MoveJobs.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "dvSelector"
#pragma resource "*.dfm"

TfrmAnalyseSpecimens *frmAnalyseSpecimens;

//---------------------------------------------------------------------------

__fastcall TfrmAnalyseSpecimens::TfrmAnalyseSpecimens( TComponent *Owner ) : TForm( Owner ) {
}

//---------------------------------------------------------------------------

void TfrmAnalyseSpecimens::init( ) {
	cbProject->Clear( );
	int selected = -1;
	for( Range< LCDbProject >pr = LCDbProjects::records( ); pr.isValid( ); ++pr ) {
		if( pr->isInCurrentSystem( ) && pr->isActive( ) && !pr->isCentral( ) ) {
			if( pr->getID( ) == LCDbProjects::getCurrentID( ) ) {
				selected = cbProject->Items->Count;
			}
			cbProject->Items->Add( pr->getName( ).c_str( ) );
		}
	}
	cbProject->ItemIndex = selected;
}

//---------------------------------------------------------------------------

void __fastcall TfrmAnalyseSpecimens::cbProjectChange( TObject *Sender ) {
	LCDbProjects &projList = LCDbProjects::records( );
	AnsiString proj = cbProject->Text;
	const LCDbProject *selected = projList.findByName( proj.c_str() );
	if( selected != NULL ) {
		projList.setCurrent( *selected );
	}
}

//---------------------------------------------------------------------------
//	Process the cryovials from the selected boxes or input file
//---------------------------------------------------------------------------

void __fastcall TfrmAnalyseSpecimens::BitBtn1Click( TObject *Sender ) {
	bool ready = false;
	switch( rgSource->ItemIndex ) {
		case 0:
			ready = addFromBoxes();
			break;
		case 1:
			ready = addFromJob();
			break;
		case 2:
			ready = addFromFile();
			break;
		default:
			Application->MessageBox( L"Please select source", NULL, MB_ICONWARNING );
	}
	if( ready ) {
		ModalResult = frmAddSpecimens->ShowModal( );
	}
}

//---------------------------------------------------------------------------
//	Copy the barcodes from the cryovials in the currently selected boxes
//---------------------------------------------------------------------------

bool TfrmAnalyseSpecimens::addFromBoxes( ) {
	bool ready = false;
	if( frmSelectBoxes->init( ) && frmAddSpecimens->init( NULL )
	 && frmSelectBoxes->ShowModal( ) == mrOk ) {
		const LPDbBoxNames & boxes = frmSelectBoxes->getSelected( );
		for( Range< LPDbBoxName > li = boxes; li.isValid( ); ++li ) {
			if( frmAddSpecimens->addFromBox( li->getID( ) ) > 0 ) {
				ready = true;
			}
		}
	}
	return ready;
}

//---------------------------------------------------------------------------
//	Copy the barcodes from a text file - assume it's a simple list
//---------------------------------------------------------------------------

bool TfrmAnalyseSpecimens::addFromFile( ) {
	bool ready = false;
	if( openDialogue->Execute( ) ) {
		std::auto_ptr< TStrings >barcodes( new TStringList );
		String fileName = openDialogue->FileName;
		barcodes->LoadFromFile( fileName );
		if( barcodes->Count == 0 ) {
			String message = "Cannot read barcodes from " + fileName;
			Application->MessageBox( message.c_str(), NULL, MB_ICONWARNING );
		} else {
			ready = frmAddSpecimens->init( barcodes.get( ) );
		}
	}
	return ready;
}

//---------------------------------------------------------------------------
//	Copy barcodes from cryovials in boxes of the given retrieval job
//---------------------------------------------------------------------------

bool TfrmAnalyseSpecimens::addFromJob( ) {
	bool ready = false;
	frmSelectJob->initRetrieval( LCDbProjects::getCurrentID() );
	if( frmAddSpecimens->init( NULL ) && frmSelectJob->ShowModal( ) == mrOk ) {
		const LCDbCryoJob * job = frmSelectJob->getSelected( );
		if( job->getJobType() == LCDbCryoJob::BOX_RETRIEVAL ) {
			LCDbBoxStores locations;
			if( locations.readJob( LIMSDatabase::getProjectDb(), job->getID( ) ) ) {
				for( Range< LCDbBoxStore > li = locations; li.isValid( ); ++li ) {
					if( frmAddSpecimens->addFromBox( li->getBoxID( ) ) > 0 ) {
						ready = true;
					}
				}
			}
		} else {
			//// FIXME -- read from cryovial_store instead
        }
	}
	return ready;
}

//---------------------------------------------------------------------------
