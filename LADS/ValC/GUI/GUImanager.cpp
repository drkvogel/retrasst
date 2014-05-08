//---------------------------------------------------------------------------
#include "Utils.h"

#pragma hdrstop
//---------------------------------------------------------------------------

#include <cmath>

#include "ConsoleWriter.h"

#include "GUImanager.h"
#include "GUIpositioning.h"
#include "DataManager.h"
#include "DataContainers.h"
#include "InfoPanels.h"
#include "TActionPanel.h"
#include "TSnapshotFrame.h"
#include "VisualComponents.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------

/** Constructs an object for managing the GUI, including setting up links to
  * managers for other parts of the program, and setting the GUI parameters.
  * When this constructor has finished, the GUImanager object is in a state
  * of being initialised, but no data has been loaded, and thus no GUI components
  * representing data are yet in existence.  At this point the only GUI
  * objects in existence are those on the main form.
  *
  * @param m                a link back to the main form
  * @param configFilename   the name of the config file for the GUI parameters
  */
GUImanager::GUImanager(TSnapshotFrame *m, DataManager* dm, LogManager* lm, const std::string & configFilename)
    :
    mainForm(m),
    dataManager(dm),
    logManager(lm),
    observer(NULL)
{
	initialiseGUIconfigValues(configFilename);
	positioner = new Positioning(this);  // uses a GUI config setting to initialise
}

/** Initialises the GUI parameters. These mostly specify lengths relating
  * to components and their positioning, for example widths, heights, margins,
  * gaps and offsets. Some of the parameters are initialised with values
  * from the GUI config file, and some are calculated from other GUI parameters.
  * These parameters are independent of any data in a database snapshot.
  *
  * The GUI parameters are stored in the GUI config file, the name of which
  * is provided (this pathname was originally retrived from the
  * <code>GUIconfig</code> value from the main config file).
  *
  * @param configFilename   the name of the config file for the GUI parameters
  */
void GUImanager::initialiseGUIconfigValues(const std::string & configFilename)
{
	paulst::Config config(paulst::loadContentsOf(configFilename));

	// now to extract all the strings from the config, and convert to ints
	paulst::Config::const_iterator iter = config.begin();
	while (iter!=config.end()) {
		  std::string key = iter->first;
		  guiConfig[key] = paulst::toInt(iter->second);
		  iter++;
	}

	// some variables used for calculation of pane heights ratio for split pane
	recordResultsHeight = guiConfig["initialResultsHeight"];
	recordWorkHeight = guiConfig["initialWorkHeight"];


	// some calculations from existing values

	// barcode bubble height should be the same as that of worklist entry bubbles
	guiConfig["barcodeHeight"] =  guiConfig["testResultHeight"];

	// vertical distance between sample runs in a list
	guiConfig["verticalRunSeparation"] =  guiConfig["sampleRunGap"] + guiConfig["testResultHeight"];

	// use this for a vaguely sensible minimum panel height
	guiConfig["minPanelHeight"] = guiConfig["verticalRunSeparation"];

	// finding out how high a text label is, with default settings
	TLabel *t = new TLabel(NULL);
	t->StyleLookup = "TestResultLabelStyle";
	t->Text =  "Test";
	guiConfig["labelTextHeight"] = (int)t->Height;
	delete t;
}


/** Deletes the GUImanager, along with its Positioning object. */
GUImanager::~GUImanager()
{
	delete positioner;
}

/** This will destroy all the visual components related to
  * sample runs, and any pop-up information panels.
  * This does not destroy the data structures from the snapshot;
  * the data manager takes care of that.
  */
void GUImanager::clearAll()
{
	mainForm->WorkInnerPanel->DeleteChildren();
	mainForm->ResultsInnerPanel->DeleteChildren();
}


/** A "getter" method providing access to various global GUI parameters.
  *
  * @param name   the name of the parameter required
  * @return       the value of the parameter
  */
int GUImanager::param(const std::string & name)
{
	return guiConfig[name];
}

/** Provides the width for this label that was calculated earlier.
  * Assumes that this is a test name or test result label.
  *
  * @param label    the label for which the width is required
  * @todo           adjust this so that it reports a config error if there
  *                 is no such configuration value
  */
