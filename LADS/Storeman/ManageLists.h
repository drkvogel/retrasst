#ifndef ManageListsH
#define ManageListsH

#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "RetrievalListBase.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>

class TfrmManageLists : public TfrmRetrievalJobList {
__published:	// IDE-managed Components
    void __fastcall sgJobsDblClick(TObject *Sender);
private:	// User declarations
protected:
    void initCustom();
public:		// User declarations
    __fastcall TfrmManageLists(TComponent* Owner);
};

extern PACKAGE TfrmManageLists *frmManageLists;
#endif
