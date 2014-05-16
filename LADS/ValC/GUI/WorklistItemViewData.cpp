#include "AcquireCriticalSection.h"
#include "LocalRunIterator.h"
#include "WorklistItemViewController.h"
#include "WorklistItemViewData.h"
#include <sstream>

namespace valcui
{


void WorklistItemViewData::Factory::operator()( WorklistItemViewData& out )
{
    out.worklistEntry = nullptr;
	out.worklistEntryID = worklistEntryID;

    out.worklistEntry = searchLocalRunSequenceForWorklistEntry( worklistEntryID );
}

const valc::WorklistEntry* WorklistItemViewData::Factory::searchLocalRunSequenceForWorklistEntry( 
    int worklistEntryID ) const
{
    for (   LocalRunIterator localRun( snapshotPtr->localBegin(), snapshotPtr->localEnd() ), end; 
            localRun != end; 
            ++localRun )
    {
        valc::Range<valc::WorklistEntryIterator> worklistEntries = 
            snapshotPtr->getWorklistEntries( localRun->getSampleDescriptor() );

        for ( valc::WorklistEntryIterator w = worklistEntries.first; w != worklistEntries.second; ++w )
        {
            if ( (*w)->getID() == worklistEntryID )
			{
				return *w;
            }
        }
    }
    return NULL;
}            


WorklistItemViewData::Factory::Order::Order( 
    valc::SnapshotPtr snapshotPtr, 
    int worklistEntryID, 
    WorklistItemViewController* c, 
    stef::ThreadPool* tp )
    :
    controller( c )
{
    WorklistItemViewData::Factory factory;
    factory.snapshotPtr = snapshotPtr;
    factory.worklistEntryID = worklistEntryID;

    paulst::AcquireCriticalSection a(m_critSec);
    {
        submission = new OrderSubmission( factory, tp, this );
    }
}

void WorklistItemViewData::Factory::Order::set( 
    bool cancelled, 
    const std::string& error, 
    const WorklistItemViewData& output )
{
    paulst::AcquireCriticalSection a(m_critSec);
    {
        controller->factoryCallback( cancelled, error, output );
    }

    delete submission;// submission owns 'this', so 'this' will also, indirectly, get deleted.
}

}

