#ifndef SAMPLERUNGROUPMODELH
#define SAMPLERUNGROUPMODELH

#include "CritSec.h"
#include <list>
#include <map>
#include "SampleRuns.h"
#include <string>

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
    ~SampleRunGroupModel();
    void assignToGroup( const std::string& runID, bool isQC, const paulst::Nullable<int>& groupID );
    int  countGroups() const;
    // Note that group ID values are not stable until all assignments have been made.
    int  getGroupID( const std::string& sampleRunID ) const;
    //  The SampleRunGroupModel destructor deletes idGenerator.
private:
    paulst::CritSec                          m_cs;
    SampleRunGroupIDGenerator*               m_groupIDGenerator;
    SampleRunGroup*                          m_currentGroup;
    std::map< std::string, SampleRunGroup* > m_mapRunIDToGroup;
    std::list< SampleRunGroup* >             m_sampleRunGroups;

    SampleRunGroupModel( const SampleRunGroupModel& );
    SampleRunGroupModel& operator=( const SampleRunGroupModel& );

    void startNewGroup( const std::string& runID, bool isQC, const paulst::Nullable<int>& groupID );
};

}

#endif

