//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TfrmSerialConfig.h"
#include "LIMSParams.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
#pragma link "TfrmCommsConfig"
#pragma resource "*.dfm"

TfrmSerialConfig *frmSerialConfig;

//---------------------------------------------------------------------------

__fastcall TfrmSerialConfig::TfrmSerialConfig(TComponent* Owner)
	: TfrmCommsConfig(Owner)
{}

//---------------------------------------------------------------------------

void __fastcall TfrmSerialConfig::FormActivate(TObject *Sender)
{
	LIMSParams & settings = LIMSParams::instance();
	if( settings.openSection( "serial", false ) )
	{
		String port = settings.findValue( "port" );
		if( !port.IsEmpty() )
			edPort -> Text = port;

		String baud = settings.findValue( "baud" );
		if( !baud.IsEmpty() )
			edBaud -> Text = baud;

		String data = settings.findValue( "data" );
		if( !data.IsEmpty() )
			edData -> Text = data;

		String parity = settings.findValue( "parity" );
		if( !parity.IsEmpty() )
			edParity -> Text = parity;

		String stop = settings.findValue( "stop" );
		if( !stop.IsEmpty() )
			edStop -> Text = stop;
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmSerialConfig::edPortExit(TObject *Sender)
{
	if( edPort -> Text.Length() < 3 ) {
		Application -> MessageBox( "Enter port name, e.g. COM1", NULL, MB_ICONWARNING );
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmSerialConfig::edDataExit(TObject *Sender)
{
	if( edData -> Text != "7" && edData -> Text != "8" ) {
		Application -> MessageBox( "Data bits should be 7 or 8", NULL, MB_ICONWARNING );
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmSerialConfig::edParityExit(TObject *Sender)
{
	const String valid = "oenOEN";
	if( edParity -> Text.Length() != 1 || valid.Pos( edParity -> Text ) == 0 ) {
		Application -> MessageBox( "Parity should be O, E or N", NULL, MB_ICONWARNING );
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmSerialConfig::edStopExit(TObject *Sender)
{
	if( edStop -> Text != "1" && edStop -> Text != "1.5" && edStop -> Text != "2" ) {
		Application -> MessageBox( "Stop bits should be 1, 1.5 or 2", NULL, MB_ICONWARNING );
	}
}

//---------------------------------------------------------------------------

void __fastcall TfrmSerialConfig::okButtonClick(TObject *Sender)
{
	TfrmCommsConfig::okButtonClick( Sender );
	LIMSParams & settings = LIMSParams::instance();
	settings.openSection( "serial", true );
	settings.setValue( "port", edPort -> Text );
	settings.setValue( "baud", edBaud -> Text );
	settings.setValue( "data", edData -> Text );
	settings.setValue( "parity", edParity -> Text );
	settings.setValue( "stop", edStop -> Text );
	settings.closeSection();
}

//---------------------------------------------------------------------------

