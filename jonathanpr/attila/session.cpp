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

//---------------------------------------------------------------------------
//	SESSION AND CLINICIAN NOW OVERLAP FAR TOO MUCH AS THE PROGRAM DEVELOPED
//  LOOK AT COMBINING THEM TOGETHER.
//---------------------------------------------------------------------------
std::vector<session > session::m_staffnames;

session::session() : m_stage(UNKNOWN), m_id(-1)
{

}
//---------------------------------------------------------------------------

void session::parse(const XCGI *cgi,const ROSETTA &R)
{
	readDetails(R);
//get any updates
	m_user_id.setText(cgi->setParamIfExistsElseDefault("consult_id",m_user_id.getText()));
	m_pin.setText(cgi->setParamIfExistsElseDefault("up",m_pin.getText()));

	m_varifiy_user_id.setText(cgi->setParamIfExistsElseDefault("vnx",m_varifiy_user_id.getText()));
	m_varifiy_pin.setText(cgi->setParamIfExistsElseDefault("vup",m_varifiy_pin.getText()));
}

//---------------------------------------------------------------------------
session::session( int recID, const std::string &name, const std::string &phone)
{
	m_id = recID;
	m_name = name;
	m_phone = phone;
}

//---------------------------------------------------------------------------

const session *session::find( int recID ) const
{
	for( std::vector<session >::const_iterator ci = m_staffnames.begin( ); ci != m_staffnames.end( ) ; ++ ci )
	{
		if( ci->m_id == recID )
			return &(*ci);
	}
	return NULL;
}

//---------------------------------------------------------------------------

void session::readList( XDB *db )
{
	m_staffnames.clear( );
	XQUERY qp( db, "SELECT staff_id,name,phone FROM staff WHERE pin <> 0 AND status = 0 AND role LIKE '%PI%' ORDER BY name");
//	XQUERY qp( db, "SELECT staff_id,name,phone FROM staff WHERE pin <> 0 ORDER BY name" );
	if( qp.open( ) )
	{
		while( qp.fetch( ) )
		{
			int id = qp.result.getInt( "staff_id" );
			std::string name = qp.result.getString( "name" );
			std::string phone = qp.result.getString( "phone" );
			m_staffnames.push_back(session( id, name,phone ) );
		}
		qp.close( );
	}
	if( m_staffnames.empty( ) )
		m_error = "Cannot read staff table";
}

//---------------------------------------------------------------------------
//	check the user's credentials
//---------------------------------------------------------------------------

void session::checkUser( RAND_UTIL *ru,XDB *db )
{
	m_error = "";
	m_stage = INCOMPLETE;

	if ( m_user_id.getText().empty() )
		m_user_id.setMessage("Please select you sign in");

	if ( m_pin.getText().empty() )
		m_pin.setMessage("Enter PIN to check authentication");

	if (m_user_id.isError || m_pin.isError)
		return;

	m_stage = INELIGIBLE;
//UNKNOWN, INCOMPLETE, INELIGIBLE, VALID

// ATTEMPT PIN MATCH
	XQUERY qp( db, "SELECT pin FROM staff WHERE staff_id  = :i" );
	int ipin = m_user_id.igetText();
	qp.param.setInt( "i", ipin);
	const int pdb = qp.fetchInt( -1 );
	if ( pdb == m_pin.igetText() )
	{
		m_stage = VALID;
		return;
	}
	else
		m_error = "User+PIN match failed";

	// PIN FAILED, TRY FOR STAFF USER
	m_error = ru->setStaffUser( m_pin.getText() );

	if ( !m_error.empty() )              // STAFF USER SUCCEEDED
	{
		m_error = "User+PIN match failed";
		return;
	}
	m_ctsu_signon = ru->getStaffUsername();
	m_stage = VALID;
}
//------------------------------------------------------------------------
void session::checkUserValidation( RAND_UTIL *ru,XDB *db )
{//NOW CHECK THE VARIFICATION
	m_vstage = INCOMPLETE;

	if ( m_varifiy_user_id.getText().empty() )
		m_varifiy_user_id.setMessage("Please select you sign in");

	if ( m_varifiy_pin.getText().empty() )
		m_varifiy_pin.setMessage("Enter PIN to check authentication");

	if (m_varifiy_user_id.isError || m_varifiy_pin.isError)
		return;

	m_vstage = INELIGIBLE;
//UNKNOWN, INCOMPLETE, INELIGIBLE, VALID

// ATTEMPT PIN MATCH
	XQUERY qp( db, "SELECT pin FROM staff WHERE staff_id  = :i" );
	int ipin = m_varifiy_user_id.igetText();
	qp.param.setInt( "i", ipin);
	const int pdb = qp.fetchInt( -1 );
	if ( pdb == m_varifiy_pin.igetText() )
	{
		m_vstage = VALID;
		return;
	}
	else
		m_error = "User+PIN match failed";

	// PIN FAILED, TRY FOR STAFF USER
	m_error = ru->setStaffUser( m_varifiy_pin.getText() );

	if (!m_error.empty() )              // STAFF USER SUCCEEDED
	{
		m_error = "User+PIN match failed";
		return;
	}
	m_ctsu_signon = ru->getStaffUsername();
	m_vstage = VALID;
}

void session::readDetails(const ROSETTA & fields )
{
//THIS SHOULD ONLY BE INCORRECT INFORMATION, OTHER WISE IS BLANK
	m_user_id.setText(fields.getStringDefault("consult_id" ,m_user_id.getText()));
	m_ctsu_signon = fields.getStringDefault("m_ctsu_signon" ,m_ctsu_signon);
	std::string stage = fields.getStringDefault("s","");
	if (stage == "3")
		m_stage = VALID;
}

//---------------------------------------------------------------------------
//	add fields from this object to save in the person_reg table
// ---------------------------------------------------------------------------

void session::addFields( ROSETTA & fields ) const
{
//	fields.setString( "nx", m_user_id.getText() );
	if (m_stage == VALID)
		fields.setString( "s", "3" ); //3 is VALID
	if (!m_ctsu_signon.empty())
		fields.setString("ctsu_signon",m_ctsu_signon);
}

//---------------------------------------------------------------------------
//	add fields from this object to save in the person_reg table
// ---------------------------------------------------------------------------

void session::addForDBFields( ROSETTA & fields ) const
{
	fields.setInt("staffpi_id", m_varifiy_user_id.igetText());
	if (!m_ctsu_signon.empty())
		fields.setString("ctsu_signon", m_ctsu_signon);
}




