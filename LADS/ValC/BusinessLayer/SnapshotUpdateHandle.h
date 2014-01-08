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
    operator bool() const { return m_snapshot; }
    void                 appendToQueue   ( const std::string& sampleDesriptor );
    void                 closeOff        ( const std::string& sampleRunID );
    int                  getDatabaseIDForSampleRun( const std::string& sampleRunID ) const;
    int                  getGroupIDForSampleRun( const std::string& candidateSampleRunID ) const;
    const WorklistEntry* getWorklistEntry( int id ) const;
    void                 insertRerun     ( int existingWorklistID, int newWorklistID );
    bool                 knownDatabaseIDForCandidateSampleRun( const std::string& candidateSampleRunID ) const;
    void                 updateWithDatabaseIDForSampleRun( const std::string& candidateSampleRunID, int dbID );

private:
    AnalysisActivitySnapshotImpl* m_snapshot;
};

}

#endif

