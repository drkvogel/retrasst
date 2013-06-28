//---------------------------------------------------------------------------

#ifndef AddQCLevel1H
#define AddQCLevel1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ColourStringGrid.h"
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class QCLotClass;
class QCLevelClass;

class TAddQCLevel : public TForm
{
__published:	// IDE-managed Components
    TLabel *lblCode;
    TEdit *edtLevel;
    TLabel *lblDescrip;
    TEdit *edtDescrip;
    TPanel *Panel1;
    TButton *btnCancel;
    TButton *btnSave;
    TLabel *Label1;
    TLabel *lblMaterial;
    TColourStringGrid *QCLevelGrid;
    TPanel *Panel2;
    TButton *btnAddLot;
    TLabel *Label3;
    TButton *btnChangeLot;
    TPanel *Panel3;
    TButton *btnClose;
	TQuery *QueryCentral;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall btnAddLotClick(TObject *Sender);
    void __fastcall btnChangeLotClick(TObject *Sender);
    void __fastcall QCLevelGridMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnCloseClick(TObject *Sender);

private:	// User declarations

    QCLevelClass* theQCLevel;
    AnsiString Function;
    void ClearGrid();
    void CopyFromQCLotTableToGrid( TQuery* query );
    void CopyFromQCLotTableToClass( TQuery* query );
    void DeleteQCLotClass();
    void CopyFromQCLotClassToGrid();
    int FindIndexOfQCLot( AnsiString LotCode );

    static const int QCL_LOT;
    static const int QCL_LOT_DESCRIP;
    static const int QCL_ARRIVAL_DATE;
    static const int QCL_LOT_INDEX;
    static const int QCL_NUMCOLS;

public:		// User declarations
    __fastcall TAddQCLevel(TComponent* Owner);
    void SetFunction( AnsiString funct ) { Function = funct; }
    void SetQCLevelClass( QCLevelClass* qcl ) { theQCLevel = qcl; }
    QCLotClass** theQCLots;
    int numQCLots;
};
//---------------------------------------------------------------------------
extern PACKAGE TAddQCLevel *AddQCLevel;
//---------------------------------------------------------------------------
#endif
