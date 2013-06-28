//---------------------------------------------------------------------------

#include <vcl.h>

#include "EditCentralDb1.h"
#include "Machine1.h"
#include "CentralTest1.h"
#include "AddProject1.h"
#include "Operator1.h"
#include "ObjectName1.h"
#include "Project1.h"
#include "QCMaterial1.h"
#include "Login1.h"
#include "BoxSize1.h"
#include "WizardLogin1.h"
#include "LCDbObject.h"
#include "RackSize1.h"

#pragma hdrstop

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TEditCentralDb *EditCentralDb;

//---------------------------------------------------------------------------

__fastcall TEditCentralDb::TEditCentralDb(TComponent* Owner)
	: TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TEditCentralDb::FormActivate(TObject *Sender)
{
	Caption = WizardLogin -> setCaption( "Central Database" );
	lbObjects -> Clear();
	for( int obj = LCDbObject::UNKNOWN; (++ obj) < LCDbObject::NUM_TYPES; )
		lbObjects->Items->Add( LCDbObject::findDescription( LCDbObject::Category( obj ) ) );
}

//---------------------------------------------------------------------------

void __fastcall TEditCentralDb::MachinesClick(TObject *Sender)
{
	Machine->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TEditCentralDb::TestsClick(TObject *Sender)
{
	CentralTest->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TEditCentralDb::ProjectInfoClick(TObject *Sender)
{
	Project->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TEditCentralDb::OperatorsClick(TObject *Sender)
{
	Operator->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TEditCentralDb::btnCloseClick(TObject *Sender)
{
    Close();
}

//---------------------------------------------------------------------------

void __fastcall TEditCentralDb::btnQCClick(TObject *Sender)
{
	QCMaterial->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TEditCentralDb::btnBoxSizeClick(TObject *Sender)
{
	BoxSize->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TEditCentralDb::btnRackSizeClick(TObject *Sender)
{
	RackSize->ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TEditCentralDb::lbObjectsClick(TObject *Sender)
{
	ObjectName->setObjectType( lbObjects -> ItemIndex + 1 );
	ObjectName->ShowModal();
}

//---------------------------------------------------------------------------

