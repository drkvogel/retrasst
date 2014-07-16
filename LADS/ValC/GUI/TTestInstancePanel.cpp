//---------------------------------------------------------------------------

#include "ExceptionHandler.h"
#include <fmx.h>
#include "Utils.h"


//---------------------------------------------------------------------------

#include <set>

#include "Trace.h"   // then can do trace("...") in constructors/destructors
#include "ConsoleWriter.h"

#include "Utils.h"
#include "WorklistEntriesView.h"
#include "WorklistEntriesPositioning.h"
#include "TTestInstancePanel.h"
#include "TSnapshotFrame.h"
#include "VisualComponents.h"

#pragma package(smart_init)



/** Used to check that TTestInstancePanel does not have any pure virtual functions
  * (uninstantiated abstract functions). Automatically generated by RAD studio
  * as part of the process of creating a new component.
  */
static inline void ValidCtrCheck(TTestInstancePanel *)
{
	new TTestInstancePanel(NULL,NULL,NULL,0,NULL);
}


/** Destroys the child components of this panel, including buttons and labels. */
__fastcall TTestInstancePanel::~TTestInstancePanel() {

	delete attributes;
	delete ids;
	delete testNameButton;
	// delete testResultDisplay;
	delete notesButton;
	delete resultButton;
	// check if myActions (if it existed) gets deleted from its parent:
	// if not, delete it here
	// same for other info panels
}


/** Constructs a panel to represent a non-queued instance of a test being
  * carried out on a sample (not quite a 1-1 correspondence with a worklist entry)
  *
  * @param v           the WorklistEntriesView object
  * @param owner       the panel to which this will belong
  * @param testName    the name of the scheduled test for this sample
  * @param nameSpace   the horizontal space allocated for the name
  *                    (should be the max space required for the name across all test results)
  * @param testResult  the result from this test, or "" if no result
  * @param resultSpace the horizontal space allocated for the result
  * @param ob          the observer of any action (e.g. a click) on this panel
  * @param small       true if this is to be displayed compactly
  * @param att         true if this result needs attention
  */
__fastcall TTestInstancePanel::TTestInstancePanel(WorklistEntriesView *v,
												  TPanel *owner,
												  const std::string & testName,
												  int nameSpace,
												  const std::string & testResult,
												  int resultSpace,
												  SnapshotFrameObserver* ob)
	: controller(v), TPanel(owner),
	  testName(testName), nameWidth(nameSpace),
	  testResult(testResult), resultWidth(resultSpace), observer(ob),
	  myBasicInfo(NULL), myNotes(NULL), myActions(NULL), clockface(NULL),
	  compact(false),  attention(false), queued(false), local(true) // <- defaults
{
	attributes = new std::map <std::string,std::string>();
	ids = new std::map <std::string,int>();

	StyleLookup = "TestResultPanelStyle"; // default

	TagString = "Level:test result";   // tags used when navigating component
									   // hierarchies and finding where we've
									   // got to
	Width = getWidth(nameSpace,resultSpace,compact,queued);
	Height = Positioning::ENTRY_HEIGHT;
	Parent = owner;
	initialiseButtons();
}



/** Constructs a panel to represent a queued worklist entry.
  *
  * @param v           the WorklistEntriesView object
  * @param owner       the panel to which this will belong
  * @param testName    the name of the scheduled test for this sample
  * @param nameSpace   the horizontal space allocated for the name
  * @param ob          the observer of any action on this panel (like a click)
  * @param small       true if this is to be displayed compactly
  */
__fastcall TTestInstancePanel::TTestInstancePanel(WorklistEntriesView *v,
												  TPanel *owner,
												  const std::string & testName,
												  int nameSpace,
												  SnapshotFrameObserver* ob)
	: TPanel(owner),
	  testName(testName), nameWidth(nameSpace),
	  testResult(""), resultWidth(0), compact(false),  attention(false),
	  controller(v), queued(true), observer(ob),  local(true),
	  myBasicInfo(NULL), myNotes(NULL), myActions(NULL), clockface(NULL)
{
	attributes = new std::map <std::string,std::string>();
	ids = new std::map <std::string,int>();

	StyleLookup = "TestQueuedPanelStyle";
	TagString = "Level:test result";   // tags used when navigating component
									   // hierarchies and finding where we've
									   // got to
	Width = getWidth(nameSpace,0,compact,queued);
	Height = Positioning::ENTRY_HEIGHT;
	Parent = owner;
	initialiseButtons();
}

