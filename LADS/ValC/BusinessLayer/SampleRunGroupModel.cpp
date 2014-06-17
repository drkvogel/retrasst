#include "AcquireCriticalSection.h"
#include "ExceptionUtil.h"
#include "Require.h"
#include "SampleRunGroup.h"
#include "SampleRunGroupModel.h"
#include <sstream>

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
    for ( std::vector< SampleRunGroup* >::iterator i = m_sampleRunGroups.begin(); i != m_sampleRunGroups.end(); ++i )
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

int SampleRunGroupModel::findListPositionOfGroupIncluding( const std::string& runID ) const
{
    int listPosition = 0;

    std::map< std::string, SampleRunGroup* >::const_iterator i = m_mapRunIDToGroup.find( runID );

    if ( i == m_mapRunIDToGroup.end() )
    {
        paulst::exception( "Failed to map '%s' to any group", runID.c_str() );
    }

    const SampleRunGroup* matchedGroup = i->second;

    if ( matchedGroup->isQC() )
    {
        paulst::exception( "Run '%s' maps to a QC group, not an Unknown group", runID.c_str() );
    }

    const int targetGroupID = matchedGroup->getID();

    for ( std::vector< SampleRunGroup* >::const_iterator j = m_sampleRunGroups.begin(); j != m_sampleRunGroups.end(); ++j )
    {
        if ( (*j)->getID() == targetGroupID )
        {
            break;
        }
        else
        {
            ++listPosition;
        }
    }

    if ( listPosition >= m_sampleRunGroups.size() )
    {
        paulst::exception( "Failed to local SampleRunGroup %d in the sequence of SampleRunGroup instances", targetGroupID );
    }

    return listPosition;
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

void SampleRunGroupModel::listFollowingQCRuns( const std::string& runID, std::vector< std::string >& out ) const
{
    int pos = findListPositionOfGroupIncluding( runID );

    listRunIDsForSampleRunGroupLocatedAt( pos + 1, out );
}

void SampleRunGroupModel::listPrecedingQCRuns( const std::string& runID, std::vector< std::string >& out ) const
{
    int pos = findListPositionOfGroupIncluding( runID );

    listRunIDsForSampleRunGroupLocatedAt( pos - 1, out );
}

void SampleRunGroupModel::listRunIDsForSampleRunGroupLocatedAt( int position, std::vector< std::string >& out ) const
{
    if ( position < 0 || position >= m_sampleRunGroups.size() )
    {
        return;
    }

    const SampleRunGroup* group = m_sampleRunGroups.at( position );

    group->listRunIDs( out );
}

void SampleRunGroupModel::startNewGroup( const std::string& runID, bool isQC, const paulst::Nullable<int>& groupID )
{
    require( m_groupIDGenerator );

    if ( m_currentGroup && (!isQC) && ! m_currentGroup->isQC() )
    {
        std::ostringstream msg;
    
        msg << "Cannot transition from a group of Unknowns to another group of Unknowns."
            << "\nState:\n"
            << "Wanting to start a new group for run " << runID << ". This is a run of an Unknown, i.e. not a QC.";

        if ( groupID.isNull() )
        {
            msg << " Supplied a groupID value of NULL for this run.";
        }
        else
        {
            msg << " Supplied a groupID value of " << (int)groupID << " for this run.";
        }

        msg << "\nDetails for the current group, in which this run would have no part:\n"
            << "isQC: " << ( m_currentGroup->isQC() ? "true" : "false" ) << "\n"
            << "ID: " << m_currentGroup->getID() << "\n";

        throwException( msg.str() );
    }

    int grpID = groupID.isNull() ? m_groupIDGenerator->nextID() : (int)groupID;

    m_currentGroup = new SampleRunGroup( isQC, grpID, groupID.isNull() );

    m_sampleRunGroups.push_back( m_currentGroup );
}


}