int GUImanager::textWidth(const std::string & label)
{
	return textWidths[label];
}

/** Calculates what the width of the given string would be, if displayed
  * using default settings.
  *
  * @param s     the string whose width is to be calculated
  * @return      the width in pixels of the text, if it were to be displayed
  */
int GUImanager::findTextWidth(std::string s)
{
	TText *txt;
	txt = new TText(NULL);
	txt->AutoSize = true;
	txt->Text = Utils::str2unicodestr(s);
	int w = (int)txt->Width;
	delete txt;
	return w;
}


/** Notes the width of text labels for this entry.
  *
  * @param e         the worklist entry
  * @param queued    true if this is a queued worklist entry
  */
void GUImanager::setTextWidths(const DSampleTest & e, bool queued)
{
	std::string name = e.displayTestName;
	if (textWidths.count(name)==0) {
		textWidths[name] = findTextWidth(name);
	 //	logManager->log("Added text width for \" " + name + " \": "
	 //						+ Utils::int2str(textWidths[name]));
	}

	if (!queued) {
		std::string result = e.getDefaultResult();
		if (textWidths.count(result)==0) {
			textWidths[result] = findTextWidth(result);
	  //		logManager->log("Added text width for result \"" + result + "\": "
	  //						+ Utils::int2str(textWidths[result]));
		}
	}
}

/*---------------------- begin Main GUI setting up -------------------*/

/** Sets up GUI components to correspond to the worklist entries for a particular
  * sample run. This assumes that the data structures for the worklist entries
  * have been already been set up earlier.
  *
  * @param s      the sample run for which the worklist entries are to be fetched
  * @param panel  the panel which will be the parent of the panels for the individual worklist entries
  * @param q      true if these are queued worklist entries
  * @param w      to hold the width required for this panel containing worklist entries
  */
void GUImanager::setUpWorklistEntryPanels(DSampleRun *s, TPanel *panel, bool q, int *w) {

	int n = (int)s->testsOnSample.size();
	int separation;
	int pWidth;
	if (q) {
		separation = param("horizontalQueuedTestSeparation");
		pWidth = param("queuedTestWidth");
	}
	else {
		separation = param("horizontalTestResultSeparation");
		pWidth = param("testResultWidth");
	}

	int posX = n * separation;
	*w = posX - separation + pWidth;  // calculate full width required for this panel

	// reminder: worklistEntries is a std::list<DSampleTest>
	DSampleTestReverseIterator iter = s->testsOnSample.rbegin();
	while (iter!=s->testsOnSample.rend()) {
		posX -= separation;
		DSampleTest *st = *iter;
		TTestPanel *p = new TTestPanel(this,panel,st,posX,q);
        p->setObserver( observer );
		p->TagString = "Level:test result";
		panel->AddObject(p);
		p->Parent = panel;
		iter++;
	}
}



/** Retrieves and displays the sample runs/QCs that are pending/completed
  * (in the top half of the screen).
  * Assumes that previous GUI components has been cleared already.
  */
