//---------------------------------------------------------------------------
#pragma hdrstop

//---------------------------------------------------------------------------
#pragma package(smart_init)

// ---------------------------------------------------------------------------
#include "patient.h"
#include <sstream>
#include "xcgi.h"
#include "xquery.h"
#include "xexec.h"
#include "xencode.h"
#include "xaes.h"
#include "xmz.h"
#include "stringsplit.h"
#include "validation.h"
#include "enckey.h"
// ---------------------------------------------------------------------------
//	accept participant details from hidden fields or user input
// ---------------------------------------------------------------------------

patient::patient() : stageCareGiver( UNKNOWN ),stageRisk( UNKNOWN ),stageIdentifyingDetails( UNKNOWN )
{

}

void patient::parse(const XCGI *cgi,const ROSETTA &R)
{
	readDetails(R);
//get any updates from parameters

//do first of all, the two which will be encrpyted.
//	m_nhsnumber.setText(cgi->setParamIfExistsElseDefault("nhsnumber",m_nhsnumber.getText()));

	std::string name = cgi->setParamIfExistsElseDefault("nhsnumber","");
	if (!name.empty() && (name != m_nhsnumber.getText()))
	{
		m_nhsnumber.setText(name);
		std::string res = encIdentifyingDetail(name, name);
		if (res != "OK")
			m_nhsnumber_enc.setMessage(res);
		else
		   m_nhsnumber_enc.setText(name);
	}

	name = cgi->setParamIfExistsElseDefault("name","");
	if (!name.empty() && (name != m_name.getText()))
	{
		m_name.setText(name);
		std::string res = encIdentifyingDetail(name, name);
		if (res != "OK")
			m_name_enc.setMessage(res);
		else
		   m_name_enc.setText(name);
	}

	m_dob.setText(cgi->setParamIfExistsElseDefault("dob",m_dob.getText()));
	m_hospitalno.setText(cgi->setParamIfExistsElseDefault("hospitalno",m_hospitalno.getText()));
	m_gp.setText(cgi->setParamIfExistsElseDefault("gp",m_gp.getText()));
	m_informedconsent.setText(cgi->setParamIfExistsElseDefault("informedconsent",m_informedconsent.getText()));

//	m_carersname.setText(cgi->setParamIfExistsElseDefault("carersname",m_carersname.getText()));
	name = cgi->setParamIfExistsElseDefault("carersname","");
	if (!name.empty() && (name != m_carersname.getText()))
	{
		m_carersname.setText(name);
		std::string res = encIdentifyingDetail(name, name);
		if (res != "OK")
			m_carersname_enc.setMessage(res);
		else
		   m_carersname_enc.setText(name);
	}

	m_carerdob.setText(cgi->setParamIfExistsElseDefault("carerdob",m_carerdob.getText()));
	m_riskwandering.setText(cgi->setParamIfExistsElseDefault("riskwandering",m_riskwandering.getText()));
	m_riskhome.setText(cgi->setParamIfExistsElseDefault("riskhome",m_riskhome.getText()));
	m_studyno.setText(cgi->setParamIfExistsElseDefault("studyno",m_studyno.getText()));
	m_rnddate.setText(cgi->setParamIfExistsElseDefault("rnddate",m_rnddate.getText()));
	m_rndalloc.setText(cgi->setParamIfExistsElseDefault("rndalloc",m_rndalloc.getText()));
	m_operatorname.setText(cgi->setParamIfExistsElseDefault("operatorname",m_operatorname.getText()));
	m_operatortelnum.setText(cgi->setParamIfExistsElseDefault("operatortelnum",m_operatortelnum.getText()));
	m_rssprofessional.setText(cgi->setParamIfExistsElseDefault("rssprofessional",m_rssprofessional.getText()));
	m_caregiversupport.setText(cgi->setParamIfExistsElseDefault("caregiversupport",m_caregiversupport.getText()));

	getCareGiverDetails(cgi);
	getGenderDetails(cgi);
}

