#ifndef SAMPLERUNH
#define SAMPLERUNH

#include "Nullable.h"
#include <string>
#include <System.hpp>

namespace valc
{

/*  Represents an instance of running a sample.

    Constructed by BuddyDatabaseBuilder.

    Typically a SampleRun instance might be expected for every row in buddy_database. However Comms-Analyser communications can sometimes
    result in there being more than one buddy_database entry for what is logically a single run of a sample on an analyser. So, 
    in some circumstances, there might be more than one row in buddy_database for a single instance of a sample-run.
*/
class SampleRun
{
public:
    SampleRun();
    SampleRun( const SampleRun& );
	// Note that 'closed' will be garbage if isOpen.
    SampleRun( const std::string& runID, const std::string& sampleDescriptor, bool isOpen, const TDateTime& created, const TDateTime& closed, 
        float sequencePosition, const paulst::Nullable<int>& groupID, bool isQC );
    SampleRun&              operator=( const SampleRun& o );
    paulst::Nullable<int>   getGroupID() const;
    std::string             getID() const;
    std::string             getSampleDescriptor() const;
    float                   getSequencePosition() const;
    bool                    isOpen() const;
    bool                    isQC() const;
private:
    std::string             m_runID;
    std::string             m_sampleDescriptor;
    bool                    m_isOpen;
    TDateTime               m_created;
    TDateTime               m_closed;
    float                   m_sequencePosition;
    paulst::Nullable<int>   m_groupID;
    bool                    m_isQC;
};

};

#endif

