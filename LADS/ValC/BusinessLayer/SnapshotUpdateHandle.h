#ifndef SNAPSHOTUPDATEHANDLEH
#define SNAPSHOTUPDATEHANDLEH

#include "API.h"

namespace valc
{

class AnalysisActivitySnapshotImpl;
class IDToken;
class WorklistEntry;

class SnapshotUpdateHandle
{
public:
    SnapshotUpdateHandle( AnalysisActivitySnapshotImpl* snapshot = 0 );
    SnapshotUpdateHandle( const SnapshotUpdateHandle& );
    SnapshotUpdateHandle& operator=( const SnapshotUpdateHandle& );
    operator bool() const { return m_snapshot; }
    void                 appendToQueue   ( const QueuedSample& qs );
    void                 closeOff        ( const IDToken& sampleRunID );
    int                  getGroupIDForSampleRun( const IDToken& sampleRunID ) const;
    const WorklistEntry* getWorklistEntry( int id ) const;
    void                 insertRerun     ( int existingWorklistID, int newWorklistID );
    void                 updateSampleRunIDValue( const IDToken& from, const std::string& to );

private:
    AnalysisActivitySnapshotImpl* m_snapshot;
};

}

#endif

