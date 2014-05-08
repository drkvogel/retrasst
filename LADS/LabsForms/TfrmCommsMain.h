//---------------------------------------------------------------------------

#ifndef TfrmCommsMainH
#define TfrmCommsMainH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
#include <stdio.h>
#include <alloc.h>
#include <ExtCtrls.hpp>
#include <Db.hpp>
//#include <DBTables.hpp>
#include <CheckLst.hpp>
#include <Buttons.hpp>
#include <Menus.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <DB.hpp>
#include <string>

#include "TfrmCommsConfig.h"
#include "LIMScommsBase.h"
#include "CommsProtocol.h"
#include "LCDbAuditTrail.h"

//---------------------------------------------------------------------------

class TmainForm : public TForm
{
__published:	// IDE-managed Components

	TMemo *history;
	TLabel *buddyDetails;
	TLabel *version;
	TDatabase *tdbCentral;
	TDatabase *tdbCluster;
	TQuery *qCentral;
	TQuery *qCluster;
	TDatabase *tdbProject;
	TQuery *qProject;
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *Help1;
	TMenuItem *About1;
	TMenuItem *Exit1;
	TStatusBar *statusBar;
	TTimer *timer;
	TMenuItem *N2;
	TMenuItem *Open1;
	TMenuItem *longAudit;
	TMenuItem *N1;
	TOpenDialog *openDialog;
	TMenuItem *N3;
	TMenuItem *Reset1;
	TMenuItem *Configuration1;
	TLabel *lblCategory;
	TComboBox *cbCategory;

	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall About1Click(TObject *Sender);
	void __fastcall longAuditClick(TObject *Sender);
	void __fastcall Exit1Click(TObject *Sender);
	void __fastcall timerTimer(TObject *Sender);
	void __fastcall Open1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Configuration1Click(TObject *Sender);
	void __fastcall Reset1Click(TObject *Sender);
	void __fastcall cbCategoryDropDown(TObject *Sender);

private:	// User declarations

	enum InputType { NONE, SERIAL_IO, FILE_AVAILABLE, READING_FILE, INPUT_FORM } inputType;

	Buffer rx_data, tx_data;
	unsigned rx_bytes, tx_bytes;
	LCDbAuditTrail trail;
	int categoryID;

	void __fastcall showException( TObject *Sender, Exception *E );
	void __fastcall checkCategory();
	void __fastcall checkFileInput();
	bool __fastcall readInput();
	bool __fastcall acceptData();
	bool __fastcall sendData();

protected:

	LIMSCommsBase * input;

	enum ProgState { STARTING, CLEARING, WAITING, HAD_DATA, SENDING, STOPPED } programState;

	void __fastcall setProgramState( ProgState newState );
	ProgState getProgramState() const { return programState; };

	void logIOCounts();
	void setSerialInput();

	virtual bool initialise();
	virtual bool reinitialise();

	// link these in from the analyser handling code
	static const FRAME_TIME, RECOVERY_TIME, ACCEPT_RETRIES, TX_RETRIES;

	virtual CommsProtocol & getProtocol() = 0;
	virtual LIMSCommsBase * getSerialComms() = 0;
	virtual TfrmCommsConfig * getConfigForm() = 0;
	virtual CommsMessage * nextMessageToSend() = 0;
	virtual bool storeReceivedData( const CommsMessage & message ) = 0;
	virtual String getSummary() = 0;

public:		// User declarations

	__fastcall TmainForm( TComponent* Owner );
	virtual int getSelectedCategoryID();

	enum LogLevel { DEBUG, DETAIL, DISPLAY, AUDIT, NOTIFY };
	virtual void Display_Message( String Message, LogLevel logging );
};

//---------------------------------------------------------------------------

extern PACKAGE TmainForm *mainForm;

//---------------------------------------------------------------------------

#endif
