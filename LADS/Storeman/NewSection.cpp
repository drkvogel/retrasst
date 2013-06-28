//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "NewSection.h"
#include "StoreUtil.h"

#pragma package(smart_init)
#pragma resource "*.dfm"

TfrmNewSection *frmNewSection;

//---------------------------------------------------------------------------
__fastcall TfrmNewSection::TfrmNewSection(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TfrmNewSection::init( IPart* lay, Section* p_section )
{
	this->ActiveControl = TxtPrefix;
	newSectionList = lay->getList();
	if( p_section == NULL )
	{
		section = new Section();
		TxtPrefix->Text = "";
		UpDownFill->Position = getDefaultFillOrder();
		UpDownFirst->Position = 1;
		UpDownLast->Position = 5;
		UpDownCap->Position = 10;
	}
	else
	{
		section = p_section;
		TxtPrefix->Text = section->getName().c_str();
		UpDownFill->Position = section->getPosition();
		UpDownFirst->Position = section->getFirstRack();
		UpDownLast->Position = section->getLastRack();
		UpDownCap->Position = section->getRackCapacity();
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmNewSection::OKClick(TObject *Sender)
{
	//Validate and return
	if( !Util::validateText( TxtPrefix, LblPrefix ) )
		return;

	AnsiString name = TxtPrefix->Text.Trim();
	if( isPrefixDuplicate( name.c_str() ) )
		return;

	int fill, first, last, cap;
	fill = Util::validateInteger( TxtFill, 1, 20 );
	if( fill == 0 || isFillOrderDuplicate( fill ) )
		return;

	first = Util::validateInteger( TxtFirst, 1, 99 );
	last = Util::validateInteger( TxtLast, first, 99 );
	if( first == 0 || last == 0 )
		return;

	cap = Util::validateInteger( TxtCap, 1, 99 );
	if( first == 0 || last == 0 )
		return;

	section->setName( name.c_str() );
	section->setPosition( fill );
	section->setFirstRack( first );
	section->setLastRack( last );
	section->setRackCapacity( cap );

	this->ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmNewSection::CancelClick(TObject *Sender)
{
	//return
	this->ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

int TfrmNewSection::getDefaultFillOrder()
{
	Section* childdata;
	int fillorder = 0;

	for( int i = 0; i <(int) newSectionList.size(); i++ )
	{
		childdata = (Section*) newSectionList[i];
		if( childdata->getPosition() > fillorder )
		{
			fillorder = childdata->getPosition();
		}
	}

	return fillorder + 1;
}

bool TfrmNewSection::isFillOrderDuplicate( int order )
{
	Section* childdata;

	for( int i = 0; i <(int) newSectionList.size(); i++ )
	{
		childdata = (Section*) newSectionList[i];
		if( childdata != section && childdata->getPosition() == order )
		{
			String buf;
			buf.sprintf( L"Section <%s> has the same fill order <%d> as this one",
					childdata->getName().c_str(), childdata->getPosition() );
			Application->MessageBox( buf.c_str(), L"Validation Error", MB_OK);
			TxtFill->SetFocus();
			return true;
		}
	}

	return false;
}

bool TfrmNewSection::isPrefixDuplicate( std::string prefix )
{
	Section* childdata;

	for( int i = 0; i <(int) newSectionList.size(); i++ )
	{
		childdata = (Section*) newSectionList[i];
		if( childdata != section && childdata->getName() == prefix )
		{
			String buf = "Section prefix must be unique";
			Application->MessageBox( buf.c_str(), L"Validation Error", MB_OK);
			TxtPrefix->SetFocus();
			return true;
		}
	}

	return false;
}


