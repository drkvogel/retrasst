//---------------------------------------------------------------------------
#include "Utils.h"

#pragma hdrstop
//---------------------------------------------------------------------------

#include <cmath>

#include "ConsoleWriter.h"

#include "ExceptionHandler.h"
#include "API.h"
#include "Nullable.h"
#include "WorklistEntriesView.h"
#include "LogManager.h"
#include "InfoPanels.h"
#include "StrUtil.h"
#include "SnapshotUtil.h"
#include "TActionPanel.h"
#include "TSnapshotFrame.h"

#include "VisualComponents.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------

const bool WorklistEntriesView::QUEUED = true;
const bool WorklistEntriesView::SENT_TO_ANALYSER = false;

/** Initialises the object for presenting a view of worklist entries.
  * This includes setting up links to related controller objects,
  * and initialising some visual display parameters.
  * After construction, this WorklistEntriesView object is in an initialised
  * state, but no worklist entry data from a snapshot has been loaded.
  *
  * @param m                a link back to the main form
  * @param configFilename   the name of the config file for the GUI parameters
  */
WorklistEntriesView::WorklistEntriesView(TSnapshotFrame *sf,
										 LogManager* lm,
										 const paulst::Config* config)
    :
	viewFrame(sf),
    logManager(lm),
    observer(NULL)
{
	initialiseGUIconfigValues(config);
	positioner = new Positioning(this);  // uses a GUI config setting to initialise
	queuedComponents = new ComponentsList();
	resultsComponents = new ComponentsList();
}





/** Initialises the parameters relating to the view of worklist entries.
  * These mostly specify lengths relating to components and their positioning,
  * for example widths, heights, margins, gaps and offsets.
  * Some of the parameters are initialised with values
  * from the GUI config file, and some are calculated from other GUI parameters.
  * These parameters are independent of any data in a database snapshot.
  *
  * The GUI parameters are stored in the GUI config file, the name of which
  * is provided (this pathname was originally retrived from the
  * <code>GUIconfig</code> value from the main config file).
  *
  * @param configFilename   the name of the config file for the GUI parameters
  */
