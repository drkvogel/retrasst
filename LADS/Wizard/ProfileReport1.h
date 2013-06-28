//---------------------------------------------------------------------------
#ifndef ProfileReport1H
#define ProfileReport1H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <quickrpt.hpp>
#include <Qrctrls.hpp>
#include <QuickRpt.hpp>
//---------------------------------------------------------------------------
class TProfileReport : public TForm
{
__published:	// IDE-managed Components
    TQuickRep *QuickRep1;
	TQuery *ddbQuery;
    TQRBand *QRBand1;
    TQRBand *QRBand2;
    TQRSysData *QRSysData1;
    TQRSysData *QRSysData2;
    TQRSysData *QRSysData3;
    TQRGroup *QRGroup1;
    TQRDBText *QRDBText6;
    TQRDBText *QRDBText2;
    TQRDBText *QRDBText7;
    TQRBand *QRBand3;
    TQRLabel *QRLabel1;
    TQRLabel *QRLabel2;
    TQRLabel *QRLabel3;
    TQRLabel *QRLabel5;
    TQRLabel *QRLabel6;
    TQRDBText *QRDBText13;
    TQRDBText *QRDBText1;
    TQRDBText *QRDBText8;
    TQRDBText *QRDBText9;
    TQRDBText *QRDBText11;
    TQRDBText *QRDBText4;
    TQRLabel *QRLabel4;
private:	// User declarations
public:		// User declarations
    __fastcall TProfileReport(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TProfileReport *ProfileReport;
//---------------------------------------------------------------------------
#endif
