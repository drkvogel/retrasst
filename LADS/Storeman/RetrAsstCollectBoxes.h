#ifndef RetrAsstCollectBoxesH
#define RetrAsstCollectBoxesH
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "LCDbJob.h"

class TfrmRetrAsstCollectBoxes : public TForm
{
__published:	// IDE-managed Components
private:	// User declarations
public:		// User declarations
    LCDbCryoJob *                               job;
    __fastcall TfrmRetrAsstCollectBoxes(TComponent* Owner);
    void                                        setJob(LCDbCryoJob * ajob) { job = ajob; };
};

extern PACKAGE TfrmRetrAsstCollectBoxes *frmRetrAsstCollectBoxes;
#endif
