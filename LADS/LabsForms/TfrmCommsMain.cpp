/*---------------------------------------------------------------------------
 *
 * 	User interface for LIMS communications programs (if no log-in screen)
 *
 *		17 June 2005, NG:   Added file input as alternative to serial I/O
 *      23 June 2005, NG:   Added CommsProtocol object with default handler
 *		14 October 2005:	Modified CommsProtocol to use CommsMessage
 *      18 October, NG:		Switch back to serial i/o after each file input
 *      20 October 05, NG:	Added form for user to configure serial port
 *		24 October 05, NG:	Moved message handling code into CommsProtocol
 *      22 November 2005:	Modidifed interface (again) to use respond()
 *      14 February 2006:	Allow input from XML file as well as ASCII text
 *      20 Nov 09, NG:		Switch alias_local_buddy: test/live/mirror
 *		23 Nov 09, NG:		Incorporated base classes for message and results
 *		30 Nov 09, NG		Split work-list handling from storing results
 *      5 Feb 2010, NG:		Display detail and summary from sub-class
 *      10 Feb. '10, NG:	Stop audit trail explicitly when form destroyed
 *      2 Sept. '10, NG:	Warn user before changing sample category
 *		8 Sep 2010, NG:		Record bytes sent or received in the audit trail
 *		25 March 11, NG:	Move database selection to configuration screen
 *      20 July 2011, NG:	Don't ask about category unless choice is valid
 *		22/11/11, NG:	  	Use static for config to allow replacement form
 *		16 Dec 2011, NG:	Don't delete input (allow for network comms form)
 *
 *--------------------------------------------------------------------------*/

#include <vcl.h>

#include "TfrmCommsMain.h"
#include "LQuery.h"
#include "TfrmAboutBox.h"
#include "LIMSParams.h"
#include "LPDbProfile.h"
#include "LCDbAnalyser.h"
#include "LCDbObject.h"
#include "LCDbProject.h"
#include "LCDbTest.h"
#include "leaseIDs.h"
#include "XMLFile.h"
#include "LCDbQCLevel.h"
#include "LDbWorklist.h"

#pragma hdrstop
#pragma package(smart_init)

#pragma resource "*.dfm"

//---------------------------------------------------------------------------

TmainForm *mainForm;

const int LCDbProjects::leaseID = LEASEE_DEFAULT;

//---------------------------------------------------------------------------

__fastcall TmainForm::TmainForm(TComponent* Owner)
 : TForm( Owner ), trail( qCentral ), input( NULL ), inputType( NONE ),
   programState( STARTING ), rx_bytes( 0 ), tx_bytes( 0 )
{
#ifdef _DEBUG
	longAudit -> Checked = true;
#else
	longAudit -> Checked = false;
#endif
	Application -> OnException = showException;
	timer -> Enabled = true;
}

//---------------------------------------------------------------------------
//  Report an error; wait longer each time for user intervention
//---------------------------------------------------------------------------

void __fastcall TmainForm::showException(TObject *, Exception *E)
{
	static int errors = 0;
	Display_Message( "Error: " + E -> Message, NOTIFY );
	Screen -> Cursor = crDefault;
	bool stop = false;
	switch( programState )
	{
		case STOPPED:
			return;

		case STARTING:
		case CLEARING:
			if( ++ errors < 5 )
				timer -> Interval += RECOVERY_TIME;
			else
				stop = true;
			break;

		default:
			errors = 1;
			setProgramState( CLEARING );
	}

	if( stop ) {
		setProgramState( STOPPED );
		Display_Message( "Too many errors - program shutting down", NOTIFY );
		Application -> Terminate();
	} else {
		timer -> Enabled = true;
	}
}

//---------------------------------------------------------------------------
//	Menu commands - show the about box, toggle message detail, configure
//					serial port, accept file input, close the application
//---------------------------------------------------------------------------

void __fastcall TmainForm::About1Click(TObject *)
{
	frmAboutBox -> ShowModal();
}

//---------------------------------------------------------------------------

void __fastcall TmainForm::Configuration1Click(TObject *)
{
	timer -> Enabled = false;
	if( getConfigForm() -> ShowModal() == mrOk )
	{
		Display_Message( "Configuration changed - restarting", AUDIT );
		setProgramState( STARTING );
	}
	timer -> Enabled = true;
}

//---------------------------------------------------------------------------

void __fastcall TmainForm::longAuditClick(TObject *)
{
	longAudit -> Checked = !longAudit -> Checked;
}

//---------------------------------------------------------------------------

