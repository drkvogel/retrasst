//---------------------------------------------------------------------------

#ifndef checklistH
#define checklistH
//---------------------------------------------------------------------------
#include <string>
#include "stage.h"

class patient;
class XCGI;
class ROSETTA;

//---------------------------------------------------------------------------

class checklist
{
	void checkLateSchiz( const patient & person );
public:

	checklist(/* const XCGI *cgi */);
	void checkDetails( const patient &person );
	stage getStage() const;
	stage getConfirmationStage() const;
	void addFields( ROSETTA & fields ) const;
	void addForDBFields( ROSETTA & fields ) const;
    void parse(const XCGI *cgi,const ROSETTA &R);
private:
	std::string m_error;

	std::string m_DOD;
//	std::string m_ATGIC;
	std::string m_FACS;
	std::string m_LIOD;
	std::string m_WTLCTH;
	std::string m_APATT;
	std::string m_UMORC;
	std::string m_PIACTFADI;
	std::string m_DETAILSCONFIRMED;

	bool ineligible, incomplete;

	void addToRosetta(ROSETTA & fields,const std::string &name,const std::string &var) const;
	void readDetails(const ROSETTA & fields );
	void getDetailsConfirmed(const XCGI *cgi);
public:
	std::string getMessage() const { return m_error; }

	int getOptionValue(std::string value) const
	{
		if (value == "0") return 0;
		if (value == "1") return 1;
		return -1;
	}

	std::string getOptionString(std::string value) const
	{
		if (value == "0") return "YES";
		if (value == "1") return "NO";
		return "";
	}

	const std::string getDODString() const {return getOptionString(m_DOD);}
//	const std::string getATGICString() const {return getOptionString(m_ATGIC);}
	const std::string getFACSString() const {return getOptionString(m_FACS);}
	const std::string getLivingOrginaryDwellingString() const {return getOptionString(m_LIOD);}
	const std::string getWorkingTelephoneString() const {return getOptionString(m_WTLCTH);}
	const std::string getProvidedATTString() const {return getOptionString(m_APATT);}
	const std::string getUnstableString() const {return getOptionString(m_UMORC);}
	const std::string getAnotherTrialString() const {return getOptionString(m_PIACTFADI);}
	const std::string getDetailsConfirmedString() const {return getOptionString(m_DETAILSCONFIRMED);}


	const int getDOD() const {return getOptionValue(m_DOD);}
//	const int getATGIC() const {return getOptionValue(m_ATGIC);}
	const int getFACS() const {return getOptionValue(m_FACS);}
	const int getLivingOrginaryDwelling() const {return getOptionValue(m_LIOD);}
	const int getWorkingTelephone() const {return getOptionValue(m_WTLCTH);}
	const int getProvidedATT() const {return getOptionValue(m_APATT);}
	const int getUnstable() const {return getOptionValue(m_UMORC);}
	const int getAnotherTrial() const {return getOptionValue(m_PIACTFADI);}
	const int getDetailsConfirmed() const {return getOptionValue(m_DETAILSCONFIRMED);}
};

//---------------------------------------------------------------------------
#endif
