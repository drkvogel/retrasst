#include "LocalRunIterator.h"
#include <set>
#include "SnapshotUtil.h"

namespace valcui
{

const valc::WorklistEntry* findWorklistEntry( int worklistEntryID, valc::Range<valc::WorklistEntryIterator> range )
{
    const valc::WorklistEntry* found{};

    for ( auto worklistEntry = begin(range); !found && (worklistEntry != end(range)); ++worklistEntry )
    {
        if ( (*worklistEntry)->getID() ==  worklistEntryID )
        {
            found = *worklistEntry;
        }
    }

    return found;
}

const valc::WorklistEntry* findWorklistEntry( int worklistEntryID, valc::SnapshotPtr snapshot )
{
    const valc::WorklistEntry* found{};
    typedef std::string SampleDescriptor;
    std::set< SampleDescriptor > tried;

    for ( LocalRunIterator run( snapshot->localBegin(), snapshot->localEnd() ), eof; !found && (run != eof); ++run )
    {
        const valc::LocalRun& lr = *run;
        const SampleDescriptor sd(lr.getSampleDescriptor());

        if ( 0 == tried.count( sd ) )
        {
            tried.insert( sd );
            found = findWorklistEntry( worklistEntryID, snapshot->getWorklistEntries(sd) );
        }
    }

    for ( auto queued = snapshot->queueBegin(); ! found && ( queued != snapshot->queueEnd() ); ++queued )
    {
        const SampleDescriptor sd(queued->getSampleDescriptor());

        if ( 0 == tried.count( sd ) )
        {
            tried.insert( sd );
            found = findWorklistEntry( worklistEntryID, snapshot->getWorklistEntries(sd) );
        }
    }

    return found;
}

bool hasRerun( const valc::WorklistEntry* wle, valc::SnapshotPtr snapshot )
{
    valc::WorklistRelative wr = snapshot->viewRelatively( wle );

    if ( wr.hasChildren() )
    {
        auto children = wr.getChildren();

        for ( auto child : children )
        {
            if ( child.getRelation() == 'R' ) // rerun
            {
                return true;
            }
        }
    }

    return false;
}

bool isQC( const valc::WorklistEntry* wle )
{
    return 0 == wle->getBarcode().find( "QC" );
}

}

