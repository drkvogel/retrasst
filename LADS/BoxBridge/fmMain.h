//---------------------------------------------------------------------------

#ifndef fmMainH
#define fmMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "frmCommon.h"
#include <ComCtrls.hpp>
#include <Db.hpp>
#include <DBTables.hpp>
#include <ExtCtrls.hpp>
#include <DB.hpp>
#include <Menus.hpp>
#include <Buttons.hpp>
#include <Graphics.hpp>
#include <vector>

//---------------------------------------------------------------------------

class TfrmMain : public TForm
{
	BEGIN_MESSAGE_MAP
	 MESSAGE_HANDLER(WM_DEVICECHANGE,TMessage,OnDeviceChange)
	END_MESSAGE_MAP(TForm)
__published:	// IDE-managed Components
    TButton *btnSync;
    TButton *btnOpenLog;
    TProgressBar *progressMajor;
    TLabel *labelMinor;
    TLabel *labelMajor;
    TProgressBar *progressMinor;
    TTable *TableCentral;
    TTable *TableLaptop;
    TBatchMove *BatchMove1;
    TPanel *panelTime;
    TLabel *labelTime;
    TTimer *Timer1;
    TButton *btnResolve;
    TMainMenu *MainMenu1;
    TMenuItem *N1;
    TMenuItem *About1;
    TMenuItem *menuItemAbout;
    TQuery *qProject;
    TQuery *qCentral;
    TQuery *qLaptop;
    TDatabase *tdbLaptop;
    TMemo *memoLog;
    TStatusBar *StatusBar1;
    TButton *btnLogToggle;
    TPanel *panelUSBIndicator;
    TPanel *PanelUSBMain;
    TGroupBox *GroupBox1;
    TGroupBox *GroupBox2;
    TButton *btnSuccessful;
    TButton *btnTransferred;
    TPanel *panelTransferred;
    TPanel *panelSuccessful;
    TPanel *panelExpected;
    TButton *btnExpected;
    TImage *Image1;
    TGroupBox *GroupBox3;
    TTrayIcon *TrayIcon1;
    TButton *btnErrorsToday;
    TPanel *panelErrorsToday;
    TPanel *panelErrorsOlder;
    TButton *btnErrorsTotal;
    void __fastcall btnSyncClick(TObject *Sender);
    void __fastcall btnExitClick(TObject *Sender);
    void __fastcall Timer1Timer(TObject *Sender);
    void __fastcall menuItemAboutClick(TObject *Sender);
    void __fastcall btnResolveClick(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall btnLogToggleClick(TObject *Sender);
    void __fastcall FormCanResize(TObject *Sender, int &NewWidth, int &NewHeight, bool &Resize);
    void __fastcall btnOpenLogClick(TObject *Sender);
    void __fastcall btnTransferredClick(TObject *Sender);
    void __fastcall btnSuccessfulClick(TObject *Sender);
    void __fastcall btnExpectedClick(TObject *Sender);
    void __fastcall OnDeviceChange(TMessage &Msg);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall panelExpectedDblClick(TObject *Sender);
    void __fastcall TrayIcon1DblClick(TObject *Sender);
    void __fastcall btnErrorsTodayClick(TObject *Sender);
    void __fastcall btnErrorsTotalClick(TObject *Sender);
private:
    String addToLog(String message, String type);
    void initialise();
    void getExpected();
	int getExpectedKadoorieType(LCDbProject proj);
	int getExpectedThriveType(LCDbProject proj);
    void getThriveTypeCryovialSlotPositions(LBoxExpectedRecord & box, LQuery q);
    void getThriveTypeCryovialBarcodes(LBoxExpectedRecord & box, LQuery q);
    String encryptBarcode(String input);
    String decryptBarcode(String input);
    void writeToLBoxExpected();
    void checkKey();
    void doSync();
    void syncArrivals();
    void getArrivalsFromLaptop();
    void updateArrivalsToLaptop();
    void processArrivals();
    void processThriveType(BoxArrivalRecord & rec);
    void processKadoorieType(BoxArrivalRecord & rec);
    void getCIDs();
    void switchProject(int cid);
    void useCentralDB();
    void copyAuditTrail();
    void copyCentralTableToLaptop(String tableName);
    void copyTables();
    void copyTable(TQuery *qSource, TQuery *qDest, String sourceTable, String destTable, bool deleteDest);
    void syncCOperator();
    void stripNewlines(String & message);
    void minorError(String message, String where);
    void fatalError(String message, String where);
    void updateStatus(String major, String minor);
    int  getKadoorieBoxType(BoxArrivalRecord rec);
    void refreshTally();
    void updateSyncTime();
    String getSysErrorMessage();
    void insertEvent(int event_code);
    void getEncryptionKey();
    //void findUSBvol();
    void findKey();
    String getSite(int tank_cid);
    tdmapFields getSpecimenFieldNames();
    String getDescriptor(const String & descripName);
    LCDbProject currentProject;
    void queryExec(LQuery q);
    void testing();
    bool logVisible;
    int  formWidthNoLog;
    int  formHeightNoLog;
    int  formWidthWithLog;
    int  formHeightWithLog;
    String LogFileName;
    char USBvol;
    bool errorThrown;
public:		// User declarations
    bool diskArrival(const long lp);
    bool diskRemoval(const long lp);
    __fastcall TfrmMain(TComponent* Owner);
};

extern PACKAGE TfrmMain *frmMain;

#endif
