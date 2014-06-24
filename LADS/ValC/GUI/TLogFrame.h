//---------------------------------------------------------------------------

#ifndef TLogFrameH
#define TLogFrameH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.Forms.hpp>
#include <FMX.Layouts.hpp>
#include <FMX.ListBox.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
//---------------------------------------------------------------------------
#include "CritSec.h"
#include "IdleServiceUserAdapter.h"
#include <string>
#include <vector>

/** Represents a message string with some additional characteristics, which
  * will dictate how such a message would get displayed in the logging window.
  */
struct LabelledMessage
{
/** Used to flag up messages intended for the GUI side of the
  * logging window.
  */
	static int PURPOSE_GUI;

/** Used to flag up message intended for the Business Layer side of the
  * logging window.
  */
	static int PURPOSE_BUSINESS_LAYER;


	int         purpose;        ///< e.g. is it from the GUI or Business Layer?
	bool        highlight;      ///< does it need highlighting?
	std::string message;        ///< the text of the message
};


class TLogFrame : public TFrame
{
__published:	// IDE-managed Components
	TLayout *TopLabelsArea;
	TLabel *Label2;
	TLabel *Label3;
	TLayout *DualPane;
	TListBox *BLlistbox;
	TListBox *GUIlistbox;
	TStyleBook *StyleBook2;
private:	// User declarations
	paulst::CritSec m_queueLock;
	std::vector<LabelledMessage> m_messageQueue;
    valcui::IdleServiceUserAdapter<TLogFrame> m_idleServiceUser;    
public:		// User declarations
	__fastcall TLogFrame(TComponent* Owner);

    void onIdle();
    void onResize();
    void queueMessage( const LabelledMessage& lm );
    void registerWithIdleService( valcui::IdleService* is );

private:
	int maxLineLength = 65;  // quick and dirty hack, for implementing line
							 // wrapping, since the list items won't


	void addMsgToListBox(const std::string & msg, TListBox *box, bool highlighting);
	void appendMessageToLogWindow(const LabelledMessage & msg);
	void processQueuedMessages();
	void scrollDown(TListBox *box);
};
//---------------------------------------------------------------------------
extern PACKAGE TLogFrame *LogFrame;
//---------------------------------------------------------------------------
#endif

