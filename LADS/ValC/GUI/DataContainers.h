//---------------------------------------------------------------------------

#ifndef DataContainersH
#define DataContainersH

#include "API.h"

#include "Utils.h"

#include <list>

/* This contains assorted data structures for holding information
   about things such as worklist entries, sample runs, etc.

   Each structure corresponds to (links to) a visual component that
   represents it.

*/
//---------------------------------------------------------------------------

										// forward declarations
										class TBarcodePanel;
										class TTestPanel;
										class DSampleRun;


/** A small class representing a note. A note will be attached to a test result. */
class DNote {
public:

	/** The text contained in this note. Attached to a test result. */
	std::string text;

	/** Whether this note was created manually (as opposed to by a machine). */
	bool manual;

	// here will go more meta-data, like time/date, author, editable etc.

	DNote(std::string note, bool hand);
	DNote(std::string note);
	DNote();
	DNote(const DNote& a);
};

/** A small class representing an alert message. Currently, alert messages can
  * note the outcome of a rule applied to a test result. Such outcomes can
  * include passing (or not) QC/validation checks.
  */
class DAlert {
public:
	/** The text that describes this alert. */
	std::string message;

	/** A description of the rule that was applied to produce this alert. */
	std::string rule;

	/** The evaluation of this result (see valc::RuleResults for the codes) */
	int code;

	/** Records whether this alert concerns a rule that was applied. */
	bool isRule;

	// leaving room here for meta-data, like time/date, author etc.


	/** Constructs an alert with text as the given string. */
	DAlert(std::string note,std::string r, int c);

	/** Constructs an alert with empty text. */
	DAlert();

	/** Makes an alert that is a copy of the given alert. */
	DAlert(const DAlert& a);

	~DAlert();

	static std::string debugResponse(int response);

	static bool goCompare(const DAlert & first, const DAlert & second);
};


/** Represents a single instance of a test being carried out on a sample,
  * and may (or may not) include the result for the test, depending on whether
  * the test has been completed yet. A DSampleTest object could correspond
  * to a queued worklist entry, or could correspond to a pending test result,
  * or a completed test result.
  * Typically, a single DSampleTest object will correspond to a single
  * worklist entry (as produced by the business layer), but occasionally
  * a worklist entry will contain several instances of a test being carried
  * out on that sample, and that will result in several corresponding
  * DSampleTest objects.
  */
class DSampleTest {

	friend class TTestPanel;  // its visual representation

public:


	/** The name of the test for this entry. For example, "CHOL" or "APOA". */
	const std::string testName;

	/** A padded version of the name of the test for this entry, for
	  * display purposes. */
	const std::string displayTestName;


	/** Indicates the status of this worklist entry. Usually this will be
	  * 'Q' for queued, 'P' for pending, or 'C' for completed, but other
	  * statuses are possible.  	  */
	char status;



	/** Any alert information attached to this test result */
	std::list<DAlert> alerts;   // don't need a list of references here as
								// a DAlert is a simple data container

	/** Any notes attached to this test result */
	std::list<DNote> notes;     // don't need a list of references here as
								// a DNote is a simple data container


// For comments on methods, please see implementation.

	~DSampleTest();
	DSampleTest(const valc::WorklistEntry *entry,
				const std::string name,
				DSampleRun *s);
	DSampleTest(const valc::WorklistEntry *entry,
				const valc::TestResult *tr,
				bool local,
				const std::string name,
				DSampleRun *s);

	bool resultExists() const;


	void setAlertLevel(int a, std::string summary);
	int getAlertLevel() const;
	std::string getAlertSummary();
	void addRuleResult(std::string msg, std::string rule, int code);

	int getNotesQuantity() const;   // a getter; does not change object's state
	void addNote(std::string msg, bool hand);

	bool getAttentionNeed() const;
	void setAttentionNeed(bool a);


	std::string getDefaultResult() const;

	std::string getEntryTime() const;
	std::string getEntryDate() const;

	int getProjectId() const;
	std::string getProjectName() const;
	int getMachineId() const;
	std::string getMachineName() const;

	int getProfileId() const;
	int getSampleId() const;
	int getTestId() const;
	int getWorklistId() const;


	std::string getResult() const;
	int getResultId() const;
	std::string getResultDate() const;
	std::string getResultTime() const;
	std::string getResultSampleRunId() const;

	TTestPanel* getVisual() const;
	void setVisual(TTestPanel *v);


private:


    bool hasManualNote;      // true if at least one manual note for this test result

	/** The result id for this test, as an integer. See also the resultIDString
	  * member variable for this as a string.  	  */
	int resultId;



	/** The sample that the test is run on. */
	DSampleRun* sample;


	int machineId; // id corresponding to the machine
	int projectId; // id corresponding to the project from which this sample comes
	int profileId; // don't know what the profile id is
	int sampleId;  // presumably an id number for the sample being tested
	int testId;    // presumably an id number for the test being run on this sample
	int worklistId;   // presumably an id number for the worklist entry that this test instance came from
	TDateTime entryDateTime;    // the timestamp on the worklist entry, possibly meaning "last udpated"

	bool needsAttention;     // e.g. true if this test result fails a QC rule

	/** This gets set to true iff this worklist entry is of significance
	  * to the local machine, i.e. the test result was obtained locally,
	  * or is expected to be. */
	bool locallyPertinent;


	int alertLevel;
	std::string alertSummary;

	bool hasResult;  // true once the test has been completed and a result exists
	float resultValue;    // the result from this test on this sample, if the test has been completed
	TDateTime resultDateTime;   // the time the test result was produced
	std::string resultSampleRunId; // the id of the sample run this is from

	TTestPanel *visual;  // the visual component that represents this test on the sample
	void initialise(const valc::WorklistEntry *entry); //common code for initialising things


};

typedef std::list<DSampleTest *> SampleTestsList;

typedef SampleTestsList::const_iterator DSampleTestIterator;
typedef SampleTestsList::const_reverse_iterator DSampleTestReverseIterator;



/** DSampleRun represents a sample run, along with worklist entries for tests
  * on that sample.  It could be a queued sample (in the bottom half of the screen),
  * or it could be a sample with some pending/completed tests (top half).
  */
class DSampleRun {
private:
	TBarcodePanel *visual;  // the visual component that represents this sample
	std::string barcode;    // the barcode for this sample run
	bool needsAttention;    // true iff >=1 worklist entry needs attention

public:

	/** Tests scheduled for this sample, i.e. the tests on this sample run.
	  * The tests could be queued (yet to happen), completed (with a result),
	  * or pending (sent to analyser but no result yet).
	  */
	SampleTestsList testsOnSample;

	/** The sample run id for this sample. Originates from getRunID()
	  * of valc::LocalRun */
	const std::string sampleRunId;

	/** A unique identifier for this sample run.  Originates from
	  * LocalRun::getSampleDescriptor(). Note that a barcode is not quite a
	  * unique identifier, because (eventually) barcodes can be recycled.  */
	const std::string sampleDescriptor;

	DSampleRun(const std::string &sd, const std::string &r);
	DSampleRun(const std::string &sd);
	DSampleRun();
	~DSampleRun();


	std::string getBarcode() const;
	void setBarcode(std::string b);

	bool getAttentionNeed() const;
	void setAttentionNeed(bool a);

	TBarcodePanel* getVisual() const;
	void setVisual(TBarcodePanel *v);
};

typedef std::list<DSampleRun *> SampleRunsList;

typedef SampleRunsList::const_iterator DSampleRunIterator;
typedef SampleRunsList::const_reverse_iterator DSampleRunReverseIterator;


#endif
