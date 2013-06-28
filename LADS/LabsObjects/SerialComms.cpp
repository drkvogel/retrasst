/*
 *	Serial communications class - based on Tim's serial line code
 *
 *		17 June 2005:	introduced base class to be sub-classed for file i/o
 *		18 Nov, NG:		base class logs messages; this adds TX and RX data
 *		24-11-05, NG:   return byte count from sendBytes(), even if incomplete
 *		22 Jan 08, NG:	moved Buffer class here from (depracated) LogFile
 *		13 Dec 2011:	moved Buffer out again, into LIMScommsBase.cpp
 */

#include <vcl.h>
#include "SerialComms.h"
#include "LIMSParams.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------
//	Open the specified serial line; throw exception if unsuccessful
//---------------------------------------------------------------------------

SerialComms::SerialComms() : LIMSCommsBase() {
	PortParams params;
	LIMSParams & settings = LIMSParams::instance();
	if( settings.openSection( "serial", false ) ) {
		params.port = settings.findValue( "port" );
	}
	if( params.port.IsEmpty() ) {
		throw Exception( "Serial parameters not configured" );
	}
	params.start = settings.findValue("start");
	params.baud = settings.findValue("baud");
	params.data = settings.findValue("data");
	params.stop = settings.findValue("stop");
	params.parity = settings.findValue("parity");
	if( !open( params ) ) {
		CloseHandle( IO_Handle );
		throw Exception( "Cannot open " + params.port );
	}
}

//---------------------------------------------------------------------------

SerialComms::SerialComms( const PortParams & settings ) : LIMSCommsBase() {
	if( !open( settings ) ) {
		CloseHandle( IO_Handle );
		throw Exception( "Cannot open " + settings.port );
	}
}

//---------------------------------------------------------------------------

bool SerialComms::open( const PortParams & settings )
{
	bool ready = false;
	LIMSCommsBase::open( settings.port, GENERIC_READ | GENERIC_WRITE, 0 );
	DCB Serial_DCB;
	Serial_DCB.DCBlength = sizeof( DCB );
	String params = "baud=" + settings.baud
				 + " parity=" + settings.parity
				 + " data=" + settings.data
				 + " stop=" + settings.stop;
	if( GetCommState( IO_Handle, &Serial_DCB )
	 && BuildCommDCB( params.c_str(), &Serial_DCB )
	 && SetCommState( IO_Handle, &Serial_DCB ) )
	{
		COMMTIMEOUTS Serial_Time;
		Serial_Time.ReadIntervalTimeout = 0;
		Serial_Time.ReadTotalTimeoutMultiplier = 0;
		Serial_Time.ReadTotalTimeoutConstant = 0;
		Serial_Time.WriteTotalTimeoutMultiplier = 0;
		Serial_Time.WriteTotalTimeoutConstant = 0;
		if( SetCommTimeouts( IO_Handle, &Serial_Time ) )
			ready = true;
	}
	return ready;
}

//---------------------------------------------------------------------------
//  Find out how many characters are still waiting to be read
//---------------------------------------------------------------------------

unsigned SerialComms::available()
{
	DWORD Error_Code;
	COMSTAT oStatus;
	return ClearCommError( IO_Handle, &Error_Code, &oStatus) ? oStatus.cbInQue : 0;
}

//---------------------------------------------------------------------------
//  Read the next few characters from the serial port; return count
//---------------------------------------------------------------------------

unsigned SerialComms::readBytes( unsigned char * buffer, unsigned count )
{
	unsigned numRead = read( buffer, count );
	record( "rx", buffer, numRead );
	return numRead;
}

//---------------------------------------------------------------------------
//  Write a message to the serial port; throw exception on error
//---------------------------------------------------------------------------

unsigned SerialComms::write( const unsigned char * bytes, unsigned count )
{
	DWORD numSent = 0;
	if( count > 0 && !WriteFile( IO_Handle, bytes, count, &numSent, NULL ) )
		throw Exception( "Error writing to " + getSource() );
	else
		return numSent;
}

//---------------------------------------------------------------------------

unsigned SerialComms::sendBytes( const unsigned char * bytes, unsigned count )
{
	unsigned numSent = write( bytes, count );
	record( "tx", bytes, numSent );
	return numSent;
}

//---------------------------------------------------------------------------
//  Clear the input and output buffers and prepare to send again
//---------------------------------------------------------------------------

void SerialComms::clear()
{
	bool ready = false;
	if( PurgeComm( IO_Handle, PURGE_TXABORT + PURGE_RXABORT + PURGE_TXCLEAR + PURGE_RXCLEAR ) )
	{
		DWORD Error_Code;
		COMSTAT oStatus;
		if( ClearCommError( IO_Handle, &Error_Code, &oStatus ) )
			ready = true;
	}

	if( !ready )
		throw Exception( "Error clearing " + getSource() );
}

//---------------------------------------------------------------------------