void patient::getGenderDetails(const XCGI *cgi )
{
	std::string details = cgi->getParamDefault("gender","");
	if (details.empty())
		return;

	if (details == "0")
		m_gender.setText("F");
	else if (details == "1")
		m_gender.setText("M");
	else
		m_gender.setMessage("Please select \'F\' or \'M\'");
}

std::string patient::getSupportText(const std::string &id) const
{
	if (id == "0")
		return "Living with spouse/partner";
	else if (id == "1")
		return "Living alone";
	else if (id == "2")
		return "Other";
	return "";
}

int patient::getIDFromLiveWith(const std::string &text) const
{
	if (text == "Living with spouse/partner")
		return 0;
	else if (text == "Living alone")
		return 1;
	else if (text.length() > 0) //OTHER
		return 2;
	return -1;
}

int patient::getIDFromSupportText(const std::string &text) const
{
	if (text == "Live-in caregiver")
		return 0;
	else if (text == "Caregiver visits at least once/day")
		return 1;
	else if (text == "Caregiver visits less than once/day")
		return 2;
	return -1;
}

void patient::getCareGiverDetails(const XCGI *cgi )
{
	std::string details = cgi->getParamDefault("livewith","");
	if (details != "")
	{
		std::string text = getSupportText(details);

		if (text == "Other")
		{
			std::string caregiversupportfreetext = cgi->getParamDefault("caregiversupportfreetext","");
			if (caregiversupportfreetext.empty())
				m_livewith.setMessage("Please enter caregiver support into text box");
			else
				m_livewith.setText(caregiversupportfreetext);
		}
		else if (text.length() > 0)
			m_livewith.setText(text);
		else
			m_livewith.setMessage("Select caregiver support");
	}
}

// ---------------------------------------------------------------------------
//	check if details are complete and participant is eligible
// ---------------------------------------------------------------------------

