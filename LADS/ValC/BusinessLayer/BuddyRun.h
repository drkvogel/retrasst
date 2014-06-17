#ifndef BUDDYRUNH
#define BUDDYRUNH

#include <string>

namespace valc
{

struct BuddyRun
{
    int buddySampleID{};
    std::string sampleRunID;

    BuddyRun( int bsid, const std::string& runID );
    BuddyRun();
    BuddyRun( const BuddyRun& other );
    BuddyRun& operator=( const BuddyRun& other );
};


}

#endif

