//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "RetrieveSamples.h"
#include "SampleEntry.h"
#include "StoreDAO.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmRetrieved *frmRetrieved;
//---------------------------------------------------------------------------
__fastcall TfrmRetrieved::TfrmRetrieved(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

void TfrmRetrieved::init( const LCDbProject* p_proj )
{
	this->ActiveControl = btnOK;
	clearGrid();
//	proj = p_proj;

	grdSamples->Cells[0][0] = "Sample ID";
	grdSamples->Cells[1][0] = "Cryovial ID";
	grdSamples->Cells[2][0] = "Aliquot Type";
	grdSamples->Cells[3][0] = "Box Name";
	grdSamples->Cells[4][0] = "Rack Name";
	grdSamples->Cells[5][0] = "Tank Name";

	for( int i = 0; i < grdSamples->ColCount; i++ )
	{
		grdSamples->ColWidths[i] = (int)(grdSamples->ClientWidth * 0.16);
	}
	Retrieve();
}

void TfrmRetrieved::clearGrid()
{
	for(int i = 1; i < grdSamples->RowCount; i++ )
		grdSamples->Rows[i]->Clear();
	grdSamples->RowCount = 2;
}

void TfrmRetrieved::Retrieve()
{
	//Add code to find samples
	std::string sid, cid, aid, adesc;
	IPart* part, *parent;
	std::string qry;
	bool found;

	for( int i = 0; i < (int)frmRetrieveMain->rows.size(); i++ )
	{
		sid = frmRetrieveMain->rows[i].sid;
		cid = frmRetrieveMain->rows[i].cid;
		aid = frmRetrieveMain->rows[i].aid;
		adesc = frmRetrieveMain->rows[i].aliquot;
		grdSamples->Cells[0][i+1] = sid.c_str();
		grdSamples->Cells[1][i+1] = cid.c_str();
		grdSamples->Cells[2][i+1] = adesc.c_str();
//		qry = StoreDAO::records().prepareSampleQuery(sid, cid, aid);
		if( qry.length() == 0 )
		{
			continue;
		}

		found = false;
//		part = proj->loadSample( qry );
		if( part != NULL )
		{
			part->loadParent(); //box
			parent = part->getParent();
			int col = 3;
			while( parent != NULL )
			{
				grdSamples->Cells[col++][i+1] = parent->getName().c_str();
				parent->loadParent();
				parent = parent->getParent();
			}
			if( col > 5 )
				found = true;
		}

		if( !found )
		{
			grdSamples->Cells[3][i+1] = "Sample Not Found";
		}
	}
	grdSamples->RowCount = grdSamples->FixedRows + frmRetrieveMain->rows.size();

}
