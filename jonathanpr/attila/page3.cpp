#pragma hdrstop

#include <vector>
#include <iostream>

#include "page3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)


void page3::deployJS() const
{
std::cout << "<SCRIPT LANGUAGE=\"JavaScript\">";
std::cout << "function initValue() {document.forms[0].caregiversupportfreetext.disabled=true;}";

std::cout << "function setCaregiversupportfreetext(text) {";
std::cout << "   document.forms[0].caregiversupportfreetext.disabled=true;";
std::cout << "	document.forms[0].caregiversupportfreetext.value = text;";
std::cout << "	}";


std::cout << "function setOtherText() {";
std::cout << "   document.forms[0].caregiversupportfreetext.disabled=false;";
std::cout << "	document.forms[0].caregiversupportfreetext.value = '';";
std::cout << "	document.forms[0].caregiversupportfreetext.focus();";
std::cout << "	}";

std::cout << "</SCRIPT>";
}

void page3::sendHeader( ) const
{
	std::cout << "<p></p><p></p>";
	std::cout << "<p align=\"left\"><span><strong><font color=\"#009900\"><u>PART C</u>: CAREGIVER DETAILS</font></strong></span>";
	std::cout <<  "</p>";

#ifdef RANDLIVE
	std::cout << 	"<form action='attila_rand_live.cgi' method=\'POST\'>\n";
#else
	std::cout << 	"<form action='attila_rand_test.cgi' method=\'GET\'>\n";
#endif
	std::cout << 	"<input type=\'hidden\' name=\'page\' value=\'3\' />\n";
}

//--------------------------------------------------------------------------

bool page3::showErrors() const
{
	if (getData().getSumbittedPage() != 3)
		return false;

	const patient& patient = getData().getPatient();

	if (!patient.getcaregiversupport().isError &&!patient.getlivewith().isError && !patient.getcarersname().isError && !patient.getcarerdob().isError)
		return false;

	std::cout << "<div class='warn'>";
	std::cout << patient.getcaregiversupport().getHTMLError();
	std::cout << patient.getlivewith().getHTMLError();
	std::cout << patient.getcarersname().getHTMLError();
	std::cout << patient.getcarerdob().getHTMLError();
	std::cout << "</div>\n";
	return true;
}

//--------------------------------------------------------------------------

void page3::exAddRadio(std::string vectorvalue, const std::string name, int value, bool selected, bool required, bool other ) const
{
	if (other)
	{
	std::cout << ' ' << vectorvalue
			  << " <input type=\'radio\' name=\'" << name << "\' value=\'" << value << "\' "
			  << (selected ? "checked" : required ? "required" : "") << " onClick=\"setOtherText()\" />\n";
	}
	else
	{
	std::cout << ' ' << vectorvalue
			  << " <input type=\'radio\' name=\'" << name << "\' value=\'" << value << "\' "
			  << (selected ? "checked" : required ? "required" : "") << " onClick=\"setCaregiversupportfreetext('"<< vectorvalue << "')\" />\n";
	}
}

//===========================================================================
void page3::sendQuestions( ) const
{
	bool errors = showErrors();
	const patient& patient = getData().getPatient();

	std::string p = patient.getlivewith().getText();
	int selection = patient.getIDFromLiveWith(p);

	std::string tsuport = patient.getcaregiversupport().getText();

	std::cout << "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" border=\"0\">\n";
	std::cout << "	  <tr>\n";
	std::cout << "		<td style=\"WIDTH: 330px\">\n";
		std::cout << "<p align=\"left\"><b>Lives with</b></p>";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";

	std::cout << "	  <tr bgcolor=\"#EEEEEE\">\n";
	std::cout << "		<td>\n";
	std::cout << "<p align=\"left\">";
	exAddRadio(std::string("spouse/partner"), std::string("livewith"), 0, selection == 0,RADIO_REQUIRED,false );
	std::cout << "&nbsp;&nbsp;";
	exAddRadio(std::string("alone"),std::string("livewith"), 1, selection == 1,RADIO_REQUIRED,false );
	std::cout << "&nbsp;&nbsp;";
	exAddRadio(std::string("Other"),std::string("livewith"), 2, selection == 2,RADIO_REQUIRED,true );
	std::cout << "&nbsp;&nbsp; Specify ";
	if (selection == 2)
		std::cout << "<input style=\"HEIGHT: 22px; WIDTH: 200px\" size=\"5\"  maxlength=\"60\" name=\"caregiversupportfreetext\" " << patient.getlivewith().getValueText() << ">";
	else
		std::cout << "<input style=\"HEIGHT: 22px; WIDTH: 200px\" size=\"5\"  maxlength=\"60\" name=\"caregiversupportfreetext\" >";
	std::cout << "</p>";
	std::cout << "		</td>\n";

	std::cout << "	  </tr>\n";

	std::cout << "	  <tr>\n";
	std::cout << "		<td style=\"WIDTH: 330px\">\n";
		std::cout << "<p align=\"left\"><b>Caregiver Support</b></p>";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";

	std::cout << "	  <tr bgcolor=\"#EEEEEE\">\n";
	std::cout << "		<td>\n";
	std::cout << "<p align=\"left\">";
	addRadio(std::string("Live-in"),std::string("caregiversupport"), 0, tsuport == "0",RADIO_REQUIRED );
	std::cout << "&nbsp;&nbsp;";
	addRadio(std::string("Visits at least once/day"),std::string("caregiversupport"), 1, tsuport == "1",RADIO_REQUIRED );
	std::cout << "&nbsp;&nbsp;";
	addRadio(std::string("Visits less than once/day"),std::string("caregiversupport"), 2, tsuport == "2",RADIO_REQUIRED );
	std::cout << "</p>\n";
	std::cout << "<p align=\"left\">";
	if (errors)
		std::cout << patient.getcaregiversupport().getHTMLErrorWarning();	std::cout << "</p>";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";
	std::cout << "  </table>\n";

	std::cout << "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" border=\"0\">\n";
	std::cout << "	  <tr>\n";
	std::cout << "		<td style=\"WIDTH: 330px\">\n";
		std::cout << "<p align=\"left\">Principal caregiver's full name</p>";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
	std::cout << "<p align=\"centre\"><input style=\"HEIGHT: 22px; WIDTH: 267px\" size=\"5\"  maxlength=\"60\"  name=\"carersname\" "  << patient.getcarersname().getValueText() << REQUIRED << " > \n";
	if (errors)
		std::cout << patient.getcarersname().getHTMLErrorWarning();
	std::cout << "</p>";
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";

	std::cout << "	  <tr bgcolor=\"#EEEEEE\">\n";
	std::cout << "		<td>\n";
		std::cout << "<p align=\"left\">Date of birth</p>";
	std::cout << "		</td>\n";
	std::cout << "		<td>\n";
		std::cout << "<p align=\"centre\"><input type=\"text\" pattern=\"\\d{1,2}/\\d{1,2}/\\d{4}\" style=\"HEIGHT: 22px; WIDTH: 267px\" size=\"5\" name=\"carerdob\" title =\"DD/MM/YYYY\"  " << patient.getcarerdob().getValueText() <<  REQUIRED << " placeholder=\"DD/MM/YYYY\" > \n";
	if (errors)
		std::cout << patient.getcarerdob().getHTMLErrorWarning();
	std::cout << "</p>";

	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";
	std::cout << "  </table>\n";

	std::cout << "<hr>";
}


