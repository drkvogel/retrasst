//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#pragma package(smart_init)
#pragma resource "*.fmx"

#include "LogForm.h"


//---------------------------------------------------------------------------

/** Sets up an empty queue. */
LogMessageQueue::LogMessageQueue()
{
}

LogMessageQueue::~LogMessageQueue()
{
}

/** Adds the given message to the queue of messages waiting to be
  * put onto the logging window.
  *
  * @param lmsg   a message, consisting of a string accompanied by
  *               characteristics as to how it will be displayed
  */
void LogMessageQueue::addMsgToQ(const LabelledMessage & lmsg)
{
	paulst::AcquireCriticalSection a(queueLock);
	q.push_back(lmsg);
}

/** Removes the message at the front of the message queue.  Invoked in order
  * to put a message onto one of the lists of the logging window.
  *
  * @return    the message at the front of the queue, labelled with its
  *            intended display characteristics
  */
LabelledMessage LogMessageQueue::removeMsgFromQ()
{
	paulst::AcquireCriticalSection a(queueLock);
	LabelledMessage lmsg = q.front();
	q.pop_front();
	return lmsg;
}

//---------------------------------------------------------------------------

TloggingForm *loggingForm;

/** Creates a logging window, as designed in the IDE. */
__fastcall TloggingForm::TloggingForm(TComponent* Owner)
	: TForm(Owner)
{
}

void __fastcall TloggingForm::FormCreate(TObject *Sender)
{
	Show();
}

__fastcall TloggingForm::~TloggingForm()
{
}
//---------------------------------------------------------------------------

/** Sets the title of the logging window to the given string.
  *
  * @param t      the string to set the caption/title of the window to
  */
void TloggingForm::setTitle(const std::string & t)
{
	BeginUpdate();
	Caption = Utils::str2unicodestr(t);
	EndUpdate();
}


/** Makes the specified listbox scroll down to the bottom.
  * This will be either the BLlistbox or the GUIlistbox, and the
  * command is given to scroll down to the bottom after a message has
  * been appended to the log.
  *
  * @param box     the list box to have its viewport set to the bottom
  */
void TloggingForm::scrollDown(TListBox *box)
{

	float viewHeight = box->Height;
	int ih = box->ItemHeight;       // use param("logEntryLineHeight")
	int n = box->Count;
	int totalHeight = ih*n;
	/*
	std::string s = "n=" + Utils::int2str(n) + ",  ih=" + Utils::int2str(ih)
					+ ", viewHeight=" + Utils::int2str(viewHeight);
	mainForm->StatusLabel->Text = Utils::str2unicodestr(s);
	*/

	TPointF pos;
	pos.x = 0.0;
	if (totalHeight>viewHeight) {
		pos.y = totalHeight - viewHeight;
	}
	else
		pos.y = 0;

	box->BeginUpdate();
	box->ViewportPosition = pos;
	box->EndUpdate();
	box->Repaint();

}


/** Displays the given message on specified list box within the logging window.
  * If the message is too long, it does a clunky home brewed line wrapping
  * (because FireMonkey won't adjust the height when it wraps lines).
  * After appending the message, the list box has its scroll position set so
  * that the message that has just been appended can be viewed, i.e. at the
  * bottom.
  *
  * @param msg            the message to be displayed
  * @param box            which listbox on the logging window to add it to
  * @param highlighting   true if this message needs highlighting, for example
  *                       it might have originated from an exception
  */
void TloggingForm::addMsgToListBox(const std::string & msg,
								   TListBox *box,
								   bool highlighting)
{
	TListBoxItem *item;
	std::string rmsg = msg;
	std::string line;
	int lineCount = 0;
	while (rmsg.length()>0) {

		// first get what to put on this line
		size_t pos = rmsg.find('\n');
		if (pos==std::string::npos) {
			line = rmsg;
			rmsg = "";
		}
		else {
			line = rmsg.substr(0,pos);
			rmsg = rmsg.substr(pos+1);
		}
		if (line.length()>maxLineLength) {
			// consider breaking the line at an earlier point
			size_t comma = line.substr(0,maxLineLength).rfind(", ");
			size_t sp = line.substr(0,maxLineLength).rfind(" ");
			if (comma!=std::string::npos) {
				rmsg = line.substr(comma+2) + rmsg;
				line = line.substr(0,comma+2);
			}
			else if (sp!=std::string::npos) {
				rmsg = line.substr(sp+1) + rmsg;
				line = line.substr(0,sp+1);
			}
			// else never mind, I tried
        }

		item = new TListBoxItem(box);   // will get deleted when the list box
										// gets deleted, which will get deleted
										// when the logging form does
		if (lineCount>0) {
			item->StyleLookup = "LogPlainStyle";
		}
		else if (highlighting) {
			item->StyleLookup = "LogExceptionStyle";
		}
		else {
			item->StyleLookup = "LogEntryStyle";
		}
		item->Text = Utils::str2unicodestr(line);

		item->Parent = box;
		//		setToBottom(box);
		lineCount++;

	}
	scrollDown(box);  // this sets the scrollbar to the bottom of the list
					  // otherwise it will get very annoying very quickly,#
					  // having to scroll down all the time
}

/** Adds the given message to the logging window. Depending on the prefix(es),
  * the message can be added to the Business Layer list, or the GUI list,
  * or it can be highlighted (as might be the case for exceptions).
  */
void TloggingForm::appendMessageToLogWindow(const LabelledMessage & lmsg)
{
	if (lmsg.purpose==LogManager::PURPOSE_GUI) {  // twas a GUI message, so
												  // put it on rhs
		addMsgToListBox(lmsg.message,GUIlistbox,lmsg.highlight);
	}
	else { // put on the lhs
		addMsgToListBox(lmsg.message,BLlistbox,lmsg.highlight);
    }
}

/** Removes the next log message from the message queue of log messages waiting
  * to be added, and displays the log message by appending it to the correct
  * list within the logging window. Note that the queue should not be empty,
  * as this method is only invoked (by the GUIandLogWriter) one-time only,
  * after adding a message to the queue.
  */
void __fastcall TloggingForm::processNextMessage()
{
	LabelledMessage lmsg = msgQueue.removeMsgFromQ();
	appendMessageToLogWindow(lmsg);
}


//---------------------------------------------------------------------------

/** This handles resizing, keeping a 50:50 width between the two halves of
  * the logging window.
  */
void __fastcall TloggingForm::FormResize(TObject *Sender)
{
	float w = this->Width;
	BLlistbox->BeginUpdate();
	BLlistbox->Width = w/2;
	BLlistbox->EndUpdate();
	DualPane->Repaint();
}