void GUImanager::makeSamplesResultsDisplay() {

	mainForm->ResultsInnerPanel->BeginUpdate();

	mainForm->ResultsInnerPanel->TagString = innerLevel;
	int n = (int)dataManager->resultslist->size();
	int max_x = 500;  // will be used for window width
	int y = param("runsPanelverticalMargin") + n * param("verticalRunSeparation");  // start at the bottom

	DSampleRunReverseIterator iter = dataManager->resultslist->rbegin();
	while (iter!=dataManager->resultslist->rend()) {

		DSampleRun *s = *iter;
		y -= param("verticalRunSeparation");  // go up for this next panel

		// deal with batch delimiters later... which will involve fiddly y thingies

		// sampleResultsI is the row on which information
		// pertaining to this sample will be displayed
		TPanel *sampleResultsI = new TPanel(mainForm->ResultsInnerPanel);
		sampleResultsI->StyleLookup = "InvisiblePanelStyle";
		sampleResultsI->Position->X = param("barcodePanelOffset");
		sampleResultsI->Position->Y = y;
		sampleResultsI->Visible = true;
		sampleResultsI->Opacity = 1;
		sampleResultsI->TagString = "Level:sample run";

		// label this item with its list index, if needed for debugging
		// std::string idStr = Utils::int2str(i);
		// queuedI->Text = Utils::str2unicodestr(idStr);

		// now constructing the panels for this sample and the associated entries
		TBarcodePanel *barcodeP = new TBarcodePanel(this,sampleResultsI,s,false); // memory management todo

		// now to set up the invisible panel on the right for the list
		TPanel *testsP = new TPanel(sampleResultsI);    // memory management todo
		testsP->StyleLookup = "TestResultsContainerStyle";
		testsP->Height = param("testResultHeight");
		testsP->Position->X = barcodeP->Width - 2*param("cornerWidth");
		testsP->Position->Y = 0;
		testsP->TagString = "Level:sample run test results";

		int w;
		setUpWorklistEntryPanels(s,testsP,false,&w);  // test results info for this sample/descriptor
		testsP->Width = w;
		if (testsP->Position->X + w > max_x) {
			max_x = (int)(testsP->Position->X + w);
		}

		testsP->Parent = sampleResultsI;      // set parent last, GUI likes this(!?)
		// link the barcodepanel into this TListBoxItem for this sample
		sampleResultsI->AddObject(barcodeP);

		mainForm->WorkInnerPanel->AddObject(sampleResultsI);
		// link the row back to its parent
		sampleResultsI->Parent = mainForm->ResultsInnerPanel;

		iter++;
	}
	mainForm->ResultsInnerPanel->Height = n*param("verticalRunSeparation")
										  - param("sampleRunGap")
										  +2*param("runsPanelverticalMargin");
	mainForm->ResultsInnerPanel->Width = max_x + param("runsPanelrightMargin");
	mainForm->ResultsInnerPanel->EndUpdate();
	mainForm->ResultsInnerPanel->Repaint();


}

/** Retrieves and displays the samples that are queued (in the bottom half
  * of the screen). Assumes that any previous GUI components have been
  * cleared already.
  */
void GUImanager::makeWorklistQueueDisplay() {

	mainForm->WorkInnerPanel->BeginUpdate();
	mainForm->WorkInnerPanel->TagString = innerLevel;

	int n = (int)dataManager->worklist->size();
	int y = param("runsPanelverticalMargin") + n * param("verticalRunSeparation");  // start at the bottom
	int max_x = 500;  // will be used for window width

	DSampleRunReverseIterator iter = dataManager->worklist->rbegin();
	while (iter!=dataManager->worklist->rend()) {

		DSampleRun *s = *iter;
		y -= param("verticalRunSeparation");  // go up for this next panel

		// queuedI is the row on which information
		// pertaining to this sample will be displayed
		TPanel *queuedI = new TPanel(mainForm->WorkInnerPanel);
		queuedI->StyleLookup = "InvisiblePanelStyle";
		queuedI->Position->X = 0;
		queuedI->Position->Y = y;
		queuedI->Visible = true;
		queuedI->Opacity = 1;
		queuedI->TagString = "Level:sample run";


		// now constructing the panels for this sample and the associated entries
		TBarcodePanel *barcodeP = new TBarcodePanel(this,queuedI,s,true); // need to manage the memory used here

		// now to set up the invisible panel on the right for the list
		TPanel *testsP = new TPanel(queuedI);   // memory management todo
		testsP->StyleLookup = "TestResultsContainerStyle";
		testsP->Height = param("testResultHeight");
		testsP->Position->X = barcodeP->Width - 2*param("cornerWidth");
		testsP->Position->Y = 0;
		testsP->TagString = "Level:sample run test results";

		int w;
		setUpWorklistEntryPanels(s,testsP,true,&w);  // test info for this sample/descriptor
        		testsP->Width = w;
		if (testsP->Position->X + w > max_x) {
			max_x = (int)(testsP->Position->X + w);
		}

		testsP->Parent = queuedI;      // set parent last, GUI likes this(!?)
		// link the barcodepanel into this TPanel for this sample
		queuedI->AddObject(barcodeP);

		mainForm->WorkInnerPanel->AddObject(queuedI);
		// link the TPanel back to its parent
		queuedI->Parent = mainForm->WorkInnerPanel;

        iter++;
	}
	mainForm->WorkInnerPanel->Height = n*param("verticalRunSeparation")
									   - param("sampleRunGap")
									   +2*param("runsPanelverticalMargin");
	mainForm->WorkInnerPanel->Width = max_x + + param("runsPanelrightMargin");
	mainForm->WorkInnerPanel->EndUpdate();
	mainForm->WorkInnerPanel->Repaint();
}

