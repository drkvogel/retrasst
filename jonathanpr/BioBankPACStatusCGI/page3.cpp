//---------------------------------------------------------------------------
#pragma hdrstop

#include <vector>
#include <iostream>
#include "page3.h"
#include <math.h>

//---------------------------------------------------------------------------
#pragma package(smart_init)

void page3::sendHeader( ) const
{
	std::cout << "<p align=\"left\"></p>";
#ifdef RANDLIVE
	std::cout << 	"<form action='biocore_pacsstatus_test.cgi' method=\'POST\'>\n";
#else
	std::cout << 	"<form action='biocore_pacsstatus_test.cgi' method=\'GET\'>\n";
#endif
	std::cout << 	"<input type=\'hidden\' name=\'page\' value=\'3\' />\n";

}

//--------------------------------------------------------------------------
void page3::deployJS() const
{
	std::cout << "<SCRIPT LANGUAGE=\"JavaScript\">";
//	std::cout <<      "$(document).ready(function(){ alert('Javascript is called'); }); ";
	std::cout << "</SCRIPT>";
}

//Generate the HTML for the page
void page3::sendQuestions( )
{
//add the security token to the HTML
	this->getData().outputFormToken();

	int MaxToShowPerPage = 20;
	int page = this->getData().getUnknownDicom().getSelectedPage().igetText();

	int totalrows = this->getData().getUnknownDicom().getList().size();
	int maxpages = (int)ceil((float)totalrows / (float)MaxToShowPerPage);
	if (maxpages == page)
		page = maxpages-1;

	int currentRow = 0;
	int showFrom = page*MaxToShowPerPage;
	int showTo = showFrom+MaxToShowPerPage;

	std::cout << "<div class=\"AleBoxSm\"><P ALIGN=CENTER STYLE=\"margin-bottom: 0cm\">";
	std::cout << "<b>Site Navigation: </b>";
	std::cout << "<input type=\'submit\' value='Status' name=\'Status\' />\n";
	std::cout << "<input type=\'submit\'  value='Logs' name=\'Logs\' /> ";
	std::cout << "<input type=\'submit\' disabled value='Correct Participant ID' name=\'ChangeID\' /> ";
	std::cout << "</P></div><br>";

	std::cout << "Update table: <input type=\'submit\' align=\"left\" value='Refresh' name=\'changeIDRefresh\'/><hr>\n";

	std::cout << "<H3>UPDATE KNOWN STUDIES</H3>";
	std::cout << "<div id='error' class='warn'>\n";
	std::cout << "</div>\n";
	std::cout << 		"<script> $('#error').hide();</script>";

	std::cout << "<table class=\"tablecss\">";
	std::cout << "<tr>";
	std::cout << 	"<th align=\"center\" style=\"width:15%\">Timestamp</th>";
	std::cout << 	"<th align=\"center\" style=\"width:15%\">AET</th>";
	std::cout << 	"<th align=\"center\" style=\"width:15%\">Anon key PID</th>";
	std::cout << 	"<th align=\"center\" style=\"width:15%\">Anon key FN</th>";
	std::cout << 	"<th align=\"center\" style=\"width:15%\">Status</th>";
	std::cout << 	"<th align=\"center\" style=\"width:15%\"></th>";
	std::cout << "</tr>";
	std::cout << "<tr>";

	std::cout << 	"<td>";
	int logTimeFilter = this->getData().getUnknownDicom().getTimespanFilter();
	if (logTimeFilter == -1)
		logTimeFilter = 1;

	std::cout << 	"<select align=\"center\" style=\"width:120px\" name=\"unknownTimeSpanFilter\">";
	addOption(1,"From last month",logTimeFilter );
	addOption(2,"From 2 months",logTimeFilter );
	addOption(3,"From 3 months",logTimeFilter );
	addOption(6,"From 6 months",logTimeFilter );
	addOption(12,"From 1 year",logTimeFilter );
	addOption(24,"From 2 year",logTimeFilter );
	std::cout << 	"</select>";
	std::cout << 	"</th>";

	std::cout << 	"<td>";
	std::cout << 	"<select align=\"center\" style=\"width:120px\" name=\"unknownAetFilter\">";
	std::string aetFilter = this->getData().getUnknownDicom().getAetFilter();
	std::set<std::string> aetSet = this->getData().getUnknownDicom().getAETs();

	addOption("","Don't Filter",aetFilter);
	std::set<std::string>::iterator it;
	for (it=aetSet.begin(); it!=aetSet.end(); ++it)
		addOption((*it),(*it),aetFilter);

	std::cout << 	"</select>";
	std::cout << 	"</td>";

	std::cout << 	"<td>";
	std::cout << 	"</th>";

	std::cout << 	"<td>";
	std::cout << 	"</th>";

	std::cout << 	"<td>";
	std::cout << 	"</th>";

	std::cout << 	"<td>";
	std::cout << 	"</th>";
	std::cout << "</tr>";


	std::cout << "<style>\n";
	std::cout << "	.highlight { background: red;}\n";
	std::cout << "</style>\n";
 //MOVE THE JS to it's own file
	std::cout << "<SCRIPT type=\"text/javascript\">";

	std::cout << 	"	function convertFromHex(hex) {";

	std::cout << 	"	var hex = hex.toString(); var str = '';";
	std::cout << 	"	for (var i = 0; i < hex.length; i += 2)  ";
	std::cout << 	"		str += String.fromCharCode(parseInt(hex.substr(i, 2), 16)); ";
	std::cout << 	"	return str;} \n";

	std::cout << 	"function showEditDetails(invalue,show){";
	std::cout <<      "if (show){";
	std::cout <<      	"$('#HidPID' + invalue).slideDown();";
	std::cout <<      	"$('#HidFN' + invalue).slideDown();";
	std::cout <<      	"$('#HidCEN' + invalue).slideDown();";
	std::cout <<      	"$('#But' + invalue).val('Cancel');";
	std::cout <<      	"$('#ButIgnore' + invalue).fadeIn();}";
	std::cout <<      "else {";
	std::cout <<      	"$('#HidPID' + invalue).slideUp();";
	std::cout <<      	"$('#HidFN' + invalue).slideUp();";
	std::cout <<      	"$('#HidCEN' + invalue).slideUp();";
	std::cout <<   	  	"$('#But' + invalue).val('Update Details');";
	std::cout <<    	"$('#ButIgnore' + invalue).fadeOut();}";
	std::cout <<      "}";

	std::cout << "function gettd(){";
	std::cout << "return '" << this->getData().getSecurotySessionString() << "';\n";
	std::cout <<      "}";

	std::cout << "function editClicked(invalue){";
	std::cout <<      "var oldstate = $('#HidPID' + invalue).is(':visible');";
	std::cout <<      "$('#HidPID' + invalue).slideToggle();";
	std::cout <<      "$('#HidFN' + invalue).slideToggle();";
	std::cout <<      "$('#HidCEN' + invalue).slideToggle();";

	std::cout <<      "if (oldstate) {   ";
	std::cout <<      	"$('#error').hide(); ";
	std::cout <<    	  "$('#But' + invalue).val('Update Details');";
	std::cout <<    	  "$('#ButIgnore' + invalue).fadeOut();";
	std::cout <<      "} else  {    ";
	std::cout <<     	  "$('#But' + invalue).val('cancel');";
	std::cout <<    	  "$('#ButIgnore' + invalue).fadeIn();";
	std::cout <<      "}";
	std::cout <<      " }";

	std::cout << "function ignoreClicked(invalue,td){\n";
	std::cout << "  var r=confirm('Are you sure you want to ignore this study? It will no longer be processed'); \n";
	std::cout << "  if (r==true) {\n";
	std::cout << "    $('#error').hide(); \n";

	std::cout << "    $.get('psicgi.cgi', { mode: 'ignoreStudy', pk: + invalue, tk: td } )\n";
	std::cout << "    .done(function( data ){\n";

	std::cout << "    var bodyStart = data.indexOf('<BODY>') + '<BODY>'.length;\n";
	std::cout << "    var bodyEnd = data.indexOf('</BODY>');\n";
	std::cout << "    var bodyText = data.substring(bodyStart,bodyEnd);\n";
	std::cout << "    var res = convertFromHex(bodyText.trim()).trim();\n";

	std::cout << "    if (res != \"OK\") {\n";
	std::cout << "      $('#error').html(res);\n      $('#error').show(); return;} else {$('#error').hide(); }";
	std::cout << "			 showEditDetails(invalue,false);";
	std::cout << "			$('#statustext' + invalue).text('IGNORED'); ";
	std::cout << "			 })";
	std::cout << " 		.fail(function() { alert('error, please try again');})";

	std::cout <<		"} ";
//	std::cout << 		"	else {alert('You pressed Cancel!');} ";
	std::cout <<      "}\n";

	std::cout << "\nfunction checkClicked(invalue,td){\n";
	//                 8 letter checking and other things to be done in JS on keypress.
	std::cout <<		"if (!validateText(invalue)) return;";
	std::cout <<		"var newPID = $('#newpidtext' + invalue).val(); ";
	std::cout <<		"var newFN = $('#newfntext' + invalue).val(); ";
	std::cout <<		"$.get('psicgi.cgi', { mode: 'updateStudy', pk: invalue, pid: newPID, fn: newFN, tk: td  } )";
	std::cout <<		 ".done(function( data ){";

	std::cout << "    var bodyStart = data.indexOf('<BODY>') + '<BODY>'.length;\n";
	std::cout << "    var bodyEnd = data.indexOf('</BODY>');\n";
	std::cout << "    var bodyText = data.substring(bodyStart,bodyEnd);\n";
	std::cout << "    var res = convertFromHex(bodyText.trim()).trim();\n";

	std::cout << " 		if (res != \"OK\") {$('#error').html(res); $('#error').show(); return;} else {$('#error').hide(); }";

	std::cout <<		 	"showEditDetails(invalue,false);";
	std::cout <<		    "$('#pidtext' + invalue).text(newPID); ";
	std::cout <<		    "$('#fntext' + invalue).text(newFN); ";
	std::cout <<			"$('#statustext' + invalue).text('OK'); ";
	std::cout <<		 	"})";
	std::cout <<		 ".fail(function() { alert('error, please try again');})";
	std::cout <<      "}\n";

	std::cout << "\nfunction validateText(invalue){\n";
	std::cout <<		"var errorString = \"\";\n";
	std::cout <<		"var pass = true;\n";
	std::cout <<		"var newPID = $('#newpidtext' + invalue).val();\n";
	std::cout <<		"var newFN = $('#newfntext' + invalue).val();\n";
	std::cout <<		"if (newPID.length != 8) {  pass = false;\n";
	std::cout <<		"errorString += 'Participant id needs to be 8 characters in length<br>';\n";
	std::cout <<		"$('#newpidtext' + invalue).addClass(\"highlight\" );}\n";
	std::cout <<        "else {$('#newpidtext' + invalue).removeClass(\"highlight\" );}\n";
	std::cout <<		"if (newFN.length != 8) {  pass = false;\n";
	std::cout <<		"errorString += 'First name needs to be 8 characters in length<br>';\n";
	std::cout <<		"$('#newfntext' + invalue).addClass(\"highlight\" );}\n";
	std::cout <<        "else {$('#newfntext' + invalue).removeClass(\"highlight\" );}\n";
 /* NO LONGER THE CASE THAT THE NAME AND THE PID ARE THE REVERSE OF ONE ANOTHER.
	std::cout <<        "if (pass) {\n";
	std::cout <<		"var revrsedPID = \"\";\n";
	std::cout <<		"for (i = 0; i <= newPID.length; i++) {revrsedPID = newPID.charAt(i) + revrsedPID;} \n";
	std::cout <<		"if (revrsedPID != newFN) {  pass = false;\n";
	std::cout <<		"errorString += 'The first name needs to be the reverse of the participant id<br>';\n";
	std::cout <<		"$('#newfntext' + invalue).addClass(\"highlight\" );";
	std::cout <<		"$('#newpidtext' + invalue).addClass(\"highlight\" );\n";
	std::cout <<        "} else {\n";*/
	std::cout <<        "$('#newfntext' + invalue).removeClass(\"highlight\");\n";
 	std::cout <<        "$('#newpidtext' + invalue).removeClass(\"highlight\");\n";
 //	std::cout <<        "$('#newpidtext' + invalue).removeClass(\"highlight\");}\n";
  //	std::cout <<        "}\n";
	std::cout << 		"if (!pass) {$('#error').html(errorString); $('#error').show(); } else {$('#error').hide(); }";
	std::cout <<		"return pass;";
	std::cout <<      "}\n";
	std::cout << "</SCRIPT>";


	const std::vector<unknowndicom> unknowndicomVector = this->getData().getUnknownDicom().getList();

	for (std::vector<unknowndicom>::const_iterator  it = unknowndicomVector.begin(); it != unknowndicomVector.end(); ++it)
	{
		if ((currentRow >= showFrom) && (currentRow < showTo))
		{
			std::cout << 	"<tr>";
			std::cout << 	"<td align=\"center\">";
			std::cout << 	View::formatNumber((*it).getStudydate().getDay()) << "/" << View::formatNumber((*it).getStudydate().getMonth()) << "/" << (*it).getStudydate().getYear() << " " << View::formatNumber((*it).getStudydate().getHour()) << ":" << View::formatNumber((*it).getStudydate().getMinute()) << ":" << View::formatNumber((*it).getStudydate().getSecond());
			std::cout << 	"</th>";

			std::cout << 	"<td align=\"center\">"<<(*it).getAet();;
			std::cout << 	"<div id=\"HidCEN" << (*it).getStudypk() << "\"> ";
			std::cout << 		"<script> $('#HidCEN" << (*it).getStudypk() << "').hide();</script>";
			std::cout <<    "<input type='button' value='Submit' id='ButCheck" << (*it).getStudypk() << "' onclick='checkClicked("<< (*it).getStudypk() <<"  , gettd() )' >" << "</td>";

			std::cout << 	"</div>";
			std::cout << 	"</th>";

			std::string patiendid =  (*it).getPatientid();
			if (patiendid.empty())
				 patiendid = "-";
			std::cout << 	"<td align=\"center\">";
			std::cout <<	"<span id='pidtext" << (*it).getStudypk() << "'>" << patiendid << "</span>";
			std::cout << 	"<div id=\"HidPID" << (*it).getStudypk() << "\"> ";
			std::cout << 		"<script> $('#HidPID" << (*it).getStudypk() << "').hide();</script>";
			std::cout << 		"<input type=\"text\" id='newpidtext" << (*it).getStudypk() << "' placeholder='ABCD1234' >";
			std::cout << 	"</div>";
			std::cout << 	"</th>";

			std::string firstname =  (*it).getFirstname();
			if (firstname.empty())
				 firstname = "-";
			std::cout << 	"<td align=\"center\">";
			std::cout <<	"<span id='fntext" << (*it).getStudypk() << "'>" << firstname << "</span>";
			std::cout << 	"<div id='HidFN" << (*it).getStudypk() << "'> ";
			std::cout << 		"<script> $('#HidFN" << (*it).getStudypk() << "').hide();</script>";
			std::cout << 		"<input type=\"text\" id='newfntext" << (*it).getStudypk() << "' placeholder='1234ABCD'  >";
			std::cout << 	"</div>";
			std::cout << 	"</th>";

			std::cout << 	"<td align=\"center\" id='statustext" << (*it).getStudypk() << "'>";
			std::cout << 		"FAILED MATCH";
			std::cout  <<   "</td>";

			std::cout << 	"<td >";
			std::cout <<       "<input type='button' value='Update Details' id='But" << (*it).getStudypk() << "' onclick='editClicked("<< (*it).getStudypk() << ")'>";
			std::cout <<       "<input type='button' value='Ignore' id='ButIgnore" << (*it).getStudypk() << "' onclick='ignoreClicked("<< (*it).getStudypk() << " , gettd() )'>";   //
			std::cout << 		"<script> $('#ButIgnore" << (*it).getStudypk() << "').hide();</script>";
			std::cout  <<   "</td>";
			std::cout << 	"</tr>";
		}
		currentRow++;
	}
	std::cout << "</table>";

	std::cout << "<div class=\"AleBoxSm\"><P ALIGN=CENTER STYLE=\"margin-bottom: 0cm\">";
	std::cout << "<b>Table Navigation:</B> ";
	if (page+1 <= 1)
		std::cout << " <input type=\'submit\' value='Prev' disabled name=\'unknownchange\' />\n";
	else
		std::cout << " <input type=\'submit\' value='Prev' name=\'unknownchange\' />\n";

	if (page+1 >= maxpages)
		std::cout << " <input type=\'submit\' value='Next' disabled name=\'unknownchange\'  />\n";
	else
		std::cout << " <input type=\'submit\' value='Next' name=\'unknownchange\'/>\n";
	std::cout << 	"<br> [ Page: " << (page+1) << " of " << maxpages << " ] ";
	std::cout << "</P></div><br>";
}

//---------------------------------------------------------------------------


