//---------------------------------------------------------------------------

#pragma hdrstop

#include <vector>
#include <iostream>

#include "page1.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


void page1::sendHeader( ) const
{
	std::cout << "<p align=\"left\"></p>";
#ifdef RANDLIVE
	std::cout << 	"<form action='attila_rand_live.cgi' method=\'POST\'>\n";
#else
	std::cout << 	"<form action='attila_rand_test.cgi' method=\'GET\'>\n";
#endif
	std::cout << 	"<input type=\'hidden\' name=\'page\' value=\'1\' />\n";
}

//--------------------------------------------------------------------------

void page1::sendQuestions( ) const
{
	bool ShowErrors = showErrors();

	std::cout << "<p align=\"left\"><span><strong><font color=\"#009900\"><u>PART A</u>: IDENTIFYING DETAILS</font></strong></span></p>";
	askSocialServicesPro(ShowErrors);
	askPatientDetils(ShowErrors);
	askOptionalDetails(ShowErrors);
	askRandomiser(ShowErrors);
}

//--------------------------------------------------------------------------

void page1::askLogin(bool ShowErrors) const
{
	std::cout << "<p align=\"left\"><span><strong><font color=\"#009900\"><u>SIGN IN</u></font></strong></span></p>";
	const session &s = getData().getLogin();

	std::cout << "<p><table><tr><td>Sign in name ";
	if (ShowErrors)
		std::cout << s.getSelectUserID().getHTMLErrorWarning();
	std::cout << "</td><td><select name=\"nx\" required > <option value=\"\" selected " << REQUIRED << "> </option>";

	const std::vector< session > & cl = s.getList( );
	for( std::vector< session >::const_iterator ci = cl.begin(); ci != cl.end(); ++ci )
		addOption( ci->getID(), ci->getName(), s.getID() );

	std::cout << "</select>";
	std::cout << "\n</td></tr>\n<tr><td>PIN";
	if (ShowErrors)
		std::cout << s.getSelectUserID().getHTMLErrorWarning();
	std::cout << "</td><td>";

	std::cout << "<input type=\"password\" name=\"up\" size=\"50\" value=\"\" " << REQUIRED << "></td></tr></table>";
	std::cout << "</p><hr>\n";
}

//--------------------------------------------------------------------------



