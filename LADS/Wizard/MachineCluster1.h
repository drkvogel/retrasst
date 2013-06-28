//---------------------------------------------------------------------------
#ifndef MachineCluster1H
#define MachineCluster1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ColourStringGrid.h"
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TMachineClusters : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel2;
    TLabel *Label1;
    TButton *Add;
    TButton *Change;
    TButton *Delete;
    TColourStringGrid *ClusterTable;
    TPanel *Panel1;
    TButton *btnClose;
    void __fastcall AddClick(TObject *Sender);
    void __fastcall ChangeClick(TObject *Sender);
    void __fastcall btnCloseClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall ClusterTableMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
private:	// User declarations
    const int maxClusters;
public:		// User declarations
    __fastcall TMachineClusters(TComponent* Owner);
    void CopyFromDatabaseToGrid();
    void CopyFromClassToGrid();
    void CopyFromDatabaseToClass();
    void DeleteClusterClass();

    ClusterClass** theClusters;
    int numMClusters;
};
//---------------------------------------------------------------------------
extern PACKAGE TMachineClusters *MachineClusters;
//---------------------------------------------------------------------------
#endif
