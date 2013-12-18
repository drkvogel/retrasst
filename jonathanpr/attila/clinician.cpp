//---------------------------------------------------------------------------

#pragma hdrstop

#include "clinician.h"
#include <cstdlib>
#include <cctype>
#include <sstream>
#include <iostream>

#include "xquery.h"
#include "xcgi.h"
#include "xexec.h"
#include "xencode.h"
#include "centre.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

std::vector< clinician > clinician::workers;

//---------------------------------------------------------------------------
//	SESSION AND CLINICIAN NOW OVERLAP FAR TOO MUCH AS THE PROGRAM DEVELOPED
//  LOOK AT COMBINING THEM TOGETHER.
//---------------------------------------------------------------------------


// ---------------------------------------------------------------------------
//	accept consultant details from a database record - always valid
// ---------------------------------------------------------------------------

clinician::clinician(int worker_id, const std::string &name, const std::string &email, const std::string &phone )
{
	selectedClinician.id = worker_id;
	selectedClinician.text = name;

	m_email = email;
	m_phone = phone;
}

// ---------------------------------------------------------------------------
//	accept worker ID from Web page - check for name later
// ---------------------------------------------------------------------------

clinician::clinician()
{
}

void clinician::parse(const XCGI *cgi,const ROSETTA &R)
{
	readDetails(R);
	selectedClinician.id = cgi->getParamAsIntDefault( "consult_id", selectedClinician.id );
}

// ---------------------------------------------------------------------------
//	check database for selected consultant - flag error if not found
// ---------------------------------------------------------------------------

void clinician::checkDetails()
{
	const clinician *selected = find( selectedClinician.id );
	if( selected == NULL )
	{
		selectedClinician.id = -1;
		m_error = "Unknown consultant";
	}
	else
	{
		selectedClinician.text = selected->selectedClinician.getText();
		selectedphone = selected->m_phone;
		selectedemail = selected->m_email;
	}
}

// ---------------------------------------------------------------------------
//	clinician ID is valid once it has been found in the database
// ---------------------------------------------------------------------------

stage clinician::getStage( ) const
{
	return selectedClinician.id == -1 ? INCOMPLETE : m_error.empty() ? VALID : INELIGIBLE;
}

// ---------------------------------------------------------------------------
//	list active consultants - user can select one from a list
// ---------------------------------------------------------------------------

void clinician::readList( XDB *db )
{
	workers.clear( );
//	XQUERY qp( db, "SELECT * FROM staff WHERE status = 0 AND role LIKE '%PI%' ");
	XQUERY qp( db, "SELECT staff_id,name,email,phone  FROM staff WHERE pin <> 0 AND status = 0 AND role LIKE '%PI%' ORDER BY name");
	if( qp.open( ) )
	{
		while( qp.fetch( ) )
		{
			int id = qp.result.getInt( "staff_id" );
			std::string name = qp.result.getString( "name" );
			std::string email = qp.result.getString( "email" );
			std::string phone = qp.result.getString( "phone" );
			workers.push_back(clinician( id, name, email, phone ) );
		}
		qp.close( );
	}
	if( workers.empty( ) )
		m_error = "Cannot read consultants table";
}

// ---------------------------------------------------------------------------
//	find the given workers in the list; return NULL if not found
// ---------------------------------------------------------------------------

const clinician *clinician::find( int recID ) const
{
	for( std::vector<clinician >::const_iterator ci = workers.begin( ); ci != workers.end( ) ; ++ ci )
	{
		if( ci->selectedClinician.id == recID )
			return &( *ci );
	}
	return NULL;
}


void clinician::readDetails(const ROSETTA & fields )
{
	selectedClinician.id = fields.getIntDefault("consult_id" ,selectedClinician.id);
}

// ---------------------------------------------------------------------------
//	check this consultant works at the given centre
// ---------------------------------------------------------------------------

void clinician::checkLink( const centre &location, XDB *db )
{
	char query[ 160 ];
	std::sprintf( query,"SELECT count(*) from centre WHERE status = 0 AND pinvestigator_id = %d AND centre_id = %d",selectedClinician.id, location.getID( ) );
	XQUERY qp( db, query );
	if( !qp.open( ) || !qp.fetch( ) )
		m_error = "Cannot read cc_link table";
	else if( qp.result.getInt( 0 ) == 0 )
		m_error = selectedClinician.text + " does not work at " + location.getName();
}

// ---------------------------------------------------------------------------
//	add fields from this object to save in the person_reg table
// ---------------------------------------------------------------------------

void clinician::addToRosetta(ROSETTA & fields,const std::string &name,int var) const
{
	if (var != -1)
		fields.setInt(name,var);
}

void clinician::addFields( ROSETTA & fields ) const
{
	addToRosetta(fields, "consult_id",selectedClinician.id);
}

// ---------------------------------------------------------------------------

void clinician::addForDBFields( ROSETTA & fields ) const
{
//none, kept in session
}
// ---------------------------------------------------------------------------