void GUImanager::positionView()
{   // sets the user's view to a suitable position

	// the default for the bottom panel (queued entries) is fine (at the top),
	// so we set the viewport position of the results to something suitable,
	// currently at the bottom of the scrollbox
	mainForm->ResultsScrollBox->BeginUpdate();
	TPointF pos;
	pos.x = 0.0;
	pos.y = mainForm->ResultsInnerPanel->Height; // this position right at the end
												 // of the panel being viewed makes
	                                             // it go down to the bottom

	// was mainForm->ResultsInnerPanel->Height - mainForm->ResultsPanel->Height;
	mainForm->ResultsScrollBox->ViewportPosition = pos;
	mainForm->ResultsScrollBox->EndUpdate();
	mainForm->ResultsScrollBox->Repaint();

}

void GUImanager::doGUIcalculations()
{	// calculates what sizes to make the top & bottom bubbles;
	// this is dependent on the data in the snapshot

	int topPanelSize = 100;   // default minimum

	// work out top half of the screen first
	DSampleRunIterator iter = dataManager->resultslist->begin();
	while (iter!=dataManager->resultslist->end()) {

		DSampleRun *s = *iter;
		// reminder: worklistEntries is a std::list<DSampleTest *>
		DSampleTestIterator iter2 = s->testsOnSample.begin();
		while (iter2!=s->testsOnSample.end()) {

			DSampleTest *e = (*iter2);

			// record text labels
			setTextWidths(*e,false);

			// also, record compact width of this entry as a panel
			int w = TTestPanel::findPanelWidth(this,e,false);
			if (w>topPanelSize) {
				topPanelSize = w;
			}
			/*
			   logManager->log("Width of panel for \" " + e->testName + " \" and "
								+ e->getDefaultResult() + ": "
								+ Utils::int2str(w));
			*/
			iter2++;
		}
		iter++;
	}

	int bottomPanelSize = 60;   // default minimum

	// work out top half of the screen first
	iter = dataManager->worklist->begin();
	while (iter!=dataManager->worklist->end()) {

		DSampleRun *s = *iter;
		// reminder: worklistEntries is a std::list<DSampleTest>
		DSampleTestIterator iter2 = s->testsOnSample.begin();
		while (iter2!=s->testsOnSample.end()) {

			DSampleTest *e = (*iter2);

			// record text labels
			setTextWidths(*e,false);

			// also, record compact width of this entry as a panel
			int w = TTestPanel::findPanelWidth(this,e,true);
			if (w>bottomPanelSize) {
				bottomPanelSize = w;
			}
			/*
			   logManager->log("Width of queued panel for \" " + e->testName + " \": "
								+ Utils::int2str(w));
			*/
			iter2++;
		}
		iter++;
	}
	// log("Width of top panels to be "
	//							+ Utils::int2str(topPanelSize));
	// log("Width of bottom panels to be "
	//							+ Utils::int2str(bottomPanelSize));

	// bubble widths
	guiConfig["testResultWidth"] = topPanelSize;
	guiConfig["queuedTestWidth"] = bottomPanelSize;

	// horizontal distances between worklist entry bubbles (specifies amount of overlap)
	guiConfig["horizontalTestResultSeparation"]
		= guiConfig["testResultWidth"] - 2*guiConfig["cornerWidth"];
	guiConfig["horizontalQueuedTestSeparation"]
		= guiConfig["queuedTestWidth"] - 2*guiConfig["cornerWidth"];


}

