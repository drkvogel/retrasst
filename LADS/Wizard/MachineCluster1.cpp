//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MachineCluster1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ColourStringGrid"
#pragma resource "*.dfm"
TMachineClusters *MachineClusters;
//---------------------------------------------------------------------------
__fastcall TMachineClusters::TMachineClusters(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TMachineClusters::FormActivate(TObject *Sender)
{
    Caption = "Machine Cluster";
    if ( !WizardLogin->GetSystemLive() )
    {
        Caption = Caption + " (TEST Database)";
    }

    ClusterTable->ColCount = 4;
    CopyFromDatabaseToGrid();

    ClusterTable->Cells[MC_CLUSTER][0] = " Cluster";
    ClusterTable->ColWidths[MC_CLUSTER] = 60;
    ClusterTable->Cells[MC_NETADD][0] = "Network address";
    ClusterTable->ColWidths[MC_NETADD] = 40;
    ClusterTable->Cells[MC_DBPATH][0] = "Database path";
    ClusterTable->ColWidths[MC_DBPATH] = 100;
    ClusterTable->EditorMode = true;
}
//---------------------------------------------------------------------------
void __fastcall TMachineClusters::AddClick(TObject *Sender)
{
    AddCluster->SetRowNumber(0);
    AddCluster->Caption = "Add Cluster";
    AddCluster->ShowModal();
    CopyFromDatabaseToGrid();
}
//---------------------------------------------------------------------------
void __fastcall TMachineClusters::ChangeClick(TObject *Sender)
{
    TColor colour;
    bool rowFound = false;
    for ( int row=1; row<ClusterTable->RowCount; row++ )
    {
        if ( ClusterTable->GetRowColour( row, colour ) )
        {
            rowFound = true;
            AddCluster->SetRowNumber(row);
            AddCluster->Caption = "Change Machine Cluster";
            AddCluster->ShowModal();
            ClusterTable->DelRowColour( row );
        }
    }
    if ( rowFound )
    {
        CopyFromDatabaseToGrid();
    }
    else
    {
        Application->MessageBox(
            "No row changed.",
            "Descriptor Message",MB_OK);
    }
}
//---------------------------------------------------------------------------
void __fastcall TMachineClusters::btnCloseClick(TObject *Sender)
{
    DeleteClusterClass();
    Close();
}
//---------------------------------------------------------------------------
void __fastcall TMachineClusters::ClusterTableMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
    int Col = 0;
    int Row = 0;
    ClusterTable->MouseToCell( X, Y, Col, Row );

// if colour is already set, unset it and remove row from list of selected rows
// if colour is not set, set it and add row to list of selected rows

    TColor colour;
    if ( ClusterTable->GetRowColour( Row, colour ) )
    {
        ClusterTable->DelRowColour( Row );
    }
    else
    {
        ClusterTable->SetRowColour( Row, clLime );
    }
}
//---------------------------------------------------------------------------

void TMachineClusters::CopyFromClassToGrid()
{
    ClusterTable->RowCount = 2;
    ClusterTable->FixedRows = 1;
    ClusterTable->DeleteRow(1);
    ClusterTable->RowCount = 2;
    ClusterTable->FixedRows = 1;

    int row = 0;
    for (int index=0; index<numClusters; index++)
    {
        row++;
        ClusterTable->RowCount = row + 1;
        ClusterTable->Cells[MC_CLUSTER][row] =
            theClusters[index]->GetName();
        ClusterTable->Cells[MC_CLUSTERID][row] =
            theClusters[index]->GetClusterID();
        ClusterTable->Cells[MC_NETADD][row] =
            theClusters[index]->GetNetworkAddress();
        ClusterTable->Cells[MC_CLUSTER][row] =
            theClusters[index]->GetDatabasePath();
    }
}
//---------------------------------------------------------------------------

void TMachineCluster::CopyFromDatabaseToClass()
{
    DeleteClusterClass();

    Login->QueryCentral -> SQL -> Clear();
    Login->QueryCentral -> SQL -> Add(
        "select * from c_cluster "
		"where delete_status <> :del "
        "order by external_name");
	Login->QueryCentral -> ParamByName( "del" )->AsSmallInt = (short)DELETE_STATUS;
    Login->QueryCentral -> Open();
//    Login->QueryCentral -> First();
    numClusters = Login->QueryCentral->RecordCount;
    theClusters = new ClusterClass*[numClusters];
//    if ( numClusters > maxClusters )
//    {
//        Application->MessageBox(
//            "Please increase size of cluster class, max = maxClusters = 20",
//            "Cluster",MB_OK);
//    }
//    else
//    {
        for (int record=0; record < Login->QueryCentral->RecordCount; record++)
        {
            theClusters[record] = new ClusterClass;
            theClusters[record]->LoadFromDatabase(Login->QueryCentral,record);
        }
//    }
    Commit( Login->QueryCentral );
}
//---------------------------------------------------------------------------

void TMachineCluster::CopyFromDatabaseToGrid()
{
// delete all clusters,
// create new instances of class from database,
// copy into grid

    CopyFromDatabaseToClass();
    CopyFromClassToGrid();
}
//---------------------------------------------------------------------------

void TMachineCluster::DeleteClusterClass()
{
    for (int clusterindex=0; clusterindex < numClusters; clusterindex++)
    {
        delete theClusters[clusterindex];
    }
    delete[] theClusters;
    numClusters = -1;
}
//---------------------------------------------------------------------------
