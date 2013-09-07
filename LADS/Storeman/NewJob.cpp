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

__fastcall TfrmNewJob::TfrmNewJob(TComponent* Owner)
 : TForm(Owner), cQuery( LIMSDatabase::getCentralDb() )
{}

//---------------------------------------------------------------------------

void __fastcall TfrmNewJob::SaveClick(TObject *Sender)
{
	if( Util::validateText( TxtName, LblName ) && Util::validateText( TxtFull, LblFull ) ) {
		AnsiString name = TxtName->Text.Trim(), desc = TxtFull->Text.Trim();
		job.setName( name.c_str() );
		job.setDescription( desc.c_str() );
		if( CbExercise->ItemIndex >= 0 ) {
			AnsiString why = CbExercise->Items->Strings[ CbExercise->ItemIndex ];
			job.setReason( why.c_str() );
		}
		ModalResult = mrOk;
	}
}

//---------------------------------------------------------------------------
// create job record; include project and aliquot type(s) if possible
//---------------------------------------------------------------------------

bool TfrmNewJob::createJob( LCDbCryoJob::JobKind kind, const std::vector<Box*> & boxes )
{
	int projID = 0, al1 = 0, al2 = 0;
	std::set<int> projects, boxTypes, aliquots;
	for( std::vector<Box*>::const_iterator bi = boxes.begin(); bi != boxes.end(); ++ bi ) {
		const Box & box = **bi;
		projects.insert( box.getProjectCID() );
		boxTypes.insert( box.getBoxTypeCID() );
	}
	if( projects.size() == 1 ) {
		projID = *(projects.begin());
		for( Range< LPDbBoxType > bt = LPDbBoxTypes::records( projID ); bt.isValid(); ++ bt ) {
			if( boxTypes.count( bt->getID() ) != 0 ) {
				for( Range< int > at = bt->getAliquots(); bt.isValid(); ++ bt ) {
					aliquots.insert( *at );
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
	job.setProjectID( projID );
	job.setPrimaryAliquot( al1 );
	job.setSecondaryAliquot( al2 );
	job.setJobType( kind );
	return job.saveRecord( cQuery );
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewJob::FormShow(TObject *Sender)
{
	job.createName( cQuery, "MOVE" );
	TxtName->Text = job.getName().c_str();
	TxtFull->Clear();
	ActiveControl = TxtFull;
	CbExercise->Clear();
	for( Range< LCDbObject > oi = LCDbObjects::records(); oi.isValid(); ++oi ) {
		if( oi->isActive() && oi->getObjectType() == LCDbObject::STORAGE_EXERCISE ) {
			CbExercise->Items->Add( oi->getName().c_str() );
		}
	}
	CbExercise->Text = "(none)";
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


