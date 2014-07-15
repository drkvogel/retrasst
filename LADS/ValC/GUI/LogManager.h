//---------------------------------------------------------------------------

#ifndef LogManagerH
#define LogManagerH

#include <fmx.h>

#include "ConsoleWriter.h"
#include "IdleServiceUserAdapter.h"
#include "LoggingService.h"
#include "ModelEventListenerAdapter.h"

//
					// forward declarations
					class TLogFrame;
namespace valcui
{
    class Model;
}

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

/** A reference to the logging service, which handles multiple threaded requests
  * for log messages.
  */
	paulst::LoggingService *logService;

// For comments on methods, please see implementation.

	LogManager( const std::string& logFilePath );
	~LogManager();

    valcui::IdleServiceUser*    getIdleServiceUserInterface();
    valcui::ModelEventListener* getModelEventListenerInterface();
    void init();
	void log(const std::string & msg);
	void logException(const std::string & msg);
	void logException(const Exception& e);
    void notify( int eventID, const valcui::EventData& );
    void onIdle();
    void onResize();
    void setModel( valcui::Model* model );
    void setView( TLogFrame* view );

private:
    valcui::ModelEventListenerAdapter<LogManager> m_modelEventListener;
    valcui::IdleServiceUserAdapter   <LogManager> m_idleServiceUser;
	std::string logTimestamp;  // to timestamp this ValC run
    const std::string m_logFilePath;
    TLogFrame* m_view;
    valcui::Model* m_model;

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

	TLogFrame *msgQueue; // a pointer to the logging window's
							   // message queue

public:
	~GUIandLogWriter();
	GUIandLogWriter(TLogFrame *q, const std::string & filename);

	void write(const std::string & msg);
};


#endif

