//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SampleEntry.h"
#include "RetrieveSamples.h"
#include "showsamples.h"
#include "StoreDAO.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmRetrieveMain *frmRetrieveMain;
//---------------------------------------------------------------------------
__fastcall TfrmRetrieveMain::TfrmRetrieveMain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TfrmRetrieveMain::init()
{
	this->ActiveControl = RadIDType;
	clearGrid();
/*	selectedProject = p_selectedProject;
	projs.loadAll();
	proj = projs.findProject( selectedProject );
	ats.loadAll();
	typelist = ats.getList();
	for( int i = 0; i < (int) typelist.size(); i++ )
	{
		CmbAliquot1->Items->Add( typelist[i]->getName().c_str() );
		CmbAliquot2->Items->Add( typelist[i]->getName().c_str() );
	}
*/
	grdSamples->Cells[0][0] = "Sample ID";
	grdSamples->Cells[1][0] = "Cryovial ID";
	grdSamples->Cells[2][0] = "Aliquot Choice 1";
	grdSamples->ColWidths[0] = (int)(grdSamples->ClientWidth * 0.33);
	grdSamples->ColWidths[1] = (int)(grdSamples->ClientWidth * 0.33);
	grdSamples->ColWidths[2] = (int)(grdSamples->ClientWidth * 0.33);
}

void TfrmRetrieveMain::clearGrid()
{
	for(int i = 1; i < grdSamples->RowCount; i++ )
		grdSamples->Rows[i]->Clear();
	grdSamples->RowCount = 2;

	rows.clear();
}

void __fastcall TfrmRetrieveMain::AddClick(TObject *Sender)
{
//make sure that two different aliquot types are selected in 2 combos
	//Add data to grid
	int row = grdSamples->RowCount - 1;
	if( row <= 0 )
	{
		Application->MessageBox(L"Invalid row", L"Info", MB_OK);
		return;
	}
	bool bIncrement = false;
	Sample* s = NULL;

	AnsiString sid = ""; //Sample barcode
	AnsiString cid = ""; //Cryovial barcode
	AnsiString bid = ""; //box barcode
	switch (RadIDType->ItemIndex)
	{
		case 0: //sample
			sid = TxtBarcode->Text.Trim();
			break;

		case 1: //cryovial
			cid = TxtBarcode->Text.Trim();
			break;

		case 2: //box (query to be written )
			bid = TxtBarcode->Text.Trim();
			break;
	}

	if( sid.Length() != 0 )
	{
		bIncrement = true;
	}

	int index = CmbAliquot1->ItemIndex;
	std::string adesc = "", aid = "";
	if( index >= 0 )
	{
		if( !bIncrement )
		{
			Application->MessageBox(L"Provide sample_id or cryovial_id", L"Info", MB_OK);
			return;
		}
		bIncrement = true;
//		adesc = typelist[index]->getName();
//		aid = AnsiString ( typelist[index]->getID() ).c_str();
	}

	if( bIncrement )
	{
		std::string qry = StoreDAO::records().prepareSampleQuery( sid.c_str(), cid.c_str(), aid.c_str());
//		proj->loadAllSamples( qry );
//		slist = proj->getSampleList();
		if( slist.size() == 0 )
		{
			Application->MessageBox(L"No samples found", L"Info", MB_OK);
			return;
		}
		else if( slist.size() > 1 )
		{
			frmAliquotTypes->init();
			if( frmAliquotTypes->ShowModal() == mrCancel )
				return;
			grdSamples->RowCount++;
			s = (Sample*) slist[frmAliquotTypes->selrow - 1];
/*			AliquotType* at;
			at = frmRetrieveMain->ats.find( s->getAliquot_type() );
			if( at != NULL )
			{
				aid = AnsiString( at->getID() ).c_str();
				adesc = at->getName();
			}
*/
			TxtBarcode->Text = "";
			grdSamples->Cells[2][row] = adesc.c_str();
			CmbAliquot1->ItemIndex = -1;

			if( RadIDType->ItemIndex == 0 )
			{
				grdSamples->Cells[0][row] = sid;
				grdSamples->Cells[1][row] = "";
				rows.push_back( GridEntry(s->getName(), "", aid, adesc, selectedProject) );
			}
			else if( RadIDType->ItemIndex == 1 )
			{
				grdSamples->Cells[0][row] = "";
				grdSamples->Cells[1][row] = cid;
				rows.push_back( GridEntry( "", cid.c_str(), aid, adesc, selectedProject) );
			}

			return;
		}

		TxtBarcode->Text = "";
//		grdSamples->Cells[2][row] = typelist[index]->getName().c_str();
		CmbAliquot1->ItemIndex = -1;
		s = (Sample*) slist[0];
		if( RadIDType->ItemIndex == 0 )
		{
			grdSamples->Cells[0][row] = sid;
			grdSamples->Cells[1][row] = "";
			rows.push_back( GridEntry(s->getName(), "", aid, adesc, selectedProject) );
		}
		else if( RadIDType->ItemIndex == 1 )
		{
			grdSamples->Cells[0][row] = "";
			grdSamples->Cells[1][row] = cid;
			rows.push_back( GridEntry("", cid.c_str(), aid, adesc, selectedProject) );
		}
		grdSamples->RowCount++;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmRetrieveMain::Retrieve(TObject *Sender)
{
	frmRetrieved->init( NULL );
	frmRetrieved->ShowModal();
}
//---------------------------------------------------------------------------

