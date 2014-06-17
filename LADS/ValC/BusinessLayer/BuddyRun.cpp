#include "BuddyRun.h"

namespace valc
{

BuddyRun::BuddyRun( int bsid, const std::string& runID )
    :
    buddySampleID( bsid ),
    sampleRunID( runID )
{
}

BuddyRun::BuddyRun()
{
}

BuddyRun::BuddyRun( const BuddyRun& other )
    :
    buddySampleID( other.buddySampleID ),
    sampleRunID( other.sampleRunID )
{
}

BuddyRun& BuddyRun::operator=( const BuddyRun& other )
{
    buddySampleID = other.buddySampleID;
    sampleRunID   = other.sampleRunID;
    return *this;
}

}

