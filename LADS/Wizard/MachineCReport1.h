//---------------------------------------------------------------------------
#ifndef MachineCReport1H
#define MachineCReport1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <DB.hpp>

//---------------------------------------------------------------------------
class TMachineCReport : public TForm
{
__published:	// IDE-managed Components
    TQuickRep *QuickRep1;
    TQRBand *QRBand1;
    TQRSysData *QRSysData1;
    TQRSysData *QRSysData2;
    TQRSysData *QRSysData3;
    TQRGroup *QRGroup1;
    TQRBand *QRBand2;
    TQRBand *QRBand3;
    TQuery *Query1;
    TQRDBText *QRDBText1;
    TQRDBText *QRDBText2;
    TQRDBText *QRDBText3;
    TQRDBText *QRDBText4;
    TQRLabel *QRLabel1;
    TQRDBText *QRDBText5;
private:	// User declarations
public:		// User declarations
    __fastcall TMachineCReport(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TMachineCReport *MachineCReport;
//---------------------------------------------------------------------------
#endif
