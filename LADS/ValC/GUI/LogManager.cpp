//---------------------------------------------------------------------------

#pragma hdrstop

#include <fmx.h>

#include "AcquireCriticalSection.h"

#include "LogManager.h"
#include "Utils.h"
#include "TLogFrame.h"

#pragma package(smart_init)

//--------------- begin LogManager class ------------------------------------


std::string LogManager::FLAG_GUI = "<><>";
std::string LogManager::FLAG_EXCEPTION = "/!\\ ";

/** Initialises the logging of ValC. This includes setting up a logging service
  * (see paulst::LoggingService) that handles the threading of requests,
  * timestamping the beginning of the log, and creating a logging window
  * if required (see TloggingForm).
  *
  * A LogManager object is expected to be a singleton and last for
  * the lifetime of a single run of ValC.
  *
  * For details of how the logging messages are communicated to the
  * logging window, see TloggingForm.
  *
  * @param m            a reference back to the main form
  * @param logWin       specifies whether a logging window is required or not
  */
LogManager::LogManager(TLogFrame *m, const std::string& logFilePath)
{
	// now setting up the writer for text log file (and maybe GUI) output
	GUIandLogWriter *logWriter
		= new GUIandLogWriter( m, logFilePath);

	// now setting up the logging service used for writing log messages
	// (logging service now owns logWriter and will delete it)
	logService = new paulst::LoggingService(logWriter);

	timestampLog();
}


/* Initialises the log. This involves timestamping the text log file, and
 * also using the timestamp for the title of the logging window, if in use.
 */
void LogManager::timestampLog()
{
	// fetch a timestamp for the current time (used for labelling the logging)
	System::TTime t = System::TDateTime::CurrentTime();
	UnicodeString u = t.FormatString("hh:mm");
	logTimestamp = Utils::unicodestr2str(u);

	log("************************ Run at time: " + logTimestamp);
}


/** Deletes the logging service and the logging window form, for which
  * the LogManager object has responsibility.
  */
LogManager::~LogManager()
{
	// no deletion of the GUIandLogWriter here,
	// as this is owned by the logging service,
	// which has responsibility for deleting it
	delete logService;
}

/** Sends the given message to the logging service (paulst::LoggingService),
  * adding on a prefix that will flag it up as having come from the GUI.
  * (The business layer will not use this method, but will instead just
  * use the logging service directly.)
  *
  * @param msg     the message to be logged
  */
void LogManager::log(const std::string & msg)
{
	std::string s = LogManager::FLAG_GUI;
	s += msg;
	logService->log(s);
}

/** Sends the given message to the logging service, adding on a prefix that
  * will flag it up as an exception (and thus can be displayed suitably).
  *
  * @param msg     the message to be logged
  */
void LogManager::logException(const std::string & msg)
{
	std::string s = LogManager::FLAG_EXCEPTION;
	s += msg;
	log(s);
}

void LogManager::logException(const Exception& e)
{
    std::string msg = AnsiString( e.Message.c_str() ).c_str();
    logException(msg);
}


//--------------- end LogManager class --------------------------------------
//--------------- begin GUIandLogWriter class ------------------------------------

/** Deletes its side-kick, the fileWriter, which it has responsibility for. */
GUIandLogWriter::~GUIandLogWriter()
{
	delete fileWriter;
}

/** Sets up references to the log manager, the message queue owned by the
  * logging window, and a reference to the method that processes messages
  * from said queue. Also creates a Writer object able to write to a text
  * log file.
  *
  * @param q          a reference to the logging window
  * @param filename   the name of the text log file to write to
  */
GUIandLogWriter::GUIandLogWriter( TLogFrame *q,
								 const std::string & filename)
	: msgQueue(q)
{
	fileWriter = new paulst::FileWriter(filename);
}


/** Parses the given string, then writes the log message it contains to the
  * log file, also possibly sending it to the logging window, if in use.
  *
  * Parsing of the string involves taking offer any special strings that
  * have been affixed to it.
  * The first prefix that might be there is LogManager::FLAG_GUI
  * which indicates that the message came from the GUI part of the code.
  * The second prefix that might be there is LogManager::FLAG_EXCEPTION
  * which indicates that the message should be displayed with highlighting.
  *
  * Sending the message to the logging window involves sending it to the
  * message queue owned by the logging window (see TloggingForm).
  *
  * @param msg    the message to be logged, including (possibly) additional
  *               prefixes describing display characteristics
  */
void GUIandLogWriter::write(const std::string & msg)
{
	std::string m = msg;
	bool highlightingRequired = false;
	bool guiOrigin = false;

	if (m.find(LogManager::FLAG_GUI)==0) { // does it have FLAG_GUI prefix?
		m = m.substr(LogManager::FLAG_GUI.size());
		guiOrigin = true;
	}
	if (m.find(LogManager::FLAG_EXCEPTION)==0) {  // does it have FLAG_EXCEPTION prefix?
		m = m.substr(LogManager::FLAG_EXCEPTION.size());
		highlightingRequired = true;

    }
	fileWriter->write(m);  // writes to the log file, regardless

    LabelledMessage lm;
    lm.purpose = guiOrigin ? LabelledMessage::PURPOSE_GUI
                           : LabelledMessage::PURPOSE_BUSINESS_LAYER;
    lm.highlight = highlightingRequired;
    lm.message = m;

    msgQueue->queueMessage(lm);
}