void __fastcall TmainForm::Open1Click(TObject *)
{
	if( openDialog -> Execute() )
		inputType = FILE_AVAILABLE;
}

//---------------------------------------------------------------------------

void __fastcall TmainForm::Reset1Click(TObject *)
{
	if( Application -> MessageBox( "Re-start analyser communications?\n"
				"Are you sure?", "Reset", MB_ICONQUESTION | MB_YESNO ) == IDYES )
	{
		Display_Message( "Reset requested - clearing", AUDIT );
		setProgramState( CLEARING );
	}
}

//---------------------------------------------------------------------------

void __fastcall TmainForm::Exit1Click(TObject *)
{
	Close();
}

//---------------------------------------------------------------------------

void __fastcall TmainForm::FormClose(TObject *, TCloseAction &)
{
	if( programState != STOPPED )
	{
		setProgramState( STOPPED );
		if( tx_bytes > 0 || rx_bytes > 0 )
			logIOCounts();
		if( trail.hasStarted() )
			trail.stop();
	}

	LIMSParams & params = LIMSParams::instance();
	if( params.openSection( "worklist", true ) )
		params.setValue( "category", categoryID );
	params.saveSize( this );
}

//---------------------------------------------------------------------------
//	Keep category, form size and position from one run to the next
//---------------------------------------------------------------------------

void __fastcall TmainForm::FormCreate(TObject *)
{
	LIMSParams & params = LIMSParams::instance();
	if( params.openSection( "worklist", false ) )
		categoryID = params.findValue( "category", 0 );
	params.restoreSize( this );
}

//---------------------------------------------------------------------------
//	Allow user to filter worklist by sample category (0 == all)
//---------------------------------------------------------------------------

void __fastcall TmainForm::cbCategoryDropDown(TObject *)
{
	std::set< int > idList = LCDbWorkList().getCategoryIDs( qCentral );
	cbCategory -> Items -> Text = "(All)";
	for( Range< LCDbObject > or = LCDbObjects::records(); or.isValid(); ++ or )	{
		if( idList.count( or -> getID() ) != 0 ) {
			cbCategory -> Items -> Add( or -> getName() );
		}
	}
}

//---------------------------------------------------------------------------
//	User should confirm category change before it's used as a filter
//---------------------------------------------------------------------------

void __fastcall TmainForm::checkCategory()
{
	static String category;
	const String selected = cbCategory -> Text;
	if( category != selected )
	{
		LCDbObjects & names = LCDbObjects::records();
		if( names.empty() )
			names.read( qCentral, false );

		const LCDbObject * active = names.findByID( categoryID );
		if( !category.IsEmpty() )
		{
			if( selected.IsEmpty() || cbCategory -> ItemIndex == 0 )
			{
				const char * change = "Are you sure you want to clear the sample category?";
				if( Application -> MessageBox( change, "Please confirm", MB_YESNO ) == IDYES )
					active = NULL;
			}
			else
			{ 	const LCDbObject * other = names.find( selected, LCDbObject::SAMPLE_CATEGORY );
				if( other != NULL )
				{
					const String change = "Are you sure you want to change to " + selected + '?';
					if( Application -> MessageBox( change.c_str(), "Please confirm", MB_YESNO ) == IDYES )
						active = other;
				}
			}
		}

		if( active == NULL )
		{
			category = "(All)";
			categoryID = 0;
		}
		else
		{	category = active -> getName();
			categoryID = active -> getID();
		}
		cbCategory -> Text = category;
	}
}

//---------------------------------------------------------------------------

int TmainForm::getSelectedCategoryID()
{
	checkCategory();
	return categoryID;
}

//---------------------------------------------------------------------------
//  Display message in memo field and/or copy to log file or audit trail
//---------------------------------------------------------------------------

void TmainForm::Display_Message( String text, LogLevel logging )
{
	LCDbAuditTrail::MessageType level;
	bool display = false, log = false, audit = false, email = false;
	switch( logging )
	{
		case NOTIFY:
			if( !text.IsEmpty() )
			{
				level = LCDbAuditTrail::MAJOR_ERROR;
				display = log = audit = email = true;
			}
			break;

		case AUDIT:
			if( !text.IsEmpty() )
			{
				level = LCDbAuditTrail::INFORMATION;
				display = log = audit = true;
			}
			break;

		case DISPLAY:
			if( !text.IsEmpty() )
				display = log = true;
			break;

#ifdef _DEBUG
		default:
			if( longAudit -> Checked && !text.IsEmpty() )
				display = log = true;
#else
		case DETAIL:
			if( longAudit -> Checked && !text.IsEmpty() )
				display = true;
#endif
	}

	try
	{	if( display )
    	{
			if( longAudit -> Checked )
				history -> Lines -> Add( Now().DateTimeString() + " - " + text );
			else
				history -> Lines -> Add( text );
		}

		if( log )
			XMLFile::logError( "message", text );

		if( email )
			trail.sendEMail( text );

		if( audit )
			trail.addRecord( text, level );
	}
	catch( ... )
	{
		// don't make things worse by throwing another exception
	}
}

