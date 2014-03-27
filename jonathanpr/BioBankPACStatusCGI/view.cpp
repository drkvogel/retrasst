//---------------------------------------------------------------------------
#pragma hdrstop
#include <iostream>
#include "view.h"
#pragma package(smart_init)
//--------------------------------------------------------------------------

void View::sendHTML( )
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
 //	std::cout << "<H1>BIOBANK PACS STATUS</H1><p>\n";
}
//---------------------------------------------------------------------------

void View::deployJS() const
{
	std::cout << "<SCRIPT LANGUAGE=\"JavaScript\">";
	std::cout << "</SCRIPT>";
}
//---------------------------------------------------------------------------

void View::sendFooter( )
{
	std::cout << "<input type=\'hidden\' name=\'rosetta\' value=\'" << getData().getSerilizedData() << "\' />\n";

	if (!m_ShowNoButtons)
	{
		std::cout << " <input type=\'reset\' value=\'Clear\' class=\'left\' />"
				 " <input type=\'submit\' value=\'Next\' class=\'middle\' />\n";
	}
	std::cout << "</form>\n";
	std::cout << "</div>\n";
}
//---------------------------------------------------------------------------

void View::addOption( int id, const std::string & name, int selected ) const
{
	std::cout << "<option value=\'" << id << "\'";
	if( id == selected )
		std::cout << " selected ";

	std::cout << ">" << name << "</option>\n";
}
//---------------------------------------------------------------------------
void View::addOption(const std::string id, const std::string & name,const std::string selected ) const
{
	std::cout << "<option value=\'" << id << "\'";
	if( id == selected )
		std::cout << " selected ";

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

//---------------------------------------------------------------------------
std::string View::formatNumber(unsigned int number, int miniumSize)
{
	std::stringstream ss,result;
	ss << number;
	int length = ss.str().length();
	int count = miniumSize - length;
	for (int i=0;i<count;i++)
		result << "0";
	result << ss.str();
	return result.str();
}
