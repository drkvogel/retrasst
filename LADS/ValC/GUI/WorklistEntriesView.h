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
#include "WorklistEntriesPositioning.h"

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
							class SnapshotFrameObserver;
							class TInfoPanel;
							class TTestInstancePanel;
							class TSampleRunPanel;

typedef std::list<TPanel *> ComponentsList;

typedef ComponentsList::iterator ComponentsIterator;

class WorklistEntriesView {

    friend class TTestInstancePanel;

public:

	/** A link to the frame that contains this view */
	TSnapshotFrame *viewFrame;

	/** For carrying out tasks such as positioning and sizing of components. */
	Positioning *positioner;



	static const bool QUEUED;
	static const bool SENT_TO_ANALYSER;

// For comments on methods, please see their implementations.

	WorklistEntriesView(TSnapshotFrame *m, LogManager* lm,
						const paulst::Config *topConfig,
						const paulst::Config *guiConfig);
	~WorklistEntriesView();

	void initialiseGUIconfigValues(const paulst::Config* tConfig,
                                   const paulst::Config* gConfig);
	void clearAll();

	int param(const std::string & name);

	void setUpGUIcomponents();
	void setUpVisualComponents(valc::SnapshotPtr sn);


	TControl* findInnerPanelAncestor(TControl *comp); // the ancestor just inside the scrollpane
	Coordinates findTestPanelPosition(const TTestInstancePanel *t); // relative to inner panel ancestor

	void popupInfoPanel(TTestInstancePanel *t, int panel_type);
	void removeInfoPanel(TInfoPanel *p);

	void setObserver(SnapshotFrameObserver *ob);

private:

	std::map <std::string,int> guiConfig; // contains config values

	ComponentsList *queuedComponents;
	ComponentsList *resultsComponents;

	LogManager *logManager;
    SnapshotFrameObserver *observer;

	int machineId; // will be initialised from the top config file

	/** a tag used to identify panels just inside the scroll views. */
	const UnicodeString innerLevel = "Level: inner panel";

	void displayWorklistQueue(valc::SnapshotPtr snapshot);
	bool preprocessQueueEntries(valc::SnapshotPtr snapshot, DisplayProperties &props);
	void createQueuedEntries(valc::SnapshotPtr snapshot,
							 const DisplayProperties &props);
	void addQueuedRuns(valc::SnapshotPtr snapshot,
					   const DisplayProperties &props);
	void addQueuedEntries(valc::SnapshotPtr snapshot, const DisplayProperties &props,
						  const std::string &sampleDescriptor, TSampleRunPanel *runPanel);
	void lineUpEntries(TSampleRunPanel *runPanel, int initX,
					   int cWidth, int jump, int cJump);

	void displayWorklistResults(valc::SnapshotPtr snapshot);
	bool preprocessResultsEntries(valc::SnapshotPtr snapshot, DisplayProperties &props);
	void createResultsEntries(valc::SnapshotPtr snapshot,
							  const DisplayProperties &props);
	void addResultsRuns(valc::SnapshotPtr snapshot,
					   const DisplayProperties &props);
	void addResultsEntries(valc::SnapshotPtr snapshot, const DisplayProperties &props,
						   const valc::LocalRun &r, TSampleRunPanel *runPanel);
	void findAttentionNeed(valc::SnapshotPtr snapshot, TSampleRunPanel *runPanel,
						   TTestInstancePanel *t, const valc::TestResult *tr);

	void updateBarcodeWidth(const std::string &barcode,DisplayProperties &props);
    int emptyRunWidth(int barcodeWidth);

	void assignAttributes(TTestInstancePanel *t, const valc::WorklistEntry *entry);
	void assignAttributes(TSampleRunPanel *runPanel, TTestInstancePanel *t,
						  valc::SnapshotPtr sn, const valc::WorklistEntry *entry,
						  const valc::TestResult *tr, const valc::IDToken &sampleRunId);


	/*

	void finaliseTestPanel(TTestInstancePanel *t, TSampleRunPanel *parent,
						   int x, bool queued);

	void postProcessEntries(bool queued);

	void addQueuedWorklistEntries(valc::SnapshotPtr sn,
								 const std::string & sampleDescriptor,
								 const std::string & barcode,
								 TSampleRunPanel *runPanel,
								 DisplayProperties &props);
   */

	/*
	void addNonQueuedWorklistEntries(valc::SnapshotPtr sn,
									 const valc::LocalRun &r,
									 TSampleRunPanel *runPanel,
									 DisplayProperties &props);
	bool createResultsEntries(valc::SnapshotPtr snapshot,
							  DisplayProperties &props);       */



};

#endif
