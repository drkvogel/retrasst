//---------------------------------------------------------------------------

#ifndef WorklistEntriesViewH
#define WorklistEntriesViewH
//---------------------------------------------------------------------------

#include <fmx.h>
#include <map>
#include <list>

#include "API.h"

#include "Config.h"
#include "StrUtil.h"
#include "Utils.h"

/** @file docs-WorklistEntriesView.h
  * This file contains an overview of the GUI components used in ValC.
  * The main program has a singleton WorklistEntriesView which sets up
  * components related to worklist entries.
  */

							// some forward declarations
							class TSnapshotFrame;
							class LogManager;
							class Positioning;
							struct Coordinates;
							class DSampleRun;
							class DSampleTest;
							class SnapshotFrameObserver;
							class TInfoPanel;
							class TTestPanel;
							class TSampleRunPanel;

typedef std::list<TPanel *> ComponentsList;

typedef ComponentsList::iterator ComponentsIterator;

class WorklistEntriesView {

public:

	/** A link to the frame that contains this view */
	TSnapshotFrame *viewFrame;

	/** For carrying out tasks such as positioning and sizing of components. */
	Positioning *positioner;

	/** Records the height of the panel containing the test results.
		Useful for when the window is resized and the same panel proportions are to be kept. */
	float recordResultsHeight;

	/** Records the height of the panel containing the queued worklist entries.
		Useful for when the window is resized and the same panel proportions are to be kept. */
	float recordWorkHeight;

	static bool QUEUED;
	static bool SENT_TO_ANALYSER;

// For comments on methods, please see their implementations.

	WorklistEntriesView(TSnapshotFrame *m, LogManager* lm,
	                    const std::string & configFilename);
	~WorklistEntriesView();

	void initialiseGUIconfigValues(const std::string & configFilename);
	void clearAll();

	int param(const std::string & name);

	void setUpGUIcomponents();
	void setUpVisualComponents(valc::SnapshotPtr sn);


	void recordPanelHeights();
	void adjustPanelHeights();

	TControl* findInnerPanelAncestor(TControl *comp); // the ancestor just inside the scrollpane
	Coordinates findTestPanelPosition(const TTestPanel *t); // relative to inner panel ancestor

	void popupInfoPanel(TTestPanel *t, int panel_type);
	void removeInfoPanel(TInfoPanel *p);

	void setObserver(SnapshotFrameObserver *ob);

private:

	ComponentsList *queuedComponents;
	ComponentsList *resultsComponents;

	LogManager *logManager;
    SnapshotFrameObserver *observer;

	/** a tag used to identify panels just inside the scroll views. */
	const UnicodeString innerLevel = "Level: inner panel";

	void assignAttributes(TTestPanel *t, const valc::WorklistEntry *entry,
	                      bool queued);
	void assignAttributes(TSampleRunPanel *runPanel, TTestPanel *t,
						  valc::SnapshotPtr sn, const valc::WorklistEntry *entry,
						  const valc::TestResult *tr, const valc::IDToken &sampleRunId);

	TSampleRunPanel* createSampleRunPanel(TPanel *parentPanel, int y, bool queued);
    void finaliseTestPanel(TTestPanel *t, TPanel *parent, int x, bool queued);
	int addWorklistEntries(valc::SnapshotPtr sn,
							const std::string & sampleDescriptor,
							TSampleRunPanel *runPanel,
							int & maxRunWidth,
							int & maxTestWidth,
							int & maxResultWidth,
							bool queued,
                            const valc::IDToken& sampleRunID = valc::IDToken() );
	void postProcessEntries(bool queued);

	bool createQueuedEntries(valc::SnapshotPtr snapshot, int & numberOfRuns,
							 int & maxRunSize, int & maxTestWidth);
	bool displayWorklistQueue(valc::SnapshotPtr snapshot);

	bool createResultsEntries(valc::SnapshotPtr snapshot, int & numberOfRuns,
							  int & maxRunSize, int & maxTestWidth,
							  int & maxResultWidth);

	bool displayWorklistResults(valc::SnapshotPtr snapshot);

	void setUpWorklistEntryPanels(DSampleRun *d, TPanel *panel, bool q, int *mx);
	void positionView();

	std::map <std::string,int> guiConfig; // contains config values

};

#endif
