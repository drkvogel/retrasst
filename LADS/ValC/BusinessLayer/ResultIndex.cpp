#include "AcquireCriticalSection.h"
#include <algorithm>
#include "API.h"
#include <boost/bind.hpp>
#include <boost/functional.hpp>
#include <cstdio>
#include <iterator>
#include "LoggingService.h"
#include "Require.h"
#include "ResultIndex.h"
#include <set>
#include "StringBuilder.h"
#include "TestResultIteratorImpl.h"

namespace valc
{

ResultIndex::ResultIndex()
{
}

void ResultIndex::addIndexEntryForResult( const TestResult* r )
{
    paulst::AcquireCriticalSection a( m_criticalSection );

    {
        m_resultMap.insert( std::make_pair( r->getID(), r ) );
    }
}

bool sameResult( const ResultIDsKeyedOnWorklistID::value_type& t, int resultID )
{
    return t.second == resultID;
}

void ResultIndex::allocateResultToWorklistEntry( int resultID, int toWorklistID )
{
    paulst::AcquireCriticalSection a( m_criticalSection );

    {
        if ( m_mapWorklistIDToResultID.end() ==
                std::find_if( m_mapWorklistIDToResultID.begin(), m_mapWorklistIDToResultID.end(), boost::bind( sameResult, _1, resultID ) ) )
        {
            m_mapWorklistIDToResultID.insert( std::make_pair(toWorklistID, resultID) );
        }
    }
}

Range<TestResultIterator> ResultIndex::equal_range( int worklistID ) const
{
    paulst::AcquireCriticalSection a( m_criticalSection );

    {
        std::pair< ResultIDsKeyedOnWorklistID::const_iterator, ResultIDsKeyedOnWorklistID::const_iterator > range = 
            m_mapWorklistIDToResultID.equal_range( worklistID );

        return Range<TestResultIterator>(
            TestResultIterator( new TestResultIteratorImpl( range.first,  range.second, this ) ),
            TestResultIterator( new TestResultIteratorImpl( range.second, range.second, this ) )
            );
    }
}

bool resultNotLoaded( const ResultIDsKeyedOnWorklistID::value_type& t, const std::map< int, const TestResult*>& loaded )
{
    const int resultID = t.second;
    return 0 == loaded.count( resultID );
}

void ResultIndex::removeReferencesToResultsNotLoaded( paulst::LoggingService* log )
{
    paulst::AcquireCriticalSection a( m_criticalSection );

    {
        ResultIDsKeyedOnWorklistID::iterator i;
        char buffer[1024];

        while ( m_mapWorklistIDToResultID.end() != 
                ( i = std::find_if( 
                    m_mapWorklistIDToResultID.begin(), 
                    m_mapWorklistIDToResultID.end(), 
                    boost::bind( resultNotLoaded, _1, m_resultMap ) ) ) )
        {
            if ( log )
            {
                std::sprintf( buffer, "Removing worklist->result %d -> %d", i->first, i->second );
                log->log( std::string(buffer) );
            }
            m_mapWorklistIDToResultID.erase( i );
        }
    }
}

const TestResult* ResultIndex::findResult( int resultID ) const
{
    paulst::AcquireCriticalSection a( m_criticalSection );

    {
        ResultsKeyedOnID::const_iterator i = m_resultMap.find( resultID );

        if( i == m_resultMap.end() )
        {
            const std::string exceptionMsg = std::string("Failed to find result with ID of ") << resultID;
            throw Exception( UnicodeString( exceptionMsg.c_str() ) );
        }

        return i == m_resultMap.end() ? 0 : i->second;
    }
}

void copyKey( const ResultIndex::ResultsKeyedOnID::value_type& vt, std::set<int>* out )
{
    out->insert( vt.first );
}

void copyValue( const ResultIDsKeyedOnWorklistID::value_type& vt, std::set<int>* out )
{
    out->insert( vt.second );
}


void ResultIndex::listUnallocatedResults( IntList& unallocatedResultIDs ) const
{
    paulst::AcquireCriticalSection a( m_criticalSection );

    {
        // Set A: set of all local result IDs (sorted)
        std::set<int> setA;
        std::for_each( m_resultMap.begin(), m_resultMap.end(), boost::bind( copyKey, _1, &setA ) );
        
        // Set B: set of allocated result IDs (sorted )
        std::set<int> setB;
        std::for_each( m_mapWorklistIDToResultID.begin(), m_mapWorklistIDToResultID.end(), boost::bind( copyValue, _1, &setB ) );

        // Return set_difference( SetA.begin(), SetA.end(), SetB.begin(), SetB.end(), std::back_inserter( unallocatedResultIDs ) )
        std::set_difference( setA.begin(), setA.end(), setB.begin(), setB.end(), std::back_inserter( unallocatedResultIDs ) );
    }
}


}

