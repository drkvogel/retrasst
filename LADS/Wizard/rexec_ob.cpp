//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "rexec_ob.h"
/*---------------------------------------------------------------------------*/

REXEC_OB::REXEC_OB( TClientSocket *socket )
	:
	soc( socket ),                  // SOCKET FOR CONNECTION
	alen(0), ulen(0), plen(0),	// LENGTH OF FIELDS
	port( 512 )			// DEFAULT REXEC PORT FOR AN ALPHA
{
	soc->Active = false;
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
REXEC_OB::~REXEC_OB()
{
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

bool REXEC_OB::set_address(  String address )
{
	soc->Active = false;
	addr = address;
	alen = addr.Length();
	return( alen > 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
bool REXEC_OB::set_port( int rexec_port_number )
{
	soc->Active = false;
	port = rexec_port_number;
	return( port > 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

bool REXEC_OB::set_username( String username )
{
	soc->Active = false;
	unam = username;
	ulen = unam.Length();
	return( ulen > 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

bool REXEC_OB::set_password( String passwd )
{
	soc->Active = false;
	pswd = passwd;
	plen = pswd.Length();
	return( plen > 0 );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

bool REXEC_OB::execute( String command )
{
	soc->Active = false;
	soc->ClientType = ctNonBlocking;
	soc->Port = port;
	soc->Host = addr;
	incoming = "";

	String	NullStr = "x";	// REXEC SERVER WANTS NULL DE-LIMITED STRING
	NullStr[1] = '\0';
	outgoing = "000" + NullStr + unam + NullStr + pswd + NullStr + command + NullStr;
	soc->Active = true;
	return( soc->Active );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/


bool REXEC_OB::OnConnect_handler( TCustomWinSocket *tcws )
{
	tcws->SendText( outgoing );
	return( true );
}
/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/

String REXEC_OB::OnRead_handler( TCustomWinSocket *tcws )
{
	incoming = tcws->ReceiveText();
	return( incoming );
}
/*---------------------------------------------------------------------------*/

#pragma package(smart_init)

