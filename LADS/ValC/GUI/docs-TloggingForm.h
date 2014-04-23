// This file contains doxygen comments giving an overview of the logging window management in the ValC GUI. 
// There is no code in this file, just documentation. 
// (This comment itself is not a doxygen comment.)


/** @class TloggingForm 
@brief Provides a logging window for ValC, displaying log messages during program execution.

The following describes how the various parts of the logging communicate with each other
and with the window. There are five objects that contribute:

+ the logging window (an instance of TloggingForm)
+ a log manager (an instance of LogManager)
+ a logging service (an instance of paulst::LoggingService)
+ a log writer for files & the logging window (an instance of GUIandLogWriter)
+ a message queue for the log writer and logging window to communicate
   (an instance of LogMessageQueue)

Here's an illustration, which will make a lot more sense if you read the explanation
below: 
   
@image html logging-diagram.png

(Note that in the diagram, an arrow represents some kind of communication initiated
by where the arrow came from, and a thread is represented by a tall wavy rectangle.)

Here's how the logging communication works:

Firstly, the log manager (LogManager) creates a logging service (paulst::LoggingService),
passing it a GUIandLogWriter object that it can use.  This logging service is then available
to the rest of ValC. The Business Layer code will use the logging service directly by sending
messages to the <code>LoggingService::log</code> method, 
whereas the GUI code will go via the log manager, which will add on some extra information so that 
the message can be displayed differently to the Business Layer messages, and then pass it on
to the <code>LoggingService::log</code> method.

When a log message reaches the logging service, it then immediately spawns a separate little worker thread
especially for that message (such threads are depicted in blue above), so as not to hold up the 
main execution of the program.  

Each worker thread then calls the GUIandLogWriter::write method of the log writer, 
which first writes the log message to the text file. It then appends the log message to the message
queue (LogMessageQueue) owned by the logging window (TloggingForm), and sends a request to the 
main thread (as depicted by the grey wavy rectangle in the GUI area of the program) using
<code>TThread::Queue</code> that it should call TloggingForm::processNextMessage  when it next gets
a spare moment.  This is because all GUI updating should take place in the main thread (otherwise
you get crashes and/or window inconsistencies), but the request should be queued so that it doesn't
hold up the main thread.

Then, once the main thread gets round to it, it scheduled the execution of TloggingForm::processNextMessage,
which takes a single message from the front of the message queue (LogMessageQueue) and appends it
to the correct list of log messages in the logging window.


*/
