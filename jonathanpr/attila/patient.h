//---------------------------------------------------------------------------

#ifndef patientH
#define patientH
//---------------------------------------------------------------------------


#include <string>
#include "stage.h"
#include "result.h"

class XDB;
class XQUERY;
class XCGI;
class ROSETTA;

//---------------------------------------------------------------------------

class patient
{
public:

	patient();
	void checkDetails();
	stage getStageCareGiver() const { return stageCareGiver; }
	stage getStageRisk() const { return stageRisk; }
	stage getStageIdentifyingDetails() const { return stageIdentifyingDetails; }
	void setPID( XDB *db );
	void readDetails(const ROSETTA & fields );
	void addFields( ROSETTA & fields ) const;
	void addForDBFields( ROSETTA & fields,const std::string &clinicianName,const std::string &clinicianNumber ) const;
	void parse(const XCGI *cgi,const ROSETTA &R);
private:

	stage stageCareGiver;
	stage stageRisk;
	stage stageIdentifyingDetails;

	std::string encIdentifyingDetail(const std::string &id, std::string &encresult);

	int getAgeGroupFromDOB(const std::string &dobstring) const;
	void setFromRosetta(const ROSETTA &fields,const std::string &name,Result &var) const;
	void addToRosetta(ROSETTA & fields,const std::string &name,const Result &var) const;
	void getCareGiverDetails(const XCGI *cgi );
	void getGenderDetails(const XCGI *cgi );

public:
	std::string getNextPersonID(XDB *db);
	bool doRandomise(XDB *db,int centre_id);
//    void encIdentifyingDetails(void);
	bool hasInformedConsent() const {return (m_informedconsent.getText() != "3");}

	std::string getSupportText(const std::string &id) const;
	int getIDFromSupportText(const std::string &text) const;
	int getIDFromLiveWith(const std::string &text) const;

	const Result &getname( ) const { return m_name; }
	const Result &getdob( ) const { return m_dob; }
	const Result &getgender( ) const { return m_gender; }
	const Result &gethospitalno( ) const { return m_hospitalno; }
	const Result &getnhsnumber( ) const { return m_nhsnumber; }
	const Result &getgp( ) const { return m_gp; }
	const Result &getcaregiversupport( ) const { return m_caregiversupport; }
	const Result &getcarersname( ) const { return m_carersname; }
	const Result &getcarersname_enc() const { return m_carersname_enc;}
	const Result &getcarerdob( ) const { return m_carerdob; }
	const Result &getstudyno( ) const { return m_studyno; }
	const Result &getrnddate( ) const { return m_rnddate; }
	const Result &getrndalloc( ) const { return m_rndalloc; }
	const Result &getoperatorname( ) const { return m_operatorname; }
	const Result &getoperatortelnum( ) const { return m_operatortelnum; }
	const Result &getrssprofessional( ) const { return m_rssprofessional;}
	const Result &getnameenc() const {return m_name_enc;}
	const Result &getlivewith() const {return m_livewith;}

	const Result &getriskwandering( ) const { return m_riskwandering;}
	const Result &getriskhome( ) const { return m_riskhome;}
	const Result &getinformedconsent( ) const { return m_informedconsent; }
	std::string getRiskWanderingString() const
	{
		if (m_riskwandering.getText() == "0")
			return "Low";
		if (m_riskwandering.getText() == "1")
			return "Moderate";
		if (m_riskwandering.getText() == "2")
			return "High";
		return "";
	}
	std::string getRiskHomeString() const
	{
		if (m_riskhome.getText() == "0")
			return "Low";
		if (m_riskhome.getText() == "1")
			return "Moderate";
		if (m_riskhome.getText() == "2")
			return "High";
		return "";
	}
	std::string getInformedConsentString() const
	{
		if (m_informedconsent.getText() == "0")
			return "From patient";
		if (m_informedconsent.getText() == "1")
			return "From carer";
		if (m_informedconsent.getText() == "2")
			return "From Consultee";
		if (m_informedconsent.getText() == "3")
			return "None";
		return "";
	}
	std::string getCaregiverDetailsString() const
	{
		if (m_caregiversupport.getText() == "0")
			return "Live-in caregiver";
		else if (m_caregiversupport.getText() == "1")
			return "Caregiver visits at least once/day";
		else if (m_caregiversupport.getText() == "2")
			return "Caregiver visits less than once/day";
		return "None";
	}

	int getPersonID() {return m_person_id;}

private:

	Result 		m_name;
	Result 		m_name_enc;
	Result  	m_dob;
	Result  	m_gender;
	Result  	m_hospitalno;
	Result  	m_nhsnumber;
	Result  	m_nhsnumber_enc;
	Result 		m_gp;
	Result 		m_rssprofessional;

	Result 		m_caregiversupport;
	Result		m_livewith;
	Result 		m_carersname;
	Result 		m_carersname_enc;
	Result  	m_carerdob;

	Result  	m_riskwandering;
	Result  	m_riskhome;
	Result  	m_informedconsent;

	Result  	m_studyno;
	Result  	m_rnddate;
	Result  	m_rndalloc;

	Result 		m_operatorname;
	Result 		m_operatortelnum;

	int 	m_person_id;
	std::string 	m_error;
	std::string 	getMessage() const { return m_error; }
};

//---------------------------------------------------------------------------
#endif
