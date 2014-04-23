//---------------------------------------------------------------------------
#include "Utils.h"

#pragma hdrstop
//---------------------------------------------------------------------------

#include <boost/variant.hpp>

#include "API.h"

#include "DataManager.h"

#include "VisualComponents.h"
#include "Main.h"
#include "ConsoleWriter.h"
#include "StrUtil.h"
#pragma package(smart_init)

//---------------------------------------------------------------------------

/** Initialises the data manager object, with fresh empty lists of queued
  * worklist entries and completed/pending test results.
  */
DataManager::DataManager(TMainForm *m)
	: mainForm(m),
	  log(m->logManager),
	  worklist(NULL),
	  resultslist(NULL),
	  haveSnapshot(false),
	  snapshotLoadedOk(false)
{
}

// Carefully returns to the heap any memory used by the given list.
// Note that this may get called when the list is not well formed
// (for example, when the snapshot only got partially loaded), so
// should be able to cope with being given a NULL list.
void DataManager::destroyData(SampleRunsList *srlist)
{
	if (srlist!=NULL) {
		std::list<DSampleRun *>::iterator iter = srlist->begin();
		while (iter!=srlist->end()) {
			delete *iter;
			iter++;
		}
	}
	delete srlist;
	srlist = NULL;
}


/** Clears away all the data structures used for the previous snapshot,
  * carefully so as to prevent memory leaks. Also records that no snapshot
  * is loaded.
  */
void DataManager::clearAll()
{
	snapshotLoadedOk = false;
	destroyData(worklist);
	destroyData(resultslist);
}

/** Clears away all heap memory owned by the data manager (for the queued
  * worklist entries and test results).
  */
DataManager::~DataManager()
{
	clearAll();
}


/** Comparison relation for sorting the worklist entries included in a
  * sample run. Used to sort the entries in order of test name.
  *
  * @param first   the first worklist entry
  * @param second  the second worklist entry
  * @return        true if the first test's name is alphabetically before that of the second
  */
bool DataManager::goCompare(const DSampleTest * first, const DSampleTest * second) {
	std::string firstname = first->testName;
	std::string secondname = second->testName;
	return (firstname.compare(secondname)<0);   // firstname < secondname
}




/** Adds the alert information to the given result of a test on a sample.
  * For example, QC alert information.
  *
  * @param st            the test result to which alert information should be added
  * @param resultId      the result id for this test result
  */
void DataManager::addAlerts(valc::SnapshotPtr snapshot,
							DSampleTest *st, int resultId)
{
	if (snapshot->hasRuleResults(resultId)) {

		// reviewing what happened when applying rules to this test result
		valc::RuleResults rr = snapshot->getRuleResults(resultId);

		// RuleResults has summary information
		//    int getSummaryResultCode()
		//    string getSummaryMsg()

		// each RuleResult has the following:
		//    int resultCode
		//    string rule
		//    string msg

		st->setAlertLevel(rr.getSummaryResultCode(),
		                 rr.getSummaryMsg());


		// going through all the rules applied to this result
		std::string rulesApplied;
		valc::RuleResults::const_iterator iterR = rr.begin();
		while (iterR!=rr.end()) {
			valc::RuleResult r = *iterR;
			if (!rulesApplied.empty()) {
				rulesApplied += "; ";
			}
			rulesApplied += r.rule;
			st->addRuleResult(r.msg,r.rule,r.resultCode);
			iterR++;
		}

		/*--- begin tmp stuff for fake alerts --*/
		if (resultId==1132152) {
			st->addRuleResult("message for alert (passed)","rule",
							  valc::ResultCode::RESULT_CODE_PASS);
		}
				/*
				if (st->resultId==1132172) {
					st->setAlertLevel(valc::ResultCode::RESULT_CODE_ERROR,
									  "pretend summary message");
					st->addRuleResult("pretend error message","Rule",
								 valc::ResultCode::RESULT_CODE_ERROR);
				}
				else if (st->resultId==1132173) {
					st->setAlertLevel(valc::ResultCode::RESULT_CODE_FAIL,
									  "pretend summary message");
					st->addRuleResult("fail alert message","rulename",
								 valc::ResultCode::RESULT_CODE_FAIL);
					st->addRuleResult("borderline alert","differentrule",
								 valc::ResultCode::RESULT_CODE_BORDERLINE);
				}
				/ *--- end tmp stuff for fake alerts --*/


	}  // end of if (snapshot->hasRuleResults(st->resultId))
	else {  // no rules were applied
		st->setAlertLevel(valc::ResultCode::RESULT_CODE_NO_RULES_APPLIED,"");
	}
}



/** Finds all the worklist entries associated with a particular sample,
  * and sets up some simple container data structures for these worklist entries.
  *
  * @param s                 the sample for which to retrieve the worklist entries
  * @param sn                provides access to the current snapshot
  * @param sampleDescriptor  the string that identifies the sample run to fetch worklist entries for (needs to be here, can't get it off the DSampleRun?!)
  * @param queued            will be true if this is a queued sample run
  */
