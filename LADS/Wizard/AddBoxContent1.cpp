//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "AddBoxContent1.h"
#include "LCDbProject.h"
#include "Login1.h"
#include "LCDbObject.h"
#include "LDbBoxSize.h"
#include "LQuery.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma resource "*.dfm"

TAddBoxContent *AddBoxContent;

//---------------------------------------------------------------------------

__fastcall TAddBoxContent::TAddBoxContent(TComponent* Owner) : TForm(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TAddBoxContent::FormActivate(TObject *Sender)
{
	Caption = Login -> setProjectCaption( "Box Content" );
	FillBoxSizeComboBox();
	FillAliquotComboBoxes();

	String proj = LCDbProjects::records().get( LCDbProjects::getCurrentID() ).getName();
	if( LPDbBoxTypes::records().findByID( boxTypeID ) == NULL )
	{
		edtBoxName->Text = "";
		edtDescription->Text = "";
		edtBoxLabel->Text = proj + "-name-number";
		cbxStatus->Checked = true;
		cbxBoxSize->ItemIndex = 0;
		cbxAliquot1->ItemIndex = 0;
		cbxAliquot2->ItemIndex = 0;
		cbxAliquot3->ItemIndex = 0;
		edtBoxSet->Text = "";
		edtBoxOrder->Text = "";
	}
	else
	{   const LPDbBoxType & theBoxContent = LPDbBoxTypes::records().get( boxTypeID );
		edtBoxName->Text = theBoxContent.getName();
		edtBoxLabel->Text =	proj + "-" + theBoxContent.getName() + "-number";
		edtDescription->Text = theBoxContent.getDescription();
		cbxStatus->Checked = theBoxContent.isActive();
		edtBoxSet->Text = theBoxContent.getLink();
		edtBoxOrder->Text = theBoxContent.getPosition();

		LPDbBoxType::Aliquots al = theBoxContent.getAliquots();
		SetAliquotItemIndex( cbxAliquot1, al.isValid() ? *al : 0 );
		SetAliquotItemIndex( cbxAliquot2, (++ al).isValid() ? *al : 0 );
		SetAliquotItemIndex( cbxAliquot3, (++ al).isValid() ? *al : 0 );
		const LCDbBoxSize * bs = LCDbBoxSizes::records().findByID( theBoxContent.getSizeID() );
		if( bs == NULL )
			cbxBoxSize -> Text = "";
		else
			cbxBoxSize -> Text = bs -> getName();
	}
}

//---------------------------------------------------------------------------

void TAddBoxContent::SetAliquotItemIndex( TComboBox* cbx, int aliquotTypeID )
{
	if( aliquotTypeID == 0 )
		cbx -> ItemIndex = -1;
	else
		cbx -> Text = LCDbObjects::records().get( aliquotTypeID ).getName();
}

//---------------------------------------------------------------------------

void TAddBoxContent::FillBoxSizeComboBox()
{
	cbxBoxSize->Clear();
	for( LCDbBoxSizes::Range bsr = LCDbBoxSizes::records(); bsr.isValid(); ++ bsr )
	{
		String capacity = (bsr -> getHole() == 0 ? bsr -> getSize() : bsr -> getSize() - 1);
		cbxBoxSize->Items->Add( bsr -> getName() + ": " + capacity );
	}
}

//---------------------------------------------------------------------------

void TAddBoxContent::FillAliquotComboBoxes()
{
	// get aliquot names
	cbxAliquot1->Items->Text = "unknown";
	cbxAliquot2->Items->Text = "unknown";
	cbxAliquot3->Items->Text = "unknown";

	for( LCDbObjects::Range aliq = LCDbObjects::records(); aliq.isValid(); ++ aliq )
	{
		if( aliq -> getObjectType() == LCDbObject::ALIQUOT_TYPE )
		{
			cbxAliquot1->Items->Add( aliq -> getName() );
			cbxAliquot2->Items->Add( aliq -> getName() );
			cbxAliquot3->Items->Add( aliq -> getName() );
		}
	}
}

//---------------------------------------------------------------------------

void __fastcall TAddBoxContent::SaveClick(TObject *Sender)
{
	String message;
	if( LCDbObjects::records().find( cbxAliquot1 -> Text, LCDbObject::ALIQUOT_TYPE ) == NULL )
		message = "Aliquot 1 must be set";

	if( edtBoxName->Text.IsEmpty() || edtDescription->Text.IsEmpty() )
		message = "Prefix and description must be filled in";
	else
	{	int len = edtBoxLabel->Text.Length();
		if( len >= 30 )
		message = "Box name has " + String( len ) + " characters.\n"
				  "It must have fewer than 30";
	}

	String boxType = cbxBoxSize -> Text;
	int end = boxType.Pos( ": " );
	if( end > 1 )
		boxType.SetLength( end - 1 );

	const LCDbBoxSize * size = LCDbBoxSizes::records().find( boxType );
	if( size == NULL )
		message = "Box size not recognised";

	if( message.IsEmpty() )
	{
		LPDbBoxType theBoxContent( boxTypeID );
		theBoxContent.setName( edtBoxName->Text );
		theBoxContent.setDescription( edtDescription->Text );
		theBoxContent.setPosition( edtBoxOrder->Text.ToIntDef(0) );
		theBoxContent.setLink( edtBoxSet->Text.ToIntDef(0) );
		theBoxContent.setActive( cbxStatus -> Checked );
		theBoxContent.setSizeID( size -> getID() );

		std::vector< int > aliquots;
		const LCDbObject * alt = LCDbObjects::records().find( cbxAliquot1->Text, LCDbObject::ALIQUOT_TYPE );
		if( alt != NULL )
			aliquots.push_back( alt -> getID() );

		alt = LCDbObjects::records().find( cbxAliquot2->Text, LCDbObject::ALIQUOT_TYPE );
		if( alt != NULL )
			aliquots.push_back( alt -> getID() );

		alt = LCDbObjects::records().find( cbxAliquot3->Text, LCDbObject::ALIQUOT_TYPE );
		if( alt != NULL )
			aliquots.push_back( alt -> getID() );
		theBoxContent.setAliquots( aliquots );
		
		theBoxContent.saveRecord( QueryProject );
		Close();
	}
	else Application -> MessageBox( message.c_str(), NULL, MB_OK );
}

//---------------------------------------------------------------------------

void __fastcall TAddBoxContent::btnCloseClick(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TAddBoxContent::edtBoxNameExit(TObject *Sender)
{
	const LCDbProject & proj = LCDbProjects::records().get( LCDbProjects::getCurrentID() );
	edtBoxLabel->Text = proj.getName() + "-" + edtBoxName->Text + "-number";
}

//---------------------------------------------------------------------------


