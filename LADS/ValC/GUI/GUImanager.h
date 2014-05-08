//---------------------------------------------------------------------------

#ifndef GUImanagerH
#define GUImanagerH
//---------------------------------------------------------------------------

#include <fmx.h>
#include <map>

#include "Config.h"
#include "StrUtil.h"
#include "Utils.h"


							// some forward declarations
							class TSnapshotFrame;
							class DataManager;
							class LogManager;
							class Positioning;
							struct Coordinates;
							class DSampleRun;
							class DSampleTest;
                            class SnapshotFrameObserver;
							class TInfoPanel;
							class TTestPanel;


/** @file docs-GUImanager.h
  * This file contains an overview of the GUI components used in ValC.
  * The main program has a singleton GUImanager object, whose job it is
  * to manage all the GUI components, everything except the logging window.
  */

class GUImanager {

public:

	/** a link back to the main form */
	TSnapshotFrame *mainForm;

	/** For carrying out tasks such as positioning and sizing of components. */
	Positioning *positioner;

	/** Records the height of the panel containing the test results.
		Useful for when the window is resized and the same panel proportions are to be kept. */
	float recordResultsHeight;

	/** Records the height of the panel containing the queued worklist entries.
		Useful for when the window is resized and the same panel proportions are to be kept. */
	float recordWorkHeight;

// For comments on methods, please see their implementations.

	GUImanager(TSnapshotFrame *m, DataManager* dm, LogManager* lm, const std::string & configFilename);
	~GUImanager();

	void initialiseGUIconfigValues(const std::string & configFilename);
	void clearAll();

	int param(const std::string & name);

	void setUpGUIcomponents();

	void recordPanelHeights();
	void adjustPanelHeights();

	int textWidth(const std::string & label);
	int findTextWidth(std::string s);
	void setTextWidths(const DSampleTest & e, bool queued);

	TObject* findInnerPanelAncestor(TObject *comp); // the ancestor just inside the scrollpane
	Coordinates findTestPanelPosition(const TTestPanel *t); // relative to inner panel ancestor

	void popupInfoPanel(TTestPanel *t, int panel_type);
	void removeInfoPanel(TInfoPanel *p);

    void setObserver( SnapshotFrameObserver *o );
private:

	DataManager *dataManager;
	LogManager *logManager;
    SnapshotFrameObserver *observer;

	/** a tag used to identify panels just inside the scroll views. */
	const UnicodeString innerLevel = "Level: inner panel";

	void doGUIcalculations();
	void makeWorklistQueueDisplay();
	void makeSamplesResultsDisplay();
	void setUpWorklistEntryPanels(DSampleRun *d, TPanel *panel, bool q, int *mx);
	void positionView();
	std::map <std::string,int> guiConfig; // contains config values
	std::map <std::string,int> textWidths; // widths of various text labels

	UnicodeString getComponentTagString(const TObject *comp);
	Coordinates getComponentPosition(const TObject *comp);
	TObject* getComponentParent(const TObject *comp);



};

#endif
