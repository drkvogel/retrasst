
#ifndef VisualComponentsH
#define VisualComponentsH

#include <SysUtils.hpp>
#include <Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <string>
#include <map>
#include <list>


/* This file contains some visual components:
       TSampleRunPanel
	   TBarcodePanel           (displays a DSampleRun)
	   TTestPanel              (displays a DSampleTest)
*/

							// some forward declarations
							class WorklistEntriesView;
                            class SnapshotFrameObserver;
							class TInfoPanel;
							class TActionPanel;
							class TNotesPanel;
							class TBasicInfoPanel;
							class TBarcodePanel;
							class TTestPanel;

typedef std::list<TTestPanel *> EntryPanelsList;

typedef EntryPanelsList::iterator EntryPanelsIterator;
typedef EntryPanelsList::reverse_iterator EntryPanelsReverseIterator;

//---------------------------------------------------------------------------

/** A simple panel to hold the barcode panel and panels representing tests on samples. */
class TSampleRunPanel : public TPanel
{
public:
	TBarcodePanel *barcodePanel;
	TPanel *testsPanel;
	EntryPanelsList *testPanels;  // list of the TTestPanels contained in testsPanel
								  // (this is a separate list so I can get at it)

	__fastcall TSampleRunPanel(TComponent *owner);
	__fastcall ~TSampleRunPanel();
};

//---------------------------------------------------------------------------

/** A panel to display the barcode for a sample run. */
class TBarcodePanel : public TPanel
{
public:
	// DSampleRun *sample; // the data associated with this barcode panel
	__fastcall TBarcodePanel(WorklistEntriesView *g, TComponent *Owner,
							 bool queued);
	__fastcall ~TBarcodePanel();

	void updateBarcode(const std::string & barcode);
	void needsAttention();

	// quick & flexible get/set attribute stuff for prototyping purposes
	std::string getAttribute(const std::string & key);
	int getIntAttribute(const std::string & key);
	void setAttribute(const std::string & key, const std::string & value);
	void setAttribute(const std::string & key, int value);

private:
	TLabel *barcodeLabel;
	WorklistEntriesView *gui;

	std::map <std::string,int> *ids;     // later: figure how to get TBarcodePanel
										 // & TTestPanel implement the same
	std::map <std::string,std::string> *attributes;	 // attributes/ids interface
};


//---------------------------------------------------------------------------

/** A panel to display a worklist entry.
  *
  * Here's an illustration of what a typical TTestPanel looks like in an early
  * ValC prototype at 2x magnification, surrounded by other TTestPanel objects:
  *
  * @image html TTestPanel.png
  */
class TTestPanel : public TPanel
{
	friend class TBasicInfoPanel;
	friend class TActionPanel;
	friend class WorklistEntriesView;

public:

	/** This button displays the name of the test, e.g. "CHOL." for a
	  * cholesterol test.  Clicking on this button allows the user to
	  * bring up an information panel giving standard information
	  * about the worklist entry.
	  */
	TSpeedButton *testNameButton;

	/** The x-coordinate of the left-hand side of the result. */
	float resultPosX;

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

	__fastcall TTestPanel(WorklistEntriesView *v, TPanel *owner,
						  const std::string & testName,
						  const std::string & testResult,
						  const std::string & label,
						  SnapshotFrameObserver* ob, bool qd);
	__fastcall ~TTestPanel();

	int findPanelLeftWidth();
	static int findPanelWidth(WorklistEntriesView *v, int maxNameWidth, int maxResultWidth);
	// static int findPanelWidth(WorklistEntriesView *gui, DSampleTest *entry, bool queued);
	static bool goCompare(const TTestPanel * first, const TTestPanel * second);

	void needsAttention();

	bool hasAttribute(const std::string & attr);
	std::string getAttribute(const std::string & key);
	int getIntAttribute(const std::string & key);
	void setAttribute(const std::string & key, const std::string & value);
	void setAttribute(const std::string & key, int value);

	void __fastcall onClick(TObject *Sender);

	void setObserver(SnapshotFrameObserver* ob);

private:
	WorklistEntriesView *gui;

	std::map <std::string,std::string> *attributes;
	std::map <std::string,int> *ids;

	// If non-null, the screen has the effect of making the TTestPanel
	// look faded, for the purposes of non-local result from worklist entries
	TRectangle *screen;

	std::string testName;
	std::string testResult;
	bool queued;

	bool displayAsCompleted;   // true for entries with results,
							   // will be displayed in a completed style even
							   // if the status is still 'P'

	bool localMachine;      // displayed highlighted, if true, as it's of local interest

    SnapshotFrameObserver *observer;

	void __fastcall basicInfoClick(TObject *Sender);
    void __fastcall notesClick(TObject *Sender);
	void __fastcall resultClick(TObject *Sender);

	void initialiseTestNameButton();
	void initialiseNotesButton();
	void initialiseResultsButton();

  //	void setUpTestNameButton();  // old
	void setUpNotesButton();
	int findResultDisplayWidth();
	void setUpResultButton(bool queued);
	void setUpNonLocal();
	void setUpSampleResults();


};


#endif
