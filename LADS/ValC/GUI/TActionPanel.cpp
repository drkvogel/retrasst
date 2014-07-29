//---------------------------------------------------------------------------

#pragma hdrstop

#include "ConsoleWriter.h"

#include "API.h"

#include "VisualComponents.h"
#include "TTestInstancePanel.h"
#include "WorklistEntriesView.h"
#include "TActionPanel.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//

static inline void ValidCtrCheck(TActionPanel *)
{
	new TActionPanel(NULL,NULL,NULL);
}


__fastcall TActionPanel::~TActionPanel()
{
	delete iconPanel;           // need to delete child components before parent,
	delete testNameLabel;      //  otherwise nasty pointer crashes happen,
	delete testNameDisplay;    //  so delete labels & iconPanel before topArea
	delete testResultLabel;
	delete testResultDisplay;
	delete testTimeLabel;
	delete testTimeDisplay;
	delete testDateLabel;
	delete testDateDisplay;
	delete topArea;

	delete alertListBox;
	delete leftArea;

	delete triggerPlaceholder;
	delete rightArea;

	delete mainArea;

}


/** Constructs panel that pops up when a test result is clicked on.
  * Later, it will contain alert information (if any), and (not implemented yet)
  * will contain action opportunities like scheduling a rerun. The triangle indicator on this callout-shaped
  * panel will point to the test result that was clicked on, and its
  * display preference is further down the screen relative to its test result.
  *
  * This does not put the contents into the panel; that is done later on once
  * the panel has been positioned (positioning it changes its size).
  *
  * @param g        the object that manages the GUI
  * @param owner   the component (if any, could be NULL) that will own this panel
  * @param t        the test panel from which the alert icon was clicked, in order to bring up this panel in the first place
  */
__fastcall TActionPanel::TActionPanel(WorklistEntriesView *g, TComponent *owner, TTestInstancePanel *t)
	: TInfoPanel(g,owner,t,panel_ACTION,
				 Positioning::BOTTOM,Positioning::CENTRE)
{
	StyleLookup = "InfoPanelStyle";  // no red heavy outline, by default

	if (originator->hasAttribute("Result Code")) {
		int a = originator->getIntAttribute("Result Code");
		if (a==valc::ResultCode::RESULT_CODE_FAIL
			|| a==valc::ResultCode::RESULT_CODE_BORDERLINE
			|| a==valc::ResultCode::RESULT_CODE_ERROR) {
			StyleLookup = "AlertPanelStyle";
		}
	}

	innerSize.height = calculateHeight(t);
	innerSize.width = gui->param("actionPanelWidth");
	Visible = true;
}


/** Initialises the contents of this pop-up panel. Called after the panel is
  * positioned. This positions some components that depend on the panel
  * positioning, and initialises the contents of the panel's inner area.
  * Includes a place to put an icon and a list of alerts, currently.
  *
  * @see WorklistEntriesView::positionInfoPanel
  * @todo  display suitable alert icons for error, and no rules applied
  */
void TActionPanel::initialiseContents()
{
	xButton->OnClick = removeClick;

	initialiseTopArea();   // top area has aligment alTop

	// mainArea is the area below, taking up the rest of the innerArea
	mainArea = new TLayout(innerArea);
	mainArea->Align = TAlignLayout::alClient;
	mainArea->Width = innerArea->Width;
	mainArea->Parent = innerArea;

	// initialiseLeftArea();
	// initialiseRightArea();
}



/** Initialises the area at the top, including icon and test result summary. */
void TActionPanel::initialiseTopArea()
{

	// topArea is the section all along the top of the inner area.
	// It will hold an icon on the left and some text on the right.
	topArea = new TLayout(innerArea);
	topArea->Position->X = 0;
	topArea->Position->Y = 0;
	topArea->Align = TAlignLayout::alTop;
	topArea->Height = gui->param("panelTopAreaHeight");
	topArea->Parent = innerArea;

	// iconPanel contains the icon indicating pass/fail/maybe
	iconPanel = new TPanel(topArea);
	if (originator->isQC() && originator->hasAttribute("Result Code")) {
		int a = originator->getIntAttribute("Result Code");
		if (a==valc::ResultCode::RESULT_CODE_FAIL) {
			iconPanel->StyleLookup = "AlertPanelFailIconStyle";
		}
		else if (a==valc::ResultCode::RESULT_CODE_BORDERLINE) {
			iconPanel->StyleLookup = "AlertPanelMaybeIconStyle";
		}
		else if (a==valc::ResultCode::RESULT_CODE_ERROR) {
			iconPanel->StyleLookup = "AlertPanelErrorIconStyle";
		}
		else if (a==valc::ResultCode::RESULT_CODE_PASS) {
			iconPanel->StyleLookup = "AlertPanelPassIconStyle";
		}
	}
	else {
        iconPanel->StyleLookup = "AlertPanelBlankIconStyle";
    }

	iconPanel->Position->X = 0;
	iconPanel->Position->Y = 0;
	iconPanel->Width = gui->param("resultIconWidth")
					  + 2*gui->param("textMargin");
	iconPanel->Align = TAlignLayout::alMostLeft;  // ensures it grabs top-left corner
	iconPanel->Parent = topArea;

	initialiseTestResultLabels();


}
/** Constructs labels at the top for the test results. e.g. like this:
  *
  * Test: DLDL.   Result: 3.56
  * <br>
  * When: 08:42 on 2013-11-26 (Tue)
  *
  * (Assumes that the icon area to the left has already been initialised.)
  */