void patient::checkDetails( )
{
	stageIdentifyingDetails = INCOMPLETE;
	stageCareGiver = INCOMPLETE;
	stageRisk = INCOMPLETE;

	if (m_gender.getText() != "M" && m_gender.getText() != "F")
	{
		stageIdentifyingDetails = INELIGIBLE;
		m_gender.setMessage("Patient's Sex, please select \'F\' or \'M\'");
	}

	if (m_name.getText().length() == 0)
	{
		stageIdentifyingDetails = INELIGIBLE;
		m_name.setMessage("Patient name is required.");
	}
	else
	{
		int res = validation::isVARCHAR(2,60,m_name.getText());
		if (res != 0)
		{
			stageIdentifyingDetails = INELIGIBLE;
			if (res == -1)
				m_name.setMessage("The patient name needs to be greater then 2 letters.");
			else
				m_name.setMessage("The patient name needs to be less then 60 characters in length.");
		}
		if (m_name_enc.isError)
			stageIdentifyingDetails = INELIGIBLE;
	}

	if (!validation::isDOB(m_dob))
	{
		stageIdentifyingDetails = INELIGIBLE;
	}

	if (m_nhsnumber.getText().length() != 0)
	{
		// expecting 3-3-4
		if (!validation::isNHSNumber(m_nhsnumber.getText()))
		{
			stageIdentifyingDetails = INELIGIBLE;
			m_nhsnumber.setMessage("NHS number format needs to be XXX-XXX-XXXX format");
		}
	}

	if (m_rssprofessional.getText().length() == 0)
	{
		stageIdentifyingDetails = INELIGIBLE;
		m_rssprofessional.setMessage("Responsible Social Services Professional is required.");
	}
	else
	{
		int res = validation::isVARCHAR(2,60,m_rssprofessional.getText());
		if (res != 0)
		{
			stageIdentifyingDetails = INELIGIBLE;
			if (res == -1)
				m_rssprofessional.setMessage("The Responsible Social Services Professional name needs to be greater then 2 letters.");
			else
				m_rssprofessional.setMessage("The Responsible Social Services Professional name needs to be less then 60 characters in length.");
		}
	}

//boundry check
	if (m_hospitalno.getText().length() > 0)
	{
		int res = validation::isVARCHAR(0,40,m_hospitalno.getText());
		if (res != 0)
		{
			stageIdentifyingDetails = INELIGIBLE;
			m_hospitalno.setMessage("The hospital number needs to be less then 40 characters in length.");
		}
	}
	if (m_gp.getText().length() > 0)
	{
		int res = validation::isVARCHAR(0,60,m_gp.getText());
		if (res != 0)
		{
			stageIdentifyingDetails = INELIGIBLE;
			m_gp.setMessage("The GP's name needs to be less then 60 characters in length.");
		}
	}

//operator is optional, but if 1 is filled, make sure the other is too.
	if (!m_operatortelnum.getText().empty() || !m_operatorname.getText().empty())
	{
		if (!m_operatortelnum.getText().empty() && m_operatorname.getText().empty())
		{
			stageIdentifyingDetails = INELIGIBLE;
			m_operatorname.setMessage("Person randomising needs filling.");
		}
		else if (m_operatortelnum.getText().empty() && !m_operatorname.getText().empty())
		{
			stageIdentifyingDetails = INELIGIBLE;
			m_operatortelnum.setMessage("Person randomising needs their telephone number added.");
		}
		else
		{
			if (!validation::isPhoneNumber(m_operatortelnum.text))
				m_operatortelnum.setMessage("Person randomising needs a valid telephone number");

			int res = validation::isVARCHAR(2,60,m_operatorname.getText());
			if (res != 0)
			{
				stageIdentifyingDetails = INELIGIBLE;
				if (res<0)
					m_operatorname.setMessage("Person randomising - needs to be greater than 2 letters.");
				else
					m_operatorname.setMessage("Person randomising - needs to be less than 60 letters.");
			}
		}
	}

	if  (m_operatorname.isError || m_operatortelnum.isError || m_gender.isError || m_name_enc.isError || m_name.isError || m_dob.isError || m_rssprofessional.isError || m_nhsnumber.isError || m_hospitalno.isError || m_gp.isError)
		return;

	stageIdentifyingDetails = VALID;

	if (!validation::isINT(0,2,m_caregiversupport.getText()))
	{
		m_caregiversupport.setMessage("Please select - caregiver support");
		stageCareGiver = INELIGIBLE;
	}

	if (m_livewith.getText().length() == 0)
	{
		stageCareGiver = INELIGIBLE;
		m_livewith.setMessage("Living with details are required.");
	}
	else
	{
		int res = validation::isVARCHAR(2,60,m_livewith.getText());
		if (res != 0)
		{
			stageCareGiver = INELIGIBLE;
			if (res<0)
				m_livewith.setMessage("Living with details needs to be greater than 2 letters.");
			else
				m_livewith.setMessage("Living with details needs to be less than 60 letters.");
		}
	}

	if (m_carersname.getText().length() == 0)
	{
		stageCareGiver = INELIGIBLE;
		m_carersname.setMessage("Principal caregiver's full name is required.");
	}
	else
	{
		int res = validation::isVARCHAR(2,60,m_carersname.getText());
		if (res != 0)
		{
			stageCareGiver = INELIGIBLE;
			if (res<0)
				m_carersname.setMessage("Caregiver's name needs to be greater than 2 letters.");
			else
				m_carersname.setMessage("Caregiver's name details needs to be less than 60 letters.");
		}
	}

	if (!validation::isDOB(m_carerdob,18,100))
		stageCareGiver = INELIGIBLE;

	if (m_livewith.isError || m_caregiversupport.isError || m_carersname.isError|| m_carerdob.isError)
		return;

	stageCareGiver = VALID;

	if (!validation::isINT(0,2,m_riskhome.getText()))
	{
		m_riskhome.setMessage("Please select - safety risk within home");
		stageRisk = INELIGIBLE;
	}
	if (!validation::isINT(0,2,m_riskwandering.text))
	{
		m_riskwandering.setMessage("Please select - risk of wandering or leaving the home inappropriately");
		stageRisk = INELIGIBLE;
	}
	if (!validation::isINT(0,3,m_informedconsent.text))
	{
		m_informedconsent.setMessage("Please select - informed consent");
		stageRisk = INELIGIBLE;
	}

	if (m_informedconsent.isError || m_riskwandering.isError || m_riskhome.isError)
		return;

	stageRisk = VALID;
}

