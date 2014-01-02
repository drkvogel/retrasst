//---------------------------------------------------------------------------

#pragma hdrstop

#include <vector>
#include <iostream>
  #include "xcgi.h"
#include "page5.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

void page5::sendHeader( ) const
{
	std::cout << "<p></p><p></p>";
	std::cout << "<p align=\"left\"><span><strong><font color=\"#009900\"><u>PART E</u>: SUMMARY AND CONFIRMATION</font></strong></span>";
	std::cout <<  "</p>";

#ifdef RANDLIVE
	std::cout << 	"<form action='attila_rand_live.cgi' method=\'POST\'>\n";
#else
	std::cout << 	"<form action='attila_rand_test.cgi' method=\'GET\'>\n";
#endif
	std::cout << 	"<input type=\'hidden\' name=\'page\' value=\'5\' />\n";
}

//--------------------------------------------------------------------------

bool page5::showErrors() const
{
	if (getData().getSumbittedPage() != 5)
		return false;

	checklist c = getData().getEligibility();
	session s = getData().getLogin();
	if ((c.getConfirmationStage() != VALID) || (s.getVarificationStage() != VALID))
	{
		std::cout << "<div class='warn'>";

		if (c.getConfirmationStage() != VALID)
			std::cout << "<p> You need to confirm the information provided is correct before you can sumbit.</p>\n";
		if (!s.getMessage().empty())
			std::cout << "<p>" << s.getMessage() << "</p>\n";
		std::cout << s.getVarifiyUserID().getHTMLError();
		std::cout << s.getVarifiyPin().getHTMLError();
		std::cout << "</div>\n";
		return true;
	}
	return false;
}

//--------------------------------------------------------------------------

void page5::askLogin(bool ShowErrors) const
{
	std::cout << "<HR><p align=\"left\"><span><strong><font color=\"#009900\"><u>VERIFY IDENTITY</u></font></strong></span></p>";
	const clinician &c = getData().getClinicians();
	const session &s = getData().getLogin();

	std::cout << "<input type=\'hidden\' name=\'vnx\' value=\'" << c.selectedClinician.id << "\' />\n";

	std::cout << "\n</td></tr>\n<tr><td>Enter your PIN again to enter this patient into the Attila study";
	if (ShowErrors)
		std::cout << s.getSelectUserID().getHTMLErrorWarning();
	std::cout << "</td><td>";

	std::cout << "<input type=\"password\" name=\"vup\" size=\"50\" value=\"\" " << REQUIRED << "></td></tr></table>";
	std::cout << "</p>\n";
}

//--------------------------------------------------------------------------

void page5::renderRow(const std::string &lable,const std::string &data,int count) const
{
	if (count%2==0)
		std::cout << "	  <tr bgcolor=\"#EEEEEE\">\n";
	else
		std::cout << "	  <tr>\n";

	std::cout << "		<td class='alignleft'>\n";
	std::cout << lable;
	std::cout << "		</td>\n";
	std::cout << "		<td class='alignleft'>";
	std::cout << data;
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";
}

void page5::renderRow(const std::string &lable,const int &data,int count) const
{
	if (count%2==0)
		std::cout << "	  <tr bgcolor=\"#EEEEEE\">\n";
	else
		std::cout << "	  <tr>\n";

	std::cout << "		<td class='alignleft'>\n";
	std::cout << lable;
	std::cout << "		</td>\n";
	std::cout << "		<td class='alignleft'>";
	std::cout << data;
	std::cout << "		</td>\n";
	std::cout << "	  </tr>\n";
}

void page5::sendQuestions( ) const
{
	bool bShowErrors = showErrors();

	const centre & center = getData().getCentre();
	const clinician & clinician = getData().getClinicians();
	const patient& patient = getData().getPatient();
	const checklist &eligibility = getData().getEligibility();

	int i=0;

	std::cout << "<table cellspacing=\"0\" cellpadding=\"0\" width=\"100%\" border=\"0\">\n";

	renderRow("Centre No:",center.getID(),i++);
	renderRow("Centre Name:",XCGI::makeWebSafe(center.getName()),i++);
	renderRow("Principal Investigator (PI):",XCGI::makeWebSafe(clinician.getName()),i++);
	renderRow("Responsible Social Services Professional:",XCGI::makeWebSafe(patient.getrssprofessional().getText()),i++);
	renderRow("Patient's full name:",XCGI::makeWebSafe(patient.getname().getText()),i++);
	renderRow("Patient's date of birth:",XCGI::makeWebSafe(patient.getdob().getText()),i++);
	renderRow("Patient's gender:",XCGI::makeWebSafe(patient.getgender().getText()),i++);

	renderRow("Hospital number:",XCGI::makeWebSafe(patient.gethospitalno().getText()),i++);
	renderRow("NHS number:",XCGI::makeWebSafe(patient.getnhsnumber().getText()),i++);
	renderRow("GP:",XCGI::makeWebSafe(patient.getgp().getText()),i++);

	renderRow("Diagnosis of dementia:",XCGI::makeWebSafe(eligibility.getDODString()),i++);
	renderRow("FACS (Fair Access to Care Services) eligible:",XCGI::makeWebSafe(eligibility.getFACSString()),i++);
	renderRow("Living in an ordinary dwelling:",XCGI::makeWebSafe(eligibility.getLivingOrginaryDwellingString()),i++);
	renderRow("Working telephone line connected to house:",XCGI::makeWebSafe(eligibility.getWorkingTelephoneString()),i++);
	renderRow("Unstable medical or psychiatric condition:",XCGI::makeWebSafe(eligibility.getUnstableString()),i++);
	renderRow("Participating in another clinical trial for a dementia intervention:",XCGI::makeWebSafe(eligibility.getAnotherTrialString()),i++);

	renderRow("Living situation:",XCGI::makeWebSafe(patient.getlivewith().getText()),i++);
	renderRow("Caregiver:",XCGI::makeWebSafe(patient.getCaregiverDetailsString()),i++);
	renderRow("Principal caregiver's full name:",XCGI::makeWebSafe(patient.getcarersname().getText()),i++);
	renderRow("Principal caregiver's Date of birth:",XCGI::makeWebSafe(patient.getcarerdob().getText()),i++);

	renderRow("Risk of wandering or leaving the home inappropriately:",XCGI::makeWebSafe(patient.getRiskWanderingString()),i++);
	renderRow("Safety risk within home:",XCGI::makeWebSafe(patient.getRiskHomeString()),i++);
	renderRow("Informed consent:",XCGI::makeWebSafe(patient.getInformedConsentString()),i++);

	if (patient.getoperatorname().getText().empty())
	{
		renderRow("Person randomising:",XCGI::makeWebSafe(clinician.getName()),i++);
		renderRow("Telephone:",XCGI::makeWebSafe(clinician.getTelNumber()),i++);
	}
	else
	{
		renderRow("Person randomising:",XCGI::makeWebSafe(patient.getoperatorname().getText()),i++);
		renderRow("Telephone:",XCGI::makeWebSafe(patient.getoperatortelnum().getText()),i++);
	}
	std::cout << "  </table>\n";
	std::cout << "<hr>\n";

	std::cout << "<p align=\"left\">&nbsp;";
	addRadio(std::string("I confirm the above details are correct"), std::string("DETAILSCONFIRMED"), 0, false,RADIO_REQUIRED );
	std::cout << "</p>\n";
	askLogin(bShowErrors);
	std::cout << "<hr>";
}


