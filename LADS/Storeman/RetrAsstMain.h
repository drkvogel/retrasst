//---------------------------------------------------------------------------

#ifndef RetrAsstMainH
#define RetrAsstMainH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "RetrievalListBase.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TfrmRetrievalAssistant : public TfrmRetrievalJobList
{
__published:	// IDE-managed Components
private:	// User declarations
public:		// User declarations
    __fastcall TfrmRetrievalAssistant(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmRetrievalAssistant *frmRetrievalAssistant;
//---------------------------------------------------------------------------
#endif
