//---------------------------------------------------------------------------
#include "Utils.h"

#pragma hdrstop
//---------------------------------------------------------------------------

#include <cmath>

#include "ConsoleWriter.h"

#include "ExceptionHandler.h"
#include "WorklistEntriesView.h"
#include "GUIpositioning.h"
#include "LogManager.h"
#include "InfoPanels.h"
#include "StrUtil.h"
#include "TActionPanel.h"
#include "TSnapshotFrame.h"
#include "VisualComponents.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------

bool WorklistEntriesView::QUEUED = true;
bool WorklistEntriesView::SENT_TO_ANALYSER = false;

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
										 const std::string & configFilename)
    :
	viewFrame(sf),
    logManager(lm),
    observer(NULL)
{
	initialiseGUIconfigValues(configFilename);
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
void WorklistEntriesView::initialiseGUIconfigValues(const std::string & configFilename)
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


/*---------------------- begin Main GUI setting up -------------------*/

/** Sets attributes of the given test panel, from the data it needs to represent
  * of the worklist entry that it depicts.  These attributes are common to both
  * queued entries and entries sent to the analyser.
  */
void WorklistEntriesView::assignAttributes(TTestPanel *t,
										   const valc::WorklistEntry *entry,
										   bool queued)
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
	// if (!queued) {  // must be pending

	// }
}

/** Sets attributes of the given test panel, from the data it needs to represent
  * from the test result corresponding to the given worklist entry.
  */
void WorklistEntriesView::assignAttributes(TSampleRunPanel *runPanel,
										   TTestPanel *t,
										   valc::SnapshotPtr sn,
										   const valc::WorklistEntry *entry,
										   const valc::TestResult *tr,
										   const valc::IDToken &runId)
{
	assignAttributes(t,entry,QUEUED); // set attributes in common with queued entries
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



/** Adds the panels for the individual tests on a sample run.
  *
  * @param snapshot          the pointer that refers to the business layer data
  * @param sampleDescriptor  the string that identifies the sample run
  * @param runPanel          the panel for the sample run, on which the panel for
  *                          the tests will be put
  * @param maxRunSize        to hold the max # of tests in a sample run
  * @param maxTestWidth      to hold the max width of a TTestPanel
  * @param queued            will be true if this is a queued sample run

  * @return the number of worklist entries added
  */
int WorklistEntriesView::addWorklistEntries(valc::SnapshotPtr sn,
											 const std::string & sampleDescriptor,
											 TSampleRunPanel *runPanel,
											 int & maxRunSize,
											 int & maxTestWidth,
											 int & maxResultWidth,
											 bool queued,
                                             const valc::IDToken& runID )
{
	std::string barcode;      // for this particular sample

	int count = 0;  // this will be a count of worklist entries found
	valc::Range<valc::WorklistEntryIterator> iterPair
			= sn->getWorklistEntries(sampleDescriptor);
	valc::WorklistEntryIterator iter = iterPair.first; //start
	while (iter!=iterPair.second) {

		const valc::WorklistEntry *entry = *iter;
		count++;
		std::string testName = sn->getTestName(entry->getTestID());

		// now to set the barcode
		if (count==1) {    // only bother for the first entry for this sample
			barcode = entry->getBarcode();
			runPanel->barcodePanel->updateBarcode(barcode);
		}

		if (queued) {
			TTestPanel *t = new TTestPanel(this,runPanel->testsPanel,testName,"",
										   "Level:test result",observer,QUEUED);
			assignAttributes(t,entry,QUEUED);
			runPanel->testPanels->push_back(t);

			maxTestWidth = std::max(t->findPanelLeftWidth(),maxTestWidth);
			// parent of t must be set later on
		}
		else { // will create a TTestPanel from each result of this worklist entry
			int countR = 0;  // to count the test results
			valc::Range<valc::TestResultIterator> iterPair =  entry->getTestResults();
			valc::TestResultIterator iterR = iterPair.first;
			while (iterR!=iterPair.second) { // going through the results
				const valc::TestResult *tr = *iterR;
				if (tr!=NULL) {
					TTestPanel *t = new TTestPanel(this,runPanel->testsPanel,testName,
												   Utils::float2str(tr->getResultValue()),
										           "Level:test result",observer,SENT_TO_ANALYSER);
					assignAttributes(runPanel,t,sn,entry,tr,runID);
					runPanel->testPanels->push_back(t);
					maxTestWidth = std::max(t->findPanelLeftWidth(),maxTestWidth);
					maxResultWidth = std::max(t->findResultDisplayWidth(),maxResultWidth);
					countR++;
				}
				iterR++;
			}
			if (countR==0) {   // looks like the result is pending
				// set up a TTestPanel anyway
				TTestPanel *t = new TTestPanel(this,runPanel->testsPanel,testName,"",
											   "Level:test result",observer,SENT_TO_ANALYSER);
				assignAttributes(t,entry,SENT_TO_ANALYSER);
				runPanel->testPanels->push_back(t);
				maxTestWidth = std::max(t->findPanelLeftWidth(),maxTestWidth);

			}
		}
		iter++;
	}

	// an alternative way of getting the barcode, if no worklist entries found
	if (!queued && count==0) {
		/*
		std::string sampleRunId =
		if (sampleRunId.length()>0) {
			valc::BuddyDatabaseEntries bdes
				= sn->listBuddyDatabaseEntriesFor(sampleRunId);
			if (bdes.size()>0) {
				barcode = bdes.at(0).barcode;
				runPanel->barcodePanel->updateBarcode(barcode);
			}
		}
		*/
	}
	if (count>maxRunSize) {
		maxRunSize = count;
    }

    return count;
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
		EntryPanelsReverseIterator riter = runPanel->testPanels->rbegin();
		while (riter!= runPanel->testPanels->rend()) {
			TTestPanel *t = *riter;
			posX -= separation;
			finaliseTestPanel(t,runPanel->testsPanel,posX,queued);
			riter++;
		}
		runPanel->testsPanel->Parent = runPanel;
		runPanel->barcodePanel->Parent = runPanel;  // now can add
													// the barcode panel
									                // so it'll be on top
		iter++;
    }

}

