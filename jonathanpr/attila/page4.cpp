//---------------------------------------------------------------------------

#pragma hdrstop

#include <vector>
#include <iostream>

#include "page4.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void page4::sendHeader( ) const
{
	std::cout << "<p></p><p></p>";
	std::cout << "<p align=\"left\"><span><strong><font color=\"#009900\"><u>PART D</u>: RANDOMISATION:  RISK CATEGORY</font></strong></span>";
	std::cout <<  "</p>";

#ifdef RANDLIVE
	std::cout << 	"<form action='attila_rand_live.cgi' method=\'POST\'>\n";
#else
	std::cout << 	"<form action='attila_rand_test.cgi' method=\'GET\'>\n";
#endif
	std::cout << 	"<input type=\'hidden\' name=\'page\' value=\'4\' />\n";
}

//--------------------------------------------------------------------------

bool page4::showErrors() const
{
	if (getData().getSumbittedPage() != 4)
		return false;

	const patient& patient = getData().getPatient();

	if (!patient.getinformedconsent().isError && !patient.getriskwandering().isError && !patient.getriskhome().isError)
		return false;

	std::cout << "<div class='warn'>";
		std::cout << patient.getriskwandering().getHTMLError();
		std::cout << patient.getriskhome().getHTMLError();
		std::cout << patient.getinformedconsent().getHTMLError();
	std::cout << "</div>\n";
	return true;
}

//--------------------------------------------------------------------------

void page4::sendQuestions( ) const
{
	const patient& patient = getData().getPatient();
	bool showErrrors = showErrors();

	std::cout << "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" border=\"0\">\n";
	std::cout << "	  <tr>\n";
	std::cout << "		<td style=\"WIDTH: 330px\">\n";
		std::cout << "<p align=\"left\"><b>Risk of wandering or leaving the home inappropriately</b>";
	if (showErrrors)
		std::cout << patient.getriskwandering().getHTMLErrorWarning();
	std::cout << "		</p></td>\n";
	std::cout << "	  </tr>\n";

	std::cout << "	  <tr bgcolor=\"#EEEEEE\">\n";
	std::cout << "		<td>\n";
	std::cout << "<p align=\"left\">";
	addRadio(std::string("Low"), std::string("riskwandering"), 0, patient.getriskwandering().getText() == "0",RADIO_REQUIRED);
	std::cout << "&nbsp;&nbsp;&nbsp;&nbsp;\n";
	addRadio(std::string("Moderate"),std::string("riskwandering"), 1, patient.getriskwandering().getText() == "1",RADIO_REQUIRED);
	std::cout << "&nbsp;&nbsp;&nbsp;\n";
	addRadio(std::string("High"),std::string("riskwandering"), 2, patient.getriskwandering().getText() == "2",RADIO_REQUIRED);
	std::cout << "</p>";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";

	std::cout << "	  <tr>\n";
	std::cout << "		<td style=\"WIDTH: 330px\">\n";
		std::cout << "<p align=\"left\"><b>Safety risk within home</b>";
	if (showErrrors)
		std::cout << patient.getriskhome().getHTMLErrorWarning();

	std::cout << "		</p></td>\n";
	std::cout << "	  </tr>\n";

	std::cout << "	  <tr bgcolor=\"#EEEEEE\">\n";
	std::cout << "		<td>\n";
	std::cout << "<p align=\"left\">";
	addRadio(std::string("Low"),std::string("riskhome"), 0, patient.getriskhome().getText() == "0",RADIO_REQUIRED);
	std::cout << "&nbsp;&nbsp;\n";
	addRadio(std::string("Moderate"),std::string("riskhome"), 1, patient.getriskhome().getText() == "1",RADIO_REQUIRED);
	std::cout << "&nbsp;\n";
	addRadio(std::string("High"),std::string("riskhome"), 2, patient.getriskhome().getText() == "2",RADIO_REQUIRED);
	std::cout << "</p>\n";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";

  	std::cout << "	  <tr>\n";
	std::cout << "		<td style=\"WIDTH: 330px\">\n";
		std::cout << "<p align=\"left\"><b>Informed consent</b>";
	if (showErrrors)
		std::cout << patient.getinformedconsent().getHTMLErrorWarning();

	std::cout << "		</p></td>\n";
	std::cout << "	  </tr>\n";

	std::cout << "	  <tr bgcolor=\"#EEEEEE\">\n";
	std::cout << "		<td>\n";
	std::cout << "<p align=\"left\">";
	addRadio(std::string("From patient?"),std::string("informedconsent"), 0, patient.getinformedconsent().getText() =="0",RADIO_REQUIRED);
	std::cout << "&nbsp;&nbsp;\n";
	addRadio(std::string("From carer?"),std::string("informedconsent"), 1, patient.getinformedconsent().getText() == "1",RADIO_REQUIRED);
	std::cout << "&nbsp;\n";
	addRadio(std::string("From Consultee?"),std::string("informedconsent"), 2, patient.getinformedconsent().getText() == "2" ,RADIO_REQUIRED);
	std::cout << "&nbsp;\n";
	addRadio(std::string("None"),std::string("informedconsent"), 3, patient.getinformedconsent().getText() == "3" ,RADIO_REQUIRED);
	std::cout << "</p>\n";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";

	std::cout << "  </table>\n";
	std::cout << "<hr>";
}

