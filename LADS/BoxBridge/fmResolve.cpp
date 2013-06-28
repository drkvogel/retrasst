//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "fmResolve.h"
#include "fmMain.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TfrmResolve *frmResolve;

extern tdvecBoxArrivalRecord  vecErrorsToday;
extern tdvecBoxArrivalRecord  vecErrorsTotal;

//---------------------------------------------------------------------------
__fastcall TfrmResolve::TfrmResolve(TComponent* Owner)
    : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmResolve::btnExitClick(TObject *Sender)
{
    Close();    
}
//---------------------------------------------------------------------------
void __fastcall TfrmResolve::FormActivate(TObject *Sender)
{
    esgErrors = new ESG(sgErrors);    
}
//---------------------------------------------------------------------------
void __fastcall TfrmResolve::FormClose(TObject *Sender, TCloseAction &Action)
{
    delete esgErrors;
}
//---------------------------------------------------------------------------
void __fastcall TfrmResolve::FormShow(TObject *Sender)
{
    tdvecBoxArrivalRecord::const_iterator erIt;

//    BoxArrivalRecord rec;
//
//    vecErrors.push_back(rec); //

//    if (vecErrorsToday.size() == 0) {
//        sgErrors->Visible = false;
//        return;
//    } else {
//        sgErrors->Visible = true;
//    }
//
//    sgErrors->RowCount = vecErrors.size() + 1;
//    int i = 1;
//    for (erIt = vecErrors.begin(); erIt != vecErrors.end(); erIt++, i++) {
//        BoxArrivalRecord rec = *erIt;
//
//        sgErrors->Cells[0][i] = "Laptop";
//        sgErrors->Cells[1][i] = String(rec.box_arrival_id);
//        sgErrors->Cells[2][i] = rec.errorMessage;
//    }
}
//---------------------------------------------------------------------------
