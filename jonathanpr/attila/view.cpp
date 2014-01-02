//---------------------------------------------------------------------------

#pragma hdrstop

#include <iostream>
#include "view.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//--------------------------------------------------------------------------

void View::sendHTML( ) const
{
	deployJS();
	sendTitle();
	sendHeader();
	sendQuestions();
	sendFooter();
}
//--------------------------------------------------------------------------

void View::sendTitle( ) const
{
	std::cout << "<div id=\"content\" style=\"WIDTH: 75%\" align=\"center\">\n";
	std::cout << "<table cellspacing=\"0\" cellpadding=\"0\" border=\"0\" style=\"HEIGHT: 100px; POSITION: relative; WIDTH: 100%\">";
	std::cout << 	  "<tr>";
	std::cout << 		  "<td style=\"HEIGHT: 100px; WIDTH: 10%\">";
	std::cout << 		  "<img hspace=\"0\" align=\"left\" src=\"image001.png\" width=\"100px\" height=\"100px\">";
	std::cout <<		  "</td>";
	std::cout << 		  "<td>";
	std::cout << 			"<div style=\"HEIGHT: 100px; BACKGROUND: #005600; WIDTH: 100%\">";
	std::cout << 				"<font face=\"Arial\" style=\"FONT-SIZE: 14pt; COLOR: white\" align=\"center\"><strong>Appendix 11<BR>Registration and Randomisation Form<BR> Version 1.6<BR> 21/11/2013</strong></font>";
	std::cout <<		     "</div>";
	std::cout <<		  "</td>" ;
	std::cout << 	   "</tr>\n";
	std::cout << "</table>\n";
	std::cout << "<hr><p>\n";
}

void View::deployJS() const
{
	std::cout << "<SCRIPT LANGUAGE=\"JavaScript\">";
	std::cout << "function deployJS() {}\n";
	std::cout << "</SCRIPT>";
}


void View::sendFooter( ) const
{
	std::cout << "<input type=\'hidden\' name=\'rosetta\' value=\'" << getData().getSerilizedData() << "\' />\n";

	if (!m_onlyShowNextButton)
	{
		std::cout << " <input type=\'reset\' value=\'Clear\' class=\'left\' />"
				 " <input type=\'submit\' value=\'Next\' class=\'middle\' />\n"
				 "</form>\n";
	}
	std::cout << "</div>\n";
}

//---------------------------------------------------------------------------

void View::addOption( int id, const std::string & name, int selected ) const
{
	std::cout << "<option value=\'" << id << "\'";
	if( id == selected )
	{
		std::cout << " selected ";
	}
	std::cout << ">" << name << "</option>\n";
}

//---------------------------------------------------------------------------

void View::addRadio( std::vector<std::string> &vectorvalues, const std::string & name, int selected, bool required ) const
{
	for(unsigned int n = 0; n < vectorvalues.size(); n++ )
	{
		std::cout << ' ' << vectorvalues.at(n)
				  << " <input type=\'radio\' name=\'" << name << "\' value=\'" << n << "\' "
				  << (((int)n) == selected ? "checked" : required ? "required" : "") << " />\n";
	}
}

//---------------------------------------------------------------------------
void View::addRadio(std::string vectorvalue, const std::string name, int value, bool selected, bool required ) const
{
		std::cout << ' ' << vectorvalue
				  << " <input type=\'radio\' name=\'" << name << "\' value=\'" << value << "\' "
				  << (selected ? "checked" : required ? "required" : "") << " />\n";
}

//---------------------------------------------------------------------------
void View::addRadioReadOnly(std::string vectorvalue, const std::string name, int value, bool selected) const
{
		std::cout << ' ' << vectorvalue
				  << " <input type=\'radio\' DISABLED name=\'" << name << "\' value=\'" << value << "\' "
				  << (selected ? "checked" : "") << " />\n";
}



