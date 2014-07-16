
#ifndef TTestInstancePanelH
#define TTestInstancePanelH

#include <SysUtils.hpp>
#include <Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <string>
#include <map>
#include <list>


/* This file contains some simple visual components that haven't evolved
   enough to have their own file yet:
       TSampleRunPanel

*/

							// some forward declarations
							class WorklistEntriesView;
                            class SnapshotFrameObserver;
							class TInfoPanel;
							class TActionPanel;
							class TNotesPanel;
							class TBasicInfoPanel;
							class TBarcodePanel;
							class TTestInstancePanel;
							
							

//---------------------------------------------------------------------------

/** A panel to display a worklist entry.
  *
  * Here's an illustration of what a typical TTestInstancePanel looks like in an early
  * ValC prototype at 2x magnification, surrounded by other TTestInstancePanel objects:
  *
  * @image html TTestInstancePanel.png
  */
class TTestInstancePanel : public TPanel
{
	friend class TBasicInfoPanel;
	friend class TActionPanel;
	friend class WorklistEntriesView;

public:

//---------- Graphical constants for displays sizes/positioning -------------//
//  (these are specific to (but not exclusively used by) TTestInstancePanel)

	/** The width of the clock icon. Used for a minimum width of test results. */
	const static int CLOCK_SIZE = 16;

	/** Vertical offset for a clock, as compared to a label
		(may need changing once font size implemented).  */
	const static int CLOCK_OFFSET_Y = 2;

	/** The radius of the corner of TTestInstancePanel objects.
		This determines the roundness of the "bubble" shape. */
	const static int CORNER_WIDTH = 13;

	/** Horizontal offsets between the right-hand end of one button and
		the left-hand end of the next. */
	const static int BUTTON_GAP = -4;

	/** Horizontal gap between the left-hand end of displayed components
		and the left-hand side of this panel  */
	const static int LHS_GAP = 4;

	/** Horizontal gap between the right-hand end of displayed components
		and the right-hand side of this panel, if everything was snug
		(it may not be, due to lining things up neatly) */
	const static int RHS_GAP = 3;

	/** Width of the icon representing a semi-transparent button
		indicating that a worklist entry has one or more notes attached. */
	const static int NOTES_WIDTH = 24;


	const static int OVERLAPPED_LHS_GAP = 2*CORNER_WIDTH;


// display positioning:
//
//    (     testName       notesButton      resultButton )
//    LHS_GAP      BUTTON_GAP       BUTTON_GAP        RHS_GAP
//      or                  NOTES_WIDTH
//  OVERLAPPED_LHS_GAP
//             ^                                 ^
//          nameSpace                          resultSpace


//---------- display subcomponents ------------//

	/** This button displays the name of the test, e.g. "CHOL." for a
	  * cholesterol test.  Clicking on this button allows the user to
	  * bring up an information panel giving standard information
	  * about the worklist entry.
	  */
	TSpeedButton *testNameButton;

	/** Displays a note icon if there is a note attached to this
	  * worklist entry (or a notes icon if there is more than one note).
	  * If so, then clicking on it will pop up a notes information panel.
	  * Otherwise, this button looks invisible, but on hover the user
	  * can press it and will be given the opportunity to attach a note.
	  */
	TSpeedButton *notesButton;

	/** Displays a little triangle warning icon if there are one or more alerts
	  * attached to this worklist entry.
	  * If so, then clicking on it will pop up an alert information panel.
	  * Otherwise, this button is disabled.
	  */
	TSpeedButton *resultButton;


	/* * the label displaying the test result for this worklist entry */
	// TLabel *testResultDisplay;

	/** the panel that gets used to display the clock face icon, if the test
	  * result is pending */
	TPanel *clockface;

	/** the pop-up panel used to display basic information associated with this
	  * worklist entry. This is NULL if no pop-up is displayed.  	  */
	TBasicInfoPanel *myBasicInfo;

	/** the pop-up panel used to display notes attached to this
	  * worklist entry. This is NULL if no pop-up is displayed.  	  */
	TNotesPanel *myNotes;

	/** the pop-up panel used to display any alerts and actions associated with this
	  * worklist entry. This is NULL if no pop-up is displayed.     */
	TActionPanel *myActions;


// For comments on methods, please see their implementations.

	__fastcall TTestInstancePanel(WorklistEntriesView *v, TPanel *owner,
								  const std::string & testName, int nameSpace,
								  const std::string & testResult, int resultSpace,
								  SnapshotFrameObserver* ob);
	__fastcall TTestInstancePanel(WorklistEntriesView *v, TPanel *owner,
								  const std::string & testName, int nameSpace,
								  SnapshotFrameObserver* ob);
	__fastcall ~TTestInstancePanel();

	static int displayWidth(const std::string &str);
	static int minResultWidth();
	static int findResultDisplayWidth(const std::string &r);

	static int getWidth(const std::string &n, const std::string &r,
						bool cmpct, bool qd);
	static int getWidth(int n, int r, bool cmpct, bool qd);

	static bool goCompare(const TTestInstancePanel * first, const TTestInstancePanel * second);

    void needsAttention();
	void setCompact(bool c, int newWidth);
	void makeNonLocal();

	bool hasAttribute(const std::string & attr);
	std::string getAttribute(const std::string & key);
	int getIntAttribute(const std::string & key);
	void setAttribute(const std::string & key, const std::string & value);
	void setAttribute(const std::string & key, int value);

	void __fastcall onClick(TObject *Sender);

	void setObserver(SnapshotFrameObserver* ob);

private:
	WorklistEntriesView *controller;

	std::map <std::string,std::string> *attributes;
	std::map <std::string,int> *ids;

	// If non-null, the screen has the effect of making the TTestInstancePanel
	// look faded, for the purposes of non-local result from worklist entries
	TRectangle *screen;

	std::string testName;
	std::string testResult;

	bool attention;   // true if this is a test result in need of attention
	bool local;       // false if the test was not carried out on the local machine
	int nameWidth;    // horizontal space taken up by the name of the test
	int resultWidth;  // horizontal space taken up by the result from the test

	/** The x-coordinate of the left-hand side of the result. */
	float resultPosX;

	bool queued;
	bool compact;  // If true, will look like (CHOL. 4.5)
				   // as compared to   (   CHOL. 4.5)
				   // The latter representation is for bubbles with an overlap on
				   // the left, the former for bubbles without such an overlap
				
	bool displayAsCompleted;   // true for entries with results,
							   // will be displayed in a completed style even
							   // if the status is still 'P'

	bool localMachine;      // displayed highlighted, if true, as it's of local interest

    SnapshotFrameObserver *observer;

	void __fastcall basicInfoClick(TObject *Sender);
    void __fastcall notesClick(TObject *Sender);
	void __fastcall resultClick(TObject *Sender);

	void initialiseButtons();
	void createButtons();
	void createTestNameButton();
	void createNotesButton();
	void createResultsButton();
	void positionButtons();
	void positionTestNameButton();
	void positionNotesButton();
	void positionResultsButton();

  //	void setUpTestNameButton();  // old
	void setUpNotesButton();
	int findResultDisplayWidth();
	void setUpResultButton(bool queued);
	void setUpNonLocal();
	void setUpSampleResults();


};


#endif