bool page1::showErrors() const
{
	if (getData().getSumbittedPage() != 1)
		return false;

	const clinician & clinician = getData().getClinicians();
	const patient& patient = getData().getPatient();
	const session &session = getData().getLogin();

	if (patient.getnhsnumber().isError ||
		patient.gethospitalno().isError ||
		patient.getgp().isError ||
		patient.getname().isError ||
		patient.getdob().isError ||
		patient.getgender().isError ||
		patient.getrssprofessional().isError ||
		session.getSelectUserID().isError ||
		session.getSelectPin().isError ||
		clinician.getMessage().length() > 0 ||
		patient.getnameenc().isError ||
		patient.getoperatorname().isError ||
		patient.getoperatortelnum().isError ||
		session.getMessage().length() > 0)
	{
		std::cout << "<div class='warn'>";

		if (!session.getMessage().empty())
			std::cout << "<p>" << session.getMessage() << "</p>\n";
		if (!clinician.getMessage().empty())
			std::cout << "<p>"<< clinician.getMessage() << "</p>\n";

		std::cout << session.getSelectUserID().getHTMLError();
		std::cout << session.getSelectPin().getHTMLError();
		std::cout << patient.getrssprofessional().getHTMLError();
		std::cout << patient.getname().getHTMLError();
		std::cout << patient.getnameenc().getHTMLError();
		std::cout << patient.getdob().getHTMLError();
		std::cout << patient.getgender().getHTMLError();
		std::cout << patient.gethospitalno().getHTMLError();
		std::cout << patient.getnhsnumber().getHTMLError();
		std::cout << patient.getgp().getHTMLError();
		std::cout << patient.getoperatorname().getHTMLError();
		std::cout << patient.getoperatortelnum().getHTMLError();

		std::cout << "</div>\n";
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------

void page1::askCentre(bool showErrors) const
{
	std::cout << "<p align=\"left\"><span><strong><font color=\"#009900\"><u>PART A</u>: IDENTIFYING DETAILS</font></strong></span></p>";
	const centre & c = getData().getCentre();

	std::cout << "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" border=\"0\">\n";
	std::cout << "	  <tr bgcolor=\"#EEEEEE\">\n";
	std::cout << "		<td style=\"WIDTH: 330px\">\n";
	std::cout << "<p align=\"left\">Centre</p>";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
	std::cout << "<p align=\"centre\"><select style=\"WIDTH: 200px\" name=\"centre_id\" " << REQUIRED << ">";
	std::cout << "<option> </option>\n";
	const std::vector< centre > & cl = c.getList( );
	for( std::vector< centre >::const_iterator ci = cl.begin(); ci != cl.end(); ++ci )
		addOption( ci->getID(), ci->getName(), c.getID() );
	std::cout << "</select>";
	if (!c.getMessage().empty())
		std::cout << "<span class ='red'>*</span>\n</p>";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";
	std::cout << "  </table>\n";
}

//---------------------------------------------------------------------------

void page1::askConsultant(bool showErrors) const
{
	const clinician & c = getData().getClinicians();
	const session & s = getData().getLogin();

	std::cout << "<p align=\"left\">Principal Investigator (PI) <select style=\"WIDTH: 150px\" name=\"consult_id\" " << REQUIRED << "><option> </option>\n";

	const std::vector< clinician > & cl = c.getList();
	for( std::vector< clinician >::const_iterator ci = cl.begin(); ci != cl.end(); ++ci )
		addOption( ci->selectedClinician.id, ci->selectedClinician.getText(), c.selectedClinician.id );
	std::cout << "</select>";

	std::cout << "\nPIN";
	if (showErrors)
		std::cout << s.getSelectUserID().getHTMLErrorWarning();
	std::cout << "<input type=\"password\" name=\"up\" size=\"8\" value=\"\" " << REQUIRED << ">";

}

//--------------------------------------------------------------------------

void page1::askSocialServicesPro(bool showErrors) const
{                                                                                                                                    //required
	const patient& p = getData().getPatient();

	std::cout << "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" border=\"0\">\n";
	std::cout << "	  <tr>\n";
	std::cout << "		<td style=\"WIDTH: 330px\">\n";
	std::cout << "<p align=\"left\">Responsible Social Services Professional</p>";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
	std::cout << "<p align=\"centre\"><input style=\"HEIGHT: 22px; WIDTH: 200px\" size=\"20\" name=\"rssprofessional\" maxlength=\"60\" " << p.getrssprofessional().getValueText() << "  " << REQUIRED << " >";
	if (showErrors)
		std::cout << p.getrssprofessional().getHTMLErrorWarning();
	std::cout << "</p>\n";

	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";
	std::cout << "  </table>\n";

}

//--------------------------------------------------------------------------
static const char *sex[ ] = { "F", "M", NULL };

void page1::askPatientDetils(bool showErrors) const
{
	const patient& p = getData().getPatient();                                            //required

	std::cout << "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" border=\"0\">\n";
	std::cout << "	  <tr bgcolor=\"#EEEEEE\">\n";
	std::cout << "		<td style=\"WIDTH: 330px\">\n";
		std::cout << "<p align=\"left\">Patient's full name</p>";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
		std::cout << "<p align=\"centre\"><input  style=\"HEIGHT: 22px; WIDTH: 200px\" size=\"22\"  maxlength=\"60\"  name=\"name\" " << p.getname().getValueText() << "  " << REQUIRED << " >";
		if (showErrors)
			std::cout << p.getname().getHTMLErrorWarning();
		std::cout << "</p>";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";

	std::cout << "	  <tr>\n";
	std::cout << "		<td>\n";
		std::cout << "<p align=\"left\">Date of birth</p>";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
		std::cout << "<p align=\"centre\"><input type=\"text\" pattern=\"\\d{1,2}/\\d{1,2}/\\d{4}\" title =\"DD/MM/YYYY\" style=\"HEIGHT: 22px; WIDTH: 200px\" size=\"16\" name=\"dob\"" << p.getdob().getValueText() << "  " << REQUIRED << " placeholder=\"DD/MM/YYYY\" >";
	if (showErrors)
		std::cout << p.getdob().getHTMLErrorWarning();
	std::cout << "</p>";

	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";

	std::cout << "	  <tr bgcolor=\"#EEEEEE\">\n";
	std::cout << "		<td>\n";
		std::cout << "<p align=\"left\">Sex</p>";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
		std::cout << "<p align=\"centre\">";
		std::cout << "<select style=\"WIDTH: 200px\" name=\"gender\"  " << REQUIRED << " >"; //required
		std::cout << "<option>" << "</option>";

		int selected = -1;
		if (p.getgender().getText() == "F")
			selected = 0;
		else if (p.getgender().getText() == "M")
			selected = 1;

		addOption(0,sex[0],selected);
		addOption(1,sex[1],selected);
		std::cout << "</select>";
		if (showErrors)
			std::cout << p.getgender().getHTMLErrorWarning();
		std::cout << "</p>";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";
	std::cout << "  </table>\n";
}

//--------------------------------------------------------------------------

void page1::askOptionalDetails(bool showErrors) const
{
	const patient& p = getData().getPatient();                                            //required

	std::cout << "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" border=\"0\">\n";
	std::cout << "	  <tr>\n";
	std::cout << "		<td style=\"WIDTH: 330px\">\n";
		std::cout << "<p align=\"left\">*Hospital number:</p>";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
	std::cout << "<p align=\"centre\"><input style=\"HEIGHT: 22px; WIDTH: 200px\" size=\"29\"  maxlength=\"40\" name=\"hospitalno\" " << p.gethospitalno().getValueText() << " >";
	if (showErrors)
		std::cout << p.gethospitalno().getHTMLErrorWarning();

		std::cout << "</p>";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";

	std::cout << "	  <tr bgcolor=\"#EEEEEE\">\n";
	std::cout << "		<td>\n";
		std::cout << "<p align=\"left\">*NHS number</p>";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
		std::cout << "<p align=\"centre\"><input name=\"nhsnumber\"  style=\"HEIGHT: 22px; WIDTH: 200px\" placeholder=\"123-456-7890\" pattern=\"^\\d{3}-\\d{3}-\\d{4}\"  size=\"24\" maxlength=\"30\" title=\"123-456-7890\" size=\"24\" " << p.getnhsnumber().getValueText() << " >";
	if (showErrors)
		std::cout << p.getnhsnumber().getHTMLErrorWarning();
	std::cout << "</p>";

	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";

	std::cout << "	  <tr>\n";
	std::cout << "		<td >\n";
		std::cout << "<p align=\"left\">*GP</p>";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
		std::cout << "<p align=\"centre\"> <input name=\"gp\" style=\"HEIGHT: 22px; WIDTH: 200px\" size=\"24\" maxlength=\"60\"  " << p.getgp().getValueText() << "  >";
	if (showErrors)
		std::cout << p.getgp().getHTMLErrorWarning();
		std::cout << "</p>";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";
	std::cout << "  </table>\n";
   	std::cout << "<p align=\"left\">* - If available</p>";
}

//---------------------------------------------------------------------------

void page1::askRandomiser(bool ShowErrors) const
{
	std::cout << "<hr>\n";
//look up the person, put in name, and put in telephone number if there is one
//this can updated by user

	const patient& p = getData().getPatient();
	const clinician &c = getData().getClinicians();

	std::cout << "<p align=\"left\">If you are not <b>" << c.getName()	<< "</b>, please enter your details below:</p>\n";

	std::cout << "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" border=\"0\">\n";
	std::cout << "	  <tr bgcolor=\"#EEEEEE\">\n";
	std::cout << "		<td style=\"WIDTH: 330px\">\n";
		std::cout << "<p align=\"left\">Person randomising</p>";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
	std::cout << "<p align=\"centre\"><input style=\"HEIGHT: 22px; WIDTH: 200px\" size=\"5\"  maxlength=\"60\" " << " name=\"operatorname\" " <<  getData().getPatient().getoperatorname().getValueText() << " >";
	if (ShowErrors)
		std::cout << p.getoperatorname().getHTMLErrorWarning();

		std::cout << "</p>";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";

	std::cout << "	  <tr>\n";
	std::cout << "		<td>\n";
		std::cout << "<p align=\"left\">Telephone</p>";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
		std::cout << "<p align=\"centre\"><input type=\"tel\" style=\"HEIGHT: 22px; WIDTH: 200px\" size=\"5\" maxlength=\"60\" " <<  " name=\"operatortelnum\" " <<  getData().getPatient().getoperatortelnum().getValueText() << " >";
	if (ShowErrors)
		std::cout << p.getoperatortelnum().getHTMLErrorWarning();
	std::cout << "</p>";

	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";
	std::cout << "  </table>\n";
	std::cout << "<hr>";
}
//--------------------------------------------------------------------------

