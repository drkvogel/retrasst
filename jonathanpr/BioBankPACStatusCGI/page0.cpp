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
	std::cout << 	"<form action='biocore_pacsstatus_test.cgi' method=\'POST\'>\n";
#else
	std::cout << 	"<form action='biocore_pacsstatus_test.cgi' method=\'GET\'>\n";
#endif
	std::cout << 	"<input type=\'hidden\' name=\'page\' value=\'0\' />\n";
}

//--------------------------------------------------------------------------
bool page0::showErrors()
{
	if (getData().getSumbittedPage() != 0)
		return false;

	const session &session = getData().getLogin();

	if (session.getSelectUserID().isError ||
		session.getSelectPin().isError ||
		session.getMessage().length() > 0)
	{
		std::cout << "<div class='warn'>";

		if (!session.getMessage().empty())
			std::cout << "<p>" << session.getMessage() << "</p>\n";

		std::cout << session.getSelectUserID().getHTMLError();
		std::cout << session.getSelectPin().getHTMLError();
		std::cout << "</div>\n";
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------

void page0::sendQuestions( )
{
	bool showE = showErrors();
	const session & s = getData().getLogin();

	std::cout << "<p align=\"left\"><span><strong><font color=\"#009900\"><u>SIGN ON</u></font></strong></span></p>";
	std::cout << "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" border=\"0\">\n";
	std::cout << "	  <tr bgcolor=\"#EEEEEE\">\n";
	std::cout << "		<td style=\"WIDTH: 330px\">\n";
	std::cout << "		  <p align=\"left\">Username\n";
	if (showE && !s.getMessage().empty())
		std::cout << "<span class ='red'>*</span>\n";
	std::cout << "		  </p>\n";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
	std::cout << "		<p align=\"left\">\n";
	std::cout << "		  <input type=\"username\" name=\"username\" size=\"50\" value=\"\" " << REQUIRED << ">";
	std::cout << "		</p>\n";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";

	std::cout << "	  <tr>\n";
	std::cout << "		<td style=\"WIDTH: 330px\">\n";
	std::cout << "		  <p align=\"left\">Password\n";
	if (showE && !s.getMessage().empty())
		std::cout << "<span class ='red'>*</span>\n";
	std::cout << "		  </p>\n";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
	std::cout << "		<p align=\"left\">\n";
	std::cout << "		  <input type=\"password\" name=\"password\" size=\"50\" value=\"\" " << REQUIRED << ">";
	std::cout << "		</p>\n";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";
	std::cout << "  </table>\n";

	std::cout << "<p align=\"left\"></p>";
	std::cout << "<p align=\"left\"></p>";
	std::cout << "<hr>";
}






