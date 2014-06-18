#include "AnalysisActivitySnapshotImpl.h"
#include "API.h"
#include "ExceptionUtil.h"
#include "Require.h"
#include "SampleRunIDResolutionService.h"
#include "SnapshotUpdateHandle.h"
#include "StrUtil.h"
#include "WorklistEntries.h"
#include "WorklistEntryImpl.h"
#include "WorklistLinks.h"

namespace valc
{

SnapshotUpdateHandle::SnapshotUpdateHandle( AnalysisActivitySnapshotImpl* snapshot )
    :
    m_snapshot( snapshot )
{
}

SnapshotUpdateHandle::SnapshotUpdateHandle( const SnapshotUpdateHandle& other )
    :
    m_snapshot( other.m_snapshot )
{
}

SnapshotUpdateHandle& SnapshotUpdateHandle::operator=( const SnapshotUpdateHandle& other )
{
    m_snapshot = other.m_snapshot;
    return *this;
}

void SnapshotUpdateHandle::appendToQueue( const std::string& sampleDescriptor )
{
    m_snapshot->m_queuedSamples.push_back( QueuedSample( sampleDescriptor ) );
}

void SnapshotUpdateHandle::closeOff( const IDToken& sampleRunID )
{
    m_snapshot->m_localRunImpl.closeOff( sampleRunID );
}

int SnapshotUpdateHandle::getGroupIDForSampleRun( const IDToken& runID ) const
{
    return m_snapshot->m_sampleRunGroupModel.getGroupID( runID );
}

const WorklistEntry* SnapshotUpdateHandle::getWorklistEntry( int id ) const
{
    return m_snapshot->m_worklistEntries->get( id );
}

void SnapshotUpdateHandle::insertRerun( int existingWorklistID, int newWorklistID )
{
    const WorklistEntry* existingOne = getWorklistEntry( existingWorklistID );
    
    require( existingOne );

    const WorklistEntry* newOne = new WorklistEntryImpl(
        newWorklistID,
        existingOne->getSampleDescriptor(),
        existingOne->getMachineID(),
        existingOne->getBarcode(),
        existingOne->getTestID(),
        existingOne->getGroupID(),
        existingOne->getCategoryID(),
        existingOne->getSampleID(),
        existingOne->getProjectID(),
        existingOne->getProfileID(),
        existingOne->getProfileName(),
        Now(),
        1, // tsSequence
        'S', // status
        0.0,//diluent
        0, // result ID
        m_snapshot->m_resultDirectory );
        
    m_snapshot->m_worklistEntries->add( newOne );

    m_snapshot->m_worklistLinks->addLink( existingWorklistID, newWorklistID, 'r' );
}

void SnapshotUpdateHandle::updateSampleRunIDValue( const IDToken& runID, const std::string& newValue )
{
    m_snapshot->m_sampleRunIDResolutionService->addMapping( runID.token(), newValue );
}

}