void TActionPanel::initialiseTestResultLabels()
{
	// (uses alNone and manual positioning because the provided
	// layout/alignment options don't suit)

	// displaying the name of the test
	testNameLabel = new TLabel(topArea);
	testNameLabel->StyleLookup = "PlainLabelStyle";
	gui->positioner->fitText(testNameLabel,"Test: ");
	testNameLabel->Align = TAlignLayout::alNone;
	testNameLabel->Position->X = iconPanel->Width;
	testNameLabel->Position->Y = 0;
	testNameLabel->Parent = topArea;

	testNameDisplay = new TLabel(topArea);
	testNameDisplay->StyleLookup = "PlainDisplayStyle";
	gui->positioner->fitText(testNameDisplay,originator->testName);
	gui->positioner->putRightOf(testNameLabel,testNameDisplay);
	testNameDisplay->Parent = topArea;

	// displaying the result of the test
	testResultLabel = new TLabel(topArea);
	testResultLabel->StyleLookup = "PlainLabelStyle";
	gui->positioner->fitText(testResultLabel,"   Result: ");
	gui->positioner->putRightOf(testNameDisplay,testResultLabel);
	testResultLabel->Parent = topArea;

	testResultDisplay = new TLabel(topArea);
	testResultDisplay->StyleLookup = "PlainDisplayStyle";
	gui->positioner->fitText(testResultDisplay,originator->testResult);
	gui->positioner->putRightOf(testResultLabel,testResultDisplay);
	testResultDisplay->Parent = topArea;

	// displaying the timestamp
	testTimeLabel = new TLabel(topArea);
	testTimeLabel->StyleLookup = "PlainLabelStyle";
	gui->positioner->fitText(testTimeLabel,"When: ");
	gui->positioner->putLowerThan(testNameLabel,testTimeLabel);
	testTimeLabel->Parent = topArea;

	testTimeDisplay = new TLabel(topArea);
	testTimeDisplay->StyleLookup = "PlainDisplayStyle";
	gui->positioner->fitText(testTimeDisplay,originator->getAttribute("Time Analysed"));
	gui->positioner->putRightOf(testTimeLabel,testTimeDisplay);
	testTimeDisplay->Parent = topArea;

	testDateLabel = new TLabel(topArea);
	testDateLabel->StyleLookup = "PlainLabelStyle";
	gui->positioner->fitText(testDateLabel," on ");
	gui->positioner->putRightOf(testTimeDisplay,testDateLabel);
	testDateLabel->Parent = topArea;

	testDateDisplay = new TLabel(topArea);
	testDateDisplay->StyleLookup = "PlainDisplayStyle";
	testDateDisplay->Text = Utils::str2unicodestr(originator->getAttribute("Date Analysed"));
	gui->positioner->putRightOf(testDateLabel,testDateDisplay);
	testDateDisplay->Parent = topArea;

}

// (not currently in use)
void TActionPanel::initialiseLeftArea()
{
	leftArea = new TLayout(mainArea);
	leftArea->Align = TAlignLayout::alLeft;
	leftArea->Width = (mainArea->Width)/2;
	leftArea->Parent = mainArea;

	TLabel *ruleApplicationsLabel = new TLabel(leftArea);
	ruleApplicationsLabel->Text = "Rule applications:";
	ruleApplicationsLabel->StyleLookup = "PlainLabelStyle";
	ruleApplicationsLabel->Align = TAlignLayout::alTop;
	ruleApplicationsLabel->Parent = leftArea;

	displayAlertItems();
}



