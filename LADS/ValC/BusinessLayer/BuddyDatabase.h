#ifndef BUDDYDATABASEH
#define BUDDYDATABASEH

#include <boost/scoped_ptr.hpp>
#include "SampleRunIDResolutionService.h"
#include "SampleRuns.h"

namespace valc
{

class SampleRunIDResolutionService;

/*  An ordered sequence of SampleRun instances, representing a sequence of sample-run instances on a given machine.
*/
class BuddyDatabase
{
public:
    BuddyDatabase( int localMachineID, const SampleRuns* sr, const SampleRunIDResolutionService* s );    
    typedef SampleRuns::const_iterator const_iterator;
    const_iterator begin() const;
    const_iterator end() const;
    int getLocalMachineID() const;
private:
    boost::scoped_ptr<const SampleRuns> m_sampleRuns;
    boost::scoped_ptr<const SampleRunIDResolutionService> m_sampleRunIDResolutionService;
    const int m_localMachineID;

    BuddyDatabase( const BuddyDatabase& );
    BuddyDatabase& operator=( const BuddyDatabase& );
};

};

#endif

