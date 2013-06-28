//---------------------------------------------------------------------------

#ifndef QCMaterial1H
#define QCMaterial1H
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
class QCMaterialClass;
class QCLevelClass;

class TQCMaterial : public TForm
{
__published:	// IDE-managed Components
    TColourStringGrid *QCMaterialGrid;
    TPanel *Panel1;
    TButton *AddMaterial;
    TButton *ChangeMaterial;
    TButton *AddLevel;
    TButton *ChangeLevel;
    TLabel *Label3;
    TPanel *Panel2;
    TButton *btnClose;
	TQuery *QueryCentral;
    void __fastcall FormActivate(TObject *Sender);
    void __fastcall AddMaterialClick(TObject *Sender);
    void __fastcall ChangeMaterialClick(TObject *Sender);
    void __fastcall AddLevelClick(TObject *Sender);
    void __fastcall ChangeLevelClick(TObject *Sender);
    void __fastcall QCMaterialGridMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
    void __fastcall btnCloseClick(TObject *Sender);

private:	// User declarations

    void CopyFromQCMaterialTableToGrid( TQuery* query );
    void CopyFromQCMaterialTableToClass( TQuery* query );
    void DeleteQCMaterialClass();
    void CopyFromQCLevelTableToClass( TQuery* query );
    void CopyFromQCMaterialClassToGrid();

    static const int QCM_MATERIAL;
    static const int QCM_LEVEL;
    static const int QCM_MATERIAL_DESCRIP;
    static const int QCM_LEVEL_DESCRIP;
    static const int QCM_MATERIAL_INDEX;
    static const int QCM_LEVEL_INDEX;
    static const int QCM_NUMCOLS;

public:		// User declarations
    __fastcall TQCMaterial(TComponent* Owner);
    QCMaterialClass** theQCMaterials;
    int numQCMaterials;
	    int FindIndexOfQCMaterial( AnsiString MaterialCode );
    int FindIndexOfQCLevel( int mat, AnsiString LevelCode );
};
//---------------------------------------------------------------------------
extern PACKAGE TQCMaterial *QCMaterial;
//---------------------------------------------------------------------------
#endif
