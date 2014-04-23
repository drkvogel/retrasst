//---------------------------------------------------------------------------
#include "Utils.h"
#include "API.h"

#pragma hdrstop
//---------------------------------------------------------------------------

#include <algorithm>

#include "DataContainers.h"

#pragma package(smart_init)

//---------------------------------------------------------------------------
//   DSampleRun

/** Initialises a sample run. This constructor is used for active sample runs,
  * as not only is there a sample descriptor, but also a run id.
  *
  * @param sd       the string giving the sample descriptor for the sample run
  * @param r        a string giving the run id (supplied by the business layer)
  */
DSampleRun::DSampleRun(const std::string &sd, const std::string &r)
		: needsAttention(false),   // default is that this doesn't need attention
		  sampleRunId(r),   // id of the machine this sample run happens on
		  sampleDescriptor(sd)
{
	 // no need to initialise testsOnSample as it's empty by default
	 // no need to do barcode = ""; as this is the default string value
}

/** Initialises a sample run with just a sample descriptor.
  * This constructor is used for queued samples, as additional information
  * relating to test results within the sample run is not available yet.
  *
  * @param sd       the string giving the sample descriptor for the sample run
  */
DSampleRun::DSampleRun(const std::string &sd)     // used for queued samples
	: sampleDescriptor(sd)
{
}

/** A default constructor, which creates this sample run as an undescribed sample. */
DSampleRun::DSampleRun()     // default constructor
	: sampleDescriptor("[undescribed sample]")
{
}

/** Deletes the heap data that this sample run owns, namely the list of
  * the tests belonging to this sample run.
  */
DSampleRun::~DSampleRun()
{
	SampleTestsList::iterator iter = testsOnSample.begin();
	while (iter!=testsOnSample.end())  {
		delete *iter;
		iter++;
	}
}

/** Getter function for the barcode of this sample run.
  * Note that a barcode does not uniquely identify a sample run (as
  * barcodes can get used again, eventually).
  *
  * @return    the barcode for this sample run
  */
std::string DSampleRun::getBarcode() const {
	return barcode;
}

/** Setter function for the barcode of this sample run.
  * Note that a barcode does not uniquely identify a sample run (as
  * barcodes can get used again, eventually).
  *
  * @param b   the string to be set as the barcode
  */
void DSampleRun::setBarcode(std::string b) {
	barcode = b;
}

/** Getter function, reporting whether this sample run needs attention. This should be the
  * case if one or more of the test results within the sample run needs attention.
  *
  * @return    true when the user needs to look at this sample run
  */
bool DSampleRun::getAttentionNeed() const {
	return needsAttention;
}

/** Setter function for whether this sample run needs attention.
  *
  * @param	   a boolean to indicate whether the user needs to look at this sample run
  */
void DSampleRun::setAttentionNeed(bool a) {
	needsAttention = a;
}

/** Getter function that returns a pointer to the TBarcodePanel that visually
  * represents the barcode for this sample run.
  *
  * @return    a pointer to its barcode's visual representation
  */
TBarcodePanel* DSampleRun::getVisual() const {
	return visual;
}

/** Setter function that links this sample run with the GUI component that
  * visually represents its barcode.
  *
  * @param v   a pointer to a TBarcodePanel to represent this sample run's barcode
  */
void DSampleRun::setVisual(TBarcodePanel *v) {
    visual = v;
}

//---------------------------------------------------------------------------
//   DNote

/** Constructs a note with the given text. The boolean specifies whether this
  * is a note written by a human, or a machine.
  *
  * @param note         the textual contents of the note
  * @param hand         true if the note was authored by a human
  */
DNote::DNote(std::string note, bool hand)
	: text(note),
	  manual(hand)
{
}

/** Constructs a note with the given text.
  *
  * @param note         the textual contents of the note
  */
DNote::DNote(std::string note)
	: text(note)
{
}

/** Constructs an empty note. */
DNote::DNote()    // default constructor
{
}

/** Copies the given note.
  *
  * @param n       the note to make a copy of
  */
DNote::DNote(const DNote & n)
	:text(n.text),
	 manual(n.manual)
{
}



//---------------------------------------------------------------------------
//   DAlert

/** Creates an alert, using a text description, information about the rule
  * that was used to produce this alert, and a code for what kind of alert it is.
  *
  * @param note     the text for the alert
  * @param r        the string describing the rule that was applied
  * @param c        the code for this alert
  */
