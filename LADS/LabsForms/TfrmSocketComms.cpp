//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TfrmSocketComms.h"
#include "LIMSParams.h"
#include "XMLFile.h"

#pragma package(smart_init)
#pragma link "TfrmCommsConfig"
#pragma resource "*.dfm"

TfrmNetComms *frmNetComms;

//---------------------------------------------------------------------------

__fastcall TfrmNetComms::TfrmNetComms(TComponent* Owner)
	: TfrmCommsConfig(Owner), client( NULL )
{}

//---------------------------------------------------------------------------

String TfrmNetComms::open()
{
	clear();
	LIMSParams & settings = LIMSParams::instance();
	if( settings.openSection( "network", false ) ) {
		int port = settings.findValue( "port" ).ToIntDef( -1 );
		if( port > 0 ) {
			serverSocket -> Port = port;
		}
	}                       
	serverSocket -> Open();
	return "port " + String( serverSocket -> Port );
}

//---------------------------------------------------------------------------

void __fastcall TfrmNetComms::serverSocketClientConnect(TObject *Sender,
	  TCustomWinSocket *Socket)
{
	XMLFile::logError( "message", Socket->RemoteHost + " connected OK" );
	client = Socket;
}

//---------------------------------------------------------------------------

void __fastcall TfrmNetComms::serverSocketClientDisconnect(TObject *Sender,
	  TCustomWinSocket *Socket)
{
	XMLFile::logError( "message", Socket->RemoteHost + " disconnected" );
	client = NULL;
}

//---------------------------------------------------------------------------

unsigned TfrmNetComms::available()
{
	return client == NULL ? 0 : client -> ReceiveLength();
}

//---------------------------------------------------------------------------

unsigned TfrmNetComms::readBytes( void * buffer, unsigned max )
{
	return client == NULL ? 0 : client -> ReceiveBuf( buffer, max );
}

//---------------------------------------------------------------------------

unsigned TfrmNetComms::sendBytes( void * bytes, unsigned count )
{
	return client == NULL ? 0 : client -> SendBuf( bytes, count );
}

//---------------------------------------------------------------------------

void TfrmNetComms::clear()
{
	client = NULL;
	serverSocket -> Close();
}

//---------------------------------------------------------------------------

void __fastcall TfrmNetComms::FormClose(TObject *Sender, TCloseAction &Action)
{
	clear();
}

//---------------------------------------------------------------------------

void __fastcall TfrmNetComms::serverSocketClientError(TObject *Sender,
	  TCustomWinSocket *Socket, TErrorEvent ErrorEvent, int &ErrorCode)
{
	clear();
	throw Exception( "code " + String( ErrorCode ) + " from socket" );
}

//---------------------------------------------------------------------------

void __fastcall TfrmNetComms::FormActivate(TObject *Sender)
{
	LIMSParams & settings = LIMSParams::instance();
	String port, rtt, retries;
	if( settings.openSection( "network", false ) ) {
		port = settings.findValue( "port" );
		rtt = settings.findValue( "rtt" );
		retries = settings.findValue( "retries" );
	}
	if( port.IsEmpty() ) {
		tbPort -> Text = serverSocket -> Port;
	} else {
		tbPort -> Text = port;
	}
	if( !rtt.IsEmpty() ) {
		tbRTT -> Text = rtt;
	}
	if( !retries.IsEmpty() ) {
		tbRetries -> Text = retries;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmNetComms::okButtonClick(TObject *Sender)
{
	TfrmCommsConfig::okButtonClick( Sender );
	LIMSParams & settings = LIMSParams::instance();
	settings.openSection( "network", true );
	if( tbPort -> Text.ToIntDef( -1 ) > 0 ) {
		settings.setValue( "port", tbPort -> Text );
	}
	if( tbRTT -> Text.ToIntDef( -1 ) > 0 ) {
		settings.setValue( "rtt", tbRTT -> Text );
	}
	if( tbRetries -> Text.ToIntDef( -1 ) > 0 ) {
		settings.setValue( "retries", tbRetries -> Text );
	}
	settings.closeSection();
}

//---------------------------------------------------------------------------

