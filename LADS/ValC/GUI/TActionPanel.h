//---------------------------------------------------------------------------

#ifndef TActionPanelH
#define TActionPanelH

#include "InfoPanels.h"

							// some forward declarations
							class TTestPanel;
							class GUImanager;



/** A pop-up panel for displaying information about test results,
  * along with any alert information and opportunities for
  * the operator to take certain actions.
  * For example, alerts can include information about QC samples
  * that have failed Westgard rules. Actions can be to schedule re-runs.
  * Alerts come in three possible flavours: a 'pass' (in which case there is
  * no alert), a 'maybe', where the user will use some discretion, or a 'fail'.
  *
  * The general layout of the pop-up action panel, with the variable names used
  * for the TLayout/TPanel areas:
  *
  * @image html TActionPanel-layout.png "The general layout of the pop-up action panel"

  */
class TActionPanel : public TInfoPanel
{
private:


// ---------------------------------------- //

	int calculateHeight(TTestPanel *t);

	/** The main area to display information and action opportunities,
	  * taking up all the space below the top of the panel. */
	TLayout *mainArea;

	void initialiseContents();

// -------- for the top area -------------- //

	/** An area at the top of this alert panel. It will include an icon
	  * and whatever buttons and other interactive components are needed later. */
	TLayout *topArea;

	/** A panel to hold an alert icon, at the left-hand side of the top area.
	  * The icon could be a pass or fail icon. */
	TPanel *iconPanel;

	/** Holds the main labels for test result & time obtained. */
	TLayout *topLabelsArea;

	/** Labels the name of the test taken. e.g. "Test: " */
	TLabel *testNameLabel;

	/** Displays the name of the test taken. e.g. "CHOL." */
	TLabel *testNameDisplay;

	/** Labels the result of the test taken. e.g. "   Result: " */
	TLabel *testResultLabel;

	/** Displays the result of the test taken. e.g. "4.2" */
	TLabel *testResultDisplay;

	/** Labels the time of the result. e.g. "When: " */
	TLabel *testTimeLabel;

	/** Displays the time of the result. e.g. "08:42" */
	TLabel *testTimeDisplay;

	/** Labels the date of the result. e.g. " on " */
	TLabel *testDateLabel;

	/** Displays the date of the result. e.g. "2013-11-26 (Tue)" */
	TLabel *testDateDisplay;

	void initialiseTopArea();
	void initialiseTestResultLabels();

// -------- for the information area on the left -------------- //

	/** An area on the left to display alerts and other information about
	  * the result of this test. */
	TLayout *leftArea;

	/** Labels the list of alert messages, e.g. "Rule applications:" */
	TLabel *ruleApplicationsLabel;

	/** the listbox holding a sequence of alert information */
	TListBox *alertListBox;

	void initialiseLeftArea();

// -------- for the action area on the right -------------- //

	/** An area on the right to display opportunities to take action on
	  * the result of this test. */
	TLayout *rightArea;

	TLabel *triggerPlaceholder;

	void initialiseRightArea();

// ---------------------------------------- //



	void displayAlertItems();


public:

	__fastcall ~TActionPanel();
	__fastcall TActionPanel(GUImanager *g, TComponent *owner, TTestPanel *t);


	void __fastcall removeClick(TObject *Sender);
};






//---------------------------------------------------------------------------
#endif