/** Sets up an empty sample run panel, with the given owner panel. */
TSampleRunPanel* WorklistEntriesView::createSampleRunPanel(TPanel *parentPanel,
														   int y,
														   bool queued)
{
	TSampleRunPanel *srPanel = new TSampleRunPanel(viewFrame->ResultsInnerPanel);
	srPanel->StyleLookup = "InvisiblePanelStyle";
	srPanel->Position->X = 0;
	srPanel->Position->Y = y;
	srPanel->Visible = true;
	srPanel->Opacity = 1;
	srPanel->TagString = "Level:sample run";

	// now for the sub panels for this sample run

	TBarcodePanel *barcodePanel = new TBarcodePanel(this,srPanel,queued);
	srPanel->barcodePanel = barcodePanel;


	// now to set up the invisible panel to hold the individual test panels
	TPanel *testsPanel = new TPanel(srPanel);
	testsPanel->StyleLookup = "TestResultsContainerStyle";
	testsPanel->Height = param("testResultHeight");
	testsPanel->Position->X = barcodePanel->Width
							  - 2*param("cornerWidth");
	testsPanel->Position->Y = 0;
	testsPanel->TagString = "Level:sample run test results";
	srPanel->testsPanel = testsPanel;

	return srPanel;
}

/** Goes through all queued sample runs and gets the worklist entries.
  *
  * @param snapshot      the pointer that refers to the business layer data
  * @param numberOfRuns  to hold the total number of queued sample runs
  * @param maxRunSize        to hold the max # of tests in a sample run
  * @param maxTestWidth  to hold the max width of a test result panel
  */