DAlert::DAlert(std::string note,std::string r, int c)
	: message(note),
	  rule(r),
	  code(c)
{
	isRule = true;
}

/** Creates an alert with an empty message. */
DAlert::DAlert()   // default constructor
{
	isRule = false;
}

/** Creates a copy of an alert.
  *
  * @param a       the alert to make a copy of
  */
DAlert::DAlert(const DAlert & a)
	:message(a.message),
	 rule(a.rule),
	 code(a.code),
	 isRule(a.isRule)
{
}

DAlert::~DAlert()
{
}

std::string DAlert::debugResponse(int response) {
	if (response==valc::ResultCode::RESULT_CODE_FAIL)
		return std::string("RESULT_CODE_FAIL");
	else if (response==valc::ResultCode::RESULT_CODE_PASS)
		return std::string("RESULT_CODE_PASS");
	else if (response==valc::ResultCode::RESULT_CODE_BORDERLINE)
		return std::string("RESULT_CODE_BORDERLINE");
	else if (response==-valc::ResultCode::RESULT_CODE_ERROR)
		return std::string("RESULT_CODE_ERROR");
	else if (response==-valc::ResultCode::RESULT_CODE_NO_RULES_APPLIED)
		return std::string("RESULT_CODE_NO_RULES_APPLIED");
	else
		return std::string("unknown result code");
}

/** Comparison relation for sorting the alerts, to put the more important ones first.
  *
  * @param first   the first alert
  * @param second  the second alert
  * @return        true if the first alert is more alerting than the second
  */
bool DAlert::goCompare(const DAlert & first, const DAlert & second)
{
	if (first.code==valc::ResultCode::RESULT_CODE_NO_RULES_APPLIED) {
		return false;
	}
	if (second.code==valc::ResultCode::RESULT_CODE_NO_RULES_APPLIED) {
		return true;
	}
	if (first.code==valc::ResultCode::RESULT_CODE_ERROR) {
		return true;
	}
	if (second.code==valc::ResultCode::RESULT_CODE_ERROR) {
		return false;
	}
	if (first.code==valc::ResultCode::RESULT_CODE_PASS) {
		return false;
	}
	if (second.code==valc::ResultCode::RESULT_CODE_PASS) {
		return true;
	}
	if (first.code==valc::ResultCode::RESULT_CODE_FAIL) {
		return true;
	}
	if (second.code==valc::ResultCode::RESULT_CODE_FAIL) {
		return false;
	}
	// must be both borderline...
	return true;  // (don't care!)
}

//---------------------------------------------------------------------------
//  DSampleTest

DSampleTest::~DSampleTest()
{
}

void DSampleTest::initialise(const valc::WorklistEntry *entry)
{ // gathers information from the WorklistEntry,
  // to initialise assorted variables

	status = entry->getStatus();
	entryDateTime = entry->getTimeStamp();

	// for now, these are just IDs; might be able to obtain names later?
	projectId = entry->getProjectID();
	machineId = entry->getMachineID();

	sampleId =  entry->getSampleID();
	profileId = entry->getProfileID();
	testId = entry->getTestID();
	worklistId = entry->getID();

	// set some defaults for the test result
	hasResult = false;
	resultSampleRunId = "";

	// setting the default alert level
	alertLevel = valc::ResultCode::RESULT_CODE_NO_RULES_APPLIED;

	needsAttention = false;   // the default

	hasManualNote = false;

	// some dummy notes for testing button icons
	if (worklistId==450114 || worklistId==450985 || worklistId==450947) {
		addNote("Here is an analyser note (placeholder)",false);
	}
	else if (worklistId==450108 || worklistId==450957) {
		addNote("A manual note (placeholder)",true);
	}
	if (worklistId==450114 || worklistId==450957) {
		addNote("A different manual note (placeholder)",true);
	}
	else if (worklistId==450985) {
		addNote("Another analyser note (placeholder)",false);
	}
}


/** Constructs an instance of a test that was carried out on a sample, from
  * information in a worklist entry supplied by the business layer.
  * Includes information pertaining to the result of the test, and
  * related information like date/timestamps.
  *
  * @param entry      supplies the worklist entry information from the business layer
  * @param tr         the result of the test on the sample
  * @param local      whether this test is local to this machine
  * @param name       the text translation of the test id of the given entry
  * @param s          the sample run to which this worklist entry belongs
  */
