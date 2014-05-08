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
static XCGI *cgi = NULL;
static XDB	*db = NULL;
static RAND_UTIL *ru = NULL;
//---------------------------------------------------------------------------

//SIMPLE CGI program which takes commands from the JavaScript on the Status web site.
//The scope of this is to updating the details of the unknown Dicom Studies so they can be
//matched with the Biobank Id's. Studies can also be ignored, where they take no more part in the system.

//Send a message back to the JavaScript.
static bool jssendMessage(const std::string &Msg)
{
	std::string serilisedHex;
	if (!XENCODE::szToHex(Msg, &serilisedHex))
	{
		printf( "Error encoding hex string" );
		fflush( stdout );
		return false;
	}
	printf(serilisedHex.c_str());
	fflush( stdout );
	return true;
}

//helper function to convert a string to an long
static long stringToLong(const std::string &pk)
{
	std::stringstream str(pk);
	long ipk;
	str >> ipk;
	if (!str)
		return -1;
	return ipk;
}

//set the status of a study to 3. (IGNORE DICOM STUDY)
void ignoreDicom(const std::string &studyPK)
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

//updates the Dicom Record's username, studyPK.
static void updateDicomRecord(const std::string &studyPK,const std::string &newPID,const std::string &newFN)
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

// we are expecting mode, which will either be equal
//  1) ignoreStudy - which will set the study status to ignore
//  2) updateStudy - Which will update the firstname and participent id
//User must be signed in for this to accept any data.

//input is valided in the Javascript. But being a good egg, we re-validate it again
static void doBody( void )
{
	std::string	modeData = "";
	if ( cgi->paramExists( "mode" ) )
	{
/*		if (!ru->hasToken())
		{
			jssendMessage("User not authenticated");
			return;
		}
*/
		std::string studyPK;
		modeData = cgi->getParam( "mode" );
		if (modeData == "ignoreStudy")
		{
			studyPK = cgi->getParam( "pk" );
			ignoreDicom(studyPK);
			return;
		}
		else if (modeData == "updateStudy")
		{
			studyPK = cgi->getParam( "pk" );
			std::string	newPID = cgi->getParam( "pid" );
			std::string newFN = cgi->getParam( "fn" );

			std::string ErrorString = "";
			if (newPID.length() != 8)
				ErrorString = "Participant id needs to be 8 characters in length.<br>";
			if (newFN.length() != 8)
				ErrorString += "First name needs to be 8 characters in length.<br>";

			if (!ErrorString.empty())
			{
				jssendMessage(ErrorString);
				return;
			}
  /* This is no longer the case, leaving in, it may get changed again
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
			updateDicomRecord(studyPK,newPID,newFN);
			return;
		}
	}
	jssendMessage("Unknown command");
	return;
}

//---------------------------------------------------------------------------
int main( int argc, char **argv )
{
	cgi = new XCGI( argc, argv );
	ru = new RAND_UTIL( cgi );
#ifndef __BORLANDC__
	db = ru->openDB( "biocore_pacs" );
#else
//	db = ru->openDB( "red::biocore_pacsclinicdata" );
//	db = ru->openDB( "biocore_bb5::biocore_pacsclinicdata" );
	db = ru->openDB( "biocore_bb5::biocore_pacs" );
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
			jssendMessage( "Unable to connect to database" );
	}
	printf( "</BODY></HTML>\n");

	if (ru)
		delete ru;
	cgi->end();
	delete cgi;
	return( EXIT_SUCCESS );
}
//---------------------------------------------------------------------------