// ---------------------------------------------------------------------------
//	read relevant fields
// ---------------------------------------------------------------------------
void patient::readDetails(const ROSETTA & fields )
{
	setFromRosetta(fields,"name",m_name);
	setFromRosetta(fields,"name_enc",m_name_enc);
	setFromRosetta(fields,"dob" ,m_dob);
	setFromRosetta(fields,"gender" ,m_gender);
	setFromRosetta(fields,"hospitalno" ,m_hospitalno);
	setFromRosetta(fields,"nhsnumber" ,m_nhsnumber);
	setFromRosetta(fields,"nhsnumber_enc" ,m_nhsnumber_enc);
	setFromRosetta(fields,"gp" ,m_gp);
	setFromRosetta(fields,"informedconsent" ,m_informedconsent);
	setFromRosetta(fields,"getlivewith", m_livewith);
	setFromRosetta(fields,"caregiversupport" ,m_caregiversupport);
	setFromRosetta(fields,"carersname" ,m_carersname);
	setFromRosetta(fields,"carersname_enc" ,m_carersname_enc);
	setFromRosetta(fields,"carerdob" ,m_carerdob);
	setFromRosetta(fields,"riskwandering" ,m_riskwandering);
	setFromRosetta(fields,"riskhome" ,m_riskhome);
	setFromRosetta(fields,"studyno" ,m_studyno);
	setFromRosetta(fields,"rnddate" ,m_rnddate);
	setFromRosetta(fields,"rndalloc" ,m_rndalloc);
	setFromRosetta(fields,"operatorname" ,m_operatorname);
	setFromRosetta(fields,"operatortelnum" ,m_operatortelnum);
	setFromRosetta(fields,"rssprofessional",m_rssprofessional);
}

// ---------------------------------------------------------------------------
//	load the fields from the ROSETTA
// ---------------------------------------------------------------------------
void patient::setFromRosetta(const ROSETTA &fields,const std::string &name,Result &var) const
{
	var.text = fields.getStringDefault(name ,"");
}

// ---------------------------------------------------------------------------
//	save the fields to the ROSETTA
// ---------------------------------------------------------------------------
void patient::addToRosetta(ROSETTA & fields,const std::string &name,const Result &var) const
{
	if (!var.getText().empty())
	{
		fields.setString(name,var.text);
	}
}

void patient::addFields( ROSETTA & fields ) const
{
	addToRosetta(fields, "name", m_name);
	addToRosetta(fields, "name_enc", m_name_enc);
	addToRosetta(fields, "dob", m_dob);
	addToRosetta(fields, "gender", m_gender);
	addToRosetta(fields, "hospitalno",m_hospitalno);
	addToRosetta(fields, "nhsnumber", m_nhsnumber);
	addToRosetta(fields, "nhsnumber_enc", m_nhsnumber_enc);
	addToRosetta(fields, "gp", m_gp);
	addToRosetta(fields, "getlivewith",m_livewith);
	addToRosetta(fields, "caregiversupport",m_caregiversupport );
	addToRosetta(fields, "carersname", m_carersname);
	addToRosetta(fields, "carersname_enc", m_carersname_enc);
	addToRosetta(fields, "carerdob", m_carerdob);
	addToRosetta(fields, "riskwandering",m_riskwandering);
	addToRosetta(fields, "riskhome", m_riskhome);
	addToRosetta(fields, "informedconsent",m_informedconsent );
	addToRosetta(fields, "studyno",m_studyno);
	addToRosetta(fields, "rnddate",m_rnddate);
	addToRosetta(fields, "rndalloc",m_rndalloc);
	addToRosetta(fields, "operatorname", m_operatorname);
	addToRosetta(fields, "operatortelnum",m_operatortelnum);
	addToRosetta(fields, "rssprofessional",m_rssprofessional);
}
// ---------------------------------------------------------------------------