//---------------------------------------------------------------------------
// 	Initialise or act on received data, depending on program state
//---------------------------------------------------------------------------

void __fastcall TmainForm::timerTimer(TObject *)
{
	timer -> Enabled = false;      			// prevent re-entry
	bool aok;
	switch( programState )
	{
		case STARTING:
			Screen -> Cursor = crSQLWait;
			aok = initialise();             // re-load from database
			Screen -> Cursor = crDefault;
			break;

		case CLEARING:
			aok = reinitialise();			// clear buffers etc.
			break;

		case STOPPED:
			return;                   		// leaving timer off

		default:
			if( sendData() || acceptData() )
				Display_Message( getSummary(), DISPLAY );
			aok = true;
	}

	if( aok )
	{
		static unsigned tickCount = 0;
		static unsigned const tenMinutes = 600000 / FRAME_TIME;
		if( ++ tickCount >= tenMinutes )
		{
			logIOCounts();
			tickCount = 0;
		}
		timer -> Enabled = true;
	}
	else
	{   Display_Message( "Cannot start - program shutting down", NOTIFY );
		Application -> Terminate();
	}
}

//---------------------------------------------------------------------------

void TmainForm::logIOCounts()
{
	String summary;
	summary.printf( "%d bytes received; %d bytes sent", rx_bytes, tx_bytes );
	tx_bytes = rx_bytes = 0;
	Display_Message( summary, DETAIL );
}

//---------------------------------------------------------------------------
//	Queue the next message the program wants to send the analyser, then
// 	try to send whatever's in the queue: ack/nak, line bid or data frame
//---------------------------------------------------------------------------

bool __fastcall TmainForm::sendData()
{
	std::auto_ptr< CommsMessage > message( nextMessageToSend() );
	if( message.get() != NULL && getProtocol().queue( *message ) )
	{
		setProgramState( SENDING );
		Display_Message( "Sending: " + message -> toString(), DEBUG );
	}

	if( tx_data.empty() )
	{
		String reply = getProtocol().respond();
		if( reply.IsEmpty() )
			return false;
		tx_data = reply.c_str();
	}

	Buffer::Contents toSend = tx_data.getContents();
	if( toSend.length > 0 )
	{           
		unsigned sent = input -> sendBytes( toSend.data, toSend.length );
		tx_bytes += sent;
		tx_data.skip( sent );
	}

	static int tx_count = 0;
	if( tx_data.empty() )
	{
		setProgramState( WAITING );
		tx_count = 0;
	}
	else if( ++ tx_count >= TX_RETRIES )
		throw Exception( "cannot send response" );

	return true;
}

//---------------------------------------------------------------------------
//	Act on any data received; pass to sub-class and update program state
//---------------------------------------------------------------------------

bool __fastcall TmainForm::acceptData()
{
	if( input != NULL )
	{
		unsigned num_received = input -> available();
		if( num_received != 0 )
		{
			setProgramState( HAD_DATA );
			unsigned char * available = rx_data.makeSpace( num_received + 1 );
			num_received = input -> readBytes( available, num_received );
			rx_bytes += num_received;
			rx_data.accept( num_received );
		}
	}

	int used = 0;
	Buffer::Contents received = rx_data.getContents();
	if( received.data == NULL || received.length == 0 )
		checkFileInput();
	else
	{	used = getProtocol().accept( received.data, received.length );
		if( used < 0 )
			throw Exception( "unexpected input data" );
		rx_data.skip( used );
	}

	static int rx_count = 0;
	if( used == received.length )
	{
		setProgramState( WAITING );				// all data processed
		rx_count = 0;
	}
	else if( used == 0 && ++ rx_count == ACCEPT_RETRIES )
		throw Exception( "incomplete message received" );

	std::auto_ptr< CommsMessage > message( getProtocol().getMessage() );
	if( message.get() == NULL )
		return used > 0;
	else
	{	Display_Message( "Received: " + message -> toString(), DEBUG );
		return storeReceivedData( *message );
	}
}

