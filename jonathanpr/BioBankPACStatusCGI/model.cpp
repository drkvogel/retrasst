//---------------------------------------------------------------------------
#pragma hdrstop

#include "xcgi.h"
#include "xdb.h"
#include "xaes.h"
#include "xexec.h"
#include "xquery.h"
#include "xencode.h"

#include "model.h"
#include "enckey.h"
#include "page0.h"
#include "page1.h"
#include "page2.h"
#include "page3.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

Model::Model(const XCGI *cgi,const XDB *db,RAND_UTIL *ru) : pageRead(cgi->getParamAsIntDefault("page",-1)), m_session()
{
	m_db = db;
	m_ru = ru;
}
// --------------------------------------------------------------------------

//Pass around the incoming web url so it can be parsed
void Model::parse( XCGI *cgi)
{
	seriliseRosetta(cgi);
	m_session.parse( cgi,m_R );
	m_dicomStudies.parse(cgi,m_R );
	m_logs.parse(cgi,m_R);
	m_unknowndicom.parse(cgi,m_R);
}

// --------------------------------------------------------------------------
void Model::seriliseRosetta( XCGI *cgi)
{
//grab the encrypted rosetta from the URL stream and decrypt it
	m_EncRosetta = cgi->getParam( "rosetta" );
	m_serilizedRosetta = "";

	if ((!m_EncRosetta.empty()))
	{
		decryptRosetta(cgi);
		if (m_serilizedRosetta.length() > 0) //was there anything to decrypt?
		{
			m_R.clear();
			if (!m_R.serializeIn(m_serilizedRosetta,true))
				m_serilizedRosetta = "";
		}
	}
}
// --------------------------------------------------------------------------
//load in the required data from the database
void Model::check(XCGI *cgi )
{
	m_session.readList(m_db);
	if (m_session.getStage() != VALID)
		m_session.checkUser(m_ru, m_db );

	m_logs.readList(m_db);
	m_unknowndicom.readList(m_db);
}

// ---------------------------------------------------------------------------
//	update the rosetta so it can be encrpyted and kept with the page and passed back to the client
void Model::updateRosettaDetails( XCGI *cgi)
{
	ROSETTA *details = new ROSETTA();

	m_session.addFields( *details );
	m_dicomStudies.addFields( *details );
	m_logs.addFields( *details );
	m_unknowndicom.addFields( *details );
	details->setString( "tupdate", "now" );
	encryptRosetta(cgi,details);
	delete details;
}

// --------------------------------------------------------------------------
void Model::encryptRosetta( XCGI *cgi,ROSETTA *details)
{
	std::string serializedRosetta = details->serializeOut();
	std::string enckey = enckey::getROSSETTAEncKey();
	XAES myAES = XAES(enckey);
	if (!myAES.isValid())
	{
		std::string error = myAES.getLastError();
		cgi->log((std::string("Error with AES:") + error).c_str());
		return;
	}
	m_EncRosetta = myAES.encryptBuf2Hex(serializedRosetta.c_str(),serializedRosetta.length());
}

// --------------------------------------------------------------------------
void Model::decryptRosetta( XCGI *cgi)
{
	m_serilizedRosetta = "";

	std::string enckey = enckey::getROSSETTAEncKey();
	XAES myAES = XAES(enckey);
	if (!myAES.isValid())
	{ //some problem with the encrpytion
		std::string error = myAES.getLastError();
		cgi->log((std::string("Error with AES:") + error).c_str());
		return;
	}
	int length = 0;
	unsigned char *clr;
	if (!myAES.decryptHex(&clr, &length,m_EncRosetta.c_str()))
		return; //some problem with the rosetta, it's got currpted or been played with

	std::string s((const char*)clr,length);
	m_serilizedRosetta = s;
}

// --------------------------------------------------------------------------
//	work out which page to display based on the state
View *Model::nextStage( ) const
{
//has page got a sign on token? If not, no matter what the state,
//go to sign on page

//	if (!m_ru->hasToken())
//		return new page0(*this);
//if not signed on, sign on page
 //	if (m_session.getStage() != VALID)
 //		return new page0(*this);
//now return one of the other pages.
	if (m_session.getViewPage() == "1")
		return new page1(*this);
	if (m_session.getViewPage() == "2")
		return new page2(*this);
	if (m_session.getViewPage() == "3")
		return new page3(*this);
	if (pageRead == 1)
		return new page1(*this);
	if (pageRead == 2)
		return new page2(*this);
	if (pageRead == 3)
		return new page3(*this);
	return new page1(*this);
}
// --------------------------------------------------------------------------

void Model::logResult( XCGI *cgi )
{
	updateRosettaDetails(cgi);
}
// --------------------------------------------------------------------------

