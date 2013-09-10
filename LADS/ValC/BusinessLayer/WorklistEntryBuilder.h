#ifndef WORKLISTENTRYBUILDERH
#define WORKLISTENTRYBUILDERH

#include "InclusionRule.h"
#include <string>
#include <System.hpp>
#include "WorklistRelation.h"

namespace valc
{

class Cursor;
class ExceptionalDataHandler;
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

        /*  The WorklistEntry instances built by 'newInstance' encapsulate knowledge of their parent (if any) and of their chidren (if any).
            The 'newInstance' method obtains this knowledge from worklistRelationsDataSource.  */
        WorklistRelationsDataSource* worklistRelationsDataSource ,
        const std::string& inclusionRule,
        ExceptionalDataHandler* exceptionalDataHandler
        );
    bool accept( Cursor* worklistCursor );
private:
    WorklistEntries*                m_worklistEntries;
    ResultIndex*                    m_resultIndex;
    WorklistRelationsDataSource*    m_worklistRelationsDataSource;
    InclusionRule                   m_inclusionRule;
    ExceptionalDataHandler*         m_exceptionalDataHandler;
    int recordNo, machineID, testID, groupID, categoryID, sampleID, projectID, profileID, tsSequence, buddyResultID;
    std::string barcode, profileName, sampleDescriptor;
    TDateTime timeStamp;
    char status;
    float diluent;
    WorklistRelations worklistRelations;

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

