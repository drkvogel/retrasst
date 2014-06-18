#ifndef BUDDYRUNH
#define BUDDYRUNH

#include "API.h"
#include <string>

namespace valc
{

struct BuddyRun
{
    int buddySampleID{};
    IDToken sampleRunID;

    BuddyRun( int bsid, const IDToken& runID );
    BuddyRun();
    BuddyRun( const BuddyRun& other );
    BuddyRun& operator=( const BuddyRun& other );
};

}

#endif

