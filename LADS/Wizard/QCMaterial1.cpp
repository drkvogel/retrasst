//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "QCMaterial1.h"
#include "QCMaterialClass.h"
#include "QCLevelClass.h"
#include "AddQCMaterial1.h"
#include "AddQCLevel1.h"
#include "CentralTest1.h"
#include "WizardLogin1.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"

TQCMaterial *QCMaterial;

const int TQCMaterial::QCM_MATERIAL = 0;
const int TQCMaterial::QCM_LEVEL    = 1;
const int TQCMaterial::QCM_MATERIAL_DESCRIP = 2;
const int TQCMaterial::QCM_LEVEL_DESCRIP    = 3;
const int TQCMaterial::QCM_MATERIAL_INDEX   = 4;
const int TQCMaterial::QCM_LEVEL_INDEX      = 5;
const int TQCMaterial::QCM_NUMCOLS          = 6;
//---------------------------------------------------------------------------
__fastcall TQCMaterial::TQCMaterial(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TQCMaterial::FormActivate(TObject *Sender)
{
	Caption = WizardLogin -> setCaption( "QC Material" );
	
    QCMaterialGrid->ColCount = QCM_NUMCOLS;
    QCMaterialGrid->Cells[QCM_MATERIAL][0] = "Material";
	QCMaterialGrid->ColWidths[QCM_MATERIAL] = 55;
	QCMaterialGrid->Cells[QCM_LEVEL][0] = " Level";
	QCMaterialGrid->ColWidths[QCM_LEVEL] = 55;
    QCMaterialGrid->Cells[QCM_MATERIAL_DESCRIP][0] = " Material description";
	QCMaterialGrid->ColWidths[QCM_MATERIAL_DESCRIP] = 180;
	QCMaterialGrid->Cells[QCM_LEVEL_DESCRIP][0] = " Level description";
	QCMaterialGrid->ColWidths[QCM_LEVEL_DESCRIP] = 180;
	QCMaterialGrid->Cells[QCM_MATERIAL_INDEX][0] = "Material index";
    QCMaterialGrid->ColWidths[QCM_MATERIAL_INDEX] = 0;
    QCMaterialGrid->Cells[QCM_LEVEL_INDEX][0] = "Level index";
    QCMaterialGrid->ColWidths[QCM_LEVEL_INDEX] = 0;

    CopyFromQCMaterialTableToGrid( QueryCentral );
}
//---------------------------------------------------------------------------
void TQCMaterial::CopyFromQCMaterialTableToGrid( TQuery* query )
{
    CopyFromQCMaterialTableToClass( query );
    CopyFromQCLevelTableToClass( query );
    CopyFromQCMaterialClassToGrid();
}
//---------------------------------------------------------------------------
void TQCMaterial::CopyFromQCMaterialTableToClass( TQuery* query )
{
    // delete all QC Materials, create anew from database

    DeleteQCMaterialClass();

    query -> SQL -> Clear();
    query -> SQL -> Add(
        "select * from qc_material "
        "order by material");
    query -> Open();
//    query -> First();
    numQCMaterials = query->RecordCount;
    theQCMaterials = new QCMaterialClass*[numQCMaterials]; // allow for adding one
    for (int record=0; record < query->RecordCount; record++)
    {
        theQCMaterials[record] = new QCMaterialClass;
        theQCMaterials[record]->LoadFromDatabase(query,record);
    }
}
//---------------------------------------------------------------------------

void TQCMaterial::DeleteQCMaterialClass()
{
    for (int index=0; index < numQCMaterials; index++)
    {
        theQCMaterials[index]->DeleteQCLevelClass();
        delete theQCMaterials[index];
    }
    delete[] theQCMaterials;
    theQCMaterials = NULL;
    numQCMaterials = -1;
}
//---------------------------------------------------------------------------

void TQCMaterial::CopyFromQCLevelTableToClass( TQuery* query )
{
    query -> SQL -> Clear();
    query -> SQL -> Add(
        "select * from qc_level "
        "where material = :mat "
        "order by material, qc_level");

    for (int mat=0; mat < numQCMaterials; mat++)
    {
        theQCMaterials[mat]->DeleteQCLevelClass();
        theQCMaterials[mat]->LoadQCLevelsFromDatabase(query);
    }
}
//---------------------------------------------------------------------------
void TQCMaterial::CopyFromQCMaterialClassToGrid()
{
    QCMaterialGrid->RowCount = 2;
    QCMaterialGrid->FixedRows = 1;
    QCMaterialGrid->DeleteRow(1);
    QCMaterialGrid->RowCount = 2;
    QCMaterialGrid->FixedRows = 1;

    int row = 0;
    int numQCLevels = -1;
    for (int mat=0; mat<numQCMaterials; mat++)
    {
        numQCLevels = theQCMaterials[mat]->GetNumQCLevels();
        for (int lev=0; lev<numQCLevels; lev++)
        {
            row++;
            QCMaterialGrid->RowCount = row + 1;
            QCMaterialGrid->Cells[QCM_MATERIAL][row] =
                theQCMaterials[mat]->GetName();
            QCMaterialGrid->Cells[QCM_MATERIAL_DESCRIP][row] =
                theQCMaterials[mat]->GetDescription();
            QCMaterialGrid->Cells[QCM_MATERIAL_INDEX][row] = mat;

            QCMaterialGrid->Cells[QCM_LEVEL][row] =
                theQCMaterials[mat]->GetQCLevel(lev)->GetName();
            QCMaterialGrid->Cells[QCM_LEVEL_DESCRIP][row] = 
                theQCMaterials[mat]->GetQCLevel(lev)->GetDescription();
            QCMaterialGrid->Cells[QCM_LEVEL_INDEX][row] = lev;
		}

		if( theQCMaterials[mat]->GetNumQCLevels() == 0 )
        {
            row++;
            QCMaterialGrid->RowCount = row + 1;
            QCMaterialGrid->Cells[QCM_MATERIAL][row] =
                theQCMaterials[mat]->GetName();
            QCMaterialGrid->Cells[QCM_MATERIAL_DESCRIP][row] =
                theQCMaterials[mat]->GetDescription();
            QCMaterialGrid->Cells[QCM_MATERIAL_INDEX][row] = mat;

            QCMaterialGrid->Cells[QCM_LEVEL][row] = "";
            QCMaterialGrid->Cells[QCM_LEVEL_DESCRIP][row] = "";
            QCMaterialGrid->Cells[QCM_LEVEL_INDEX][row] = -1;
        }
    }
}
//---------------------------------------------------------------------------
void __fastcall TQCMaterial::AddMaterialClick(TObject *Sender)
{
    AddQCMaterial->SetFunction( "Add" );
    QCMaterialClass* Material = new QCMaterialClass;
    AddQCMaterial->SetMaterialClass( Material );
    AddQCMaterial->ShowModal();
    delete Material;
    CopyFromQCMaterialTableToGrid( QueryCentral );
}
//---------------------------------------------------------------------------
int TQCMaterial::FindIndexOfQCMaterial( AnsiString MaterialCode )
{
    int index = -1;

    for ( int mat=0; mat<numQCMaterials; mat++ )
    {
        if ( theQCMaterials[mat]->GetName() == MaterialCode )
        {
            index = mat;
        }
    }
    return index;
}
//---------------------------------------------------------------------------

void __fastcall TQCMaterial::ChangeMaterialClick(TObject *Sender)
{
    TColor colour;
    bool rowFound = false;

    for ( int row=1; row<QCMaterialGrid->RowCount; row++ )
    {
        if ( QCMaterialGrid->GetRowColour( row, colour ) )
        {
            rowFound = true;
            AddQCMaterial->SetFunction( "Change" );
			int index = StrToInt(QCMaterialGrid->Cells[QCM_MATERIAL_INDEX][row]);
            QCMaterialClass* theMat = theQCMaterials[index];
            AddQCMaterial->SetMaterialClass( theMat );
            AddQCMaterial->ShowModal();
            QCMaterialGrid->DelRowColour( row );
        }
    }

    if ( rowFound )
    {
        CopyFromQCMaterialTableToGrid( QueryCentral );
    }
    else
    {
        Application->MessageBox(
            "No row changed.",
            "QCMaterial Message",MB_OK);
    }
}
//---------------------------------------------------------------------------
void __fastcall TQCMaterial::AddLevelClick(TObject *Sender)
{
    TColor colour;
    bool rowFound = false;

    for ( int row=1; row<QCMaterialGrid->RowCount; row++ )
    {
        if ( QCMaterialGrid->GetRowColour( row, colour ) )
        {
            rowFound = true;
			AddQCLevel->SetFunction( "Add" );
			QCLevelClass* theQCLevel = new QCLevelClass;
            theQCLevel->SetMaterial( QCMaterialGrid->Cells[QCM_MATERIAL][row] );
            AddQCLevel->SetQCLevelClass( theQCLevel );
            AddQCLevel->ShowModal();
            delete theQCLevel;
        }
    }

    if ( rowFound )
    {
        CopyFromQCMaterialTableToGrid( QueryCentral );
    }
    else
    {
        Application->MessageBox(
            "No row specified.",
            "QCMaterial Message",MB_OK);
    }
}
//---------------------------------------------------------------------------
int TQCMaterial::FindIndexOfQCLevel( int mat, AnsiString LevelCode )
{
    int index = -1;

    for ( int lev=0; lev<theQCMaterials[mat]->GetNumQCLevels(); lev++ )
    {
        if ( theQCMaterials[mat]->GetQCLevel(lev)->GetName() == LevelCode )
        {
            index = lev;
        }
    }
    return index;
}
//---------------------------------------------------------------------------

void __fastcall TQCMaterial::ChangeLevelClick(TObject *Sender)
{
    TColor colour;
    bool rowFound = false;

    for ( int row=1; row<QCMaterialGrid->RowCount; row++ )
    {
		if( QCMaterialGrid->GetRowColour( row, colour ) )
		{
			int lev = QCMaterialGrid->Cells[QCM_LEVEL_INDEX][row].ToIntDef( -1 );
			if( lev >= 0 )
			{
				rowFound = true;
				AddQCLevel->SetFunction( "Change" );
				int mat = QCMaterialGrid->Cells[QCM_MATERIAL_INDEX][row].ToIntDef( 0 );
				AddQCLevel->SetQCLevelClass( theQCMaterials[mat]->GetQCLevel(lev) );
                AddQCLevel->ShowModal();
                QCMaterialGrid->DelRowColour( row );
            }
            else
            {
                Application->MessageBox(
                    "No level to change for this row.\n"
                    "Use Add Level.",
                    "QCMaterial Message",MB_OK);
            }
        }
    }

    if ( rowFound )
		CopyFromQCMaterialTableToGrid( QueryCentral );
	else
	{  	Application->MessageBox(
            "No row changed.",
            "QCMaterial Message",MB_OK);
    }
    return;
}
//---------------------------------------------------------------------------

void __fastcall TQCMaterial::QCMaterialGridMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	int Col = 0, Row = 0;
    QCMaterialGrid->MouseToCell( X, Y, Col, Row );
	if( Row < 1 )
    {
        Application->MessageBox(
            "Can not access row.\n"
            "Scroll down and try again.",
            "QCMaterial Message", MB_OK );
    }
    else
    {

    // if colour is already set, unset it and remove row from list of selected rows
    // if colour is not set, set it and add row to list of selected rows
    TColor colour;
	if ( QCMaterialGrid->GetRowColour( Row, colour ) )
		QCMaterialGrid->DelRowColour( Row );
	else
		QCMaterialGrid->SetRowColour( Row, clLime );
	}
}
//---------------------------------------------------------------------------

void __fastcall TQCMaterial::btnCloseClick(TObject *Sender)
{
    DeleteQCMaterialClass();
    Close();
}
//---------------------------------------------------------------------------

