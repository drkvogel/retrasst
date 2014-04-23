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

void __fastcall TfrmNewJob::SaveClick(TObject *Sender)
{
	if( Util::validateText( TxtName, LblName )
	 && Util::validateText( TxtFull, LblFull )
	 && Util::validateText( TxtReason, LblReason ) ) {
		job.setName( AnsiString( TxtName->Text ).c_str() );
		job.setDescription( AnsiString( TxtFull->Text ).c_str() );
		job.setReason( AnsiString( TxtReason->Text ).c_str() );
		if( CbExercise->ItemIndex >= 0 ) {
			AnsiString why = CbExercise->Items->Strings[ CbExercise->ItemIndex ];
			job.setExercise( why.c_str() );
		}
		ModalResult = mrOk;
	}
}

//---------------------------------------------------------------------------
// create job record; include project and aliquot type(s) if possible
//---------------------------------------------------------------------------

void TfrmNewJob::init( LCDbCryoJob::JobKind kind )
{
	job.setJobType( kind );
	job.createName( LIMSDatabase::getCentralDb() );
	TxtName->Text = job.getName().c_str();
	TxtFull->Clear();
	ActiveControl = TxtFull;
	TxtReason->Clear();
	CbExercise->Clear();
	for( const LCDbObject & obj : LCDbObjects::records() ) {
		if( obj.isActive() && obj.getObjectType() == LCDbObject::STORAGE_EXERCISE ) {
			CbExercise->Items->Add( obj.getName().c_str() );
		}
	}
	CbExercise->Text = "(none)";
}

//---------------------------------------------------------------------------

bool TfrmNewJob::createJob( const std::vector<Box*> & boxes )
{
	int projID = 0, al1 = 0, al2 = 0;
	std::set<int> projects, boxTypes, aliquots;
	for( const Box * box : boxes ) {
		projects.insert( box->getProjectCID() );
		boxTypes.insert( box->getBoxTypeCID() );
	}
	if( projects.size() == 1 ) {
		projID = *(projects.begin());
		for( const LPDbBoxType & bt : LPDbBoxTypes::records() ) {
			if( boxTypes.count( bt.getID() ) != 0 ) {
				for( int at : bt.getAliquots() ) {
					aliquots.insert( at );
				}
			}
		}
	}
	std::set<int>::const_iterator ai = aliquots.begin();
	if( ai != aliquots.end() ) {
		al1 = *ai;
		if( ++ ai != aliquots.end() ) {
			al2 = *ai;
		}
	}
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