bool WorklistEntriesView::createQueuedEntries(valc::SnapshotPtr snapshot,
											  int & numberOfRuns,
											  int & maxRunSize,
											  int & maxTestWidth)
{
	numberOfRuns = 0;
	maxRunSize = 0;  // default minimum
	maxTestWidth = 60;  // default minimum bubble width
	int maxResultWidth = 0; // dummy

	int y = param("runsPanelverticalMargin");  // starting just below the top

	try {
		valc::QueuedSampleIterator qi = snapshot->queueBegin();   // -> overloaded
		while (qi!=snapshot->queueEnd()) {
			valc::QueuedSample qs = *qi;
			std::string descriptor = qs.getSampleDescriptor();

			// now construct the panel for the info for this sample run
			TSampleRunPanel *runPanel
				= createSampleRunPanel(viewFrame->WorkInnerPanel,y,QUEUED);



			addWorklistEntries(snapshot,descriptor,runPanel,
							   maxRunSize,maxTestWidth,maxResultWidth,QUEUED);

			queuedComponents->push_back(runPanel);
			runPanel->Parent = viewFrame->WorkInnerPanel;

			numberOfRuns++;
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
bool WorklistEntriesView::displayWorklistQueue(valc::SnapshotPtr snapshot) {

	viewFrame->WorkInnerPanel->BeginUpdate();
	viewFrame->WorkInnerPanel->TagString = innerLevel;

	int numberOfSampleRuns;   // will hold # of sample runs in total
	int maxSampleRunSize;     // will hold max # of tests in a sample run
	int maxQueuedTestWidth;   // will hold max bubble width
	int maxSampleRunWidth;

	bool success = createQueuedEntries(snapshot,
									  numberOfSampleRuns,
									  maxSampleRunSize,
									  maxQueuedTestWidth);
	if (success) {
		guiConfig["queuedTestWidth"] = maxQueuedTestWidth;
		int separation = maxQueuedTestWidth - 2*guiConfig["cornerWidth"];
		guiConfig["horizontalQueuedTestSeparation"] = separation;
		maxSampleRunWidth = (numberOfSampleRuns + 1) * maxQueuedTestWidth
							- separation;

		postProcessEntries(QUEUED);

		viewFrame->WorkInnerPanel->Height
					= numberOfSampleRuns*param("verticalRunSeparation")
					  - param("sampleRunGap") + 2*param("runsPanelverticalMargin");
		viewFrame->WorkInnerPanel->Width
					= maxSampleRunWidth + param("runsPanelrightMargin");
	}


	viewFrame->WorkInnerPanel->EndUpdate();
	viewFrame->WorkInnerPanel->Repaint();

	return success;
}


/** Goes through all sample runs sent to the analyser and gets the
  * relevant worklist entries and results.
  *
  * @param snapshot      the pointer that refers to the business layer data
  * @param numberOfRuns  to hold the total number of queued sample runs
  * @param maxRunSize    to hold the max # of tests in a sample run
  * @param maxTestWidth  to hold the max width of a test result panel
  */
bool WorklistEntriesView::createResultsEntries(valc::SnapshotPtr snapshot,
											   int & numberOfRuns,
											   int & maxRunSize,
											   int & maxTestLeftWidth,
											   int & maxResultWidth)
{
	numberOfRuns = 0;
	maxRunSize = 0;  // default minimum
	maxTestLeftWidth = 40;  // default minimum bubble width
	maxResultWidth = param("defaultResultWidth");

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
					= createSampleRunPanel(viewFrame->ResultsInnerPanel,y,SENT_TO_ANALYSER);
				std::string descriptor = r.getSampleDescriptor();
				int numWorklistEntries = addWorklistEntries(snapshot,descriptor,runPanel,
								   maxRunSize,maxTestLeftWidth,maxResultWidth,
								   SENT_TO_ANALYSER, r.getRunID() );

                if ( 0 == numWorklistEntries )
                {
                    logManager->log( paulst::format( "No worklist entries for sample-run %s", r.getRunID().token().c_str() ).c_str() );
                }

				resultsComponents->push_back(runPanel);
				runPanel->Parent = viewFrame->ResultsInnerPanel;

				numberOfRuns++;
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
bool WorklistEntriesView::displayWorklistResults(valc::SnapshotPtr snapshot)
{
	viewFrame->ResultsInnerPanel->BeginUpdate();
	viewFrame->ResultsInnerPanel->TagString = innerLevel;

	int numberOfSampleRuns;   // will hold # of sample runs in total
	int maxSampleRunSize;     // will hold max # of tests in a sample run
	int maxResultWidth;
	int maxResultLeftWidth;   // will hold max width of rest of the bubble

	bool success = createResultsEntries(snapshot,
										numberOfSampleRuns,
										maxSampleRunSize,
										maxResultLeftWidth,
										maxResultWidth);


	if (success) {
		guiConfig["testResultWidth"] = maxResultLeftWidth + maxResultWidth;;
		int separation = guiConfig["testResultWidth"] - 2*guiConfig["cornerWidth"];
		guiConfig["horizontalTestResultSeparation"] = separation;
		guiConfig["resultWidth"] = maxResultWidth;

		postProcessEntries(SENT_TO_ANALYSER);

		viewFrame->ResultsInnerPanel->Height
			= numberOfSampleRuns*param("verticalRunSeparation")
			  - param("sampleRunGap") + 2*param("runsPanelverticalMargin");

		int maxSampleRunWidth = (numberOfSampleRuns + 1) * guiConfig["testResultWidth"]
								- separation;
		viewFrame->ResultsInnerPanel->Width
			= maxSampleRunWidth + param("runsPanelrightMargin");
	}

	viewFrame->ResultsInnerPanel->EndUpdate();
	viewFrame->ResultsInnerPanel->Repaint();

	return success;

}

void WorklistEntriesView::positionView()
{   // sets the user's view to a suitable position

	// the default for the bottom panel (queued entries) is fine (at the top),
	// so we set the viewport position of the results to something suitable,
	// currently at the bottom of the scrollbox
	viewFrame->ResultsScrollBox->BeginUpdate();
	TPointF pos;
	pos.x = 0.0;
	pos.y = viewFrame->ResultsInnerPanel->Height; // this position right at the end
												 // of the panel being viewed makes
												 // it go down to the bottom

	// was viewFrame->ResultsInnerPanel->Height - viewFrame->ResultsPanel->Height;
	viewFrame->ResultsScrollBox->ViewportPosition = pos;
	viewFrame->ResultsScrollBox->EndUpdate();
	viewFrame->ResultsScrollBox->Repaint();

}


void WorklistEntriesView::setUpVisualComponents(valc::SnapshotPtr sn)
{
	bool ok = displayWorklistQueue(sn);
	ok = displayWorklistResults(sn) && ok;


	positionView(); // set the user's view to a suitable position
	recordPanelHeights();
}


/*---------------------- end Main GUI setting up -------------------*/


/*------------------- begin Split Pane manoeuvrings ----------------*/

/** Records the current heights of the panels affected by the main
  * split pane. This is so that later on, if the window is resized, the
  * proportions of the splitting can be maintained.
  */
void WorklistEntriesView::recordPanelHeights()
{
		recordResultsHeight = viewFrame->ResultsPanel->Height;
		recordWorkHeight = viewFrame->WorkPanel->Height;
}

/** This keeps the ratio of top:bottom pane heights, and is called when
  * the main window gets resized. (Without this, the resizing would
  * just affect the top pane, in a non-intuitive fashion.)
  *
  * @todo   when adjusting, make it look like the splitter is sticky to the
  *         top of the worklist and the bottom of the active/results list,
  *         like anchors
  */
void WorklistEntriesView::adjustPanelHeights()
{
	// Was having problems when the split pane was adjusted in too
	// extreme a ratio. Turns out that when you make the top pane
	// pretty large compared to the bottom pane, then resize to a smaller
	// size, the height of the bottom pane has already got set to zero
	// by the time that this method sees it.
	// So have to work things out based on other things, not
	// viewFrame->WorkPanel->Height


	float oldTotalHeight = recordResultsHeight + recordWorkHeight;

	float newResultsHeight =  viewFrame->ResultsPanel->Height;
	float newWorkHeight =  viewFrame->WorkPanel->Height;
	float totalHeight = newResultsHeight+newWorkHeight;


	if (totalHeight>0) {
		float improvedResultsHeight = recordResultsHeight * totalHeight
									  / oldTotalHeight;
		if (improvedResultsHeight<param("minPanelHeight")) {
			 improvedResultsHeight = (float)param("minPanelHeight");
		}

		viewFrame->ResultsPanel->Height = improvedResultsHeight;
		viewFrame->WorkPanel->Height = totalHeight - improvedResultsHeight;

		recordPanelHeights();
	}

}

/*------------------- end Split Pane manoeuvrings ----------------*/

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



