//---------------------------------------------------------------------------

#pragma hdrstop

#include "xcgi.h"
#include "randutil.h"
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
#include "page4.h"
#include "page5.h"
#include "page6.h"

#include "pageIneligible.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

Model::Model(const XCGI *cgi ) :pageRead(cgi->getParamAsIntDefault("page",-1)), m_location(),m_login(),	m_eligibility(),m_clinician(),m_patient(), m_bPatientSavedOK(false)
{

}
// --------------------------------------------------------------------------

void Model::parse( XCGI *cgi)
{
	seriliseRosetta(cgi);

	m_clinician.parse(cgi,m_R);
	m_location.parse( cgi,m_R);
	m_login.parse( cgi,m_R );
	m_eligibility.parse( cgi,m_R );
	m_patient.parse(cgi,m_R);



}

// --------------------------------------------------------------------------
//	add information from database, validate input and check eligibility
// --------------------------------------------------------------------------

void Model::seriliseRosetta( XCGI *cgi)
{
//grab encrypted rosetta, decrypt it
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

void Model::check( XDB *db, RAND_UTIL *ru, XCGI *cgi )
{
	m_login.readList(db);
	m_clinician.readList( db );
	m_location.readList( db );

	m_clinician.checkDetails();
	m_location.checkDetails();

	if (pageRead <= 0)
	{
		m_login.checkUser(ru, db );
		m_clinician.checkLink(m_location, db );
	}
	else if (pageRead == 5)
		m_login.checkUserValidation(ru, db );

	// first page gives user, hospital and patient details
	if( pageRead >= 1 )
	{
		m_patient.checkDetails();
	}
	// second page adds criteria for eligibility
	if( pageRead >= 2 )
		m_eligibility.checkDetails( m_patient );

	//register patient
	if ((m_login.getStage() 					== VALID) &&
		(m_location.getStage() 					== VALID) &&
		(m_clinician.getStage() 				== VALID) &&
		(m_patient.getStageIdentifyingDetails() == VALID) &&
		(m_eligibility.getStage() 				== VALID) &&
		(m_login.getVarificationStage() 		== VALID) &&
		(m_eligibility.getConfirmationStage() 	== VALID))
		m_bPatientSavedOK = storeDetailsToDB(db,ru->getTrialsDB(),cgi);
}

// --------------------------------------------------------------------------
//	record the current state in the database and log file
// --------------------------------------------------------------------------

void Model::logResult( XDB *db, XCGI *cgi )
{
	std::stringstream summary;
	stage us = m_login.getStage( ), ls = m_location.getStage( ), cs = m_clinician.getStage( ) , pscg = m_patient.getStageCareGiver(), psr = m_patient.getStageRisk(), psid = m_patient.getStageIdentifyingDetails();
//	if( us == VALID )
//		summary << "User: " << m_login.getName( );
//	else
//		summary << "Error: " << m_login.getMessage( );

	summary << "centre: " << m_location.getName( ) << " (status=" << ls << ")"
			", PI: " << m_clinician.selectedClinician.getText() << " (status=" << cs << ")"
			", participant: " << m_patient.getname().getText() << " (status Caregiver =" << pscg << ") (status Risk =" << psr << ") (status Ident =" <<  psid << ")";
	summary << ", result: " << m_patient.getrndalloc().getText();
	cgi->log( summary.str( ).c_str( ) );

	updateRosettaDetails(cgi);
}

// --------------------------------------------------------------------------

int Model::getNextemailidSequence(XDB *emaildb)
{
	int c_id_sequence = -1;
	std::string sqlQuery = "SELECT NEXT VALUE FOR email_nextid";
	XQUERY query( emaildb, sqlQuery);
	if (!query.open())
		return c_id_sequence;
	if ( query.fetch() )
		c_id_sequence = query.result.getInt(0);
	query.close();
	return c_id_sequence;
}

// --------------------------------------------------------------------------
//	save to the DB
// --------------------------------------------------------------------------
bool Model::storeDetailsToDB( XDB *db,XDB *emaildb,XCGI *cgi  )
{
//test so see if this is a re-refesh
//this works by the following, when user submits patient name to server
//it is encrpyted. Each encrpytion will generate a different series of HEX as
//output, even for the same name.
//once on the submit page, it checks to see if this encrpyted name is already
//in the database


	std::stringstream sqlquery;

	sqlquery <<  "SELECT count(*) FROM person WHERE name_enc = '" << m_patient.getnameenc().getText() << "'";

	XQUERY query( db, sqlquery.str());
	if (!query.open())
	{
		std::string e = query.getLastError();
		m_PatientSavedOKMsg = std::string("An Error occurred when trying to open the database, try using the back button and re-submitting.<br>If this message persists, contact your systems administrator.");
		cgi->log(m_PatientSavedOKMsg.c_str( ) );
		cgi->log(e.c_str( ) );
		return false;
	}
	if ( query.fetch() )
	{
		int count = query.result.getInt(0);
		if (count > 0)
		{
			query.close();
			m_PatientSavedOKMsg = std::string("Our records show you have already try to register this person.<br>If you are getting this message in error, contact your systems administrator.");
			return false;
		}
	}
	query.close();
	if (!m_patient.doRandomise(db,m_location.getID()))
	{
		m_PatientSavedOKMsg = std::string("There has been an error processing the submitted data by the server, try using the back button and re-submitting.<br>If this message persists, contact your systems administrator.");
		cgi->log("Error in radmonisation");
		return false;
	}

	std::string res = m_patient.getNextPersonID(db);
	if (res != "OK")
	{
		m_PatientSavedOKMsg = std::string("An Error occurred when trying to open the database, try using the back button and re-submitting.<br>If this message persists, contact your systems administrator.");
		cgi->log(m_PatientSavedOKMsg.c_str( ) );
		cgi->log(res.c_str());
		return false;
	}
	std::auto_ptr< ROSETTA > details( new ROSETTA );
	m_login.addForDBFields( *details );
	m_location.addForDBFields( *details );
	m_clinician.addForDBFields( *details );
	m_patient.addForDBFields( *details,m_clinician.getName(),m_clinician.getTelNumber() );
	m_eligibility.addForDBFields( *details );

// get email stuff here
	std::stringstream ss;

	std::stringstream StudyNoString;
	if (m_location.getID() < 10)
		StudyNoString << "0";
	StudyNoString << m_location.getID();

	int personID = m_patient.getPersonID();
	if (personID < 10)
		StudyNoString << "0";
	if (personID < 100)
		StudyNoString << "0";
	StudyNoString << personID;

	ss << "Dear " << m_clinician.getName() << "\n\nThis is to confirm that a new person has been entered into the ATTILA study, with details as follows:\n\n";
	ss << "Day/month of birth: " << m_patient.getname().getText() << "\n";
	ss << "ATTILA Study No: " << StudyNoString.str() << "\n";
	ss << "Entry date: " << m_patient.getrnddate().getText() << "\n";
	if (m_patient.getrndalloc().igetText() == 0)
		ss << "Randomisation Allocation: Control\n\n";
	else
		ss << "Randomisation Allocation: Intervention\n\n";
	if (m_patient.getoperatorname().getText().length() > 0)
		ss << "This person was entered by " << m_patient.getoperatorname().getText();
	else
		ss << "This person was entered by " << m_clinician.getName();

	ss << "This email has been auto generated by ATTILA Randomisation web site.\n";
	ss << "If you have any queries then please contact ?@ndph (tel: )\n";

	int emailid = getNextemailidSequence(emaildb);

	if (emailid == -1)
	{
		m_PatientSavedOKMsg = std::string("There has been an error processing the submitted data by the server, try using the back button and re-submitting.<br>If this message persists, contact your systems administrator.");
		cgi->log(m_PatientSavedOKMsg.c_str( ) );
		cgi->log(emaildb->getLastError().c_str() );
		return false;
	}

	std::auto_ptr< ROSETTA > email( new ROSETTA );
	email->setInt("email_id",emailid);
	email->setString("owner_ident","ERG-ATTILA");
	email->setString("subj","ATTILA randomisation notification");
	email->setString("body",ss.str());
	email->setString("ad_to",m_clinician.getEmail());
	email->setString("ad_sender","jonathan.price@ctsu.ox.ac.uk");
	email->setString("ad_reply","jonathan.price@ctsu.ox.ac.uk");
	email->setString("ad_cc","");
	email->setString("ad_bcc","");
	email->setString("notes","");
	email->setString("auth","ATTILA");
	email->setInt("node_id",0); //TEST MODE, NO SENDING

	db->setAutoCommit(false);
	emaildb->setAutoCommit(false);

	XEXEC exePerson( db);
	if (!exePerson.insert( "person", details.get() ))
	{
		db->rollback();
		db->setAutoCommit(true);
		emaildb->setAutoCommit(true);
		m_PatientSavedOKMsg = std::string("There has been an error processing the submitted data by the server, try using the back button and re-submitting.<br>If this message persists, contact your systems administrator.");
		cgi->log(m_PatientSavedOKMsg.c_str( ) );
		cgi->log(exePerson.getLastError().c_str() );
		return false;
	}

	XEXEC exeEmail(emaildb);
	if (!exeEmail.insert("email_out",email.get()))
	{
		db->rollback();
		emaildb->rollback();
		db->setAutoCommit(true);
		emaildb->setAutoCommit(true);
		m_PatientSavedOKMsg = std::string("There has been an error processing the submitted data by the server, try using the back button and re-submitting.<br>If this message persists, contact your systems administrator.");
		cgi->log(m_PatientSavedOKMsg.c_str( ) );
		cgi->log(exeEmail.getLastError().c_str() );
		return false;
	}
/*
//		XEXEC exeEmail(emaildb);
//		std::stringstream sql;
	//sql << "INSERT INTO email_out(email_id,owner_ident,subj,body,ad_to,ad_cc,ad_bcc,ad_sender,ad_reply,notes,auth)";
	//sql << " VALUES(email_nextid.nextval,'ERG-ATTILA','ATTILA randomisation notification','" << ss.str() << "'",'jonathan.price@ctsu.ox.ac.uk','','','jonathan.price@ctsu.ox.ac.uk','jonathan.price@ctsu.ox.ac.uk','','ATTILA')";
//		sql << "INSERT INTO email_out(email_id,owner_ident,subj,body,ad_to,ad_cc,ad_bcc,ad_sender,ad_reply,notes,auth)";
//		sql << " VALUES(email_nextid.nextval,'ERG-ATTILA','ATTILA randomisation notification','" << ss.str() << "'",'jonathan.price@ctsu.ox.ac.uk','','','jonathan.price@ctsu.ox.ac.uk','jonathan.price@ctsu.ox.ac.uk','','ATTILA')";
//		XEXEC exeEmail(ru->getTrialsDB(),sql);

	if (!exeEmail.exec())
	{
		db->rollback();
		emaildb->rollback();
		m_PatientSavedOKMsg = std::string("There has been an error processing the submitted data by the server, try using the back button and re-submitting.<br>If this message persists, contact your systems administrator.");
		cgi->log(m_PatientSavedOKMsg.c_str( ) );
		cgi->log(exeEmail.getLastError().c_str() );
		return false;
	}
*/	db->commit();
	emaildb->commit();
	db->setAutoCommit(true);
	emaildb->setAutoCommit(true);
	return true;
}

// ---------------------------------------------------------------------------
//	update the rosetta so it can be passed back
// ---------------------------------------------------------------------------
void Model::updateRosettaDetails( XCGI *cgi)
{
	std::auto_ptr< ROSETTA > details( new ROSETTA );

	m_login.addFields( *details );
	m_location.addFields( *details );
	m_clinician.addFields( *details );
	m_patient.addFields( *details );
	m_eligibility.addFields( *details );
	details->setString( "tupdate", "now" );
	encryptRosetta(cgi,details.get());
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
		return; //some problem with the rosetta, it's got current or been played with

	std::string s((const char*)clr,length);
	m_serilizedRosetta = s;
}

// --------------------------------------------------------------------------
//	work out which page to display next: page3 shows the final outcome
// --------------------------------------------------------------------------
const View *Model::nextStage( ) const
{
	if (m_clinician.getStage() != VALID || m_location.getStage() != VALID || m_login.getStage() != VALID)
		return new page0(*this);
	if (m_patient.getStageIdentifyingDetails() != VALID)
		return new page1(*this);
	if (m_eligibility.getStage() == INCOMPLETE)
		return new page2(*this);
	if (m_eligibility.getStage() == INELIGIBLE)
		return new pageIneligible(*this);
	if (m_patient.getStageCareGiver() != VALID)
		return new page3(*this);
	if (m_patient.getStageRisk() != VALID)
		return new page4(*this);
	if (!m_patient.hasInformedConsent())
		return new pageIneligible(*this);
	if ((m_eligibility.getConfirmationStage()!= VALID) || (m_login.getVarificationStage() !=  VALID))
		return new page5(*this);
	return new page6(*this);
}

// --------------------------------------------------------------------------

