#ifndef SAMPLERUNIDRESOLUTIONSERVICEH
#define SAMPLERUNIDRESOLUTIONSERVICEH

#include <map>
#include <string>

namespace valc
{

/*  Certain SampleRunID instances are constructed with a reference to SampleIDResolutionService  in order that they may 
    bind lazily to their value. Refer to SampleRunID.h for more commentary on this.
*/
class SampleRunIDResolutionService
{
public:
    SampleRunIDResolutionService();
    void addMapping( const std::string& fromCandidate, const std::string& to );
    bool canResolve( const std::string& candidateID ) const;
    /*  Returns candidateID, unless a mapping has been added, in which case returns the mapped value. */
    std::string resolve( const std::string& candidateID ) const;
private:
    typedef std::map< std::string, std::string > IDMap;
    IDMap m_idMap;
    SampleRunIDResolutionService( const SampleRunIDResolutionService& );
    SampleRunIDResolutionService& operator=( const SampleRunIDResolutionService& );
};

};

#endif

