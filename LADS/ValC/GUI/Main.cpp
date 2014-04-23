//---------------------------------------------------------------------------

#include <fmx.h>
#include <System.DateUtils.hpp>     // for date/time stuff
#include "Utils.h"

#pragma hdrstop

// underlying API
#include "API.h"
#include "ConsoleWriter.h"
#include "DirUtil.h"
#include "StrUtil.h"

// GUI-related
#include "VisualComponents.h"  // do we need this any more?
#include "Main.h"
#include "LogForm.h"
#include <ostream>


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.fmx"


/** @file docs-overview.h
  * This file contains some documentation giving an overview of
  * the ValC GUI; this gets used on the home page of the doxygen
  * code documentation.
  */

TMainForm *MainForm;


/** Constructs the main form, and loads the top-level configuration file.  */
__fastcall TMainForm::TMainForm(TComponent* Owner)
   : 
    TForm( Owner ),
    appDataDir( paulst::appDataDir() + "\\ValC" ),
    config( paulst::loadContentsOf( appDataDir.path() + "\\config-top.txt" ) )
{
}

/** Initialises the logging window and the singleton managers for various
  * aspects of ValC: data, the GUI, and logging. Afterwards, the program is in
  * a state of "GUI exists but shows no data, managers & business layer
  * initialised, but no data loaded".
  */
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
	// set up the logging window
	bool wantLogWindow = (Utils::toLowerCase(config.get("logWindow")).compare("yes")==0);
	logManager = new LogManager(this,wantLogWindow);

	dataManager = new DataManager(this);

	guiManager = new GUImanager(this,config.get("GUIconfig"));

	// To do: NULL argument needs to get changed to a pointer to an implementation of
	// valc::UserAdvisor   , once that has been implemented
	valc::InitialiseApplicationContext(-1019429,
									   1234,
									   paulst::loadContentsOf(config.get("BusinessLayerConfig")),
									   logManager->logService,
									   NULL);

}
//---------------------------------------------------------------------------


/** Deletes the application context of the business layer, as well as deleting
  * the GUI manager, data manager, and log manager, which will also delete the
  * memory they manage.
  */
__fastcall TMainForm::~TMainForm()
{
	valc::DeleteApplicationContext();
	delete dataManager;
	delete guiManager;
	delete logManager;
	bool b = (this->ResultsInnerPanel)!=NULL;
}

/** Carefully clears all previous data structures used to store data from
  * the database snapshot, along with their corresponding GUI components.
  */
void TMainForm::clearAll()
{
	if (dataManager->haveSnapshot) {
		valc::Unload(snapshot);
		dataManager->haveSnapshot = false;
	}
	dataManager->clearAll();
	guiManager->clearAll();
}


/** Does a "Force Reload" operation, i.e. tries to load a fresh snapshot of data
  * from the database.
  */
void TMainForm::forceReload(void) {

	try {
		clearAll();    // clears previous data structures and visual components

		snapshot = valc::Load();  // if not successful, exception will get thrown
		dataManager->haveSnapshot = true;   // got to here, so we have a snapshot

		dataManager->makeDataModel(snapshot);
		guiManager->setUpGUIcomponents();  // components representing the data model
	}
	catch (const Exception & e) {
		logManager->logException(std::string("Caught an exception whilst attempting a ForceReload: ")
			+ AnsiString(e.Message.c_str()).c_str());
	}
	catch (const std::string & str) {
		logManager->logException((std::string("Caught an unspecified string exception whilst attempting a ForceReload: ")
			                      + str).c_str());
	}
	catch (...) {
		logManager->logException("Caught an unspecified kind of exception (was attempting a ForceReload).");
	}
}

//---------------------------------------------------------------------------

/** Carries out a "Force Reload" operation, in addition to setting some
  * status information and logging some timing information.
  */
void __fastcall TMainForm::ForceReloadButtonClick(TObject *Sender)
{
	System::TTime tBefore = System::TDateTime::CurrentTime();
	UnicodeString uBefore = tBefore.FormatString("hh:mm:ss.zzz");
	logManager->log("Initiating force reload at " + Utils::unicodestr2str(uBefore));

	StatusLabel->Text = "Reloading...";

	Application->ProcessMessages();

	forceReload();

	StatusLabel->Text = "Done.";

	// now to write something about the timings into the log
	System::TTime tAfter = System::TDateTime::CurrentTime();
	UnicodeString uAfter = tAfter.FormatString("hh:mm:ss.zzz");
	logManager->log("Time after ForceReload: " + Utils::unicodestr2str(uAfter));

	TDateTime t = tAfter - tBefore;
	UnicodeString u = t.FormatString("hh:mm:ss.zzz");
	logManager->log("Time to do a Force Reload: " + Utils::unicodestr2str(u));

	Application->ProcessMessages();
}


//---------------------------------------------------------------------------

/** Notes the new heights of the panels containing the worklist entries and
  * the test results. (This method gets invoked when the split pane
  * between the queued worklist entries and the test results gets moved.)
  *
  * @see FormResize
  */
void __fastcall TMainForm::MainSplitterMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, float X, float Y)
{
	// We need to keep track of current panel heights so if the user
	// resizes the window in future, the same ratio between panel heights
	// can be maintained.
	guiManager->recordPanelHeights();

}
//---------------------------------------------------------------------------

/** Adjusts the heights of the panels containing the worklist entries and
  * the test results, to approximately maintain the existing ratio between
  * these panel heights. (This method gets invoked when either the user has
  * dragged the edge of the form to resize it, or minimised or maximised it.)
  */
void __fastcall TMainForm::FormResize(TObject *Sender)
{
	guiManager->adjustPanelHeights();
}

//---------------------------------------------------------------------------


