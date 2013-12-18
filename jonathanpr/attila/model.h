//---------------------------------------------------------------------------

#ifndef modelH
#define modelH

#include <sstream>
#include <string>

#include "centre.h"
#include "clinician.h"
#include "patient.h"
#include "checklist.h"
#include "session.h"
#include "rosetta.h"

class XCGI;
class XDB;
class View;
class RAND_UTIL;

//---------------------------------------------------------------------------
class Model
{
	int pageRead;
	session m_login;
	centre m_location;
	clinician m_clinician;
	patient m_patient;
	checklist m_eligibility;
	bool storeDetailsToDB( XDB *db,XDB *emaildb,XCGI *cgi );
	void updateRosettaDetails( XCGI *cgi);
private:
	std::string m_serilizedRosetta;
	std::string m_EncRosetta;
	bool m_bPatientSavedOK;
	std::string m_PatientSavedOKMsg;

	int getNextemailidSequence(XDB *emaildb);
	void encryptRosetta( XCGI *cgi,ROSETTA *details);
	void decryptRosetta( XCGI *cgi);

	ROSETTA m_R;
public:
	std::string sd;

	Model( const XCGI *cgi );
	void parse( XCGI *cgi);

	bool hasPatientSavedOK() const {return m_bPatientSavedOK;}
	std::string getPatientSavedOKMsg() const {return m_PatientSavedOKMsg;}

	void seriliseRosetta( XCGI *cgi);
	const View *nextStage( ) const;
	void check( XDB *db, RAND_UTIL *ru, XCGI *cgi );
	void logResult( XDB *db, XCGI *cgi );

	std::string getSerilizedData() const {return m_EncRosetta;}
	void setSerilizedData(const std::string &EncRoss) {m_EncRosetta = EncRoss;}

	const session &getLogin() const { return m_login; }
	const patient &getPatient( ) const { return m_patient; }
	const centre &getCentre( ) const { return m_location; }
	const clinician &getClinicians( ) const { return m_clinician; }
	const checklist &getEligibility( ) const { return m_eligibility; }
	int getSumbittedPage()const {return pageRead;}
	const ROSETTA &getRosetta() const {return m_R;}

};

//---------------------------------------------------------------------------




#endif
