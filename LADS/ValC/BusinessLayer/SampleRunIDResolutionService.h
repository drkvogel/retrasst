#ifndef SAMPLERUNIDRESOLUTIONSERVICEH
#define SAMPLERUNIDRESOLUTIONSERVICEH

#include "CritSec.h"
#include <map>
#include <string>

namespace valc
{

typedef std::map< std::string, std::string > IDMap;

/*  
    Avoid sampleRunID values from becoming stale by maintaining a mapping to their updated values.

    Circumstances in which a sample run ID becomes stale:

    A new SampleRun instance is created but it is then discovered that there is an existing open 
    SampleRun that does the job, so the ID of the former should be mapped to the latter as the 
    latter is redundant and never created on the database.

    Thus, SampleRunIDResolutionService maps temporary run ID values to values that exist on 
    the database.

    Components that persist newly observed sample-run instances to the database are obliged to 
    inform SampleRunIDResolutionService, using 'addMapping', the first parameter being the 
    temporary run ID and the second parameter being the ID of the newly inserted row in the 
    buddy_sample_run database table.
*/
class SampleRunIDResolutionService
{
public:

    SampleRunIDResolutionService();
    void        addMapping( const std::string& fromCandidate, const std::string& toExisting );
    void        clear();
    std::string resolve( const std::string& token ) const;
private:
    paulst::CritSec m_cs;
    IDMap m_idMap;
    SampleRunIDResolutionService( const SampleRunIDResolutionService& );
    SampleRunIDResolutionService& operator=( const SampleRunIDResolutionService& );

};

};

#endif

