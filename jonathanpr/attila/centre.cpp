//---------------------------------------------------------------------------

#pragma hdrstop
#include <cstdlib>
#include <cctype>
#include <sstream>
#include <iostream>
#include "xquery.h"
#include "xexec.h"
#include "xcgi.h"
#include "centre.h"
#include "xencode.h"

#include <assert.h>
//---------------------------------------------------------------------------
#pragma package(smart_init)

std::vector<centre > centre::centres;

// ---------------------------------------------------------------------------
//	keep centre details from the database - always valid
// ---------------------------------------------------------------------------
centre::centre( int recID, const std::string & nam,const std::string & nhstrust,const std::string & accocuniv,int pinvestigatorid)
{
	selectedCentre.id = recID;
	selectedCentre.text = nam;

	m_nhstrust = nhstrust;
	m_accocuniv = accocuniv;
	m_pinvestigator_id = pinvestigatorid;
}

// ---------------------------------------------------------------------------
//	accept centre number from Web page
// ---------------------------------------------------------------------------
centre::centre()
{
}
// ---------------------------------------------------------------------------

void centre::parse(const XCGI *cgi,const ROSETTA &R)
{
	readDetails(R);
	selectedCentre.id = cgi->getParamAsIntDefault( "centre_id", selectedCentre.id );
}

// ---------------------------------------------------------------------------
//	centre is valid if there's a name to match the ID
// ---------------------------------------------------------------------------

stage centre::getStage() const
{
	return selectedCentre.id == -1 ? INCOMPLETE : selectedCentre.getText().empty() ? INELIGIBLE : VALID;
}

// ---------------------------------------------------------------------------
//	check centre number is for a currently valid centre
// ---------------------------------------------------------------------------

void centre::checkDetails()
{
	const centre *selected = find( selectedCentre.id );
	if( selected == NULL )
	{
		selectedCentre.id = -1;
		m_error = "Unknown centre";
	}
	else
		selectedCentre.text = selected->selectedCentre.getText();
}

// ---------------------------------------------------------------------------
//	find active centres
// ---------------------------------------------------------------------------

void centre::readList( XDB *db )
{
	centres.clear( );
	XQUERY qp( db, "SELECT * FROM centre WHERE status = 0  ORDER BY name" );
	if( qp.open( ) )
	{
		while( qp.fetch( ) )
		{

			int id = qp.result.getInt( "centre_id" );

			std::stringstream ss;
			ss /*<< id << ":"*/ << qp.result.getString( "name" );
			std::string name = ss.str();
			std::string nhstrust = qp.result.getString( "nhstrust" );
			std::string ossocuniv = qp.result.getString( "assocuniv" );
			int pinvestigatorid = qp.result.getIntDefault("pinvestigator_id",-1 );
			centres.push_back(centre( id, name, nhstrust,ossocuniv,pinvestigatorid ) );
		}
		qp.close( );
	}
	if( centres.empty( ) )
		m_error = "Cannot read centres table";
}

void centre::readDetails(const ROSETTA & fields )
{
	selectedCentre.id = fields.getIntDefault("centre_id" ,selectedCentre.id);
}

// ---------------------------------------------------------------------------
//	find the centre with the given ID; return NULL if not found
// ---------------------------------------------------------------------------

const centre *centre::find( int recID ) const
{
	for( std::vector< centre >::const_iterator ci = centres.begin( ); ci != centres.end( ) ; ++ ci )
	{
		if( ci->getID( ) == recID )
			return &( *ci );
	}
	return NULL;
}

// ---------------------------------------------------------------------------
//	add fields from this object
// ---------------------------------------------------------------------------

void centre::addToRosetta(ROSETTA & fields,const std::string &name,int var) const
{
	if (var != -1)
		fields.setInt(name,var);
}
// ---------------------------------------------------------------------------
void centre::addFields( ROSETTA & fields ) const
{
	addToRosetta(fields, "centre_id",selectedCentre.id);
}

// ---------------------------------------------------------------------------
void centre::addForDBFields( ROSETTA & fields ) const
{
	addFields(fields);
}
// ---------------------------------------------------------------------------



