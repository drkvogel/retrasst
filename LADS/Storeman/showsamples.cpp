//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ShowSamples.h"
#include "SampleEntry.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmAliquotTypes *frmAliquotTypes;
//---------------------------------------------------------------------------
__fastcall TfrmAliquotTypes::TfrmAliquotTypes(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void TfrmAliquotTypes::init()
{
	this->ActiveControl = grdSamples;
	clearGrid();

	grdSamples->Cells[0][0] = "Sample ID";
	grdSamples->Cells[1][0] = "Cryovial ID";
	grdSamples->Cells[2][0] = "Position";
	grdSamples->Cells[3][0] = "Aliquot Type";

	for( int i = 0; i < grdSamples->ColCount; i++ )
	{
		grdSamples->ColWidths[i] = (int)(grdSamples->ClientWidth * 0.25);
	}

	Sample* s;
//	AliquotType* at;
	for( int i = 0; i < (int)frmRetrieveMain->slist.size(); i++ )
	{
		s = (Sample*)frmRetrieveMain->slist[i];
		grdSamples->Cells[0][i+1] = AnsiString( s->getID() );
		grdSamples->Cells[1][i+1] = s->getName().c_str();
		grdSamples->Cells[2][i+1] = AnsiString( s->getPosition() );
//		at = frmRetrieveMain->ats.find( s->getAliquot_type() );
//		if( at != NULL )
//			grdSamples->Cells[3][i+1] = at->getName().c_str();
	}

	grdSamples->RowCount = grdSamples->FixedRows + frmRetrieveMain->slist.size();
}

void TfrmAliquotTypes::clearGrid()
{
	for(int i = 1; i < grdSamples->RowCount; i++ )
		grdSamples->Rows[i]->Clear();
	grdSamples->RowCount = 2;
}

void __fastcall TfrmAliquotTypes::OKClicked(TObject *Sender)
{
	//return to caller with the index of row selected
	if( grdSamples->Row <= 0 )
	{
		Application->MessageBox(L"Select a row", L"Info", MB_OK);
		return;
	}

	selrow = grdSamples->Row;
	this->ModalResult = mrOk;
}
//---------------------------------------------------------------------------
