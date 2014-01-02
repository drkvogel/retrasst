//---------------------------------------------------------------------------

#pragma hdrstop

#include "checklist.h"
#include <iterator>
#include <algorithm>
#include <string>
#include <sstream>
#include <cctype>
#include "xcgi.h"
#include "xencode.h"
#include "patient.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
//	eligibility checklist
// ---------------------------------------------------------------------------

checklist::checklist( ) : incomplete( true )
{

}

void checklist::parse(const XCGI *cgi,const ROSETTA &R)
{
	readDetails(R);
	m_DOD = cgi->getParamDefault( "DOD",m_DOD);
	m_FACS = cgi->getParamDefault( "FACS",m_FACS );
	m_LIOD = cgi->getParamDefault( "LIOD",m_LIOD );
	m_WTLCTH = cgi->getParamDefault( "WTLCTH",m_WTLCTH);
	m_APATT = cgi->getParamDefault( "APATT",m_APATT);
	m_UMORC = cgi->getParamDefault( "UMORC",m_UMORC);
	m_PIACTFADI = cgi->getParamDefault( "PIACTFADI",m_PIACTFADI);

	getDetailsConfirmed(cgi);
}

void checklist::getDetailsConfirmed(const XCGI *cgi )
{
	m_DETAILSCONFIRMED = cgi->getParamDefault("DETAILSCONFIRMED",m_DETAILSCONFIRMED);
	if (m_DETAILSCONFIRMED == "YES")
		return;
	if (m_DETAILSCONFIRMED == "0")
		m_DETAILSCONFIRMED = "YES";
	else
		m_DETAILSCONFIRMED = "";
}

// ---------------------------------------------------------------------------
//	report as incomplete until questions answered, then report outcome
// ---------------------------------------------------------------------------

stage checklist::getStage() const
{
	return incomplete ? INCOMPLETE : ineligible ? INELIGIBLE : VALID;
}

stage checklist::getConfirmationStage() const
{
	if (m_DETAILSCONFIRMED == "YES")
		return VALID;
	return INCOMPLETE;
}

// ---------------------------------------------------------------------------
//	check the user's input for completeness and eligibility
// ---------------------------------------------------------------------------

void checklist::checkDetails( const patient &person )
{
	// 0 yes 1 no
	const std::string YES = "0";
	const std::string NO = "1";

	ineligible = incomplete = true;

	if (m_FACS.empty() || m_LIOD.empty() || m_WTLCTH.empty() || m_APATT.empty() || m_UMORC.empty() || m_PIACTFADI.empty())
		return;

	if (m_FACS != YES && m_FACS != NO)
		return;
	if (m_LIOD != YES && m_LIOD != NO)
		return;
	if (m_WTLCTH != YES && m_WTLCTH != NO)
		return;
	if (m_APATT != YES && m_APATT != NO)
		return;
	if (m_UMORC != YES && m_UMORC != NO)
		return;
	if (m_PIACTFADI != YES && m_PIACTFADI != NO)
		return;

	incomplete = false;
	m_error = "";
	//stay as ineligible if any of these are set this way
	if (m_FACS == NO || m_LIOD == NO || m_WTLCTH == NO || m_APATT == YES || m_UMORC == YES || m_PIACTFADI == YES)
	{
		if (m_FACS == NO)
			m_error = "Is not eligible to Fair Access to Care Services.";
		else if (m_LIOD == NO)
			m_error = "Is not living in an ordinary dwelling.";
		else if (m_WTLCTH == NO)
			m_error = "No working telephone line connrected to the house.";
		else if (m_APATT == YES)
			m_error = "Has already provided ATT intervention for dementia.";
		else if (m_UMORC == YES)
			m_error = "Unstable medical or psychiatric condition.";
		else if (m_PIACTFADI == YES)
			m_error = "Participating in another clinical trial for a dementia intervention.";
		return;
	}
//We have a pass.
   ineligible = false;
}


void checklist::readDetails(const ROSETTA & fields )
{
	m_DOD = fields.getStringDefault("DOD" ,"");
 	m_FACS = fields.getStringDefault("FACS" ,"");
	m_LIOD = fields.getStringDefault("LIOD" ,"");
	m_WTLCTH = fields.getStringDefault("WTLCTH" ,"");
	m_APATT = fields.getStringDefault("APATT" ,"");
	m_UMORC = fields.getStringDefault("UMORC" ,"");
	m_PIACTFADI = fields.getStringDefault("PIACTFADI" ,"");
	m_DETAILSCONFIRMED = fields.getStringDefault("DETAILSCONFIRMED","");
}

// ---------------------------------------------------------------------------
//	save the fields to the ROSETTA
// ---------------------------------------------------------------------------

void checklist::addToRosetta(ROSETTA & fields,const std::string &name,const std::string &var) const
{
	if (!var.empty())
		fields.setString(name,var);
}

// ---------------------------------------------------------------------------

void checklist::addFields( ROSETTA & fields ) const
{
	addToRosetta(fields, "DOD", m_DOD );
	addToRosetta(fields, "FACS", m_FACS );
	addToRosetta(fields, "LIOD", m_LIOD );
	addToRosetta(fields, "WTLCTH", m_WTLCTH);
	addToRosetta(fields, "APATT", m_APATT);
	addToRosetta(fields, "UMORC", m_UMORC );
	addToRosetta(fields, "PIACTFADI", m_PIACTFADI);
	addToRosetta(fields, "DETAILSCONFIRMED", m_DETAILSCONFIRMED);
}

// ---------------------------------------------------------------------------

void checklist::addForDBFields( ROSETTA & fields ) const
{
	int idialog = atoi(m_DOD.c_str());
	fields.setInt("diagnosis", idialog);
}