int patient::getAgeGroupFromDOB(const std::string &dobstring) const
{     //   Age (<65, 65-74, 75-84, 85+)
	//Three way:  <65, 65-80, 81+)
	XDATE dob;
	validation::stringToXDATE(dobstring,dob);
	int ageGroup = dob.getAge();
	if (ageGroup < 65)
		ageGroup = 0;
	else if (ageGroup <= 80)
		ageGroup = 1;
	ageGroup = 2;
	return ageGroup;
}

// ---------------------------------------------------------------------------
bool patient::doRandomise(XDB *db,int centre_id)
{
/*
(i)            Gender
(ii)           Age (<65, 65-74, 75-84, 85+)
(iii)          Risk of wandering or leaving the home inappropriately (Not at risk, probably not at risk, possibly at risk, probably at risk)
(iv)         Safety risks (falls, fire, etc) identified within the home (Not at risk, probably not at risk, possibly at risk, probably at risk)
(v)          Level of caregiver support available (Live-in caregiver 24 hours per day, live-in caregiver > 8 hours per day, caregiver visits at least once daily, caregiver visits less often than daily).
 */

	int ageGroup = getAgeGroupFromDOB(m_dob.getText());

	int gender = 0;
	if (m_gender.getText() == "M")
		gender = 1;
	int riskwandering = m_riskwandering.igetText();
	int riskhome = m_riskhome.igetText();
	int caregiversupport = m_caregiversupport.igetText();

	XMZ	m( "Unknown" );
// DEFINE COLUMN/STRATA PROPERTIES
	XMZ_VAR *ma = m.createVar( "Gender",2,"gender");
	XMZ_VAR *mb = m.createVar( "Age group",3,"agegroup");
	XMZ_VAR *mc = m.createVar( "Risk of wandering",3,"riskwandering");
	XMZ_VAR *md = m.createVar( "Risk in home",3,"riskhome");
	XMZ_VAR *me = m.createVar( "Caregiver support",3,"caregiversupport");

	XMZ_VAR *mz = m.createVar( "Allocation",   2, "rndalloc" );

	if ( NULL == ma || NULL == mb || NULL == mc || NULL == md || NULL == me || NULL == mz )
		return false;

	// LOAD PREVIOUS VALID (REC_STATUS=0) ENTRIES
	// NO "WHERE" CLAUSE
	m.addFromDB( db, "person", "" );

	mz->setRole( XMZ_VAR::Out );    // HOLDS RESULT OF the CALCULATION

	std::vector<int> current;
	current.resize(6);
	current[0] = gender;
	current[1] = ageGroup;
	current[2] = riskwandering;
	current[3] = riskhome;
	current[4] = caregiversupport;

	std::vector<int> weights;
	weights.resize(2);
	weights[0] = 1;
	weights[1] = 1;

	if ( ! m.eval( &current, &weights ) )
		return false;

	const int bestg = m.select( 0 );  // THIS IS CURRENT[2]
	{
	std::stringstream ss;
	ss << bestg;
	m_rndalloc.setText(ss.str());
	}
	{
	std::stringstream ss;
	XDATE D;
	ss << D.getDay() << "/" << D.getMonth() << "/" << D.getYear();
	m_rnddate.setText(ss.str());
	}
	return true;
}

//----------------------------------------------------------------------------


std::string patient::getNextPersonID(XDB *db)
{
	m_person_id =-1;
	std::stringstream sqlquery;
	sqlquery <<  "SELECT patient_seq.nextval";

	XQUERY query( db, sqlquery.str());
	if (!query.open())
	{
		return query.getLastError().c_str();
	}
	if ( query.fetch() )
	{
		m_person_id = atoi(query.result.getStringDefault(0,"0").c_str());
		query.close();
		return std::string("OK");
	}
	query.close();
	return "No next sequence returned";
}

