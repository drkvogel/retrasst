#pragma hdrstop

#include <vector>
#include <iostream>

#include "page6.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void page6::sendHeader( ) const
{
	std::cout << "<p></p><p></p>";
	std::cout << "<p align=\"left\"><span><strong><font color=\"#009900\"><u>RANDOMISATION:</u> TREATMENT ALLOCATION</font></strong></span>";
	std::cout <<  "</p>";
}

//--------------------------------------------------------------------------

void page6::sendQuestions( ) const
{
	if (!getData().hasPatientSavedOK())
	{
		std::cout << "<div class='warn'>";
		std::cout << getData().getPatientSavedOKMsg();
		std::cout << "</div>\n";
	}
	else
	{
		patient p = getData().getPatient();
		centre c = getData().getCentre();

		std::stringstream StudyNoString;
		if (c.getID() < 10)
			StudyNoString << "0";
		StudyNoString << c.getID();

		int personID = p.getPersonID();
		if (personID < 10)
			StudyNoString << "0";
		if (personID < 100)
			StudyNoString << "0";
		StudyNoString << personID;

		std::string StudyNo = StudyNoString.str();

		std::cout << "<p align=\"left\">ATTILA Study No: <b>" << StudyNo << "</b> </p>\n";
		std::cout << "<p align=\"left\">Randomisation date: <b>" << p.getrnddate().getText() << "</b></p>\n";

		std::cout << "<p align=\"left\">Randomisation Allocation: ";
		if (0 == p.getrndalloc().igetText())
			std::cout << "<b>Control</b>";
		else if (1 == p.getrndalloc().igetText())
			std::cout << "<b>Intervention</b>";
		else
			std::cout << "<b>ERROR</b>";
		std::cout << "</p>\n";
		std::cout << "The PI will recieve an email confirmation within the next few hours.";
	}
	std::cout << "<hr>";
}
