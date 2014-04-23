//---------------------------------------------------------------------------

#ifndef DataManagerH
#define DataManagerH
//---------------------------------------------------------------------------

#include <list>
#include "DataContainers.h"

#include "API.h"


							// forward declarations
							class TMainForm;
							class DSampleRun;
							class DSampleTest;
							class GUImanager;
							class LogManager;
							typedef SampleRunsList; // see DataContainers.h

/** @file docs-DataManager.h
  * This file contains an overview of how the data is managed in ValC.
  * The singleton DataManager object has the job of managing the data
  * extracted from the database, including loading from the snapshot
  * provided by the business layer, and clearing data afterwards.
  */

class DataManager {

public:

	/** Used by the main form to record whether the business layer currently
	  * has a snapshot loaded.
	  */
	bool haveSnapshot;

    /** A link to the log manager, useful for writing log messages. */
	LogManager *log;

	/** Stores the queued sample runs for the current snapshot.
	  * The data manager (a singleton instance of this class)
	  * owns this list and has responsibility for destroying all
	  * the data it refers to (not GUI components, just the data parts).
	  *
	  * It is a list of references to DSampleRun objects (as opposed to
	  * a list of DSampleRun objects, so that copy constructors working for
	  * the list class can't mess up the references, leaving ownership
	  * of the full data hierarchy in this class.
	  */
	SampleRunsList *worklist;

	/** Stores the pending/completed sample runs in the current snapshot.
	  * This is owned by the data manager (see the worklist members for
	  * further information concerning ownership and datatype).
	  */
	SampleRunsList *resultslist;

// For comments on methods, please see their implementations.


	DataManager(TMainForm *m);
	~DataManager();

	void clearAll();
	void makeDataModel(valc::SnapshotPtr sn);


private:
	TMainForm *mainForm;  // link back to the main form (main program)
						  // not that it is currently used

	bool snapshotLoadedOk;  // records whether the data was loaded from the
							// snapshot successfully.
	// Note that if the snapshot got partially loaded, then worklist and
	// resultslist may have some stuff that they point to, so you can't just
	// look to see whether they are NULL or not to figure out whether the
	// snapshot was loaded successfully or not.


// For comments on methods, please see their implementations.

	void destroyData(SampleRunsList *srlist);

	bool goCompare(const DSampleTest *first, const DSampleTest *second);
	void orderedInsert(DSampleTest *e, DSampleRun *d);
	void addAlerts(valc::SnapshotPtr snapshot,
				   DSampleTest *st, int resultId);
	void findWorkListEntries(DSampleRun &s, valc::SnapshotPtr sn,
	                         std::string sampleDescriptor, bool queued);
	bool fetchQueuedEntries(valc::SnapshotPtr snapshot);
	bool fetchResultsEntries(valc::SnapshotPtr snapshot);


};


#endif
