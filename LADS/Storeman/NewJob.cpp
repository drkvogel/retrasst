//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewJob.h"
#include "StoreUtil.h"
#include "LIMSDatabase.h"
#include "LDbBoxType.h"
#include "LCDbObject.h"
#include "LCDbCanned.h"
#include "NewReason.h"

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
	int prefix = job.getBoxSet();
	if( prefix > 0 ) {
		txtBoxSet->Text = prefix;
	} else {
		txtBoxSet->Clear();
	}
	TxtFull->Clear();
	ActiveControl = TxtFull;

	std::string groupName;
	switch (kind) {
		case LCDbCryoJob::BOX_MOVE:
			groupName = "MovementReason";
			break;
		case LCDbCryoJob::BOX_DISCARD:
		case LCDbCryoJob::SAMPLE_DISCARD:
			groupName = "DisposalReason";
	}
	reasonGroup = 0;
	if( !groupName.empty() ) {
		const LCDbObject * group = LCDbObjects::records().findByName( groupName );
		if( group != NULL && group->isActive() && group->getObjectType() == LCDbObject::CANNED_TEXT ) {
			reasonGroup = group->getID();
        }
	}
	cmbReason->Clear();
	if( reasonGroup == 0 ) {
		btnAddReason->Enabled = false;
	} else {
		btnAddReason->Enabled = true;
		for( const LCDbCanned & text : LCDbCanneds::records() ) {
			if( text.isActive() && text.getGroupID() == reasonGroup ) {
				cmbReason->Items->Add( text.getDescription().c_str());
			}
		}
	}
	cmbReason->Enabled = (cmbReason->Items->Count != 0);
}

//---------------------------------------------------------------------------

void TfrmNewJob::setExercise( int exID ) {
	const LCDbObject * ex = LCDbObjects::records().findByID( exID );
	if( ex == NULL ) {
		lblExercise->Caption = "n/a";
		job.setExercise( 0 );
	} else {
		lblExercise->Caption = ex->getName().c_str();
		job.setExercise( exID );
	}
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
	job.setProjectID(projects.size() == 1 ? *projects.begin() : 0);
	std::set<int>::const_iterator ai = aliquots.begin();
	job.setPrimaryAliquot( ai == aliquots.end() ? 0 : *ai ++ );
	job.setSecondaryAliquot( ai == aliquots.end() ? 0 : *ai ++ );
	return job.saveRecord( LIMSDatabase::getCentralDb() );
}

//---------------------------------------------------------------------------
// create job record; include project and aliquot type(s) if possible
//---------------------------------------------------------------------------

void __fastcall TfrmNewJob::BitBtn1Click(TObject *Sender)
{
	if( Util::validateText( TxtName, LblName ) && Util::validateText( TxtFull, LblFull )
	 && (txtBoxSet->Text.IsEmpty() || Util::validateInteger( txtBoxSet, 1, 999 )) ) {
		job.setName( AnsiString( TxtName->Text ).c_str() );
		job.setBoxSet( txtBoxSet->Text.ToIntDef( 0 ) );
		job.setDescription( AnsiString( TxtFull->Text ).c_str() );
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

void __fastcall TfrmNewJob::btnAddReasonClick(TObject *Sender)
{
	if( frmNewReason->init( reasonGroup ) && frmNewReason->ShowModal() == mrOk ) {
		const LCDbCanned * text = frmNewReason->createRecord();
		if( text != NULL ) {
			int index = cmbReason->Items->Count;
			cmbReason->Items->Add( text->getDescription().c_str() );
			cmbReason->ItemIndex = index;
			cmbReason->Enabled = true;
		}
	}
}

//---------------------------------------------------------------------------

