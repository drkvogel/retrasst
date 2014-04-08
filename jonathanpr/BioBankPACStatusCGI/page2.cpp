//---------------------------------------------------------------------------
#pragma hdrstop

#include <vector>
#include <iostream>
#include "page2.h"
#include <math.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)

void page2::sendHeader( ) const
{
	std::cout << "<p align=\"left\"></p>";
#ifdef RANDLIVE
	std::cout << 	"<form action='biocore_pacsstatus_test.cgi' method=\'POST\'>\n";
#else
	std::cout << 	"<form action='biocore_pacsstatus_test.cgi' method=\'GET\'>\n";
#endif
	std::cout << 	"<input type=\'hidden\' name=\'page\' value=\'2\' />\n";
}

//--------------------------------------------------------------------------
//Generate the HTML for the page
void page2::sendQuestions( )
{
//add the security token to the HTML
	this->getData().outputFormToken();
	int MaxToShowPerPage = 20;
	int page = this->getData().getLogs().getSelectedPage().igetText();

	int totalrows = this->getData().getLogs().getList().size();
	int currentRow = 0;
	int showFrom = page*MaxToShowPerPage;
	int showTo = showFrom+MaxToShowPerPage;
	int maxpages = (int)ceil((float)totalrows / (float)MaxToShowPerPage);

	std::cout << "<div class=\"AleBoxSm\"><P ALIGN=CENTER STYLE=\"margin-bottom: 0cm\">";
	std::cout << "<b>Site Navigation: </b>";
	std::cout << "<input type=\'submit\' value='Status' name=\'Status\' />\n";
	std::cout << "<input type=\'submit\' disabled value='Logs' name=\'Logs\' /> ";
	std::cout << "<input type=\'submit\' value='Correct Participant ID' name=\'ChangeID\' /> ";
	std::cout << "</P></div><br>";

	std::cout << "Update table: <input type=\'submit\' align=\"left\" value='Refresh' name=\'logRefresh\'/>\n<hr>";
	std::cout << "<H3>SYSTEM LOGS</H3>";
	std::cout << "<table class=\"tablecss\">";
	std::cout << "<tr>";
	std::cout << 	"<th align=\"center\" style=\"width:15%\">Timestamp</th>";
	std::cout << 	"<th align=\"center\" style=\"width:15%\">Type</th>";
	std::cout << 	"<th align=\"center\" style=\"width:15%\">Level</th>";
	std::cout << 	"<th align=\"center\" style=\"width:15%\">PatientID</th>";
	std::cout << 	"<th align=\"center\" style=\"width:55%\">Message</th>";
	std::cout << "</tr>";
	std::cout << "<tr>";

	std::cout << 	"<td>";
	int logTimeFilter = this->getData().getLogs().gettimespanFilter();
	if (logTimeFilter == -1)
		logTimeFilter = 1;

	std::cout << 	"<select align=\"center\" style=\"width:120px\" name=\"logtimespanFilter\">";
	addOption(1,"From last month",logTimeFilter );
	addOption(2,"From 2 months",logTimeFilter );
	addOption(3,"From 3 months",logTimeFilter );
	addOption(6,"From 6 months",logTimeFilter );
	addOption(12,"From 1 year",logTimeFilter );
	addOption(24,"From 2 year",logTimeFilter );
	std::cout << 	"</select>";
	std::cout << 	"</th>";

	std::cout << 	"<td>";
	std::cout << 	"<select align=\"center\" style=\"width:120px\" name=\"logTypeFilter\">";
	int logFilter = this->getData().getLogs().gelogTypeFilter();

	addOption(-1,"Don't Filter",logFilter );
	addOption(0,"General",logFilter );
	addOption(1,"Database",logFilter );
	addOption(2,"File System",logFilter );
	addOption(3,"PACS",logFilter );

	std::cout << 	"</select>";
	std::cout << 	"</th>";
	std::cout << 	"<td>";

	std::cout << 	"<select align=\"center\" style=\"width:120px\" name=\"logLevelFilter\">";
	int levelFilter = this->getData().getLogs().getlogLevelFilter();

	addOption(-1,"Don't Filter",levelFilter );
	addOption(0,"Debug",levelFilter );
	addOption(1,"Info",levelFilter );
	addOption(2,"Error",levelFilter );
	addOption(3,"Critical",levelFilter );

	std::cout << 	"</select>";
	std::cout << 	"</th>";
	std::cout << 	"<td>";
		std::cout << "<input title=\"Use % as wildcards to pattern match\" id=\"fpid\" value=\"" << this->getData().getLogs().getlogPatientIDfilter() << "\" name=\"logPatientIDfilter\" type=\"text\" /> ";
	std::cout << 	"</th>";
	std::cout << 	"<td>";
	std::cout << 	"</th>";
	std::cout << "</tr>";

	const std::vector<PACSlog> logs = this->getData().getLogs().getList();

	for (std::vector<PACSlog>::const_iterator  it = logs.begin(); it != logs.end(); ++it)
	{
		if ((currentRow >= showFrom) && (currentRow < showTo))
		{
			std::cout << "<tr>";
			std::cout << 	"<td align=\"center\">" << View::formatNumber((*it).getTimestamp().getDay()) << "/" << View::formatNumber((*it).getTimestamp().getMonth()) << "/" << (*it).getTimestamp().getYear() << " " << View::formatNumber((*it).getTimestamp().getHour()) << ":" << View::formatNumber((*it).getTimestamp().getMinute()) << ":" << View::formatNumber((*it).getTimestamp().getSecond()) << "</th>";
			std::cout << "</th>";

			if ((*it).getLogtype() == 0)
				std::cout << 	"<td align=\"center\">General</th>";
			else if ((*it).getLogtype() == 1)
				std::cout << 	"<td align=\"center\">Database</th>";
			else if ((*it).getLogtype() == 2)
				std::cout << 	"<td align=\"center\">File System</th>";
			else if ((*it).getLogtype() == 3)
				std::cout << 	"<td align=\"center\">PACS</th>";
			else
				std::cout << 	"<td align=\"center\">-</th>";

			if ((*it).getLoglevel() == 0)
				std::cout << 	"<td align=\"center\">DEBUG</th>";
			else if ((*it).getLoglevel() == 1)
				std::cout << 	"<td align=\"center\">INFO</th>";
			else if ((*it).getLoglevel() == 2)
				std::cout << 	"<td align=\"center\">ERROR</th>";
			else if ((*it).getLoglevel() == 3)
				std::cout << 	"<td align=\"center\">CRITICAL</th>";
			else
				std::cout << 	"<td align=\"center\">-</th>";

			std::string pid = (*it).getPatientid();
			if (pid.empty())
				pid = "-";
			std::cout << 	"<td align=\"center\">"<<pid<<"</th>";
			std::cout << 	"<td align=\"center\">"<<(*it).getMessage()<<"</th>";

			std::cout << "</tr>";
		}
		currentRow++;
	}
	std::cout << "</table>";

	std::cout << "<div class=\"AleBoxSm\"><P ALIGN=CENTER STYLE=\"margin-bottom: 0cm\">";
	std::cout << "<b>Table Navigation:</B> ";
	if (page+1 <= 1)
		std::cout << " <input type=\'submit\' value='Prev' disabled name=\'logchange\' />\n";
	else
		std::cout << " <input type=\'submit\' value='Prev' name=\'logchange\' />\n";

	if (page+1 >= maxpages)
		std::cout << " <input type=\'submit\' value='Next' disabled name=\'logchange\'  />\n";
	else
		std::cout << " <input type=\'submit\' value='Next' name=\'logchange\'/>\n";
	std::cout << 	"<br> [ Page: " << (page+1) << " of " << maxpages << " ] ";
	std::cout << "</P></div><br>";
}

//---------------------------------------------------------------------------


