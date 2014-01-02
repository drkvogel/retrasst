//---------------------------------------------------------------------------

#pragma hdrstop

#include "pageIneligible.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#include <vector>
#include <iostream>

void pageIneligible::sendHeader( ) const
{
	std::cout << "<p align=\"left\">";
	std::cout << "</p>";

	std::cout << "<p></p><p></p>";
	std::cout << "<p align=\"left\"><span><strong><font color=\"#009900\"><u>PART B</u>: ELIGIBILITY CHECKLIST</font></strong></span>";
	std::cout <<  "</p>";
}

//--------------------------------------------------------------------------

void pageIneligible::showNotEligibile() const
{
	const checklist c = getData().getEligibility();


	std::cout << "<B>The patient is not eligible to be put on the ATTILA trial.</B><BR><BR>";
	std::cout << "<p>Reason:</p>";
	if (!c.getMessage().empty())
		std::cout << c.getMessage();
	else
		std::cout << "No informed consent was given.";
	std::cout << "<hr>";
}

//--------------------------------------------------------------------------

void pageIneligible::sendQuestions( ) const
{
	showNotEligibile();
}