//---------------------------------------------------------------------------
//  Attempt to re-initialise the communications protocol after an error
//---------------------------------------------------------------------------

bool TmainForm::reinitialise()
{
	setSerialInput();
	tx_data = SerialComms::NAK;
	rx_data.clear();
	setProgramState( WAITING );
	return true;
}

//---------------------------------------------------------------------------
//  Routine for setting and displaying the current protocol state
//---------------------------------------------------------------------------

void __fastcall TmainForm::setProgramState( ProgState newState )
{
	switch( newState )
	{
		case STARTING:
			statusBar -> SimpleText = "Initialising communication";
			break;

		case CLEARING:
			statusBar -> SimpleText = "Error: reinitialising";
			break;

		case WAITING:
			statusBar -> SimpleText = "Awaiting next message";
			break;

		case HAD_DATA:
			statusBar -> SimpleText = "Processing received data";
			break;

		case SENDING:
			statusBar -> SimpleText = "Sending response to analyser";
			break;

		case STOPPED:
			statusBar -> SimpleText = "Program closing; please wait";
			break;

		default:
			Display_Message( "Unexpected change of state", NOTIFY );
	}
	programState = newState;
}

//---------------------------------------------------------------------------
//  Attempt to initialise the program; assume serial input's required
//---------------------------------------------------------------------------

bool TmainForm::initialise()
{
	DbFamily system;
	DbFamily::DbSystem ltm( getConfigForm() -> rgDatabase -> ItemIndex );
	system.setDbSystem( tdbCentral, ltm, false );

	LCDbProjects & projects = LCDbProjects::records();
	projects.read( qCentral, true );
	LCDbObjects & names = LCDbObjects::records();
	names.read( qCentral, false );
	LCDbAnalysers & analysers = LCDbAnalysers::records();
	analysers.read( qCentral, false );

	LIMSParams & params = LIMSParams::instance();
	if( !params.checkUnique() || !params.checkMachine() || !params.checkDiskSpace() )
		return false;

	const LCDbAnalyser * analyser = analysers.findByID( LCDbAnalysers::getCurrentID() );
	if( analyser != NULL )
	{
		std::set< int > clusterIDs = analyser -> getClusterIDs();
		if( clusterIDs.empty() )
			buddyDetails -> Caption = analyser -> getName() + "; not clustered";
		else
		{	String list = analyser -> getName();
			std::set< int >::const_iterator ci = clusterIDs.begin();
			do list = list + "; " + names.get( *ci ).getName();
			while( ++ ci != clusterIDs.end() );
			buddyDetails -> Caption = list;
		}
	}

	LCDbTests::records().read( qCentral, false );
	LCDbQCLevels::records().read( qCentral, false );
	LPDbProfiles::initialise( qProject, false );

	trail.start();
	Caption = Application -> Title + " (" + trail.getProcessID() + ')';
	getProtocol().init();
	setSerialInput();

	String central = system.getCentralDb();
	system.changeDbName( tdbCluster, central );
	String systemName = system.getName();
	Display_Message( "Using " + systemName + " database system (" + central + ")", DISPLAY );
	return true;
}

//---------------------------------------------------------------------------
//	Switch from serial I/O to read from a file, then switch back again
//---------------------------------------------------------------------------

void __fastcall TmainForm::checkFileInput()
{
	switch( inputType )
	{
		case READING_FILE:

			if( input -> available() == 0 )
				setSerialInput();
			break;

		case FILE_AVAILABLE:
		{
			InputFile * file;
			if( openDialog -> FilterIndex == 2 )
				file = new XMLInputFile( openDialog -> FileName );
			else
				file = new TextInputFile( openDialog -> FileName );

			if( input != NULL )
			{
				logIOCounts();
				getProtocol().reset();
			}
			input = file;

			Display_Message( "Reading from file: " + input -> getSource(), AUDIT );
			timer -> Interval = 50;
			inputType = READING_FILE;
		}
	}
}

//---------------------------------------------------------------------------
//	Initialise serial I/O and/or switch back from data file input
//---------------------------------------------------------------------------

void TmainForm::setSerialInput()
{
	if( input != NULL )
	{
		logIOCounts();
		getProtocol().reset();
	}

	input = getSerialComms();
	Display_Message( "Ready to receive data from " + input -> getSource(), AUDIT );
	timer -> Interval = FRAME_TIME;
	inputType = SERIAL_IO;
	setProgramState( WAITING );
}

//---------------------------------------------------------------------------

