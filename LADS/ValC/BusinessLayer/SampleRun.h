#ifndef SAMPLERUNH
#define SAMPLERUNH

#include <string>
#include <System.hpp>

namespace valc
{

/*  Represents an instance of running a sample.

    Constructed by BuddyDatabaseBuilder.

    Typically a SampleRun instance might be expected for every row in buddy_database. However Comms-Analyser communications can sometimes
    result in there being more than one buddy_database entry for what is logically a single run of a sample on an analyser. So, 
    in some circumstances, there might be more than one row in buddy_database for a single instance of a sample-run.

    Ignoring the default constructor and the copy constructor, there are 2 principal constructors:

    1) One ( for which the first parameter is dbID) is for constructing sample run instances that are representative of 
        already-persisted instances, stored on the sample_run table. 
    2) The other is for constructing sample-run instances that are representative of sample-runs that are INFERRED (on 
        the basis of rows in buddy_database for which run_sample_id is zero) and WITHOUT any persistent representation in the sample_run table.
*/
class SampleRun
{
public:
    SampleRun();
    SampleRun( const SampleRun& );
    SampleRun( const std::string& sampleDescriptor, int buddySampleID );
    SampleRun( int dbID, const std::string& sampleDescriptor, bool isOpen, const TDateTime& created, const TDateTime& closed, 
        float sequencePosition );
    SampleRun& operator=( const SampleRun& o );
    std::string getDatabaseID() const;
    std::string getID() const;
    std::string getSampleDescriptor() const;
    float       getSequencePosition() const;
    bool        hasDatabaseID() const;
    bool        isOpen() const;
private:
    std::string   m_databaseID;
    std::string   m_id;
    std::string   m_sampleDescriptor;
    bool          m_isOpen;
    TDateTime     m_created;
    TDateTime     m_closed;
    float         m_sequencePosition;
};

};

#endif

