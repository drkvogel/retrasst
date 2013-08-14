#ifndef BUDDYDATABASEH
#define BUDDYDATABASEH

#include "API.h"
#include <boost/scoped_ptr.hpp>
#include <memory>
#include "SampleRuns.h"

namespace valc
{

class BuddyDatabaseEntryIndex;
class BuddySampleIDKeyedOnSampleRunID;

/*  An ordered sequence of SampleRun instances, representing a sequence of sample-run instances on a given machine.
*/
class BuddyDatabase
{
public:
    BuddyDatabase( 
        int                                     localMachineID, 
        const SampleRuns*                       sr, 
        const BuddySampleIDKeyedOnSampleRunID*  bsidKeyedOnRunID, 
        const BuddyDatabaseEntryIndex*          buddyDatabaseEntries );    

    typedef SampleRuns::const_iterator const_iterator;

    const_iterator          begin() const;
    const_iterator          end() const;
    int                     getLocalMachineID() const;
    BuddyDatabaseEntries    listBuddyDatabaseEntriesFor( const std::string& sampleRunID )   const;
private:
    boost::scoped_ptr<const SampleRuns>                      m_sampleRuns;
    std::auto_ptr<const BuddySampleIDKeyedOnSampleRunID> m_buddySampleIDKeyedOnSampleRunID;
    std::auto_ptr<const BuddyDatabaseEntryIndex>         m_buddyDatabaseEntryIndex;
    const int                                            m_localMachineID;

    BuddyDatabase( const BuddyDatabase& );
    BuddyDatabase& operator=( const BuddyDatabase& );
};

};

#endif

