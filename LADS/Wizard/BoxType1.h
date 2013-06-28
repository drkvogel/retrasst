//---------------------------------------------------------------------------

#ifndef BoxType1H
#define BoxType1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ColourStringGrid.h"
#include <ExtCtrls.hpp>
#include <Grids.hpp>
class ProjectClass;
class BoxTypeClass;
class TAllAliquotTypes;
//---------------------------------------------------------------------------
class TBoxType : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel2;
    TLabel *Label1;
    TButton *Add;
    TButton *Change;
    TButton *Delete;
    TColourStringGrid *BoxTypeTable;
    TPanel *Panel1;
    TButton *btnClose;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall AddClick(TObject *Sender);
    void __fastcall ChangeClick(TObject *Sender);
    void __fastcall DeleteClick(TObject *Sender);
    void __fastcall btnCloseClick(TObject *Sender);
    void __fastcall BoxTypeTableMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:	// User declarations
    TQuery* QueryCentral;
    ProjectClass* theProject;
    BoxTypeClass** theBoxTypes;
    int numBoxes;
    void CopyFromDatabaseToGrid( TQuery* query );
    void CopyFromClassToGrid();
    bool DeleteBox( int row, TQuery* query  );
    void DeleteBoxTypeClass();
    bool IsBoxTypeInTable( int boxID, AnsiString tableName, TQuery* query );
    // columns in grid
    static const int BT_NAME;
    static const int BT_STATUS;
    static const int BT_CAPACITY;
    static const int BT_TYPE_ID;
    static const int BT_TYPE_IDX;
    static const int BT_NUM_COLS;
public:		// User declarations
    __fastcall TBoxType(TComponent* Owner);
    void SetQueryCentral( TQuery* q ) { QueryCentral = q; }
    void SetProject( ProjectClass* proj ) { theProject = proj; }
    void CopyFromDatabaseToClass( TQuery* query );
    TAllAliquotTypes* AllAliquotTypes;

};
//---------------------------------------------------------------------------
extern PACKAGE TBoxType *BoxType;
//---------------------------------------------------------------------------

#endif
