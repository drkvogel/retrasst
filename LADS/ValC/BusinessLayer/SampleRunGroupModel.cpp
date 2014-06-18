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

void SampleRunGroupModel::assignToGroup( const IDToken& runID, bool isQC, const paulst::Nullable<int>& groupID )
{
    require( -1 == findListPositionOfGroupIncluding( runID ) );

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
}

int  SampleRunGroupModel::countGroups() const
{
    paulst::AcquireCriticalSection a(m_cs);

    return m_sampleRunGroups.size();
}

int SampleRunGroupModel::findListPositionOfGroupIncluding( const IDToken& runID ) const
{
    for ( int listPosition = 0 ; listPosition < m_sampleRunGroups.size(); ++listPosition )
    {
        const SampleRunGroup* group = m_sampleRunGroups.at(listPosition);

        
        if ( group->includes( runID ) )
        {
            return listPosition;
        }
    }

    return -1;
}

int  SampleRunGroupModel::getGroupID( const IDToken& sampleRunID ) const
{
    paulst::AcquireCriticalSection a(m_cs);

    const int listPos = findListPositionOfGroupIncluding( sampleRunID );

    require( listPos != -1 );

    const SampleRunGroup* foundGroup = m_sampleRunGroups.at( listPos );
    
    require ( foundGroup );

    return foundGroup->getID();
}

void SampleRunGroupModel::listFollowingQCRuns( const IDToken& runID, IDTokenSequence& out ) const
{
    const int pos = findListPositionOfGroupIncluding( runID );

    require( pos != -1 );

    const SampleRunGroup* foundGroup = m_sampleRunGroups.at( pos );

    require( ! foundGroup->isQC() );

    listRunIDsForSampleRunGroupLocatedAt( pos + 1, out );
}

void SampleRunGroupModel::listPrecedingQCRuns( const IDToken& runID, IDTokenSequence& out ) const
{
    const int pos = findListPositionOfGroupIncluding( runID );

    require( pos != -1 );

    const SampleRunGroup* foundGroup = m_sampleRunGroups.at( pos );

    require( ! foundGroup->isQC() );

    listRunIDsForSampleRunGroupLocatedAt( pos - 1, out );
}

void SampleRunGroupModel::listRunIDsForSampleRunGroupLocatedAt( int position, IDTokenSequence& out ) const
{
    if ( position < 0 || position >= m_sampleRunGroups.size() )
    {
        return;
    }

    const SampleRunGroup* group = m_sampleRunGroups.at( position );

    group->listRunIDs( out );
}

void SampleRunGroupModel::startNewGroup( const IDToken& runID, bool isQC, const paulst::Nullable<int>& groupID )
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

