#include "AnalysisActivitySnapshotImpl.h"
#include "SnapshotUpdateHandle.h"
#include "WorklistEntries.h"

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

const WorklistEntry* SnapshotUpdateHandle::getWorklistEntry( int id )
{
    return m_snapshot->m_worklistEntries->get( id );
}

void SnapshotUpdateHandle::insertRerun( int existingWorklistID, int newWorklistID )
{
}

}