// Constructs labels and icons to show all (if any) of the alerts for
// this test result. e.g. like this:      3:2s O  Pass
// (not currently in use)
void TActionPanel::displayAlertItems()
{

	// then we need a list box
	alertListBox = new TListBox(leftArea);
	alertListBox->StyleLookup = "InfoListStyle";
	alertListBox->Align = TAlignLayout::alClient;
	alertListBox->Width = leftArea->Width;

	// If I uncomment this, it makes the height of the alertListBox
	// 50 whether the height of the mainArea is 60 or 40 ?!?!?!
	// alertListBox->Height = mainArea->Height;
	alertListBox->Parent = leftArea;

	int height = gui->param("panelAlertItemHeight");    // pre-fetch

	/*
	// and some alert items to put in it   (for some reason word wrap doesn't work here)
	std::list<DAlert>::const_iterator iter = originator->alerts.begin();
	while (iter!=originator->entry->alerts.end()) {

		const DAlert *a = &*iter;
		TListBoxItem *item = new TListBoxItem(alertListBox);
		item->StyleLookup = "AlertListBoxItemStyle";

		item->Height = height;

		// this next bit is a hacky way of indenting, but should work
		// in the case that the alert messages wrap their words
		item->WordWrap = true;
		int tinySize = gui->param("alertTinyIconSize");
		std::string buffer = "   ";
		while (Utils::findTextWidth(buffer)<tinySize+2) {
			buffer += " ";
		}
		item->Text = Utils::str2unicodestr(buffer + "[" + a->rule + "] " + a->message);




		TPanel *icon = new TPanel(item);
		if (a->code==valc::ResultCode::RESULT_CODE_FAIL) {
			icon->StyleLookup = "AlertTinyFailIconStyle";
			//  need to use a set   item->Font->Style = TFontStyle::fsBold;
			item->Font->Family = "Arial Narrow";
		}
		else if (a->code==valc::ResultCode::RESULT_CODE_PASS) {
			icon->StyleLookup = "AlertTinyPassIconStyle";
		}
		else if (a->code==valc::ResultCode::RESULT_CODE_BORDERLINE) {
			icon->StyleLookup = "AlertTinyMaybeIconStyle";
		}
		else if (a->code==valc::ResultCode::RESULT_CODE_ERROR) {
			icon->StyleLookup = "AlertTinyErrorIconStyle";
		}
		icon->Align = TAlignLayout::alNone;

		icon->Position->X = 0;
		icon->Position->Y = (height - tinySize)/2;
		icon->Height = tinySize;
		icon->Width = icon->Height;
		icon->Parent = item;

		item->Parent = alertListBox;

		iter++;
	} */
}

// (not currently in use)
void TActionPanel::initialiseRightArea()
{
	rightArea = new TLayout(mainArea);
	rightArea->Align = TAlignLayout::alClient;
	rightArea->Parent = mainArea;

	triggerPlaceholder = new TLabel(rightArea);
	triggerPlaceholder->Align = TAlignLayout::alLeft;
	triggerPlaceholder->Text = "<placeholder for reruns, triggers etc.>";
	triggerPlaceholder->StyleLookup = "DeveloperStyle";
	triggerPlaceholder->Parent = rightArea;
}


/** Calculates how tall this panel should be, to fit its contents.
  * Uses a maximum size (the GUI config parameter "panelAlertItemHeight")
  * if the contents are too lengthy.
  *
  * @param t     the TTestInstancePanel which this alert panel will appear to point to
  * @return      the height that this panel is to have
  */
int TActionPanel::calculateHeight(TTestInstancePanel *t)
{

// TODO  make for rectangleheight, not with the triangle  (that we know)

	int n = 0; // TKTKTK    t->entry->alerts.size();
	int h = gui->param("calloutTriangleHeight")
			+ 3*gui->param("edgeGap")
			+ gui->param("panelTopAreaHeight")
			+ n*gui->param("panelAlertItemHeight");
	if (h>gui->param("actionPanelHeight")) {
		return gui->param("actionPanelHeight");
	}
	else {
		return h;
	}
}


void __fastcall TActionPanel::removeClick(TObject *Sender) {

	TSpeedButton *x = (TSpeedButton *)Sender; // x is the button clicked on
	TLayout *a = (TLayout *)(x->Parent);      // a is the innerArea of the panel
	TActionPanel *p = (TActionPanel *)(a->Parent); // p is the panel it belongs to

	if (p!=NULL) {
		gui->removeInfoPanel(p);
	}


} // end of TActionPanel::removeClick



