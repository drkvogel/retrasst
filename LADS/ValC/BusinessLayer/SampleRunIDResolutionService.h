#ifndef SAMPLERUNIDRESOLUTIONSERVICEH
#define SAMPLERUNIDRESOLUTIONSERVICEH

#include "CritSec.h"
#include <map>
#include <string>

namespace valc
{

/*  
    Avoid sampleRunID values from becoming stale by maintaining a mapping to their updated values.

    Circumstances in which a sample run ID becomes stale:

    A new SampleRun instance is created but it is then discovered that there is an existing open 
    SampleRun that does the job, so the ID of the former should be mapped to the latter as the 
    latter is redundant and never created on the database.
*/
class SampleRunIDResolutionService
{
public:
    SampleRunIDResolutionService();
    void        addMapping( const std::string& fromCandidate, const std::string& toExisting );
    bool        compareSampleRunIDs( const std::string& oneRunID, const std::string& anotherRunID ) const;
    /*
        Returns true if 'forRunID' exists as a key or a value.
    */
    bool        existsAnAlternativeEquivalentFor( const std::string& runID ) const;
    /*
        Only call this method if existsAnAlternativeEquivalentFor returns true.

        If 'forRunID' exists as a key, then returns its value.  If 'forRunID' exists as a value, then returns its key.
    */
    std::string getAlternativeEquivalentFor( const std::string& runID ) const;
    std::string getMappingFor( const std::string& candidateNewSampleRunID, const std::string& ifNoMapping = "" ) const;
    bool        hasMappingFor( const std::string& candidateNewSampleRunID ) const;
private:
    paulst::CritSec m_cs;
    typedef std::map< std::string, std::string > IDMap;
    IDMap m_idMap;
    SampleRunIDResolutionService( const SampleRunIDResolutionService& );
    SampleRunIDResolutionService& operator=( const SampleRunIDResolutionService& );

    bool containsValue( const std::string& runID ) const;

};

};

#endif

