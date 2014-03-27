#include <stdlib.h>
#include "xcgi.h"
#include "xdb.h"
#include "xexec.h"
#include "randutil.h"
#include "xencode.h"
#include "openers.h"
#include "xquery.h"
#include <sstream>
//---------------------------------------------------------------------------
XCGI *cgi = NULL;
XDB	*db = NULL;
RAND_UTIL *ru = NULL;
//---------------------------------------------------------------------------

static bool processOutput(ROSETTA &R)
{
	std::string out = R.serializeOut();

	std::string serilisedHex;
	if (!XENCODE::szToHex(out, &serilisedHex))
	{
		printf( "Error encoding hex string" );
		fflush( stdout );
		return false;
	}

	//encrypt...
	printf(serilisedHex.c_str());
	fflush( stdout );
	return true;
}

static bool jssendMessage(const std::string &Msg)
{
	std::string serilisedHex;
	if (!XENCODE::szToHex(Msg, &serilisedHex))
	{
		printf( "Error encoding hex string" );
		fflush( stdout );
		return false;
	}

	//encrypt...
	printf(serilisedHex.c_str());
	fflush( stdout );
	return true;
}

static bool sendError(const std::string &Error)
{
	ROSETTA R;
	ROSETTA RContainer;

	R.setString(std::string("Msg"),Error);
	RContainer.setRosetta(std::string("ERROR"),&R);
	return processOutput(RContainer);
}

static bool sendOK()
{
	ROSETTA R;
	ROSETTA RContainer;

	R.setString(std::string("Msg"),std::string("OK"));
	RContainer.setRosetta(std::string("OK"),&R);
	return processOutput(RContainer);
}

void signIn(ROSETTA *R)
{
	std::string queryString = "SELECT pacsuser,pacspassword from staff where name = :name AND password = :password";

	XQUERY q(db,queryString);
	q.setParamSource(R);

	ROSETTA output;
	if (!q.fetchSingle(&output))
	{
		sendError("Bad username or password");
		q.close();
		return;
	}
	q.close();

	ROSETTA RContainer;
	RContainer.setRosetta(std::string("OK"),&output);
	processOutput(RContainer);
 }

long stringToLong(const std::string &pk)
{
	std::stringstream str(pk);
	long ipk;
	str >> ipk;
	if (!str)
		return -1;
	return ipk;
}

//set status to 3. (INGORE DICOM STUDY)
void ingoreDicom(const std::string &studyPK)
{
	bool insertedOK = false;

	long ipk = stringToLong(studyPK);
	if (ipk == -1)
	{
		jssendMessage("Error - StudyPK not an int");
		return;
	}
	ROSETTA R;
	R.setInt("status",3);
	ROSETTA W;
	W.setLint("studypk",ipk);
	XEXEC ex(db);
	if (ex.update("pacsstudy",&R,&W))
		insertedOK = true;

	if (!insertedOK)
		jssendMessage("Error inserting into database: " + ex.getLastError());
	else
		jssendMessage("OK");
}

void updateDicomRecord(const std::string &studyPK,const std::string &newPID,const std::string &newFN)
{
	bool insertedOK = false;

	long ipk = stringToLong(studyPK);
	if (ipk == -1)
	{
		jssendMessage("Error - StudyPK not an int");
		return;
	}

	ROSETTA R;
	R.setString("confirmedpatientid",newPID);
	R.setString("firstname",newFN);
	R.setInt("status",1);
	ROSETTA W;
	W.setLint("studypk",ipk);
	XEXEC ex(db);
	if (ex.update("pacsstudy",&R,&W))
		insertedOK = true;

	if (!insertedOK)
		jssendMessage("Error inserting into database: " + ex.getLastError());
	else
		jssendMessage("OK");
}

