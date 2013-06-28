#ifndef SAMPLERUNIDH
#define SAMPLERUNIDH

#include <string>

namespace valc
{

class SampleRunIDResolutionService;

/*
Allows the possibility for lazy binding to the string value that describes a sample-run. 
So TestResultImpl instances (which require a run ID) can be constructed with a SampleRunID whose 
underlying value may change.
The specific scenario in which this has value is as follows:
As BuddyDatabaseBuilder processes saved records sequentially. For a given sample, it may first see a buddy_database entry 
for which no sample_run_id has been allocated. It will therefore construct something new that serves to describe the run with which this entry
should be associated.
However, it may subsequently be presented with another database entry for the same sample - and find that, in this case, there is already a sample_run_id and, furthermore, the sample-run instance described by this identifier represents a sample-run that remains open.
In this case, the construction of 'something new' for the earlier record is now seen as premature and unnecessary.  But all is well if, thanks to lazy-binding, the 'new' one binds to this, existing sample-run that remains open.
*/
class SampleRunID
{
public:
    SampleRunID();
    SampleRunID( const SampleRunID& );
    SampleRunID( int dbID ); // lazy binding not required
    SampleRunID( const std::string& candidateID, const SampleRunIDResolutionService* s ); // will bind lazily
    SampleRunID& operator=( const SampleRunID& );
    std::string toString() const;
private:
    std::string m_dbID;
    std::string m_candidateID;
    const SampleRunIDResolutionService* m_sampleRunIDResolutionService;

};

};

#endif

