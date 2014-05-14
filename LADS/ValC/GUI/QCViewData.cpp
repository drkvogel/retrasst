#include "AcquireCriticalSection.h"
#include <boost/variant/get.hpp>
#include "LocalRunIterator.h"
#include "QCViewController.h"
#include "QCViewData.h"
#include <set>
#include <sstream>

namespace valcui
{

const valc::TestResult* QCViewData::lookupResult( int resultID ) const
{
    const valc::TestResult* r{};

    const valc::WorklistEntry* worklistEntry = lookupWorklistEntry( resultID );

    if ( worklistEntry )
    {
		valc::Range<valc::TestResultIterator> results = worklistEntry->getTestResults();

		for ( auto result = results.first; result != results.second; ++result )
		{
			if ( (*result)->getID() == resultID )
			{
				r = *result;
				break;
			}
		}
    }

    return r;
}

const valc::WorklistEntry* QCViewData::lookupWorklistEntry( int resultID ) const
{
	auto cacheEntry = supplement.find( resultID );

	return ( cacheEntry == supplement.end() ) ? nullptr : cacheEntry->second;
}

void QCViewData::Factory::operator()( QCViewData& out )
{
    out.worklistEntry = nullptr;
    out.localResults.clear();
	out.supplement.clear();
	out.worklistEntryID = worklistEntryID;

    listLocalResultsForWorklistEntry( out );

    std::vector<int> qcResultIDs;

    for( auto result = out.localResults.begin(); result != out.localResults.end(); ++result )
    {
        listResultIDsOfControllingQCsFor( *result, qcResultIDs );
    }

    if ( ! qcResultIDs.empty() )
    {
        findWorklistEntriesFor( qcResultIDs, out.supplement );
    }
}

void QCViewData::Factory::findWorklistEntriesFor( const std::vector<int>& resultIDs, SupplementaryQCData& out )
{
    std::set<int> targetSet( resultIDs.begin(), resultIDs.end() );

    for ( LocalRunIterator localRun( snapshotPtr->localBegin(), snapshotPtr->localEnd() ), end; localRun != end; ++localRun )
    {
        valc::Range<valc::WorklistEntryIterator> worklistEntries = snapshotPtr->getWorklistEntries( localRun->getSampleDescriptor() );

        for ( valc::WorklistEntryIterator w = worklistEntries.first; w != worklistEntries.second; ++w )
        {
            const valc::WorklistEntry* currentWorklistEntry = *w;
            valc::Range<valc::TestResultIterator> testResults = currentWorklistEntry->getTestResults();
            for ( valc::TestResultIterator tri = testResults.first; tri != testResults.second; ++tri )
            {
                const valc::TestResult* tr = *tri;
                auto iter = targetSet.find( tr->getID() );
                if ( iter != targetSet.end() )
                {
                    // Found a worklist entry mapping for the member of the target set referenced by 'iter'.
                    out.insert( std::make_pair(*iter,currentWorklistEntry) );
                    targetSet.erase( iter );
                    if ( targetSet.empty() )// all done?
                    {
                        return;
                    }
                }
            }
        }
    }
}

void QCViewData::Factory::listLocalResultsForWorklistEntry( QCViewData& out )
{
    for ( LocalRunIterator localRun( snapshotPtr->localBegin(), snapshotPtr->localEnd() ), end; localRun != end; ++localRun )
    {
        valc::Range<valc::WorklistEntryIterator> worklistEntries = snapshotPtr->getWorklistEntries( localRun->getSampleDescriptor() );
        for ( valc::WorklistEntryIterator w = worklistEntries.first; w != worklistEntries.second; ++w )
        {
            if ( (*w)->getID() == worklistEntryID )
			{
				out.worklistEntry = *w;

				// Note that the same worklist entry may be listed more than once.
				// For example, it may be listed when it was run, and then listed later,
				// side-by-side with its rerun.

                valc::Range<valc::TestResultIterator> testResults = (*w)->getTestResults();
                for ( valc::TestResultIterator tri = testResults.first; tri != testResults.second; ++tri )
                {
                    const valc::TestResult* tr = *tri;
                    if ( snapshotPtr->compareSampleRunIDs( localRun->getRunID(), tr->getSampleRunID() ) )
                    {
                        out.localResults.push_back( tr );
                    }
                }
                if ( out.localResults.size() )// the worklist entry has results which are local to 'localRun'.
                {
                    return;
                }
            }
        }
    }
}

void QCViewData::Factory::listResultIDsOf( const valc::QCControls& controls, std::vector<int>& out )
{
	for ( int i = 0; i < controls.size(); ++i )
	{
		const valc::QCControl& c = controls[i];
        out.push_back( c.resultID() );
    }
}

void QCViewData::Factory::listResultIDsOfControllingQCsFor( const valc::TestResult* result, std::vector<int>& out )
{
    valc::ControlStatus cs(result->getControlStatus());

    listResultIDsOf( cs.precedingQCs(), out );
    listResultIDsOf( cs.followingQCs(), out );
}

QCViewData::Factory::Order::Order( valc::SnapshotPtr snapshotPtr, int worklistEntryID, QCViewController* c, stef::ThreadPool* tp )
    :
    controller( c )
{
    QCViewData::Factory factory;
    factory.snapshotPtr = snapshotPtr;
    factory.worklistEntryID = worklistEntryID;

    paulst::AcquireCriticalSection a(m_critSec);
    {
        submission = new OrderSubmission( factory, tp, this );
    }
}

void QCViewData::Factory::Order::set( bool cancelled, const std::string& error, const QCViewData& output )
{
    paulst::AcquireCriticalSection a(m_critSec);
    {
        controller->factoryCallback( cancelled, error, output );
    }

    delete submission;// submission owns 'this', so 'this' will also, indirectly, get deleted.
}

}

