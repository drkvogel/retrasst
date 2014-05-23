#include "API.h"
#include "LocalRunIterator.h"
#include "QCViewData.h"
#include <set>
#include <sstream>

namespace valcui
{

QCViewData::QCViewData( const valc::SnapshotPtr& snapshot, int worklistEntryID )
    :
    m_worklistEntryID( worklistEntryID )
{
    listLocalResultsForWorklistEntry( snapshot );

    std::vector<int> qcResultIDs;

    for( auto result = localResultBegin(); result != localResultEnd(); ++result )
    {
        listResultIDsOfControllingQCsFor( *result, qcResultIDs );
    }

    if ( ! qcResultIDs.empty() )
    {
        cacheSupplementaryWorklistEntriesFor( qcResultIDs, snapshot );
    }
}

const valc::WorklistEntry* QCViewData::getWorklistEntry() const
{
    return m_worklistEntry;
}

int QCViewData::getWorklistEntryID() const
{
    return m_worklistEntryID;
}

bool QCViewData::hasLocalResults() const
{
    return ! m_localResults.empty();
}


QCViewData::const_iterator QCViewData::localResultBegin() const
{
    return m_localResults.begin();
}

QCViewData::const_iterator QCViewData::localResultEnd() const
{
    return m_localResults.end();
}

const valc::TestResult* QCViewData::lookupQCResult( int resultID ) const
{
    const valc::TestResult* r{};

    const valc::WorklistEntry* worklistEntry = lookupQCWorklistEntry( resultID );

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

const valc::WorklistEntry* QCViewData::lookupQCWorklistEntry( int resultID ) const
{
	auto cacheEntry = m_supplement.find( resultID );

	return ( cacheEntry == m_supplement.end() ) ? nullptr : cacheEntry->second;
}

void QCViewData::cacheSupplementaryWorklistEntriesFor( const std::vector<int>& resultIDs, const valc::SnapshotPtr& snapshot )
{
    std::set<int> targetSet( resultIDs.begin(), resultIDs.end() );

    for ( LocalRunIterator localRun( snapshot->localBegin(), snapshot->localEnd() ), end; localRun != end; ++localRun )
    {
        valc::Range<valc::WorklistEntryIterator> worklistEntries = snapshot->getWorklistEntries( localRun->getSampleDescriptor() );

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
                    m_supplement.insert( std::make_pair(*iter,currentWorklistEntry) );
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

void QCViewData::listLocalResultsForWorklistEntry( const valc::SnapshotPtr& snapshot )
{
    for ( LocalRunIterator localRun( snapshot->localBegin(), snapshot->localEnd() ), end; localRun != end; ++localRun )
    {
        valc::Range<valc::WorklistEntryIterator> worklistEntries = snapshot->getWorklistEntries( localRun->getSampleDescriptor() );
        for ( valc::WorklistEntryIterator w = worklistEntries.first; w != worklistEntries.second; ++w )
        {
            if ( (*w)->getID() == m_worklistEntryID )
			{
				m_worklistEntry = *w;

				// Note that the same worklist entry may be listed more than once.
				// For example, it may be listed when it was run, and then listed later,
				// side-by-side with its rerun.

                valc::Range<valc::TestResultIterator> testResults = (*w)->getTestResults();
                for ( valc::TestResultIterator tri = testResults.first; tri != testResults.second; ++tri )
                {
                    const valc::TestResult* tr = *tri;
                    if ( snapshot->compareSampleRunIDs( localRun->getRunID(), tr->getSampleRunID() ) )
                    {
                        m_localResults.push_back( tr );
                    }
                }
                if ( m_localResults.size() )// the worklist entry has results which are local to 'localRun'.
                {
                    return;
                }
            }
        }
    }
}

void QCViewData::listResultIDsOf( const valc::QCControls& controls, std::vector<int>& out )
{
	for ( int i = 0; i < controls.size(); ++i )
	{
		const valc::QCControl& c = controls[i];
        out.push_back( c.resultID() );
    }
}

void QCViewData::listResultIDsOfControllingQCsFor( const valc::TestResult* result, std::vector<int>& out )
{
    valc::ControlStatus cs(result->getControlStatus());

    listResultIDsOf( cs.precedingQCs(), out );
    listResultIDsOf( cs.followingQCs(), out );
}

}

