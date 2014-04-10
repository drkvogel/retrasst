//---------------------------------------------------------------------------
#pragma hdrstop

#include "session.h"
#include "xcgi.h"
#include "xencode.h"
#include "randutil.h"
#include "xquery.h"
#include "xexec.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


std::vector<session > session::m_staffnames;

session::session() : m_stage(UNKNOWN)
{

}
//---------------------------------------------------------------------------
//Parse the url information
void session::parse(const XCGI *cgi,const ROSETTA &R)
{
//Parse the Rosetta, get the old values back out
	readDetails(R);
//Has any tabs been pressed? If so change page
	if (cgi->setParamIfExistsElseDefault("ChangeID","") == "Correct Participant ID")
		m_viewpage = "3";
	else if (cgi->setParamIfExistsElseDefault("Logs","") == "Logs")
		m_viewpage = "2";
	else if (cgi->setParamIfExistsElseDefault("Status","") == "Status")
		m_viewpage = "1";
//get the current username password of the user we are going to try and sign in
	m_username.setText(cgi->setParamIfExistsElseDefault("username",m_username.getText()));
	m_password.setText(cgi->setParamIfExistsElseDefault("password",m_password.getText()));
}
//---------------------------------------------------------------------------

session::session( const std::string &name)
{
	m_name = name;
}
//---------------------------------------------------------------------------
//pass in a staff name and get back a ptr if that name exsists
const session *session::find( std::string name ) const
{
	for( std::vector<session >::const_iterator ci = m_staffnames.begin( ); ci != m_staffnames.end( ) ; ++ ci )
	{
		if( ci->m_name == name )
			return &(*ci);
	}
	return NULL;
}
//---------------------------------------------------------------------------
//load in the names of allowed staff which can sign in to the web site
void session::readList(const XDB *db )
{
	m_staffnames.clear( );
	XQUERY qp((XDB *) db, "SELECT name FROM staff WHERE status = 1 ORDER BY name");
	if( qp.open( ) )
	{
		while( qp.fetch( ) )
		{
			std::string name = qp.result.getString( "name" );
			m_staffnames.push_back(session(name) );
		}
		qp.close( );
	}
	if( m_staffnames.empty( ) )
		m_error = "Cannot read staff table";
}

//---------------------------------------------------------------------------
//Perform sign in, uses the ICE sign in from Rand utils (Alans library)
void session::checkUser(RAND_UTIL *ru,const XDB *db )
{
	m_error = "";
	m_stage = INCOMPLETE;

	std::string username = m_username.getText();
	std::string password = m_password.getText();

	if ( username.empty() )
		m_username.setMessage("Please enter your username");
	if ( password.empty() )
		m_password.setMessage("Please enter your password");
	if (m_username.isError || m_password.isError)
		return;

	m_stage = INELIGIBLE;

	if (find(username) != NULL)
		m_error = ru->setStaffUser(username+"&"+password);
	else
		m_error = "username+password not found";

	if (!m_error.empty())              // STAFF USER SUCCEEDED
		return;

	m_ctsu_signon = ru->getStaffUsername();
	if (ru->createToken(m_ctsu_signon) == -9)
	{
		m_error = "error creating security token";
		return;
	}
	m_stage = VALID;
}
//---------------------------------------------------------------------------
//read the details from the encrypted rosetta
void session::readDetails(const ROSETTA & fields )
{
	m_username.setText(fields.getStringDefault("username" ,m_username.getText()));
	m_ctsu_signon = fields.getStringDefault("m_ctsu_signon" ,m_ctsu_signon);

	std::string stage = fields.getStringDefault("s","");
	if (stage == "3")
		m_stage = VALID;
}
//---------------------------------------------------------------------------

void session::addFields( ROSETTA & fields ) const
{
// Save the session info into the rosetta, rosetta is encrpyted
	if (m_stage == VALID)
		fields.setString( "s", "3" ); //3 is VALID
	if (!m_ctsu_signon.empty())
		fields.setString("ctsu_signon",m_ctsu_signon);
}
//---------------------------------------------------------------------------




