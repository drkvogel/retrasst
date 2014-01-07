#include "AcquireCriticalSection.h"
#include "ExceptionUtil.h"
#include "Require.h"
#include "SampleRunGroup.h"
#include "SampleRunGroupModel.h"

namespace valc
{

SampleRunGroupIDGenerator::SampleRunGroupIDGenerator()
{
}

SampleRunGroupIDGenerator::~SampleRunGroupIDGenerator()
{
}

SampleRunGroupModel::SampleRunGroupModel( SampleRunGroupIDGenerator* idGenerator )
    :
    m_groupIDGenerator(idGenerator),
    m_currentGroup(0)
{
}

SampleRunGroupModel::~SampleRunGroupModel()
{
    for ( std::list< SampleRunGroup* >::iterator i = m_sampleRunGroups.begin(); i != m_sampleRunGroups.end(); ++i )
    {
        delete *i;
    }

    delete m_groupIDGenerator;
}

void SampleRunGroupModel::assignToGroup( const std::string& runID, bool isQC, const paulst::Nullable<int>& groupID )
{
    if ( m_mapRunIDToGroup.end() != m_mapRunIDToGroup.find( runID ) )
    {
        paulst::exception( "Run '%s' already assigned to a group", runID.c_str() );
    }

    if (    ( ! m_currentGroup )               || // Should only evaluate to true the first time this method is called.
            ( m_currentGroup->isQC() != isQC ) || // Transition from QC to Unknown or vice-versa 
            ( !(groupID.isNull()) && ( ! m_currentGroup->canAcceptIDRevision((int)groupID) ) ) // Transition from one group ID to another
       )
    { 
        startNewGroup( runID, isQC, groupID );
    }

    m_currentGroup->push_back( runID );

    if ( ! (groupID.isNull()) && ( m_currentGroup->getID() != (int)groupID ) )
    {
        require( m_currentGroup->canAcceptIDRevision( (int)groupID ) );
        m_currentGroup->reviseGroupID( (int)groupID );
    }

    m_mapRunIDToGroup.insert( std::make_pair(runID, m_currentGroup) );
}

int  SampleRunGroupModel::countGroups() const
{
    paulst::AcquireCriticalSection a(m_cs);

    return m_sampleRunGroups.size();
}

int  SampleRunGroupModel::getGroupID( const std::string& sampleRunID ) const
{
    paulst::AcquireCriticalSection a(m_cs);

    std::map< std::string, SampleRunGroup* >::const_iterator i = m_mapRunIDToGroup.find( sampleRunID );
    
    if ( i == m_mapRunIDToGroup.end() )
    {
        paulst::exception( "Failed to find group for run %s", sampleRunID.c_str() );
    }

    const SampleRunGroup* group = i->second;

    return group->getID();
}

void SampleRunGroupModel::startNewGroup( const std::string& runID, bool isQC, const paulst::Nullable<int>& groupID )
{
    require( m_groupIDGenerator );

    if ( m_currentGroup && (!isQC) && ! m_currentGroup->isQC() )
    {
        paulst::exception( "Cannot transition from a group of Unknowns to another group of Unknowns (sample run: %s)", runID.c_str() );
    }

    int grpID = groupID.isNull() ? m_groupIDGenerator->nextID() : (int)groupID;

    m_currentGroup = new SampleRunGroup( isQC, grpID, groupID.isNull() );

    m_sampleRunGroups.push_back( m_currentGroup );
}


}