//----------------------------------------------------------------------------
void patient::addForDBFields( ROSETTA & fields,const std::string &clinicianName,const std::string &clinicianNumber) const
{
// convert the data to the correct types.
//all this data has been validated before
	XDATE dobDATE,carerdobDATE,rnddateDATE;
	validation::stringToXDATE(m_dob.getText(),dobDATE);
	validation::stringToXDATE(m_carerdob.getText(),carerdobDATE);
	validation::stringToXDATE(m_rnddate.getText(),rnddateDATE);

//	fields.setString("name", m_name.getText());
	fields.setString("name_enc", m_name_enc.getText());

	if (!m_nhsnumber.getText().empty())
	{
//		fields.setString( "nhsnumber",m_nhsnumber.getText());
		fields.setString("nhsnumber_enc", m_nhsnumber_enc.getText());
	}

	if (!m_hospitalno.getText().empty())
		fields.setString( "hospitalno",m_hospitalno.getText());

	fields.setDate("dob", dobDATE);

	//save age group
	int ageGroup = getAgeGroupFromDOB(m_dob.getText());
	fields.setInt("agegroup",ageGroup);

	int igender = 0;
	if (m_gender.getText() == "M")
		igender = 1;
	fields.setInt("gender", igender);    //int

	if (!m_gp.getText().empty())
		addToRosetta(fields, "gp", m_gp); //optional

	addToRosetta(fields, "livewith",m_livewith );

	fields.setInt("caregiversupport",m_caregiversupport.igetText()); //int

	fields.setDate("carerdob", carerdobDATE);
	fields.setInt("riskwandering",m_riskwandering.igetText()); //int
	fields.setInt("riskhome", m_riskhome.igetText());          //int
	fields.setInt("informedconsent",m_informedconsent.igetText());  //int
	fields.setDate("rnddate", rnddateDATE);
	fields.setInt("rndalloc",m_rndalloc.igetText());             //int

//	addToRosetta(fields, "carersname", m_carersname);
	addToRosetta(fields, "carersname_enc", m_carersname_enc);

	if (getoperatorname().getText().empty())
	{
		fields.setString("operatorname",clinicianName);
		fields.setString("operatortelnum",clinicianNumber);
	}
	else
	{
		addToRosetta(fields, "operatorname", m_operatorname);
		addToRosetta(fields, "operatortelnum",m_operatortelnum);
	}


	addToRosetta(fields, "rssprofessional",m_rssprofessional);

	fields.setInt("person_id",m_person_id);
}
 // ---------------------------------------------------------------------------
/*
void patient::encIdentifyingDetails()
{
	if ((m_nhsnumber.getText().empty()) && (m_carersname.getText().empty()) && (m_name.getText().empty()))
		return;

	std::string enckey = enckey::getDBEncKey();
	XAES myAES = XAES(enckey);
	if (!myAES.isValid())
	{
		m_name_enc.setMessage("Error with encrpyting patient name. If this error persists, contact support.");
		m_nhsnumber_enc.setMessage("Error with encrpyting NHS number.");
		std::string error = myAES.getLastError();
		return;
	}
	if (!m_name.getText().empty())
		m_name_enc.setText(myAES.encryptBuf2Hex(m_name.getText().c_str(),m_name.getText().length()));
	if (!m_nhsnumber.getText().empty())
		m_nhsnumber_enc.setText(myAES.encryptBuf2Hex(m_nhsnumber.getText().c_str(),m_nhsnumber.getText().length()));
	if (!m_carersname.getText().empty())
		m_carersname_enc.setText(myAES.encryptBuf2Hex(m_carersname.getText().c_str(),m_carersname.getText().length()));
}
*/

std::string patient::encIdentifyingDetail(const std::string &id, std::string &encresult)
{
	if (id.empty())
		return "OK";

	std::string enckey = enckey::getDBEncKey();
	XAES myAES = XAES(enckey);
	if (!myAES.isValid())
	{
		std::string error = myAES.getLastError();
		return ("Error with encrpyting information. If this error persists, contact support.");
	}
	encresult = myAES.encryptBuf2Hex(id.c_str(),id.length());
	return "OK";
}

