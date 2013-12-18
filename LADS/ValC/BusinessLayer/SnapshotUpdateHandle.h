#ifndef SNAPSHOTUPDATEHANDLEH
#define SNAPSHOTUPDATEHANDLEH

namespace valc
{

class AnalysisActivitySnapshotImpl;
class WorklistEntry;

class SnapshotUpdateHandle
{
public:
    SnapshotUpdateHandle( AnalysisActivitySnapshotImpl* snapshot = 0 );
    SnapshotUpdateHandle( const SnapshotUpdateHandle& );
    SnapshotUpdateHandle& operator=( const SnapshotUpdateHandle& );
    const WorklistEntry* getWorklistEntry( int id );
    void                 insertRerun     ( int existingWorklistID, int newWorklistID );

private:
    AnalysisActivitySnapshotImpl* m_snapshot;
};

}

#endif

