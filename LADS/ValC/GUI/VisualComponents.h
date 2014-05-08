
#ifndef VisualComponentsH
#define VisualComponentsH

#include <SysUtils.hpp>
#include <Classes.hpp>
#include <FMX.Controls.hpp>
#include <FMX.StdCtrls.hpp>
#include <FMX.Types.hpp>
#include <string>




/* This file contains some visual components:
	   TBarcodePanel           (displays a DSampleRun)
	   TTestPanel              (displays a DSampleTest)
*/

							// some forward declarations
							class DSampleRun;
							class DSampleTest;
							class GUImanager;
                            class SnapshotFrameObserver;
							class TInfoPanel;
							class TActionPanel;
							class TNotesPanel;
							class TBasicInfoPanel;


//---------------------------------------------------------------------------

/** A panel to display the barcode for a sample run. */
class TBarcodePanel : public TPanel
{
public:
	DSampleRun *sample; // the data associated with this barcode panel
	__fastcall TBarcodePanel(GUImanager *g, TComponent *Owner,
							 DSampleRun *s, bool queued);
	__fastcall ~TBarcodePanel();

private:
	TLabel *barcodeLabel;
	GUImanager *gui;

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

	TTestPanel *next, *prev;  // not sure if I'll need these, but anyway...

	/** the worklist entry associated with this panel */
	const DSampleTest *entry;

// For comments on methods, please see their implementations.


	__fastcall TTestPanel(GUImanager *g, TPanel *Owner,
						  DSampleTest *d, const int posX, bool queued);
	__fastcall ~TTestPanel();

    static int findPanelWidth(GUImanager *gui, DSampleTest *entry, bool queued);

    void __fastcall onClick(TObject *Sender);

    void setObserver( SnapshotFrameObserver* o );

private:
	GUImanager *gui;


	TRectangle *screen;   // for making things a bit transparent
	std::string testName;
	std::string testResult;
	bool displayAsCompleted;   // true for entries with results,
							   // will be displayed in a completed style even
							   // if the status is still 'P'

	bool localMachine;      // displayed highlighted, if true, as it's of local interest

    SnapshotFrameObserver *observer;

	void __fastcall basicInfoClick(TObject *Sender);
    void __fastcall notesClick(TObject *Sender);
	void __fastcall resultClick(TObject *Sender);

	void setUpTestNameButton();
	void setUpNotesButton();
	static float findResultDisplayWidth(GUImanager *gui, std::string res);
	void setUpResultButton(bool queued);
	void setUpNonLocal();
	void setUpQueuedSample();
	void setUpSampleResults();


};


#endif
