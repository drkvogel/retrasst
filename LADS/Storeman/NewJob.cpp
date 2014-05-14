//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewJob.h"
#include "StoreUtil.h"
#include "LIMSDatabase.h"
#include "LDbBoxType.h"
#include "LCDbObject.h"
#include "NewExercise.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmNewJob *frmNewJob;

//---------------------------------------------------------------------------

__fastcall TfrmNewJob::TfrmNewJob(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void TfrmNewJob::init( LCDbCryoJob::JobKind kind )
{
	job.setJobType( kind );
	job.createName( LIMSDatabase::getCentralDb() );
	TxtName->Text = job.getName().c_str();
	txtBoxSet->Text = job.getBoxSet();
	TxtFull->Clear();
	ActiveControl = TxtFull;

	CbExercise->Clear();
	for( const LCDbObject & obj : LCDbObjects::records() ) {
		if( obj.isActive() && obj.getObjectType() == LCDbObject::STORAGE_EXERCISE ) {
			CbExercise->Items->Add( obj.getName().c_str() );
		}
	}
	CbExercise->Text = "(none)";

	// FIXME - use canned text
	std::vector< std::string > reasons;
	reasons.push_back("patient has withdrawn consent");
	reasons.push_back("sample has evaporated");
	reasons.push_back("order 66");
	cmbReason->Clear();
	for( const std::string & reason : reasons ) {
		cmbReason->Items->Add(reason.c_str());
	}
	cmbReason->Text = "(none)";
	cmbReason->Enabled = !reasons.empty();
}

//---------------------------------------------------------------------------

bool TfrmNewJob::createJob( const std::vector<Box*> & boxes )
{
	std::set<int> projects, boxTypes, aliquots;
	for( const Box * box : boxes ) {
		projects.insert( box->getProjectCID() );
		boxTypes.insert( box->getBoxTypeCID() );
	}
	for( const LPDbBoxType & bt : LPDbBoxTypes::records() ) {
		if( boxTypes.count( bt.getID() ) != 0 ) {
			for( int at : bt.getAliquots() ) {
				aliquots.insert( at );
			}
		}
	}
	int projID = (projects.size() == 1 ? *(projects.begin()) : 0);
	std::set<int>::const_iterator ai = aliquots.begin();
	int al1 = (ai == aliquots.end() ? 0 : *ai ++ );
	int al2 = (ai == aliquots.end() ? 0 : *ai ++ );
	return createJob( projID, al1, al2 );
}

//---------------------------------------------------------------------------

bool TfrmNewJob::createJob( int projID, int al1, int al2 )
{
	job.setProjectID( projID );
	job.setPrimaryAliquot( al1 );
	job.setSecondaryAliquot( al2 );
	return job.saveRecord( LIMSDatabase::getCentralDb() );
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewJob::BtnNewExClick(TObject *Sender)
{
	if( frmNewExercise -> ShowModal() == mrOk ) {
		const LCDbObject * ex = frmNewExercise -> createRecord();
		if( ex != NULL ) {
			int pos = CbExercise->Items->Count;
			CbExercise->Items->Add( ex->getName().c_str() );
			CbExercise->ItemIndex = pos;
		}
	}
}

//---------------------------------------------------------------------------
// create job record; include project and aliquot type(s) if possible
//---------------------------------------------------------------------------

void __fastcall TfrmNewJob::BitBtn1Click(TObject *Sender)
{
	if( Util::validateText( TxtName, LblName ) && Util::validateText( TxtFull, LblFull )
	 && Util::validateInteger( txtBoxSet, 1, 999 ) ) {
		job.setName( AnsiString( TxtName->Text ).c_str() );
		job.setBoxSet( txtBoxSet->Text.ToIntDef( 0 ) );
		job.setDescription( AnsiString( TxtFull->Text ).c_str() );
		if( CbExercise->ItemIndex >= 0 ) {
			AnsiString why = CbExercise->Items->Strings[ CbExercise->ItemIndex ];
			job.setExercise( why.c_str() );
		}
		if( cmbReason->ItemIndex >= 0 ) {
			AnsiString why = cmbReason->Items->Strings[ cmbReason->ItemIndex ];
			job.setReason( why.c_str() );
		}
		ModalResult = mrOk;
	} else {
		ModalResult = mrNone;
	}
}

//---------------------------------------------------------------------------

