#ifndef SAMPLERUNGROUPMODELH
#define SAMPLERUNGROUPMODELH

#include "CritSec.h"
#include <map>
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
    void assignToGroup( const std::string& runID, bool isQC, const paulst::Nullable<int>& groupID );
    int  countGroups() const;
    // Note that group ID values are not stable until all assignments have been made.
    int  getGroupID( const std::string& sampleRunID ) const;
    void listFollowingQCRuns( const std::string& runID, std::vector< std::string >& out ) const;
    void listPrecedingQCRuns( const std::string& runID, std::vector< std::string >& out ) const;
private:
    paulst::CritSec                          m_cs;
    SampleRunGroupIDGenerator*               m_groupIDGenerator;
    SampleRunGroup*                          m_currentGroup;
    std::map< std::string, SampleRunGroup* > m_mapRunIDToGroup;
    std::vector< SampleRunGroup* >           m_sampleRunGroups;

    SampleRunGroupModel( const SampleRunGroupModel& );
    SampleRunGroupModel& operator=( const SampleRunGroupModel& );

    int  findListPositionOfGroupIncluding( const std::string& runID ) const;
    void listRunIDsForSampleRunGroupLocatedAt( int position, std::vector< std::string >& out ) const;
    void startNewGroup( const std::string& runID, bool isQC, const paulst::Nullable<int>& groupID );
};

}

#endif