void WorklistEntriesView::initialiseGUIconfigValues(const paulst::Config* config)
{
	// now to extract all the strings from the config, and convert to ints
	paulst::Config::const_iterator iter = config->begin();
	while (iter!=config->end()) {
		  std::string key = iter->first;
          try
          {
		      guiConfig[key] = paulst::toInt(iter->second);
          }
          catch( ... )
          {
          }
		  iter++;
	}


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


void WorklistEntriesView::setObserver(SnapshotFrameObserver *ob)
{
	observer = ob;
}

/** Deletes the WorklistEntriesView, along with its Positioning object. */
WorklistEntriesView::~WorklistEntriesView()
{
	delete positioner;
	delete queuedComponents;
	delete resultsComponents;
}

/** This clears (destroys) all the visual components related to
  * sample runs, and any pop-up information panels.
  * Used to wipe everything related to a display of snapshot data,
  * ready for a fresh snapshot to be loaded.
  */
void WorklistEntriesView::clearAll()
{
	queuedComponents->clear();
	resultsComponents->clear();
	viewFrame->WorkInnerPanel->DeleteChildren();
	viewFrame->ResultsInnerPanel->DeleteChildren();
}

/** A "getter" method providing access to various global GUI parameters.
  *
  * @param name   the name of the parameter required
  * @return       the value of the parameter
  */
int WorklistEntriesView::param(const std::string & name)
{
	return guiConfig[name];
}


/*-------------- beginning of populating snapshot view -----------------*/

/** Sets certain attributes of the given test panel (which represents an
  * instance of a test being carried out on a sample), corresponding to
  * attributes of the worklist entry that it depicts. The attributes set
  * here are common to both queued entries and analysed/ing entries.
  */
void WorklistEntriesView::assignAttributes(TTestPanel *t,
										   const valc::WorklistEntry *entry)
{
	t->setAttribute("Worklist Id",entry->getID());
	t->setAttribute("Test Id",entry->getTestID());
	t->setAttribute("Profile Id",entry->getProfileID());
	t->setAttribute("Project Id",entry->getProjectID());
	t->setAttribute("Machine Id",entry->getMachineID());
	t->setAttribute("Sample Id",entry->getSampleID());
	char st = entry->getStatus();
	std::string status = "";
	status += st;
	t->setAttribute("Status ",status);
}

/** Sets attributes of the given test panel, from the data it needs to represent
  * from the test result corresponding to the given worklist entry.
  */
void WorklistEntriesView::assignAttributes(TSampleRunPanel *runPanel,
										   TTestPanel *t,
										   valc::SnapshotPtr sn,
										   const valc::WorklistEntry *entry,
										   const valc::TestResult *tr,
										   const valc::IDToken & runId)
{
	assignAttributes(t,entry); // set attributes in common
							   // with queued worklist entries
	std::string res = Utils::float2str(tr->getResultValue());
	t->setAttribute("Result",res);
	TDateTime dt = tr->getDateAnalysed();
	std::string d = Utils::unicodestr2str(dt.FormatString("YYYY-MM-DD (ddd)"));
	std::string ti = Utils::unicodestr2str(dt.FormatString("HH:MM"));
	t->setAttribute("Date Analysed",d);
	t->setAttribute("Time Analysed",ti);
	int resultId = tr->getID();
	t->setAttribute("Result Id",resultId);

	// is it a test performed locally on this analyser?
	bool local = tr->getSampleRunID() == runId;
	if (!local) {
        t->setUpNonLocal();
    }

	// alert information needed too
	try {
		if (sn->hasRuleResults(resultId)) {
			valc::RuleResults rr = sn->getRuleResults(resultId);
			// RuleResults has summary information
			//    int getSummaryResultCode()
			//    string getSummaryMsg()

			// each RuleResult has the following:
			//    int resultCode
			//    string rule
			//    string msg

			int r = rr.getSummaryResultCode();
			t->setAttribute("Result Code",r);
			t->setAttribute("Result Summary", rr.getSummaryMsg());
			if (r==valc::ResultCode::RESULT_CODE_FAIL
				|| r==valc::ResultCode::RESULT_CODE_BORDERLINE
				|| r==valc::ResultCode::RESULT_CODE_ERROR) {
				t->needsAttention();
				runPanel->barcodePanel->needsAttention();
            }
        }
	}
	catch (const Exception & e) {
		logManager->logException(std::string("Exception while attempting to get Rule Results. More details:")
								+ AnsiString(e.Message.c_str()).c_str());
	}
	catch (...) {
		logManager->logException("Caught an unspecified kind of exception attempting to get Rule Results.");
	}
	// also include alerts, like   addAlerts(sn,entry,tr->getID());
	//if (a==valc::ResultCode::RESULT_CODE_FAIL
	//	|| a==valc::ResultCode::RESULT_CODE_BORDERLINE
	//	|| a==valc::ResultCode::RESULT_CODE_ERROR) {
}



/** Sets the width, position and parent of the given TTestPanel.
  * (These parameters are calculated earlier, so can't simply be set from
  * a style.)
  */
void WorklistEntriesView::finaliseTestPanel(TTestPanel *t,
											TPanel *parent,
											int x,
											bool queued)
{
	t->Height = param("testResultHeight");
	t->Width = queued ? param("queuedTestWidth") : param("testResultWidth");
	t->Position->X = x;
	t->Position->Y = 0;
	if (!queued && t->resultButton!=NULL) {
		t->resultButton->Position->X = t->Width - param("resultWidth");
		t->resultButton->Parent = t;
	}
	if (t->screen!=NULL) {
	    t->screen->Width = t->Width;
		t->screen->Height = t->Height;
		t->screen->Parent = t;
		t->screen->BringToFront();
    }
	t->Parent = parent;
}


/** This sets the widths of the test result panels and adds them to their
  * parents.
  */
void WorklistEntriesView::postProcessEntries(bool queued)
{
	// set the horizontal distance between bubbles, as calculated earlier
	int separation = queued
					 ? param("horizontalQueuedTestSeparation")
					 : param("horizontalTestResultSeparation");

	// now iterate through the sample runs
	ComponentsIterator iter = queued
							  ? queuedComponents->begin()
							  : resultsComponents->begin();
	ComponentsIterator finish = queued
							  ? queuedComponents->end()
							  : resultsComponents->end();

	while (iter!=finish) {
		TSampleRunPanel *runPanel = (TSampleRunPanel *)(*iter);

		runPanel->testPanels->sort(TTestPanel::goCompare);
		int posX = separation * runPanel->testPanels->size();
		if (runPanel->barcodePanel->closed)  {
			if (runPanel->barcodePanel->needingAttention()) {
				runPanel->testsPanel->StyleLookup = "ClosureAlertPanelStyle";
			}
			else {
                runPanel->testsPanel->StyleLookup = "ClosurePanelStyle";
			}
			runPanel->testsPanel->Width = posX + 3*param("cornerWidth");
			runPanel->testsPanel->Height = param("testResultHeight");
		}
		else {
			runPanel->testsPanel->Width = 10;    // hack for now
			runPanel->testsPanel->Height = 10;   // hack for now
        }
		EntryPanelsReverseIterator riter = runPanel->testPanels->rbegin();
		while (riter!= runPanel->testPanels->rend()) {
			TTestPanel *t = *riter;
			posX -= separation;
			finaliseTestPanel(t,runPanel->testsPanel,posX,queued);
			riter++;
		}
		iter++;
    }

}

/** Sets up an empty sample run panel, with the given owner panel. */
TSampleRunPanel* WorklistEntriesView::createSampleRunPanel(TPanel *parentPanel,
														   int y,
														   bool queued,
														   bool closed)
{
	TSampleRunPanel *srPanel = new TSampleRunPanel(parentPanel);
	// srPanel->StyleLookup = "InvisiblePanelStyle";  // it ignores this
	srPanel->Position->X = 0;
	srPanel->Position->Y = y;
	srPanel->Width = 10;    // these two lines are hacky, but for now, they
	srPanel->Height = 10;   // shrink & hide these panels that refuse to be invisible
	srPanel->Parent = parentPanel;

	// now for the sub panels for this sample run

	// first the panel holding the barcode
	TBarcodePanel *barcodePanel = new TBarcodePanel(this,srPanel,closed,queued);
	srPanel->barcodePanel = barcodePanel;

	// setting up the invisible panel to hold the individual test panels
	TPanel *testsPanel = new TPanel(srPanel);
	testsPanel->Parent = srPanel;
	// testsPanel->StyleLookup = "TestResultsContainerStyle"; // red, but visible=false
	testsPanel->Position->X = barcodePanel->Width
							  - 2*param("cornerWidth");
	testsPanel->Position->Y = 0;
	testsPanel->TagString = "Level:sample run test results";
	srPanel->testsPanel = testsPanel;

	barcodePanel->Parent = srPanel; // needs to be a later-added child to srPanel
									// in order to display it on top

	return srPanel;
}





/** Creates the panels for the individual tests queued for a given sample.
  *
  * @param snapshot          the pointer that refers to the business layer data
  * @param sampleDescriptor  the string that identifies the sample run
  * @param runPanel          the panel for the sample run, on which the panel for
  *                          the tests will be put
  * @param maxRunSize        to hold the max # of tests in a sample run
  * @param maxTestWidth      to hold the max width of a TTestPanel
  *
  * @return the number of worklist entries added
  */
void WorklistEntriesView::addQueuedWorklistEntries(valc::SnapshotPtr sn,
												  const std::string &sampleDescriptor,
												  const std::string &barcode,
												  TSampleRunPanel *runPanel,
												  DisplayProperties &props)
{
    runPanel->barcodePanel->updateBarcode(barcode);
	valc::Range<valc::WorklistEntryIterator> iterPair
			= sn->getWorklistEntries(sampleDescriptor);
	valc::WorklistEntryIterator iter = iterPair.first; //start
	int count = 0;  // for counting the number of relevant worklist entries
	while (iter!=iterPair.second) {  // iterating through the worklist entries

		const valc::WorklistEntry *entry = *iter;
		std::string testName = sn->getTestName(entry->getTestID());
		valc::Range<valc::TestResultIterator> entryResult = entry->getTestResults();
		if (valcui::empty(entryResult)) {
			TTestPanel *t = new TTestPanel(this,runPanel->testsPanel,testName,"",
										   "Level:test result",observer,QUEUED);
			assignAttributes(t,entry);
			runPanel->testPanels->push_back(t);
			count++;
			props.elementSize = std::max(t->findPanelLeftWidth(),props.elementSize);
			// parent of t will be set later on
		}
		// else if it has results, it must be a worklist entry for a non-queued result
		iter++;
	}
	props.rowSize = std::max(count,props.rowSize);
}

/** Goes through all queued sample runs and gets the worklist entries.
  *
  * @param snapshot      the pointer that refers to the business layer data
  * @param props         for holding found characteristics of the dataset,
  *                      such as number of sample runs, max size of a worklist entry
  */
bool WorklistEntriesView::createQueuedEntries(valc::SnapshotPtr snapshot,
											  DisplayProperties &props)
{
	props.numberOfRows = 0;
	props.rowSize = 0;       // for counting max # of entries in a row
	props.elementSize = 60;  // default minimum width

	int y = param("runsPanelverticalMargin");  // starting just below the top

	try {
		valc::QueuedSampleIterator qi = snapshot->queueBegin();   // -> overloaded
		while (qi!=snapshot->queueEnd()) {
			valc::QueuedSample qs = *qi;

			// now construct the panel for the info for this sample run
			TSampleRunPanel *runPanel
				= createSampleRunPanel(viewFrame->WorkInnerPanel,y,QUEUED);

			addQueuedWorklistEntries(snapshot,
									 qs.getSampleDescriptor(),
									 qs.getBarcode(),
									 runPanel,
									 props);

			queuedComponents->push_back(runPanel);


			props.numberOfRows++;
			y += param("verticalRunSeparation");  // go up for this next panel
			qi++;
		}
	}
	catch (const Exception & e) {
		logManager->logException(std::string("Caught an exception (was attempting to load the queued worklist entries). More details:")
								+ AnsiString(e.Message.c_str()).c_str());
		return false;
	}
	catch (...) {
		logManager->logException("Caught an unspecified kind of exception (was attempting to load the queued worklist entries).");
		return false;
	}
	return true;
}


/** Constructs and displays the GUI components for the queued sample tests
  * in the bottom half of the screen. Assumes that any previous GUI components
  * have been cleared already.
  *
  * @param snapshot    the pointer that refers to the business layer data
  */
void WorklistEntriesView::displayWorklistQueue(valc::SnapshotPtr snapshot) {

	viewFrame->WorkInnerPanel->BeginUpdate();
	viewFrame->WorkInnerPanel->TagString = innerLevel;

	DisplayProperties queueProperties;
	if (createQueuedEntries(snapshot,queueProperties)) {
		guiConfig["queuedTestWidth"] = queueProperties.elementSize; // maxQueuedTestWidth;
		int separation = queueProperties.elementSize - 2*guiConfig["cornerWidth"];
		guiConfig["horizontalQueuedTestSeparation"] = separation;
		int maxSampleRunWidth = (queueProperties.rowSize + 1)
								 * queueProperties.elementSize
							     - separation;

		postProcessEntries(QUEUED);

		viewFrame->WorkInnerPanel->Height
					= queueProperties.numberOfRows*param("verticalRunSeparation")
					  - param("sampleRunGap") + 2*param("runsPanelverticalMargin");
		viewFrame->WorkInnerPanel->Width
					= maxSampleRunWidth + param("runsPanelrightMargin");
	}
	viewFrame->WorkInnerPanel->EndUpdate();
	viewFrame->WorkInnerPanel->Repaint();
}



/** Create the panels for the individual tests on a active/closed-off sample run.
  *
  * @param snapshot          the pointer that refers to the business layer data
  * @param sampleDescriptor  the string that identifies the sample run
  * @param runPanel          the panel for the sample run, on which the panel for
  *                          the tests will be put
  * @param props             for holding found characteristics of the dataset,
  *                          such as number of sample runs, max size of a worklist entry
  * @param open              if this is not called for a queued sample,
  *                          the open/closed status of the local run
  * @param runID             the token holding the run id of the sample run,
  *                          or an empty token if no run id (e.g. because queued)
  *
  * @return the number of worklist entries added
  */
void WorklistEntriesView::addNonQueuedWorklistEntries(valc::SnapshotPtr sn,
													  const valc::LocalRun &r,
													  TSampleRunPanel *runPanel,
													  DisplayProperties &props)
											 /* r.getSampleDescriptor(),
											r.getBarcode(),
											r.isOpen(),r.getRunID()
											*/
{
	runPanel->barcodePanel->updateBarcode(r.getBarcode());
	int count = 0;  // # relevant worklist entries found for this sample run
	valc::Range<valc::WorklistEntryIterator> iterPair
			= sn->getWorklistEntries(r.getSampleDescriptor());
	valc::WorklistEntryIterator iter = iterPair.first; // start
	while (iter!=iterPair.second) {  // iterating through the worklist entries
		const valc::WorklistEntry *entry = *iter;
		std::string testName = sn->getTestName(entry->getTestID());
		valc::Range<valc::TestResultIterator> entryResult = entry->getTestResults();
		if (valcui::empty(entryResult)) {
			if (r.isOpen()) {  // this result must be pending
				count++;
				TTestPanel *t = new TTestPanel(this,runPanel->testsPanel,testName,"",
											   "Level:test result",observer,SENT_TO_ANALYSER);
				assignAttributes(t,entry);
				runPanel->testPanels->push_back(t);
				// parent of t is set later on
				props.leftSize = std::max(t->findPanelLeftWidth(),props.leftSize);
			}
			// no test results, therefore queued entry, therefore ignore
		}
		else { // test result(s) exist for this worklist entry
			valc::Range<valc::TestResultIterator> iterPair =  entry->getTestResults();
			valc::TestResultIterator iterR = iterPair.first;
			while (iterR!=iterPair.second) { // going through the results
				const valc::TestResult *tr = *iterR;
				if (tr!=NULL && r.getRunID()==tr->getSampleRunID()) {
					count++;
					TTestPanel *t = new TTestPanel(this,runPanel->testsPanel,testName,
												   Utils::float2str(tr->getResultValue()),
										           "Level:test result",observer,SENT_TO_ANALYSER);
					assignAttributes(runPanel,t,sn,entry,tr,r.getRunID());
					runPanel->testPanels->push_back(t);
					props.leftSize = std::max(t->findPanelLeftWidth(),
											  props.leftSize);
					props.rightSize = std::max(t->findResultDisplayWidth(),
											   props.rightSize);
				}
				iterR++;
			}
		}
		iter++;
	}
	props.rowSize = std::max(count,props.rowSize);
	if (count==0) {
		logManager->log(paulst::format("No worklist entries for sample-run %s",
		                               r.getRunID().token().c_str() ).c_str() );
    }
}


/** Goes through all sample runs sent to the analyser and gets the
  * relevant worklist entries and results.
  *
  * @param snapshot      the pointer that refers to the business layer data
  * @param props         for holding found characteristics of the dataset,
  *                      such as number of sample runs, max size of a worklist entry
  */
bool WorklistEntriesView::createResultsEntries(valc::SnapshotPtr snapshot,
											   DisplayProperties &props)
{
	props.numberOfRows = 0;
	props.rowSize = 0;
	props.leftSize = 40;  // default minimum bubble width
	props.rightSize = param("defaultResultWidth");

	int y = param("runsPanelverticalMargin");  // starting just below the top
	try {

		valc::LocalEntryIterator ri = snapshot->localBegin();
		while (ri!=snapshot->localEnd()) {

			valc::LocalEntry ms = *ri;  // (this could be a sample or a delimiter)
			// note that the type LocalEntry is a typedef for
			// boost::variant<LocalRun,BatchDelimiter>

			if (ms.type()==typeid(valc::LocalRun)) { // is local run (not batch delimiter)
				valc::LocalRun r = boost::get<valc::LocalRun>(ms);


				TSampleRunPanel *runPanel
					= createSampleRunPanel(viewFrame->ResultsInnerPanel,y,
										   SENT_TO_ANALYSER,!r.isOpen());
				addNonQueuedWorklistEntries(snapshot,r,runPanel,props);
				resultsComponents->push_back(runPanel);
				props.numberOfRows++;
				y += param("verticalRunSeparation");  // go up for this next panel
			}
			ri++;
		}
	}
	catch (const Exception & e) {
		logManager->logException(std::string("Caught an exception (was attempting to load the results from worklist entries). More details:")
								+ AnsiString(e.Message.c_str()).c_str());
		return false;
	}
	catch (...) {
		logManager->logException("Caught an unspecified kind of exception (was attempting to load the results from worklist entries).");
		return false;
	}
	return true;

}


/** Constructs and displays the GUI components for the queued sample tests
  * in the bottom half of the screen. Assumes that any previous GUI components
  * have been cleared already.
  *
  * @param snapshot    the pointer that refers to the business layer data
  */
void WorklistEntriesView::displayWorklistResults(valc::SnapshotPtr snapshot)
{
	viewFrame->ResultsInnerPanel->BeginUpdate();
	viewFrame->ResultsInnerPanel->TagString = innerLevel;

	DisplayProperties queueProperties;
	if (createResultsEntries(snapshot,queueProperties)) {
		guiConfig["testResultWidth"] = queueProperties.leftSize
									   + queueProperties.rightSize;
		int separation = guiConfig["testResultWidth"] - 2*guiConfig["cornerWidth"];
		guiConfig["horizontalTestResultSeparation"] = separation;
		guiConfig["resultWidth"] = queueProperties.rightSize;

		postProcessEntries(SENT_TO_ANALYSER);

		viewFrame->ResultsInnerPanel->Height
			= queueProperties.numberOfRows*param("verticalRunSeparation")
			  - param("sampleRunGap") + 2*param("runsPanelverticalMargin");

		int maxSampleRunWidth = (queueProperties.rowSize + 1)
								 * guiConfig["testResultWidth"]  - separation;
		viewFrame->ResultsInnerPanel->Width
			= maxSampleRunWidth + param("runsPanelrightMargin");
	}

	viewFrame->ResultsInnerPanel->EndUpdate();
	viewFrame->ResultsInnerPanel->Repaint();
}

/** Populates the two views (for queued worklist entries, and worklist
  * entries for samples sent to the analyser) from the given snapshot.
  *
  * @param sn   the snapshot supplied by the business layer
  */
void WorklistEntriesView::setUpVisualComponents(valc::SnapshotPtr sn)
{
	displayWorklistQueue(sn);
	displayWorklistResults(sn);


	positioner->positionViews();
	positioner->recordViewsHeights();
}


/*-------------- end of populating snapshot view -----------------*/


/*------- begin Component information retrieval utilities --------*/


/** Finds the ancestor of the given component that is immediately inside the
  * scrollpane. (The ancestor panel will be tagged with the string "inner panel".)
  *
  * @param comp   the component for which to find its inner panel ancestor
  * @return       the inner panel ancestor, as a reference to a TObject
  */
TControl* WorklistEntriesView::findInnerPanelAncestor(TControl *comp)
{  // finds the ancestor panel of comp that is immediately inside
   // the scrollpane ("inner panel")

	TControl *p = comp;

	UnicodeString tag = p->TagString;
	while (tag!=innerLevel) {
		p = (TControl *)p->Parent;
		tag = p->TagString;
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
Coordinates WorklistEntriesView::findTestPanelPosition(const TTestPanel *t)
{
	Coordinates c;
	c.x = t->Position->X;      // position of t relative to its parent
	c.y = t->Position->Y;
	TControl *p = (TControl *)t->Parent;
	UnicodeString tag = p->TagString;
	// need to keep going up whilst p isn't the inner panel itself
	while (tag!=innerLevel) {
		c.x += p->Position->X;
		c.y += p->Position->Y;
		p = (TControl *)p->Parent;
		tag = p->TagString;
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
void WorklistEntriesView::popupInfoPanel(TTestPanel *t, int panel_type)
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
void WorklistEntriesView::removeInfoPanel(TInfoPanel *p)
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