void DataManager::findWorkListEntries(DSampleRun &s,
									  valc::SnapshotPtr sn,
									  std::string sampleDescriptor,
									  bool queued) {

	std::string barcode;      // to hold the barcode for this sample

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
			s.setBarcode(barcode);
		}

		if (queued) {
			DSampleTest *e = new DSampleTest(entry,testName,&s);
			s.testsOnSample.push_back(e); // add to list of worklist entries for s
		}

		else { // !queued

			// now to go through results from worklist entry, if any
			int countR = 0;  // counting how many test results there are
			valc::Range<valc::TestResultIterator> iterPair =  entry->getTestResults();
			valc::TestResultIterator iterR = iterPair.first;
			while (iterR!=iterPair.second) {
				const valc::TestResult *tr = *iterR;
				if (tr!=NULL) {

					// now to find out whether this is local or not
					bool local = sn->compareSampleRunIDs(tr->getSampleRunID(),
														 s.sampleRunId);

					// now to construct a DSampleTest for this result
					DSampleTest *er = new DSampleTest(entry,tr,local,testName,&s);
					addAlerts(sn,er,tr->getID());
					s.testsOnSample.push_back(er); // add to list of worklist entries for s

					countR++;
				}
				iterR++;
			}
			if (countR==0) {   // result could be pending, for example
				// set up a DSampleTest anyway
				DSampleTest *ep = new DSampleTest(entry,testName,&s);
				s.testsOnSample.push_back(ep); // add to list of worklist entries for s
			}
		}
		iter++;

	}

	// an alternative way of getting the barcode, if no worklist entries found
	if (count==0 && s.sampleRunId.length()>0) {
		valc::BuddyDatabaseEntries bdes = sn->listBuddyDatabaseEntriesFor(s.sampleRunId);
		if (bdes.size()>0) {
			barcode = bdes.at(0).barcode;
		}
	}

	// now that we've got all the entries, let's sort the list...
	s.testsOnSample.sort(goCompare);

}

/** Fetches all the completed/pending results from the current snapshot.
  * (These correspond to those visible in the upper portion of the screen.)
  *
  * @pre               resultslist is NULL
  *
  * @param snapshot    a pointer to the latest snapshot of the database
  * @return            true if all loaded succesfully
  */
bool DataManager::fetchResultsEntries(valc::SnapshotPtr snapshot) {

	resultslist = new SampleRunsList();
	try {
		valc::LocalEntryIterator ri = snapshot->localBegin();
		while (ri!=snapshot->localEnd()) {

			valc::LocalEntry ms = *ri;  // (this could be a sample or a delimiter)
			// note that the type LocalEntry is a typedef for
			// boost::variant<LocalRun,BatchDelimiter>

			if (ms.type()==typeid(valc::LocalRun)) {
									// "if local entry is a local run
									// (not a batch delimiter)"
				valc::LocalRun r = boost::get<valc::LocalRun>(ms);
				// first set up information about the sample itself
				std::string sd = r.getSampleDescriptor();
				// r.getRunID() returns a std::string
				DSampleRun *s = new DSampleRun(sd,r.getRunID());

				// add this sample into the data list, along with its worklist entries
				resultslist->push_back(s);
				findWorkListEntries(*resultslist->back(),snapshot,sd,false);
			}
			ri++;
		}
	}
	catch (const Exception & e) {
		log->logException(std::string("Caught an exception (was attempting to load the completed/pending results). More details:")
								+ AnsiString(e.Message.c_str()).c_str());
		return false;
	}
	catch (...) {
		log->logException("Caught an unspecified kind of exception (was attempting to load the completed/pending results).");
		return false;
	}
	return true;
}

/** Fetches the worklist entries for all the queued sample runs.
  * (These correspond to those visible in the lower portion of the screen.)
  *
  * @pre               worklist is NULL
  *
  * @param snapshot    a pointer to the latest snapshot of the database
  * @return            true if all loaded succesfully
  */
bool DataManager::fetchQueuedEntries(valc::SnapshotPtr snapshot)
{
	worklist = new SampleRunsList();
	try {
		valc::QueuedSampleIterator qi = snapshot->queueBegin();   // -> overloaded
		while (qi!=snapshot->queueEnd()) {

			valc::QueuedSample qs = *qi;  // getting the sample
			// first set up information about the sample itself
			std::string sd = qs.getSampleDescriptor();

			DSampleRun *s = new DSampleRun(sd);  // carefully put on heap

			// add this sample into the data list, along with its worklist entries
			worklist->push_back(s);   // pushing a pointer, note
			findWorkListEntries(*worklist->back(),snapshot,sd,true);

			qi++;
		}
	}
	catch (const Exception & e) {
		log->logException(std::string("Caught an exception (was attempting to load the queued worklist entries). More details:")
								+ AnsiString(e.Message.c_str()).c_str());
		return false;
	}
	catch (...) {
		log->logException("Caught an unspecified kind of exception (was attempting to load the queued worklist entries).");
		return false;
	}
	return true;
}

/** Loads the data for the queued worklist entries and the completed/pending
  * test results from the given snapshot. Assumes that the data has already
  * been cleared just previously.
  *
  * @pre   worklist and resultslist are both NULL
  *
  * @param sn    the pointer to the fresh snapshot from the database
  */
void DataManager::makeDataModel(valc::SnapshotPtr sn)
{
	bool qOk = fetchQueuedEntries(sn);
	bool rOk = fetchResultsEntries(sn);  // pending & completed results
	snapshotLoadedOk = qOk && rOk;
}

