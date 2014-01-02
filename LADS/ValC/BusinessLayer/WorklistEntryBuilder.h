#ifndef WORKLISTENTRYBUILDERH
#define WORKLISTENTRYBUILDERH

#include "InclusionRule.h"
#include <string>
#include <System.hpp>

namespace paulstdb
{
    class Cursor;
}

namespace valc
{

class ExceptionalDataHandler;
class QCSampleDescriptorDerivationStrategy;
class ResultIndex;
class WorklistEntries;

/*  Builds new WorklistEntry instances from a Cursor.

    WorklistEntry instances are assigned a sample-descriptor. This is composed as follows:
        - for QCs:      barcode  + '/' + machineID
        = for Unknowns: sampleID + '/' + projectID
*/
class WorklistEntryBuilder
{
public:
    WorklistEntryBuilder( 
        WorklistEntries* worklistEntries,

        /*  If newInstance is invoked with a Cursor value that describes a worklist that explicitly names its result, then
            this information is passed to resultIndex (using the method: allocateResultToWorklistEntry). */
        ResultIndex* resultIndex, 
        const std::string& inclusionRule,
        ExceptionalDataHandler* exceptionalDataHandler,
        QCSampleDescriptorDerivationStrategy* qcsdds
        );
    bool accept( paulstdb::Cursor* worklistCursor );
private:
    WorklistEntries*                m_worklistEntries;
    ResultIndex*                    m_resultIndex;
    InclusionRule                   m_inclusionRule;
    ExceptionalDataHandler*         m_exceptionalDataHandler;
    QCSampleDescriptorDerivationStrategy* m_QCSampleDescriptorDerivationStrategy;
    int recordNo, machineID, testID, groupID, categoryID, sampleID, projectID, profileID, tsSequence, buddyResultID;
    std::string barcode, profileName, sampleDescriptor;
    TDateTime timeStamp;
    char status;
    float diluent;

    enum Cols { COL_WL_RECORD_NO,   COL_WL_MACHINE_CID,     COL_WL_BARCODE,     COL_WL_TEST_CID,    COL_WL_GROUP_ID,    COL_WL_CATEGORY_ID, 
                COL_WL_SAMPLE_ID,   COL_WL_PROJECT_CID,     COL_WL_PROFILE_ID,  COL_WL_PROFILE_NAME,COL_WL_TIME_STAMP,  COL_WL_TS_SEQUENCE, 
                COL_WL_STATUS,      COL_WL_DILUENT,         COL_WL_BUDDY_RESULT_ID };


    WorklistEntryBuilder( const WorklistEntryBuilder& );
    WorklistEntryBuilder& operator=( const WorklistEntryBuilder& );

    bool isQC() const;
    void reset();
};

};

#endif

