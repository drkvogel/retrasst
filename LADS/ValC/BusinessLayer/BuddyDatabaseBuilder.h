#ifndef BUDDYDATABASEBUILDERH
#define BUDDYDATABASEBUILDERH

#include "InclusionRule.h"
#include "SampleRuns.h"
#include <string>
#include <System.hpp>

namespace valc
{

class AnalysisActivity;
class BuddyDatabase;
class BuddyDatabaseEntryIndex;
class BuddySampleIDKeyedOnSampleRunID;
class Cursor;
class DBUpdateSchedule;
class Projects;
class ResultIndex;
class SampleRunIDResolutionService;
class TestResultImpl;

/*  Usage: 

    An instance is constructed and then its 'accept' method is invoked zero or more times.

    Each invocation of 'accept' should have the effect of:
        - adding an entry EITHER to the sampleRuns collection OR to the candidateSampleRuns collection.
        - (provisionally) adding a TestResult to the resultIndex (if the Cursor describes a result). Such TestResult instances
            are assigned a sample-descriptor which is composed as follows:
                - for QCs: barcode + '/' + machine_id
                - for Unknowns: alphaSampleID + '/' + projectID 

    Note that if 'accept' is invoked for a Cursor value that describes a sample-run for which an entry ALREADY EXISTS (whether in the 
    'sampleRuns' collection or the 'candidateSampleRuns' collection), then ANOTHER entry will still get added. 
    Therefore, following multiple invocations of 'accept', some deduplication of these collections will be required.
*/
class BuddyDatabaseBuilder
{
public:
    BuddyDatabaseBuilder( 
        const Projects*                     p,                      // input parameter. Used, but not changed.
        ResultIndex*                        r,                      // output parameter
        SampleRuns*                         sampleRuns,             // output parameter 
        SampleRuns*                         candidateSampleRuns,    // output parameter
        const SampleRunIDResolutionService* s,                      // input parameter. Used but not changed. 
        DBUpdateSchedule*                   dbUpdateSchedule,
        BuddySampleIDKeyedOnSampleRunID*    buddySampleIDKeyedOnSampleRunID,
        BuddyDatabaseEntryIndex*            buddyDatabaseEntryIndex,
        const std::string&                  inclusionRule
        );
    bool accept( Cursor* c );
private:
    char resActionFlag, srFAOLevelOne;
    TDateTime dateAnalysed, resDateAnalysed, resUpdateWhen, srCreatedWhen, srClosedWhen;
    int buddySampleID, machineID, resID, alphaSampleID, resTestID, resWorklistID, srSequencePosition, srID, srIsOpen;
    std::string barcode, databaseName, sampleDescriptor, resText, sampleRunID;
    float resValue;
    bool hasResult, hasSampleRun;
    TestResultImpl*                     result;
    ResultIndex*                        m_resultIndex;
    const Projects*                     m_projects;
    SampleRuns*                         m_sampleRuns;
    SampleRuns*                         m_candidateSampleRuns;
    const SampleRunIDResolutionService* m_sampleRunIDResolutionService;
    DBUpdateSchedule*                   m_dbUpdateSchedule;
    BuddySampleIDKeyedOnSampleRunID*    m_buddySampleIDKeyedOnSampleRunID;
    BuddyDatabaseEntryIndex*            m_buddyDatabaseEntryIndex;
    InclusionRule                       m_inclusionRule;

    BuddyDatabaseBuilder( const BuddyDatabaseBuilder& );
    BuddyDatabaseBuilder& operator=( const BuddyDatabaseBuilder& );

    bool isQC() const;
    void reset();
};

};

#endif

