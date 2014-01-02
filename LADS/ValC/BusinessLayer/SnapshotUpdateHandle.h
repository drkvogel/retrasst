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
    void                 appendToQueue   ( const std::string& sampleDesriptor );
    void                 closeOff        ( const std::string& sampleRunID );
    int                  getDatabaseIDForSampleRun( const std::string& sampleRunID );
    const WorklistEntry* getWorklistEntry( int id );
    void                 insertRerun     ( int existingWorklistID, int newWorklistID );

private:
    AnalysisActivitySnapshotImpl* m_snapshot;
};

}

#endif

