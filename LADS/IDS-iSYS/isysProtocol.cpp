//---------------------------------------------------------------------------
//	Data link layer handler for IDS-iSys comms: ack/nak and data frames
//---------------------------------------------------------------------------

#include <vcl.h>
#include "SerialComms.h"
#include "isysMessage.h"
#include "isysProtocol.h"
// #include "isysDebugComms.h"

#pragma hdrstop
#pragma package(smart_init)

//---------------------------------------------------------------------------

void IsysProtocol::reset()
{
	txQueue.clear();
	rxBuffer.clear();
	protocolState = NEUTRAL;
}

//---------------------------------------------------------------------------
//	queue a message to be sent (don't reject it - caller may not wait)
//---------------------------------------------------------------------------

bool IsysProtocol::queue(const CommsMessage & message)
{
	txQueue.push_back(message.toString());
	return true;
}

//---------------------------------------------------------------------------
//	handle received data, depending on current state; return amount used
//---------------------------------------------------------------------------

int IsysProtocol::accept(const unsigned char * data, unsigned length)
{
	if (length == 0)
		return 0;

	switch(protocolState)
	{
		case NEUTRAL:
			startListening(data[0]);
			return 1;

		case LISTENING:
			return checkFrame(data, length);

		case RECEIVING:
			return readFrame(data, length);

		case WAITING:
			checkReadyToSend(data[0]);
			return 1;

		case SENDING:
			checkMessageSent(data[0]);
			return 1;

		default:
			throw Exception("invalid state; clearing");
	}
}

//---------------------------------------------------------------------------
//	send next frame/acknowledgement to analyser and/or prepare line bid
//---------------------------------------------------------------------------

String IsysProtocol::respond()
{
	String reply;
	if (!response.IsEmpty()) {
		reply = response;
		response = "";
	} else if (protocolState == NEUTRAL && !txQueue.empty() && Now() > sleepTime) {
		reply = SerialComms::ENQ;
		protocolState = WAITING;
	}
	return reply;
}

//---------------------------------------------------------------------------
//	return true once the next complete message has been received
//---------------------------------------------------------------------------

bool IsysProtocol::messageReady() const
{
	//return endFrame && !rxBuffer.empty();

    bool notEmpty; // for debug

    notEmpty = !rxBuffer.empty();

    return endFrame && notEmpty;
}

//---------------------------------------------------------------------------
//	pass the next message to the main program; remove from RX buffer
//---------------------------------------------------------------------------

CommsMessage * IsysProtocol::getMessage()
{
	IsysMessage * next = NULL;
	if( messageReady() )
	{
		next = new IsysMessage(rxBuffer.getContents());
		rxBuffer.clear();
	}
	return next;
}

//---------------------------------------------------------------------------
//	analyser should send ENQ before any data; ignore anything else
//---------------------------------------------------------------------------

void IsysProtocol::startListening( unsigned char byte )
{
	if( byte == SerialComms::ENQ )
	{
		rxBuffer.clear();
		response = SerialComms::ACK;
		rxFrame = '1';
		protocolState = LISTENING;
	}
}

//---------------------------------------------------------------------------
//	we only start sending data if the analyser accepts our line bid
//---------------------------------------------------------------------------

void IsysProtocol::checkReadyToSend( unsigned char byte )
{
	switch( byte )
	{
		case SerialComms::ACK:        	   		// the line is ours
			txFrame = '1';
			createFrame( txQueue.front() );		// send first message
			protocolState = SENDING;
			break;

		case SerialComms::ENQ:         			// mid-air collision
			response = "";
			sleepTime = Now() + TTime( 0, 0, 20, 0 );	// wait 20s
			protocolState = NEUTRAL;
			break;

		default:                                // unexpected response
			protocolState = NEUTRAL;					// try again later
	}
}

//---------------------------------------------------------------------------
//	analyser should acknowledge data has been sent; resend if necessary
//---------------------------------------------------------------------------

void IsysProtocol::checkMessageSent( unsigned char byte )
{
	switch( byte )
	{
		case SerialComms::ACK:        				// current frame sent OK
			if( getNextFrame() )
				createFrame( txQueue.front() );     // send the next one
			else
			{	protocolState = NEUTRAL;
				response = SerialComms::EOT;        // free line when done
			}
			break;

		case SerialComms::EOT:         				// current frame sent OK
			getNextFrame();
			protocolState = NEUTRAL;           				// but analyser wants line
			response = "";
			break;

		default:
			createFrame( txQueue.front() );    		// re-send if not acknowledged
	}
}