void GUImanager::setObserver( SnapshotFrameObserver *o )
{
    observer = o;
}

void GUImanager::setUpGUIcomponents()
{
	doGUIcalculations();

	makeSamplesResultsDisplay();  // for the top half of the screen
	makeWorklistQueueDisplay();   // for the bottom half of the screen

	positionView(); // set the user's view to a suitable position
    recordPanelHeights();
}

/*---------------------- end Main GUI setting up -------------------*/

/*------------------- begin Split Pane manoeuvrings ----------------*/

/** Records the current heights of the panels affected by the main
  * split pane. This is so that later on, if the window is resized, the
  * proportions of the splitting can be maintained.
  */
void GUImanager::recordPanelHeights()
{
		recordResultsHeight = mainForm->ResultsPanel->Height;
		recordWorkHeight = mainForm->WorkPanel->Height;
}

/** This keeps the ratio of top:bottom pane heights, and is called when
  * the main window gets resized. (Without this, the resizing would
  * just affect the top pane, in a non-intuitive fashion.)
  *
  * @todo   when adjusting, make it look like the splitter is sticky to the
  *         top of the worklist and the bottom of the active/results list,
  *         like anchors
  */
void GUImanager::adjustPanelHeights()
{
	// Was having problems when the split pane was adjusted in too
	// extreme a ratio. Turns out that when you make the top pane
	// pretty large compared to the bottom pane, then resize to a smaller
	// size, the height of the bottom pane has already got set to zero
	// by the time that this method sees it.
	// So have to work things out based on other things, not
	// mainForm->WorkPanel->Height


	float oldTotalHeight = recordResultsHeight + recordWorkHeight;

	float newResultsHeight =  mainForm->ResultsPanel->Height;
	float newWorkHeight =  mainForm->WorkPanel->Height;
	float totalHeight = newResultsHeight+newWorkHeight;


	if (totalHeight>0) {
		float improvedResultsHeight = recordResultsHeight * totalHeight
									  / oldTotalHeight;
		if (improvedResultsHeight<param("minPanelHeight")) {
			 improvedResultsHeight = (float)param("minPanelHeight");
		}

		mainForm->ResultsPanel->Height = improvedResultsHeight;
		mainForm->WorkPanel->Height = totalHeight - improvedResultsHeight;

		recordPanelHeights();
	}

}

/*------------------- end Split Pane manoeuvrings ----------------*/

/*------- begin Component information retrieval utilities --------*/

UnicodeString GUImanager::getComponentTagString(const TObject *comp)
{  // assumes that comp is a component (control) of some kind

	if (comp->ClassNameIs("TPanel")) {
		TPanel *p = (TPanel *)comp;
		return p->TagString;
	}
	// I'll add more cases if/when I use more components than panels
	else {
		return "";
	}
}

Coordinates GUImanager::getComponentPosition(const TObject *comp)
{  // assumes that comp is a component (control) of some kind

	Coordinates pos;
	if (comp->ClassNameIs("TPanel")) {
		TPanel *p = (TPanel *)comp;
		pos.x = p->Position->X;
		pos.y = p->Position->Y;
		return pos;
	}
	if (comp->ClassNameIs("TTestPanel")) {
		TTestPanel *p = (TTestPanel *)comp;
		pos.x = p->Position->X;
		pos.y = p->Position->Y;
		return pos;
	}
	// I'll add more cases if/when I use more components than panels
	else {
		pos.x = 0;
		pos.y = 0;
		return pos;
	}
}

TObject* GUImanager::getComponentParent(const TObject *comp)
{  // assumes that comp is a component (control) of some kind

	if (comp->ClassNameIs("TPanel")) {
		TPanel *p = (TPanel *)comp;
		return p->Parent;
	}
	if (comp->ClassNameIs("TTestPanel")) {
		TTestPanel *p = (TTestPanel *)comp;
		return p->Parent;
	}
	// I'll add more cases if/when I use more components than panels
	else {
		return NULL;
	}
}

