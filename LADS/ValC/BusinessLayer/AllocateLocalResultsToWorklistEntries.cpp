#include "AllocateLocalResultsToWorklistEntries.h"
#include "API.h"
#include <boost/foreach.hpp>
#include "DBUpdateSchedule.h"
#include "ExceptionalDataHandler.h"
#include "LoggingService.h"
#include "Require.h"
#include "ResultIndex.h"
#include "StrUtil.h"
#include "WorklistEntries.h"

namespace valc
{

AllocateLocalResultsToWorklistEntries::AllocateLocalResultsToWorklistEntries( 
    int localMachineID, const ClusterIDs* clusterIDs, paulst::LoggingService* log, 
    WorklistEntries* worklistEntries, ResultIndex* resultIndex, 
    DBUpdateSchedule* dbUpdateSchedule, ExceptionalDataHandler* exceptionalDataHandler  )
    :
    m_log( log ),
    m_worklistEntries( worklistEntries ),
    m_resultIndex( resultIndex ),
    m_result(0),
    m_clusterIDs( clusterIDs ),
    m_localMachineID( localMachineID ),
    m_dbUpdateSchedule( dbUpdateSchedule ),
    m_exceptionalDataHandler( exceptionalDataHandler )
{
}

void AllocateLocalResultsToWorklistEntries::doStuff()
{
    IntList unallocated;

    m_resultIndex->listUnallocatedResults( unallocated );

    LOG( paulst::format("Number of unallocated results: %d", unallocated.size() ) );


    BOOST_FOREACH( const int& id, unallocated )
    {
        m_result = m_resultIndex->findResult( id );

        require( m_result );

        if( m_result->getActionFlag() != '0'  )
        {
            LOG( paulst::format(
                "Not concerned with allocating result %d to a worklist entry because its action_flag is non-zero", id ) );
            continue;
        }

        m_matchingWorklistEntries.resize(0);

        require( m_result->getSampleDescriptor().size() );

        m_worklistEntries->forEach( *this, m_result->getSampleDescriptor() );

        if ( m_matchingWorklistEntries.size() == 1 )
        {
            const int worklistEntryID = m_matchingWorklistEntries.at(0);
            m_resultIndex->allocateResultToWorklistEntry( id, worklistEntryID );
            LOG( paulst::format("Allocated result %d to worklist entry %d", id, worklistEntryID ) );
            m_dbUpdateSchedule->scheduleUpdateLinkingResultToWorklistEntry( id, worklistEntryID );
        }
        else
        {
            const std::string problem = m_matchingWorklistEntries.size() == 0 ? "No matching worklist entries found." : 
                "More than one matching worklist entry found.";

            if ( m_exceptionalDataHandler )
            {
                m_exceptionalDataHandler->notifyCannotAllocateResultToWorklistEntry( id, problem );
            }
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

void AllocateLocalResultsToWorklistEntries::notifyCancelled()
{
}

int AllocateLocalResultsToWorklistEntries::releaseReturnValue()
{
    return 1;
}

}