void TTestInstancePanel::needsAttention() {
	attention = true;
	StyleLookup = "TestResultActionPanelStyle";
}

void TTestInstancePanel::initialiseButtons()
{
	createButtons();
	positionButtons();
}

void TTestInstancePanel::createButtons()
{
	createTestNameButton();
	if (!queued) {
		createNotesButton();
		createResultsButton();
	}
}

void TTestInstancePanel::positionButtons()
{
	positionTestNameButton();
	if (!queued) {
		positionNotesButton();
		positionResultsButton();
	}
}


// Sets up a button to hold the name of the test.
// This button looks like a label, until the user mouses over,
// then subtle 3d shading appears that makes it look like a button.
void TTestInstancePanel::createTestNameButton()
{
	testNameButton = new TSpeedButton(this);
	testNameButton->Parent = this;
	testNameButton->StyleLookup = "HalfVisibleButtonStyle";
	std::string displayText = " " + testName + " ";
	testNameButton->Text = Utils::str2unicodestr(displayText);
	testNameButton->Width = Utils::findTextWidth(displayText);
	testNameButton->HitTest = true;
	testNameButton->OnClick = onClick;
}

void TTestInstancePanel::positionTestNameButton()
{
	testNameButton->Position->X = (float)(compact ? LHS_GAP : OVERLAPPED_LHS_GAP);
	testNameButton->Position->Y = Positioning::RESULT_Y;
}


// Sets up a button for viewing/adding notes to a non-queued entry.
// Snugs up the notes button to the test name button to the left of it,
// so initialiseTestNameButton() needs to have been called earlier.
void TTestInstancePanel::createNotesButton()
{
	notesButton = new TSpeedButton(this);
	notesButton->Parent = this;
	notesButton->StyleLookup = "InvisibleNoteButtonStyle";  // default
	notesButton->Width = NOTES_WIDTH;
	notesButton->Text = "";
	notesButton->OnClick = onClick;
}

void TTestInstancePanel::positionNotesButton()
{
	notesButton->Position->X = testNameButton->Position->X
							   + testNameButton->Width
							   + BUTTON_GAP;
	notesButton->Position->Y = Positioning::RESULT_Y;
}

// Sets up the result display for a non-queued entry.
// This is either the test result or a pending icon (a clockface).
void TTestInstancePanel::createResultsButton()
{

	if (testResult.length()==0) {  // assume pending
		clockface = new TPanel(this);
		clockface->Parent = this;
		clockface->StyleLookup = "PendingPanelStyle";
		clockface->Width = CLOCK_SIZE;   // yes, these are in the style, but if I don't
		clockface->Height = CLOCK_SIZE;  // put them in, weird things happen
	}
	else {
		// sets up a button (invisible unless moused over)
		resultButton = new TSpeedButton(this);
		resultButton->StyleLookup = "HalfVisibleFixedButtonStyle";
		resultButton->Width = findResultDisplayWidth(testResult);
		resultButton->Text = testResult.c_str();
		resultButton->HitTest = true;
		resultButton->OnClick = onClick;
		resultButton->Parent = this;
	}
}

// Sets up the result display for a non-queued entry.
// This is either the test result or a pending icon (a clockface).
void TTestInstancePanel::positionResultsButton()
{
	float posX = (float)(compact ? LHS_GAP : OVERLAPPED_LHS_GAP)
				 + nameWidth + 2*BUTTON_GAP + NOTES_WIDTH;

	if (testResult.length()==0) {  // assume pending
		clockface->Position->X = posX;
		clockface->Position->Y = Positioning::RESULT_Y + CLOCK_OFFSET_Y;
	}
	else {
		resultButton->Position->X = posX;
		resultButton->Position->Y = Positioning::RESULT_Y;
	}
}

