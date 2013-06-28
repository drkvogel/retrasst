//---------------------------------------------------------------------------

#ifndef AddBoxType1H
#define AddBoxType1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ColourStringGrid.h"
#include <Grids.hpp>
class AliquotTypeClass;
class BoxTypeClass;
//---------------------------------------------------------------------------
class TAddBoxType : public TForm
{
__published:	// IDE-managed Components
    TLabel *Label1;
    TEdit *edtBoxName;
    TLabel *Label6;
    TPanel *Panel1;
    TButton *Save;
    TButton *Cancel;
    TLabel *Label2;
    TEdit *edtCapacity;
    TComboBox *cbxStatus;
    TPanel *Panel3;
    TLabel *Label3;
    TButton *btnAddAliquot;
    TButton *btnDeleteAliquot;
    TColourStringGrid *grdAliquot;
    TLabel *Label4;
    TPanel *Panel2;
    TButton *btnClose;
    void __fastcall SaveClick(TObject *Sender);
    void __fastcall CancelClick(TObject *Sender);
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall btnAddAliquotClick(TObject *Sender);
    void __fastcall btnDeleteAliquotClick(TObject *Sender);
    void __fastcall grdAliquotMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall grdAliquotMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
    void __fastcall btnCloseClick(TObject *Sender);

private:	// User declarations
    int  aliquotRowMoved;
    int  mouseDownRow;
    TQuery* QueryCentral;
    BoxTypeClass* theBoxType;
    AnsiString Function;

    static const int ALI_NAME;
    static const int ALI_ALIORDER;
    static const int ALI_ALITYPEID;
    static const int ALI_ALITYPEIDX;
    static const int ALI_NUMCOLS;

    void DefineAliquotGrid();
    bool SaveBoxType();
    bool SaveAliquotTypes();
    void DeleteAliquotRow( int row );
    void CopyFromDatabaseToGrid();
    void CopyFromDatabaseToClass();
    void CopyFromClassToGrid();
    bool CheckDuplicateAliquotOrder();
    void MoveRow( int fromRow, int toRow );

public:		// User declarations
    __fastcall TAddBoxType(TComponent* Owner);
    void SetQueryCentral( TQuery* q ) { QueryCentral = q; }
    void SetBoxType( BoxTypeClass* btc ) { theBoxType = btc; }
    void SetFunction( AnsiString funct ) { Function = funct; }
};
//---------------------------------------------------------------------------
extern PACKAGE TAddBoxType *AddBoxType;
//---------------------------------------------------------------------------
#endif
