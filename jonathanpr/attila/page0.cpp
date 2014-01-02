//---------------------------------------------------------------------------

#pragma hdrstop

#include <vector>
#include <iostream>

#include "page0.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void page0::sendHeader( ) const
{
	std::cout << "<p align=\"left\"></p>";
	std::cout << "<p align=\"left\">";
	std::cout << "</p>";
#ifdef RANDLIVE
	std::cout << 	"<form action='attila_rand_live.cgi' method=\'POST\'>\n";
#else
	std::cout << 	"<form action='attila_rand_test.cgi' method=\'GET\'>\n";
#endif
	std::cout << 	"<input type=\'hidden\' name=\'page\' value=\'0\' />\n";
}

//--------------------------------------------------------------------------
bool page0::showErrors() const
{
	if (getData().getSumbittedPage() != 0)
		return false;

	const centre & center = getData().getCentre();
	const session &session = getData().getLogin();
	const clinician & clinician = getData().getClinicians();

	if (session.getSelectUserID().isError ||
		center.getMessage().length() > 0 ||
		session.getSelectPin().isError ||
		clinician.getMessage().length() > 0 ||
		session.getMessage().length() > 0)
	{
		std::cout << "<div class='warn'>";
		if (!center.getMessage().empty())
			std::cout << "<p>" << center.getMessage() << "</p>\n";

		if (!session.getMessage().empty())
			std::cout << "<p>" << session.getMessage() << "</p>\n";

		if (!clinician.getMessage().empty())
			std::cout << "<p>"<< clinician.getMessage() << "</p>\n";

		std::cout << session.getSelectUserID().getHTMLError();
		std::cout << session.getSelectPin().getHTMLError();
		std::cout << "</div>\n";
		return true;
	}
	return false;
}

void page0::askCentre(bool showErrors) const
{
	const centre & c = getData().getCentre();

	std::cout << "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" border=\"0\">\n";
	std::cout << "	  <tr bgcolor=\"#EEEEEE\">\n";
	std::cout << "		<td style=\"WIDTH: 330px\">\n";
	std::cout << "<p align=\"left\">Centre";
	if (showErrors && (!c.getMessage().empty()))
		std::cout << "<span class ='red'>*</span>\n</p>";
	std::cout << "</p>";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
	std::cout << "		<p align=\"centre\">\n";
	std::cout << "<p align=\"left\"><select style=\"WIDTH: 200px\" name=\"centre_id\" " << REQUIRED << ">";
	std::cout << "<option> </option>\n";
	const std::vector< centre > & cl = c.getList( );
	for( std::vector< centre >::const_iterator ci = cl.begin(); ci != cl.end(); ++ci )
		addOption( ci->getID(), ci->getName(), c.getID() );
	std::cout << "</select>";
	std::cout << "		</p>\n";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";
	std::cout << "  </table>\n";
}

//--------------------------------------------------------------------------

void page0::sendQuestions( ) const
{
	bool showE = showErrors();
	const clinician & c = getData().getClinicians();
	const session & s = getData().getLogin();

	std::cout << "<p align=\"left\"><span><strong><font color=\"#009900\"><u>SIGN ON</u></font></strong></span></p>";

	askCentre(showE);

	std::cout << "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" border=\"0\">\n";
	std::cout << "	  <tr>\n";
	std::cout << "		<td style=\"WIDTH: 330px\">\n";
	std::cout << "		  <p align=\"left\">Principal Investigator (PI)\n";
	if (showE && !s.getMessage().empty())
		std::cout << "<span class ='red'>*</span>\n";
	std::cout << "		  </p>\n";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
	std::cout << "		<p align=\"left\">\n";
	std::cout << "      <select style=\"WIDTH: 200px\" name=\"consult_id\" " << REQUIRED << "><option> </option>\n";
	const std::vector< clinician > & cl = c.getList();
	for( std::vector< clinician >::const_iterator ci = cl.begin(); ci != cl.end(); ++ci )
		addOption( ci->selectedClinician.id, ci->selectedClinician.getText(), c.selectedClinician.id );
	std::cout << "</select>";
	std::cout << "		</p>\n";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";

	std::cout << "	  <tr bgcolor=\"#EEEEEE\">\n";
	std::cout << "		<td style=\"WIDTH: 330px\">\n";
	std::cout << "		  <p align=\"left\">PIN\n";
	if (showE && !s.getMessage().empty())
		std::cout << "<span class ='red'>*</span>\n";
	std::cout << "		  </p>\n";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
	std::cout << "		<p align=\"left\">\n";
	std::cout << "		  <input type=\"password\" name=\"up\" size=\"10\" value=\"\" " << REQUIRED << ">";
	std::cout << "		</p>\n";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";
	std::cout << "  </table>\n";

	std::cout << "<p align=\"left\"></p>";
	std::cout << "<p align=\"left\"></p>";
	std::cout << "<hr>";
}






