//---------------------------------------------------------------------------

#pragma hdrstop

#include <vector>
#include <iostream>

#include "page2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void page2::sendHeader( ) const
{
	std::cout << "<p align=\"left\">";
	std::cout << "</p>";

	std::cout << "<p></p><p></p>";
	std::cout << "<p align=\"left\"><span><strong><font color=\"#009900\"><u>PART B</u>: ELIGIBILITY CHECKLIST</font></strong></span>";
	std::cout <<  "</p>";

#ifdef RANDLIVE
	std::cout << 	"<form action='attila_rand_live.cgi' method=\'POST\'>\n";
#else
	std::cout << 	"<form action='attila_rand_test.cgi' method=\'GET\'>\n";
#endif
	std::cout << 	"<input type=\'hidden\' name=\'page\' value=\'2\' />\n";
}

//--------------------------------------------------------------------------

void page2::showErrors() const
{
	const checklist & check = getData().getEligibility();

	if ((check.getStage() == INCOMPLETE) && (getData().getSumbittedPage() == 2))
	{
		std::cout << "<div class='warn'>";
		std::cout << "<p> You have to answer every part of the checklist.</p>\n";
		std::cout << "</div>\n";
	}
}

//--------------------------------------------------------------------------

void page2::sendQuestions( ) const
{
	showErrors();
	askEligibility();
}

//--------------------------------------------------------------------------

void page2::askEligibility() const
{
	const checklist & check = getData().getEligibility();
	std::vector<std::string> values;
	values.push_back("Yes");
	values.push_back("No");

	std::vector<RADIO_OPTIONS> textOutPut;

	RADIO_OPTIONS option;
	option.text = "Diagnosis of dementia?";
	option.varname = "DOD";
	option.selected = check.getDOD();
	textOutPut.push_back(option);
	option.text = "Able to give informed consent?";
	option.text = "FACS (Fair Access to Care Services) eligible?";
	option.varname = "FACS";
	option.selected = check.getFACS();
	textOutPut.push_back(option);
	option.text = "Living in an ordinary dwelling?";
	option.varname = "LIOD";
	option.selected = check.getLivingOrginaryDwelling();
	textOutPut.push_back(option);
	option.text = "Working telephone line connected to house?";
	option.varname = "WTLCTH";
	option.selected = check.getWorkingTelephone();
	textOutPut.push_back(option);
	option.text = "Already provided ATT intervention for dementia?";
	option.varname = "APATT";
	option.selected = check.getProvidedATT();
	textOutPut.push_back(option);
	option.text = "Unstable medical or psychiatric condition?";
	option.varname = "UMORC";
	option.selected = check.getUnstable();
	textOutPut.push_back(option);
	option.text = "Participating in another clinical trial for a dementia intervention?";
	option.varname = "PIACTFADI";
	option.selected = check.getAnotherTrial();
	textOutPut.push_back(option);

	std::cout << "<table style=\"HEIGHT: 408px; WIDTH: 600px\" cellspacing=\"1\" cellpadding=\"1\" border=\"0\">\n";

	for (unsigned int i=0;i<textOutPut.size();i++)
	{
		RADIO_OPTIONS &thisoption = textOutPut.at(i);

		std::cout << "	  <tr";
		if (i%2==0)
			std::cout << " bgcolor=\"#EEEEEE\" ";
		std::cout << ">\n";

		std::cout << "		<td style=\"WIDTH: 400px\">";
		std::cout << "<p align=\"left\">";
		std::cout << thisoption.text;
		std::cout << "</p>";
		std::cout << "</td>\n";
		std::cout << "		<td></td>\n";
		std::cout << "		<td>\n";
		addRadio(values, thisoption.varname, thisoption.selected,REQUIREDbool);
		std::cout << "     </td>\n";
		std::cout << "	 </tr>\n";
	}
	std::cout << "</table>\n";
	std::cout << "<hr>";
}



