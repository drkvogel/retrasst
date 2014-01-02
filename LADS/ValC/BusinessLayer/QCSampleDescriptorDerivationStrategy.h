#ifndef QCSAMPLEDESCRIPTORDERIVATIONSTRATEGYH
#define QCSAMPLEDESCRIPTORDERIVATIONSTRATEGYH

#include <string>

namespace valc
{

/*
    Encapsulates the construction of string values that serves as sample-descriptors for QCs.

    Implementations should return an emtpy string if they cannot return a sensible value.

    A sample-descriptor is an identifier.  Some examples of how sample-descriptors are used:

    1) 

    If 2 instances of WorklistEntry have the same 
    value for sample-descriptor then they are both considered as relating to the same sample.

    2)

    A LocalRun represents a run of a sample.  The latter is identified by a sample-descriptor.  All the WorklistEntry instances that 
    have the same sample-descriptor have an association with that LocalRun in so far as they are for the same sample.
*/
class QCSampleDescriptorDerivationStrategy
{
public:
    QCSampleDescriptorDerivationStrategy();

    virtual ~QCSampleDescriptorDerivationStrategy();

    virtual std::string deriveFromBuddyDatabaseEntry( 
        int                 buddySampleID, 
        const std::string&  barcode, 
        int                 machineID, 
        int                 resTestID,    // Might be zero if the buddy_database entry has no results associated with it.
        int                 resWorklistID // Can also be zero.
        ) = 0; 

    virtual std::string deriveFromWorklistEntry( 
        int                 worklistID, 
        const std::string&  barcode, 
        int                 machineID, 
        int                 testID, 
        int                 buddyResultID,
        char                status
        ) = 0;
};

}

#endif

