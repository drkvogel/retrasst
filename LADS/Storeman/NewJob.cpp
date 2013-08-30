//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewJob.h"
#include "StoreUtil.h"
#include "LIMSDatabase.h"
#include "LDbBoxType.h"

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
	if( Util::validateText( TxtName, LblName )
	 && Util::validateText( TxtFull, LblFull )
	 && Util::validateText( TxtReason, LblWhy ) )
	{
		AnsiString name = TxtName->Text.Trim();
		AnsiString desc = TxtFull->Text.Trim();
		AnsiString why = TxtReason->Text.Trim();
		job.setName( name.c_str() );
		job.setDescription( desc.c_str() );
		job.setReason( why.c_str() );
		ModalResult = mrOk;
	}
}

//---------------------------------------------------------------------------

bool TfrmNewJob::createJob( LCDbCryoJob::JobKind kind, const std::vector<Box*> & boxes )
{
	int projID = 0, aliquot = 0;
	// record project and aliquot type if all boxes are similar
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
	if( aliquots.size() == 1 ) {
		aliquot = *(aliquots.begin());
	}
	job.setProjectID( projID );
	job.setPrimaryAliquot( aliquot );
	job.setJobType( kind );
	return job.saveRecord( cQuery );
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewJob::FormShow(TObject *Sender)
{
//	job.setID( 0 );
	job.createName( cQuery, "MOVE" );
	TxtName->Text = job.getName().c_str();
	TxtFull->Text = "";
	ActiveControl = TxtFull;
}

//---------------------------------------------------------------------------