DSampleTest::DSampleTest(const valc::WorklistEntry *entry,
						 const valc::TestResult *tr,
						 bool local,
						 const std::string name,
						 DSampleRun *s)
	: testName(name),
	  displayTestName(" " + name + " "),
	  locallyPertinent(local),
	  sample(s)
{
	initialise(entry);
	if (tr!=NULL) {
		hasResult = true;
		resultValue = tr->getResultValue();
		resultId = tr->getID();
		resultDateTime = tr->getDateAnalysed();
		resultSampleRunId = tr->getSampleRunID();
	}
}


/** Constructs an instance of a test being carried out on a sample, from
  * information in a worklist entry supplied by the business layer.
  * Stores related information like date/timestamps.
  *
  * @param entry      supplies the worklist entry information from the business layer
  * @param name       the text translation of the test id of the given entry
  * @param s          the sample run to which this worklist entry belongs
  */
DSampleTest::DSampleTest(const valc::WorklistEntry *entry,
						 const std::string name,
						 DSampleRun *s)
	: testName(name),
	  displayTestName(" " + name + " "),
	  sample(s)
{
	initialise(entry);

	// set the default for whether this is a local test being carried out or not
	// (assume local for pending results, as no result information to confirm yet)
	locallyPertinent = true;

}

/** Returns true when the test has been completed for this sample and
  * a result exists.
  *
  * @return       true if a result exists for this test
  */
bool DSampleTest::resultExists() const
{
 	return hasResult;
}

/** Sets the alert level for this test result.
  *
  * @param a         the alert level to set this result to
  * @param summary   a textual description of the overall alert status
  */
void DSampleTest::setAlertLevel(int a, std::string summary)
{
	alertLevel = a;
	alertSummary = summary;
	if (a==valc::ResultCode::RESULT_CODE_FAIL
		|| a==valc::ResultCode::RESULT_CODE_BORDERLINE
		|| a==valc::ResultCode::RESULT_CODE_ERROR) {
		 setAttentionNeed(true);
	}
}

/** Returns the alert level (fail, pass, borderline, none applied etc.)
  * of this test result.
  *
  * @return      the alert level
  */
int DSampleTest::getAlertLevel() const
{
	return alertLevel;
}

/** Returns the summary of the alert information for the test on this sample.
  *
  * @return      the alert summary
  */
std::string DSampleTest::getAlertSummary()
{
	return alertSummary;
}


/** Attaches an alert to this test result.
  *
  * @param msg    the message for the alert
  * @param rule   which rule was used to evaluate the test result
  * @param code   the code for this test result outcome
  *               (e.g. valc::ResultCode::RESULT_CODE_FAIL)
  */
void DSampleTest::addRuleResult(std::string msg, std::string rule, int code)
{
	DAlert a(msg,rule,code);
	alerts.push_back(a);
	alerts.sort(DAlert::goCompare);
}

/** Describes whether the test result from this sample needs attention or not.
  *
  * @return     true if the user should look at this entry further
  */
bool DSampleTest::getAttentionNeed() const {
	return needsAttention;
}

/** Sets the test run on this sample to need attention according to the
  * given boolean. Also, if this sample needs attention, notifies the whole
  * sample run of the same.
  *
  * @param a       the boolean describing whether this needs attention or not
  */
void DSampleTest::setAttentionNeed(bool a) {
	needsAttention = a;
	if (a && sample!=NULL) {
		sample->setAttentionNeed(true);   // this needing attention means that
	}                                     // the whole sample run does too
}


/** Adds a note to this test result.
  *
  * @param msg         the contents of the note
  * @param hand        true if this was a note written by a human
  */
void DSampleTest::addNote(std::string msg, bool hand) {
	DNote n(msg,hand);
	hasManualNote = hasManualNote || hand;
	notes.push_back(n);
}

/** Will return the number of notes associated with this test result (once
  * this has been implemented properly). If no result, this will be 0.
  *
  * @return      the number of notes attached to this test result
  */
int DSampleTest::getNotesQuantity() const {
	return notes.size();
}

/** Returns a string representing the floating-point number that is the result
  * of this test, if there is a result yet. Otherwise, it just returns a
  * string to indicate a lack of result.
  *
  * @return     the result of the test on this sample, if completed
  */
