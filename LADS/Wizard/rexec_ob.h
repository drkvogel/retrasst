//---------------------------------------------------------------------------
#ifndef rexec_obH
#define rexec_obH
#include <vcl.h>
#include <ScktComp.hpp>
//---------------------------------------------------------------------------
/* THIS OBJECT NEEDS TO USE VCL COMPONENTS AND LINK INTO MESSAGE QUEUE, THUS:
	(1) ADD A TClientSocket COMPONENT (X) TO A FORM
	(2) CREATE REXEC_OB OBJECT WITH PARAMETER X
	(3) CALL OnConnect_handler IN OnConnect EVENT OF X, PASS SOCKET POINTER
	(4) CALL OnRead_handler IN OnRead EVENT OF X, PASS SOCKET POINTER
*/
//---------------------------------------------------------------------------
class REXEC_OB
{
protected:
	int	status;
	TClientSocket	*soc;
	int	port, alen, ulen, plen;
	String	addr;
	String	unam;
	String	pswd;
	String	outgoing;
	String	incoming;
public:
	REXEC_OB( TClientSocket *socket );
	~REXEC_OB();
	bool 	set_port( int rexec_port_number );
	bool	set_address(  String address );
	bool	set_username( String username );
	bool	set_password( String passwd );
	bool	execute( String command );
	bool	OnConnect_handler( TCustomWinSocket *tcws );
	String	OnRead_handler( TCustomWinSocket *tcws );
};
//---------------------------------------------------------------------------
#endif

