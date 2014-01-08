#ifndef RUNIDC14NH
#define RUNIDC14NH

#include "SampleRunIDResolutionService.h"

namespace valc
{


// C14n is short for Canonicalization
// For a given SampleRunID value, obtained from a TestResult (in API.cpp), RunIDC14n gives the canonical form.
// The canonical form of a RunID is defined as the form used by LocalRun::getRunID() (also in API.cpp).
class RunIDC14n
{
public:
    /*
    For RunIDC14n to be initialised correctly, it must be constructed using an instance of SampleRunIDResolutionService 
    which:
    a) already has mappings for all SampleRunID values that were fabricated to construct TestResult instance only for it 
        later to emerge that an existing SampleRunID value was appropriate.
    b) does not yet contain any mappings from temporary SampleRunID values assigned during snapshot load and persistent 
        SampleRunID values created subsequent to snapshot load.
    */
    RunIDC14n( const SampleRunIDResolutionService& s );
    std::string toCanonicalForm( const std::string& sampleRunID ) const;
private:
    const IDMap m_idMap;

    RunIDC14n( const RunIDC14n& );
    RunIDC14n& operator=( const RunIDC14n& );
};

}

#endif

