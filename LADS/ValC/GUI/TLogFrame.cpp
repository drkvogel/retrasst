//---------------------------------------------------------------------------

#include <fmx.h>
#pragma hdrstop

#include "AcquireCriticalSection.h"
#include "TLogFrame.h"
#include "Utils.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"
TLogFrame *LogFrame;

//---------------------------------------------------------------------------

int LabelledMessage::PURPOSE_BUSINESS_LAYER = 0;
int LabelledMessage::PURPOSE_GUI = 1;

//---------------------------------------------------------------------------
__fastcall TLogFrame::TLogFrame(TComponent* Owner)
	: TFrame(Owner),
    m_idleServiceUser(this)
{
}
//---------------------------------------------------------------------------


/** Makes the specified listbox scroll down to the bottom.
  * This will be either the BLlistbox or the GUIlistbox, and the
  * command is given to scroll down to the bottom after a message has
  * been appended to the log.
  *
  * @param box     the list box to have its viewport set to the bottom
  */
void TLogFrame::scrollDown(TListBox *box)
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
void TLogFrame::addMsgToListBox(const std::string & msg,
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
void TLogFrame::appendMessageToLogWindow(const LabelledMessage & lmsg)
{
	if (lmsg.purpose==LabelledMessage::PURPOSE_GUI) {  // twas a GUI message, so
												  // put it on rhs
		addMsgToListBox(lmsg.message,GUIlistbox,lmsg.highlight);
	}
	else { // put on the lhs
		addMsgToListBox(lmsg.message,BLlistbox,lmsg.highlight);
    }
}

void TLogFrame::queueMessage( const LabelledMessage& lm )
{
	paulst::AcquireCriticalSection a(m_queueLock);

	m_messageQueue.push_back( lm );
}


//---------------------------------------------------------------------------

void TLogFrame::onIdle()
{
    processQueuedMessages();
}

/** This handles resizing, keeping a 50:50 width between the two halves of
  * the logging window.
  */
void TLogFrame::onResize()
{
	float w = this->Width;
	BLlistbox->BeginUpdate();
	BLlistbox->Width = w/2;
	BLlistbox->EndUpdate();
	DualPane->Repaint();
}

void TLogFrame::processQueuedMessages()
{
	paulst::AcquireCriticalSection a(m_queueLock);

	for ( const LabelledMessage& lm : m_messageQueue )
	{
		appendMessageToLogWindow(lm);
	}

	m_messageQueue.clear();
}
//---------------------------------------------------------------------------
void TLogFrame::registerWithIdleService( valcui::IdleService* is )
{
    is->registerUser( &m_idleServiceUser );
}

