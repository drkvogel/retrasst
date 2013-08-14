#ifndef BUDDYSAMPLEIDKEYEDONSAMPLERUNIDH
#define BUDDYSAMPLEIDKEYEDONSAMPLERUNIDH

#include "IntList.h"
#include <map>
#include <string>

namespace valc
{

class SampleRunIDResolutionService;

class BuddySampleIDKeyedOnSampleRunID
{
public:
    typedef std::pair< std::string, int > Entry;

    BuddySampleIDKeyedOnSampleRunID( const SampleRunIDResolutionService* s );
    /*
        Could be called multiple times for the same combination of sampleRunID/buddySampleID, in which case
        second and subsequent calls should be ignored.

        A return value of false indicates that an equivalent entry already exists.
    */
    bool addEntry( const std::string& sampleRunID, int buddySampleID );
    /*
        Writes ID values to buffer.  Updates bufferSize to reflect the number written.

        Caller should set bufferSize to describe the space available at buffer.
        Should there be more ID values than can fit, no IDs are written to buffer, but 
        bufferSize is updated to describe the size required.
    */
    void getBuddySampleIDsFor( const std::string& sampleRunID, int* buffer, int& bufferSize ) const;
private:
    const SampleRunIDResolutionService* m_sampleRunIDResolutionService;
    typedef std::multimap< std::string, int > RunBuddyMap;
    RunBuddyMap m_runBuddyMap;

    BuddySampleIDKeyedOnSampleRunID( const BuddySampleIDKeyedOnSampleRunID& );
    BuddySampleIDKeyedOnSampleRunID& operator=( const BuddySampleIDKeyedOnSampleRunID& );

    void findBuddySampleIDsFor( const std::string& sampleRunID, IntList& out ) const;
};

};

#endif

