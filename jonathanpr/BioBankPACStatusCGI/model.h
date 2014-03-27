//---------------------------------------------------------------------------

#ifndef modelH
#define modelH

#include <sstream>
#include <string>

#include "session.h"
#include "rosetta.h"
#include "dicom.h"
#include "log.h"
#include "unknowndicom.h"
#include "randutil.h"

class XCGI;
class XDB;
class View;

//---------------------------------------------------------------------------
class Model
{
	int pageRead;
	session m_session;
	dicom m_dicomStudies;
	unknowndicom m_unknowndicom;
	PACSlog m_logs;
	bool storeDetailsToDB(XDB *emaildb,XCGI *cgi );
	void updateRosettaDetails( XCGI *cgi);
private:
	std::string m_serilizedRosetta;
	std::string m_EncRosetta;

	int getNextemailidSequence(XDB *emaildb);
	void encryptRosetta( XCGI *cgi,ROSETTA *details);
	void decryptRosetta( XCGI *cgi);
	const XDB *m_db;
	RAND_UTIL *m_ru;
	ROSETTA m_R;
public:
	std::string sd;
	void logResult(XCGI *cgi );

	Model( const XCGI *cgi,const XDB *db,RAND_UTIL *ru );
	void parse( XCGI *cgi);

	void seriliseRosetta( XCGI *cgi);
	View *nextStage( ) const;
	void check(XCGI *cgi );

	std::string getSecurotySessionString() const {return m_ru->getTokenString();}
	bool outputFormToken() const {return m_ru->outputFormToken();} const

	std::string getSerilizedData() const {return m_EncRosetta;}
	void setSerilizedData(const std::string &EncRoss) {m_EncRosetta = EncRoss;}
	const session &getLogin() const { return m_session; }

	const dicom &getDicomStudies()
	{
		if (!m_dicomStudies.isReady())
			m_dicomStudies.readList(m_db);
		return m_dicomStudies;
	}

	const PACSlog &getLogs()
	{
		if (!m_logs.isReady())
			m_logs.readList(m_db);
		return m_logs;
	}

	const unknowndicom &getUnknownDicom()
	{
		if (!m_unknowndicom.isReady())
			m_unknowndicom.readList(m_db);
		return m_unknowndicom;
	}

	int getSumbittedPage()const {return pageRead;}
	const ROSETTA &getRosetta() const {return m_R;}

	const XDB *getDB() const {return m_db;}
};
//---------------------------------------------------------------------------

#endif
