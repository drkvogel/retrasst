//---------------------------------------------------------------------------

#include <vcl.h>
#include "TfrmAboutBox.h"
#include "TfrmCommsMain.h"
#include "isysProtocol.h"
#include "isysMessage.h"
#include "isysResult.h"
#include "isysWorklist.h"
#include "LIMSParams.h"
#include "isysMainForm.h"
#include "TfrmSerialConfig.h"
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TfrmCommsMain"
#pragma link "TfrmCommsMain"
#pragma link "IdExplicitTLSClientServerBase"
#pragma link "IdSMTPBase"
#pragma resource "*.dfm"
TisysMainForm *isysMainForm;
//---------------------------------------------------------------------------

const TmainForm::FRAME_TIME = 175;
const TmainForm::ACCEPT_RETRIES = 20000 / TmainForm::FRAME_TIME;
const TmainForm::TX_RETRIES = 3;
const TmainForm::RECOVERY_TIME = 60000;

const char * TfrmAboutBox::author = "Chris Bird";
const char * TfrmAboutBox::extension = "x3511";
const char * LCDbAuditTrail::default_email = "chris.bird@ctsu.ox.ac.uk";
const char * TfrmAboutBox::compiled = __DATE__;

//---------------------------------------------------------------------------

__fastcall TisysMainForm::TisysMainForm(TComponent* Owner)
	: TmainForm(Owner)
{
#ifdef _DEBUG
	Application -> Title = "iSys Comms **DEBUG**";
#else
	Application -> Title = "IDS-iSYS Comms";
#endif
	version -> Caption = "iSys v" + LIMSParams::instance().getProgVersion();
}
//---------------------------------------------------------------------------
//	Scan worklist after a minute's inactivity; send test order if necessary
//---------------------------------------------------------------------------

CommsMessage * TisysMainForm::nextMessageToSend()
{
	static const TTime ONE_MINUTE = 1.0 / (24 * 60);
	if (Now() >= update + ONE_MINUTE)
	{
		worklist.addPendingSamples(qCluster, qCentral);
		update = Now();
	}

//    if (IsysResult::INFO_REQUESTED == received.getResultStage()) {
//        worklist.setState(IsysWorklist::INFO_REQUESTED);
//    }

/*    if (IsysDebugComms::useDebugComms) {
		char* strProgramState[]    = {"STARTING", "CLEARING", "WAITING", "HAD_DATA", "SENDING", "STOPPED"};

		if (Now() >= update + TENTHOFASECOND) {
			String debugMsg;
			debugMsg.sprintf("programState: %s [%d]", strProgramState[getProgramState()], getProgramState());
			((IsysDebugComms *)input)->printMessage(debugMsg);
			debugMsg.sprintf("protocolState: %s [%d]", IsysProtocol::strProtocolState(protocol.getProtocolState()), protocol.getProtocolState());
			((IsysDebugComms *)input)->addMessage(debugMsg);
			debugMsg.sprintf("resultState: %s [%d]", IsysResult::strResultStage(received.getResultStage()), received.getResultStage());
			((IsysDebugComms *)input)->addMessage(debugMsg);
			debugMsg.sprintf("worklistState: %s [%d]", IsysWorklist::strWorklistState(worklist.getState()), worklist.getState());
			((IsysDebugComms *)input)->addMessage(debugMsg);
		}
	 }
*/
	return worklist.nextRequest(qCluster, qCentral);
}

//---------------------------------------------------------------------------

void __fastcall TisysMainForm::cbCategoryChange(TObject *Sender)
{
	worklist.setCategory(getSelectedCategoryID());
}

//---------------------------------------------------------------------------
//  We have data from the analyser; store it in the buddy database
//---------------------------------------------------------------------------

bool TisysMainForm::storeReceivedData( const CommsMessage & message )
{
	if (received.add(message, qCluster, qCentral))
		update = Now();
	return true;                 	// message always processed
}

//---------------------------------------------------------------------------
//	report response to last message and summary of actions taken
//---------------------------------------------------------------------------

String TisysMainForm::getSummary()
{
	String rxSummary = received.summary();
	return rxSummary.IsEmpty() ? worklist.summary() : rxSummary;
}

//---------------------------------------------------------------------------
//	default comms handler and configuration for (most) serial comms
//---------------------------------------------------------------------------

LIMSCommsBase * TisysMainForm::getSerialComms()
{
	static SerialComms * io = NULL;
	if( io == NULL )
		io = new SerialComms();
	return io;
}

//---------------------------------------------------------------------------

TfrmCommsConfig * TisysMainForm::getConfigForm()
{
	return frmSerialConfig;
}

//---------------------------------------------------------------------------