void insertDicomRecord(const ROSETTA &R)
{
	bool insertedOK = false;
	XEXEC ex(db);
	if (!ex.insert("dicompacs",&R))
	{
		//if can't insert.. lets over write what is there
		ROSETTA W;
		W.setString("patientid",R.getString("patientid"));
		W.setString("studyid",R.getString("studyid"));
		if (ex.update("dicomrecord",&R,&W))
			insertedOK = true;
	}
	if (!insertedOK)
		sendError("Error inserting into database: " + ex.getLastError());
	else
		jssendMessage("OK");
}

static bool processInput(ROSETTA &R)
{
	std::string	resHexData = "";

	if ( cgi->paramExists( "rosetta" ) )
		resHexData = cgi->getParam( "rosetta" );
	else
	{
		sendError( "Error, No data" );
		return false;
	}

	std::string unserilisedHex;
	if (!XENCODE::hexToSz(resHexData.c_str(),&unserilisedHex))
	{
		sendError( "Error, unable to unhex string" );
		return false;
	}

	R.serializeIn(unserilisedHex,true);
	if (!R.isValid())
	{
		sendError("Unable to serilise Rosetta");
		return false;
	}
	return true;
}


static void doBody( void )
{
//either will be a rosetta or will have a mode
//Rosetta's currently aren't being used
	std::string	modeData = "";

	if ( cgi->paramExists( "mode" ) )
	{
		if (!ru->hasToken())
		{
			jssendMessage("User not authicated");
			return;
		}
		std::string studyPK;
		modeData = cgi->getParam( "mode" );
		if (modeData == "ingoreStudy")
		{//MUST VALIDATE USER AND STUFF HERE
			studyPK = cgi->getParam( "pk" );
			ingoreDicom(studyPK);
			return;
		}
		else if (modeData == "updateStudy")
		{//MUST VALIDATE USER AND STUFF HERE
			studyPK = cgi->getParam( "pk" );
			std::string	newPID = cgi->getParam( "pid" );
			std::string newFN = cgi->getParam( "fn" );

			std::string ErrorString = "";
			if (newPID.length() != 8)
				ErrorString = "Participant id needs to be 8 characters in length<br>";
			if (newFN.length() != 8)
				ErrorString += "First name needs to be 8 characters in length<br>";

			if (!ErrorString.empty())
			{
				jssendMessage(ErrorString);
				return;
			}
  /*
			std::stringstream rev;
			for (int i=7;i>=0;i--)
				rev << newPID[i];

			std::string revPID = rev.str();

			if (newFN != revPID)
				ErrorString = "The first name needs to be the reverse of the participant id!!!<br>";

			if (!ErrorString.empty())
			{
				jssendMessage(ErrorString);
				return;
			}
   */
//Now check to see if this patient is known and in our db?

			updateDicomRecord(studyPK,newPID,newFN);
			return;
		}
	}
	jssendMessage("Unknown command");
	return;
/*
	ROSETTA R;
	if (!processInput(R))
		return;

	//lets see what is data is about
	if (R.isRosetta("insertDicomRecord"))
	{
		insertDicomRecord(R.getRosetta("insertDicomRecord"));
	}
	else if (R.isRosetta("signin"))
	{
		signIn(R.getRosetta("signin"));
	}
*/
}

//---------------------------------------------------------------------------
int main( int argc, char **argv )
{
	cgi = new XCGI( argc, argv );
	ru = new RAND_UTIL( cgi );
#ifndef __BORLANDC__
	db = ru->openDB( "biocore_pacsclinicdata" );
#else
//	db = ru->openDB( "red::biocore_pacsclinicdata" );
//	db = ru->openDB( "biocore_bb5::biocore_pacsclinicdata" );
	db = ru->openDB( "bb5_extract::biocore_pacs" );
#endif
	printf("Content-type: text/html\n\n");
	printf("<HTML><BODY>\n");

	if ( ru->isValid() )
	{
		if ( NULL != db )
		{
			doBody();
			delete db;
		}
		else
			sendError( "Unable to connect to database" );
	}

	printf( "</BODY></HTML>\n");

	if (ru)
		delete ru;
	cgi->end();
	delete cgi;
	return( EXIT_SUCCESS );
}
//---------------------------------------------------------------------------

