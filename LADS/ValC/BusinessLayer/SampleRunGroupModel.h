#ifndef SAMPLERUNGROUPMODELH
#define SAMPLERUNGROUPMODELH

#include "CritSec.h"
#include "IDTokenSequence.h"
#include "SampleRuns.h"
#include <string>
#include <vector>

namespace valc
{

class SampleRunGroupIDGenerator
{
public:
    SampleRunGroupIDGenerator();
    virtual ~SampleRunGroupIDGenerator();
    virtual int nextID() = 0;
private:
    SampleRunGroupIDGenerator( const SampleRunGroupIDGenerator& );
    SampleRunGroupIDGenerator& operator=( const SampleRunGroupIDGenerator& );
};

class SampleRunGroup;

class SampleRunGroupModel
{
public:

    SampleRunGroupModel( SampleRunGroupIDGenerator* idGenerator);
    //  The SampleRunGroupModel destructor deletes idGenerator.
    ~SampleRunGroupModel();
    void assignToGroup( const IDToken& runID, bool isQC, const paulst::Nullable<int>& groupID );
    int  countGroups() const;
    // Note that group ID values are not stable until all assignments have been made.
    int  getGroupID( const IDToken& sampleRunID ) const;
    void listFollowingQCRuns( const IDToken& runID, IDTokenSequence& out ) const;
    void listPrecedingQCRuns( const IDToken& runID, IDTokenSequence& out ) const;
private:
    paulst::CritSec                         m_cs;
    SampleRunGroupIDGenerator*              m_groupIDGenerator;
    SampleRunGroup*                         m_currentGroup;
    std::vector< SampleRunGroup* >          m_sampleRunGroups;

    SampleRunGroupModel( const SampleRunGroupModel& );
    SampleRunGroupModel& operator=( const SampleRunGroupModel& );

    int  findListPositionOfGroupIncluding( const IDToken& runID ) const;
    void listRunIDsForSampleRunGroupLocatedAt( int position, IDTokenSequence& out ) const;
    void startNewGroup( const IDToken& runID, bool isQC, const paulst::Nullable<int>& groupID );
};

}

#endif