std::string DSampleTest::getDefaultResult() const
{
	if (hasResult) {
		return getResult();
	}
	else {
		return "-";
	}
}

/** Returns the date portion of the timestamp from the worklist entry.
  * The timestamp might correspond to when the worklist entry was last updated.
  *
  * @return     the worklist entry date, in "YYYY-MM-DD (ddd)" format
  */
std::string DSampleTest::getEntryDate() const
{
	return Utils::unicodestr2str(entryDateTime.FormatString("YYYY-MM-DD (ddd)"));
}

/** Returns the time portion of the timestamp from the worklist entry.
  * The timestamp might correspond to when the worklist entry was last updated.
  *
  * @return     the worklist entry time, in "HH:MM" format.
  */
std::string DSampleTest::getEntryTime() const
{
	return Utils::unicodestr2str(entryDateTime.FormatString("HH:MM"));
															// ("HH:MM:SS" );
}


/** Returns the integer that is the id for the project this sample comes from.
  *
  * @return    the project id
  */
int DSampleTest::getProjectId() const
{
	return projectId;
}

/** Later, this will return the name of the project that this sample is on.
  * For now, it returns just the id.
  *
  * @return     the id of the project, as a string
  */
std::string DSampleTest::getProjectName() const
{
	return Utils::int2str(projectId);
}

/** Returns the integer that is the machine id (not sure which machine).
  *
  * @return    the machine id
  */
int DSampleTest::getMachineId() const
{
	return machineId;
}

/** Later, this will return the name of the machine corresponding to the
  * machine id. For now, it returns just the id.
  *
  * @return     the id of the project, as a string
  */
std::string DSampleTest::getMachineName() const
{
	return Utils::int2str(machineId);
}

/** Returns the integer that is the profile id (no idea what this is).
  *
  * @return    the profile id
  */
int DSampleTest::getProfileId()  const
{
	return profileId;
}

/** Returns the id of the sample being tested.
  *
  * @return    the sample id
  */
int DSampleTest::getSampleId() const
{
	return sampleId;
}

/** Returns the id corresponding to the test being carried out on this sample.
  *
  * @return    the test id
  */
int DSampleTest::getTestId() const
{
	return testId;
}

/** Returns the id of the worklist entry from where this test on a sample came.
  *
  * @return    the worklist entry id
  */
int DSampleTest::getWorklistId() const
{
	return worklistId;
}


/** Returns the result from this test, as a string.
  *
  * @return    the floating point number that is the test result
  */
std::string DSampleTest::getResult() const
{
// need to put an if in here, in case no result
 // if ... {
	 //	return "-";
	return Utils::float2str(resultValue);
}

/** Returns the id of the result of the test on this sample, if a result exists.
  * If there is no test result, no guarantee is made as to what number is returned.
  *
  * @return    the result id corresponding to the test result
  */
int DSampleTest::getResultId() const
{
	return resultId;
}

/** Returns the date portion of the timestamp of this test result,
  * if the result exists. Will be formatted in the "YYYY-MM-DD (ddd)" format.
  */
std::string DSampleTest::getResultDate() const
{
	return Utils::unicodestr2str(resultDateTime.FormatString("YYYY-MM-DD (ddd)"));
}

/** Returns the time portion of the timestamp of this test result,
  * if the result exists. Will be formatted in the "HH:MM" format.
  */
std::string DSampleTest::getResultTime() const
{
	return Utils::unicodestr2str(resultDateTime.FormatString("HH:MM"));
													  // ("HH:MM:SS" );
}


/** Returns the identifier of the sample run this test is from, if the
  * result exists.
  *
  * @return 	the sample run id for the test result
  */
std::string DSampleTest::getResultSampleRunId() const
{
   	return resultSampleRunId;
}




/** Returns a link to the visual component representing this test carried out on
  * this sample.
  *
  * @return       a link to the TTestPanel representing the test
  */
TTestPanel* DSampleTest::getVisual() const
{
	return visual;
}

/** Sets the visual component representing this test carried out on
  * this sample to the given pointer.
  *
  * @param v       the TTestPanel with which to associate this test on this sample
  */
void DSampleTest::setVisual(TTestPanel *v)
{
    visual = v;
}

//---------------------------------------------------------------------------