/** Finds the ancestor of the given component that is immediately inside the
  * scrollpane. (The ancestor panel will be tagged with the string "inner panel".)
  *
  * @param comp   the component for which to find its inner panel ancestor
  * @return       the inner panel ancestor, as a reference to a TObject
  */
TObject* GUImanager::findInnerPanelAncestor(TObject *comp)
{  // finds the ancestor panel of comp that is immediately inside
   // the scrollpane ("inner panel")

	TObject *p = comp;
	UnicodeString tag = getComponentTagString(p);
	while (tag!=innerLevel) {
		p = getComponentParent(p);
		tag = getComponentTagString(p);
	}
	return p;
}

/** Finds the position of the given panel relative to its ancestor panel
  * immediately inside the scrollpane. (The ancestor panel will be tagged with
  * the string "inner panel".)
  *
  * @param t    the panel for which to find its relative position
  * @return     the coordinates of the panel's top left corner
  */
Coordinates GUImanager::findTestPanelPosition(const TTestPanel *t)
{
	Coordinates c;
	c.x = t->Position->X;      // position of t relative to its parent
	c.y = t->Position->Y;
	TObject *p = t->Parent;
	UnicodeString tag = getComponentTagString(p);
	// need to keep going up whilst p isn't the inner panel itself
	while (tag!=innerLevel) {
		Coordinates pos = getComponentPosition(p);
		c.x += pos.x;
		c.y += pos.y;
		p = getComponentParent(p);
		tag = getComponentTagString(p);
	}
	return c;
}

/*-------- end Component information retrieval utilities ---------*/




/*------------- begin Info Panel popping-ups ----------------*/

/** Pops up an information panel, on the relevant scrollview.
  *
  * @param t            the testpanel containing the information that the information panel will point to
  * @param panel_type   the type of panel to put up (e.g. alert, notes)
  */
void GUImanager::popupInfoPanel(TTestPanel *t, int panel_type)
{   // (can assume that the relevant panel is not already displayed for t,
	// as this was checked earlier)

	// first, get the panel that is just inside the scrollpane
	TPanel *ip = (TPanel *)(findInnerPanelAncestor(t)); // e.g. ResultsInnerPanel
	ip->BeginUpdate();
	
	// choose which panel to put up
	if (panel_type==TInfoPanel::panel_BASIC) {
		t->myBasicInfo = new TBasicInfoPanel(this,ip,t); // see removeInfoPanel for deletion
		positioner->positionInfoPanel(t->myBasicInfo); // positions panel intelligently
		t->myBasicInfo->Parent = ip;
	}
	else if (panel_type==TInfoPanel::panel_NOTES) {

		// now to put a notes panel on the big panel that the scrollpane sees
		t->myNotes = new TNotesPanel(this,ip,t); // see removeInfoPanel for deletion
		positioner->positionInfoPanel(t->myNotes);  // positions panel intelligently
		t->myNotes->Parent = ip;
	}
	else if (panel_type==TInfoPanel::panel_ACTION) {

		// now to put an action panel on the big panel that the scrollpane sees
		t->myActions = new TActionPanel(this,ip,t); // see removeInfoPanel for deletion
		positioner->positionInfoPanel(t->myActions); // positions panel intelligently
		t->myActions->Parent = ip;
	}

	ip->EndUpdate();
	ip->Repaint();
}

/** Removes the given information panel.  Also destroys the memory that it used.
  *
  * @param p            the TInfoPanel to be removed
  */
void GUImanager::removeInfoPanel(TInfoPanel *p)
{
	TTestPanel *t = p->originator;
	TPanel *ip = (TPanel *)(findInnerPanelAncestor(t)); // e.g. ResultsInnerPanel
	ip->BeginUpdate();

	if (p->panel_type==TInfoPanel::panel_BASIC) {
		t->myBasicInfo = NULL;
	}
	else if (p->panel_type==TInfoPanel::panel_NOTES) {
		t->myNotes = NULL;
	}
	else if (p->panel_type==TInfoPanel::panel_ACTION) {
		t->myActions = NULL;
	}
	ip->RemoveObject(p);
	delete p;
	ip->EndUpdate();
	ip->Repaint();
}
/*------------- end Info Panel popping-ups ----------------*/



