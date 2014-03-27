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
	std::cout << 	"<form action='biocore_pacsstatus_test.cgi' method=\'POST\'>\n";
#else
	std::cout << 	"<form action='biocore_pacsstatus_test.cgi' method=\'GET\'>\n";
#endif
	std::cout << 	"<input type=\'hidden\' name=\'page\' value=\'1\' />\n";
}


//--------------------------------------------------------------------------
void page1::sendStatusInfo()
{
	ROSETTA GeneralStatsTotal;
	ROSETTA GeneralStatsIngored;
	ROSETTA GeneralStatsComplete;
	ROSETTA GeneralStatsInprogress;
	ROSETTA UnknownStudies;

	this->getData().getDicomStudies().getGeneralStats(this->getData().getDB(), "Total",GeneralStatsTotal);
	this->getData().getDicomStudies().getGeneralStats(this->getData().getDB(), "Complete",GeneralStatsComplete);
	this->getData().getDicomStudies().getGeneralStats(this->getData().getDB(), "Inprogress",GeneralStatsInprogress);
	this->getData().getDicomStudies().getGeneralStats(this->getData().getDB(), "Ignored",GeneralStatsIngored);
	this->getData().getDicomStudies().getUnknownStats(this->getData().getDB(), UnknownStudies);

	std::cout << "<div>";
	std::cout << "<H3>CRON JOBS</H3>";

	std::cout << "<table class=\"tablecss\">";
	std::cout << "<tr>";
	std::cout << 	"<th align=\"center\" style=\"width:10%\">CRON</th>";
	std::cout << 	"<th align=\"center\" style=\"width:10%\">Last run details</th>";
	std::cout << "</tr>";

	std::cout << "<tr>";
	std::cout << 	"<td align=\"center\">";
		std::cout << "QUERY";
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
		std::cout << this->getData().getDicomStudies().getCronJobString(this->getData().getDB(),"QUERY");
	std::cout <<    "</td>";
	std::cout << "</tr>";

	std::cout << "<tr>";
	std::cout << 	"<td align=\"center\">";
		std::cout << "UPLOAD QUIZ";
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
		std::cout << this->getData().getDicomStudies().getCronJobString(this->getData().getDB(),"UPLOADQUES");
	std::cout <<    "</td>";
	std::cout << "</tr>";

	std::cout << "<tr>";
	std::cout << 	"<td align=\"center\">";
		std::cout << "DOWNLOAD DICOM";
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
		std::cout << this->getData().getDicomStudies().getCronJobString(this->getData().getDB(),"DOWNLOADDI");
	std::cout <<    "</td>";
	std::cout << "</tr>";

	std::cout << "<tr>";
	std::cout << 	"<td align=\"center\">";
		std::cout << "LETTER GENERATOR";
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
		std::cout << this->getData().getDicomStudies().getCronJobString(this->getData().getDB(),"LETTERS");
	std::cout <<    "</td>";
	std::cout << "</tr>";

	std::cout << "<tr>";
	std::cout << 	"<td align=\"center\">";
		std::cout << "DECRYPT";
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
		std::cout << this->getData().getDicomStudies().getCronJobString(this->getData().getDB(),"DECRYPT");
	std::cout <<    "</td>";
	std::cout << "</tr>";

	std::cout << "</table>";


	std::cout << "<H3>PARTICIPENTS</H3>";
	std::cout << "<table class=\"tablecss\">";
	std::cout << "<tr>";
	std::cout << 	"<th align=\"center\" style=\"width:10%\">Participent Status</th>";
	std::cout << 	"<th align=\"center\" style=\"width:10%\">BIOBANK</th>";
	std::cout << 	"<th align=\"center\" style=\"width:10%\">BIOBANK_QA</th>";
	std::cout << 	"<th align=\"center\" style=\"width:10%\">BIOBANK_PSI</th>";
	std::cout << 	"<th align=\"center\" style=\"width:10%\">REPORTED</th>";
	std::cout << 	"<th align=\"center\" style=\"width:10%\">TOTAL</th>";
	std::cout << "</tr>";

	std::cout << "<tr>";
	std::cout << 	"<td align=\"center\">";
		std::cout << "Complete";
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
		std::cout << GeneralStatsComplete.getString("BIOBANK");
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
		std::cout << GeneralStatsComplete.getString("BIOBANK_QA");
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
		std::cout << GeneralStatsComplete.getString("BIOBANK_PSI");
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
		std::cout << GeneralStatsComplete.getString("REPORTED");
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
		std::cout << GeneralStatsComplete.getString("TOTAL");
	std::cout <<    "</td>";
	std::cout << "</tr>";

	std::cout << "<tr>";
	std::cout << 	"<td align=\"center\">";
		std::cout << "Inprogress";
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << GeneralStatsInprogress.getString("BIOBANK");
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << GeneralStatsInprogress.getString("BIOBANK_QA");
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << GeneralStatsInprogress.getString("BIOBANK_PSI");
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << GeneralStatsInprogress.getString("REPORTED");
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << GeneralStatsInprogress.getString("TOTAL");
	std::cout <<    "</td>";
	std::cout << "</tr>";


	std::cout << "<tr>";
	std::cout << 	"<td align=\"center\">";
		std::cout << "Ingored";
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << GeneralStatsIngored.getString("BIOBANK");
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << GeneralStatsIngored.getString("BIOBANK_QA");
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << GeneralStatsIngored.getString("BIOBANK_PSI");
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << GeneralStatsIngored.getString("REPORTED");
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << GeneralStatsIngored.getString("TOTAL");
	std::cout <<    "</td>";
	std::cout << "</tr>";

	std::cout << "<tr>";
	std::cout << 	"<td align=\"center\">";
		std::cout << "Total";
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << GeneralStatsTotal.getString("BIOBANK");
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << GeneralStatsTotal.getString("BIOBANK_QA");
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << GeneralStatsTotal.getString("BIOBANK_PSI");
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << GeneralStatsTotal.getString("REPORTED");
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << GeneralStatsTotal.getString("TOTAL");
	std::cout <<    "</td>";
	std::cout << "</tr>";

	std::cout << "</table>";

	std::cout << "<H3>UNKNOWN STUDIES</H3>";
	std::cout << "<table class=\"tablecss\">";
	std::cout << "<tr>";
	std::cout << 	"<th align=\"center\" style=\"width:10%\">AU</th>";
	std::cout << 	"<th align=\"center\" style=\"width:10%\">Total</th>";
	std::cout << "</tr>";

	std::cout << "<tr>";
	std::cout << 	"<td align=\"center\">";
		std::cout << "BIOBANK";
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << UnknownStudies.getString("BIOBANK");
	std::cout <<    "</td>";
	std::cout << "</tr>";

	std::cout << "<tr>";
	std::cout << 	"<td align=\"center\">";
		std::cout << "BIOBANK_QA";
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << UnknownStudies.getString("BIOBANK_QA");
	std::cout <<    "</td>";
	std::cout << "</tr>";

	std::cout << "<tr>";
	std::cout << 	"<td align=\"center\">";
		std::cout << "BIOBANK_PSI";
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << UnknownStudies.getString("BIOBANK_PSI");
	std::cout <<    "</td>";
	std::cout << "</tr>";
	std::cout << "<tr>";

	std::cout << "<tr>";
	std::cout << 	"<td align=\"center\">";
		std::cout << "REPORTED";
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << UnknownStudies.getString("REPORTED");
	std::cout <<    "</td>";
	std::cout << "</tr>";
	std::cout << "<tr>";

	std::cout << 	"<td align=\"center\">";
		std::cout << "TOTAL";
	std::cout <<    "</td>";
	std::cout << 	"<td align=\"center\">";
	std::cout << UnknownStudies.getString("TOTAL");
	std::cout <<    "</td>";
	std::cout << "</tr>";
	std::cout << "</table>";
	std::cout << "</div>";
}
//--------------------------------------------------------------------------
void page1::sendTabControls()
{
	this->getData().outputFormToken();
	std::cout << "<div class=\"AleBoxSm\"><P ALIGN=CENTER STYLE=\"margin-bottom: 0cm\">";
	std::cout << "<b>Site Navigation: </b>";
	std::cout << "<input type=\'submit\' disabled value='Status' name=\'Status\' />\n";
	std::cout << "<input type=\'submit\' value='Logs' name=\'Logs\' /> ";
	std::cout << "<input type=\'submit\' value='Correct Participant ID' name=\'ChangeID\' /> ";
	std::cout << "</P></div><br>";

	std::cout << "Update tables: <input type=\"submit\" value=\"Refresh\" name=\"statusRefresh\"/>\n<hr>";
}

//--------------------------------------------------------------------------
void page1::sendQuestions( )
{
	sendTabControls();
	sendStatusInfo();
}

//---------------------------------------------------------------------------


