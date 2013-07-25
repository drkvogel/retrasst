#include "AllocateLocalResultsToWorklistEntries.h"
#include "API.h"
#include <boost/foreach.hpp>
#include "LoggingService.h"
#include "Require.h"
#include "ResultIndex.h"
#include "WorklistEntries.h"

namespace valc
{

AllocateLocalResultsToWorklistEntries::AllocateLocalResultsToWorklistEntries( int localMachineID, const ClusterIDs* clusterIDs, paulst::LoggingService* log, 
    WorklistEntries* worklistEntries, ResultIndex* resultIndex, DBUpdateSchedule* dbUpdateSchedule  )
    :
    m_log( log ),
    m_worklistEntries( worklistEntries ),
    m_resultIndex( resultIndex ),
    m_result(0),
    m_clusterIDs( clusterIDs ),
    m_localMachineID( localMachineID ),
    m_dbUpdateSchedule( dbUpdateSchedule )
{
}

void AllocateLocalResultsToWorklistEntries::execute()
{
    IntList unallocated;

    m_resultIndex->listUnallocatedLocalResults( unallocated );

    LOG( std::string("Number of unallocated results: ") << unallocated.size() );


    BOOST_FOREACH( const int& id, unallocated )
    {
        m_result = m_resultIndex->findResult( id );

        require( m_result );

        require( ( m_result->getActionFlag() == '0' )  || ( m_result->getActionFlag() == 'X' ) );

        m_matchingWorklistEntries.resize(0);

        require( m_result->getSampleDescriptor().size() );

        m_worklistEntries->forEach( *this, m_result->getSampleDescriptor() );

        if ( m_matchingWorklistEntries.size() == 1 )
        {
            m_resultIndex->allocateResultToWorklistEntry( id, m_matchingWorklistEntries.at(0) );
            LOG( std::string("Allocated result ") << id << " to worklist entry " << m_matchingWorklistEntries.at(0) << "." );
        }
        else
        {
            std::string logMsg = std::string("Failed to allocate result ") << id << " to a single worklist entry. Found " 
                << m_matchingWorklistEntries.size() << " matching worklist entries. ";

            if ( m_matchingWorklistEntries.size() )
            {
                logMsg += "Obtained the following matches: ";

                BOOST_FOREACH( int worklistEntryID, m_matchingWorklistEntries )
                {
                    logMsg += ( std::string(" ") << worklistEntryID );
                }
            }

            LOG( logMsg );
        }
    }
}

void AllocateLocalResultsToWorklistEntries::execute( const WorklistEntry* wle )
{
    if  (   ( 0 == wle->getBuddyResultID() )
            &&
            ( wle->getTestID() == m_result->getTestID() ) 
            &&
            (   ( wle->getMachineID() == m_localMachineID ) || 
                ( m_clusterIDs->end() != m_clusterIDs->find( wle->getMachineID() ) ) 
            )
            &&
            isWorklistEntryStatusEligibleForAllocatingToResult( wle->getStatus() )
        )
    {
        m_matchingWorklistEntries.push_back( wle->getID() );
    }
    
}

int AllocateLocalResultsToWorklistEntries::releaseReturnValue()
{
    return 1;
}

}

