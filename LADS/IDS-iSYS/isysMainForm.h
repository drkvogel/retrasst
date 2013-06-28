//---------------------------------------------------------------------------

#ifndef isysMainFormH
#define isysMainFormH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <DB.hpp>
#include <DBTables.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include "TfrmCommsMain.h"
#include "IdExplicitTLSClientServerBase.hpp"
#include "IdSMTPBase.hpp"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdMessageClient.hpp>
#include <IdSMTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>

#include "isysProtocol.h"
#include "isysMessage.h"
#include "isysResult.h"
#include "isysWorklist.h"
//#include "isysDebugComms.h"

//---------------------------------------------------------------------------
class TisysMainForm : public TmainForm
{
__published:	// IDE-managed Components
	void __fastcall cbCategoryChange(TObject *Sender);

protected:

	LIMSCommsBase* newSerialComms() {
			return new SerialComms();
		}

private:	// User declarations

	IsysProtocol protocol;
	IsysWorklist worklist;
	IsysResult received;
	TDateTime update;

	// interface with main comms form
	IsysProtocol & getProtocol() { return protocol; }
	bool storeReceivedData( const CommsMessage & message );
	CommsMessage * nextMessageToSend();
	virtual String getSummary();
	virtual LIMSCommsBase * getSerialComms();
	virtual TfrmCommsConfig * getConfigForm();

public:		// User declarations
	IsysWorklist & getWorklist() { return worklist; }
	LIMSCommsBase * getInput() {return input; };
	__fastcall TisysMainForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TisysMainForm *isysMainForm;
//---------------------------------------------------------------------------
#endif