// c is the new value for the compact boolean
// - a button is displayed compactly if it's at the front of the list (no overlap)
void TTestInstancePanel::setCompact(bool c, int newWidth)
{
	compact = c;
	Width = newWidth;
	positionButtons();
	if (screen!=NULL) {
        screen->Width = newWidth;
    }
}

// this is called for entries in a sample run not analysed locally
// - this is displayed as a faded effect for the panel
void TTestInstancePanel::makeNonLocal()
{
	local = false;
	// the faded effect is achieved by means of a semi-transparent
	// white rounded rectangle in front of the panel, same size & shape
	screen = new TRectangle(this);
	screen->Parent = this;
	screen->Width = Width;
	screen->Height = Height;
	screen->Fill->Kind = TBrushKind::bkSolid;
	screen->Fill->Color = TAlphaColorRec::White;   // not TColorRec, has no effect
	screen->Stroke->Thickness = 0;

	screen->CornerType = TCornerType::ctRound;
	screen->XRadius = CORNER_WIDTH;
	screen->YRadius = CORNER_WIDTH;
	screen->Position->X = 0;
	screen->Position->Y = 0;
	screen->HitTest = false;
	screen->Opacity = 0.5;
    screen->BringToFront();
   // to do: put this onto a style

}


/** Returns the size of a displayed string, in pixels, when put on a label
  *
  * @param  str           the string to find out the display width of
  */
int TTestInstancePanel::displayWidth(const std::string &str)
{
	std::string d = " ";
	d += str + " ";  // the " "s are so the edges of the characters don't
					 // bump into things
	return Utils::findTextWidth(d);
}

/** Returns a suitable default minimum width for a test result,
  * namely something big enough to accommodate the clock icon.
  */
int TTestInstancePanel::minResultWidth()
{
	return CLOCK_SIZE;
}

int TTestInstancePanel::findResultDisplayWidth(const std::string &r)
{
	return Utils::findTextWidth(r) + RHS_GAP;
}

/** Returns the width that a TTestInstancePanel would be,
  * if displayed given certain labels.
  */
int TTestInstancePanel::getWidth(const std::string &n,
								 const std::string &r,
								 bool cmpct, bool qd)
{
	if (qd) {
		return (cmpct ? LHS_GAP : OVERLAPPED_LHS_GAP)
			   + displayWidth(n) + RHS_GAP;
	}
	else {
		return (cmpct ? LHS_GAP : OVERLAPPED_LHS_GAP)
			   + displayWidth(n)
			   + 2*BUTTON_GAP
			   + NOTES_WIDTH
			   + Utils::findTextWidth(r)
			   + RHS_GAP;
	}
}

/** Returns the width of a TTestInstancePanel would be, if displayed
  * given certain label widths. (Used when creating a TTestInstancePanel,
  * having been given widths for its labels.)
  */
int TTestInstancePanel::getWidth(int n, int r, bool cmpct, bool qd)
{
	if (qd) {
		return (cmpct ? LHS_GAP : OVERLAPPED_LHS_GAP)
			   + n + RHS_GAP;
	}
	else {
		return (cmpct ? LHS_GAP : OVERLAPPED_LHS_GAP)
			   + n
			   + 2*BUTTON_GAP
			   + NOTES_WIDTH
			   + r
			   + RHS_GAP;
	}
}

// findResultDisplayWidth was 	return Utils::findTextWidth(testResult) + RHS_GAP;
// findPanelLeftWidth    gone too, no need, instead compare widths of displayed labels
//                        using displayWidth


//---------------------------------------------------------------------------

/** Brings up a pop-up information panel that shows basic information for this
  * worklist entry. It is called when the name of the test (this is also a
  * button) is clicked on.
  *
  * @param Sender   this will be the test name button that got clicked
  */
