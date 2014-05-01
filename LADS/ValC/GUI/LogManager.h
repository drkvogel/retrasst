//---------------------------------------------------------------------------

#ifndef LogManagerH
#define LogManagerH

#include <fmx.h>

#include "ConsoleWriter.h"
#include "LoggingService.h"
#include "CritSec.h"

//
					// forward declarations
					class TloggingForm;
					class TMainForm;
					class GUImanager;
					class LogMessageQueue;
//---------------------------------------------------------------------------

/** There is a singleton instance of LogManager in the ValC program, and
  * its job is to manage the handling of log messages.
  * It sets up a logging service (see paulst::LoggingService),
  * a log window if required (see TloggingForm),
  * and sets up a writer (see GUIandLogWriter) to write log messages.
  * It also timestamps the beginning of the log with when the program started.
  *
  * The <b><code>logWindow</code></b> configuration value in the main config
  * file specifies whether a logging window is to be used or not:
  * set this either to <code>yes</code> or <code>no</code>.
  *
  * For details of how the handling and communication of log messages is
  * managed, see the documentation for TloggingForm.
  */
class LogManager {

public:

/** A string prefix used to flag up that a log message originated from the GUI.
  * To write a message to the log, code in the GUI section of ValC uses the
  * log method, which tacks on this prefix before passing it to
  * LoggingServices::log (which in turn will queue up GUIandLogWriter::write()
  * for execution).
  *
  * (Yes, this is a hacky way of doing it, but hacky will do, here, and
  * it's less effort than going to the trouble of deriving a fresh class from
  * the LoggingService. The existing methods of LoggingService only use a single
  * string parameter to convey to WorkerThread objects what writing task
  * needs to be done.)
  */
	static std::string FLAG_GUI;

/** A string prefix that can be (optionally) used to flag up that a log message
  * was generated as a result of an exception. Such log messages are displayed
  * in a more red/highlighted way in the logging window.
  *
  * GUI code can use the method logException() for such a log message,
  * which will automatically add this prefix, before sending it to log
  * for additionally flagging it up as having come from the GUI.
  */
	static std::string FLAG_EXCEPTION;

/** Used to flag up messages intended for the GUI side of the
  * logging window.
  */
	static int PURPOSE_GUI;

/** Used to flag up message intended for the Business Layer side of the
  * logging window.
  */
	static int PURPOSE_BUSINESS_LAYER;

/** Indicates whether a logging window is used for displaying log messages. */
	bool useWindow;

/** A reference to the logging service, which handles multiple threaded requests
  * for log messages.
  */
	paulst::LoggingService *logService;


// For comments on methods, please see implementation.

	LogManager(TMainForm *m, bool logWin);
	~LogManager();


	void log(const std::string & msg);
	void logException(const std::string & msg);

private:
	TMainForm *mainForm;    // a link back to the main form of the program

	std::string logTimestamp;  // to timestamp this ValC run

	void timestampLog();

};





/** Provides a log writing facility, to be used by the logging service
  * paulst::LoggingService.
  *
  * The main role of a GUIandLogWriter is to provide the write() method,
  * which gets used by the various threads created by the logging service in order
  * to execute writing-to-the-log tasks.
  * A GUIandLogWriter can writes to a plaintext log file, and also to a
  * logging window, if one is in use.
  */
class GUIandLogWriter : public paulst::Writer {

private:

	paulst::FileWriter *fileWriter;  // use Paul's facility for writing to files
									 // this class has responsibility for
									 // destroying it afterwards

	LogManager *logMan;

	TThreadMethod handleNextLogMessage;  // reference to the method that processes
										 // the next message from the logging
										 // window's queue

	LogMessageQueue *msgQueue; // a pointer to the logging window's
							   // message queue


public:
	~GUIandLogWriter();
	GUIandLogWriter(LogManager *logMan, TThreadMethod handle,
					LogMessageQueue *q, const std::string & filename);

	void write(const std::string & msg);
};


#endif