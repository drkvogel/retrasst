//---------------------------------------------------------------------------

#ifndef AddQCLot1H
#define AddQCLot1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ColourStringGrid.h"
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
//---------------------------------------------------------------------------
class QCLotClass;
class QCTestClass;

class TAddQCLot : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TLabel *lblMaterial;
    TLabel *lblCode;
    TEdit *edtLot;
    TLabel *lblDescrip;
    TEdit *edtDescrip;
    TPanel *pnlTest;
    TLabel *Label3;
    TButton *btnAddTest;
    TButton *btnChangeTest;
    TColourStringGrid *QCLotGrid;
    TPanel *Panel1;
    TButton *btnCancel;
    TButton *btnSave;
    TLabel *lblLevel;
    TLabel *Label2;
    TLabel *Label4;
    TEdit *edtArrivalDate;
    TButton *btnCopyTests;
    TButton *btnDeleteTest;
    TPanel *Panel2;
    TButton *btnClose;
	TQuery *QueryCentral;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall btnSaveClick(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall btnAddTestClick(TObject *Sender);
    void __fastcall btnChangeTestClick(TObject *Sender);
    void __fastcall QCLotGridMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnCopyTestsClick(TObject *Sender);
    void __fastcall btnDeleteTestClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);

private:	// User declarations

	QCLotClass* theQCLot;
    int PrevLotNumber;
    AnsiString Material;
    AnsiString Level;
    AnsiString Function;
    void ClearGrid();
    void CopyFromDatabaseToGrid( TQuery* query );
    void CopyFromDatabaseToClass( TQuery* query, QCLotClass* theQCLot );
    void CopyFromClassToGrid();
    bool GetPreviousLot( QCLotClass* thePrevQCLot, TQuery* query );
    void DeleteTest( int row );

    static const int QCLT_TEST;
    static const int QCLT_PACKET_MEAN;
    static const int QCLT_PACKET_STDDEV;
    static const int QCLT_TEST_INDEX;
    static const int QCLT_NUMCOLS;

public:		// User declarations
    __fastcall TAddQCLot(TComponent* Owner);
    void SetFunction( AnsiString funct ) { Function = funct; }
    void SetQCPrevLotNumber( int lotnum ) { PrevLotNumber = lotnum; }
    void SetQCLotClass( QCLotClass* lotClass ) { theQCLot = lotClass; }
};
//---------------------------------------------------------------------------
extern PACKAGE TAddQCLot *AddQCLot;
//---------------------------------------------------------------------------
#endif