//---------------------------------------------------------------------------
//	Discard current frame once it has been acknowledged
//---------------------------------------------------------------------------

bool IsysProtocol::getNextFrame()
{
	txQueue.pop_front();
	if( txFrame == '7' )
		txFrame = '0';
	else
		txFrame ++;
	return !txQueue.empty();
}

//---------------------------------------------------------------------------
//	Frame should start with STX and frame number; EOT drops the line
//---------------------------------------------------------------------------

int IsysProtocol::checkFrame( const unsigned char * data, unsigned length )
{
	if (data[0] == SerialComms::EOT)
		protocolState = NEUTRAL;            		// allow buddy to claim line

	if (data[0] != SerialComms::STX)
		return 1;                   		// ignore line noise etc.

	if (length < 2)
		return 0;							// wait for the frame number

#ifndef _DEBUG
	if (data[1] != rxFrame)
		throw Exception(String("frame ") + rxFrame + " expected");
#endif

	protocolState = RECEIVING;
	return 2;		                   		// ready for transmitted data
}

//---------------------------------------------------------------------------
//	Frame ends with ETX/ETB, checksum, CR + LF; store if it's OK
//---------------------------------------------------------------------------

int IsysProtocol::readFrame( const unsigned char * data, unsigned length )
{
	unsigned checked = 0;
	bool intermediate = endFrame = false;
	unsigned checkSum = rxFrame;

	while( checked < length && !intermediate && !endFrame )
	{
		unsigned char next = data[ checked ++ ];
		if( next == SerialComms::ETB )
			intermediate = true;
		if( next == SerialComms::ETX )
			endFrame = true;
		checkSum = (checkSum + next) & 0xFF;
	}
/*
if (IsysDebugComms::useDebugComms) {
	if( length < checked )
		return 0;
	checked += 4;
} else {
*/
	if( length < checked + 4 )
		return 0;                 			// wait for tx checksum

	unsigned e1 = fromHex( data[ checked ++ ] );
	unsigned e2 = fromHex( data[ checked ++ ] );

	if( checkSum != (e1 << 4) + e2 )
	{
		String error;
		error.printf( "invalid checksum %02x for frame %c", checkSum, rxFrame );
		throw Exception( error );
	}

	rxBuffer.add( data, checked - 3 );      // strip off ETX and checksum
	if( data[ checked ] == '\r' )
		checked ++;
	if( data[ checked ] == '\n' )
		checked ++;                    		// usually one record per line

	protocolState = LISTENING;
	response = SerialComms::ACK;
	if( rxFrame == '7' )
		rxFrame = '0';
	else
		rxFrame ++;
	return checked;                         // discard copied frame
}

//---------------------------------------------------------------------------
//	convert a single byte from hexadecimal, if valid, otherwise -1
//---------------------------------------------------------------------------

unsigned IsysProtocol::fromHex( unsigned char byte )
{
	if( byte >= '0' && byte <='9' )
		return byte - '0';

	if( byte >= 'A' && byte <='F' )
		return byte - 'A' + 10;

	if( byte >= 'a' && byte <='f' )
		return byte - 'a' + 10;

	return -1;
}

//---------------------------------------------------------------------------
//	prepare to send the next message from the queue, if there is one
//---------------------------------------------------------------------------

void IsysProtocol::createFrame( const String & message )
{
	char * frame = (char *) malloc( message.Length() + 8 );
	unsigned index = 0;

	// STX, frame number, message, ETX
	frame[ index ++ ] = SerialComms::STX;
	frame[ index ++ ] = txFrame;

	for( char * ch = message.c_str(); *ch != '\0'; ch ++ )
		frame[ index ++ ] = *ch;

	frame[ index ++ ] = SerialComms::ETX;

	// finish with hexadecimal checksum (ignoring STX)
	unsigned char checkSum = 0;
	for( unsigned i = 1; i < index; i ++ )
		checkSum += frame[ i ];

	static const char * hexdigits = "0123456789ABCDEF";
	frame[ index ++ ] = hexdigits[ checkSum >> 4 ];
	frame[ index ++ ] = hexdigits[ checkSum & 0xF ];

	frame[ index ++ ] = '\r';
	frame[ index ++ ] = '\n';
	response = String( frame, index );
	free( frame );
}

//---------------------------------------------------------------------------

