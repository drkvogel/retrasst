//---------------------------------------------------------------------------

#ifndef RetrAsstCollectEmptiesH
#define RetrAsstCollectEmptiesH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.CheckLst.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TfrmCollectEmpties : public TForm
{
__published:	// IDE-managed Components
    TGroupBox *groupBoxes;
    TPanel *Panel1;
    TButton *btnClose;
    TStringGrid *StringGrid1;
    void __fastcall btnCloseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmCollectEmpties(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmCollectEmpties *frmCollectEmpties;
//---------------------------------------------------------------------------
#endif