void __fastcall TTestInstancePanel::basicInfoClick(TObject *Sender)
{
	// Sender is a TSpeedButton (this I checked)
	TSpeedButton *b = (TSpeedButton *)Sender; // b is the button clicked on
	// b->Parent is a TTestInstancePanel (this I checked too)
	TTestInstancePanel *t = (TTestInstancePanel *)(b->Parent); // t is the test result panel

	if (t->myBasicInfo==NULL) { // there isn't already a pop-up panel here

		controller->popupInfoPanel(t,TInfoPanel::panel_BASIC);
	}
}


/** Brings up a pop-up notes panel that indicates at the test result displayed
  * for this worklist entry. It is called when the little button with a note
  * or notes icon is clicked on, which will only be displayed when the
  * worklist entry has one or more notes attached to its test results.
  *
  * @param Sender   this will be the notes button that got clicked
  */
void __fastcall TTestInstancePanel::notesClick(TObject *Sender)
{
	TSpeedButton *b = (TSpeedButton *)Sender; // b is the button clicked on
	TTestInstancePanel *t = (TTestInstancePanel *)(b->Parent); // t is the test result panel

	if (t->myNotes==NULL) { // there isn't already a pop-up panel here

		controller->popupInfoPanel(t,TInfoPanel::panel_NOTES);
	}
}


void __fastcall TTestInstancePanel::onClick(TObject* sender)
{
	valcui::assertion((sender == testNameButton
					   || sender == notesButton
					   || sender == resultButton),
					  "origin of click not recognised" );

	int i = getIntAttribute("Worklist Id");

	observer->notifySelected(i);

	if (sender == testNameButton) {
		basicInfoClick(sender);
	}
	else if (sender == notesButton) {
		notesClick(sender);
	}
	else {
        resultClick(sender);
    }
}

/** Pops up panel which will display any results and provide facilities for
  * the user to take various actions based on the test result.
  *
  * @param Sender   this will be the result button that got clicked
  */
void __fastcall TTestInstancePanel::resultClick(TObject *Sender)
{
	TSpeedButton *b = (TSpeedButton *)Sender; // b is the button clicked on
	TTestInstancePanel *t = (TTestInstancePanel *)(b->Parent); // t is the test result panel

	if (t->myActions==NULL) { // there isn't already an action panel here
		controller->popupInfoPanel(t,TInfoPanel::panel_ACTION);
	}

} // end of TTestInstancePanel::resultClick

void TTestInstancePanel::setObserver(SnapshotFrameObserver* ob)
{
    observer = ob;
}

/** Comparison relation for sorting the worklist entries included in a
  * sample run. Used to sort the entries in order of test name.
  *
  * @param first   the first worklist entry
  * @param second  the second worklist entry
  * @return        true if the first test's name is alphabetically before
  *                that of the second
  */
bool TTestInstancePanel::goCompare(const TTestInstancePanel * first, const TTestInstancePanel * second)
{
	std::string firstname = first->testName;
	std::string secondname = second->testName;
	return (firstname.compare(secondname)<0);   // firstname < secondname
}


/** Returns true if this TTestInstancePanel has this attribute. */
bool TTestInstancePanel::hasAttribute(const std::string & attr)
{
	return (attributes->count(attr)>0 || ids->count(attr)>0);
}

/** Returns the value for the given attribute, or "" otherwise if
  * no such key exists.
  */
std::string TTestInstancePanel::getAttribute(const std::string & attr)
{
	return (attributes->count(attr)>0) ? (*attributes)[attr] : "";
}

/** Returns the value for the given attribute, or "" otherwise if
  * no such key exists.
  */
int TTestInstancePanel::getIntAttribute(const std::string & key)
{
	return (ids->count(key)>0) ? (*ids)[key] : 0;
}


/** Records a string attribute for this instance of a sample being tested. */
void TTestInstancePanel::setAttribute(const std::string & key, const std::string & value)
{
	(*attributes)[key] = value;
}

/** Records an integer attribute for this instance of a sample being tested. */
void TTestInstancePanel::setAttribute(const std::string & key, int value)
{
	(*ids)[key] = value;
}

