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
#include "StrUtil.h"
#include "SnapshotUtil.h"
#include "TActionPanel.h"
#include "TTestInstancePanel.h"
#include "TSnapshotFrame.h"

#include "VisualComponents.h"
#include "WorklistEntriesPositioning.h"

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
										 LogManager *lm,
										 const paulst::Config *topConfig,
										 const paulst::Config *guiConfig)
	:
	viewFrame(sf),
    logManager(lm),
    observer(NULL)
{
	initialiseGUIconfigValues(topConfig,guiConfig);
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
void WorklistEntriesView::initialiseGUIconfigValues(const paulst::Config* tConfig,
													const paulst::Config* gConfig)
{
	// a little information is required from the main config
	try {
		machineId = paulst::toInt(tConfig->get("localMachineID"));
	}
	catch( ... ) {
		logManager->logException("Could not extract machine id from 'localMachineID' property in top configuration file.");
	}
	// now to extract all the strings from the gui config, and convert to ints
	paulst::Config::const_iterator iter = gConfig->begin();
	while (iter!=gConfig->end()) {
		std::string key = iter->first;
		try {
		  guiConfig[key] = paulst::toInt(iter->second);
		}
		catch( ... ) { }
		iter++;
	}

	// now some calculations from existing values

	// vertical distance between sample runs in a list
	guiConfig["verticalRunSeparation"] = guiConfig["sampleRunGap"]
										 + Positioning::ENTRY_HEIGHT;

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


/** Constructs and displays the GUI components for the queued sample tests
  * in the bottom half of the screen. Assumes that any previous GUI components
  * have been cleared already.
  *
  * @param snapshot    the pointer that refers to the business layer data
  */
void WorklistEntriesView::displayWorklistQueue(valc::SnapshotPtr snapshot) {

	DisplayProperties queueProperties;
	if (preprocessQueueEntries(snapshot,queueProperties)) {

		// logManager->log("Q barcode size: " + Utils::int2str(queueProperties.firstSize));
		// logManager->log("Q # of rows: " + Utils::int2str(queueProperties.numberOfRows));
		// logManager->log("Q label size: " + Utils::int2str(queueProperties.leftSize));
		// logManager->log("Q max row size: " + Utils::int2str(queueProperties.rowSize));


		guiConfig["queuedTestWidth"]
			= TTestInstancePanel::getWidth(queueProperties.leftSize,0,false,QUEUED);
		guiConfig["queuedCompactTestWidth"]
			= TTestInstancePanel::getWidth(queueProperties.leftSize,0,true,QUEUED);

		guiConfig["queuedTestSeparation"]
			= guiConfig["queuedTestWidth"] - 2*TTestInstancePanel::CORNER_WIDTH;
		guiConfig["queuedCompactTestSeparation"]
			= guiConfig["queuedCompactTestWidth"] - 2*TTestInstancePanel::CORNER_WIDTH;


		guiConfig["queuedBarcodeSectionWidth"]
			= queueProperties.firstSize	+ param("barcodeLabelOffsetX");

		createQueuedEntries(snapshot,queueProperties);
	}

}


/** Scans the queued worklist entries to retrieve information about how
  * the display should be sized, for the lower half of the screen.
  *
  * @param snapshot    the pointer that refers to the business layer data
  * @param props       for holding found characteristics of the dataset,
  *                    such as number of sample runs, max label size
  */
bool WorklistEntriesView::preprocessQueueEntries(valc::SnapshotPtr snapshot,
												 DisplayProperties &props)
{
	// set min defaults for display properties
	props.numberOfRows = 0;
	props.rowSize = 0;       // for counting max # of entries in a row
	props.firstSize = 40;    // default minimum size of left-hand barcode area
	props.leftSize = 10;     // default minimum width of test name label

	try {
		valc::QueuedSampleIterator qi = snapshot->queueBegin();
		while (qi!=snapshot->queueEnd()) {
			valc::QueuedSample qs = *qi;

			updateBarcodeWidth(qs.getBarcode(),props);

			// check entry widths for queued entries for this sample
			std::string sampleDescriptor = qs.getSampleDescriptor();
			valc::Range<valc::WorklistEntryIterator> iterPair
					= snapshot->getWorklistEntries(sampleDescriptor);
			valc::WorklistEntryIterator iter = iterPair.first; //start
			int count = 0;  // for counting the number of relevant worklist entries
			while (iter!=iterPair.second) {  // iterating through all worklist entries
											 // for this sample (inc. maybe non-queued ones)

				const valc::WorklistEntry *entry = *iter;
				valc::Range<valc::TestResultIterator> entryResult = entry->getTestResults();
				if (valcui::empty(entryResult)) { // then 'tis in the queue
					count++;
					std::string testName = snapshot->getTestName(entry->getTestID());

					props.leftSize = std::max(TTestInstancePanel::displayWidth(testName),
											  props.leftSize);
				}

				iter++;
			}
			props.rowSize = std::max(count,props.rowSize);
			props.numberOfRows++;
			qi++;
		}
	}
	catch (const Exception & e) {
		std::string msg = "Caught an exception (was attempting to scan the queued worklist entries). More details:";
		logManager->logException(msg + AnsiString(e.Message.c_str()).c_str());
		return false;
	}
	catch (...) {
		std::string msg = "Caught an unspecified kind of exception (was attempting to scan the queued worklist entries).";
		logManager->logException(msg);
		return false;
	}

	return true;
}

/** Goes through all queued sample runs creates visual components to represent
  * their worklist entries, adding them to the lower half of the the view.
  * Also sets the sizes of the panel being viewed in the split pane.
  *
  * @param snapshot      the pointer that refers to the business layer data
  * @param props         for holding found characteristics of the dataset,
  *                      such as number of sample runs, max size of a worklist entry
  */
void WorklistEntriesView::createQueuedEntries(valc::SnapshotPtr snapshot,
											  const DisplayProperties &props)
{
	viewFrame->WorkInnerPanel->BeginUpdate();
	viewFrame->WorkInnerPanel->TagString = innerLevel;

	addQueuedRuns(snapshot,props);

	int maxSampleRunWidth = (props.rowSize + 1)*guiConfig["queuedTestWidth"]
							 - param("queuedTestSeparation");
	// this is a slight overestimate, allowing for a small edge gap

	viewFrame->WorkInnerPanel->Width = maxSampleRunWidth
									   + param("runsPanelrightMargin");
	viewFrame->WorkInnerPanel->Height
		= props.numberOfRows*param("verticalRunSeparation") - param("sampleRunGap")
		  + 2*param("runsPanelverticalMargin");
	viewFrame->WorkInnerPanel->EndUpdate();
	viewFrame->WorkInnerPanel->Repaint();
}

/** Goes through all queued sample runs creates visual components to represent
  * their worklist entries, adding them to the lower half of the the view.
  * Also sets the sizes of the panel being viewed in the split pane.
  *
  * @param snapshot      the pointer that refers to the business layer data
  * @param props         for holding found characteristics of the dataset,
  *                      such as number of sample runs, max size of a worklist entry
  */
void WorklistEntriesView::addQueuedRuns(valc::SnapshotPtr snapshot,
										const DisplayProperties &props)
{
	try {
		int yPos = param("runsPanelverticalMargin");  // starting just below the top
		valc::QueuedSampleIterator qi = snapshot->queueBegin();
		while (qi!=snapshot->queueEnd()) {
			valc::QueuedSample qs = *qi;

			TSampleRunPanel *runPanel
				= new TSampleRunPanel(this,viewFrame->WorkInnerPanel,
									  qs.getBarcode(),
									  emptyRunWidth(param("queuedBarcodeSectionWidth")),
									  yPos,QUEUED);
			addQueuedEntries(snapshot,props,qs.getSampleDescriptor(),runPanel);

			runPanel->testPanels->sort(TTestInstancePanel::goCompare);
			lineUpEntries(runPanel,
						  param("queuedBarcodeSectionWidth"),
						  param("queuedCompactTestWidth"),
						  param("queuedTestSeparation"),
						  param("queuedCompactTestSeparation"));


			// queued runPanel don't need attention, nor are they closed,
			// so nothing further to do here
			queuedComponents->push_back(runPanel);

			yPos += param("verticalRunSeparation");  // yPos now the position of next queued run
			qi++;
		}
	}
	catch (const Exception & e) {
		std::string msg = "Caught an exception (was attempting to load the queued worklist entries). More details:";
		logManager->logException(msg + AnsiString(e.Message.c_str()).c_str());
	}
	catch (...) {
		std::string msg = "Caught an unspecified kind of exception (was attempting to load the queued worklist entries).";
		logManager->logException(msg);

	}
}

// Retrieves queued entries for a given sample descriptor.
void WorklistEntriesView::addQueuedEntries(valc::SnapshotPtr snapshot,
										   const DisplayProperties &props,
										   const std::string &sampleDescriptor,
										   TSampleRunPanel *runPanel)
{
	valc::Range<valc::WorklistEntryIterator> iterPair
		= snapshot->getWorklistEntries(sampleDescriptor);
	valc::WorklistEntryIterator iter = iterPair.first; //start
	while (iter!=iterPair.second) {  // iterating through all worklist entries
									 // for this sample (inc. maybe non-queued ones)

		const valc::WorklistEntry *entry = *iter;
		valc::Range<valc::TestResultIterator> entryResult = entry->getTestResults();
		if (valcui::empty(entryResult)) { // then 'tis in the queue
			std::string testName = snapshot->getTestName(entry->getTestID());
			TTestInstancePanel *t
				= new TTestInstancePanel(this,runPanel,testName,
										 props.leftSize,observer);
			assignAttributes(t,entry);
			t->setStyle();
			runPanel->testPanels->push_back(t);
		}
		iter++;
	}
}

// Stacks up the entries for a sample run/queued run, with appropriate z-depth.
// Also displays the front entry more compactly.
void WorklistEntriesView::lineUpEntries(TSampleRunPanel *runPanel, int initX,
										int cWidth, int jump, int cJump)
{
	int xPos = initX;
	int count = 0;
	EntryPanelsIterator ti = runPanel->testPanels->begin();
	while (ti!=runPanel->testPanels->end()) {
		TTestInstancePanel *t = *ti;
		count++;
		t->Position->X = xPos;
		t->Position->Y = 0;
		t->SendToBack();
		if (count==1) {
			t->makeCompact(true,cWidth);
			xPos += cJump;
		}
		else {
			xPos += jump;
		}
		ti++;
	}
}


/** Constructs and displays the GUI components for the test results
  * in the upper half of the screen. Assumes that any previous GUI components
  * have been cleared already.
  *
  * @param snapshot    the pointer that refers to the business layer data
  */
void WorklistEntriesView::displayWorklistResults(valc::SnapshotPtr snapshot)
{

	 DisplayProperties resultsProperties;
	 if (preprocessResultsEntries(snapshot,resultsProperties)) {
		// logManager->log("R barcode size: " + Utils::int2str(resultsProperties.firstSize));
		// logManager->log("R # of rows: " + Utils::int2str(resultsProperties.numberOfRows));
		// logManager->log("R name size: " + Utils::int2str(resultsProperties.leftSize));
		// logManager->log("R result size: " + Utils::int2str(resultsProperties.rightSize));
		// logManager->log("R max row size: " + Utils::int2str(resultsProperties.rowSize));

		guiConfig["resultTestWidth"]
			= TTestInstancePanel::getWidth(resultsProperties.leftSize,
										   resultsProperties.rightSize,
										   false,SENT_TO_ANALYSER);
		guiConfig["resultCompactTestWidth"]
			= TTestInstancePanel::getWidth(resultsProperties.leftSize,
										   resultsProperties.rightSize,
										   true,SENT_TO_ANALYSER);
		guiConfig["resultTestSeparation"]
			= guiConfig["resultTestWidth"] - 2*TTestInstancePanel::CORNER_WIDTH;
		guiConfig["resultCompactTestSeparation"]
			= guiConfig["resultCompactTestWidth"] - 2*TTestInstancePanel::CORNER_WIDTH;

		guiConfig["resultsBarcodeSectionWidth"]
			= resultsProperties.firstSize	+ param("barcodeLabelOffsetX");

		createResultsEntries(snapshot,resultsProperties);
	 }

}

/** Scans the active worklist entries to retrieve information about how
  * the display should be sized, for the upper half of the screen.
  *
  * @param snapshot    the pointer that refers to the business layer data
  * @param props       for holding found characteristics of the dataset,
  *                    such as number of sample runs, max label size
  */
bool WorklistEntriesView::preprocessResultsEntries(valc::SnapshotPtr snapshot,
												   DisplayProperties &props)
{
	// set min defaults for display properties
	props.numberOfRows = 0;
	props.rowSize = 0;       // for counting max # of entries in a row
	props.firstSize = 40;    // default minimum size of left-hand barcode area
	props.leftSize = 10;     // default minimum width of test name label
	props.rightSize = TTestInstancePanel::minResultWidth();
	try {

		valc::LocalEntryIterator ri = snapshot->localBegin();
		while (ri!=snapshot->localEnd()) {

			valc::LocalEntry ms = *ri;  // (this could be a sample or a delimiter)
			// note that the type LocalEntry is a typedef for
			// boost::variant<LocalRun,BatchDelimiter>
			if (ms.type()==typeid(valc::LocalRun)) { // is local run (not batch delimiter)
				valc::LocalRun r = boost::get<valc::LocalRun>(ms);

				updateBarcodeWidth(r.getBarcode(),props);
				int count = 0;  // # relevant worklist entries found for this sample run
				std::string sampleDescriptor = r.getSampleDescriptor();
				valc::Range<valc::WorklistEntryIterator> iterPair
					= snapshot->getWorklistEntries(sampleDescriptor);
				valc::WorklistEntryIterator iter = iterPair.first; // start
				while (iter!=iterPair.second) {  // iterating through the worklist entries
					const valc::WorklistEntry *entry = *iter;
					std::string testName = snapshot->getTestName(entry->getTestID());
					valc::Range<valc::TestResultIterator> entryResult = entry->getTestResults();
					if (valcui::empty(entryResult)) {
						if (r.isOpen()) {  // this result must be pending
							count++;
							props.leftSize = std::max(TTestInstancePanel::displayWidth(testName),
													  props.leftSize);
						}
						// else no test results, therefore queued entry, therefore ignore
					}
					else { // test result(s) exist for this worklist entry
						props.leftSize = std::max(TTestInstancePanel::displayWidth(testName),
												  props.leftSize);
						valc::Range<valc::TestResultIterator> iterPair =  entry->getTestResults();
						valc::TestResultIterator iterR = iterPair.first;
						while (iterR!=iterPair.second) { // going through the results
							const valc::TestResult *tr = *iterR;
							if (tr!=NULL && r.getRunID()==tr->getSampleRunID()) {
								count++;
								props.rightSize = std::max(TTestInstancePanel::findResultDisplayWidth(Utils::float2str(tr->getResultValue())),
														   props.rightSize);
							}
							iterR++;
						}
					}
					iter++;
				}
				props.numberOfRows++;
				props.rowSize = std::max(count,props.rowSize);
				if (count==0) {
					logManager->log(paulst::format("No worklist entries for sample-run %s",
									r.getRunID().token().c_str() ).c_str());
				}
			}
			ri++;
		}
	}
	catch (const Exception & e) {
		std::string msg = "Caught an exception (was attempting to load the active (pending/completed) worklist entries). More details:";
		logManager->logException(msg + AnsiString(e.Message.c_str()).c_str());
		return false;
	}
	catch (...) {
		std::string msg = "Caught an unspecified kind of exception (was attempting to load the active (pending/completed) worklist entries).";
		logManager->logException(msg);
		return false;
	}

	return true;
}


/** Goes through all active sample runs, creating visual components to represent
  * their worklist entries, adding them to the upper half of the the view.
  * Also sets the sizes of the panel being viewed in the split pane.
  *
  * @param snapshot      the pointer that refers to the business layer data
  * @param props         for holding found characteristics of the dataset,
  *                      such as number of sample runs, max size of a worklist entry
  */
void WorklistEntriesView::createResultsEntries(valc::SnapshotPtr snapshot,
											  const DisplayProperties &props)
{
	viewFrame->ResultsInnerPanel->BeginUpdate();
	viewFrame->ResultsInnerPanel->TagString = innerLevel;

	addResultsRuns(snapshot,props);

	int maxSampleRunWidth = (props.rowSize + 1)*param("resultTestWidth")
							 - param("resultTestSeparation");
	// this is a slight overestimate, allowing for a small edge gap

	viewFrame->ResultsInnerPanel->Width
		= maxSampleRunWidth + param("runsPanelrightMargin");
	viewFrame->ResultsInnerPanel->Height
			= props.numberOfRows*param("verticalRunSeparation")
			  - param("sampleRunGap") + 2*param("runsPanelverticalMargin");

	viewFrame->ResultsInnerPanel->EndUpdate();
	viewFrame->ResultsInnerPanel->Repaint();
}


/** Goes through all queued sample runs creates visual components to represent
  * their worklist entries, adding them to the lower half of the the view.
  * Also sets the sizes of the panel being viewed in the split pane.
  *
  * @param snapshot      the pointer that refers to the business layer data
  * @param props         for holding found characteristics of the dataset,
  *                      such as number of sample runs, max size of a worklist entry
  */
void WorklistEntriesView::addResultsRuns(valc::SnapshotPtr snapshot,
										 const DisplayProperties &props)
{
	try {

		int yPos = param("runsPanelverticalMargin");  // starting just below the top
		valc::LocalEntryIterator ri = snapshot->localBegin();
		while (ri!=snapshot->localEnd()) {
			valc::LocalEntry ms = *ri;  // (this could be a sample or a delimiter)
			// note that the type LocalEntry is a typedef for
			// boost::variant<LocalRun,BatchDelimiter>
			if (ms.type()==typeid(valc::LocalRun)) { // is local run (not batch delimiter)
				valc::LocalRun r = boost::get<valc::LocalRun>(ms);

				TSampleRunPanel *runPanel
					= new TSampleRunPanel(this,viewFrame->ResultsInnerPanel,
										  r.getBarcode(),
										  emptyRunWidth(param("resultsBarcodeSectionWidth")),
										  yPos,SENT_TO_ANALYSER);
				addResultsEntries(snapshot,props,r,runPanel);

				runPanel->testPanels->sort(TTestInstancePanel::goCompare);
				lineUpEntries(runPanel,param("resultsBarcodeSectionWidth"),
							  param("resultCompactTestWidth"),
							  param("resultTestSeparation"),
							  param("resultCompactTestSeparation"));

				if (!r.isOpen()) { // closed sample run
					runPanel->makeClosed(emptyRunWidth(param("resultsBarcodeSectionWidth")),
								         param("resultTestSeparation"));
				}

				resultsComponents->push_back(runPanel);
				yPos += param("verticalRunSeparation");

			}
			ri++;
		}

	}
	catch (const Exception & e) {
		std::string msg = "Caught an exception (was attempting to load the active (pending/completed) worklist entries). More details:";
		logManager->logException(msg + AnsiString(e.Message.c_str()).c_str());
	}
	catch (...) {
		std::string msg = "Caught an unspecified kind of exception (was attempting to load the active (pending/completed) worklist entries).";
		logManager->logException(msg);

	}
}

// Retrieves the entries for an active sample run and makes panels for them
void WorklistEntriesView::addResultsEntries(valc::SnapshotPtr snapshot,
											const DisplayProperties &props,
											const valc::LocalRun &r,
											TSampleRunPanel *runPanel)
{

	valc::Range<valc::WorklistEntryIterator> iterPair
					= snapshot->getWorklistEntries(r.getSampleDescriptor());
	valc::WorklistEntryIterator iter = iterPair.first; // start
	while (iter!=iterPair.second) {  // iterating through the worklist entries
		const valc::WorklistEntry *entry = *iter;
		std::string testName = snapshot->getTestName(entry->getTestID());
		valc::Range<valc::TestResultIterator> entryResult = entry->getTestResults();
		if (valcui::empty(entryResult)) {
			if (r.isOpen()) {  // this result must be pending
				TTestInstancePanel *t
					= new TTestInstancePanel(this,runPanel,
											 testName,props.leftSize,
											 "",props.rightSize,observer);
				assignAttributes(t,entry);
				t->setStyle();
				runPanel->testPanels->push_back(t);
			} // else no test results, therefore queued entry, therefore ignore
		}
		else {  // test result(s) exist for this worklist entry
			valc::Range<valc::TestResultIterator> iterPair =  entry->getTestResults();
			valc::TestResultIterator iterR = iterPair.first;
			while (iterR!=iterPair.second) { // going through the results
				const valc::TestResult *tr = *iterR;
				if (tr!=NULL && r.getRunID()==tr->getSampleRunID()) {

					std::string resStr = Utils::float2str(tr->getResultValue());
					TTestInstancePanel *t
						= new TTestInstancePanel(this,runPanel,
												 testName,props.leftSize,
												 resStr,props.rightSize,
												 observer);
					assignAttributes(runPanel,t,snapshot,entry,tr,r.getRunID());
					t->setStyle();
					runPanel->testPanels->push_back(t);
                }
				iterR++;
			}
		}
		iter++;
	}
}


/** Updates the information so far about the barcode width for this section. */
void WorklistEntriesView::updateBarcodeWidth(const std::string &barcode,
											 DisplayProperties &props)
{
	int width = Utils::findTextWidth(barcode)
				+ 3*TTestInstancePanel::CORNER_WIDTH;
	props.firstSize = std::max(width,props.firstSize);
}

int WorklistEntriesView::emptyRunWidth(int barcodeWidth)
{
	return barcodeWidth + 2*TTestInstancePanel::CORNER_WIDTH;
}



/** Sets certain attributes of the given test panel (which represents an
  * instance of a test being carried out on a sample), corresponding to
  * attributes of the worklist entry that it depicts. The attributes set
  * here are common to both queued entries and analysed/ing entries.
  */
void WorklistEntriesView::assignAttributes(TTestInstancePanel *t,
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
										   TTestInstancePanel *t,
										   valc::SnapshotPtr sn,
										   const valc::WorklistEntry *entry,
										   const valc::TestResult *tr,
										   const valc::IDToken & runId)
{
	assignAttributes(t,entry); // set attributes in common
							   // with queued worklist entries

	std::string res = Utils::float2str(tr->getResultValue());
	t->setAttribute("Result",res);
	t->setAttribute("Result Id",tr->getID());

	TDateTime dt = tr->getDateAnalysed();
	std::string d = Utils::unicodestr2str(dt.FormatString("YYYY-MM-DD (ddd)"));
	std::string ti = Utils::unicodestr2str(dt.FormatString("HH:MM"));
	t->setAttribute("Date Analysed",d);
	t->setAttribute("Time Analysed",ti);

	t->setQC(valcui::isQC(entry));

	findAttentionNeed(sn,runPanel,t,tr);

	// was this test carried out on the local analyser?
	t->setLocal(tr->getMachineID() == machineId);

}


// works out whether a given result is in need of attention
void WorklistEntriesView::findAttentionNeed(valc::SnapshotPtr snapshot,
                                            TSampleRunPanel *runPanel,
											TTestInstancePanel *t,
											const valc::TestResult *tr)
{
	// the need for attention is worked out from what the rule results engine says
	int resultId = tr->getID();
	if (t->isQC()) {  // QCs have different results to those for unknown samples
		try {
			if (snapshot->hasRuleResults(resultId)) {
				valc::RuleResults rr = snapshot->getRuleResults(resultId);

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
					runPanel->needsAttention();
				}
			}
		}
		catch (const Exception & e) {
			logManager->logException(std::string("Exception in WorklistEntriesView::findAttentionNeed while attempting to get QC Rule Results. More details:")
									+ AnsiString(e.Message.c_str()).c_str());
		}
		catch (...) {
			logManager->logException("Caught an unspecified kind of exception in WorklistEntriesView::findAttentionNeed while attempting to get QC Rule Results.");
		}
	}
	else { // it's a test result from an unknown sample
		try {

			int r = tr->getControlStatus().summaryCode();
			t->setAttribute("Result Code",r);

			if (r==valc::CONTROL_STATUS_ERROR || r==valc::CONTROL_STATUS_FAIL
				|| r==valc::CONTROL_STATUS_BORDERLINE
				|| r==valc::CONTROL_STATUS_UNCONTROLLED) {
				t->needsAttention();
				runPanel->needsAttention();
			}
		}
		catch (const Exception & e) {
			logManager->logException(std::string("Exception in WorklistEntriesView::findAttentionNeed while attempting to get rule results for an unknown sample. More details:")
									+ AnsiString(e.Message.c_str()).c_str());
		}
		catch (...) {
			logManager->logException("Caught an unspecified kind of exception in WorklistEntriesView::findAttentionNeed while attempting to get rule results for an unknown sample.");
		}
	}
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
Coordinates WorklistEntriesView::findTestPanelPosition(const TTestInstancePanel *t)
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
void WorklistEntriesView::popupInfoPanel(TTestInstancePanel *t, int panel_type)
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
	TTestInstancePanel *t = p->originator;
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



