#include "IsysDebugComms.h"

bool IsysDebugComms::useDebugComms = false;
bool IsysDebugComms::disableReply = false;

IsysDebugComms::IsysDebugComms()
	: frm( new TdebugComms(NULL) )
{
	setSource("IsysDebugComms");
    frm->Show();
}

unsigned IsysDebugComms::available()
{
    return frm->available();
}

unsigned IsysDebugComms::readBytes( unsigned char * buffer, unsigned count )
{
	//unsigned numRead = read( buffer, count );
	if( count > 0 )
		record( "rx", buffer, count );

    return frm->readBytes( buffer, count );
}

unsigned IsysDebugComms::sendBytes( const unsigned char * bytes, unsigned count )
{
	//unsigned numSent = write( bytes, count );
//	if( numSent > 0 )
//		record( "tx", bytes, numSent );
	if( count > 0 )
		record( "tx", bytes, count );
    return frm->sendBytes( bytes, count );
}

void IsysDebugComms::printMessage(String msg) {
    frm->printMessage(msg);
}

void IsysDebugComms::addMessage(String msg) {
    frm->addMessage(msg);
}

void IsysDebugComms::clear()
{
}

