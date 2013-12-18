#ifndef WORKLISTENTRYIMPLH
#define WORKLISTENTRYIMPLH

#include "API.h"

namespace valc
{

class WorklistDirectory;
class ResultDirectory;

class WorklistEntryImpl : public WorklistEntry
{
public:
    WorklistEntryImpl(
        int                         recordNo,
        const std::string&          sampleDescriptor,
        int                         machineID,
        const std::string&          barcode,
        int                         testID,
        int                         groupID,
        int                         categoryID,
        int                         sampleID,
        int                         projectID,
        int                         profileID,
        const std::string&          profileName,
        TDateTime                   timeStamp,
        int                         tsSequence,
        char                        status,
        float                       diluent,
        int                         buddyResultID,
        const WorklistDirectory*    worklistDirectory,
        const ResultDirectory*      resultDirectory );

    ~WorklistEntryImpl();
    std::string                 getBarcode()    const;
    int                         getBuddyResultID() const;
    int                         getCategoryID() const;
    RelatedEntries              getChildren()   const;
    float                       getDiluent()    const;
    int                         getGroupID()    const;
    int                         getID()         const;
    IntList                     getIDsOfRelatedEntries() const;
    int                         getMachineID()  const;
    RelatedEntry                getParent()     const;
    int                         getProfileID()  const;
    int                         getProjectID()  const;
    Range<TestResultIterator>   getTestResults() const;
    std::string                 getSampleDescriptor() const;
    int                         getSampleID()   const;
    int                         getTestID()     const;
    TDateTime                   getTimeStamp()  const;
    int                         getTSSequence() const;
    char                        getStatus()     const;
    bool                        hasChildren()   const;
    bool                        hasParent()     const;

private:
    const int                 m_recordNo;
    const std::string         m_sampleDescriptor;
    const int                 m_machineID;
    const std::string         m_barcode;
    const int                 m_testID;
    const int                 m_groupID;
    const int                 m_categoryID;
    const int                 m_sampleID;
    const int                 m_projectID;
    const int                 m_profileID;
    const std::string         m_profileName;
    const TDateTime           m_timeStamp;
    const int                 m_tsSequence;
    const char                m_status;
    const float               m_diluent;
    const int                 m_buddyResultID;
    const WorklistDirectory*  m_worklistDirectory;
    const ResultDirectory*    m_resultDirectory;

};

};
#endif

